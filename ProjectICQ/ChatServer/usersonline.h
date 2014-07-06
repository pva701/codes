#ifndef USERSONLINE_H
#define USERSONLINE_H
#include <QTcpSocket>

class UsersOnline
{
    std::map <int, QTcpSocket* > users;
    std::map <QTcpSocket*, int> sockets;
public:
    UsersOnline();
    void setSocket(int userId, QTcpSocket *sock);
    bool isAuth(int userId);
    QTcpSocket *socket(int userId);
    void remove(QTcpSocket *socket);
    int usersOnline();
};

#endif // USERSONLINE_H
