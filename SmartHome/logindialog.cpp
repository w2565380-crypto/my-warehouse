#include "logindialog.h"
#include "ui_logindialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    QString user = ui->editUser->text().trimmed();
    QString pwd = ui->editPwd->text().trimmed();

    QSqlQuery query;
    // 增加调试信息：打印当前尝试登录的信息
    qDebug() << "Attempting login for:" << user << pwd;

    query.prepare("SELECT * FROM user WHERE username = :u AND password = :p");
    query.bindValue(":u", user);
    query.bindValue(":p", pwd);

    if (query.exec()) {
        if (query.next()) {
            qDebug() << "Login success!";
            accept();
        } else {
            // 如果查不到，打印一下库里到底有没有数据
            QSqlQuery check;
            check.exec("SELECT count(*) FROM user");
            if(check.next()) qDebug() << "Database user count:" << check.value(0).toInt();

            QMessageBox::warning(this, "错误", "用户名或密码错误！");
        }
    } else {
        QMessageBox::critical(this, "数据库错误", query.lastError().text());
    }
}


void LoginDialog::on_btnLoginOut_clicked()
{
    this->close();
}

