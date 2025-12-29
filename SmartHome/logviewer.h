#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui { class LogViewer; }

class LogViewer : public QDialog {
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = nullptr);
    ~LogViewer();

private slots:
    void on_backBtn_clicked(); // 返回按钮槽函数

private:
    Ui::LogViewer *ui;
    QSqlTableModel *m_model;
};

#endif
