#include "chatclientform.h"
#include "clientsocket.h"

ChatClientForm::ChatClientForm(const QString& hosty, int porty):host(hosty), port(porty), chatClient(NULL) {}

void ChatClientForm::start() {
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    clSocket = new ClientSocket(socket);
    autFrm = new AuthenticationForm(clSocket, host, port);////////////////////////// mysql host, mysql port
    QApplication::connect(autFrm, SIGNAL(authenticated(quint16, QString)), this, SLOT(slotAuthenticated(quint16, QString)));
    autFrm->show();
}

void ChatClientForm::slotAuthenticated(quint16 userId, QString pseud) {
    qDebug() << "SLOT AUTH\n";
    autFrm->close();
    chatClient = new ChatClient(clSocket, host, port, userId, pseud);
    chatClient->show();
}

ChatClientForm::~ChatClientForm() {
    delete autFrm;
    delete socket;
    delete clSocket;
    if (chatClient != NULL)
        delete chatClient;
}
