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
public:
    ServerTalker(QTcpSocket *socket);

    void sendToServer(const QByteArray& bytearray);
    void sendMessage(quint16 dialog, quint16 myId, QDateTime sendTime, const QString& message);
    void changeStatus(quint16 myId, quint16 frId, int status);
    void readMessageNotify(int myId, int dialog);
};

#endif // SERVERTALKER_H
