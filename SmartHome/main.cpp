#include "mainwindow.h"
#include <QApplication>
#include "DatabaseManager.h" // 引用头文件

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 在显示界面前初始化数据库
    if (!DatabaseManager::initDatabase()) {
        return -1; // 初始化失败则退出
    }

    MainWindow w;
    w.show();
    return a.exec();
}
