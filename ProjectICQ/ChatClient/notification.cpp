#include "notification.h"

Notification::Notification(int tpp):tp(tpp)
{
}

Notification::Notification() {}

int Notification::type() {
    return tp;
}

QVector <Notification*> Notification::convert(const QVector <Notification>& ar) {
    QVector <Notification*> res;
    for (int i = 0; i < ar.size(); ++i)
        res.push_back(new Notification(ar[i]));
    return res;
}
