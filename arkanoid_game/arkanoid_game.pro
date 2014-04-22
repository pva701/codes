#-------------------------------------------------
#
# Project created by QtCreator 2013-08-18T21:56:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = arkanoid_game
TEMPLATE = app

SOURCES += main.cpp\
    Interface.cpp \
    Balls.cpp \
    Platform.cpp \
    Blocks.cpp

HEADERS  += \
    Interface.h \
    Balls.h \
    Platform.h \
    Blocks.h \
    Constants.h
RESOURCES += res.qrc

