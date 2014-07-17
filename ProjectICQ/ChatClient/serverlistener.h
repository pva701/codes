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
private:
    QTcpSocket *pSocket;
    quint16 sizeOfBlock;
    bool fullStreamRecieved;
public:
    ServerListener(QTcpSocket *socket);
    bool waitFullStream(int mills);
private slots:
    void slotReadServer();
signals:
    void tryRegister(quint16 userId);
    void tryAuthenticate(quint16 status, quint16 userId, const QString& userLogin, const QString& msg);
    void messageRecieved(quint16 dg, quint16 from, QDateTime sendTime, QString content);
    void youAddedInUserlist(quint16 frId, const QString& pseud);
    void userlistRecieved(const QVector <User>& us);
    void historyRecieved(const QVector <Message>& hs);
    void userAddedById(quint16 frId, quint16 dialog, const QString& pseud, bool status, bool isOn);
    void userAddedByLogin(quint16 frId, quint16 dialog, const QString& pseud, bool status, bool isOn);
    void foundUser(quint16 userId, const QString& pseud, bool isOn);
    void notifyOnOff(quint16 userId, bool stat);
};

#endif // SERVERLISTENER_H
