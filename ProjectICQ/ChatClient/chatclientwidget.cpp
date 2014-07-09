#include "chatclientwidget.h"
#include "clientsocket.h"

ChatClientWidget::ChatClientWidget(const QString& hosty, int porty):host(hosty), port(porty) {}

void ChatClientWidget::start() {
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    clSocket = new ClientSocket(socket);
    autFrm = new AuthenticationForm(clSocket, host, port);////////////////////////// mysql host, mysql port
    QApplication::connect(autFrm, SIGNAL(authenticated(quint16, QString)), this, SLOT(slotAuthenticated(quint16, QString)));
    autFrm->show();
}

void ChatClientWidget::slotAuthenticated(quint16 userId, QString pseud) {
    autFrm->hide();
    delete autFrm;
    qDebug() << "SLOT AUTH\n";
    chatClient = new ChatClient(clSocket, host, port, userId, pseud);
    chatClient->show();

}
