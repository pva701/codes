#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QString>
#include "serverlistener.h"
#include "servertalker.h"
#include "../common/serverflags.h"
#include "notification.h"

class ClientSocket:public QObject
{
    Q_OBJECT
private:
    static const int INTERVAL_AUTH = 20000;
    static const int INTERVAL_LOAD_USERLIST = 5000;//ms
    static const int INTERVAL_ADD_USER = 5000;
    static const int INTERVAL_LOAD_HISTORY = 5000;
    static const int INTERVAL_REGISTER_USER = 5000;
    static const int INTERVAL_LOAD_NOTIFYS = 5000;
    static const int INTERVAL_LOAD_DATETIME = 5000;

    User* fakeUser;
    QVector <Message> fakeHistory;
    QVector <User*> fakeUserlist;
    quint16 fakeUserId;
    quint16 fakeStatus;
    QString fakePseud;
    QString fakeMsg;
    QVector <Notification> fakeNotifys;
    QDateTime fakeDateTimeFromServ;

    ServerListener *listenerx;
    ServerTalker *talkerx;
    QTcpSocket *socketx;

    QDateTime dateTimeFromServer();
    QTime counter;
    QDateTime startDate;
public:
    ClientSocket(QTcpSocket *sock);

    void authenticate(const QString& log, const QString& pass, quint16& status, quint16& userId,
                      QString& pseud, QString& msg);
    QVector <User*> loadUserlist(quint16 myId);
    User* addUserById(quint16 mid, quint16 fid, int status);
    User* addUserByLogin(quint16 myId, const QString& log, int status);
    User* addUserByDialog(quint16 myId, quint16 dial, int status);
    QVector <Message> loadHistory(Dialog *dg);
    quint16 registerUser(const QString& log, const QString& pseud, const QString& pass);
    User* findUser(const QString& log);
    QVector <Notification> loadNotifys(int userId);
    void initDateTime();
    QDateTime currentDateTimeFromServer();

    QTcpSocket *socket();
    ServerListener* listener();
    ServerTalker* talker();

    ~ClientSocket();
private slots:
    void slotUserlistReceived(const QVector <User*>& us);
    void slotUserAdded(quint16 mid, quint16 did, const QString& pseud, int status, bool isOn);
    void slotHistoryReceived(const QVector<Message> &hs);
    void slotTryRegistered(quint16 userId);
    void slotAuthentificated(quint16 status, quint16 userId,
                             const QString& pseud, const QString& msg);
    void slotFoundUser(quint16 userId, const QString& pseud, bool isOn);
    void sloNotifysReceived(const QVector<Notification>& nf);
    void slotDateTimeFromServer(QDateTime);
};

#endif // CLIENTSOCKET_H
