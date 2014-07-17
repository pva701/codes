#ifndef SERVERTALKER_H
#define SERVERTALKER_H

#include <QTcpSocket>
#include <QString>
#include <QDateTime>
#include "user.h"
#include "../common/servercommands.h"
#include "../common/bytesreaderwriter.h"
#include "message.h"

class ServerTalker:public QObject
{
    Q_OBJECT
private:
    QTcpSocket *pSocket;
    quint16 sizeOfBlock;
    QDateTime startDate;
    QTime counter;
public:
    ServerTalker(QTcpSocket *socket);

    void sendToServer(const QByteArray& bytearray);
    //bool waitFullStream(int mills, BytesReaderWriter& in);
    //QString addUserById(quint16 myId, quint16 userId, bool status);
    //void addUserByLogin(quint16 myId, const QString& log, bool status, quint16& userId, quint16& dialog, QString& pseud);
    //void loadUserlist(quint16 myId, QVector <User*>& fr);
    //void loadHistory(Dialog *dg, QVector <Message*>& history);
    QDateTime currentDateTimeFromServer();
    void sendMessage(quint16 dialog, quint16 myId, QDateTime sendTime, const QString& message);
    void changeStatus(quint16 myId, quint16 frId, bool status);
    void readMessageNotify(int myId, int dialog);
};

#endif // SERVERTALKER_H
