#include "user.h"

User::User() {}
User::User(int id, int numDialogg, QString pseudonymm, bool isF): Dialog(numDialogg, pseudonymm), userId(id),isFr(isF) {
}

int User::id() {
    return userId;
}

bool User::isFriend() {
    return isFr;
}
