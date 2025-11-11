#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~ReplaceDialog();

private slots:
    void btFindNext_clicked();
    void btReplace_clicked();
    void btReplaceAll_clicked();
    void btCancel_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *m_textEdit;
    int m_lastFoundPosition;

    bool findNext(bool showMessage = true);
};

#endif // REPLACEDIALOG_H
