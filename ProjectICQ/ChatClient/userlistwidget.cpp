#include "userlistwidget.h"

#include <QDebug>

UserListWidget::UserListWidget(QWidget *parent) :
    QListWidget(parent)
{
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotDoubleClickedUserlistItem(QListWidgetItem*)));
}

UserListWidget::UserListWidget(const QVector <User*>& listOfFriends) {
    frnds = listOfFriends;
    qSort(frnds.begin(), frnds.end(), cmpr);
    for (int i = 0; i < frnds.size(); ++i)
        addItem(frnds[i]);
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotDoubleClickedUserlistItem(QListWidgetItem*)));
}


void UserListWidget::add(const QVector <User>& listOfFriends) {
    for (int i = 0; i < listOfFriends.size(); ++i)
        frnds.push_back(new User(listOfFriends[i]));
    qSort(frnds.begin(), frnds.end(), cmpr);

    for (int i = 0; i < frnds.size(); ++i)
        addItem(frnds[i]);
}

void UserListWidget::add(User* us) {
    frnds.push_back(us);
    int i = frnds.size() - 1;
    for (;i > 0 && cmpr(frnds[i], frnds[i - 1]); --i)
        qSwap(frnds[i], frnds[i - 1]);
    insertItem(i, us);
}

void UserListWidget::remove(User *us) {//wrong?
    for (int i = 0; i < frnds.size(); ++i)
        if (frnds[i] == us) {
            takeItem(i);
            frnds.erase(frnds.begin() + i);
            return;
        }
}

void UserListWidget::sort() {
    qSort(frnds.begin(), frnds.end(), cmpr);
    rebuildList();
}

void UserListWidget::rebuildList() {
    while(count() > 0)
        takeItem(0);
    for (int i = 0; i < frnds.size(); ++i)
        addItem(frnds[i]);
}

void UserListWidget::slotDoubleClickedUserlistItem(QListWidgetItem* item) {
    emit doubleClickUser(frnds[row(item)]);
}

User* UserListWidget::userByDialog(int dialog) {
    for (int i = 0; i < frnds.size(); ++i)
        if (frnds[i]->dialog() == dialog)
            return frnds[i];
    return NULL;
}

User* UserListWidget::userByWidget(QWidget *wid) {
    for (int i = 0; i < frnds.size(); ++i)
        if (frnds[i]->widget() == wid)
            return frnds[i];
    return NULL;
}

User* UserListWidget::userById(int id) {
    for (int i = 0; i < frnds.size(); ++i)
        if (frnds[i]->id() == id)
            return frnds[i];
    return NULL;
}


void UserListWidget::updateStatus(User *us) {
    int oldi = -1;
    for (int i = 0; i < frnds.size(); ++i)
        if (frnds[i] == us)
            oldi = i;
    //qDebug() << "oldi = " << oldi;
    int i = oldi;
    for (;i > 0 && cmpr(frnds[i], frnds[i - 1]); --i)
        qSwap(frnds[i], frnds[i - 1]);
    //qDebug() << "MY = " << frnds[i]->isFriend() << " " << frnds[i]->isOnline();
   // qDebug() << "HE = " << frnds[i + 1]->isFriend() << " " << frnds[i + 1]->isOnline();
    //qDebug() << "COMPARE = " << (frnds[i + 1] < frnds[i]);
    for (; i + 1 < frnds.size() && cmpr(frnds[i + 1], frnds[i]); ++i)
        qSwap(frnds[i], frnds[i + 1]);
    takeItem(oldi);
    insertItem(i, us);
}

UserListWidget::~UserListWidget() {
    for (int i = 0; i < frnds.size(); ++i)
        delete frnds[i];
}
