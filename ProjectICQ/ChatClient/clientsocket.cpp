#include "clientsocket.h"

ClientSocket::ClientSocket(QTcpSocket *sock) {
    listenerx = new ServerListener(sock);
    talkerx = new ServerTalker(sock);
    socketx = sock;
}


void ClientSocket::authenticate(const QString& log, const QString& pass, quint16& status, quint16& userId,
                                QString& pseud, QString& msg) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::AUTH) << log << pass;
    out.confirm();
    connect(listenerx, SIGNAL(tryAuthenticate(quint16,quint16,QString,QString)), this, SLOT(slotAuthentificated(quint16,quint16,QString,QString)));
    talkerx->sendToServer(outArray);
    //new thread
    if (listenerx->waitFullStream(INTERVAL_AUTH)) {
        status = fakeStatus;
        userId = fakeUserId;
        pseud = fakePseud;
        msg = fakeMsg;
    } else
        status = ServerFlags::Time_Out;
}

QVector<User> ClientSocket::loadUserlist(quint16 myId) {
    qDebug() << "load userlist";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_USERLIST);
    out << quint16(myId);
    out.confirm();
    //qRegisteredMetaType
    connect(listenerx, SIGNAL(userlistRecieved(QVector<User>)), this, SLOT(slotUserlistRecieved(QVector<User>)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_LOAD_USERLIST);
    qDebug() << "out from load userlist";
    return fakeUserlist;
}

void ClientSocket::slotUserlistRecieved(const QVector <User>& us) {
    fakeUserlist = us;
}

QVector <Notification> ClientSocket::loadNotifys(int myId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_NOTIFYS);
    out << myId;
    out.confirm();
    //qRegisteredMetaType
    connect(listenerx, SIGNAL(notifysRecieved(QVector<Notification>)), this, SLOT(sloNotifysRecieved(QVector<Notification>)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_LOAD_NOTIFYS);
    return fakeNotifys;
}

void ClientSocket::sloNotifysRecieved(const QVector<Notification>& notf) {
    fakeNotifys = notf;
}

User ClientSocket::addUserById(quint16 mid, quint16 fid, int status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_ID);
    out << quint16(mid) << quint16(fid) << status;
    out.confirm();
    connect(listenerx, SIGNAL(userAddedById(quint16,quint16,QString, int, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, int, bool)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser;
}

void ClientSocket::slotUserAdded(quint16 mid, quint16 did, const QString& pseud, int status, bool isOn) {
    fakeUser = User(mid, did, pseud, status, isOn);
}

User ClientSocket::addUserByLogin(quint16 myId, const QString& log, int status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_LOGIN);
    out << myId << log << status;
    out.confirm();
    connect(listenerx, SIGNAL(userAddedByLogin(quint16,quint16,QString, int, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, int, bool)), Qt::QueuedConnection);
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser;
}

QVector <Message> ClientSocket::loadHistory(Dialog *dg) {
    qDebug() << "load history";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_HISTORY);
    out << quint16(dg->dialog());
    out.confirm();
    connect(listenerx, SIGNAL(historyRecieved(QVector<Message>)), this, SLOT(slotHistoryRecieved(QVector<Message>)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_LOAD_HISTORY);
    return fakeHistory;
}

void ClientSocket::slotHistoryRecieved(const QVector<Message>& hs) {
    fakeHistory = hs;
}

User ClientSocket::findUser(const QString& log) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::FIND_USER);
    out << log;
    out.confirm();
    connect(listenerx, SIGNAL(foundUser(quint16,QString, bool)), this, SLOT(slotFoundUser(quint16, QString, bool)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser;
}

quint16 ClientSocket::registerUser(const QString& log, const QString& pseud, const QString& pass) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::REGISTER_USER);
    out << log << pseud << pass;
    out.confirm();
    connect(listenerx, SIGNAL(tryRegister(quint16)), this, SLOT(slotTryRegistered(quint16)));
    talkerx->sendToServer(outArray);
    //new thread
    listenerx->waitFullStream(INTERVAL_REGISTER_USER);
    return fakeUserId;
}

void ClientSocket::slotTryRegistered(quint16 userId) {
    qDebug() << "slot userRegistered";
    fakeUserId = userId;
}

void ClientSocket::slotAuthentificated(quint16 status, quint16 userId, const QString &pseud, const QString &msg) {
    qDebug() << "in slotAuth";
    fakeStatus = status;
    fakeUserId = userId;
    fakePseud = pseud;
    fakeMsg = msg;
}

void ClientSocket::slotFoundUser(quint16 userId, const QString &pseud, bool isOn) {
    qDebug() << "slot FoundedUser";
    fakeUser = User(userId, 0, pseud, 0, isOn);
}

QTcpSocket* ClientSocket::socket() {
    return socketx;
}

ServerListener* ClientSocket::listener() {
    return listenerx;
}

ServerTalker* ClientSocket::talker() {
    return talkerx;
}

ClientSocket::~ClientSocket() {
    delete listenerx;
    delete talkerx;
}
