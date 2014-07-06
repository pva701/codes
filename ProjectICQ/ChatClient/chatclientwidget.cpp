#include "chatclientwidget.h"

ChatClientWidget::ChatClientWidget(const QString& hosty, int porty):host(hosty), port(porty) {}

void ChatClientWidget::start() {
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    autFrm = new AuthenticationForm(socket, host, port);////////////////////////// mysql host, mysql port
    QApplication::connect(autFrm, SIGNAL(authenticated(int, QString)), this, SLOT(slotAuthenticated(int, QString)));
    autFrm->show();
}

void ChatClientWidget::slotAuthenticated(int userId, QString userLogin) {
    autFrm->hide();
    delete autFrm;
    qDebug() << "SLOT AUTH\n";
    chatClient = new ChatClient(socket, host, port, userId, userLogin);
    chatClient->show();

}
