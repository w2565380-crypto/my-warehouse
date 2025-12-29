#include "mainwindow.h"
#include "logindialog.h"
#include "modeselectiondialog.h" // 记得包含新头文件
#include <QApplication>
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::initDatabase()) return -1;

    bool keepRunning = true;
    while (keepRunning) {
        keepRunning = false; // 默认执行完就退出

        LoginDialog login;
        if (login.exec() == QDialog::Accepted) {

            // 登录成功后，弹出模式选择对话框
            ModeSelectionDialog selection;
            int result = selection.exec();

            if (result == QDialog::Accepted) {
                // 情况 A：选择了某种模式进入主界面
                if (!selection.isLogRequested()) {
                    MainWindow *w = new MainWindow(nullptr, selection.getSelectedMode());
                    w->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动释放内存
                    w->show();
                    return a.exec();
                } else {
                    // 情况 B：跳转到日志界面（如果还没写，可以先弹个窗）
                    // LogViewer *v = new LogViewer();
                    // v->show();
                    // return a.exec();
                }
            } else if (selection.isBackRequested()) {
                // 情况 C：点击了“返回上一级”，重新循环回到登录界面
                keepRunning = true;
            }
        }
    }
    return 0;
}
