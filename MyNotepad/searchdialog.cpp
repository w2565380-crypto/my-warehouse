//searchdialog.cpp
#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>
#include <QTextCursor>
#include <QDebug>

SearchDialog::SearchDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
    , m_textEdit(textEdit)
    , m_lastFoundPosition(-1)
{
    ui->setupUi(this);

    // 设置默认查找方向为向下
    ui->rbDown->setChecked(true);

    // 手动连接信号槽 - 使用原来的槽函数名
    connect(ui->btFindNext, &QPushButton::clicked,
            this, &SearchDialog::bfFindNext_clicked);
    connect(ui->btCancel, &QPushButton::clicked,
            this, &SearchDialog::btCancel_clicked);

    // 连接回车键到查找按钮
    connect(ui->SearchText, &QLineEdit::returnPressed,
            this, &SearchDialog::bfFindNext_clicked);

    qDebug() << "查找对话框已创建，文本编辑器指针:" << m_textEdit;
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::bfFindNext_clicked()
{
    qDebug() << "=== 查找按钮被点击 ===";

    QString searchText = ui->SearchText->text();
    qDebug() << "查找文本:" << searchText;

    if (searchText.isEmpty()) {
        qDebug() << "查找文本为空，显示提示";
        QMessageBox::information(this, "查找", "请输入要查找的文本。");
        ui->SearchText->setFocus();
        return;
    }

    if (!m_textEdit) {
        qDebug() << "错误：文本编辑器指针为空";
        QMessageBox::critical(this, "错误", "文本编辑器未初始化");
        return;
    }

    qDebug() << "文本编辑器有效，开始查找...";
    QTextCursor currentCursor = m_textEdit->textCursor();
    qDebug() << "当前光标位置:" << currentCursor.position();
    qDebug() << "文档总长度:" << m_textEdit->toPlainText().length();

    QTextDocument::FindFlags flags;

    // 设置查找方向
    bool searchUp = ui->rbUp->isChecked();
    if (searchUp) {
        qDebug() << "查找方向: 向上";
        flags |= QTextDocument::FindBackward;
    } else {
        qDebug() << "查找方向: 向下";
    }

    // 设置是否区分大小写
    if (ui->checkBox->isChecked()) {
        qDebug() << "区分大小写: 是";
        flags |= QTextDocument::FindCaseSensitively;
    } else {
        qDebug() << "区分大小写: 否";
    }

    bool found = false;

    // 对于向上查找，我们需要特殊处理
    if (searchUp) {
        // 向上查找的特殊逻辑
        // 首先尝试从当前位置向前查找
        found = m_textEdit->find(searchText, flags);

        if (!found) {
            qDebug() << "向上查找未找到，尝试从文档末尾重新查找";
            // 将光标移动到文档末尾
            QTextCursor cursor = m_textEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            m_textEdit->setTextCursor(cursor);

            // 再次尝试向上查找
            found = m_textEdit->find(searchText, flags);

            // 如果还是没找到，说明真的没有匹配项
            if (!found) {
                qDebug() << "向上查找仍然未找到，重置光标位置";
                // 重置光标到原始位置
                m_textEdit->setTextCursor(currentCursor);
            }
        }
    } else {
        // 向下查找的逻辑保持不变
        found = m_textEdit->find(searchText, flags);

        if (!found) {
            qDebug() << "向下查找未找到，尝试从文档开头重新查找";
            QTextCursor cursor = m_textEdit->textCursor();
            cursor.movePosition(QTextCursor::Start);
            m_textEdit->setTextCursor(cursor);
            found = m_textEdit->find(searchText, flags);
        }
    }

    qDebug() << "查找结果:" << (found ? "找到" : "未找到");

    if (found) {
        m_lastFoundPosition = m_textEdit->textCursor().position();
        qDebug() << "找到文本，新光标位置:" << m_lastFoundPosition;

        // 确保找到的文本可见
        m_textEdit->ensureCursorVisible();
        qDebug() << "已滚动到可见区域";

        // 高亮显示找到的文本
        QTextCursor cursor = m_textEdit->textCursor();
        if (cursor.hasSelection()) {
            qDebug() << "选中文本长度:" << cursor.selectedText().length();
        }
    } else {
        qDebug() << "未找到文本，显示提示信息";

        // 没找到，显示提示信息
        QMessageBox::information(this, "查找",
                                 QString("找不到 \"%1\"").arg(searchText));

        // 重置查找位置
        qDebug() << "重置查找位置";
        m_lastFoundPosition = -1;

        // 将光标恢复到原始位置
        m_textEdit->setTextCursor(currentCursor);
    }

    qDebug() << "=== 查找操作结束 ===\n";
}

void SearchDialog::btCancel_clicked()
{
    this->close();
}
