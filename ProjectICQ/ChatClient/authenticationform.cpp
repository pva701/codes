#include "authenticationform.h"

AuthenticationForm::AuthenticationForm(ClientSocket *socket, const QString& hostx, int portx, QWidget *parent /*= 0*/):QWidget(parent),
    regForm(NULL), sizeOfBlock(0), host(hostx), port(portx) {
    setWindowTitle("Authentication");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    //setAttribute(Qt::WA_DeleteOnClose);
    QRect sizeOfMonitor = QApplication::desktop()->screenGeometry();
    const QSize sizeOfWindow = QSize(300, 200);
    const QRect positonOfWindow = QRect(sizeOfMonitor.width() / 2 - sizeOfWindow.width() / 2, 100, sizeOfWindow.width(), sizeOfWindow.height());
    setGeometry(positonOfWindow);
    setFixedSize(sizeOfWindow);

    lblLogin = new QLabel("Login:", this);
    teLogin = new QLineEdit(this);
    lblLogin->setBuddy(teLogin);
    lblPassword = new QLabel("Password:", this);
    tePassword = new QLineEdit(this);
    lblLogin->setBuddy(tePassword);
    tePassword->setEchoMode(QLineEdit::Password);

    QPushButton *butLogin = new QPushButton("Log in", this);
    QPushButton *butRegisterForm = new QPushButton("Register", this);
    QVBoxLayout *lytAut = new QVBoxLayout(this);
    QHBoxLayout *lytButtons = new QHBoxLayout();
    //lytId->setSizeConstraint(QVBoxLayout::SetFixedSize);
    //lytId->setGeometry(QRect(0, sizeOfWindow.height() / 3, sizeOfWindow.width(), sizeOfWindow.height() / 3));
    lytAut->addWidget(lblLogin);
    lytAut->addWidget(teLogin);
    lytAut->addWidget(lblPassword);
    lytAut->addWidget(tePassword);
    lytButtons->addWidget(butLogin);
    lytButtons->addWidget(butRegisterForm);
    lytAut->addLayout(lytButtons);
    setLayout(lytAut);

    pSocket = socket;
    connect(pSocket->socket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    //connect(pSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(butLogin, SIGNAL(clicked()), this, SLOT(slotLogin()));
    connect(butRegisterForm, SIGNAL(clicked()), this, SLOT(slotRegisterFormOpen()));
}

void AuthenticationForm::slotLogin() {
    //if (pSocket->state() == QAbstractSocket::UnconnectedState)
        //pSocket->connectToHost(host, port);
    quint16 status, userId;
    QString pseud, msg;
    pSocket->authenticate(teLogin->text(), tePassword->text(), status, userId, pseud, msg);

    if (status == ServerFlags::Success_Auth)
        emit authenticated(userId, pseud);
    else if (status == ServerFlags::CE_Database)
        QMessageBox::critical(this, "ConnectionError", msg);
    else if (status == ServerFlags::AE_Wrong_Login)
        QMessageBox::critical(this, "AuthenticationError", "Wrong login! Try again.");
    else if (status == ServerFlags::AE_Wrong_Password)
       QMessageBox::critical(this, "AuthenticationError", "Wrong password! Try again.");
    else if (status == ServerFlags::Time_Out)
        QMessageBox::critical(this, "ConnectionError", "Expired connection time");
}

void AuthenticationForm::slotRegisterFormOpen() {
    setEnabled(false);
    if (regForm == NULL) {
        regForm = new RegistrationForm(pSocket, this);
        connect(regForm, SIGNAL(registered(quint16,QString)), this, SLOT(slotRegistered(quint16,QString)), Qt::QueuedConnection);
        regForm->show();
        connect(regForm, SIGNAL(closed()), this, SLOT(slotClosedRegForm()));
    } else
        regForm->show();
}

void AuthenticationForm::slotClosedRegForm() {
    setEnabled(true);
}


void AuthenticationForm::slotRegistered(quint16 userId, const QString& pseud) {
    regForm->close();
    emit authenticated(userId, pseud);
}

void AuthenticationForm::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(pSocket->socket()->errorString()));
    QMessageBox::critical(this, "ConnectionError", msg);
}


void AuthenticationForm::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
        slotLogin();
}

AuthenticationForm::~AuthenticationForm() {
    if (regForm != NULL)
        delete regForm;
}
