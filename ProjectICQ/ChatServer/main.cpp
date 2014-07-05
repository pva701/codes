#include <QtGui/QApplication>
#include "chatserver.h"

const int PORT = 2323;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatServer server(PORT);
    server.show();
    return a.exec();
}
