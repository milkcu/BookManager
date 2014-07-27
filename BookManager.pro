#-------------------------------------------------
#
# Project created by QtCreator 2014-06-16T00:25:07
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addbookdialog.cpp \
    addreaderdialog.cpp \
    borrowbookdialog.cpp \
    returnbookdialog.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    addbookdialog.h \
    addreaderdialog.h \
    borrowbookdialog.h \
    returnbookdialog.h \
    logindialog.h

FORMS    += mainwindow.ui \
    addbookdialog.ui \
    addreaderdialog.ui \
    borrowbookdialog.ui \
    returnbookdialog.ui \
    logindialog.ui

RESOURCES += \
    res.qrc
