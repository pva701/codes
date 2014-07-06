#ifndef DIALOG_H
#define DIALOG_H
#include <QListWidgetItem>
#include <QTextEdit>
class Dialog
{
protected:
    QListWidgetItem *item;
    QTextEdit *teHistory;
    QTextEdit *teMessage;
    quint16 numDialog;
    QString tittle;
    QWidget *tabWidget;
public:
    Dialog();
    Dialog(quint16 numDialogg, QString namee);
    QListWidgetItem* pointerToItem();
    QString name();
    quint16 dialog();
    void setListWidgetItem(QListWidgetItem* ptr);
    void setTEHistory(QTextEdit *te);
    void setTEMessage(QTextEdit *te);
    QTextEdit* history();
    QTextEdit* message();
    QWidget* widget();
    void setWidget(QWidget *wid);

};

#endif // DIALOG_H
