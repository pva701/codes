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

    template <typename T>
    T readT() {
        T value;
        *this >> value;
        return value;
    }

    void confirm();
};

#endif // BYTESREADERWRITER_H
