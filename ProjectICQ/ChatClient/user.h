#ifndef USER_H
#define USER_H

#include "dialog.h"

class User:public Dialog
{
    int userId;
    QString userLogin;
    bool isFr;
public:
    User();
    User(int id, const QString& log, int numDialog, QString pseudonymm, bool isF);
    int id();
    QString login();
    bool isFriend();
};

#endif // USER_H
