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
#include "clientsocket.h"
#include "registrationform.h"

class AuthenticationForm : public QWidget
{
    Q_OBJECT

    QLabel *lblLogin;
    QLineEdit *teLogin;
    QLabel *lblPassword;
    QLineEdit *tePassword;
    RegistrationForm *regForm;

    ClientSocket *pSocket;
    quint16 sizeOfBlock;
    QString host;
    int port;
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);
public:
    AuthenticationForm(ClientSocket *socket, const QString& host, int port, QWidget *parent = 0);
    ~AuthenticationForm();
signals:
    void authenticated(quint16, const QString&);
private slots:
    void slotLogin();
    void slotRegisterFormOpen();
    void slotError(QAbstractSocket::SocketError err);
    void slotDisconnected();
    void slotClosedRegForm();
    void slotRegistered(quint16 userId, const QString& pseud);
};

#endif // AUTHENTICATIONFORM_H
