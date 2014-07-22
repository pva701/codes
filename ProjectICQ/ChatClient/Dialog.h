#ifndef DIALOG_H
#define DIALOG_H

#include <QListWidgetItem>
#include <QListWidget>
#include <QTextEdit>
#include "texteditmessage.h"

class Dialog: public QObject, public QListWidgetItem
{
    Q_OBJECT
protected:
    QListWidgetItem *item;
    QTextEdit *teHistory;
    TextEditMessage *teMessage;
    quint16 numDialog;
    QString tittle;
    QWidget *tabWidget;
    QWidget *smileWidget;
    int unreadMessage;

    static int W_CNT;
    static int H_CNT;
    static QImage *smiles;
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
    static void loadSmiles();
};


#endif // DIALOG_H

