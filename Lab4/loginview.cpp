#include "loginview.h"
#include "ui_loginview.h"
#include <idatabase.h>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

// 修正后的 LoginView::on_btSignIn_clicked()
void LoginView::on_btSignIn_clicked()
{
    QString status = IDatabase::getInstance().userLogin(
        ui->inputUserName->text(),
        ui->inputUserPassWord->text()
    );

    if (status == "登录成功") {
        emit loginSuccess();
    } else {
        // 登录失败，发出信号
        emit loginFailed();
        // 建议在这里或 MainWindow 中显示 status 内容，例如：
        // ui->labelError->setText(status);
    }
}

