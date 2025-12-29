//replacedialog.cpp
#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QTextCursor>
#include <QDebug>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
    , m_textEdit(textEdit)
    , m_lastFoundPosition(-1)
{
    ui->setupUi(this);

    // 设置默认查找方向为向下
    ui->rbDown->setChecked(true);

    // 手动连接信号槽
    connect(ui->btFindNext, &QPushButton::clicked,
            this, &ReplaceDialog::btFindNext_clicked);
    connect(ui->btReplace, &QPushButton::clicked,
            this, &ReplaceDialog::btReplace_clicked);
    connect(ui->btReplaceAll, &QPushButton::clicked,
            this, &ReplaceDialog::btReplaceAll_clicked);
    connect(ui->btCancel, &QPushButton::clicked,
            this, &ReplaceDialog::btCancel_clicked);

    // 查找输入框并连接回车键
    // 使用 findChild 方法按类型查找
    QLineEdit *findEdit = findChild<QLineEdit*>();
    if (findEdit) {
        connect(findEdit, &QLineEdit::returnPressed,
                this, &ReplaceDialog::btFindNext_clicked);
    }

    qDebug() << "替换对话框已创建，文本编辑器指针:" << m_textEdit;
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

bool ReplaceDialog::findNext(bool showMessage)
{
    // 获取查找文本 - 使用安全的方法
    QLineEdit *findEdit = findChild<QLineEdit*>();
    if (!findEdit) {
        if (showMessage) {
            QMessageBox::information(this, "查找", "找不到查找输入框。");
        }
        return false;
    }

    QString searchText = findEdit->text();
    if (searchText.isEmpty()) {
        if (showMessage) {
            QMessageBox::information(this, "查找", "请输入要查找的文本。");
            findEdit->setFocus();
        }
        return false;
    }

    if (!m_textEdit) {
        qDebug() << "错误：文本编辑器指针为空";
        return false;
    }

    QTextDocument::FindFlags flags;

    // 设置查找方向
    bool searchUp = ui->rbUp->isChecked();
    if (searchUp) {
        flags |= QTextDocument::FindBackward;
    }

    // 设置是否区分大小写
    if (ui->checkBox->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    bool found = false;

    // 如果之前找到过，从上次位置开始查找
    if (m_lastFoundPosition != -1) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.setPosition(m_lastFoundPosition);
        m_textEdit->setTextCursor(cursor);
    }

    // 执行查找
    found = m_textEdit->find(searchText, flags);

    // 如果向上查找没找到，尝试从文档末尾重新开始查找
    if (!found && searchUp) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        m_textEdit->setTextCursor(cursor);
        found = m_textEdit->find(searchText, flags);
    }
    // 如果向下查找没找到，尝试从文档开头重新开始查找
    else if (!found && !searchUp) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        m_textEdit->setTextCursor(cursor);
        found = m_textEdit->find(searchText, flags);
    }

    if (found) {
        m_lastFoundPosition = m_textEdit->textCursor().position();
        // 确保找到的文本可见
        m_textEdit->ensureCursorVisible();
    } else if (showMessage) {
        QMessageBox::information(this, "查找",
                                 QString("找不到 \"%1\"").arg(searchText));
        // 重置查找位置
        m_lastFoundPosition = -1;
    }

    return found;
}

void ReplaceDialog::btFindNext_clicked()
{
    qDebug() << "查找下一个按钮被点击";
    findNext(true);
}

void ReplaceDialog::btReplace_clicked()
{
    qDebug() << "替换按钮被点击";

    // 获取替换文本 - 使用安全的方法
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
    QString replaceText = "";

    if (lineEdits.size() > 1) {
        // 假设第二个QLineEdit是替换输入框
        replaceText = lineEdits[1]->text();
    } else if (!lineEdits.isEmpty()) {
        // 如果只有一个输入框，使用它
        replaceText = lineEdits[0]->text();
    }

    // 检查当前是否有选中的文本（即是否已经找到了要替换的文本）
    QTextCursor cursor = m_textEdit->textCursor();
    if (cursor.hasSelection()) {
        // 替换选中的文本
        cursor.insertText(replaceText);
        qDebug() << "已替换选中文本";

        // 查找下一个
        findNext(true);
    } else {
        // 如果没有选中文本，先尝试查找下一个
        if (findNext(false)) {
            // 如果找到了，现在文本已经被选中，等待用户点击替换或继续查找
            qDebug() << "已找到文本，等待用户确认替换";
        } else {
            QMessageBox::information(this, "替换", "没有找到要替换的文本。");
        }
    }
}

void ReplaceDialog::btReplaceAll_clicked()
{
    qDebug() << "全部替换按钮被点击";

    // 获取查找文本 - 使用安全的方法
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
    if (lineEdits.isEmpty()) {
        QMessageBox::information(this, "全部替换", "找不到查找输入框。");
        return;
    }

    QString findText = lineEdits[0]->text();
    if (findText.isEmpty()) {
        QMessageBox::information(this, "全部替换", "请输入要查找的文本。");
        lineEdits[0]->setFocus();
        return;
    }

    // 获取替换文本
    QString replaceText = "";
    if (lineEdits.size() > 1) {
        replaceText = lineEdits[1]->text();
    }

    if (!m_textEdit) {
        qDebug() << "错误：文本编辑器指针为空";
        return;
    }

    // 保存当前光标位置
    QTextCursor originalCursor = m_textEdit->textCursor();

    // 移动到文档开始
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    m_textEdit->setTextCursor(cursor);

    QTextDocument::FindFlags flags;
    // 设置是否区分大小写
    if (ui->checkBox->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }

    int replaceCount = 0;
    bool found = false;

    // 循环查找并替换所有匹配项
    do {
        found = m_textEdit->find(findText, flags);
        if (found) {
            // 替换找到的文本
            QTextCursor foundCursor = m_textEdit->textCursor();
            foundCursor.insertText(replaceText);
            replaceCount++;
        }
    } while (found);

    // 恢复原始光标位置
    m_textEdit->setTextCursor(originalCursor);

    // 显示替换结果
    QMessageBox::information(this, "全部替换",
                             QString("已完成 %1 处替换").arg(replaceCount));

    qDebug() << "全部替换完成，共替换:" << replaceCount << "处";
}

void ReplaceDialog::btCancel_clicked()
{
    this->close();
}
