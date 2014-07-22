#include "smilebutton.h"
#include <QBitmap>

SmileButton::SmileButton(const QImage& smile, int num, QWidget *parent) :
    QPushButton(parent)
{
    nm = "[smile:" + QString::number(num) + "]";
    QPalette p;
    p.setBrush(backgroundRole(), QBrush(smile));
    setPalette(p);
    setFlat(true);
    setAutoFillBackground(true);
    setMask(QPixmap::fromImage(smile).mask());
    this->smile = smile;
}


QImage SmileButton::image() {
    return smile;
}

QString SmileButton::name() {
    return nm;
}
