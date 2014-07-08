#include "chatserver.h"

ChatServer::ChatServer(int port, QWidget *widget /*= 0*/):QWidget(widget),
    DRIVER_OF_DATABASE("QMYSQL"), NAME_OF_CONNECTION_TO_DATABASE("connection"),
    NAME_OF_DATABASE("clientinfodb"), LOGIN_TO_DATABASE("root"),
    PASSWORD_TO_DATABASE("v19a70parina"), sizeOfBlock(0) {

    pTCPServer = new QTcpServer(this);
    if (!pTCPServer->listen(QHostAddress("127.0.0.2"), port)) {
    //if (!pTCPServer->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(0, "ServerError", "Unable to start the server: " + pTCPServer->errorString());
        pTCPServer->close();
        return;
    }

    connect(pTCPServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    setGeometry(0, 0, 500, 500);
    pTEMessage = new QTextEdit();
    pTEMessage->setReadOnly(true);
    QVBoxLayout *lyt = new QVBoxLayout();
    lyt->addWidget(new QLabel("<H1>Server</H1>"));
    lyt->addWidget(pTEMessage);
    setLayout(lyt);

    clientinfodb = QSqlDatabase::addDatabase(DRIVER_OF_DATABASE, NAME_OF_CONNECTION_TO_DATABASE);
    clientinfodb.setHostName("localhost");
    clientinfodb.setDatabaseName(NAME_OF_DATABASE);
    clientinfodb.setUserName(LOGIN_TO_DATABASE);
    clientinfodb.setPassword(PASSWORD_TO_DATABASE);

    pTEMessage->append("Server started!");
}


//FOR WORKING ========================================================================================
//====================================================================================================
void ChatServer::insertIntoDatabase(quint16 dialog, quint16 fromId, QDateTime sendTime, const QString& content) {
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

    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    qDebug() << "dialog in database " << dialog << endl;
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

//END OF WORKING =======================================================================================


//COMMANDS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
QByteArray ChatServer::login(const QString& userLogin, const QString& userPassword, QTcpSocket *socket) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::AUTH);

    pTEMessage->append("Try authentication: " + userLogin);
    bool opened = true;
    if (!clientinfodb.isOpen())
        if (!clientinfodb.open()) {
            opened = false;
            out << quint16(ServerFlags::CE_Database) << "Unable to connected to a database: \n" +
                   clientinfodb.lastError().driverText() + "\n" +
                   clientinfodb.lastError().databaseText();
        }
    bool ok = false;
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    if (opened) {
        query.exec("SELECT id, user_password FROM users WHERE user_login = '" + userLogin + "';");
        if (query.size() == 0)
            out << quint16(ServerFlags::AE_Wrong_Login);
        else {
            query.next();
            if (query.value(1).toString() != userPassword)
                out << quint16(ServerFlags::AE_Wrong_Password);
            else ok = true;
        }
    }
    if (ok) {
        out << quint16(ServerFlags::Success_Auth) << quint16(query.value(0).toInt()) << userLogin;
        online.setSocket(query.value(0).toInt(), socket);
    }
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
    QVector <bool> isFriend;
    while (query.next()) {
        usersId.push_back(query.value(0).toInt());
        dialogId.push_back(query.value(1).toInt());
        isFriend.push_back(query.value(2).toBool());
    }
    out << quint16(usersId.size());
    for (int i = 0; i < usersId.size(); ++i) {
        query.exec("SELECT pseudonym FROM users WHERE id=" + QString("%1").arg(usersId[i]) + ";");
        query.next();
        QString pseud = query.value(0).toString();
        out << usersId[i] << dialogId[i] << pseud << isFriend[i];
    }

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
    pTEMessage->append("Send message from " + QString("%1").arg(fromId) + " in dialog " + QString("%1").arg(dialog));
    QVector <QTcpSocket*> members = onlineMembersOfDialog(dialog);
    qDebug() << "members " << members.size();
    for (int i = 0; i < members.size(); ++i)
        sendToClient(members[i], outArray);
    insertIntoDatabase(dialog, fromId, sendTime, content);
    //insert into database
}

QByteArray ChatServer::addUserById(quint16 myId, quint16 frId, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_ID);

    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT pseudonym FROM users WHERE id =%1;").arg(frId));
    query.next();
    QString pseud = query.value(0).toString();
    qDebug() << query.size() << " " << pseud << endl;
    query.exec(QString("SELECT dialog_id FROM userlist_user%1 WHERE friend_id=%2;").arg(frId).arg(myId));
    query.next();
    quint16 dialogId = query.value(0).toInt();
    query.exec(QString("INSER INTO userlist_user%1 VALUE(%2, %3, %4);").arg(myId).arg(frId).arg(dialogId).arg(status));

    out << quint16(frId) << quint16(dialogId) << pseud;
    out.confirm();
    return outArray;
}

QByteArray ChatServer::addUserByLogin(quint16 myId, const QString& frLog, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::ADD_USER_BY_LOGIN);

    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("SELECT id, pseudonym FROM users WHERE user_login=%1").arg(frLog) + ";");
    query.next();
    quint16 dialogId;
    quint16 frId = query.value(0).toInt();
    QString pseud = query.value(1).toString();
    query.exec(QString("SELECT dialog_id FROM userlist_user%1 WHERE friend_id=%2;").arg(myId).arg(frId));
    if (query.size() == 0) {
        query.exec("SELECT COUNT(*) FROM members_dialogs;");
        query.next();
        quint16 numberOfDialogs = query.value(0).toInt();
        //history_dialog1: id, from_id, send_time, content
        query.exec(QString("CREATE TABLE history_dialog%1 ("
                   "`id` integer not null primary key auto_increment,"
                   "`from_id` integer not null,"
                   "`send_time` datetime not null,"
                   "`content` longtext not null);").arg(numberOfDialogs + 1));
        query.exec(QString("INSERT INTO members_dialogs VALUE(%1, '%2 %3');").
                   arg(numberOfDialogs + 1).arg(myId).arg(frId));
        dialogId = numberOfDialogs + 1;
    } else if (query.size() == 1) {
        dialogId = query.value(0).toInt();
        query.exec(QString("INSER INTO userlist_user%1 VALUE(%2, %3, %4);").
                   arg(myId).arg(frId).arg(dialogId).arg(bool(status)));
    } else
        qDebug() << "Strange in addUserByLogin";


    out << quint16(frId) << quint16(dialogId) << pseud << status;
    out.confirm();
    return outArray;
}

void ChatServer::changeStatus(quint16 myId, quint16 frId, bool status) {
    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    query.exec(QString("UPDATE userlist_user%1 SET is_friend=%2 WHERE friend_id=%3;").
               arg(myId).arg(status).arg(frId));
}

QByteArray ChatServer::youAddedInUserlist(quint16 myId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::YOU_ADDED_IN_USERLIST);
    out << quint16(myId);
    out.confirm();
    return outArray;
}

QByteArray ChatServer::loadHistory(quint16 dialogId) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::LOAD_HISTORY);

    QVector <quint16> members = membersOfDialog(dialogId);
    qDebug() << "members " << members.size() << " " << members[0] << " " << members[1];
    QVector <QString> pseuds;
    for (int i = 0; i < members.size(); ++i) {
        QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
        query.exec(QString("SELECT pseudonym FROM users WHERE id=%1;").arg(members[i]));
        query.next();
        pseuds.push_back(query.value(0).toString());
    }

    out << quint16(members.size());
    for (int i = 0; i < members.size(); ++i)
        out << quint16(members[i]) << pseuds[i];

    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
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

//END OF COMMANDS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//INTERFACE OF THE SERVER ============================================================================
//====================================================================================================

void ChatServer::slotNewConnection() {
    QTcpSocket *pSock = pTCPServer->nextPendingConnection();
    qDebug() << "new connection " << pSock << endl;
    connect(pSock, SIGNAL(disconnected()), pSock, SLOT(deleteLater()));
    connect(pSock, SIGNAL(disconnected()), this, SLOT(slotDisconnectedClient()));///
    connect(pSock, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    //sendToClient(pSock, "Server response: connected!");
}

void ChatServer::slotDisconnectedClient() {//TODO
    QTcpSocket *pClientSocket = (QTcpSocket*)sender();
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
        if (typeOfCommand == ServerCommands::AUTH) {
            QString log, pass;
            in >> log >> pass;
            sendToClient(pClientSocket, login(log, pass, pClientSocket));
            return;
        } else if (typeOfCommand == ServerCommands::LOAD_USERLIST) {
            quint16 id;
            in >> id;
            qDebug() << "request server loadUserlist " << id;
            sendToClient(pClientSocket, loadUserlist(id));
            return;
        } else if (typeOfCommand == ServerCommands::SEND_MESSAGE) {
            quint16 dialog, fromId;
            QDateTime timeSend;
            QString content;
            in >> dialog >> fromId >> timeSend >> content;
            sendMessage(dialog, fromId, timeSend, content);
            return;
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_ID) {
            quint16 myId, frId;
            bool status;
            in >> myId >> frId >> status;
            sendToClient(pClientSocket, addUserById(myId, frId, status));
            return;
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_LOGIN) {
            QString log;
            quint16 myId;
            bool status;
            in >> myId >> log >> status;
            sendToClient(pClientSocket, addUserByLogin(myId, log, status));
            sendToClient(pClientSocket, youAddedInUserlist(myId));
            return;
        } else if (typeOfCommand == ServerCommands::CHANGE_STATUS) {
            quint16 myId, frId;
            bool fl;
            in >> myId >> frId >> fl;
            changeStatus(myId, frId, fl);
            return;
        } else if (typeOfCommand == ServerCommands::LOAD_HISTORY) {
            quint16 dialogId;
            in >> dialogId;
            qDebug() << "load history " << dialogId;
            sendToClient(pClientSocket, loadHistory(dialogId));
            return;
        }
    }
}

void ChatServer::sendToClient(QTcpSocket *pClientSock, const QByteArray& bytearray) {
    qDebug() << "send to client " << pClientSock << endl;
    //pTEMessage->append("Authentication -> Send answer." + QString("%1").arg(bytearray.size()));
    pClientSock->write(bytearray);
}

//END OF THE INTERFACE ==============================================================================
