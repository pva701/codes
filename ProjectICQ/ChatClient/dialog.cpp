#include "dialog.h"

#include <QDebug>
#include <QSplitter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include "smilebutton.h"
#include <QUrl>

Dialog::Dialog(QWidget *parent): QSplitter(Qt::Vertical, parent), QListWidgetItem() {}

Dialog::Dialog(quint16 numDialogg, QString namee, QWidget *parent):QSplitter(Qt::Vertical, parent), QListWidgetItem(namee), teHistory(NULL), teMessage(NULL),
    numDialog(numDialogg), tittle(namee), tabWidget(NULL), unreadMessage(0), createdWidgetx(false) {
    QListWidgetItem::setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
    QListWidgetItem::setFont(QFont(QFont().defaultFamily(), 15, QFont::Light, false));
    QListWidgetItem::font().setFixedPitch(true);
    loadSmiles();
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

QWidget* Dialog::widget() {
    return this;
    //return tabWidget;
}

bool Dialog::createdWidget() {
    return createdWidgetx;
}

void Dialog::createWidget() {
    createdWidgetx = true;

    int w = smiles->size().width() / W_CNT;
    int h = smiles->size().height() / H_CNT;
    teHistory = new QTextEdit();
    teMessage = new TextEditMessage();
    teHistory->setReadOnly(true);
    addWidget(teHistory);
    setStretchFactor(0, 2);

    panel = new QToolBar();
    QIcon newIcon = QPixmap::fromImage(smiles->copy(w, 0, w, h));
    QAction *a = new QAction(newIcon, QString("&Smiles"), 0);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(a, SIGNAL(triggered()), this, SLOT(slotClickedSmileMenu()));
    panel->addAction(a);
    panel->setFixedHeight(30);
    //panel->pos()
    /*QMenuBar *panel = new QMenuBar(this);
    panel->setFixedHeight(30);
    panel->setAutoFillBackground(true);
    panel->setStyleSheet("QMenuBar { background-color: #f3f2f1; } QMenuBar::item { background-color: #f3f2f1; }");
    //panel->setStyleSheet("QMenuBar::item { icon-size: 32px; } ");
    //panel->setStyleSheet("QMenuBar::button { background-color: #ffffff; }");*/
    addWidget(panel);
    addWidget(teMessage);

    smileMenu = new QMenu("Smiles");
    smileMenu->setIcon(QPixmap::fromImage(smiles->copy(w, 0, w, h)));
    smileMenu->setStyleSheet("QMenu { background-color: #ffffff;}");

    QGridLayout *lytSmiles = new QGridLayout(smileMenu);
    QPalette palw;
    palw.setBrush(smileMenu->backgroundRole(), Qt::white);
    smileMenu->setAutoFillBackground(true);
    smileMenu->setPalette(palw);
    for (int i = 0; i < W_CNT; ++i)
        for (int j = 0; j < H_CNT; ++j) {
            QImage icon = smiles->copy(j * w, i * h, w, h);
            SmileButton *but = new SmileButton(icon, W_CNT * i + j);
            connect(but, SIGNAL(clicked()), this, SLOT(slotSmilesClicked()));
            lytSmiles->addWidget(but, i, j);
            //teMessage->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), smiles->copy(j * w, i * h, w, h));
            teHistory->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), smiles->copy(j * w, i * h, w, h));
        }
    reloadResource();
    smileMenu->setFixedSize(lytSmiles->sizeHint());
}

void Dialog::slotSmilesClicked() {
    SmileButton *but = (SmileButton*)sender();
    //message()->textCursor().insertImage(but->image());
    //teMessage->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), but->image());
    teMessage->textCursor().insertImage(but->name());
    smileMenu->hide();
}

void Dialog::slotClickedSmileMenu() {
    QPoint ps = mapToGlobal(panel->pos());
    smileMenu->setGeometry(QRect(ps.x(), ps.y(), smileMenu->size().width(), smileMenu->size().height()));
    smileMenu->show();
}

void Dialog::reloadResource() {
    int w = smiles->size().width() / W_CNT;
    int h = smiles->size().height() / H_CNT;
    for (int i = 0; i < W_CNT; ++i)
        for (int j = 0; j < H_CNT; ++j) {
            QImage icon = smiles->copy(j * w, i * h, w, h);
            QString nm = "[smile:" + QString::number(W_CNT * i + j) + "]";
            teMessage->document()->addResource(QTextDocument::ImageResource, QUrl(nm), icon);
            //teHistory->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), smiles->copy(j * w, i * h, w, h));
        }
}

void Dialog::setUnreadMessage(int x) {
    if (x == 0)
        setText(name());
    else {
        int w = listWidget()->visualItemRect(this).width();
        if (w > 300) w /= 2.5;
        QFontMetrics me(QListWidgetItem::font());
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

void Dialog::loadSmiles() {
    const QString PATH = "/home/pva701/smiles.png";
    smiles = new QImage(PATH);
    W_CNT = 7;
    H_CNT = 7;
}
