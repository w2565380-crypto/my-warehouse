#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QSqlQuery>
#include "datacollector.h"

MainWindow::MainWindow(QWidget *parent,SceneMode mode)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentMode(mode) // 初始化成员变量
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


    // 2. 根据不同模式执行初始动作
    QSqlQuery query;
    if (m_currentMode == HomeMode) {
        // 回家模式：开灯 + 开启自动感应
        query.exec("UPDATE devices SET status = 1 WHERE name = '灯'");
        qDebug() << "场景切换：回家模式，欢迎回来，灯已开启";
    }
    else if (m_currentMode == AwayMode) {
        // 离家模式：全关（包括灯、空调、除湿器）
        query.exec("UPDATE devices SET status = 0");
        qDebug() << "场景切换：离家模式，全屋设备已关闭";
    }
    else if (m_currentMode == SleepMode) {
        // 睡眠模式：关灯 + 开启空调恒温
        query.exec("UPDATE devices SET status = 0 WHERE name = '灯'");
        query.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
        qDebug() << "场景切换：睡眠模式，灯已熄灭，空调运行中";
    }

    // 别忘了刷新模型，让 UI 表格立即更新状态
    if(m_model) m_model->select();




    // 3.多线程日志
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

        // 只有【回家模式】下，才允许环境数据自动控制开关
        if (m_currentMode == HomeMode) {
            QSqlQuery query;
            // 温度控制空调
            if (temp > 28.0) query.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
            else if (temp < 22.0) query.exec("UPDATE devices SET status = 0 WHERE name = '空调'");

            // 湿度控制除湿器
            if (humi > 80.0) query.exec("UPDATE devices SET status = 1 WHERE name = '除湿器'");
            else if (humi < 40.0) query.exec("UPDATE devices SET status = 0 WHERE name = '除湿器'");

            m_model->select();
        }
        // 如果是睡眠模式，我们可以保持空调开启，不做变动
        else if (m_currentMode == SleepMode) {
            // 睡眠模式下逻辑... (例如：保持静默或执行特定温度微调)
        }

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






