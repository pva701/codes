#include "chatclient.h"

ChatClient::ChatClient(QTcpSocket *sockett, const QString& strHost, int nPort, int userIdx, QString userLoginx, QWidget *pwig):QWidget(pwig){
    qDebug() << "chat client\n";
    socket = sockett;
    listener = new ServerListener(socket);
    talker = new ServerTalker(socket);

    myId = userIdx;
    userLogin = userLoginx;

    setWindowTitle("FriendlyChatClient");

    QPushButton *pBut = new QPushButton("&Send");

    QRect sizeOfMonitor = QApplication::desktop()->screenGeometry();
    setGeometry(sizeOfMonitor);

    QVBoxLayout *lytVBox = new QVBoxLayout();
    QHBoxLayout *lytHBox = new QHBoxLayout();

    tbwDialogs = new QTabWidget();
    lytVBox->addWidget(tbwDialogs);
    lytVBox->addWidget(pBut);
    pBut->setMaximumSize(50, 40);


    lwOfFriends = new QListWidget();
    //listOfFriends->setIconSize(QSize(48, 48));
    lwOfFriends->setSelectionMode(QAbstractItemView::SingleSelection);
    loadUserlist(myId);

    //pItem1->setBackgroundColor(QColor(255, 0, 0));
    //pItem1->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    connect(lwOfFriends, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotDoubleClickedUserlistItem(QListWidgetItem*)));

    lytHBox->addLayout(lytVBox, 2);
    lytHBox->addWidget(lwOfFriends);
    setLayout(lytHBox);
    tbwDialogs->setMovable(true);
    tbwDialogs->setTabsClosable(true);
    connect(tbwDialogs, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabClosed(int)));
    connect(tbwDialogs, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));

    connect(pBut, SIGNAL(clicked()), this, SLOT(slotPrepareSendMessage()));
    connect(listener, SIGNAL(messageRecieved(quint16,quint16,QDateTime,QString)), this, SLOT(slotMessageRecieved(quint16,quint16,QDateTime,QString)));
    connect(listener, SIGNAL(youAddedInUserlist(quint16)), this, SLOT(slotYouAddedInUserlist(quint16)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void ChatClient::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(socket->errorString()));
    //pTEInfo->append(msg); TODO
}

void ChatClient::slotPrepareSendMessage() {
    talker->sendMessage(activeDialog->dialog(), myId, talker->currentDateTimeFromServer(), activeDialog->message()->toPlainText());
    activeDialog->message()->clear();
}

void ChatClient::loadUserlist(quint16 myId) {
    qDebug() << "load userlist";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_USERLIST);
    out << quint16(myId);
    out.confirm();
    connect(listener, SIGNAL(userlistRecieved(QVector<User*>)), this, SLOT(slotUserlistRecieved(QVector<User*>)));
    talker->sendToServer(outArray);
    //new thread
    listener->waitFullStream(INTERVAL_LOAD_USERLIST);
}

void ChatClient::slotUserlistRecieved(const QVector <User*>& us) {
    userlist = us;
    for (int i = 0; i < userlist.size(); ++i) {
        QListWidgetItem *pointer = new QListWidgetItem(userlist[i]->name(), lwOfFriends);
        userlist[i]->setListWidgetItem(pointer);//pg 214
    }
}

QString ChatClient::addUserById(quint16 mid, quint16 fid, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_ID);
    out << quint16(mid) << quint16(fid) << status;
    out.confirm();
    connect(listener, SIGNAL(userAdded(quint16,quint16,QString, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, bool)));
    talker->sendToServer(outArray);
    //new thread
    listener->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser.name();
}

void ChatClient::slotUserAdded(quint16 mid, quint16 did, const QString& pseud, bool status) {
    fakeUser = User(mid, did, pseud, status);
}

User ChatClient::addUserByLogin(quint16 myId, const QString& log, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_LOGIN);
    out << myId << log << status;
    out.confirm();
    connect(listener, SIGNAL(userAdded(quint16,quint16,QString, bool)), this, SLOT(slotUserAdded(quint16,quint16,QString, bool)));
    talker->sendToServer(outArray);
    //new thread
    listener->waitFullStream(INTERVAL_ADD_USER);
    return fakeUser;
}

QVector <Message*> ChatClient::loadHistory(Dialog *dg) {
    qDebug() << "load history";
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_HISTORY);
    out << quint16(dg->dialog());
    out.confirm();
    connect(listener, SIGNAL(historyRecieved(QVector<Message*>)), this, SLOT(slotHistoryRecieved(QVector<Message*>)));
    talker->sendToServer(outArray);
    //new thread
    listener->waitFullStream(INTERVAL_LOAD_HISTORY);
    return fakeHistory;
}

void ChatClient::slotHistoryRecieved(const QVector<Message*> &hs) {
    fakeHistory = hs;
}

void ChatClient::slotMessageRecieved(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString &message) {
    Dialog *dg = NULL;
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->dialog() == dialogNum)
            dg = userlist[i];
    qDebug() << dg;
    if (dg == NULL) {//not found
        QString pseud = addUserById(myId, fromId, 0);
        userlist.push_back(new User(fromId, dialogNum, pseud, User::Not_Friend));
        dg = userlist.back();
        userlist.back()->setListWidgetItem(new QListWidgetItem(userlist.back()->name(), lwOfFriends));
    }
    qDebug() << "got message " << fromId << endl;
    createTab(dg);
    if (fromId == myId)//oneColor
        dg->history()->append("You (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message);
    else//otherColor
        dg->history()->append(dg->name() + " (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message);
}

void ChatClient::slotDoubleClickedUserlistItem(QListWidgetItem *item) {
    Dialog *dg = NULL;
    for (int i = 0; i < lwOfFriends->count(); ++i)/////
        if (item == userlist[i]->pointerToItem()) {
            dg = userlist[i];
            break;
        }
    createTab(dg);
    activateTab(dg);
}

void ChatClient::slotTabClosed(int tab) {
    tbwDialogs->removeTab(tab);
}

void ChatClient::slotCurrentTabChanged(int newTab) {
    if (newTab == -1)
        activeDialog = NULL;
    else {
        for (int i = 0; i < userlist.size(); ++i)////wrong: dialogs, when will be added conference
            if (userlist[i]->widget() == tbwDialogs->widget(newTab))
                activeDialog = userlist[i];
    }
    qDebug() << "NEW TAB " << newTab << endl;
}

void ChatClient::createTab(Dialog *dg) {
    for (int i = 0; i < tbwDialogs->count(); ++i)
        if (tbwDialogs->widget(i) == dg->widget())
            return;
    if (dg->widget() != NULL) {
        tbwDialogs->addTab(dg->widget(), dg->name());
        return;
    }

    QSplitter *spl = new QSplitter(Qt::Vertical);
    dg->setTEHistory(new QTextEdit());
    dg->setTEMessage(new QTextEdit());
    dg->history()->setReadOnly(true);
    spl->addWidget(dg->history());
    spl->addWidget(dg->message());

    QVector <Message*> history = loadHistory(dg);
    for (int i = 0; i < history.size(); ++i)
        if (history[i]->from() == myId)
            dg->history()->append("You (" + history[i]->time().toString("dd-MM-yyyy hh:mm:ss") + "): " + history[i]->message());
        else
            dg->history()->append(history[i]->pseudonym() + " (" + history[i]->time().toString("dd-MM-yyyy hh:mm:ss") + "): " + history[i]->message());

    dg->setWidget(spl);
    tbwDialogs->addTab(spl, dg->name());
}

void ChatClient::activateTab(Dialog *dg) {
    activeDialog = dg;
    for (int i = 0; i < tbwDialogs->count(); ++i)
        if (tbwDialogs->widget(i)->children().contains(dg->history())) {
            tbwDialogs->setCurrentIndex(i);
            return;
        }
}

void ChatClient::slotYouAddedInUserlist(quint16 frId) {

}
