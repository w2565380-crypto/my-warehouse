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

    bool restartApp = true;
    while (restartApp) {
        restartApp = false; // 默认不重复执行

        LoginDialog login;
        if (login.exec() == QDialog::Accepted) {

            // 模式选择界面
            ModeSelectionDialog selection;
            int ret = selection.exec();

            if (ret == QDialog::Accepted) {
                if (selection.isLogRequested()) {
                    // --- 情况 A：查看日志 ---
                    LogViewer logWin;
                    logWin.exec();     // 阻塞在这里，直到日志窗口关闭
                    restartApp = true; // 【关键】关闭日志窗口后，回到 while 开头重新选模式
                } else {
                    // --- 情况 B：选择场景进入主控制中心 ---
                    MainWindow w(nullptr, selection.getSelectedMode());
                    w.show();
                    return a.exec(); // 进入 Qt 事件循环，主程序正常运行
                }
            }
            else if (selection.isBackRequested()) {
                // --- 情况 C：点击返回上一级 ---
                restartApp = true; // 回到登录界面
            }
        }
    }

    return 0;
}
