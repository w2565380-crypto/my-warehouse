#include "datacollector.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>
#include <QSqlError>


DataCollector::DataCollector(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    // 尝试连接本地模拟服务器（假设 IP 为 127.0.0.1，端口 8080）
    m_socket->connectToHost("127.0.0.1", 8080);
}

void DataCollector::collectData() {
    // 1. 检查当前线程是否有可用的数据库连接
    // SQLite 要求每个线程使用独立的连接名
    QString connectionName = "thread_recorder";
    QSqlDatabase db;

    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        // 如果是第一次运行，为子线程建立专属连接
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName("smarthome.db"); // 必须和主线程的文件名一致
    }

    if (!db.isOpen() && !db.open()) {
        qDebug() << "子线程打开数据库失败:" << db.lastError().text();
        return;
    }

    // 2. 生成随机数据
    double temp = QRandomGenerator::global()->bounded(200, 320) / 10.0;
    double humi = QRandomGenerator::global()->bounded(350, 800) / 10.0;

    // 3. 执行写入（注意：QSqlQuery 构造时必须指定 db）
    QSqlQuery query(db);
    query.prepare("INSERT INTO env_data (temp, humi, time) VALUES (:t, :h, :m)");
    query.bindValue(":t", temp);
    query.bindValue(":h", humi);
    query.bindValue(":m", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec()) {
        qDebug() << "子线程写入成功 | 温度:" << temp << " 湿度:" << humi;
        // 发送信号，把数据带给 MainWindow
        emit dataUpdated(temp, humi);
    } else {
        qDebug() << "写入失败:" << query.lastError().text();
    }

    // 3. TCP 发送逻辑
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        // 构建符合 IoT 标准的报文格式
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString payload = QString("{\"device\":\"ControlCenter\",\"temp\":%1,\"humi\":%2,\"time\":\"%3\"}")
                              .arg(temp).arg(humi).arg(timeStr);

        m_socket->write(payload.toUtf8());
        m_socket->flush(); // 强制发送缓冲区数据
        qDebug() << "TCP数据已上报网关:" << payload;
    } else {
        // 如果未连接，尝试重连，不阻塞本次采集
        qDebug() << "TCP未连接，正在尝试重连网关...";
        m_socket->connectToHost("127.0.0.1", 8080);
    }
}
