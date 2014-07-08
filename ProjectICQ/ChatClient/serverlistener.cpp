#include "serverlistener.h"

ServerListener::ServerListener(QTcpSocket *socket):sizeOfBlock(0), fullStreamRecieved(false) {
    pSocket = socket;
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(slotReadServer()));
}

void ServerListener::slotReadServer() {//read from the server
    qDebug() << "read from the server";
    BytesReaderWriter in(pSocket);
    in.setVersion(QDataStream::Qt_4_5);
    fullStreamRecieved = false;
    for (;;) {
        if (!sizeOfBlock) {
            if (pSocket->bytesAvailable() < sizeof(quint16))
                break;
            in >> sizeOfBlock;
        }

        if (pSocket->bytesAvailable() < sizeOfBlock)
            break;
        sizeOfBlock = 0;
        fullStreamRecieved = true;
        quint16 typeOfCommand;
        in >> typeOfCommand;
        qDebug() << "typeOfCommand = " << typeOfCommand;
        if (typeOfCommand == ServerCommands::SEND_MESSAGE) {
            quint16 dg, from;
            QDateTime sendTime;
            QString content;
            in >> dg >> from >> sendTime >> content;
            emit messageRecieved(dg, from, sendTime, content);
        } else if (typeOfCommand == ServerCommands::YOU_ADDED_IN_USERLIST) {
            quint16 userId;
            in >> userId;
            emit youAddedInUserlist(userId);
        } else if (typeOfCommand == ServerCommands::LOAD_USERLIST) {
            quint16 numberOfFriends;
            QVector <User*> us;
            in >> numberOfFriends;
            qDebug() << "load userlist in listener " << numberOfFriends << endl;
            for (int i = 0; i < numberOfFriends; ++i) {
                int uid, did;
                QString pseud;
                bool isfr;
                in >> uid >> did >> pseud >> isfr;
                us.push_back(new User(uid, did, pseud, isfr));
            }
            emit userlistRecieved(us);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_ID) {
            quint16 frId, dialog;
            QString pseud;
            in >> frId >> dialog >> pseud;
            emit userAdded(frId, dialog, pseud, 0);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_LOGIN) {
            quint16 frId, dialog;
            QString pseud;
            in >> frId >> dialog >> pseud;
            bool status;
            emit userAdded(frId, dialog, pseud, status);
        } else if (typeOfCommand == ServerCommands::LOAD_HISTORY) {
            quint16 numberOfMessages, numbersOfUserIdDialog;
            in >> numbersOfUserIdDialog;
            QVector <Message*> history;
            std::map <quint16, QString> pseuds;
            for (int i = 0; i < numbersOfUserIdDialog; ++i) {
                quint16 id;
                QString name;
                in >> id >> name;
                pseuds[id] = name;
            }
            in >> numberOfMessages;
            qDebug() << "dialog = " << numbersOfUserIdDialog << " " << numberOfMessages << endl;
            for (int i = 0; i < numberOfMessages; ++i) {
                quint16 id, fromId;
                QString content;
                QDateTime sendTime;
                in >> id >> fromId >> sendTime >> content;
                history.push_back(new Message(id, fromId, pseuds[fromId], sendTime, content));
            }
            emit historyRecieved(history);
        }
    }
    qDebug() << sizeOfBlock;
}


bool ServerListener::waitFullStream(int mills) {
    qDebug() << "wait full stream";
    bool ret = false;
    while (!fullStreamRecieved && pSocket->waitForReadyRead(mills)) {
        qDebug() << "innnnn wait";
        slotReadServer();
        if (fullStreamRecieved)
            ret = true;
    }
    fullStreamRecieved = false;
    return ret;
}
