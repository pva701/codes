#include "user.h"

User::User() {}
User::User(int id, const QString& log, int numDialogg, QString pseudonymm, bool isF): Dialog(numDialogg, pseudonymm), userId(id), userLogin(log), isFr(isF) {
    teHistory = NULL;
    teMessage = NULL;
}

int User::id() {
    return userId;
}

QString User::login() {
    return userLogin;
}

bool User::isFriend() {
    return isFr;
}
