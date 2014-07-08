#include "servertalker.h"

ServerTalker::ServerTalker(QTcpSocket *socket):pSocket(socket), sizeOfBlock(0) {
}

void ServerTalker::sendToServer(const QByteArray& bytearray) {//send to the server
    pSocket->write(bytearray);
}

void ServerTalker::sendMessage(quint16 dialog, quint16 myId, QDateTime sendTime, const QString& message) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::SEND_MESSAGE);
    out << quint16(dialog) << quint16(myId) << sendTime << message;
    out.confirm();
    sendToServer(outArray);
}

QDateTime ServerTalker::currentDateTimeFromServer() {
    return QDateTime::currentDateTime();
}

void ServerTalker::changeStatus(quint16 myId, quint16 frId, bool status) {
    QByteArray outArray;
    BytesReaderWriter out(&outArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << quint16(ServerCommands::CHANGE_STATUS);
    out << quint16(myId) << quint16(frId) << status;
    out.confirm();
    sendToServer(outArray);
}
