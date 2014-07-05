#include "chatclient.h"

ChatClient::ChatClient(const QString& strHost, int nPort, int userIdx, QString userLoginx, QWidget *pwig):QWidget(pwig), sizeOfBlock(0) {
    qDebug() << "chat client\n";
    userId = userIdx;
    userLogin = userLoginx;

    pSocket = new QTcpSocket(this);
    pSocket->connectToHost(strHost, nPort);

    setWindowTitle("FriendlyChatClient");

    QPushButton *pBut = new QPushButton("&Send");

    setGeometry(600, 300, 700, 700);
    QVBoxLayout *lytVBox = new QVBoxLayout();
    QHBoxLayout *lytHBox = new QHBoxLayout();

    tbwDialogs = new QTabWidget();
    lytVBox->addWidget(tbwDialogs);
    lytVBox->addWidget(pBut);
    pBut->setMaximumSize(50, 40);


    lwOfFriends = new QListWidget();
    //listOfFriends->setIconSize(QSize(48, 48));
    lwOfFriends->setSelectionMode(QAbstractItemView::SingleSelection);
    loadUserlist(userlist);
    for (int i = 0; i < userlist.size(); ++i) {
        QListWidgetItem *pointer = new QListWidgetItem(userlist[i]->name(), lwOfFriends);
        userlist[i]->setListWidgetItem(pointer);//pg 214
    }

    //pItem1->setBackgroundColor(QColor(255, 0, 0));
    //pItem1->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    connect(lwOfFriends, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotClickedUserlistItem(QListWidgetItem*)));

    lytHBox->addLayout(lytVBox, 2);
    lytHBox->addWidget(lwOfFriends);
    setLayout(lytHBox);
    tbwDialogs->setMovable(true);
    tbwDialogs->setTabsClosable(true);
    connect(tbwDialogs, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabClosed(int)));
    connect(tbwDialogs, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));

    //connect(pBut, SIGNAL(clicked()), this, SLOT(sendToServer()));
    connect(pSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(slotReadServer()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    //connect(pTEInput, SIGNAL(overrideKeyPress(QKeyEvent*)), this, SLOT(slotOverrideKeyPress(QKeyEvent*)));
}

void ChatClient::slotReadServer() {//read from the server
    qDebug() << "slot read server!" << endl;
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
        sizeOfBlock = 0;
        quint16 typeOfCommand = in.readT<quint16>();
    }
}

void ChatClient::sendToServer(const QByteArray& bytearray) {//send to the server
    pSocket->write(bytearray);
}

void ChatClient::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(pSocket->errorString()));
    //pTEInfo->append(msg); TODO
}

void ChatClient::slotConnected() {
    qDebug() << "Connected client!!!\n";
    //pTEInfo->append("Received the connected() signal");
    //qDebug() << "SLOT CONNECTED IN CLIENT\n";
}


void ChatClient::slotClickedUserlistItem(QListWidgetItem *item) {
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
    qDebug() << "CLOSE TAB " << tab << endl;
}

void ChatClient::slotCurrentTabChanged(int newTab) {
    qDebug() << "NEW TAB " << newTab << endl;

}

void ChatClient::loadUserlist(QVector <User*>& us) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_USERLIST);
    out << quint16(userId);
    out.confirm();
    sendToServer(outArray);

    //new thread
    BytesReaderWriter in(pSocket);
    waitFullStream(INTERVAL_TIME_USERLIST, in);
    in.readT<quint16>();//type of command
    int numberOfFriends = in.readT<quint16>();
    for (int i = 0; i < numberOfFriends; ++i) {
        int uid, did;
        QString ulog, pseud;
        bool isfr;
        in >> uid >> ulog >> did >> pseud >> isfr;
        us.push_back(new User(uid, ulog, did, pseud, isfr));
    }
    //out << usersId[i] << userLogin << dialogId[i] << pseud << isFriend[i];
}

void ChatClient::loadHistory(int dialog) {

}

void ChatClient::createTab(Dialog *dg) {
    for (int i = 0; i < tbwDialogs->count(); ++i)
        if (tbwDialogs->widget(i)->children().contains(dg->history()))
            return;
    QSplitter *spl = new QSplitter(Qt::Vertical);
    dg->setTEHistory(new QTextEdit());
    dg->setTEMessage(new QTextEdit());
    dg->history()->setReadOnly(true);
    spl->addWidget(dg->history());
    spl->addWidget(dg->message());
    loadHistory(dg->dialog());////
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

bool ChatClient::waitFullStream(int mills, BytesReaderWriter& in) {
    in.setVersion(QDataStream::Qt_4_5);
    while (pSocket->waitForReadyRead(mills)) {
        if (!sizeOfBlock) {
            if (pSocket->bytesAvailable() < sizeof(quint16))
                continue;
            in >> sizeOfBlock;
        }

        if (pSocket->bytesAvailable() < sizeOfBlock)
            continue;
        sizeOfBlock = 0;
        return true;
    }
    return false;
}
