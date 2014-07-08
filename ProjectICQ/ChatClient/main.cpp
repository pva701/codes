#include <QtGui/QApplication>
#include "chatclientwidget.h"
const int PORT = 2323;
const QString HOST = "lalkachat.ru";

/*
clientinfodb
    users                                 -> 1) insert when add new user

    userlist_user1: friend_id, dialog_id, is_friend   -> 1) insert when add new friend
    userlist_user2
    ....

    dialogs_user1: dialog_id, closed_time   ->     1) insert when create conference
    dialogs_user2                                  2) update closed_time when close_dialog
    ....                                           3) insert when add new friend

    members_dialogs: dialog_id, members[i1, i2, ...] ->  1) insert when create conference
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
