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

class ChatClient:public QWidget
{
    Q_OBJECT

    static const int INTERVAL_TIME_USERLIST = 2000;//ms

    QTcpSocket *pSocket;
    QTabWidget *tbwDialogs;
    QListWidget *lwOfFriends;

    QVector <User*> userlist;
    quint16 sizeOfBlock;
    Dialog *activeDialog;
    quint16 userId;
    QString userLogin;
public:
    ChatClient(QTcpSocket *socket, const QString& strHost, int nPort, int idUserx, QString userLoginx, QWidget *pwig = 0);

private:
    void loadUserlist(QVector <User*>& fr);
    void loadHistory(Dialog *dg);
    void sendToServer(const QByteArray& bytearray);
    QDateTime currentDateTimeFromServer();
    void gotMessage(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString& message);

    void createTab(Dialog *dg);
    void activateTab(Dialog *dg);

    bool waitFullStream(int mills, BytesReaderWriter& in);
private slots:
    void slotReadServer();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();

    void slotDoubleClickedUserlistItem(QListWidgetItem*);
    void slotTabClosed(int);
    void slotCurrentTabChanged(int);
    void slotSendMessage();
    //void slotOverrideKeyPress(QKeyEvent *e);
};

#endif // CHATCLIENT_H
