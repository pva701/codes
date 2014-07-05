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
    int numDialog;
    QString tittle;
public:
    Dialog();
    Dialog(int numDialogg, QString namee);
    QListWidgetItem* pointerToItem();
    QString name();
    int dialog();
    void setListWidgetItem(QListWidgetItem* ptr);
    void setTEHistory(QTextEdit *te);
    void setTEMessage(QTextEdit *te);
    QTextEdit* history();
    QTextEdit* message();

};

#endif // DIALOG_H
