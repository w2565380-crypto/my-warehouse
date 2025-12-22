#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class DatabaseManager {
public:
    // 初始化数据库并创建表
    static bool initDatabase();

    // 之后可以在这里添加其他的数据库操作函数，比如：
    // static bool addDevice(QString name, QString type, int status);
};

#endif // DATABASEMANAGER_H
