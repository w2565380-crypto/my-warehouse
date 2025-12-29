#include "logviewer.h"
#include "ui_logviewer.h"
#include <QHeaderView>

LogViewer::LogViewer(QWidget *parent) : QDialog(parent), ui(new Ui::LogViewer) {
    ui->setupUi(this);
    this->setWindowTitle("服务器环境日志中心");
    this->resize(600, 400); // 设置一个舒适的大小

    m_model = new QSqlTableModel(this);
    m_model->setTable("env_data"); // 绑定环境数据表

    // 设置表头显示名称
    m_model->setHeaderData(0, Qt::Horizontal, "编号");
    m_model->setHeaderData(1, Qt::Horizontal, "温度 (℃)");
    m_model->setHeaderData(2, Qt::Horizontal, "湿度 (%)");
    m_model->setHeaderData(3, Qt::Horizontal, "记录时间");

    // 按照时间戳列(索引为3)进行降序排列，方便查看最新数据
    m_model->setSort(3, Qt::DescendingOrder);

    m_model->select(); // 执行查询

    ui->tableView->setModel(m_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁用编辑触发器
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自适应
}

LogViewer::~LogViewer() { delete ui; }

// 点击返回按钮
void LogViewer::on_backBtn_clicked() {
    this->accept(); // 关闭当前对话框
}
