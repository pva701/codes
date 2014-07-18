#ifndef TEXTEDITMESSAGE_H
#define TEXTEDITMESSAGE_H

#include <QTextEdit>
#include <QKeyEvent>

class TextEditMessage : public QTextEdit
{
    Q_OBJECT

    void keyPressEvent(QKeyEvent *e);
public:
    explicit TextEditMessage(QWidget *parent = 0);
signals:
    void enter();
public slots:
    
};

#endif // TEXTEDITMESSAGE_H
