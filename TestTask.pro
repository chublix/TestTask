#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T17:57:09
#
#-------------------------------------------------

QT       += core gui network widgets x11

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

win32: LIBS += $$PWD/libvlc.lib
win32: INCLUDEPATH += $$PWD/vlc2
win32: DEPENDPATH += $$PWD/vlc2

#unix: LIBS += -L$$PWD/libvlc.a


unix: LIBS += -lvlc

unix: INCLUDEPATH += $$PWD/vlc1
unix: DEPENDPATH += $$PWD/vlc1

unix: PRE_TARGETDEPS += $$PWD/libvlc.a
