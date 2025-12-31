#include "mainwindow.h"
#include "logindialog.h"
#include "modeselectiondialog.h"
#include <QApplication>
#include "DatabaseManager.h"
#include "logviewer.h"
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 【重要】禁止在最后一个窗口关闭时退出应用程序
    // 否则当 MainWindow 关闭而 ModeSelectionDialog 还没显现的间隙，程序会直接结束
    a.setQuitOnLastWindowClosed(false);

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
                        backToSelection = true; // 日志关闭后，回到模式选择循环
                    }
                    else {
                        // --- 情况 B：进入主界面 ---
                        // 创建主窗口，注意：一定要确保 MainWindow 构造函数里设置了 Qt::WA_DeleteOnClose
                        MainWindow *w = new MainWindow(nullptr, selection.getSelectedMode());
                        w->show();

                        // 创建局部事件循环，阻塞在这里直到 w 被销毁
                        QEventLoop loop;
                        QObject::connect(w, &MainWindow::destroyed, &loop, &QEventLoop::quit);
                        loop.exec();

                        // 当 MainWindow 被销毁（close并发出了destroyed信号）后，逻辑执行到这里
                        backToSelection = true; // 返回模式选择界面
                    }
                }
                else if (selection.isBackRequested()) {
                    // --- 情况 C：点击返回登录 ---
                    backToLogin = true;
                }
                else {
                    // 用户直接点击了关闭按钮或取消，彻底退出程序
                    return 0;
                }
            }
        }
    }

    return 0;
}
