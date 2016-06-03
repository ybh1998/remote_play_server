#-------------------------------------------------
#
# Project created by QtCreator 2015-08-03T08:35:06
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = remote_play_linux
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    background.cpp

HEADERS += \
    ../include/defines.h \
    server.h \
    background.h
LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lswscale -lavutil -lxdo

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

