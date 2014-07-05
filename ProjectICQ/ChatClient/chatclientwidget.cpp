#include "chatclientwidget.h"

ChatClientWidget::ChatClientWidget(const QString& hosty, int porty):host(hosty), port(porty) {}

void ChatClientWidget::start() {
    autFrm = new AuthenticationForm(host, port);////////////////////////// mysql host, mysql port
    QApplication::connect(autFrm, SIGNAL(authenticated(int, QString)), this, SLOT(slotAuthenticated(int, QString)));
    autFrm->show();
}

void ChatClientWidget::slotAuthenticated(int userId, QString userLogin) {
    autFrm->hide();
    delete autFrm;
    qDebug() << "YEESSS SLOT AUTH\n";
    server = new ChatClient(host, port, userId, userLogin);
    server->show();

}
