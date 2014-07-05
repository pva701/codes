#ifndef CHATCLIENTWIDGET_H
#define CHATCLIENTWIDGET_H

#include <QtGui/QApplication>
#include <QString>
#include "authenticationform.h"
#include "chatclient.h"

class ChatClientWidget:public QObject
{
    Q_OBJECT

    const QString& host;
    int port;
    AuthenticationForm *autFrm;
    ChatClient *server;
public:
    ChatClientWidget(const QString& hosty, int porty);
    void start();
private slots:
    void slotAuthenticated(int, QString);
};

#endif // CHATCLIENTWIDGET_H
