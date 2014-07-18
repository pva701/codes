#include "dialog.h"

#include <QDebug>
#include <QSplitter>

Dialog::Dialog():QListWidgetItem() {}

Dialog::Dialog(quint16 numDialogg, QString namee):QListWidgetItem(namee), teHistory(NULL), teMessage(NULL),
    numDialog(numDialogg), tittle(namee), tabWidget(NULL), unreadMessage(0) {
    setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
    setFont(QFont(QFont().defaultFamily(), 15, QFont::Light, false));
    font().setFixedPitch(true);
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
    teMessage = new TextEditMessage();
    teHistory->setReadOnly(true);
    spl->addWidget(teHistory);
    spl->addWidget(teMessage);
    tabWidget = spl;
}

void Dialog::setUnreadMessage(int x) {
    if (x == 0)
        setText(name());
    else {
        int w = listWidget()->visualItemRect(this).width();
        if (w > 300) w /= 2.5;
        QFontMetrics me(font());
        int wName = me.width(name());
        QString t = QString("+%1").arg(x);
        int wMess = me.width(t);
        int cntSpaces = (w - wName - wMess) /  me.width(' ');
        QString res = name();
        for (int i = 1; i <= cntSpaces; ++i)
            res += " ";
        res += t;
        setText(res);
    }
    unreadMessage = x;
}

int Dialog::unread() {
    return unreadMessage;
}
