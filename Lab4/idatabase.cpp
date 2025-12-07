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

bool IDatabase::initPatientModel()
{
    patientTabModel=new QSqlTableModel(this,database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);

    if(!(patientTabModel->select()))
        return false;

    thePatientSelection=new QItemSelectionModel(patientTabModel);
    return true;
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex=thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();

}

bool IDatabase::submitPatientEdit()
{
    if (patientTabModel->submitAll()) {
        patientTabModel->select(); // 提交成功后重新加载数据，确保显示正确
        qDebug() << "Patient data submitted successfully.";
        return true;
    } else {
        qDebug() << "Patient data submission failed:" << patientTabModel->lastError().text();
        patientTabModel->revertAll(); // 提交失败则回滚
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
