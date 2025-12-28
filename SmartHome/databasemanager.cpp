#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

bool DatabaseManager::initDatabase() {
    // 1. 添加驱动并设置路径
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("smarthome.db");

    // 2. 打开数据库
    if (!db.open()) {
        qDebug() << "Error: Failed to open database." << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    // 3. 创建设备表 (原有)
    QString sqlDevices = "CREATE TABLE IF NOT EXISTS devices ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "name TEXT NOT NULL, "
                         "type TEXT, "
                         "status INTEGER DEFAULT 0, "
                         "room TEXT)";
    if (!query.exec(sqlDevices)) {
        qDebug() << "Error: Fail to create devices table." << query.lastError().text();
    }

    // 4. 创建用户表 (新增：用于登录模块)
    QString sqlUser = "CREATE TABLE IF NOT EXISTS user ("
                      "username TEXT PRIMARY KEY, "
                      "password TEXT NOT NULL)";
    if (!query.exec(sqlUser)) {
        qDebug() << "Error: Fail to create user table." << query.lastError().text();
    }

    // 5. 创建环境数据表 (新增：用于多线程采集和图表)
    QString sqlEnv = "CREATE TABLE IF NOT EXISTS env_data ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "temp REAL, "
                     "humi REAL, "
                     "time TEXT)";
    if (!query.exec(sqlEnv)) {
        qDebug() << "Error: Fail to create env_data table." << query.lastError().text();
    }

    qDebug() << "Database initialized successfully with 3 tables!";
    return true;
}













