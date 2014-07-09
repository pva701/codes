#include "chatclient.h"

ChatClient::ChatClient(ClientSocket *sockett, const QString& strHost, int nPort, int userIdx, const QString& pseud, QWidget *pwig):QWidget(pwig){
    qDebug() << "chat client\n";
    pSocket = sockett;
    //listener = new ServerListener(socket);
    //talker = new ServerTalker(socket);

    myId = userIdx;
    pseudonym = pseud;
    informationAboutFriend = NULL;

    setWindowTitle("FriendlyChatClient");

    pButSend = new QPushButton("&Send", this);
    pButFind = new QPushButton("Find friends", this);

    QRect sizeOfMonitor = QApplication::desktop()->screenGeometry();
    setGeometry(600, 0, 600, 600);

    QVBoxLayout *lytVBox = new QVBoxLayout();
    QHBoxLayout *lytHBox = new QHBoxLayout();
    QHBoxLayout *lytHButtonBox = new QHBoxLayout();

    tbwDialogs = new QTabWidget();
    lytVBox->addWidget(tbwDialogs);

    lytHButtonBox->addWidget(pButSend);
    lytHButtonBox->addWidget(pButFind);

    lytVBox->addLayout(lytHButtonBox);
    pButSend->setMaximumSize(50, 40);
    pButSend->setEnabled(false);
    pButFind->setMaximumSize(100, 40);

    lwOfFriends = new QListWidget();
    //listOfFriends->setIconSize(QSize(48, 48));
    lwOfFriends->setSelectionMode(QAbstractItemView::SingleSelection);
    userlist = pSocket->loadUserlist(myId);
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

    connect(pButSend, SIGNAL(clicked()), this, SLOT(slotPrepareSendMessage()));
    connect(pButFind, SIGNAL(clicked()), this, SLOT(slotFindFriends()));
    //connect(listener, SIGNAL(messageRecieved(quint16,quint16,QDateTime,QString)), this, SLOT(slotMessageRecieved(quint16,quint16,QDateTime,QString)));
    connect(pSocket->listener(), SIGNAL(messageRecieved(quint16,quint16,QDateTime,QString)), this, SLOT(slotMessageRecieved(quint16,quint16,QDateTime,QString)), Qt::QueuedConnection);
    connect(pSocket->listener(), SIGNAL(youAddedInUserlist(quint16, QString)), this, SLOT(slotYouAddedInUserlist(quint16, QString)), Qt::QueuedConnection);
    connect(pSocket->socket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

void ChatClient::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(pSocket->socket()->errorString()));
    QMessageBox::critical(0, "ConnectionError", msg);
    //pTEInfo->append(msg); TODO
}

void ChatClient::slotPrepareSendMessage() {
    if (activeDialog->message()->toPlainText() == "")
        return;
    pSocket->talker()->sendMessage(activeDialog->dialog(), myId, pSocket->talker()->currentDateTimeFromServer(), activeDialog->message()->toPlainText());
    activeDialog->message()->clear();
}

void ChatClient::slotMessageRecieved(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString &message) {
    Dialog *dg = NULL;
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->dialog() == dialogNum)
            dg = userlist[i];
    qDebug() << dg;
    if (dg == NULL) {//not found
        User us = pSocket->addUserById(myId, fromId, false);
        userlist.push_back(new User(us));
        dg = userlist.back();
        userlist.back()->setListWidgetItem(new QListWidgetItem(userlist.back()->name(), lwOfFriends));
    }

    qDebug() << "got message " << fromId << endl;
    createTab(dg);

    QString name = (fromId == myId ? "You" : dg->name());
    if (dg->history()->document()->lastBlock().text() != name + " (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message)
        dg->history()->append(name + " (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message);
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
    if (newTab == -1) {
        pButSend->setEnabled(false);
        activeDialog = NULL;
    } else {
        for (int i = 0; i < userlist.size(); ++i)////wrong: dialogs, when will be added conference
            if (userlist[i]->widget() == tbwDialogs->widget(newTab))
                activeDialog = userlist[i];
    }
    qDebug() << "NEW TAB " << newTab << endl;
}

void ChatClient::createTab(Dialog *dg) {
    pButSend->setEnabled(true);
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

    QVector <Message*> history = pSocket->loadHistory(dg);
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

void ChatClient::slotYouAddedInUserlist(quint16 frId, const QString& pseud) {
    qDebug() << "slot YouAddedInUserlist" << frId << pseud;
    newFriend = User(frId, 0, pseud, 0);
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->id() == frId)
            return;

    informationAboutFriend = new QWidget();
    informationAboutFriend->setWindowTitle("Notification");
    QLabel *inftext = new QLabel(pseud + " added you in his userlist. "
                   "Do you want to add " + pseud + " in your userlist?", informationAboutFriend);
    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *lytBut = new QHBoxLayout();
    lyt->addWidget(inftext);
    butYes = new QPushButton("Yes", informationAboutFriend);
    butWrite = new QPushButton("Write", informationAboutFriend);
    butNo = new QPushButton("No", informationAboutFriend);
    lytBut->addWidget(butYes);
    lytBut->addWidget(butWrite);
    lytBut->addWidget(butNo);
    lyt->addLayout(lytBut);
    informationAboutFriend->setLayout(lyt);

    connect(butYes, SIGNAL(clicked()), this, SLOT(slotAddFriend()));
    connect(butWrite, SIGNAL(clicked()), this, SLOT(slotWriteToFriend()));
    connect(butNo, SIGNAL(clicked()), this, SLOT(slotNoAddFriend()));
    informationAboutFriend->show();
}

void ChatClient::slotFindFriends() {
    newFriend = User(0, 0, 0, 0);
    if (informationAboutFriend != NULL) {
        informationAboutFriend->activateWindow();
        return;
    }
    informationAboutFriend = new QWidget();
    informationAboutFriend->setWindowTitle("Find friend");
    inftext = new QLabel(informationAboutFriend);
    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *lytBut = new QHBoxLayout();
    lyt->addWidget(inftext);

    QLabel *lblUserLogin = new QLabel("Enter a login of a friend:", informationAboutFriend);
    leFriendLogin = new QLineEdit(informationAboutFriend);
    lyt->addWidget(lblUserLogin);
    lyt->addWidget(leFriendLogin);

    QPushButton *butFind = new QPushButton("Find", informationAboutFriend);
    butYes = new QPushButton("Add", informationAboutFriend);
    butWrite = new QPushButton("Write", informationAboutFriend);

    lytBut->addWidget(butFind);
    lytBut->addWidget(butYes);
    lytBut->addWidget(butWrite);
    lyt->addLayout(lytBut);
    informationAboutFriend->setLayout(lyt);

    connect(butFind, SIGNAL(clicked()), this, SLOT(slotFindFriend()));
    connect(butYes, SIGNAL(clicked()), this, SLOT(slotAddFriend()));
    connect(butWrite, SIGNAL(clicked()), this, SLOT(slotWriteToFriend()));
    //connect(butNo, SIGNAL(clicked()), this, SLOT(slotNoAddFriend()));
    butYes->setEnabled(false);
    butWrite->setEnabled(false);
    informationAboutFriend->show();
}

void ChatClient::slotFindFriend() {
    newFriend = pSocket->findUser(leFriendLogin->text());
    bool yourFriend = false;
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->id() == newFriend.id())
            yourFriend = true;

    if (newFriend.id() == 0) {
        inftext->setText("User with login " + leFriendLogin->text() + " not found!");
        butYes->setEnabled(false);
        butWrite->setEnabled(false);
    } else if (newFriend.id() != myId){
        butYes->setEnabled(!yourFriend);
        butWrite->setEnabled(true);
        inftext->setText("User with login " + leFriendLogin->text() + " was found!\nHis pseudonym is " + newFriend.name() + "." +
                         (yourFriend ? "\n" + newFriend.name() + " has already added in your userlist." : ""));
    } else {
        butYes->setEnabled(false);
        butWrite->setEnabled(false);
        inftext->setText("User with login " + leFriendLogin->text() + " was found!\nHis pseudonym is " + newFriend.name() + "." +
                         "\nIt is your login.");

    }
}

void ChatClient::slotAddFriend() {
    User fr = pSocket->addUserById(myId, newFriend.id(), true);
    userlist.push_back(new User(fr));
    QListWidgetItem *pointer = new QListWidgetItem(userlist.back()->name(), lwOfFriends);
    userlist.back()->setListWidgetItem(pointer);//pg 214
    delete informationAboutFriend;
    informationAboutFriend = NULL;
}

void ChatClient::slotWriteToFriend() {
    User fr = pSocket->addUserById(myId, newFriend.id(), false);
    Dialog *dg = NULL;
    for (int i = 0; i < userlist.size(); ++i)
        if (userlist[i]->id() == newFriend.id())
            dg = userlist[i];

    if (dg == NULL) {
        userlist.push_back(new User(fr));
        QListWidgetItem *pointer = new QListWidgetItem(userlist.back()->name(), lwOfFriends);
        userlist.back()->setListWidgetItem(pointer);
        dg = userlist.back();
    }

    createTab(dg);
    activateTab(dg);
    delete informationAboutFriend;
    informationAboutFriend = NULL;
}

void ChatClient::slotNoAddFriend() {
    delete informationAboutFriend;
    informationAboutFriend = NULL;
}
