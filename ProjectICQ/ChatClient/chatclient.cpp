#include "chatclient.h"

ChatClient::ChatClient(QTcpSocket *socket, const QString& strHost, int nPort, int userIdx, QString userLoginx, QWidget *pwig):QWidget(pwig), sizeOfBlock(0) {
    qDebug() << "chat client\n";
    userId = userIdx;
    userLogin = userLoginx;

    pSocket = socket;
    //pSocket->connectToHost(strHost, nPort);

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
    loadUserlist(userlist);
    for (int i = 0; i < userlist.size(); ++i) {
        QListWidgetItem *pointer = new QListWidgetItem(userlist[i]->name(), lwOfFriends);
        userlist[i]->setListWidgetItem(pointer);//pg 214
    }

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

    connect(pBut, SIGNAL(clicked()), this, SLOT(slotSendMessage()));
    connect(pSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(slotReadServer()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    //connect(pTEInput, SIGNAL(overrideKeyPress(QKeyEvent*)), this, SLOT(slotOverrideKeyPress(QKeyEvent*)));
}

///COMMANDS===========================================================================================
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
    waitFullStream(INTERVAL_LOAD_USERLIST, in);
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

void ChatClient::loadHistory(Dialog *dg) {

}

void ChatClient::gotMessage(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString &message) {
    Dialog *dg = NULL;
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->dialog() == dialogNum)
            dg = userlist[i];
    //qDebug() << "got message " << dialogNum << " " << fromId;
    if (fromId == userId)
        dg->history()->append("You (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message);
    else
        dg->history()->append(dg->name() + " (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message);
    createTab(dg);
}

QDateTime ChatClient::currentDateTimeFromServer() {
    return QDateTime::currentDateTime();
}
//END OF COMMANDS====================================================================================


void ChatClient::slotConnected() {
    qDebug() << "Connected client!!!\n";
}

void ChatClient::slotReadServer() {//read from the server
    //qDebug() << "slot read server!" << endl;
    BytesReaderWriter in(pSocket);
    in.setVersion(QDataStream::Qt_4_5);
    for (;;) {
        if (!sizeOfBlock) {
            if (pSocket->bytesAvailable() < sizeof(quint16))
                break;
            sizeOfBlock = in.readT<quint16>();
        }

        if (pSocket->bytesAvailable() < sizeOfBlock)
            break;
        sizeOfBlock = 0;
        quint16 typeOfCommand = in.readT<quint16>();
        if (typeOfCommand == ServerCommands::SEND_MESSAGE) {
            quint16 dg = in.readT<quint16>();
            quint16 from = in.readT<quint16>();
            QDateTime sendTime = in.readT<QDateTime>();
            QString content = in.readT<QString>();
            gotMessage(dg, from, sendTime, content);
        }
    }
}

void ChatClient::slotSendMessage() {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::SEND_MESSAGE);
    out << quint16(activeDialog->dialog()) << quint16(userId) << currentDateTimeFromServer() << activeDialog->message()->toPlainText();
    out.confirm();
    activeDialog->message()->clear();
    sendToServer(outArray);
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
    loadHistory(dg);
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

