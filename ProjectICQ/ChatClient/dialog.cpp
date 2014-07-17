#include "dialog.h"

#include <QDebug>
#include <QSplitter>

Dialog::Dialog():QListWidgetItem() {}

Dialog::Dialog(quint16 numDialogg, QString namee):QListWidgetItem(namee), numDialog(numDialogg), tittle(namee), tabWidget(NULL), teHistory(NULL), teMessage(NULL) {
    setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
}

quint16 Dialog::dialog() const {
    return numDialog;
}


QString Dialog::name() const {
    return tittle;
}

QTextEdit* Dialog::history() const {
    return teHistory;
}

QTextEdit* Dialog::message() const {
    return teMessage;
}

QWidget* Dialog::widget() const {
    return tabWidget;
}

void Dialog::createWidget() {
    QSplitter *spl = new QSplitter(Qt::Vertical);
    teHistory = new QTextEdit();
    teMessage = new QTextEdit(new QTextEdit());
    teHistory->setReadOnly(true);
    spl->addWidget(teHistory);
    spl->addWidget(teMessage);
    tabWidget = spl;
}
