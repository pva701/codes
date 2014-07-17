#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QVector>

class Message
{
    quint16 id;
    quint16 fromId;
    QString pseud;
    QDateTime sendTime;
    QString content;
public:
    Message();
    Message(quint16 idx, quint16 fromId, const QString& pseudx, const QDateTime& sendTimex, const QString& conentx);
    quint16 from();
    QString pseudonym();
    QDateTime time();
    QString message();

    static QVector <Message*> convert(const QVector <Message>& hs);
};

#endif // MESSAGE_H
