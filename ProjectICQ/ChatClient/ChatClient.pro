#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T14:18:10
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = ChatClient
TEMPLATE = app


SOURCES += \
    user.cpp \
    main.cpp \
    chatclient.cpp \
    authenticationform.cpp \
    ../common/bytesreaderwriter.cpp \
    dialog.cpp \
    serverlistener.cpp \
    servertalker.cpp \
    message.cpp \
    clientsocket.cpp \
    registrationform.cpp \
    chatclientform.cpp \
    dialogfindfriends.cpp \
    notification.cpp \
    userlistwidget.cpp

HEADERS  += \
    user.h \
    dialog.h \
    chatclient.h \
    authenticationform.h \
    ../common/bytesreaderwriter.h \
    ../common/serverflags.h \
    ../common/servercommands.h \
    serverlistener.h \
    servertalker.h \
    message.h \
    clientsocket.h \
    registrationform.h \
    chatclientform.h \
    dialogfindfriends.h \
    notification.h \
    userlistwidget.h

