#include "DatabaseManager.h"

bool DatabaseManager::initDatabase() {
    // 1. 添加驱动（SQLite）
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 2. 设置数据库文件路径
    // 提示：直接写文件名会生成在 build 目录下，方便用 Navicat 查看
    db.setDatabaseName("smarthome.db");

    // 3. 打开数据库
    if (!db.open()) {
        qDebug() << "Error: Failed to open database." << db.lastError().text();
        return false;
    }

    // 4. 创建设备表 (满足智能家居选题需求)
    QSqlQuery query;
    QString sql = "CREATE TABLE IF NOT EXISTS devices ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, " // 自动增长 ID
                  "name TEXT NOT NULL, "                  // 设备名
                  "type TEXT, "                           // 类型（灯、空调等）
                  "status INTEGER DEFAULT 0, "            // 状态（0关 1开）
                  "room TEXT)";                           // 所属房间

    if (!query.exec(sql)) {
        qDebug() << "Error: Fail to create table." << query.lastError().text();
        return false;
    }

    qDebug() << "Database initialized successfully!";
    return true;
}
