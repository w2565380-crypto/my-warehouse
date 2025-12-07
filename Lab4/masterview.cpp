#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include <idatabase.h>

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    IDatabase::getInstance(); // 先初始化数据库

    // 确保 loginView 成员指针初始化为 nullptr
    loginView = nullptr; // 通常在 .h 文件中初始化，或在初始化列表显式设置

    goLoginView(); // <--- 仅调用一次跳转函数来创建和显示
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    qDebug() << "goLoginView";

    // 1. 惰性创建：仅在第一次调用时创建实例
    if (!loginView) {
        loginView = new LoginView(this);
        // 2. 确保信号槽仅连接一次
        connect(loginView, SIGNAL(loginSuccess()), this, SLOT(goWelcomeView()));
    }

    // 3. 将已有的实例添加到 QStackedWidget 并显示
    // ⚠️ 注意：addWidget 如果 widget 已经在 Stack 中，它会忽略，这是安全的。
    // 但是，在调用 setCurrentWidget 之前，建议先确保 loginView 确实被 add 进去了。
    ui->stackedWidget->addWidget(loginView);
    ui->stackedWidget->setCurrentWidget(loginView);
    ui->labelTitle->setText(loginView->windowTitle());
}

void MasterView::goWelcomeView()
{
    qDebug()<<"goWelcomeView";
    welcomeView=new WelcomeView(this);
    pushWidgetToStackView(welcomeView);
    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
}

void MasterView::goDoctorView()
{
    qDebug()<<"goDoctorView";
    doctorView=new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void MasterView::goDepartmentView()
{
    qDebug()<<"goDepartmentView";
    departmentView=new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void MasterView::goPatientEditView(int rowNo)
{
    qDebug()<<"goPatientEditView";
    patientEditView=new PatientEditView(this,rowNo);
    pushWidgetToStackView(patientEditView);

    connect(patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goPatientView()
{
    qDebug()<<"goPatientView";
    patientView=new PatientView(this);
    pushWidgetToStackView(patientView);
    connect(patientView,SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    qDebug()<<"goPreviousView";
    int count=ui->stackedWidget->count();
    if(count>1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget=ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    int count=ui->stackedWidget->count();
    ui->stackedWidget->addWidget(widget);
    ui->stackedWidget->setCurrentIndex(count-1);
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count=ui->stackedWidget->count();
    if(count>1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);

    QString title=ui->stackedWidget->currentWidget()->windowTitle();
    if(title=="欢迎"){
        ui->btLogout->setEnabled(true);
        ui->btBack->setEnabled(false);
    }else
        ui->btLogout->setEnabled(false);
}


void MasterView::on_btLogout_clicked()
{
    goPreviousView();
}

