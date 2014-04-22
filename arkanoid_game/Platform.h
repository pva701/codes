#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED
#include "Constants.h"
#include "Balls.h"

class Platform//:public QPointF
{
public:
  Platform();
  Platform(double, double, int, int);
  QBrush brush() const;
  double x() const;
  double y() const;
protected:
  friend class Interface;
  friend class GameForm;
  int lenShift, len, heig;
  QGraphicsRectItem *pRect;
  QLinearGradient gr;
};

#endif // PLATFORM_H_INCLUDED
