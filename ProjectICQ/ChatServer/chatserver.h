#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../common/bytesreaderwriter.h"
#include "../common/serverflags.h"
#include "../common/servercommands.h"
#include "usersonline.h"

class ChatServer:public QWidget
{
    Q_OBJECT

    const QString DRIVER_OF_DATABASE;// = "QMYSQL";
    const QString NAME_OF_CONNECTION_TO_DATABASE;// = "connection";
    const QString NAME_OF_DATABASE;// = "clientinfodb";
    const QString LOGIN_TO_DATABASE;
    const QString PASSWORD_TO_DATABASE;

    QTcpServer *pTCPServer;
    QTextEdit *pTEMessage;
    quint16 sizeOfBlock;
    QSqlDatabase clientinfodb;
    UsersOnline online;
public:
    ChatServer(int port, QWidget *widget = 0);
private:
    //for working
    void insertIntoDatabaseMessage(quint16 dialog, quint16 fromId, QDateTime sendTime, const QString& content);
    QVector <quint16> membersOfDialog(quint16 dialog);
    QVector <QTcpSocket*> onlineMembersOfDialog(quint16 dialog);
    //commands
    QByteArray login(const QString& userLogin, const QString& userPassword, QTcpSocket *socket);
    QByteArray loadUserlist(int userId);
    void sendMessage(quint16 dialog, quint16 fromId, QDateTime sendTime, const QString& content);
    QByteArray addUserById(quint16 myId, std::pair <quint16, QString> fr, bool status);
    QByteArray addUserByLogin(quint16 myId, std::pair <quint16, QString> fr, bool status);
    void changeStatus(quint16 userId, quint16 frId, bool status);
    void youAddedInUserlist(std::pair <quint16, QString> me, quint16 frId);
    QByteArray loadHistory(quint16 dialogId);
    QByteArray registerUser(const QString& userLogin, const QString& pseud, const QString& pass);
    void sendToClient(QTcpSocket *pSock, const QByteArray& s);
    quint16 addUser(quint16 myId, std::pair <quint16, QString> fr, bool status);
    QByteArray findUser(const QString& log);

    std::pair <quint16, QString> userId(const QString& log);
    std::pair <quint16, QString> userId(quint16 id);
private slots:
    void slotNewConnection();
    void slotDisconnectedClient();
    void slotReadClient();
};

#endif // CHATSERVER_H
