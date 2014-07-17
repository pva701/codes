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
#include <QMessageBox>
#include "user.h"
#include "../common/bytesreaderwriter.h"
#include "../common/servercommands.h"
#include "../common/serverflags.h"
#include "serverlistener.h"
#include "servertalker.h"
#include "clientsocket.h"
#include <QLineEdit>
#include <QTextBlock>
#include <QInputDialog>
#include "dialogfindfriends.h"
#include "notification.h"
#include "userlistwidget.h"

class ChatClient:public QWidget
{
    Q_OBJECT

    QTabWidget *tbwDialogs;
    UserListWidget *userlist;

    Dialog *activeDialog;
    quint16 myId;
    QString pseudonym;
    ClientSocket *pSocket;

    QPushButton *pButSend;
    QPushButton *pButFind;

    DialogFindFriends *dialogFindFriends;
public:
    ChatClient(ClientSocket *socket, const QString& strHost, int nPort, int idUserx, const QString& pseud, QWidget *pwig = 0);
    ~ChatClient();
private:
    void createTab(Dialog *dg);
    void activateTab(Dialog *dg);
private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotPrepareSendMessage();
    //void slotUserlistRecieved(const QVector <User*>& us);
    //void slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status);
    //void slotHistoryRecieved(const QVector<Message*>& hs);
    void slotMessageRecieved(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString& message);
    void slotYouAreAddedInUserlist(quint16 frId, const QString& pseud);

    void slotDoubleClickedUserlistItem(Dialog*);
    void slotTabClosed(int);
    void slotCurrentTabChanged(int);
    void slotFindFriendsOpen();
    void slotFindFriend(const QString& name);
    void slotAddFriend(User);
    void slotNoAddFriend();
    void slotWriteToFriend(User);
    void slotNotifyOnOff(quint16, bool flag);
    //void slotOverrideKeyPress(QKeyEvent *e);
};

#endif // CHATCLIENT_H
