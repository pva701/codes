#include "servertalker.h"

ServerTalker::ServerTalker(QTcpSocket *socket):pSocket(socket), sizeOfBlock(0) {
}


void ServerTalker::sendToServer(const QByteArray& bytearray) {//send to the server
    pSocket->write(bytearray);
}

void ServerTalker::sendMessage(quint16 dialog, quint16 myId, QDateTime sendTime, const QString& message) {
    qDebug() << "send message " << dialog << myId << sendTime.toString() << message;
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::SEND_MESSAGE);
    out << quint16(dialog) << quint16(myId) << sendTime << message;
    out.confirm();
    sendToServer(outArray);
}

void ServerTalker::changeStatus(quint16 myId, quint16 frId, int status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::CHANGE_STATUS_FRIEND);
    out << quint16(myId) << quint16(frId) << status;
    out.confirm();
    sendToServer(outArray);
}


void ServerTalker::readMessageNotify(int myId, int dialog) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::READ_MESSAGE_NOTIFY);
    out << myId << dialog;
    out.confirm();
    sendToServer(outArray);
}
