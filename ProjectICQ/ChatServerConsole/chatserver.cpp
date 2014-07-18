#include "chatserver.h"

ChatServer::ChatServer(const QString& host, int port, QObject *widget /*= 0*/):QObject(widget),
    DRIVER_OF_DATABASE("QMYSQL"), NAME_OF_CONNECTION_TO_DATABASE("connection"),
    NAME_OF_DATABASE("clientinfodb"), LOGIN_TO_DATABASE("root"),
    PASSWORD_TO_DATABASE("v19a70parina"), sizeOfBlock(0) {
    pTCPServer = new QTcpServer(this);
    if (!pTCPServer->listen(QHostAddress(host), port)) {
        printf("ServerError: Unable to start the server: %s\n", pTCPServer->errorString().toStdString().c_str());
        //QMessageBox::critical(this, "ServerError", "Unable to start the server: " + pTCPServer->errorString());
        pTCPServer->close();
        return;
    }

    connect(pTCPServer, SIGNAL(newConnection()), this, SLOT(slotConnectionClient()));

    //setGeometry(0, 0, 500, 500);
    /*pTEMessage = new QTextEdit();
    pTEMessage->setReadOnly(true);
    QVBoxLayout *lyt = new QVBoxLayout();
    lyt->addWidget(new QLabel("<H1>Server</H1>"));
    lyt->addWidget(pTEMessage);
    setLayout(lyt);*/

    clientinfodb = QSqlDatabase::addDatabase(DRIVER_OF_DATABASE, NAME_OF_CONNECTION_TO_DATABASE);
    clientinfodb.setHostName("localhost");
    clientinfodb.setDatabaseName(NAME_OF_DATABASE);
    clientinfodb.setUserName(LOGIN_TO_DATABASE);
    clientinfodb.setPassword(PASSWORD_TO_DATABASE);

    printf("Server started!\n");
    //pTEMessage->append("Server started!");
}


//FOR WORKING ========================================================================================
//====================================================================================================
void ChatServer::insertIntoDatabaseMessage(quint16 dialog, quint16 fromId, QDateTime sendTime, const QString& content) {
    if (!clientinfodb.isOpen())
        if (!clientinfodb.open()) {
            qDebug() << "Server: database isn't open!";
            return;
        }
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    bool status = query.exec(QString("INSERT INTO %1 (from_id, send_time, content) VALUES (%2, '%3', '%4');")
            .arg(QString("history_dialog%1").arg(dialog))
            .arg(QString("%1").arg(fromId))
            .arg(sendTime.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString("%1").arg(content)));
    if (!status)
        qDebug() << "Server: Cannot insert into database!";
}

QVector <quint16> ChatServer::membersOfDialog(quint16 dialog) {
    QVector <quint16> members;
    if (!clientinfodb.isOpen())
        if (!clientinfodb.open()) {
            qDebug() << "Server: database isn't open!";
            return members;
        }

    qDebug() << "dialog in database " << dialog << endl;
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    bool status = query.exec(QString("SELECT members FROM members_dialogs WHERE dialog_id=%1;").arg(dialog));
    if (!status) {
        qDebug() << "Server: Cannot insert into database!";
        return members;
    }
    query.next();
    QString str = query.value(0).toString();
    QTextStream input(&str);

    while (!input.atEnd()) {
        int number;
        input >> number;
        members.push_back(number);
    }
    return members;
}

QVector <QTcpSocket*> ChatServer::onlineMembersOfDialog(quint16 dialog) {
    QVector <QTcpSocket*> result;
    QVector <quint16> members = membersOfDialog(dialog);
    for (int i = 0; i < members.size(); ++i) {
        QTcpSocket *res = online.socket(members[i]);
        if (res != NULL)
            result.push_back(res);
    }
    return result;
}

std::pair <quint16, QString> ChatServer::userDescribe(const QString& log) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT id, pseudonym FROM users WHERE user_login='%1';").arg(log));
    if (query.size() == 0)
        return std::make_pair(0, "");
    query.next();
    quint16 frId = query.value(0).toInt();
    QString pseud = query.value(1).toString();
    return std::make_pair(frId, pseud);
}

std::pair <quint16, QString> ChatServer::userDescribe(quint16 id) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT pseudonym FROM users WHERE id='%1';").arg(id));
    if (query.size() == 0)
        return std::make_pair(0, "");
    query.next();
    QString pseud = query.value(0).toString();
    return std::make_pair(id, pseud);
}

void ChatServer::notifyOnOff(quint16 userId, ServerFlags::StatusType flag) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT * FROM inv_user%1;").arg(userId));
    QVector <quint16> memb;
    while (query.next())
        memb.push_back(query.value(0).toInt());
    for (int i = 0; i < memb.size(); ++i)
        if (online.isOnline(memb[i]))
            sendToClient(online.socket(memb[i]), notify(userId, flag));
}

//END OF WORKING =======================================================================================


//COMMANDS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
QByteArray ChatServer::login(const QString& userLogin, const QString& userPassword, QTcpSocket *socket) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::AUTH);

    //QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    //pTEMessage->append("Try authentication: " + userLogin);
    printf("Try authentication %s\n", userLogin.toStdString().c_str());
    bool opened = true;
    if (!clientinfodb.isOpen())
        if (!clientinfodb.open()) {
            opened = false;
            out << quint16(ServerFlags::CE_Database) << "Unable to connected to a database: \n" +
                   clientinfodb.lastError().driverText() + "\n" +
                   clientinfodb.lastError().databaseText();
        }
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    bool ok = false;
    if (opened) {
        query.exec("SELECT id, user_password, pseudonym FROM users WHERE user_login = '" + userLogin + "';");
        if (query.size() == 0)
            out << quint16(ServerFlags::AE_Wrong_Login);
        else {
            query.next();
            if (query.value(1).toString() != userPassword)
                out << quint16(ServerFlags::AE_Wrong_Password);
            else
                ok = true;
        }
    }

    if (ok) {
        quint16 userId = query.value(0).toInt();
        printf("Authenticated %s\n", userLogin.toStdString().c_str());
        //pTEMessage->append("Authenticated " + userLogin);
        out << quint16(ServerFlags::Success_Auth) << quint16(userId) << query.value(2).toString();
        online.setSocket(userId, socket);
        notifyOnOff(userId, ServerFlags::UserOnline);
    }
    out.confirm();
    return outArray;
}

QByteArray ChatServer::registerUser(const QString& userLogin, const QString &pseud, const QString &pass, QTcpSocket *socket) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::REGISTER_USER);
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT * FROM users WHERE user_login ='%1';").arg(userLogin));
    qDebug() << query.size();
    if (query.size() == 1) {
        out << quint16(0);
        out.confirm();
        return outArray;
    }

    query.exec(QString("INSERT INTO users (user_login, user_password, time_reg, pseudonym) VALUE('%1', '%2', '%3', '%4');").
               arg(userLogin).arg(pass).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(pseud));
    query.exec(QString("SELECT id FROM users WHERE user_login = '%1';").arg(userLogin));
    query.next();
    quint16 numberOfUser = query.value(0).toInt();
    qDebug() << numberOfUser;

    query.exec(QString("CREATE TABLE userlist_user%1 ("
               "`friend_id` integer not null primary key,"
               "`dialog_id` integer not null unique key,"
               "`is_friend` bool not null);").arg(numberOfUser));

    query.exec(QString("CREATE TABLE inv_user%1 (`friend_id` integer not null unique key);").arg(numberOfUser));

    query.exec(QString("CREATE TABLE notify_user%1 (`type` integer not null, `field1` integer not null default 0, `field2` integer not null default 0);").arg(numberOfUser));

    printf("Authenticated %s\n", userLogin.toStdString().c_str());
    online.setSocket(numberOfUser, socket);
    out << quint16(numberOfUser);
    out.confirm();
    return outArray;

}

QByteArray ChatServer::loadUserlist(int userId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_USERLIST);
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT * FROM userlist_user%1;").arg(userId));
    QVector <int> usersId, dialogId;
    QVector <bool> friendStatus;
    while (query.next()) {
        usersId.push_back(query.value(0).toInt());
        dialogId.push_back(query.value(1).toInt());
        friendStatus.push_back(query.value(2).toBool());
    }

    out << quint16(usersId.size());
    for (int i = 0; i < usersId.size(); ++i) {
        query.exec("SELECT pseudonym FROM users WHERE id=" + QString("%1").arg(usersId[i]) + ";");
        query.next();
        QString pseud = query.value(0).toString();
        out << usersId[i] << dialogId[i] << pseud << friendStatus[i] << online.isOnline(usersId[i]);
    }
    out.confirm();
    return outArray;
}

QByteArray ChatServer::loadNotifys(int userId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_NOTIFYS);
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT * FROM notify_user%1;").arg(userId));
    out << int(query.size());
    qDebug() << int(query.size());
    while (query.next()) {
        int type = query.value(0).toInt();
        if (type == ServerFlags::RequestAddToFriends) {
            int id = query.value(1).toInt();
            QString pseud = userDescribe(id).second;
            out << type << id << pseud;
        } else if (type == ServerFlags::UnreadMessages)
            out << type << query.value(1).toInt() << query.value(2).toInt();
    }

    out.confirm();
    return outArray;
}

QByteArray ChatServer::currentDateTime() {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::TIME_SERVER);
    out << QDateTime::currentDateTime();
    out.confirm();
    return outArray;
}

void ChatServer::sendMessage(quint16 dialog, quint16 fromId, QDateTime sendTime, const QString& content) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::SEND_MESSAGE);
    out << dialog << fromId << sendTime << content;
    out.confirm();
    printf("Send message from %s in dialog %d\n", userDescribe(fromId).second.toStdString().c_str(), dialog);
    //pTEMessage->append("Send message from " + QString("%1").arg(userId(fromId).second) + " in dialog " + QString("%1").arg(dialog));
    QVector <QTcpSocket*> members = onlineMembersOfDialog(dialog);
    QTcpSocket *senderSocket = online.socket(fromId);
    for (int i = 0; i < members.size(); ++i)
        if (senderSocket != members[i])
            sendToClient(members[i], outArray);

    //insert into database
    insertIntoDatabaseMessage(dialog, fromId, sendTime, content);

    QVector <quint16> membs = membersOfDialog(dialog);
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    for (int i = 0; i < membs.size(); ++i) {
        if (membs[i] == fromId)
            continue;
        query.exec(QString("SELECT * FROM notify_user%1 WHERE field1 = %2;").arg(membs[i]).arg(dialog));
        if (query.size() == 1)
            query.exec(QString("UPDATE notify_user%1 SET field2=field2+1 WHERE field1=%2;").arg(membs[i]).arg(dialog));
        else
            query.exec(QString("INSERT INTO notify_user%1 (type, field1, field2) VALUE(%2, %3, %4);").arg(membs[i]).arg(ServerFlags::UnreadMessages).arg(dialog).arg(1));
    }
}

QByteArray ChatServer::addUser(quint16 myId, std::pair <quint16, QString> fr, int status, int typeAdd) {//TODO
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("DELETE FROM notify_user%1 WHERE field1=%2;").arg(myId).arg(fr.first));
    quint16 dialogId;
    if (status == ServerFlags::Discard)
        dialogId = 0;
    else {
        query.exec(QString("SELECT dialog_id FROM userlist_user%1 WHERE friend_id=%2;").arg(fr.first).arg(myId));

        if (query.size() == 0) {//doesn't exists
            query.exec("SELECT MAX(dialog_id) AS mx FROM members_dialogs;");
            query.next();
            dialogId = query.value(0).toInt() + 1;
            //history_dialog1: id, from_id, send_time, content
            query.exec(QString("CREATE TABLE history_dialog%1 ("
                       "`id` integer not null primary key auto_increment,"
                       "`from_id` integer not null,"
                       "`send_time` datetime not null,"
                       "`content` longtext not null);").arg(dialogId));

            query.exec(QString("ALTER TABLE history_dialog%1 CONVERT TO CHARACTER SET utf8;").arg(dialogId));

            query.exec(QString("INSERT INTO members_dialogs VALUE(%1, '%2 %3');").
                       arg(dialogId).arg(myId).arg(fr.first));

            query.exec(QString("INSERT INTO userlist_user%1 VALUE(%2, %3, %4);").
                       arg(myId).arg(fr.first).arg(dialogId).arg(bool(status)));
        } else if (query.size() == 1) {
            query.next();
            dialogId = query.value(0).toInt();
            query.exec(QString("INSERT INTO userlist_user%1 VALUE(%2, %3, %4);").
                       arg(myId).arg(fr.first).arg(dialogId).arg(bool(status)));
        } else
            qDebug() << "Strange in addUser";

        if (status == ServerFlags::Friend)
            query.exec(QString("INSERT INTO inv_user%1 VALUE(%2);").arg(fr.first).arg(myId));
    }

    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(typeAdd);
    out << quint16(fr.first) << quint16(dialogId) << fr.second << status << online.isOnline(fr.first);
    out.confirm();
    return outArray;

}

void ChatServer::changeStatus(quint16 myId, quint16 frId, int status) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("UPDATE userlist_user%1 SET is_friend=%2 WHERE friend_id=%3;").
               arg(myId).arg(status).arg(frId));
}

void ChatServer::readMessageNotify(int myId, int dialog) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("DELETE FROM notify_user%1 WHERE field1=%2;").
               arg(myId).arg(dialog));
}

void ChatServer::youAddedInUserlist(std::pair <quint16, QString> me, quint16 frId) {//TODO
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT * FROM userlist_user%1 WHERE friend_id=%2;").arg(frId).arg(me.first));
    int sz = query.size();
    query.next();
    if (sz == 1 && query.value(2).toBool())//if in friendlist
        return;

    query.exec(QString("INSERT INTO notify_user%1 (type, field1) VALUE(%2, %3);").arg(frId).arg(ServerFlags::RequestAddToFriends).arg(me.first));
    if (online.isOnline(frId)) {
        QByteArray outArray;
        BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_5);
        out << quint16(ServerCommands::YOU_ADDED_IN_USERLIST);
        out << quint16(me.first) << me.second;
        out.confirm();
        sendToClient(online.socket(frId), outArray);
    }
}

QByteArray ChatServer::loadHistory(quint16 dialogId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_HISTORY);
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));

    QVector <quint16> members = membersOfDialog(dialogId);
    qDebug() << "members " << members.size() << " " << members[0] << " " << members[1];
    QVector <QString> pseuds;
    for (int i = 0; i < members.size(); ++i) {
        query.exec(QString("SELECT pseudonym FROM users WHERE id=%1;").arg(members[i]));
        query.next();
        pseuds.push_back(query.value(0).toString());
    }

    out << quint16(members.size());
    for (int i = 0; i < members.size(); ++i)
        out << quint16(members[i]) << pseuds[i];

    query.exec(QString("SELECT * FROM history_dialog%1;").arg(dialogId));
    quint16 size = query.size();
    out << quint16(size);
    qDebug() << "size of history = " << size;
    while (query.next()) {
        quint16 id = query.value(0).toInt();
        quint16 fromId = query.value(1).toInt();
        QDateTime sendTime = query.value(2).toDateTime();
        QString content = query.value(3).toString();
        out << id << fromId << sendTime << content;
    }

    out.confirm();
    return outArray;
    //history_dialog1: id, from_id, send_time, content
    //members_dialogs: dialog_id, members[i1, i2, ...]
}

QByteArray ChatServer::findUser(const QString& log) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::FIND_USER);
    std::pair <quint16, QString> us = userDescribe(log);
    qDebug() << "find user = " << us.first << endl;
    out << quint16(us.first) << us.second << online.isOnline(us.first);
    out.confirm();
    return outArray;
}

QByteArray ChatServer::notify(quint16 userId, ServerFlags::StatusType flag) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::NOTIFY_ON_OFF);
    out << userId << quint16(flag);
    out.confirm();
    return outArray;
}

//END OF COMMANDS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


void ChatServer::slotConnectionClient() {
    QTcpSocket *pSock = pTCPServer->nextPendingConnection();
    qDebug() << "new connection " << pSock << endl;
    connect(pSock, SIGNAL(disconnected()), pSock, SLOT(deleteLater()));
    connect(pSock, SIGNAL(disconnected()), this, SLOT(slotDisconnectedClient()));///
    connect(pSock, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    //sendToClient(pSock, "Server response: connected!");
}

void ChatServer::slotDisconnectedClient() {//TODO
    QTcpSocket *pClientSocket = (QTcpSocket*)sender();
    int idus = online.userId(pClientSocket);
    printf("Logoff %s\n", userDescribe(idus).second.toStdString().c_str());
    //pTEMessage->append(QString("Logoff %1").arg(userId(idus).second));
    notifyOnOff(online.userId(pClientSocket), ServerFlags::UserOffline);
    online.remove(pClientSocket);
}

void ChatServer::slotReadClient() {
    QTcpSocket *pClientSocket = (QTcpSocket*)sender();
    BytesReaderWriter in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_5);

    for (;;) {//for several packages
        if (sizeOfBlock == 0) {//unknown size of block
            if (pClientSocket->bytesAvailable() < sizeof(quint16))//if sizeOfBlock doesnt hold in package
                break;
            in >> sizeOfBlock;
        }

        if (pClientSocket->bytesAvailable() < sizeOfBlock)//if all data doesnt received yet
            break;
        sizeOfBlock = 0;
        quint16 typeOfCommand;
        in >> typeOfCommand;
        qDebug() << "Type of command on server = " << typeOfCommand;
        if (typeOfCommand == ServerCommands::AUTH) {
            QString log, pass;
            in >> log >> pass;
            sendToClient(pClientSocket, login(log, pass, pClientSocket));
        } else if (typeOfCommand == ServerCommands::LOAD_USERLIST) {
            quint16 id;
            in >> id;
            sendToClient(pClientSocket, loadUserlist(id));
        } else if (typeOfCommand == ServerCommands::SEND_MESSAGE) {
            quint16 dialog, fromId;
            QDateTime timeSend;
            QString content;
            in >> dialog >> fromId >> timeSend >> content;
            sendMessage(dialog, fromId, QDateTime::currentDateTime(), content);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_ID) {
            quint16 myId, frId;
            int status;
            in >> myId >> frId >> status;
            std::pair <quint16, QString> fr = userDescribe(frId);
            std::pair <quint16, QString> me = userDescribe(myId);
            sendToClient(pClientSocket, addUser(myId, fr, status, ServerCommands::ADD_USER_BY_ID));
            if (status == ServerFlags::Friend) youAddedInUserlist(me, fr.first);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_LOGIN) {
            QString logFr;
            quint16 myId;
            int status;
            in >> myId >> logFr >> status;
            std::pair <quint16, QString> fr = userDescribe(logFr);
            std::pair <quint16, QString> me = userDescribe(myId);
            sendToClient(pClientSocket, addUser(myId, fr, status, ServerCommands::ADD_USER_BY_LOGIN));
            if (status == ServerFlags::Friend) youAddedInUserlist(me, fr.first);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_DIALOG) {
            quint16 myId, dial, frId;
            int status;
            in >> myId >> dial >> status;
            QVector <quint16> memb = membersOfDialog(dial);
            if (memb[0] == myId) frId = memb[1];
            else frId = memb[0];
            std::pair <quint16, QString> fr = userDescribe(frId);
            std::pair <quint16, QString> me = userDescribe(myId);
            sendToClient(pClientSocket, addUser(myId, fr, status, ServerCommands::ADD_USER_BY_DIALOG));
            if (status == ServerFlags::Friend) youAddedInUserlist(me, fr.first);
        } else if (typeOfCommand == ServerCommands::CHANGE_STATUS_FRIEND) {
            quint16 myId, frId;
            int fl;
            in >> myId >> frId >> fl;
            changeStatus(myId, frId, fl);
        } else if (typeOfCommand == ServerCommands::LOAD_HISTORY) {
            quint16 dialogId;
            in >> dialogId;
            qDebug() << "load history " << dialogId;
            sendToClient(pClientSocket, loadHistory(dialogId));
            //return;
        } else if (typeOfCommand == ServerCommands::REGISTER_USER) {
            QString userLogin, pseud, pass;
            in >> userLogin >> pseud >> pass;
            sendToClient(pClientSocket, registerUser(userLogin, pseud, pass, pClientSocket));
            //return;
        } else if (typeOfCommand == ServerCommands::FIND_USER) {
            QString log;
            in >> log;
            sendToClient(pClientSocket, findUser(log));
            //return;
        } else if (typeOfCommand == ServerCommands::LOAD_NOTIFYS) {
            int id;
            in >> id;
            sendToClient(pClientSocket, loadNotifys(id));
        } else if (typeOfCommand == ServerCommands::READ_MESSAGE_NOTIFY) {
            int usId, dialog;
            in >> usId >> dialog;
            readMessageNotify(usId, dialog);
        } else if (typeOfCommand == ServerCommands::TIME_SERVER)
            sendToClient(pClientSocket, currentDateTime());
    }
}

void ChatServer::sendToClient(QTcpSocket *pClientSock, const QByteArray& bytearray) {
    //pTEMessage->append("Authentication -> Send answer." + QString("%1").arg(bytearray.size()));
    pClientSock->write(bytearray);
}


//END OF THE INTERFACE ==============================================================================
