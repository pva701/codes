#ifndef DIALOG_H
#define DIALOG_H
#include <QListWidgetItem>
#include <QListWidget>
#include <QTextEdit>
#include "texteditmessage.h"

class Dialog: public QListWidgetItem
{
protected:
    QListWidgetItem *item;
    QTextEdit *teHistory;
    TextEditMessage *teMessage;
    quint16 numDialog;
    QString tittle;
    QWidget *tabWidget;
    int unreadMessage;
public:
    Dialog();
    Dialog(quint16 numDialogg, QString namee);
    QString name() const;
    quint16 dialog() const;
    QTextEdit* history() const;
    QTextEdit* message() const;
    QWidget* widget() const;
    void createWidget();
    void setUnreadMessage(int x);
    int unread();
};

#endif // DIALOG_H
