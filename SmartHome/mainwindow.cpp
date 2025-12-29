#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QSqlQuery>
#include "datacollector.h"
#include <QDateTime>

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



    // --3. 根据不同模式执行初始动作
    QSqlQuery query;
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString modeName;

    if (m_currentMode == HomeMode) {
        query.exec("UPDATE devices SET status = 1 WHERE name = '灯'");
        modeName = "【回家模式】：欢迎回来，系统已为您开启照明";
    }
    else if (m_currentMode == AwayMode) {
        query.exec("UPDATE devices SET status = 0");
        modeName = "【离家模式】：安全防护中，全屋电器已一键关闭";
    }
    else if (m_currentMode == SleepMode) {
        query.exec("UPDATE devices SET status = 0 WHERE name = '灯'");
        query.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
        modeName = "【睡眠模式】：灯光已熄灭，空调进入恒温运行";
    }

    // 记录模式切换日志
    QListWidgetItem *modeItem = new QListWidgetItem(QString("[%1] 系统状态：%2").arg(timeStr).arg(modeName));
    modeItem->setForeground(Qt::darkMagenta); // 紫色标注模式切换
    ui->realTimeLogList->insertItem(0, modeItem);

    if(m_model) m_model->select();




    // --3.多线程日志
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


        // --- 2. 新增：将信息实时显示到 UI 列表 ---
        QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");

        // 1. 记录原始环境数据
        QString logEntry = QString("[%1] 环境监测 -> 温度: %2℃, 湿度: %3%").arg(timeStr).arg(temp).arg(humi);
        ui->realTimeLogList->insertItem(0, logEntry);

        // 2. 自动化决策逻辑（仅限回家模式）
        if (m_currentMode == HomeMode) {
            // --- 温度控制空调 ---
            if (temp > 28.0) {
                if (query.exec("UPDATE devices SET status = 1 WHERE name = '空调'")) {
                    QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：温度过高(%2℃)，开启空调").arg(timeStr).arg(temp));
                    item->setForeground(Qt::red);
                    ui->realTimeLogList->insertItem(0, item);
                }
            }
            else if (temp < 22.0) {
                if (query.exec("UPDATE devices SET status = 0 WHERE name = '空调'")) {
                    QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：温度适宜(%2℃)，关闭空调").arg(timeStr).arg(temp));
                    item->setForeground(Qt::darkGreen);
                    ui->realTimeLogList->insertItem(0, item);
                }
            }

            // --- 湿度控制除湿器 (补全这部分) ---
            if (humi > 80.0) {
                if (query.exec("UPDATE devices SET status = 1 WHERE name = '除湿器'")) {
                    QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：湿度过高(%2%)，开启除湿器").arg(timeStr).arg(humi));
                    item->setForeground(Qt::red);
                    ui->realTimeLogList->insertItem(0, item);
                }
            }
            else if (humi < 40.0) {
                if (query.exec("UPDATE devices SET status = 0 WHERE name = '除湿器'")) {
                    QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：空气干燥(%2%)，关闭除湿器").arg(timeStr).arg(humi));
                    item->setForeground(Qt::darkGreen);
                    ui->realTimeLogList->insertItem(0, item);
                }
            }

            m_model->select(); // 更新数据库视图
        }

        // 3. 限制日志行数，防止内存占用过高
        while(ui->realTimeLogList->count() > 50) {
            delete ui->realTimeLogList->takeItem(ui->realTimeLogList->count() - 1);
        }


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






