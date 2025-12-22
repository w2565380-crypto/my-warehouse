#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 初始化模型
    m_model = new QSqlTableModel(this);

    // 2. 关联数据库表名（必须和你 Navicat 里建的表名一模一样）
    m_model->setTable("devices");

    // 3. 设置编辑策略：点击单元格修改后立即同步到数据库
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    // 4. 从数据库拉取数据
    if (!m_model->select()) {
        QMessageBox::critical(this, "错误", "无法读取数据库表格: " + m_model->lastError().text());
    }

    // 5. 设置漂亮的中文表头（对应报告 2.4 节设计说明）
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "设备名称");
    m_model->setHeaderData(2, Qt::Horizontal, "类型");
    m_model->setHeaderData(3, Qt::Horizontal, "状态 (0/1)");
    m_model->setHeaderData(4, Qt::Horizontal, "房间");

    // 6. 核心步骤：将数据模型绑定到 UI 控件上
    ui->deviceTableView->setModel(m_model);

    // 让表格自动拉伸填充窗口
    ui->deviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow() {
    delete ui;
}
