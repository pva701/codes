#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include "clientsocket.h"

class RegistrationForm:public QWidget
{
    Q_OBJECT

    QLabel *lblPseud;
    QLineEdit *tePseudonym;
    QLabel *lblLoginReg;
    QLineEdit *teLoginReg;
    QLabel *lblPasswordReg;
    QLineEdit *tePasswordReg;
    QLabel *lblConfPasswordReg;
    QLineEdit *teConfPasswordReg;
    ClientSocket *pSocket;

    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
public:
    explicit RegistrationForm(ClientSocket* socket, QWidget *parent = 0);
private slots:
    void slotRegister();
signals:
    void closed();
    void registered(quint16 userId, const QString& pseud);
};

#endif // REGISTRATIONFORM_H
