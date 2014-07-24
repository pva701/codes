#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H

#include <QTcpSocket>
#include <QString>
#include <QDateTime>
#include "../common/bytesreaderwriter.h"
#include "../common/servercommands.h"
#include "user.h"
#include "message.h"
#include "../common/serverflags.h"
#include "notification.h"

class ServerListener:public QObject
{
    Q_OBJECT
private:
    QTcpSocket *pSocket;
    quint16 sizeOfBlock;
    bool fullStreamReceived;
public:
    ServerListener(QTcpSocket *socket);
    bool waitFullStream(int mills);
private slots:
    void slotReadServer();
signals:
    void tryRegister(quint16 userId);
    void tryAuthenticate(quint16 status, quint16 userId, const QString& userLogin, const QString& msg);
    void messageReceived(quint16 dg, quint16 from, QDateTime sendTime, QString content);
    void youAddedInUserlist(quint16 frId, const QString& pseud);
    void userlistReceived(const QVector <User*>& us);
    void historyReceived(const QVector <Message>& hs);
    void userAdded(quint16 frId, quint16 dialog, const QString& pseud, int status, bool isOn);
    //void userAddedByLogin(quint16 frId, quint16 dialog, const QString& pseud, int status, bool isOn);
    void foundUser(quint16 userId, const QString& pseud, bool isOn);
    void notifyOnOff(quint16 userId, bool stat);
    void notifysReceived(const QVector<Notification>& nf);
    void dateTimeFromServer(QDateTime);
    void readAllYouMessageNotify(int dialogId);
};

#endif // SERVERLISTENER_H
