#include "user.h"

#include <QDebug>
#include <QColor>

QColor User::ONLINE_COLOR = QColor(0, 200, 0);
QColor User::OFFLINE_COLOR = QColor(Qt::red);
QColor User::OUT_USERLIST_COLOR = QColor(Qt::gray);

User::User() {}
User::User(int id, int numDialogg, QString pseudonymm, bool isF, bool isO):
    Dialog(numDialogg, pseudonymm), userId(id), isFr(isF), isOn(isO) {

    if (!isF)
        setForeground(OUT_USERLIST_COLOR);
    else if (isOn)
        setForeground(ONLINE_COLOR);
    else
        setForeground(OFFLINE_COLOR);
}

int User::id() {
    return userId;
}

bool User::isFriend() {
    return isFr;
}

void User::setFriend(bool status) {
    if (status == isFr)
        return;
    isFr = status;
    setOnline(isOn);
}

bool User::isOnline() {
    return isOn;
}

void User::setOnline(bool status) {
    isOn = status;
    if (!isFr)
        setForeground(OUT_USERLIST_COLOR);
    else if (status)
        setForeground(ONLINE_COLOR);
    else
        setForeground(OFFLINE_COLOR);
}

bool User::operator < (const User& other) const {
    return isFr > other.isFr || isFr == other.isFr && isOn > other.isOn ||
            isFr == other.isFr && isOn == other.isOn && name() < other.name();
}

bool cmpr(User *a, User *b) {
    return *a < *b;
}
