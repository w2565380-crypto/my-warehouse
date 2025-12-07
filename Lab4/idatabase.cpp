#include "idatabase.h"
#include <QDebug>
#include <QSqlError>

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
    // 1. 【核心修正】检查模型是否已初始化，防止重复创建和内存泄漏
    if (patientTabModel != nullptr) {
        // 如果模型已经存在，直接返回 true
        return true;
    }

    // 2. 创建 QSqlTableModel
    // 使用 this (IDatabase 单例) 作为父对象
    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");

    // 设置编辑策略为手动提交 (OnManualSubmit)
    // 这要求在 PatientEditView 中必须调用 submitAll() 才能将更改写入数据库
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置排序方式：按 'name' 字段升序排列
    // 必须使用 fieldIndex() 确保列名正确
    int nameIndex = patientTabModel->fieldIndex("name");
    if (nameIndex != -1) {
        patientTabModel->setSort(nameIndex, Qt::AscendingOrder);
    } else {
        qDebug() << "Warning: 'name' field not found for sorting.";
    }

    // 3. 执行查询
    if (!(patientTabModel->select())) {
        qDebug() << "Error: Failed to select data from patient table:" << patientTabModel->lastError().text();
        // 如果查询失败，应清理指针（虽然很少见，但安全起见）
        delete patientTabModel;
        patientTabModel = nullptr;
        return false;
    }

    // 4. 创建 QItemSelectionModel
    // thePatientSelection 必须在模型创建成功后才能创建
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

        // 3. 【核心修正 1】如果模型提交成功，提交数据库事务到磁盘
        if (database.commit()) {
            patientTabModel->select(); // 4. 【核心修正 2】强制模型重新查询数据（确保刷新）
            qDebug() << "Patient data submitted successfully and committed to disk.";
            return true;
        } else {
            qDebug() << "Error: Database commit failed:" << database.lastError().text();
            database.rollback(); // 提交失败，回滚
            patientTabModel->revertAll();
            return false;
        }
    } else {
        // 模型提交失败（可能是因为 NOT NULL 约束等）
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
