#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtCore/QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../common/bytesreaderwriter.h"
#include "../common/serverflags.h"
#include "../common/servercommands.h"

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
public:
    ChatServer(int port, QWidget *widget = 0);
private:
    void sendToClient(QTcpSocket *pSock, const QByteArray& s);
    QByteArray login(const QString& userLogin, const QString& userPassword);
    QByteArray loadUserlist(int userId);
private slots:
    void slotNewConnection();
    void slotReadClient();
};

#endif // CHATSERVER_H
