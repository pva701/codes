#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T16:14:39
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

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
