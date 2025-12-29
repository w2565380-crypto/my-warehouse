#include "idatabase.h"
#include <QDebug>
#include <QSqlError>
#include <QUuid>

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

// 修正后的 IDatabase::initPatientModel()
bool IDatabase::initPatientModel()
{
    if (patientTabModel != nullptr) {
        return true;
    }

    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");

    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    int nameIndex = patientTabModel->fieldIndex("name");
    if (nameIndex != -1) {
        patientTabModel->setSort(nameIndex, Qt::AscendingOrder);
    } else {
        qDebug() << "Warning: 'name' field not found for sorting.";
    }

    if (!(patientTabModel->select())) {
        qDebug() << "Error: Failed to select data from patient table:" << patientTabModel->lastError().text();
        delete patientTabModel;
        patientTabModel = nullptr;
        return false;
    }

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    qDebug() << "Patient model initialized successfully.";
    return true;
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

// 修正后的 IDatabase::deleteCurrentPatient()
bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
        if (!curIndex.isValid())
            return false; // 没有选中行
        // 尝试删除行（本地操作）
        if (patientTabModel->removeRow(curIndex.row())) {
        // 提交到数据库
        if (patientTabModel->submitAll()) {
            patientTabModel->select(); // 重新加载数据
            return true;
        } else {
            // 提交失败，回滚
          patientTabModel->revertAll();
          patientTabModel->select(); // 重新加载数据
          qDebug() << "Delete submission failed:" << patientTabModel->lastError().text();
          return false;
     }
    }
    return false;
}

bool IDatabase::submitPatientEdit()
{
    // 1. 开始事务
    if (!database.transaction()) {
        qDebug() << "Failed to start database transaction.";
        return false;
    }

    // 2. 提交模型缓存到数据库（内存）
    if (patientTabModel->submitAll()) {

        if (database.commit()) {
            patientTabModel->select();
            qDebug() << "Patient data submitted successfully and committed to disk.";
            return true;
        } else {
            qDebug() << "Error: Database commit failed:" << database.lastError().text();
            database.rollback(); // 提交失败，回滚
            patientTabModel->revertAll();
            return false;
        }
    } else {
        qDebug() << "Error: Model submission failed:" << patientTabModel->lastError().text();
        database.rollback();
        patientTabModel->revertAll();
        return false;
    }
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex=patientTabModel->index(patientTabModel->rowCount()-1,1);

    int curRecNo=curIndex.row();
    QSqlRecord curRec=patientTabModel->record(curRecNo);
    curRec.setValue("CREATETIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo,curRec);
    return curIndex.row();
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
