#include "message.h"

Message::Message() {}

Message::Message(quint16 idx, quint16 userIdx, const QString& pseudx, const QDateTime& sendTimex, const QString& conentx):
    id(idx), fromId(userIdx), pseud(pseudx), sendTime(sendTimex), content(conentx) {
}

quint16 Message::from() {
    return fromId;
}

QString Message::pseudonym() {
    return pseud;
}

QDateTime Message::time() {
    return sendTime;
}

QString Message::message() {
    return content;
}

QVector <Message*> Message::convert(const QVector <Message>& hs) {
    QVector <Message*> res;
    for (int i = 0; i < hs.size(); ++i)
        res.push_back(new Message(hs[i]));
    return res;
}
