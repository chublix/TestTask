#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T17:57:09
#
#-------------------------------------------------

QT       += core gui network

TARGET = TestTask
TEMPLATE = app


SOURCES += main.cpp\
    networkwrapper.cpp \
    json.cpp \
    vlcwidget.cpp \
    form.cpp

HEADERS  += \
    networkwrapper.h \
    json.h \
    vlcwidget.h \
    form.h

FORMS    += \
    form.ui

LIBS += -lvlc
