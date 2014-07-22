#ifndef DIALOG_H
#define DIALOG_H

#include <QListWidgetItem>
#include <QListWidget>
#include <QTextEdit>
#include "texteditmessage.h"
#include <QDialog>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QMenu>

class Dialog: public QSplitter, public QListWidgetItem
{
    Q_OBJECT
protected:
    QListWidgetItem *item;
    QTextEdit *teHistory;
    QToolBar *panel;
    TextEditMessage *teMessage;
    quint16 numDialog;
    QString tittle;
    QWidget *tabWidget;
    QMenu *smileMenu;
    int unreadMessage;
    bool createdWidgetx;
public:
    Dialog(QWidget *parent = 0);
    Dialog(quint16 numDialogg, QString namee, QWidget *parent = 0);
    QString name() const;
    quint16 dialog() const;
    QTextEdit* history() const;
    QTextEdit* message() const;
    QWidget* widget();
    void createWidget();
    bool createdWidget();
    void setUnreadMessage(int x);
    int unread();
    void reloadResource();

    int W_CNT;
    int H_CNT;
    QImage *smiles;
    void loadSmiles();
private slots:
    void slotSmilesClicked();
    void slotClickedSmileMenu();
};

#endif // DIALOG_H
