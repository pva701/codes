#include <QtCore/QCoreApplication>
#include "chatserver.h"
#include <cstdio>
const int PORT = 2323;

#include <QStringList>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char host[100];
    int port;
    printf("Input host port ");
    scanf("%s %d", host, &port);
    ChatServer server(QString(host), PORT);
    //server.show();
    return a.exec();
}
