#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStack>
#include<QLabel>
#include <QKeyEvent>
#include <QPushButton>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString operate;
    QStack<QString> operates;
    QStack<QString> opcodes;
    QStack<QString> opcode;
    QMap<int,QPushButton*> digitBTNs;
    QString calculation(bool *ok = nullptr);

private slots:
    void btnNumClicked();

    void on_btnPoints_clicked();

    void on_btnDel_clicked();

    void on_btnClear_clicked();

    void btnBinaryOperatorClicked();

    void on_btnEqual_clicked();

    void btnUnaryOperatorClicked();

    virtual void keyPressEvent(QKeyEvent *event);

    void on_btnInverse_clicked();

    void on_btnClearEntry_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
