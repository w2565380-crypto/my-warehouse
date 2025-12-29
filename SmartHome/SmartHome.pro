QT       += core gui sql # 必须加这个才能连数据库
QT += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    datacollector.cpp \
    logindialog.cpp \
    logviewer.cpp \
    main.cpp \
    mainwindow.cpp \
    modeselectiondialog.cpp

HEADERS += \
    databasemanager.h \
    datacollector.h \
    logindialog.h \
    logviewer.h \
    mainwindow.h \
    modeselectiondialog.h

FORMS += \
    logindialog.ui \
    logviewer.ui \
    mainwindow.ui \
    modeselectiondialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
