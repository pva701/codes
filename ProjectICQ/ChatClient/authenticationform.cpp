#include "authenticationform.h"

AuthenticationForm::AuthenticationForm(QTcpSocket *socket, const QString& hostx, int portx, QWidget *parent /*= 0*/):QWidget(parent), sizeOfBlock(0), host(hostx), port(portx) {
    setWindowTitle("Authentication");
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
    QVBoxLayout *lytAut = new QVBoxLayout(this);
    //lytId->setSizeConstraint(QVBoxLayout::SetFixedSize);
    //lytId->setGeometry(QRect(0, sizeOfWindow.height() / 3, sizeOfWindow.width(), sizeOfWindow.height() / 3));
    lytAut->addWidget(lblLogin);
    lytAut->addWidget(teLogin);
    lytAut->addWidget(lblPassword);
    lytAut->addWidget(tePassword);
    lytAut->addWidget(butLogin);
    butLogin->setMaximumHeight(40);
    butLogin->setMaximumWidth(50);
    setLayout(lytAut);

    pSocket = socket;
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(slotReadFromServer()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

    connect(butLogin, SIGNAL(clicked()), this, SLOT(slotLogin()));
}

void AuthenticationForm::sendToServer(const QByteArray& bytearray) {
    pSocket->write(bytearray);
}

void AuthenticationForm::slotLogin() {
    //if (pSocket->state() == QAbstractSocket::UnconnectedState)
        //pSocket->connectToHost(host, port);

    const QString& log = teLogin->text();
    const QString& pass = tePassword->text();
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::AUTH) << log << pass;
    out.confirm();
    sendToServer(outArray);
}

void AuthenticationForm::slotReadFromServer() {//read from the server
    qDebug() << "slot Read from server!";
    BytesReaderWriter in(pSocket);
    in.setVersion(QDataStream::Qt_4_5);
    for (;;) {
        if (!sizeOfBlock) {
            if (pSocket->bytesAvailable() < sizeof(quint16))
                break;
            in >> sizeOfBlock;
        }
        if (pSocket->bytesAvailable() < sizeOfBlock)
            break;
        quint16 typeOfCommand;
        in >> typeOfCommand;
        sizeOfBlock = 0;
        if (typeOfCommand == ServerCommands::AUTH) {
            quint16 status, userId;
            QString userLogin, msg;
            in >> status >> userId >> userLogin >> msg;
            statusAuth(status, userId, userLogin, msg);
            return;
       } else
            emit unsupportedCommand();
    }
}

void AuthenticationForm::statusAuth(quint16 status, int userId, const QString& userLogin, const QString& msg) {
    if (status == ServerFlags::Success_Auth)
        emit authenticated(userId, userLogin);
    else if (status == ServerFlags::CE_Database)
        QMessageBox::critical(0, "ConnectionError", msg);
    else if (status == ServerFlags::AE_Wrong_Login)
        QMessageBox::critical(0, "AuthenticationError", "Wrong login! Try again.");
    else if (status == ServerFlags::AE_Wrong_Password)
       QMessageBox::critical(0, "AuthenticationError", "Wrong password! Try again.");
}

void AuthenticationForm::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(pSocket->errorString()));
    QMessageBox::critical(0, "ConnectionError", msg);
}

void AuthenticationForm::slotDisconnected() {
}


void AuthenticationForm::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return)
        slotLogin();
}

/*AuthenticationForm::~AuthenticationForm() {
    disconnect(pSocket, SIGNAL(readyRead()), this, SLOT(slotReadFromServer()));
    disconnect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    pSocket->close();
    pSocket->disconnectFromHost();
}*/
