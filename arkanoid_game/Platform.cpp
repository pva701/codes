#include "Platform.h"

Platform::Platform() {}
Platform::Platform(double px, double py, int l, int h)
{
  //setX(px - THICKNESS_PEN_BLOCK / 2);
  //setY(py - THICKNESS_PEN_PLATFORM / 2.0 + 1);
  len = l + THICKNESS_PEN_PLATFORM;
  heig = h + THICKNESS_PEN_PLATFORM;
  pRect = new QGraphicsRectItem(px, py, l, h);
  gr.setStart(px + l / 2, py);
  gr.setFinalStop(px + l / 2, py + h);
  gr.setColorAt(0.5, "#16E4EC");
  gr.setColorAt(1.0, "#18A5AA");
  QPen myPen(brush(), THICKNESS_PEN_PLATFORM, Qt::SolidLine);
  myPen.setJoinStyle(Qt::RoundJoin);
  pRect->setPen(myPen);
  pRect->setBrush(brush());
  lenShift = 20;
  //pRect->setFlags(QGraphicsItem::ItemIsMovable);
}

QBrush Platform::brush() const {return gr;}
double Platform::x() const {return pRect->x() - THICKNESS_PEN_PLATFORM / 2.0 + START_X_PLATFORM;}
double Platform::y() const {return pRect->y() - THICKNESS_PEN_PLATFORM / 2.0 + START_Y_PLATFORM;}
QVariant QGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
  qDebug() << "in item change\n";
  if (change == ItemPositionChange && scene())
  {
    // value - это новое положение.
    QPointF newPos = value.toPointF();
    if (!RECT_PLATFORM.contains(newPos))
    {
      newPos.setX(qMin(RECT_PLATFORM.right(), qMax(newPos.x(), RECT_PLATFORM.left())));
      newPos.setY(qMin(RECT_PLATFORM.bottom(), qMax(newPos.y(), RECT_PLATFORM.top())));
      return newPos;
    }
  }
  return QGraphicsItem::itemChange(change, value);
}
