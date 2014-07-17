#ifndef DIALOGFINDFRIENDS_H
#define DIALOGFINDFRIENDS_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "user.h"

class DialogFindFriends : public QDialog
{
    Q_OBJECT

    QLabel *inftext;
    QLineEdit *leFriendLogin;
    QPushButton *butFind;
    QPushButton *butAdd;
    QPushButton *butWrite;
    User focusUser;
public:
    explicit DialogFindFriends(QWidget *parent = 0);
    QLabel *info();
    QPushButton *buttonFind();
    QPushButton *buttonAdd();
    QPushButton *buttonWrite();
    void setFocusUser(User us);
private:
    void closeEvent(QCloseEvent *);
signals:
    void findFriend(const QString&);
    void add(const User&);
    void write(const User&);
private slots:
    void slotFindFr();
    void slotAdd();
    void slotWrite();
};

#endif // DIALOGFINDFRIENDS_H
