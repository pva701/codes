#ifndef SMILEBUTTON_H
#define SMILEBUTTON_H

#include <QPushButton>
#include <QImage>

class SmileButton : public QPushButton
{
    Q_OBJECT
    QImage smile;
    QString nm;
public:
    explicit SmileButton(const QImage& smile, int num, QWidget *parent = 0);
    QImage image();
    QString name();
signals:
    
public slots:
    
};

#endif // SMILEBUTTON_H
