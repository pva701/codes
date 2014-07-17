#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "user.h"

class Notification : public QDialog
{
    Q_OBJECT

    quint16 userId;
    QString pseud;
public:
    explicit Notification(quint16 id, const QString& pseud, QWidget *parent = 0);
    
signals:
    void yes(User us);
    void write(User us);
    void no();
private slots:
    void slotYes();
    void slotWrite();
    void slotNo();
};

#endif // NOTIFICATION_H
