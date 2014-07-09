#include "smartclientsocket.h"

/*SmartClientSocket::SmartClientSocket(QTcpSocket *sock) {
    listenerx = new ServerListener(sock);
    talkerx = new ServerTalker(sock);
    socketx = sock;
}*/

QVector<User*> SmartClientSocket::loadUserlist(quint16 myId) {
    qDebug() << "load userlist";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_USERLIST);
    out << quint16(myId);
    out.confirm();
    connect(listenerx, SIGNAL(userlistRecieved(QVector<User*>)), this, SLOT(slotUserlistRecieved(QVector<User*>)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_LOAD_USERLIST);
    return fakeUserlist;
}

void SmartClientSocket::slotUserlistRecieved(const QVector <User*>& us) {
    fakeUserlist = us;
}

QString SmartClientSocket::addUserById(quint16 mid, quint16 fid, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_ID);
    out << quint16(mid) << quint16(fid) << status;
    out.confirm();
    connect(listenerx, SIGNAL(userAdded(quint16,quint16,QString, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, bool)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser.name();
}

void SmartClientSocket::slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status) {
    fakeUser = User(mid, did, pseud, status);
}

User SmartClientSocket::addUserByLogin(quint16 myId, const QString& log, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_LOGIN);
    out << myId << log << status;
    out.confirm();
    connect(listenerx, SIGNAL(userAdded(quint16,quint16,QString, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, bool)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser;
}

QVector <Message*> SmartClientSocket::loadHistory(Dialog *dg) {
    qDebug() << "load history";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_HISTORY);
    out << quint16(dg->dialog());
    out.confirm();
    connect(listenerx, SIGNAL(historyRecieved(QVector<Message*>)), this, SLOT(slotHistoryRecieved(QVector<Message*>)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_LOAD_HISTORY);
    return fakeHistory;
}

void SmartClientSocket::slotHistoryRecieved(const QVector<Message*> &hs) {
    fakeHistory = hs;
}

QTcpSocket* SmartClientSocket::socket() {
    return socketx;
}

ServerListener* SmartClientSocket::listener() {
    return listenerx;
}

ServerTalker* SmartClientSocket::talker() {
    return talkerx;
}
