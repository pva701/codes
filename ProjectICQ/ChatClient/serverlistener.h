#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H

#include <QTcpSocket>
#include <QString>
#include <QObject>

#include "../common/bytesreaderwriter.h"
#include "../common/servercommands.h"
#include "user.h"
#include "message.h"

class ServerListener:public QObject
{
    Q_OBJECT
    QTcpSocket *pSocket;
    quint16 sizeOfBlock;
    bool fullStreamRecieved;
public:
    ServerListener(QTcpSocket *socket);
    bool waitFullStream(int mills);
private slots:
    void slotReadServer();
signals:
    void messageRecieved(quint16 dg, quint16 from, QDateTime sendTime, QString content);
    void youAddedInUserlist(quint16);
    void userlistRecieved(const QVector <User*>& us);
    void historyRecieved(const QVector <Message*>& hs);
    void userAdded(quint16 frId, quint16 dialog, const QString& pseud, bool);
};

#endif // SERVERLISTENER_H
