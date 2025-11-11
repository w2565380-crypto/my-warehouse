<<<<<<< HEAD
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs={
        {Qt::Key_0,ui->btnNum0},
        {Qt::Key_1,ui->btnNum1},
        {Qt::Key_2,ui->btnNum2},
        {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9},
        };
    foreach(auto btn, digitBTNs)
    connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivision,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//双操作符处理
QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    bool success = false;

    qDebug() << "=== Starting Calculation ===";
    qDebug() << "Operates stack before:" << operates;
    qDebug() << "Opcodes stack before:" << opcodes;

    if (operates.size() >= 2 && opcodes.size() >= 1) {
        double operate1 = operates.pop().toDouble();
        double operate2 = operates.pop().toDouble();
        QString op = opcodes.pop();

        qDebug() << "Popped operands: operate2 =" << operate2 << "(first input), operate1 =" << operate1 << "(second input)";
        qDebug() << "Operator:" << op;
        qDebug() << "Calculating:" << operate2 << op << operate1;

        if (op == "+") {
            result = operate2 + operate1;
            success = true;
        } else if (op == "-") {
            result = operate2 - operate1;
            success = true;
        } else if (op == "×") {
            result = operate2 * operate1;
            success = true;
        } else if (op == "÷") {
            if (operate1 != 0) {
                result = operate2 / operate1;
                success = true;
            } else {
                ui->display->setText("Error: Division by zero");
                operates.clear();
                opcodes.clear();
                if (ok) *ok = false;
                return "Error";
            }
        }

        qDebug() << "Calculation result:" << result;
        ui->statusbar->showMessage(
            QString("Calculation completed. Result: %1").arg(result));
    }

    qDebug() << "=== Calculation Finished ===";
    if (ok) *ok = success;
    return QString::number(result);
}

//按键事件处理
void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton *>(sender())->text();
    if(digit=="0" && operate=="0")
        digit="";
    if(operate=="0"&&digit!="0")
        operate="";
    operate+=digit;
    ui->display->setText(operate);
    //ui->statusbar->showMessage("clicked");
}


//小数点处理
void MainWindow::on_btnPoints_clicked()
{
    if(!operate.contains("."))
        operate+=qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operate);
}

//退格操作
void MainWindow::on_btnDel_clicked()
{
    operate=operate.left(operate.length()-1);
    ui->display->setText(operate);
}

//清除操作
void MainWindow::on_btnClear_clicked()
{
    operate.clear();
    operates.clear();
    opcodes.clear();
    ui->display->setText(operate);
}

//操作数操作符提示
void MainWindow::btnBinaryOperatorClicked()
{
    QString newOpcode = qobject_cast<QPushButton *>(sender())->text();

    qDebug() << "=== Before Binary Operator ===";
    qDebug() << "Operate:" << operate;
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    if(operate != "")
    {
        operates.push_back(operate);
        operate = "";

        qDebug() << "Pushed operate to stack, new operates:" << operates;
    }

    if(operates.size() >= 2 && opcodes.size() >= 1)
    {
        QString result = calculation();
        operates.push_back(result);
        qDebug() << "After calculation, operates:" << operates;
    }

    opcodes.push_back(newOpcode);
    qDebug() << "After pushing opcode, opcodes:" << opcodes;

    if(!operates.isEmpty()) {
        ui->display->setText(operates.last());
    }

    qDebug() << "=== After Binary Operator ===";
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    ui->statusbar->showMessage(QString("Operates: %1, Opcodes: %2").arg(operates.size()).arg(opcodes.size()));
}

void MainWindow::on_btnEqual_clicked()
{
    if(operate != "")
    {
        operates.push_back(operate);
        operate = "";
    }

    while(operates.size() >= 2 && opcodes.size() >= 1) {
        QString result = calculation();
        operates.push_back(result);
    }

    if(!operates.isEmpty()) {
        ui->display->setText(operates.last());
    }
}

//单操作符处理
void MainWindow::btnUnaryOperatorClicked()
{
    if(operate!=""){
        double result=operate.toDouble();
        operate="";

        QString op=qobject_cast<QPushButton*>(sender())->text();
        if(op=="%")
            result=result/100.0;
        else if(op=="1/x")
            result=1/result;
        else if(op=="x^2")
            result=result*result;
        else if(op=="2√x"){
            if(result==0){
                ui->display->setText("error");
                operates.clear();
                opcodes.clear();
            }
            else
                result=sqrt(result);
        }
        ui->display->setText(QString::number(result));
    }
}

//键盘事件点击
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey, digitBTNs.keys()){
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }
    digitBTNs[Qt::Key_Plus] = ui->btnAdd;
    digitBTNs[Qt::Key_Minus] = ui->btnMinus;
    digitBTNs[Qt::Key_Asterisk] = ui->btnMultiple;
    digitBTNs[Qt::Key_Slash] = ui->btnDivision;
    digitBTNs[Qt::Key_Enter] = ui->btnEqual;
    digitBTNs[Qt::Key_Return] = ui->btnEqual;
    digitBTNs[Qt::Key_Equal] = ui->btnEqual;
    digitBTNs[Qt::Key_Period] = ui->btnPoints;
    digitBTNs[Qt::Key_Comma] = ui->btnPoints;
    digitBTNs[Qt::Key_Backspace] = ui->btnDel;
    digitBTNs[Qt::Key_Delete] = ui->btnClear;
    digitBTNs[Qt::Key_Escape] = ui->btnClear;
}




void MainWindow::on_btnInverse_clicked()
{
    QString currentText = ui->display->text().trimmed();

    if (currentText.isEmpty() || currentText == "0") {
        return;
    }

    QString newText;
    if (currentText.startsWith('-')) {
        newText = currentText.mid(1);
    } else {
        newText = "-" + currentText;
    }

    // 更新显示
    ui->display->setText(newText);

    // 更新当前操作数
    operate = newText;

    qDebug() << "=== After Inverse Click ===";
    qDebug() << "Display:" << newText;
    qDebug() << "Operate variable:" << operate;
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    // 关键修复：如果操作数栈不为空，更新栈顶的值
    // 因为用户可能在输入数字后立即点击正负号，此时数字已经在栈中
    if (!operates.isEmpty()) {
        // 检查栈顶是否与当前文本（修改前）匹配
        if (operates.last() == currentText) {
            operates.last() = newText;
            qDebug() << "Updated stack top from" << currentText << "to" << newText;
        }
    }

    qDebug() << "Operates stack after update:" << operates;
}




void MainWindow::on_btnClearEntry_clicked()
{
    operate = "";
    ui->display->setText("0");

    qDebug() << "CE clicked - Current entry cleared";
    qDebug() << "Operate variable reset to empty";
}

=======
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs={
        {Qt::Key_0,ui->btnNum0},
        {Qt::Key_1,ui->btnNum1},
        {Qt::Key_2,ui->btnNum2},
        {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9},
        };
    foreach(auto btn, digitBTNs)
    connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivision,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    bool success = false;

    qDebug() << "=== Starting Calculation ===";
    qDebug() << "Operates stack before:" << operates;
    qDebug() << "Opcodes stack before:" << opcodes;

    if (operates.size() >= 2 && opcodes.size() >= 1) {
        double operate1 = operates.pop().toDouble();
        double operate2 = operates.pop().toDouble();
        QString op = opcodes.pop();

        qDebug() << "Popped operands: operate2 =" << operate2 << "(first input), operate1 =" << operate1 << "(second input)";
        qDebug() << "Operator:" << op;
        qDebug() << "Calculating:" << operate2 << op << operate1;

        if (op == "+") {
            result = operate2 + operate1;
            success = true;
        } else if (op == "-") {
            result = operate2 - operate1;
            success = true;
        } else if (op == "×") {
            result = operate2 * operate1;
            success = true;
        } else if (op == "÷") {
            if (operate1 != 0) {
                result = operate2 / operate1;
                success = true;
            } else {
                ui->display->setText("Error: Division by zero");
                operates.clear();
                opcodes.clear();
                if (ok) *ok = false;
                return "Error";
            }
        }

        qDebug() << "Calculation result:" << result;
        ui->statusbar->showMessage(
            QString("Calculation completed. Result: %1").arg(result));
    }

    qDebug() << "=== Calculation Finished ===";
    if (ok) *ok = success;
    return QString::number(result);
}


void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton *>(sender())->text();
    if(digit=="0" && operate=="0")
        digit="";
    if(operate=="0"&&digit!="0")
        operate="";
    operate+=digit;
    ui->display->setText(operate);
    //ui->statusbar->showMessage("clicked");
}



void MainWindow::on_btnPoints_clicked()
{
    if(!operate.contains("."))
        operate+=qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operate);
}


void MainWindow::on_btnDel_clicked()
{
    operate=operate.left(operate.length()-1);
    ui->display->setText(operate);
}


void MainWindow::on_btnClear_clicked()
{
    operate.clear();
    operates.clear();
    opcodes.clear();
    ui->display->setText(operate);
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString newOpcode = qobject_cast<QPushButton *>(sender())->text();

    qDebug() << "=== Before Binary Operator ===";
    qDebug() << "Operate:" << operate;
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    if(operate != "")
    {
        operates.push_back(operate);
        operate = "";

        qDebug() << "Pushed operate to stack, new operates:" << operates;
    }

    if(operates.size() >= 2 && opcodes.size() >= 1)
    {
        QString result = calculation();
        operates.push_back(result);
        qDebug() << "After calculation, operates:" << operates;
    }

    opcodes.push_back(newOpcode);
    qDebug() << "After pushing opcode, opcodes:" << opcodes;

    if(!operates.isEmpty()) {
        ui->display->setText(operates.last());
    }

    qDebug() << "=== After Binary Operator ===";
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    ui->statusbar->showMessage(QString("Operates: %1, Opcodes: %2").arg(operates.size()).arg(opcodes.size()));
}

void MainWindow::on_btnEqual_clicked()
{
    if(operate != "")
    {
        operates.push_back(operate);
        operate = "";
    }

    while(operates.size() >= 2 && opcodes.size() >= 1) {
        QString result = calculation();
        operates.push_back(result);
    }

    if(!operates.isEmpty()) {
        ui->display->setText(operates.last());
    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operate!=""){
        double result=operate.toDouble();
        operate="";

        QString op=qobject_cast<QPushButton*>(sender())->text();
        if(op=="%")
            result=result/100.0;
        else if(op=="1/x")
            result=1/result;
        else if(op=="x^2")
            result=result*result;
        else if(op=="2√x"){
            if(result==0){
                ui->display->setText("error");
                operates.clear();
                opcodes.clear();
            }
            else
                result=sqrt(result);
        }
        ui->display->setText(QString::number(result));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey, digitBTNs.keys()){
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }
    digitBTNs[Qt::Key_Plus] = ui->btnAdd;
    digitBTNs[Qt::Key_Minus] = ui->btnMinus;
    digitBTNs[Qt::Key_Asterisk] = ui->btnMultiple;
    digitBTNs[Qt::Key_Slash] = ui->btnDivision;
    digitBTNs[Qt::Key_Enter] = ui->btnEqual;
    digitBTNs[Qt::Key_Return] = ui->btnEqual;
    digitBTNs[Qt::Key_Equal] = ui->btnEqual;
    digitBTNs[Qt::Key_Period] = ui->btnPoints;
    digitBTNs[Qt::Key_Comma] = ui->btnPoints;
    digitBTNs[Qt::Key_Backspace] = ui->btnDel;
    digitBTNs[Qt::Key_Delete] = ui->btnClear;
    digitBTNs[Qt::Key_Escape] = ui->btnClear;
}




void MainWindow::on_btnInverse_clicked()
{
    QString currentText = ui->display->text().trimmed();

    if (currentText.isEmpty() || currentText == "0") {
        return;
    }

    QString newText;
    if (currentText.startsWith('-')) {
        newText = currentText.mid(1);
    } else {
        newText = "-" + currentText;
    }

    // 更新显示
    ui->display->setText(newText);

    // 更新当前操作数
    operate = newText;

    qDebug() << "=== After Inverse Click ===";
    qDebug() << "Display:" << newText;
    qDebug() << "Operate variable:" << operate;
    qDebug() << "Operates stack:" << operates;
    qDebug() << "Opcodes stack:" << opcodes;

    // 关键修复：如果操作数栈不为空，更新栈顶的值
    // 因为用户可能在输入数字后立即点击正负号，此时数字已经在栈中
    if (!operates.isEmpty()) {
        // 检查栈顶是否与当前文本（修改前）匹配
        if (operates.last() == currentText) {
            operates.last() = newText;
            qDebug() << "Updated stack top from" << currentText << "to" << newText;
        }
    }

    qDebug() << "Operates stack after update:" << operates;
}




void MainWindow::on_btnClearEntry_clicked()
{
    operate = "";
    ui->display->setText("0");

    qDebug() << "CE clicked - Current entry cleared";
    qDebug() << "Operate variable reset to empty";
}

>>>>>>> 28edf83d02aa91b9b4972741e4c235380ab33ab4
