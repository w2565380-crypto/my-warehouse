#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QSqlQuery>
#include "datacollector.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent, SceneMode mode)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentMode(mode)
{
    ui->setupUi(this);

    // 1. 初始化数据库模型
    m_model = new QSqlTableModel(this);
    m_model->setTable("devices");
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    if (!m_model->select()) {
        QMessageBox::critical(this, "错误", "无法读取数据库表格: " + m_model->lastError().text());
    }

    // 设置表头
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "设备名称");
    m_model->setHeaderData(2, Qt::Horizontal, "类型");
    m_model->setHeaderData(3, Qt::Horizontal, "状态 (0/1)");
    m_model->setHeaderData(4, Qt::Horizontal, "房间");

    ui->deviceTableView->setModel(m_model);
    ui->deviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. 初始模式切换逻辑 (仅在窗口创建时运行一次)
    QSqlQuery initQuery;
    QString timeStr = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString modeName;

    if (m_currentMode == HomeMode) {
        initQuery.exec("UPDATE devices SET status = 1 WHERE name = '灯'");
        initQuery.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
        modeName = "【回家模式】：欢迎回来，照明与空调已开启";
    }
    else if (m_currentMode == AwayMode) {
        initQuery.exec("UPDATE devices SET status = 0");
        modeName = "【离家模式】：全屋电器已一键关闭";
    }
    else if (m_currentMode == SleepMode) {
        initQuery.exec("UPDATE devices SET status = 0 WHERE name = '灯'");
        initQuery.exec("UPDATE devices SET status = 1 WHERE name = '空调'");
        modeName = "【睡眠模式】：灯光已熄灭，空调持续运行";
    }

    QListWidgetItem *modeItem = new QListWidgetItem(QString("[%1] 系统状态：%2").arg(timeStr).arg(modeName));
    modeItem->setForeground(Qt::darkMagenta);
    ui->realTimeLogList->insertItem(0, modeItem);
    m_model->select();

    // 3. 多线程数据采集
    QThread* thread = new QThread(this);
    DataCollector* worker = new DataCollector();
    worker->moveToThread(thread);

    QTimer* timer = new QTimer();
    timer->setInterval(3000);
    timer->moveToThread(thread);

    connect(thread, &QThread::started, timer, QOverload<>::of(&QTimer::start));
    connect(timer, &QTimer::timeout, worker, &DataCollector::collectData);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, timer, &QObject::deleteLater);

    // 4. 核心控制逻辑：根据温湿度自动控制设备
    connect(worker, &DataCollector::dataUpdated, this, [=](double temp, double humi){
        QSqlQuery query;
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        bool isChanged = false; // 用于判断是否需要刷新表格

        // 记录环境日志
        ui->realTimeLogList->insertItem(0, QString("[%1] 环境监测 -> 温度: %2℃, 湿度: %3%").arg(currentTime).arg(temp).arg(humi));

        // 仅在回家模式下开启自动温控
        if (m_currentMode == HomeMode) {

            // --- A. 空调逻辑 ---
            query.exec("SELECT status FROM devices WHERE name = '空调'");
            if (query.next()) {
                int status = query.value(0).toInt();
                // 低于22度且开着 -> 关闭
                if (temp < 22.0 && status == 1) {
                    if(query.exec("UPDATE devices SET status = 0 WHERE name = '空调'")) {
                        QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：温度适宜(%2℃)，已关闭空调").arg(currentTime).arg(temp));
                        item->setForeground(Qt::darkGreen);
                        ui->realTimeLogList->insertItem(0, item);
                        isChanged = true;
                    }
                }
                // 高于28度且关着 -> 开启
                else if (temp > 28.0 && status == 0) {
                    if(query.exec("UPDATE devices SET status = 1 WHERE name = '空调'")) {
                        QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：温度过高(%2℃)，已开启空调").arg(currentTime).arg(temp));
                        item->setForeground(Qt::red);
                        ui->realTimeLogList->insertItem(0, item);
                        isChanged = true;
                    }
                }
            }

            // --- B. 除湿器逻辑 ---
            query.exec("SELECT status FROM devices WHERE name = '除湿器'");
            if (query.next()) {
                int status = query.value(0).toInt();
                // 湿度低且开着 -> 关闭
                if (humi < 40.0 && status == 1) {
                    if(query.exec("UPDATE devices SET status = 0 WHERE name = '除湿器'")) {
                        QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：空气干燥(%2%)，已关闭除湿器").arg(currentTime).arg(humi));
                        item->setForeground(Qt::darkGreen);
                        ui->realTimeLogList->insertItem(0, item);
                        isChanged = true;
                    }
                }
                // 湿度高且关着 -> 开启
                else if (humi > 60.0 && status == 0) {
                    if(query.exec("UPDATE devices SET status = 1 WHERE name = '除湿器'")) {
                        QListWidgetItem *item = new QListWidgetItem(QString("[%1] 自动控制：湿度过高(%2%)，已开启除湿器").arg(currentTime).arg(humi));
                        item->setForeground(Qt::red);
                        ui->realTimeLogList->insertItem(0, item);
                        isChanged = true;
                    }
                }
            }
        }

        // 如果状态变了，刷新界面显示
        if(isChanged) {
            m_model->select();
        }

        // 日志清理
        while(ui->realTimeLogList->count() > 50) {
            delete ui->realTimeLogList->takeItem(ui->realTimeLogList->count() - 1);
        }
    });

    thread->start();

    // 安全退出线程
    connect(this, &MainWindow::destroyed, thread, [=]() {
        thread->quit();
        thread->wait();
        thread->deleteLater();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
