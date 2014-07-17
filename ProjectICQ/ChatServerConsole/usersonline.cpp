#include "usersonline.h"

UsersOnline::UsersOnline()
{
}

void UsersOnline::setSocket(int userId, QTcpSocket *sock) {
    users[userId] = sock;
    sockets[sock] = userId;
}


bool UsersOnline::isOnline(int userId) {
    return users.find(userId) != users.end();
}

QTcpSocket* UsersOnline::socket(int userId) {
    if (isOnline(userId))
        return users[userId];
    return NULL;
}

quint16 UsersOnline::idUser(QTcpSocket *sock) {
    return sockets[sock];
}

void UsersOnline::remove(QTcpSocket *socket) {
    if (sockets.find(socket) == sockets.end())
        return;
    int userId = sockets[socket];
    users.erase(userId);
    sockets.erase(socket);
}

int UsersOnline::usersOnline() {
    return users.size();
}
