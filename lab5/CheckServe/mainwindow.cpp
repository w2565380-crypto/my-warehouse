#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_checkServer=new CheckServer(this);

    connect(m_checkServer,&CheckServer::logMessage,this,&MainWindow::logmessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_clicked()
{
    if(m_checkServer->isListening()){
        m_checkServer->stopServer();
        ui->startStopButton->setText("启动服务器");
        logmessage("服务器已经停止");
    }else{
        if(!m_checkServer->listen(QHostAddress::Any,1967)){
            QMessageBox::critical(this,"错误","无法启动服务器");
            return;
        }
        logmessage("服务器已启动");
        ui->startStopButton->setText("停止服务器");
    }
}

void MainWindow::logmessage(const QString &msg)
{
    ui->logEditor->appendPlainText(msg);
}

