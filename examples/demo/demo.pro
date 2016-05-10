#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T14:54:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../../src/
LIBS += -L$$OUT_PWD/../../src/debug/ -liot

