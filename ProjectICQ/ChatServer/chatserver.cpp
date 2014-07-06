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


QVector <QTcpSocket*> ChatServer::membersOfDialog(int dialog) {
    QVector <QTcpSocket*> result;
    if (!clientinfodb.isOpen())
        if (!clientinfodb.open()) {
            qDebug() << "Server: database isn't open!";
            return result;
        }

    QSqlQuery query(QSqlDatabase::database(NAME_OF_CONNECTION_TO_DATABASE));
    qDebug() << "dialog in database " << dialog << endl;
    bool status = query.exec(QString("SELECT members FROM members_dialogs WHERE id_dialog=%1;").arg(dialog));
    if (!status) {
        qDebug() << "Server: Cannot insert into database!";
        return result;
    }

    query.next();
    QString str = query.value(0).toString();
    QTextStream input(&str);

    QVector <int> members;
    while (!input.atEnd()) {
        int number;
        input >> number;
        members.push_back(number);
    }

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
    query.exec("SELECT * FROM userlist_user" + QString("%1").arg(userId) + ";");
    QVector <int> usersId, dialogId;
    QVector <bool> isFriend;
    while (query.next()) {
        usersId.push_back(query.value(0).toInt());
        dialogId.push_back(query.value(1).toInt());
        isFriend.push_back(query.value(2).toBool());
    }
    out << quint16(usersId.size());
    for (int i = 0; i < usersId.size(); ++i) {
        query.exec("SELECT user_login, pseudonym FROM users WHERE id=" + QString("%1").arg(usersId[i]) + ";");
        query.next();
        QString userLogin = query.value(0).toString();
        QString pseud = query.value(1).toString();
        out << usersId[i] << userLogin << dialogId[i] << pseud << isFriend[i];
        //fr.push_back(new User(userId[i], userLogin, dialogId[i], pseud, isFriend[i]));
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
    QVector <QTcpSocket*> members = membersOfDialog(dialog);
    qDebug() << "members " << members.size();
    for (int i = 0; i < members.size(); ++i)
        sendToClient(members[i], outArray);
    insertIntoDatabase(dialog, fromId, sendTime, content);
    //insert into database
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
            sizeOfBlock = in.readT<quint16>();
        }

        if (pClientSocket->bytesAvailable() < sizeOfBlock)//if all data doesnt received yet
            break;
        sizeOfBlock = 0;
        quint16 typeOfCommand = in.readT<quint16>();
        if (typeOfCommand == ServerCommands::AUTH) {
            QString log = in.readT<QString>();
            QString pass = in.readT<QString>();
            sendToClient(pClientSocket, login(log, pass, pClientSocket));
            return;
        } else if (typeOfCommand == ServerCommands::LOAD_USERLIST) {
            quint16 id = in.readT<quint16>();
            sendToClient(pClientSocket, loadUserlist(id));
            return;
        } else if (typeOfCommand == ServerCommands::SEND_MESSAGE) {
            quint16 dialog = in.readT<quint16>();
            quint16 fromId = in.readT<quint16>();
            QDateTime timeSend = in.readT<QDateTime>();
            QString content = in.readT<QString>();
            sendMessage(dialog, fromId, timeSend, content);
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
