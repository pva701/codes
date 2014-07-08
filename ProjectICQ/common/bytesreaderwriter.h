#ifndef BYTESREADERWRITER_H
#define BYTESREADERWRITER_H
#include <QString>
#include <QTime>
#include <QTcpSocket>

class BytesReaderWriter:public QDataStream
{
public:
    BytesReaderWriter(QTcpSocket* socket);//Read
    BytesReaderWriter(QByteArray *ar, QIODevice::OpenMode flags);
    void confirm();
};

#endif // BYTESREADERWRITER_H
