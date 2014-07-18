#include "chatclient.h"

ChatClient::ChatClient(ClientSocket *sockett, const QString& strHost, int nPort, int userIdx, const QString& pseud, QWidget *pwig):QWidget(pwig),
    userlist(new UserListWidget()), dialogFindFriends(NULL) {
    qDebug() << "chat client\n";
    pSocket = sockett;

    myId = userIdx;
    pseudonym = pseud;
    //infoAboutFriend = NULL;

    dialogFindFriends = new DialogFindFriends(this);
    connect(dialogFindFriends, SIGNAL(findFriend(QString)), this, SLOT(slotFindFriend(QString)));
    connect(dialogFindFriends, SIGNAL(add(User)), this, SLOT(slotAddFriend(User)));
    connect(dialogFindFriends, SIGNAL(write(User)), this, SLOT(slotWriteToFriend(User)));

    setWindowTitle("FriendlyChatClient");

    pButSend = new QPushButton("&Send", this);
    pButFind = new QPushButton("Find friends", this);

    QRect sizeOfMonitor = QApplication::desktop()->screenGeometry();
    setGeometry(600, 0, 600, 600);

    QVBoxLayout *lytVBox = new QVBoxLayout();
    QHBoxLayout *lytHBox = new QHBoxLayout();
    QHBoxLayout *lytHButtonBox = new QHBoxLayout();

    tbwDialogs = new QTabWidget();///
    lytVBox->addWidget(tbwDialogs);

    lytHButtonBox->addWidget(pButSend);
    lytHButtonBox->addWidget(pButFind);

    lytVBox->addLayout(lytHButtonBox);
    pButSend->setMaximumSize(50, 40);
    pButSend->setEnabled(false);
    pButFind->setMaximumSize(100, 40);

    //listOfFriends->setIconSize(QSize(48, 48));
    userlist->setSelectionMode(QAbstractItemView::SingleSelection);
    userlist->add(pSocket->loadUserlist(myId));
    notifs = Notification::convert(pSocket->loadNotifys(myId));

    connect(userlist, SIGNAL(doubleClickUser(Dialog*)), this, SLOT(slotDoubleClickDialog(Dialog*)));
    //pItem1->setBackgroundColor(QColor(255, 0, 0));
    //pItem1->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    lytHBox->addLayout(lytVBox, 2);
    lytHBox->addWidget(userlist);
    setLayout(lytHBox);
    tbwDialogs->setMovable(true);
    tbwDialogs->setTabsClosable(true);
    connect(tbwDialogs, SIGNAL(tabCloseRequested(int)), this, SLOT(slotTabClosed(int)));
    connect(tbwDialogs, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)));

    connect(pButSend, SIGNAL(clicked()), this, SLOT(slotPrepareSendMessage()));
    connect(pButFind, SIGNAL(clicked()), this, SLOT(slotFindFriendsOpen()));
    //connect(listener, SIGNAL(messageRecieved(quint16,quint16,QDateTime,QString)), this, SLOT(slotMessageRecieved(quint16,quint16,QDateTime,QString)));
    connect(pSocket->listener(), SIGNAL(messageRecieved(quint16,quint16,QDateTime,QString)), this, SLOT(slotMessageRecieved(quint16,quint16,QDateTime,QString)), Qt::QueuedConnection);
    connect(pSocket->listener(), SIGNAL(youAddedInUserlist(quint16, QString)), this, SLOT(slotYouAreAddedInUserlist(quint16, QString)), Qt::QueuedConnection);
    connect(pSocket->listener(), SIGNAL(notifyOnOff(quint16,bool)), this, SLOT(slotNotifyOnOff(quint16, bool)));
    connect(pSocket->socket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    pSocket->initDateTime();
    showNotifications();
}

void ChatClient::showNotifications() {
    qDebug() << "show notif";
    for (int i = 0; i < notifs.size(); ++i)
        if (notifs[i]->type() == ServerFlags::RequestAddToFriends)
            slotYouAreAddedInUserlist(notifs[i]->field[1].toInt(), notifs[i]->field[2].toString());
        else if (notifs[i]->type() == ServerFlags::UnreadMessages) {
            User *us = userlist->userByDialog(notifs[i]->field[1].toInt());
            qDebug() << "US = " << us;
            if (us == NULL) {
                us = new User(pSocket->addUserByDialog(myId, notifs[i]->field[1].toInt(), ServerFlags::InUserlist));
                userlist->add(us);
            }
            us->setUnreadMessage(notifs[i]->field[2].toInt());
        }
        qDebug() << "out fron notif";
}

void ChatClient::slotError(QAbstractSocket::SocketError err) {
    QString msg = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." :
                               err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." :
                               err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
                                                                                QString(pSocket->socket()->errorString()));
    qDebug() << "INNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN ERROR";
    QMessageBox::critical(this, "ConnectionError", msg);
    //pTEInfo->append(msg); TODO
}

void ChatClient::slotPrepareSendMessage() {
    QString txt = activeDialog->message()->toPlainText();
    int i = txt.size() - 1;
    while (i >= 0 && txt[i] == ' ') --i;
    txt.remove(i + 1, txt.size() - i - 1);
    if (txt == "")
        return;
    QDateTime sendTime = pSocket->currentDateTimeFromServer();
    appendHistory(activeDialog, "You", sendTime, txt);
    pSocket->talker()->sendMessage(activeDialog->dialog(), myId, sendTime, txt);
    //activeDialog->history()->append(activeDialog->message()->toPlainText());////wrong
    activeDialog->message()->clear();
}

void ChatClient::slotMessageRecieved(quint16 dialogNum, quint16 fromId, QDateTime sendTime, const QString &message) {
    Dialog *dg = userlist->userByDialog(dialogNum);////WRONG
    if (dg == activeDialog) {
        QString name = (fromId == myId ? "You" : dg->name());
        appendHistory(dg, name, sendTime, message);
        pSocket->talker()->readMessageNotify(myId, dg->dialog());
        return;
    }

    if (dg == NULL) {//not found
        User *us = new User(pSocket->addUserById(myId, fromId, ServerFlags::InUserlist));
        dg = us;
        userlist->add(us);
    }
    createDialog(dg);
    dg->setUnreadMessage(dg->unread() + 1);
    QString name = (fromId == myId ? "You" : dg->name());
    appendHistory(dg, name, sendTime, message);
}

void ChatClient::slotDoubleClickDialog(Dialog *dg) {
    createDialog(dg);
    activateTab(dg);
    dg->setUnreadMessage(0);
    pSocket->talker()->readMessageNotify(myId, dg->dialog());
}

void ChatClient::slotTabClosed(int tab) {
    tbwDialogs->removeTab(tab);
}

void ChatClient::slotCurrentTabChanged(int newTab) {
    if (newTab == -1) {
        pButSend->setEnabled(false);
        activeDialog = NULL;
    } else {
        activeDialog = userlist->userByWidget(tbwDialogs->widget(newTab));
        activeDialog->setUnreadMessage(0);
        pSocket->talker()->readMessageNotify(myId, activeDialog->dialog());
    }
}

void ChatClient::createDialog(Dialog *dg) {
    if (dg->widget() != NULL)
        return;
    dg->createWidget();
    connect(dg->message(), SIGNAL(enter()), this, SLOT(slotPrepareSendMessage()));
    QVector <Message*> history = Message::convert(pSocket->loadHistory(dg));
    for (int i = 0; i < history.size(); ++i) {
        QString name = (history[i]->from() == myId ? "You" : dg->name());
        appendHistory(dg, name, history[i]->time(), history[i]->message());
    }
    for (int i = 0; i < history.size(); ++i)
        delete history[i];
    //tbwDialogs->addTab(dg->widget(), dg->name());
}

void ChatClient::activateTab(Dialog *dg) {
    pButSend->setEnabled(true);
    activeDialog = dg;
    for (int i = 0; i < tbwDialogs->count(); ++i)
        if (tbwDialogs->widget(i) == dg->widget()) {
            tbwDialogs->setCurrentIndex(i);
            return;
        }
    tbwDialogs->addTab(dg->widget(), dg->name());
    tbwDialogs->setCurrentIndex(tbwDialogs->count() - 1);
}

void ChatClient::slotYouAreAddedInUserlist(quint16 frId, const QString& pseud) {
    if (userlist->userById(frId) != NULL)
        return;

    //newFriend = User(frId, 0, pseud, 0, 0);
    NotificationForm *notAdd = new NotificationForm(frId, pseud, this);
    connect(notAdd, SIGNAL(yes(User)), this, SLOT(slotAddFriend(User)));
    connect(notAdd, SIGNAL(write(User)), this, SLOT(slotWriteToFriend(User)));
    connect(notAdd, SIGNAL(no(User)), this, SLOT(slotNoAddFriend(User)));
    notAdd->show();
    //where wrong
}

void ChatClient::slotFindFriendsOpen() {
    dialogFindFriends->show();
    dialogFindFriends->activateWindow();
}

void ChatClient::slotFindFriend(const QString& name) {
    qDebug() << "slot find friend";
    User fndUser = pSocket->findUser(name);
    bool yourFriend = userlist->userById(fndUser.id()) != NULL; //false;
    dialogFindFriends->setFocusUser(fndUser);
    if (fndUser.id() == 0) {
        dialogFindFriends->buttonAdd()->setEnabled(false);
        dialogFindFriends->buttonWrite()->setEnabled(false);
        QString msg = "<font size = 5 color = \"red\" >User with login <font color=\"black\">" + name + "</font> not found!</font>";
        dialogFindFriends->info()->setText(msg);
    } else if (fndUser.id() != myId){
        dialogFindFriends->buttonAdd()->setEnabled(!yourFriend);
        dialogFindFriends->buttonWrite()->setEnabled(true);
        QString msg = "<font size = 5 color=\"green\"> User with login <font color=\"black\">" + name + "</font> was found!</font>"
                "<br>His pseudonym is <b>" + fndUser.name() + "</b>." +
                (yourFriend ? "<br><b>" + fndUser.name() + "</b> has already added in your userlist." : "");
        dialogFindFriends->info()->setText(msg);
    } else {
        dialogFindFriends->buttonAdd()->setEnabled(false);
        dialogFindFriends->buttonWrite()->setEnabled(false);
        QString msg = "<font size = 5 color=\"green\"> User with login <font color=\"black\">" + name + "</font> was found!</font>"
                "<br>His pseudonym is <b>" + fndUser.name() + "</b>. <br>It is your login.";
        dialogFindFriends->info()->setText(msg);

    }
}


void ChatClient::slotAddFriend(User us) {
    User *fndUs = userlist->userById(us.id());
    if (fndUs != NULL) {
        pSocket->talker()->changeStatus(myId, us.id(), ServerFlags::Friend);
        fndUs->setFriend(true);
        return;
    }
    User fr = pSocket->addUserById(myId, us.id(), ServerFlags::Friend);
    userlist->add(new User(fr));
    dialogFindFriends->close();
}

void ChatClient::slotWriteToFriend(User us) {
    User fr = pSocket->addUserById(myId, us.id(), ServerFlags::InUserlist);
    Dialog *dg = userlist->userById(us.id());

    if (dg == NULL) {
        User *addUs = new User(fr);
        userlist->add(addUs);
        dg = addUs;
    }

    createDialog(dg);
    activateTab(dg);
    dialogFindFriends->close();
}

void ChatClient::slotNoAddFriend(User us) {
    NotificationForm *send = (NotificationForm*)sender();
    send->close();
    pSocket->addUserById(myId, us.id(), ServerFlags::Discard);
}


void ChatClient::slotNotifyOnOff(quint16 userId, bool stat) {
    User *us = userlist->userById(userId);
    us->setOnline(stat);
    userlist->updateStatus(us);
}

void ChatClient::appendHistory(Dialog* dg, const QString& name, const QDateTime& sendTime, const QString& message) {
    QString txt = "<b>" + name + "</b> (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "): " + message;
    if (dg->history()->document()->lastBlock().text() != txt)
        dg->history()->append(txt);
}


//void ChatClient::keyPressEvent(QKeyEvent *e) {
//    if ((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) && (e->modifiers() & Qt::ControlModifier))
//        return;
//    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
//        qDebug() << "enter";
//    else
//        QWidget::keyPressEvent(e);
//}

ChatClient::~ChatClient() {
    if (userlist != NULL)
        delete userlist;
    if (tbwDialogs != NULL)
        delete tbwDialogs;
}
