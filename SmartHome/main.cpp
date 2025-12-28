#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. 初始化数据库
    if (!DatabaseManager::initDatabase()) return -1;

    // 2. 弹出登录对话框
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        // 只有登录成功（Accepted），才创建并显示主窗口
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        // 否则直接退出
        return 0;
    }
}
