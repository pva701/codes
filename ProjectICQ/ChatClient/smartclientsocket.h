#ifndef SMARTCLIENTSOCKET_H
#define SMARTCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QVector>
#include <QString>

#include "serverlistener.h"
#include "servertalker.h"

class SmartClientSocket:public QObject
{
    Q_OBJECT
private:
    static const int INTERVAL_LOAD_USERLIST = 2000;//ms
    static const int INTERVAL_ADD_USER = 2000;
    static const int INTERVAL_LOAD_HISTORY = 2000;

    User fakeUser;
    QVector <Message*> fakeHistory;
    QVector <User*> fakeUserlist;
    ServerListener *listenerx;
    ServerTalker *talkerx;
    QTcpSocket *socketx;
public:
    SmartClientSocket(QTcpSocket *sock);

    QVector <User*> loadUserlist(quint16 myId);
    QString addUserById(quint16 mid, quint16 fid, bool status);
    User addUserByLogin(quint16 myId, const QString& log, bool status);
    QVector <Message*> loadHistory(Dialog *dg);
    QTcpSocket *socket();
    ServerListener* listener();
    ServerTalker* talker();
private slots:
    void slotUserlistRecieved(const QVector <User*>& us);
    void slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status);
    void slotHistoryRecieved(const QVector<Message*> &hs);
};

#endif // SMARTSOCKET_H
