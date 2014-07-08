#ifndef USER_H
#define USER_H

#include "dialog.h"

class User:public Dialog
{
    int userId;
    bool isFr;
public:
    enum UserStatus {Not_Friend, Is_Friend};
    User();
    User(int id, int numDialog, QString pseudonymm, bool isF);
    int id();
    bool isFriend();
};

#endif // USER_H
