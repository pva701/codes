#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QListWidget>
#include "user.h"

class UserListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit UserListWidget(QWidget *parent = 0);
    explicit UserListWidget(const QVector <User*>& listOfFriends);
    void add(User* us);
    void add(const QVector <User*>& listOfFriends);
    void remove(User* us);
    void sort();
    User* userByDialog(int dialog);
    User* userByWidget(QWidget *wid);
    User* userById(int id);
    void updateStatus(User *us);
    ~UserListWidget();
private:
    QVector <User*> frnds;
    void rebuildList();
signals:
    void doubleClickUser(Dialog* dg);
private slots:
    void slotDoubleClickedUserlistItem(QListWidgetItem *item);
};

#endif // FRIENDLISTWIDGET_H
