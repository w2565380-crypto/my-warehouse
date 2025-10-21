/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *btnNum1;
    QPushButton *btnNum4;
    QPushButton *btnEqual;
    QPushButton *btnNum0;
    QPushButton *btnDivide;
    QPushButton *btnPercentage;
    QPushButton *btnDel;
    QPushButton *btnNum2;
    QPushButton *btnSquare;
    QPushButton *btnInverse;
    QPushButton *btnNum6;
    QPushButton *btnDivision;
    QPushButton *btnNum9;
    QPushButton *btnClearEntry;
    QPushButton *btnNum8;
    QPushButton *btnNum5;
    QPushButton *btnMultiple;
    QPushButton *btnNum7;
    QPushButton *btnMinus;
    QPushButton *btnAdd;
    QPushButton *btnClear;
    QPushButton *btnNum3;
    QPushButton *btnPoints;
    QPushButton *btnSqrt;
    QLineEdit *display;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(414, 359);
        MainWindow->setStyleSheet(QString::fromUtf8("/* \350\256\276\347\275\256\344\270\273\347\252\227\345\217\243\350\203\214\346\231\257 */\n"
"MainWindow {\n"
"    background-color: #f0f0f0;\n"
"}\n"
"\n"
"/* \346\230\276\347\244\272\346\241\206\357\274\210QLineEdit\357\274\211\347\232\204\346\240\267\345\274\217 */\n"
"QLineEdit {\n"
"    background-color: rgb(245, 245, 245); /* \346\265\205\347\201\260\350\211\262\350\203\214\346\231\257 */\n"
"    border: 0px groove rgb(243, 243, 243); /* \346\227\240\350\276\271\346\241\206\346\210\226\346\236\201\347\256\200\350\276\271\346\241\206 */\n"
"    border-radius: 5px; /* \345\234\206\350\247\222 */\n"
"    font: bold 22px 'Arial'; /* \345\212\240\347\262\227\345\255\227\344\275\223 - \344\273\21618px\350\260\203\346\225\264\345\210\26022px */\n"
"    padding: 5px; /* \345\206\205\350\276\271\350\267\235 */\n"
"    color: #333333; /* \346\267\261\347\201\260\350\211\262\345\255\227\344\275\223 */\n"
"}\n"
"\n"
"/* \346\231\256\351\200\232\346\214\211\351\222\256\347\232\204\345\237\272\347\241\200\346\240\267"
                        "\345\274\217 */\n"
"QPushButton {\n"
"    background-color: rgb(243, 243, 243); /* \346\265\205\347\201\260\350\211\262\350\203\214\346\231\257 */\n"
"    border: 1px groove rgb(220, 220, 220); /* \346\265\205\347\201\260\350\211\262\350\276\271\346\241\206 */\n"
"    border-radius: 5px; /* \345\234\206\350\247\222 */\n"
"    font: 16px 'Arial'; /* \345\255\227\344\275\223 - \344\273\21611px\350\260\203\346\225\264\345\210\26016px */\n"
"    min-width: 35px;\n"
"    min-height: 38px;\n"
"}\n"
"\n"
"/* \346\214\211\351\222\256\347\232\204\351\274\240\346\240\207\346\202\254\345\201\234\346\225\210\346\236\234 */\n"
"QPushButton:hover {\n"
"    background-color: rgb(193, 193, 193); /* \346\202\254\345\201\234\346\227\266\351\242\234\350\211\262\345\212\240\346\267\261 */\n"
"}\n"
"\n"
"/* \346\214\211\351\222\256\347\232\204\346\214\211\344\270\213\346\225\210\346\236\234 */\n"
"QPushButton:pressed {\n"
"    background-color: #CD6090; /* \346\214\211\344\270\213\346\227\266\351\242\234\350\211\262\346\233\264\346"
                        "\267\261 */\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        btnNum1 = new QPushButton(centralwidget);
        btnNum1->setObjectName("btnNum1");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnNum1->sizePolicy().hasHeightForWidth());
        btnNum1->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum1, 5, 0, 1, 1);

        btnNum4 = new QPushButton(centralwidget);
        btnNum4->setObjectName("btnNum4");
        sizePolicy1.setHeightForWidth(btnNum4->sizePolicy().hasHeightForWidth());
        btnNum4->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum4, 4, 0, 1, 1);

        btnEqual = new QPushButton(centralwidget);
        btnEqual->setObjectName("btnEqual");
        sizePolicy1.setHeightForWidth(btnEqual->sizePolicy().hasHeightForWidth());
        btnEqual->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnEqual, 6, 3, 1, 1);

        btnNum0 = new QPushButton(centralwidget);
        btnNum0->setObjectName("btnNum0");
        sizePolicy1.setHeightForWidth(btnNum0->sizePolicy().hasHeightForWidth());
        btnNum0->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum0, 6, 1, 1, 1);

        btnDivide = new QPushButton(centralwidget);
        btnDivide->setObjectName("btnDivide");
        sizePolicy1.setHeightForWidth(btnDivide->sizePolicy().hasHeightForWidth());
        btnDivide->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnDivide, 2, 0, 1, 1);

        btnPercentage = new QPushButton(centralwidget);
        btnPercentage->setObjectName("btnPercentage");
        sizePolicy1.setHeightForWidth(btnPercentage->sizePolicy().hasHeightForWidth());
        btnPercentage->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnPercentage, 1, 0, 1, 1);

        btnDel = new QPushButton(centralwidget);
        btnDel->setObjectName("btnDel");
        sizePolicy1.setHeightForWidth(btnDel->sizePolicy().hasHeightForWidth());
        btnDel->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnDel, 1, 3, 1, 1);

        btnNum2 = new QPushButton(centralwidget);
        btnNum2->setObjectName("btnNum2");
        sizePolicy1.setHeightForWidth(btnNum2->sizePolicy().hasHeightForWidth());
        btnNum2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum2, 5, 1, 1, 1);

        btnSquare = new QPushButton(centralwidget);
        btnSquare->setObjectName("btnSquare");
        sizePolicy1.setHeightForWidth(btnSquare->sizePolicy().hasHeightForWidth());
        btnSquare->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnSquare, 2, 1, 1, 1);

        btnInverse = new QPushButton(centralwidget);
        btnInverse->setObjectName("btnInverse");
        sizePolicy1.setHeightForWidth(btnInverse->sizePolicy().hasHeightForWidth());
        btnInverse->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnInverse, 6, 0, 1, 1);

        btnNum6 = new QPushButton(centralwidget);
        btnNum6->setObjectName("btnNum6");
        sizePolicy1.setHeightForWidth(btnNum6->sizePolicy().hasHeightForWidth());
        btnNum6->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum6, 4, 2, 1, 1);

        btnDivision = new QPushButton(centralwidget);
        btnDivision->setObjectName("btnDivision");
        sizePolicy1.setHeightForWidth(btnDivision->sizePolicy().hasHeightForWidth());
        btnDivision->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnDivision, 2, 3, 1, 1);

        btnNum9 = new QPushButton(centralwidget);
        btnNum9->setObjectName("btnNum9");
        sizePolicy1.setHeightForWidth(btnNum9->sizePolicy().hasHeightForWidth());
        btnNum9->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum9, 3, 2, 1, 1);

        btnClearEntry = new QPushButton(centralwidget);
        btnClearEntry->setObjectName("btnClearEntry");
        sizePolicy1.setHeightForWidth(btnClearEntry->sizePolicy().hasHeightForWidth());
        btnClearEntry->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnClearEntry, 1, 1, 1, 1);

        btnNum8 = new QPushButton(centralwidget);
        btnNum8->setObjectName("btnNum8");
        sizePolicy1.setHeightForWidth(btnNum8->sizePolicy().hasHeightForWidth());
        btnNum8->setSizePolicy(sizePolicy1);
        btnNum8->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(btnNum8, 3, 1, 1, 1);

        btnNum5 = new QPushButton(centralwidget);
        btnNum5->setObjectName("btnNum5");
        sizePolicy1.setHeightForWidth(btnNum5->sizePolicy().hasHeightForWidth());
        btnNum5->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum5, 4, 1, 1, 1);

        btnMultiple = new QPushButton(centralwidget);
        btnMultiple->setObjectName("btnMultiple");
        sizePolicy1.setHeightForWidth(btnMultiple->sizePolicy().hasHeightForWidth());
        btnMultiple->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnMultiple, 3, 3, 1, 1);

        btnNum7 = new QPushButton(centralwidget);
        btnNum7->setObjectName("btnNum7");
        sizePolicy1.setHeightForWidth(btnNum7->sizePolicy().hasHeightForWidth());
        btnNum7->setSizePolicy(sizePolicy1);
        btnNum7->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(btnNum7, 3, 0, 1, 1);

        btnMinus = new QPushButton(centralwidget);
        btnMinus->setObjectName("btnMinus");
        sizePolicy1.setHeightForWidth(btnMinus->sizePolicy().hasHeightForWidth());
        btnMinus->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnMinus, 4, 3, 1, 1);

        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName("btnAdd");
        sizePolicy1.setHeightForWidth(btnAdd->sizePolicy().hasHeightForWidth());
        btnAdd->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnAdd, 5, 3, 1, 1);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");
        sizePolicy1.setHeightForWidth(btnClear->sizePolicy().hasHeightForWidth());
        btnClear->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnClear, 1, 2, 1, 1);

        btnNum3 = new QPushButton(centralwidget);
        btnNum3->setObjectName("btnNum3");
        sizePolicy1.setHeightForWidth(btnNum3->sizePolicy().hasHeightForWidth());
        btnNum3->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnNum3, 5, 2, 1, 1);

        btnPoints = new QPushButton(centralwidget);
        btnPoints->setObjectName("btnPoints");
        sizePolicy1.setHeightForWidth(btnPoints->sizePolicy().hasHeightForWidth());
        btnPoints->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnPoints, 6, 2, 1, 1);

        btnSqrt = new QPushButton(centralwidget);
        btnSqrt->setObjectName("btnSqrt");
        sizePolicy1.setHeightForWidth(btnSqrt->sizePolicy().hasHeightForWidth());
        btnSqrt->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(btnSqrt, 2, 2, 1, 1);

        display = new QLineEdit(centralwidget);
        display->setObjectName("display");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(display->sizePolicy().hasHeightForWidth());
        display->setSizePolicy(sizePolicy2);
        display->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout->addWidget(display, 0, 0, 1, 4);


        verticalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 414, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\345\231\250-\347\277\201\345\234\263\346\254\243-2023414290231", nullptr));
        btnNum1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        btnNum4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        btnEqual->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        btnNum0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        btnDivide->setText(QCoreApplication::translate("MainWindow", "1/x", nullptr));
        btnPercentage->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        btnDel->setText(QCoreApplication::translate("MainWindow", "\342\214\253", nullptr));
        btnNum2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        btnSquare->setText(QCoreApplication::translate("MainWindow", "x^2", nullptr));
        btnInverse->setText(QCoreApplication::translate("MainWindow", "+/-", nullptr));
        btnNum6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        btnDivision->setText(QCoreApplication::translate("MainWindow", "\303\267", nullptr));
        btnNum9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        btnClearEntry->setText(QCoreApplication::translate("MainWindow", "CE", nullptr));
        btnNum8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        btnNum5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        btnMultiple->setText(QCoreApplication::translate("MainWindow", "\303\227", nullptr));
        btnNum7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        btnMinus->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        btnAdd->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        btnNum3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        btnPoints->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        btnSqrt->setText(QCoreApplication::translate("MainWindow", "2\342\210\232x", nullptr));
        display->setPlaceholderText(QCoreApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
