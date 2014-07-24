#include "serverlistener.h"

#include "notification.h"

ServerListener::ServerListener(QTcpSocket *socket):sizeOfBlock(0), fullStreamReceived(false) {
    pSocket = socket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadServer()));
}

void ServerListener::slotReadServer() {//read from the server
    qDebug() << "read from the server";
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
        fullStreamReceived = true;
        quint16 typeOfCommand;
        in >> typeOfCommand;
        qDebug() << "typeOfCommand = " << typeOfCommand;
        if (typeOfCommand == ServerCommands::AUTH) {//Qt::Que
            quint16 status, userId;
            QString pseud, msg;
            in >> status >> userId >> pseud >> msg;
            qDebug() << "auth\n";
            emit tryAuthenticate(status, userId, pseud, msg);
       } else if (typeOfCommand == ServerCommands::LOAD_USERLIST) {//stoped
            quint16 numberOfFriends;
            QVector <User*> us;
            in >> numberOfFriends;
            qDebug() << "load userlist in listener " << numberOfFriends;
            for (int i = 0; i < numberOfFriends; ++i) {
                int uid, did;
                QString pseud;
                bool isfr, stat;
                in >> uid >> did >> pseud >> isfr >> stat;
                us.push_back(new User(uid, did, pseud, isfr, stat));
            }
            emit userlistReceived(us);
        } else if (typeOfCommand == ServerCommands::LOAD_HISTORY) {//stoped
            quint16 numberOfMessages, numbersOfUserIdDialog;
            in >> numbersOfUserIdDialog;
            QVector <Message> history;
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
                history.push_back(Message(id, fromId, pseuds[fromId], sendTime, content));
            }
            emit historyReceived(history);
        } else if (typeOfCommand == ServerCommands::SEND_MESSAGE) {//Qued
            quint16 dg, from;
            QDateTime sendTime;
            QString content;
            in >> dg >> from >> sendTime >> content;
            emit messageReceived(dg, from, sendTime, content);
        } else if (typeOfCommand == ServerCommands::YOU_ADDED_IN_USERLIST) {//Qued
            quint16 userId;
            QString pseud;
            in >> userId >> pseud;
            emit youAddedInUserlist(userId, pseud);
        } else if (typeOfCommand == ServerCommands::ADD_USER_BY_ID ||
                   typeOfCommand == ServerCommands::ADD_USER_BY_LOGIN ||
                   typeOfCommand == ServerCommands::ADD_USER_BY_DIALOG) {//Qued
            quint16 frId, dialog;
            QString pseud;
            int status;
            bool isOn;
            in >> frId >> dialog >> pseud >> status >> isOn;
            emit userAdded(frId, dialog, pseud, status, isOn);
        } else if (typeOfCommand == ServerCommands::REGISTER_USER) {//stoped
            quint16 userId;
            in >> userId;
            emit tryRegister(userId);
        } else if (typeOfCommand == ServerCommands::FIND_USER) {
            quint16 userId;
            QString pseud;
            bool isOn;
            in >> userId >> pseud >> isOn;
            emit foundUser(userId, pseud, isOn);
        } else if (typeOfCommand == ServerCommands::NOTIFY_ON_OFF) {
            quint16 userId, stat;
            in >> userId >> stat;
            emit notifyOnOff(userId, stat);
        } else if (typeOfCommand == ServerCommands::LOAD_NOTIFYS) {
            int size;
            in >> size;
            QVector <Notification> res;
            for (int i = 0; i < size; ++i) {
                int type;
                in >> type;
                res.push_back(Notification(type));
                if (type == ServerFlags::RequestAddToFriends) {
                    int id;
                    QString pseud;
                    in >> id >> pseud;
                    res.back().field[1] = id;
                    res.back().field[2] = pseud;
                } else if (type == ServerFlags::UnreadMessages || type == ServerFlags::SendMessages) {
                    int dialId, cnt;
                    in >> dialId >> cnt;
                    res.back().field[1] = dialId;
                    res.back().field[2] = cnt;
                }
            }
            emit notifysReceived(res);
        } else if (typeOfCommand == ServerCommands::TIME_SERVER) {
            QDateTime cur;
            in >> cur;
            emit dateTimeFromServer(cur);
        } else if (typeOfCommand == ServerCommands::READ_MESSAGE_NOTIFY) {
            int dialId;
            in >> dialId;
            emit readAllYouMessageNotify(dialId);
        }
    }
}


bool ServerListener::waitFullStream(int mills) {
    qDebug() << "wait full stream";
    fullStreamReceived = false;
    while (pSocket->waitForReadyRead(mills) && !fullStreamReceived) {
        qDebug() << "innnnn wait";
    }
    bool ret = fullStreamReceived;
    fullStreamReceived = false;
    qDebug() << "exit from wait full stream\n";
    return ret;
}
