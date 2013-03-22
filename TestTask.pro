#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T17:57:09
#
#-------------------------------------------------

QT       += core gui network widgets

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

win32: INCLUDEPATH += $$PWD/vlc
win32: DEPENDPATH += $$PWD/vlc

#win32: LIBS += $$PWD/libvlccore.lib
win32: LIBS += $$PWD/libvlc.lib

unix: LIBS +=-llibvlc
unix: INCLUDEPATH += /usr/include
unix: DEPENDPATH += /usr/include




