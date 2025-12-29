#include "mainwindow.h"
#include "logindialog.h"
#include "modeselectiondialog.h" // 记得包含新头文件
#include <QApplication>
#include "DatabaseManager.h"
#include "logviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!DatabaseManager::initDatabase()) return -1;

    bool backToLogin = true;
    while (backToLogin) { // 第一层：登录控制
        backToLogin = false;

        LoginDialog login;
        if (login.exec() == QDialog::Accepted) {

            bool backToSelection = true;
            while (backToSelection) { // 第二层：模式选择控制
                backToSelection = false;

                ModeSelectionDialog selection;
                int ret = selection.exec();

                if (ret == QDialog::Accepted) {
                    if (selection.isLogRequested()) {
                        // --- 情况 A：查看日志 ---
                        LogViewer logWin;
                        logWin.exec();
                        backToSelection = true; // 【修正】日志关闭后，回到模式选择循环
                    } else {
                        // --- 情况 B：进入主界面 ---
                        MainWindow w(nullptr, selection.getSelectedMode());
                        w.show();
                        return a.exec();
                    }
                }
                else if (selection.isBackRequested()) {
                    // --- 情况 C：点击返回登录 ---
                    backToLogin = true; // 退出当前的模式选择，回到第一层登录循环
                }
            }
        }
    }
    return 0;
}
