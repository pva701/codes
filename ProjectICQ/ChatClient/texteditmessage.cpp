#include "texteditmessage.h"

#include <QDebug>
#include <QPlainTextEdit>

TextEditMessage::TextEditMessage(QWidget *parent) :
    QTextEdit(parent)
{
}

void TextEditMessage::keyPressEvent(QKeyEvent *e) {
    if ((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) && (e->modifiers() & Qt::ControlModifier))
        append("");
    else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        emit enter();
    else
        QTextEdit::keyPressEvent(e);
    emit gotFocus();
}


void TextEditMessage::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e);
    emit gotFocus();
}

void TextEditMessage::focusOutEvent(QFocusEvent *e) {
    QTextEdit::focusOutEvent(e);
    emit lostFocus();
}
