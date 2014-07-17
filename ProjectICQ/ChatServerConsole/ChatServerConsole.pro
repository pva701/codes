#-------------------------------------------------
#
# Project created by QtCreator 2014-07-14T21:20:03
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

#QTPLUGIN += qsqlmysql
#QTPLUGIN += /usr/lib/x86_64-linux-gnu/qt4/plugins/sqldrivers/libqsqlmysql.so
QTPLUGIN += qsqlmysql

TARGET = ChatServerConsole
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    usersonline.cpp \
    main.cpp \
    chatserver.cpp \
    ../common/bytesreaderwriter.cpp

HEADERS += \
    usersonline.h \
    chatserver.h \
    ../common/serverflags.h \
    ../common/servercommands.h \
    ../common/bytesreaderwriter.h
