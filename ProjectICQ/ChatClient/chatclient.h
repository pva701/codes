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
#include "notificationform.h"
#include "userlistwidget.h"

class ChatClient:public QWidget
{
    Q_OBJECT

    QTabWidget *tbwDialogs;
    UserListWidget *userlist;
    QVector <Notification*> notifs;

    Dialog *activeDialog;
    quint16 myId;
    QString pseudonym;
    ClientSocket *pSocket;

    QPushButton *pButSend;
    QPushButton *pButFind;
    DialogFindFriends *dialogFindFriends;
    //void keyPressEvent(QKeyEvent *e);
public:
    ChatClient(ClientSocket *socket, const QString& strHost, int nPort, int idUserx, const QString& pseud, QWidget *pwig = 0);
    ~ChatClient();
private:
    void createDialog(Dialog *dg, bool loadLast = true);
    void activateTab(Dialog *dg);
    void showNotifications();
    //QTextDocument* prepareRecievedMessage(const QString& msg);
    //int metainfoSmile(const QString& s, int pos);
    //static QTextDocument *toTextDocumentFromString
    //QString toStringFromTextDocument
private slots:
    void slotError(QAbstractSocket::SocketError);
    void slotPrepareSendMessage();
    void slotMessageReceived(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString& message);
    void slotYouAreAddedInUserlist(quint16 frId, const QString& pseud);
    void slotDoubleClickDialog(Dialog*);
    void slotTabClosed(int);
    void slotCurrentTabChanged(int);
    void slotFindFriendsOpen();
    void slotFindFriend(const QString& name);
    void slotAddFriend(User*);
    void slotNoAddFriend(User* us);
    void slotWriteToFriend(User*);
    void slotNotifyOnOff(quint16, bool flag);
    void slotReadMessageNotify();
    void slotReadAllYouMessageNotify(int);
    //void appendHistory(Dialog* dg, const QString& name, const QDateTime& sendTime, QTextDocument *document);
    //void slotOverrideKeyPress(QKeyEvent *e);
};

#endif // CHATCLIENT_H
