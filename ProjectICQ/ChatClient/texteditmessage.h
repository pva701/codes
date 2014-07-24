#ifndef TEXTEDITMESSAGE_H
#define TEXTEDITMESSAGE_H

#include <QTextEdit>
#include <QKeyEvent>

class TextEditMessage : public QTextEdit
{
    Q_OBJECT

    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
public:
    explicit TextEditMessage(QWidget *parent = 0);
signals:
    void enter();
    void gotFocus();
    void lostFocus();
public slots:
    
};

#endif // TEXTEDITMESSAGE_H
