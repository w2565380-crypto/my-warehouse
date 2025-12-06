#include "idatabase.h"

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="G:/Qt/sql/lab4.db";
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"打开数据库失败。";
    }else
        qDebug()<<"成功打开数据库。";
}

QString IDatabase::userLogin(QString userName, QString password)
{
    //return "登录成功";
    QSqlQuery query;
    query.prepare("select username,password from user where username=:USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first()&&query.value("username").isValid()){
        QString passwd=query.value("password").toString();
        if(passwd==password){
            qDebug()<<"登录成功";
            return "登录成功";
        }
        else{
            qDebug()<<"密码错误";
            return "用户密码错误";
        }
    }
    else{
        qDebug()<<"未能查找到用户"<<userName;
        return "用户密码错误";
    }
}

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
