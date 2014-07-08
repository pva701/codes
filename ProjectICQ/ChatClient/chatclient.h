#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtCore/QDateTime>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QSplitter>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlError>
#include "user.h"
#include "../common/bytesreaderwriter.h"
#include "../common/servercommands.h"
#include "../common/serverflags.h"
#include "serverlistener.h"
#include "servertalker.h"

class ChatClient:public QWidget
{
    Q_OBJECT

    static const int INTERVAL_LOAD_USERLIST = 2000;//ms
    static const int INTERVAL_ADD_USER = 2000;
    static const int INTERVAL_LOAD_HISTORY = 2000;

    QTabWidget *tbwDialogs;
    QListWidget *lwOfFriends;

    QVector <User*> userlist;
    Dialog *activeDialog;
    quint16 myId;
    QString userLogin;
    QTcpSocket *socket;
    ServerListener *listener;
    ServerTalker *talker;

    User fakeUser;
    QVector <Message*> fakeHistory;
public:
    ChatClient(QTcpSocket *socket, const QString& strHost, int nPort, int idUserx, QString userLoginx, QWidget *pwig = 0);

private:
    void createTab(Dialog *dg);
    void activateTab(Dialog *dg);

    void loadUserlist(quint16 myId);
    QString addUserById(quint16 mid, quint16 fid, bool status);
    User addUserByLogin(quint16 myId, const QString& log, bool status);
    QVector <Message*> loadHistory(Dialog *dg);
private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotPrepareSendMessage();
    void slotUserlistRecieved(const QVector <User*>& us);
    void slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status);
    void slotHistoryRecieved(const QVector<Message*>& hs);
    void slotMessageRecieved(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString& message);
    void slotYouAddedInUserlist(quint16 frId);

    void slotDoubleClickedUserlistItem(QListWidgetItem*);
    void slotTabClosed(int);
    void slotCurrentTabChanged(int);
    //void slotOverrideKeyPress(QKeyEvent *e);
};

#endif // CHATCLIENT_H
