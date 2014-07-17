#include <QtGui/QApplication>
#include "chatclientform.h"
const int PORT = 2323;
const QString HOST = "127.0.0.1"; //"188.120.238.32";

/*
clientinfodb
    users                                 -> 1) insert when add new user

    userlist_user1: friend_id, dialog_id, is_friend   -> 1) insert when add new friend
    userlist_user2
    ....

    inv_user1: friend_id
    inv_user2: ...

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
    ChatClientForm client(HOST, PORT);
    client.start();
    return a.exec();
}
