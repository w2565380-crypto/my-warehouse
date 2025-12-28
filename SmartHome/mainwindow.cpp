#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QSqlQuery>
#include "datacollector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 初始化模型
    m_model = new QSqlTableModel(this);

    // 2. 关联数据库表名（必须和你 Navicat 里建的表名一模一样）
    m_model->setTable("devices");

    // 3. 设置编辑策略：点击单元格修改后立即同步到数据库
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    // 4. 从数据库拉取数据
    if (!m_model->select()) {
        QMessageBox::critical(this, "错误", "无法读取数据库表格: " + m_model->lastError().text());
    }

    // 5. 设置漂亮的中文表头（对应报告 2.4 节设计说明）
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "设备名称");
    m_model->setHeaderData(2, Qt::Horizontal, "类型");
    m_model->setHeaderData(3, Qt::Horizontal, "状态 (0/1)");
    m_model->setHeaderData(4, Qt::Horizontal, "房间");

    // 6. 核心步骤：将数据模型绑定到 UI 控件上
    ui->deviceTableView->setModel(m_model);

    // 让表格自动拉伸填充窗口
    ui->deviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);




    // 在 MainWindow 构造函数内添加：
    QThread* thread = new QThread(this);
    DataCollector* worker = new DataCollector();
    worker->moveToThread(thread); // 将工人移动到子线程

    QTimer* timer = new QTimer();
    timer->setInterval(3000); // 3秒采集一次
    timer->moveToThread(thread); // 定时器也跑在子线程，不干扰UI

    // 逻辑链：线程启动 -> 定时器开始 -> 定时器超时 -> 执行采集
    connect(thread, &QThread::started, timer, QOverload<>::of(&QTimer::start));
    connect(timer, &QTimer::timeout, worker, &DataCollector::collectData);

    // 资源管理：线程结束时清理内存
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, timer, &QObject::deleteLater);
    // mainwindow.cpp 构造函数中，在 thread->start() 之前添加：

    connect(worker, &DataCollector::dataUpdated, this, [=](double temp, double humi){
        QSqlQuery query;

        // --- 温度联动：空调 ---
        if (temp > 28.0) {
            query.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
            qDebug() << "检测到高温 (" << temp << "℃)，已自动开启空调！";
        } else if (temp < 22.0) {
            query.exec("UPDATE devices SET status = 0 WHERE name = '空调'");
            qDebug() << "环境凉爽 (" << temp << "℃)，已自动关闭空调。";
        }

        // --- 湿度联动：除湿器 (新增) ---
        if (humi > 80.0) {
            // 湿度高于 80%，开启除湿器
            query.exec("UPDATE devices SET status = 1 WHERE name = '除湿器'");
            qDebug() << "湿度过高 (" << humi << "%)，已自动开启除湿器！";
        } else if (humi < 40.0) {
            // 湿度低于 40%，关闭除湿器
            query.exec("UPDATE devices SET status = 0 WHERE name = '除湿器'");
            qDebug() << "湿度正常 (" << humi << "%)，已自动关闭除湿器。";
        }

        // 最后统一刷新界面模型，显示最新状态
        m_model->select();
    });



    thread->start(); // 正式开启

    // 当主窗口销毁时，让线程停止并安全退出
    connect(this, &MainWindow::destroyed, thread, [=]() {
        thread->quit();
        thread->wait(); // 等待子线程完全停止
        thread->deleteLater();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}






