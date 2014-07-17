#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QMap>
#include <QVariant>
#include <QVector>

class Notification
{
    int tp;
public:
    Notification();
    Notification(int tpp);
    QMap <int, QVariant> field;
    int type();
    static QVector <Notification*> convert(const QVector <Notification>& ar);
};

#endif // NOTIFICATION_H
