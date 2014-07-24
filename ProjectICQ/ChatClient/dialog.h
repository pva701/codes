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
#include <QTimer>
#include <QQueue>

class Dialog: public QSplitter, public QListWidgetItem
{
    Q_OBJECT
private:
    //QListWidgetItem *item;
    QListWidget *lwHistory;
    QToolBar *panel;
    TextEditMessage *teMessage;
    quint16 numDialog;
    QString tittle;
    QWidget *tabWidget;
    QMenu *smileMenu;
    int unreadMessage;
    int wroteMessage;
    bool createdWidgetx;
    bool dgReadByUser;
    void processCalc(int& heig, int& mx, int& curLine, int curw, int curh);
    QTimer tmrNotActive;
    QQueue <QTextEdit*> queUnreadWrote;

    static const int INTERVAL_NOT_ACTIVE = 25000;
    //static const QString UNREAD_MESSAGE = "";
    //static const QString WROTE_MESSAGE = "";
    //static const QString USUAL_MESSAGE = "";
public:
    Dialog(QWidget *parent = 0);
    Dialog(quint16 numDialogg, QString namee, QWidget *parent = 0);
    QString name() const;
    quint16 dialog() const;
    //QTextEdit* history() const;
    QTextEdit* message() const;
    QWidget* widget();
    void createWidget();
    bool createdWidget();
    void setUnreadMessage(int x);
    int unread();
    void reloadResource(QTextEdit *doc);
    bool dialogReadByUser();
    void setWroteMessage(int x);
    enum InsertingMode {LoadHistory, ReceivedMessage, SendMessage};

    int W_CNT;
    int H_CNT;
    QImage *smiles;
    void loadSmiles();
    static int metainfoSmile(const QString& s, int pos);
    QString toStringFromDocument();
    static QTextDocument* toDocumentFromString(const QString& msg);
    void appendToHistory(const QString& name, const QDateTime& sendTime, QTextDocument *document, InsertingMode mode);
private slots:
    void slotSmilesClicked();
    void slotClickedSmileMenu();
    void slotFinishReadMessage();
    void slotNotActiveBehav();
signals:
    void readMessages();
};

#endif // DIALOG_H
