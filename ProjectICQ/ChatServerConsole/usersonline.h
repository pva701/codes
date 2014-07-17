#ifndef USERSONLINE_H
#define USERSONLINE_H
#include <QtNetwork/QTcpSocket>

class UsersOnline
{
    std::map <int, QTcpSocket* > users;
    std::map <QTcpSocket*, int> sockets;
public:
    UsersOnline();
    void setSocket(int userId, QTcpSocket *sock);
    bool isOnline(int userId);
    QTcpSocket *socket(int userId);
    void remove(QTcpSocket *socket);
    int countUsersOnline();
    quint16 userId(QTcpSocket *sock);
};

#endif // USERSONLINE_H
