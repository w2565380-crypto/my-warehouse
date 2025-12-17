#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    m_checkClient=new CheckClient(this);

    connect(m_checkClient, &CheckClient::connected, this, &MainWindow::connectedToServer);
    connect(m_checkClient,&CheckClient::messageReceived,this,&MainWindow::messageReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    m_checkClient->connectToServer(QHostAddress(ui->ServerEdit->text()),1967);
}

void MainWindow::connectedToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    m_checkClient->sendMessage(ui->usernameEdit->text(),"login");
}

void MainWindow::messageReceived(const QString &text)
{
    ui->roomTextEdit->append(text);
}

void MainWindow::on_sayButton_clicked()
{
    if(!ui->sayLineEdit->text().isEmpty())
        m_checkClient->sendMessage(ui->sayLineEdit->text());
}


void MainWindow::on_logoutBotton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

}




