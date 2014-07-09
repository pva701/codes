#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QString>
#include "serverlistener.h"
#include "servertalker.h"

class ClientSocket:public QObject
{
    Q_OBJECT
private:
    static const int INTERVAL_AUTH = 30000;
    static const int INTERVAL_LOAD_USERLIST = 2000;//ms
    static const int INTERVAL_ADD_USER = 2000;
    static const int INTERVAL_LOAD_HISTORY = 2000;
    static const int INTERVAL_REGISTER_USER = 2000;

    User fakeUser;
    QVector <Message*> fakeHistory;
    QVector <User*> fakeUserlist;
    quint16 fakeUserId;
    quint16 fakeStatus;
    QString fakePseud;
    QString fakeMsg;

    ServerListener *listenerx;
    ServerTalker *talkerx;
    QTcpSocket *socketx;
public:
    ClientSocket(QTcpSocket *sock);

    void authenticate(const QString& log, const QString& pass, quint16& status, quint16& userId,
                      QString& pseud, QString& msg);
    QVector <User*> loadUserlist(quint16 myId);
    User addUserById(quint16 mid, quint16 fid, bool status);
    User addUserByLogin(quint16 myId, const QString& log, bool status);
    QVector <Message*> loadHistory(Dialog *dg);
    quint16 registerUser(const QString& log, const QString& pseud, const QString& pass);
    User findUser(const QString& log);

    QTcpSocket *socket();
    ServerListener* listener();
    ServerTalker* talker();
private slots:
    void slotUserlistRecieved(const QVector <User*>& us);
    void slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status);
    void slotHistoryRecieved(const QVector<Message*> &hs);
    void slotUserRegistered(quint16 userId);
    void slotAuthentificated(quint16 status, quint16 userId,
                             const QString& pseud, const QString& msg);
    void slotFoundUser(quint16 userId, const QString& pseud);
};

#endif // CLIENTSOCKET_H
