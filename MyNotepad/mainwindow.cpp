<<<<<<< HEAD
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include <QFontDialog>
#include <QScrollBar>
#include "codeeditor.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 新增：使用 CodeEditor 替换 textEdit ---
    CodeEditor *editor = new CodeEditor(this);
    editor->setFont(QFont("Consolas", 11));
    ui->verticalLayout->replaceWidget(ui->textEdit, editor);
    delete ui->textEdit;
    ui->textEdit = editor;
    // ----------------------------------------

    textChanged = false;
    filePath = "";
    originalWindowTitle = tr("新建文本文档-编辑器");
    this->setWindowTitle(originalWindowTitle);

    // 状态栏设置
    statusLabel.setMaximumWidth(200);
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(200);
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(tr("翁圳欣"), ui->statusbar);
    ui->statusbar->addPermanentWidget(author);

    connect(ui->textEdit, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::on_cursorPositionChanged);


    updateStatusBar();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionFind_triggered()
{
    qDebug() << "打开查找对话框";
    SearchDialog *dlg = new SearchDialog(this, ui->textEdit);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
    dlg->activateWindow();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog *dlg = new ReplaceDialog(this, ui->textEdit);
    dlg->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    dlg->show();
    dlg->activateWindow(); // 激活对话框
}

void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
    {
        return;
    }
    filePath = "";
    ui->textEdit->clear();
    originalWindowTitle = tr("新建文本文档-编辑器");
    updateWindowTitle();
    textChanged = false;
    updateStatusBar();
}

void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
    {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".",
                                                    tr("Text file (*.txt);;All(*.*)"));
    if(filename.isEmpty()) {
        return;  // 用户取消了选择
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", "打开文件失败");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    file.close();

    filePath = filename;
    originalWindowTitle = QFileInfo(filename).fileName() + " - 编辑器";
    updateWindowTitle();
    textChanged = false;
    updateStatusBar();
}

void MainWindow::on_actionSave_triggered()
{
    if(filePath.isEmpty()) {
        on_actionSaveAs_triggered();
        return;
    }

    if(saveFile(filePath)) {
        textChanged = false;
        updateWindowTitle();
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                    tr("Text file (*.txt);;All(*.*)"));
    if(filename.isEmpty()) {
        return;
    }

    if(saveFile(filename)) {
        filePath = filename;
        originalWindowTitle = QFileInfo(filename).fileName() + " - 编辑器";
        textChanged = false;
        updateWindowTitle();
    }
}

bool MainWindow::saveFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", "保存文件失败");
        return false;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
    return true;
}

void MainWindow::on_textEdit_textChanged()
{
    if(!textChanged) {
        textChanged = true;
        updateWindowTitle();
    }
    updateStatusBar();
}

void MainWindow::on_cursorPositionChanged()
{
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    // 对于 QPlainTextEdit，获取文本信息
    QString text = ui->textEdit->toPlainText();
    int length = text.length();
    int lineCount = ui->textEdit->blockCount(); // QPlainTextEdit 使用 blockCount 获取行数

    // 获取光标位置信息
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1; // 行号从1开始
    int column = cursor.positionInBlock() + 1; // 列号从1开始

    statusLabel.setText(QString("长度: %1 行数: %2").arg(length).arg(lineCount));
    statusCursorLabel.setText(QString("行: %1 列: %2").arg(line).arg(column));
}

void MainWindow::updateWindowTitle()
{
    QString title = originalWindowTitle;
    if(textChanged) {
        title = "*" + title;
    }
    this->setWindowTitle(title);
}

bool MainWindow::userEditConfirmed()
{
    if(!textChanged) {
        return true;
    }

    QString path = filePath.isEmpty() ? "无标题.txt" : QFileInfo(filePath).fileName();

    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle("保存更改");
    msg.setText(QString("是否将更改保存至 \"%1\"?").arg(path));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    int result = msg.exec();
    switch(result) {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        return !textChanged;
    case QMessageBox::No:
        textChanged = false;
        updateWindowTitle();
        return true;
    case QMessageBox::Cancel:
        return false;
    }

    return true;
}

void MainWindow::on_actionExit_triggered()
{
    // 检查是否需要保存
    if(!userEditConfirmed()) {
        return; // 用户取消了退出
    }
    QApplication::quit();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->font(), this);
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void MainWindow::on_actionShowLineNumber_triggered()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(ui->textEdit);
    if (!editor) return;

    bool currentlyVisible = editor->isLineNumberVisible();
    editor->setLineNumberVisible(!currentlyVisible);

    if (currentlyVisible) {
        QMessageBox::information(this, "行号隐藏", "已隐藏左侧行号显示。");
    } else {
        QMessageBox::information(this, "行号显示", "已显示左侧行号。");
    }
}

void MainWindow::on_actionLine_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1;
    QMessageBox::information(this, "行号信息",
                             QString("光标当前位置：第 %1 行").arg(line));
}

void MainWindow::on_actionShowToolBar_triggered()
{
    // 显示/隐藏工具栏
    if (ui->toolBar->isVisible()) {
        ui->toolBar->hide();
    } else {
        ui->toolBar->show();
    }
}

void MainWindow::on_actionShowStatusBar_triggered()
{
    // 显示/隐藏状态栏
    if (ui->statusbar->isVisible()) {
        ui->statusbar->hide();
    } else {
        ui->statusbar->show();
    }
}


=======
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include <QFontDialog>
#include <QScrollBar>
#include "codeeditor.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- 新增：使用 CodeEditor 替换 textEdit ---
    CodeEditor *editor = new CodeEditor(this);
    editor->setFont(QFont("Consolas", 11));
    ui->verticalLayout->replaceWidget(ui->textEdit, editor);
    delete ui->textEdit;
    ui->textEdit = editor;
    // ----------------------------------------

    textChanged = false;
    filePath = "";
    originalWindowTitle = tr("新建文本文档-编辑器");
    this->setWindowTitle(originalWindowTitle);

    // 状态栏设置
    statusLabel.setMaximumWidth(200);
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(200);
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(tr("翁圳欣"), ui->statusbar);
    ui->statusbar->addPermanentWidget(author);

    connect(ui->textEdit, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::on_cursorPositionChanged);


    updateStatusBar();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionFind_triggered()
{
    qDebug() << "打开查找对话框";
    SearchDialog *dlg = new SearchDialog(this, ui->textEdit);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
    dlg->activateWindow();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog *dlg = new ReplaceDialog(this, ui->textEdit);
    dlg->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    dlg->show();
    dlg->activateWindow(); // 激活对话框
}

void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
    {
        return;
    }
    filePath = "";
    ui->textEdit->clear();
    originalWindowTitle = tr("新建文本文档-编辑器");
    updateWindowTitle();
    textChanged = false;
    updateStatusBar();
}

void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
    {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".",
                                                    tr("Text file (*.txt);;All(*.*)"));
    if(filename.isEmpty()) {
        return;  // 用户取消了选择
    }

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", "打开文件失败");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    file.close();

    filePath = filename;
    originalWindowTitle = QFileInfo(filename).fileName() + " - 编辑器";
    updateWindowTitle();
    textChanged = false;
    updateStatusBar();
}

void MainWindow::on_actionSave_triggered()
{
    if(filePath.isEmpty()) {
        on_actionSaveAs_triggered();
        return;
    }

    if(saveFile(filePath)) {
        textChanged = false;
        updateWindowTitle();
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".",
                                                    tr("Text file (*.txt);;All(*.*)"));
    if(filename.isEmpty()) {
        return;
    }

    if(saveFile(filename)) {
        filePath = filename;
        originalWindowTitle = QFileInfo(filename).fileName() + " - 编辑器";
        textChanged = false;
        updateWindowTitle();
    }
}

bool MainWindow::saveFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "错误", "保存文件失败");
        return false;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
    return true;
}

void MainWindow::on_textEdit_textChanged()
{
    if(!textChanged) {
        textChanged = true;
        updateWindowTitle();
    }
    updateStatusBar();
}

void MainWindow::on_cursorPositionChanged()
{
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    // 对于 QPlainTextEdit，获取文本信息
    QString text = ui->textEdit->toPlainText();
    int length = text.length();
    int lineCount = ui->textEdit->blockCount(); // QPlainTextEdit 使用 blockCount 获取行数

    // 获取光标位置信息
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1; // 行号从1开始
    int column = cursor.positionInBlock() + 1; // 列号从1开始

    statusLabel.setText(QString("长度: %1 行数: %2").arg(length).arg(lineCount));
    statusCursorLabel.setText(QString("行: %1 列: %2").arg(line).arg(column));
}

void MainWindow::updateWindowTitle()
{
    QString title = originalWindowTitle;
    if(textChanged) {
        title = "*" + title;
    }
    this->setWindowTitle(title);
}

bool MainWindow::userEditConfirmed()
{
    if(!textChanged) {
        return true;
    }

    QString path = filePath.isEmpty() ? "无标题.txt" : QFileInfo(filePath).fileName();

    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle("保存更改");
    msg.setText(QString("是否将更改保存至 \"%1\"?").arg(path));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    int result = msg.exec();
    switch(result) {
    case QMessageBox::Yes:
        on_actionSave_triggered();
        return !textChanged;
    case QMessageBox::No:
        textChanged = false;
        updateWindowTitle();
        return true;
    case QMessageBox::Cancel:
        return false;
    }

    return true;
}

void MainWindow::on_actionExit_triggered()
{
    // 检查是否需要保存
    if(!userEditConfirmed()) {
        return; // 用户取消了退出
    }
    QApplication::quit();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->font(), this);
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void MainWindow::on_actionShowLineNumber_triggered()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(ui->textEdit);
    if (!editor) return;

    bool currentlyVisible = editor->isLineNumberVisible();
    editor->setLineNumberVisible(!currentlyVisible);

    if (currentlyVisible) {
        QMessageBox::information(this, "行号隐藏", "已隐藏左侧行号显示。");
    } else {
        QMessageBox::information(this, "行号显示", "已显示左侧行号。");
    }
}

void MainWindow::on_actionLine_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int line = cursor.blockNumber() + 1;
    QMessageBox::information(this, "行号信息",
                             QString("光标当前位置：第 %1 行").arg(line));
}

void MainWindow::on_actionShowToolBar_triggered()
{
    // 显示/隐藏工具栏
    if (ui->toolBar->isVisible()) {
        ui->toolBar->hide();
    } else {
        ui->toolBar->show();
    }
}

void MainWindow::on_actionShowStatusBar_triggered()
{
    // 显示/隐藏状态栏
    if (ui->statusbar->isVisible()) {
        ui->statusbar->hide();
    } else {
        ui->statusbar->show();
    }
}


>>>>>>> 28edf83d02aa91b9b4972741e4c235380ab33ab4
