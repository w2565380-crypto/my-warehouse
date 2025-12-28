#include "datacollector.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDebug>
#include <QSqlError>

DataCollector::DataCollector(QObject *parent) : QObject(parent) {}

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
    double humi = QRandomGenerator::global()->bounded(400, 700) / 10.0;

    // 3. 执行写入（注意：QSqlQuery 构造时必须指定 db）
    QSqlQuery query(db);
    query.prepare("INSERT INTO env_data (temp, humi, time) VALUES (:t, :h, :m)");
    query.bindValue(":t", temp);
    query.bindValue(":h", humi);
    query.bindValue(":m", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec()) {
        qDebug() << "子线程写入成功 | 温度:" << temp << " 湿度:" << humi;
    } else {
        qDebug() << "写入失败:" << query.lastError().text();
    }
}
