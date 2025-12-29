#include "patientview.h"
#include "ui_patientview.h"
#include "idatabase.h"
#include <QMessageBox>

PatientView::PatientView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PatientView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase=IDatabase::getInstance();
    if(iDatabase.initPatientModel()){
        ui->tableView->setModel(iDatabase.patientTabModel);
        ui->tableView->setSelectionModel(iDatabase.thePatientSelection);
    }
}

PatientView::~PatientView()
{
    delete ui;
}

void PatientView::on_btAdd_clicked()
{
    int currow=IDatabase::getInstance().addNewPatient();
    emit goPatientEditView(currow);
}


void PatientView::on_btSearch_clicked()
{
    QString filter=QString("name like'%%1%'").arg(ui->txtSearch->text());
    IDatabase::getInstance().searchPatient(filter);
}


// 增强的 PatientView::on_btDelete_clicked()
void PatientView::on_btDelete_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().thePatientSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请选择要删除的病人记录。");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除选中的病人记录吗？",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // 假设 IDatabase::deleteCurrentPatient() 返回 bool
        if (IDatabase::getInstance().deleteCurrentPatient()) {
            QMessageBox::information(this, "成功", "病人记录删除成功。");
        } else {
            QMessageBox::critical(this, "错误", "病人记录删除失败。");
        }
    }
}


// 修正后的 PatientView::on_btEdit_clicked()
void PatientView::on_btEdit_clicked()
{
    QModelIndex curIndex=IDatabase::getInstance().thePatientSelection->currentIndex();

    if (curIndex.isValid()) {
        emit goPatientEditView(curIndex.row());
    } else {
        qDebug() << "No patient selected for editing.";
    }
}

