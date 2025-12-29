#include "modeselectiondialog.h"
#include "ui_modeselectiondialog.h"

ModeSelectionDialog::ModeSelectionDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ModeSelectionDialog) {
    ui->setupUi(this);
    this->setWindowTitle("选择工作场景");
}

ModeSelectionDialog::~ModeSelectionDialog() { delete ui; }

// 点击回家模式
void ModeSelectionDialog::on_homeBtn_clicked() {
    m_selectedMode = HomeMode;
    m_showLog = false;
    accept(); // 关闭对话框，返回 QDialog::Accepted
}

// 点击离家模式
void ModeSelectionDialog::on_awayBtn_clicked() {
    m_selectedMode = AwayMode;
    m_showLog = false;
    accept();
}

// 点击睡眠模式
void ModeSelectionDialog::on_sleepBtn_clicked() {
    m_selectedMode = SleepMode;
    m_showLog = false;
    accept();
}

// 点击日志查询
void ModeSelectionDialog::on_logBtn_clicked() {
    m_showLog = true;
    accept();
}

//返回上一级
void ModeSelectionDialog::on_backBtn_clicked() {
    m_backToLogin = true;
    this->reject(); // 关闭当前对话框，返回 QDialog::Rejected
}
