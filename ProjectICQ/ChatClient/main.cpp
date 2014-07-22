#include <QtGui/QApplication>
#include "chatclientform.h"

#include <QMenu>
#include <QToolBar>
#include <QBitmap>
#include <QMenuBar>

const int PORT = 2323;
const QString HOST = "188.120.238.32";

/*
clientinfodb
    users                                 -> 1) insert when add new user

    userlist_user1: friend_id, dialog_id, is_friend   -> 1) insert when add new friend
    userlist_user2
    ....

    inv_user1: friend_id
    inv_user2: ...

    members_dialogs: dialog_id, members[i1, i2, ...] ->  1) insert when create conference
                                                  2) insert when add new friend
    history_dialog1: id, from_id, send_time, content  -> 1) insert when submit message
    history_dialog2
    ...
    ...
*/


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*for (int i = 0; i < wCnt; ++i)
        for (int j = 0; j < hCnt; ++j)
            ed.textCursor().insertImage(smiles.copy(i * w, j * h, w, h));
    ed.show();*/


    QImage smiles("/home/pva701/smiles.png");
    int w = smiles.size().width() / 7;
    int h = smiles.size().height() / 7;

    /*QToolBar *tb = new QToolBar();
    QIcon newIcon = QPixmap::fromImage(smiles.copy(w, 0, w, h));
    QAction *a = new QAction(newIcon, QString("&Smiles"), 0);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_S);
    tb->addAction(a);
    tb->show();
    tb->setGeometry(QRect(300, 300, tb->size().width(), tb->size().height()));*/


    /*QMenuBar *br = new QMenuBar();
    QMenu *wg = new QMenu();
    wg->setTearOffEnabled(true);
    wg->setIcon(QPixmap::fromImage(smiles.copy(w, 0, w, h)));
    QGridLayout lyt;
    QPalette palw;
    palw.setBrush(wg->backgroundRole(), QBrush(Qt::white));
    wg->setAutoFillBackground(true);
    wg->setPalette(palw);
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j) {
            QPushButton *but = new QPushButton(wg);
            //but->setIcon(QPixmap::fromImage(smiles.copy(i * w, j * h, w, h)));
            //but->setIconSize(QSize(w, h));
            QPalette p;
            p.setBrush(but->backgroundRole(), QBrush(smiles.copy(i * w, j * h, w, h)));
            but->setPalette(p);
            but->setFlat(true);
            but->setAutoFillBackground(true);
            but->setMask(QPixmap::fromImage(smiles.copy(i * w, j * h, w, h)).mask());
            lyt.addWidget(but, i, j);
            //ed.textCursor().insertImage(but->palette());
        }
    wg->setLayout(&lyt);
    wg->setFixedSize(lyt.sizeHint());
    wg->setWindowFlags(Qt::FramelessWindowHint);
    //wg.show();
    br->addMenu(wg);
    br->setGeometry(QRect(300, 300, 100, 30));
    br->show();

    /*QTableView tw;
    tw.setIconSize();
    QTableView
    tw.show();*/

    /*QMenuBar *panel = new QMenuBar();
    panel->setFixedHeight(30);
    panel->setAutoFillBackground(true);
    QPalette pal = panel->palette();
    pal.setColor(QPalette::Background,  Qt::white);

    panel->setStyleSheet("QMenuBar { background-color: #ffffff; }");

    QMenu *men = new QMenu("OK");
    panel->addMenu(men);*/

    //pal.setBrush(panel->backgroundRole(), Qt::white);
    //panel->setPalette(pal);
   // panel->show();

    /*int r = 243, g = 242, b = 241;
    qDebug() << QString::number(256 * 256 * r + 256 * g + 241, 16) << endl;
     qDebug() << 256 * 0.94902;
     qDebug() << 256 * 0.945098;
     qDebug() << 256 * 0.941176;
    QSplitter wg;
    wg.show();
    //qDebug();
    //qDebug() << wg.palette().color(wg.backgroundRole()).toRgb();
    //qDebug() << wg.palette().color(wg.backgroundRole()).toHsv();
    //qDebug() << wg.palette().color(wg.backgroundRole()).toCmyk()*/

    ChatClientForm client(HOST, PORT);
    client.start();
    return app.exec();
}
