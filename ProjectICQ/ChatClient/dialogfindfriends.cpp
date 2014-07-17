#include "dialogfindfriends.h"

DialogFindFriends::DialogFindFriends(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Find a friend");
    inftext = new QLabel(this);
    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *lytBut = new QHBoxLayout();
    lyt->addWidget(inftext);

    QLabel *lblUserLogin = new QLabel("Enter a login of a friend:", this);
    leFriendLogin = new QLineEdit(this);
    lyt->addWidget(lblUserLogin);
    lyt->addWidget(leFriendLogin);

    butFind = new QPushButton("Find", this);
    butAdd = new QPushButton("Add", this);
    butWrite = new QPushButton("Write", this);

    lytBut->addWidget(butFind);
    lytBut->addWidget(butAdd);
    lytBut->addWidget(butWrite);
    lyt->addLayout(lytBut);
    setLayout(lyt);

    connect(butFind, SIGNAL(clicked()), this, SLOT(slotFindFr()));
    connect(butAdd, SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(butWrite, SIGNAL(clicked()), this, SLOT(slotWrite()));
    //connect(butNo, SIGNAL(clicked()), this, SLOT(slotNoAddFriend()));
    butAdd->setEnabled(false);
    butWrite->setEnabled(false);

    //inftext->setText("<font size = 14 color = \"red\" >Lalay, saska!</font>");
}

void DialogFindFriends::slotAdd() {
    emit add(focusUser);
}

void DialogFindFriends::slotWrite() {
    emit write(focusUser);
}

void DialogFindFriends::slotFindFr() {
    emit findFriend(leFriendLogin->text());
}

QLabel* DialogFindFriends::info() {
    return inftext;
}

QPushButton* DialogFindFriends::buttonFind() {
    return butFind;
}

QPushButton* DialogFindFriends::buttonAdd() {
    return butAdd;
}

QPushButton* DialogFindFriends::buttonWrite() {
    return butWrite;
}

void DialogFindFriends::closeEvent(QCloseEvent *) {
    butAdd->setEnabled(false);
    butWrite->setEnabled(false);
    leFriendLogin->clear();
    inftext->clear();
}

void DialogFindFriends::setFocusUser(User us) {
    focusUser = us;
}
