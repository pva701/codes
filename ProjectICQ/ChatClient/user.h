#ifndef USER_H
#define USER_H

#include "dialog.h"
#include <QObject>
#include <QColor>

class User: public Dialog
{
    int userId;
    bool isFr;
    bool isOn;

    static QColor ONLINE_COLOR;
    static QColor OFFLINE_COLOR;
    static QColor OUT_USERLIST_COLOR;

public:
    enum UserStatus {Not_Friend, Is_Friend};
    User();
    User(int id, int numDialog, QString pseudonymm, bool isF, bool isO);
    int id();
    bool isFriend();
    bool isOnline();
    void setOnline(bool stat);
    bool operator < (const User& other) const;

    friend class UserListWidget;
signals:
    void changedStatus();
};

bool cmpr(User *a, User *b);
#endif // USER_H
