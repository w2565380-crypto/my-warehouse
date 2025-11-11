#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~SearchDialog();

private slots:
    void bfFindNext_clicked();  // 保持原名，但手动连接
    void btCancel_clicked();    // 保持原名，但手动连接

private:
    Ui::SearchDialog *ui;
    QPlainTextEdit *m_textEdit;
    int m_lastFoundPosition;
};

#endif // SEARCHDIALOG_H
