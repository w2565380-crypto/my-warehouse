#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

PatientEditView::PatientEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper=new QDataWidgetMapper();
    QSqlTableModel *tabModel=IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);


    dataMapper->addMapping(ui->dbEditID,tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard,tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinHeight,tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinWeiget,tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile,tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDataEditDOB,tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbCombSex,tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreatedTimeStamp,tabModel->fieldIndex("CREATETIMESTAMP"));
    dataMapper->setCurrentIndex(index);
}


PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    // 1. 强制将当前控件中的数据写入 QSqlTableModel 缓存
    dataMapper->submit();
    // 2. 将 QSqlTableModel 缓存中的所有数据写入数据库
    IDatabase::getInstance().submitPatientEdit();
    emit goPreviousView();
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

