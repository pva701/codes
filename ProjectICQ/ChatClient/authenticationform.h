#ifndef AUTHENTICATIONFORM_H
#define AUTHENTICATIONFORM_H
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QtSql>
#include <QMessageBox>
#include <QKeyEvent>
#include <QAbstractSocket>
#include "../common/bytesreaderwriter.h"
#include "../common/serverflags.h"
#include "../common/servercommands.h"

class AuthenticationForm : public QWidget
{
    Q_OBJECT
public:
    explicit AuthenticationForm(const QString& host, int port, QWidget *parent = 0);
    QLabel *lblLogin;
    QLineEdit *teLogin;
    QLabel *lblPassword;
    QLineEdit *tePassword;
    QTcpSocket *pSocket;
    quint16 sizeOfBlock;
    void keyPressEvent(QKeyEvent *e);

    void sendToServer(const QByteArray& bytearray);
    void statusAuth(quint16 status, int userId, const QString& userLogin, const QString& msg);
    ~AuthenticationForm();
signals:
    void authenticated(int, QString);
    void unsupportedCommand();
private slots:
    void slotLogin();
    void slotReadFromServer();
    void slotError(QAbstractSocket::SocketError err);

public slots:
};

#endif // AUTHENTICATIONFORM_H
