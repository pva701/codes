#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T14:23:15
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = ChatServer
TEMPLATE = app


SOURCES += \
    main.cpp \
    chatserver.cpp \
    ../common/bytesreaderwriter.cpp \
    usersonline.cpp

HEADERS  += \
    chatserver.h \
    ../common/bytesreaderwriter.h \
    ../common/serverflags.h \
    ../common/servercommands.h \
    usersonline.h

FORMS    +=
