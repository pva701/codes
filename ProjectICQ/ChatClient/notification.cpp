#include "notification.h"
#include <QDebug>
Notification::Notification(quint16 id, const QString& pse, QWidget *parent) :
    userId(id), pseud(pse), QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Notification");
    QLabel *inftext = new QLabel("<b>" + pseud + "</b> added you in his userlist.<br>"
                       "Do you want to add <b>" + pseud + "</b> in your userlist?", this);
    QVBoxLayout *lyt = new QVBoxLayout();
    QHBoxLayout *lytBut = new QHBoxLayout();
    lyt->addWidget(inftext);
    QPushButton *butYes = new QPushButton("Yes", this);
    QPushButton *butWrite = new QPushButton("Write", this);
    QPushButton *butNo = new QPushButton("No", this);
    lytBut->addWidget(butYes);
    lytBut->addWidget(butWrite);
    lytBut->addWidget(butNo);
    lyt->addLayout(lytBut);
    setLayout(lyt);
    setFixedSize(lyt->sizeHint());

    connect(butYes, SIGNAL(clicked()), this, SLOT(slotYes()));
    connect(butWrite, SIGNAL(clicked()), this, SLOT(slotWrite()));
    connect(butNo, SIGNAL(clicked()), this, SLOT(slotNo()));
}

void Notification::slotYes() {
    emit yes(User(userId, 0, pseud, 0, 0));
    close();
}

void Notification::slotWrite() {
    emit write(User(userId, 0, pseud, 0, 0));
    close();
}

void Notification::slotNo() {
    emit no();
    close();
}
