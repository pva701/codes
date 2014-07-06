#include <QtGui/QApplication>
#include "chatclientwidget.h"
const int PORT = 2323;
const QString HOST = "lalkachat.ru";

/*
clientinfodb
    users                                 -> 1) insert when add new user

    friends_user1: friend_id, id_dialog   -> 1) insert when add new friend
    friends_user2
    ....

    dialogs_user1: id_dialog, closed_time   ->     1) insert when create conference
    dialogs_user2                                  2) update closed_time when close_dialog
    ....                                           3) insert when add new friend

    members_dialogs: id_dialog, members[i1, i2, ...] ->  1) insert when create conference
                                                  2) insert when add new friend
    history_dialog1: id, from_id, send_time, content  -> 1) insert when submit message
    history_dialog2
    ...
    ...
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatClientWidget client(HOST, PORT);
    client.start();
    return a.exec();
}
