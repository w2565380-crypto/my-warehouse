#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel> // 必须包含这个头文件
#include "modeselectiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // 修改构造函数，增加 mode 参数
    explicit MainWindow(QWidget *parent = nullptr, SceneMode mode = HomeMode);
    ~MainWindow();

private slots:
    void on_btnBack_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *m_model; // 定义一个指向数据库模型的指针
    SceneMode m_currentMode; // 增加成员变量存储当前模式


signals:
    void backToMenu();

};
#endif
