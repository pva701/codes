#ifndef CHATCLIENTFORM_H
#define CHATCLIENTFORM_H

#include <QtGui/QApplication>
#include <QString>
#include "authenticationform.h"
#include "chatclient.h"
#include "clientsocket.h"

class ChatClientForm:public QObject
{
    Q_OBJECT

    const QString& host;
    int port;
    AuthenticationForm *autFrm;
    ChatClient *chatClient;
    QTcpSocket *socket;
    ClientSocket *clSocket;
public:
    ChatClientForm(const QString& hosty, int porty);
    void start();
    ~ChatClientForm();
private slots:
    void slotAuthenticated(quint16, QString);
};

#endif // CHATCLIENTWIDGET_H
