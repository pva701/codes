#include "bytesreaderwriter.h"

BytesReaderWriter::BytesReaderWriter(QTcpSocket* socket):QDataStream(socket) {}
BytesReaderWriter::BytesReaderWriter(QByteArray *ar, QIODevice::OpenMode flags):QDataStream(ar, flags) {
    *this << quint16(0);
}

void BytesReaderWriter::confirm() {
    device()->seek(0);
    *this << quint16(device()->size() - sizeof(quint16));
}
