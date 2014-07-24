#include <QtGui/QApplication>
#include "chatclientform.h"

#include <QListWidget>
#include <QListWidgetItem>

const int PORT = 2323;
const QString HOST = "188.120.238.32";

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
    QApplication app(argc, argv);
    ChatClientForm client(HOST, PORT);
    client.start();
    /*QListWidget *lw = new QListWidget();
    QListWidgetItem *it1 = new QListWidgetItem("Item1");
    QListWidgetItem *it2 = new QListWidgetItem("Item2");
    lw->addItem(it1);
    lw->addItem(it2);

    QTextEdit *t1 = new QTextEdit("fldkflf");
    QTextEdit *t2 = new QTextEdit();
    t1->setTextBackgroundColor(Qt::yellow);
    t1->setReadOnly(true);
    QTextEdit *t3 = new QTextEdit();
    lw->setItemWidget(it1, t1);

    it1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    it2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    it2->setText("<font color = \"red\"> red </font>");
    //lw->setItemWidget(it2, t2);

    it1->setSizeHint(QSize(300, 30));
    t1->setStyleSheet("QTextEdit { background-color: #FFFCCC; }");
   //t3->show();

    lw->show();*/
    return app.exec();
}
