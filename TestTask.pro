#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T17:57:09
#
#-------------------------------------------------

QT       += core gui network webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestTask
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networkwrapper.cpp \
    json.cpp

HEADERS  += mainwindow.h \
    networkwrapper.h \
    json.h

FORMS    += mainwindow.ui

