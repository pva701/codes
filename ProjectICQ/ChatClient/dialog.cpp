#include "dialog.h"

#include <QDebug>
#include <QSplitter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include "smilebutton.h"
#include <QUrl>
#include <QTextBlock>
#include <QDateTime>
#include <QTextDocumentFragment>
#include <QMenu>
#include <cmath>
#include <QTimer>

Dialog::Dialog(QWidget *parent): QSplitter(Qt::Vertical, parent), QListWidgetItem() {}

Dialog::Dialog(quint16 numDialogg, QString namee, QWidget *parent):QSplitter(Qt::Vertical, parent),
    QListWidgetItem(namee), lwHistory(NULL), teMessage(NULL),
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
    dgReadByUser = false;

    int w = smiles->size().width() / W_CNT;
    int h = smiles->size().height() / H_CNT;
    lwHistory = new QListWidget();
    teMessage = new TextEditMessage();
    //teHistory->setReadOnly(true);
    addWidget(lwHistory);
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
            //teHistory->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), smiles->copy(j * w, i * h, w, h));
        }
    reloadResource(message());
    smileMenu->setFixedSize(lytSmiles->sizeHint());
    lwHistory->scrollToBottom();

    connect(&tmrNotActive, SIGNAL(timeout()), this, SLOT(slotNotActiveBehav()));
    connect(teMessage, SIGNAL(gotFocus()), this, SLOT(slotFinishReadMessage()));
    connect(teMessage, SIGNAL(lostFocus()), this, SLOT(slotNotActiveBehav()));
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
    const QString PATH = "./SMILES.png";
    smiles = new QImage(PATH);
    W_CNT = 7;
    H_CNT = 7;
}

QString Dialog::toStringFromDocument() {
    QTextDocument *doc = message()->document();
    QString txt;
    for (QTextBlock bl = doc->begin(); bl != doc->end(); bl = bl.next())
        if (bl.isValid()) {
            for (QTextBlock::iterator it = bl.begin(); !it.atEnd(); ++it) {
                QTextFragment fragm = it.fragment();
                if (fragm.isValid() && fragm.charFormat().isImageFormat()) {
                    QString imgName = fragm.charFormat().toImageFormat().name();
                    txt += imgName;
                } else if (fragm.isValid())
                    txt += fragm.text();
            }

            if (bl != doc->begin())
                txt += "\n";
        }
    int i = (int)txt.size() - 1;
    while (i >= 0 && (txt[i] == ' ' || txt[i] == '\n')) --i;
    txt.remove(i + 1, txt.size() - i - 1);
    return txt;
}

int Dialog::metainfoSmile(const QString& s, int pos) {
    const int SIZE_OF_METAWORDS = 1;
    QString metaWords[SIZE_OF_METAWORDS] = {"smile"};
    for (int i = 0; i < SIZE_OF_METAWORDS; ++i) {
        int j = pos;
        if (s[j] == '[') ++j;
        if (j + metaWords[i].size() + 2 <= s.size()) {
            QString sub = s.mid(j, metaWords[i].size());
            if (sub == metaWords[i]) j += metaWords[i].size();
            QString num;
            if (s[j] != ':')
                continue;
            ++j;
            while (j < s.size() && isdigit(s[j].toAscii()))
                num += s[j++];
            if (j < s.size() && s[j] == ']' && num.size())
                return num.toInt();
        }
    }
    return -1;
}

QTextDocument* Dialog::toDocumentFromString(const QString& msg) {
    QTextDocument *res = new QTextDocument();
    QTextCursor cursor(res);
    QString cur;
    for (int i = 0; i <= msg.size(); ++i)
        if (i == msg.size() || msg[i] == '\n') {
            for (int j = 0; j < cur.size(); ++j) {
                int sm = metainfoSmile(cur, j);
                if (sm != -1) {
                    cursor.insertImage("[smile:" + QString::number(sm) + "]");
                    while (cur[j] != ']') ++j;
                } else
                    cursor.insertText(QString(cur[j]));
            }
            cursor.insertBlock();
            cur = "";
        } else
            cur += msg[i];
    return res;
}

void Dialog::reloadResource(QTextEdit *ed) {
    QTextDocument *doc = ed->document();
    int w = smiles->size().width() / W_CNT;
    int h = smiles->size().height() / H_CNT;
    for (int i = 0; i < W_CNT; ++i)
        for (int j = 0; j < H_CNT; ++j) {
            QImage icon = smiles->copy(j * w, i * h, w, h);
            QString nm = "[smile:" + QString::number(W_CNT * i + j) + "]";
            doc->addResource(QTextDocument::ImageResource, QUrl(nm), icon);
            //teHistory->document()->addResource(QTextDocument::ImageResource, QUrl(but->name()), smiles->copy(j * w, i * h, w, h));
        }
}

void Dialog::processCalc(int& heig, int& mx, int& curLine, int curw, int curh) {
    int wiTe = parentWidget()->width();
    if (curLine + curw <= wiTe) {
        mx = qMax(mx, curh);
        curLine += curw;
    } else {
        heig += mx;
        mx = curw;
        curLine = curh;
    }
}

void Dialog::appendToHistory(const QString& name, const QDateTime& sendTime, QTextDocument *document, InsertingMode mode) {
    QListWidgetItem *item = new QListWidgetItem();
    QTextEdit *te = new QTextEdit();
    lwHistory->addItem(item);
    te->setReadOnly(true);
    reloadResource(te);
    lwHistory->setItemWidget(item, te);
    QString color;
    if (name == "You")
        color = "blue";
    else
        color = "red";

    te->append(QString("<font color = \"%1\"> <b>" + name + "</b> (" + sendTime.toString("dd-MM-yyyy hh:mm:ss") + "):</font>").arg(color));
    te->moveCursor(QTextCursor::End);
    te->textCursor().insertBlock();
    te->textCursor().insertFragment(QTextDocumentFragment(document));

    int heig = 17, widthTe = parentWidget()->width();
    int curLine = 0;
    int mx = 0;

    for (QTextBlock bl = te->document()->begin(); bl != te->document()->end(); bl = bl.next())
        if (bl.isValid()) {
            if (bl.begin().atEnd()) {
                heig += 17 + mx;//&&&
                curLine = mx = 0;
                continue;
            }

            for (QTextBlock::iterator it = bl.begin(); !it.atEnd(); ++it) {
                QTextFragment fragm = it.fragment();
                int curw, curh;
                if (fragm.isValid() && fragm.charFormat().isImageFormat()) {
                    curw = smiles->width() / W_CNT;
                    curh = smiles->height() / H_CNT;
                    processCalc(heig, mx, curLine, curw, curh);
                } else if (fragm.isValid()) {
                    QString s = fragm.text();
                    QFontMetrics me(fragm.charFormat().font());
                    curh = me.lineSpacing();
                    for (int j = 0; j < s.size(); ++j) {
                        curw = me.width(s[j]);
                        processCalc(heig, mx, curLine, curw, curh);
                    }
                }
            }
            heig += mx;
            mx = curLine = 0;
        }

    te->setStyleSheet(QString("QFrame {"
                 "border: 2px solid #f3f2f1;"
                 "border-radius: 4px;"
                  "padding: 2px;}"));
    item->setSizeHint(QSize(0, heig + 18));
    te->resize(QSize(widthTe, heig));
    lwHistory->scrollToBottom();

    if (mode == ReceivedMessage && !dgReadByUser) {
        setUnreadMessage(unreadMessage + 1);
        queUnreadWrote.push_back(te);
        te->setStyleSheet("QTextEdit { background-color: #FFFCCC; }");
    } else if (mode == LoadHistory) {

        if (unreadMessage != 0) {
            queUnreadWrote.push_back(te);
            te->setStyleSheet("QTextEdit { background-color: #FFFCCC; }");
            if (queUnreadWrote.size() > unreadMessage) {
                queUnreadWrote.front()->setStyleSheet("QTextEdit { background-color: #FFFFFF; }");
                queUnreadWrote.pop_front();
            }
        }

        if (wroteMessage != 0) {
            queUnreadWrote.push_back(te);
            te->setStyleSheet("QTextEdit { background-color: #DFFFCC; }");
            if (queUnreadWrote.size() > wroteMessage) {
                queUnreadWrote.front()->setStyleSheet("QTextEdit { background-color: #FFFFFF; }");
                queUnreadWrote.pop_front();
            }
        }

    } else if (mode == SendMessage) {
        teMessage->setFocus();
        te->setStyleSheet("QTextEdit { background-color: #DFFFCC; }");
        wroteMessage++;
        queUnreadWrote.push_back(te);
    }
}

bool Dialog::dialogReadByUser() {
    return dgReadByUser;
}

void Dialog::slotFinishReadMessage() {
    int un = unreadMessage;
    for (int i = 0; i < un; ++i) {
        queUnreadWrote.front()->setStyleSheet("QTextEdit { background-color: #FFFFFF; }");
        queUnreadWrote.pop_front();
    }
    dgReadByUser = true;
    setUnreadMessage(0);
    if (un > 0)
        emit readMessages();
    tmrNotActive.start(INTERVAL_NOT_ACTIVE);
}

void Dialog::slotNotActiveBehav() {
    dgReadByUser = false;
    tmrNotActive.stop();
    teMessage->clearFocus();
}

void Dialog::setWroteMessage(int x) {
    if (x == 0)
        for (int i = 0; i < wroteMessage; ++i) {
            queUnreadWrote.front()->setStyleSheet("QTextEdit { background-color: #FFFFFF; }");
            queUnreadWrote.pop_front();
        }
    wroteMessage = x;
}
