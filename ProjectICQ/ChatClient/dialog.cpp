#include "dialog.h"

Dialog::Dialog() {}

Dialog::Dialog(int numDialogg, QString namee): numDialog(numDialogg), tittle(namee) {}

int Dialog::dialog() {
    return numDialog;
}

QListWidgetItem* Dialog::pointerToItem() {
    return item;
}

QString Dialog::name() {
    return tittle;
}

void Dialog::setListWidgetItem(QListWidgetItem* ptr) {
    item = ptr;
}

void Dialog::setTEHistory(QTextEdit *te) {
    teHistory = te;
}

void Dialog::setTEMessage(QTextEdit *te) {
    teMessage = te;
}

QTextEdit* Dialog::history() {
    return teHistory;
}

QTextEdit* Dialog::message() {
    return teMessage;
}
