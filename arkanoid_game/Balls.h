#ifndef BALLS_H_INCLUDED
#define BALLS_H_INCLUDED

#include "Constants.h"

#include <cmath>
#include <vector>
using namespace std;

class Ball
{
public:
  Ball();
  Ball(int, int, int, double, double);
  QBrush brush() const;
  bool checkCollisionWithLine(QPointF, QPointF) const;
  QPointF centr() const;
  void shift();
  void shift(QPointF);
  int checkRectangle(double, double, double, double, QPointF&, bool);
protected:
  friend class Interface;
  friend class GameForm;
  int timeMove, totalScore, lenMove;
  vector <string> historyBlocks;
  QGraphicsEllipseItem *pCirc;
  int r;
  QRadialGradient gr;
  QPointF vecMove;
private:
  double vect(QPointF, QPointF) const;
  double scal(QPointF, QPointF) const;
  double distToPoint(QPointF, QPointF) const;
  double distToSegment(QPointF, QPointF, QPointF) const;
  double distToVertical(QPointF, QPointF);
  double distToHorizontal(QPointF, QPointF);
  bool checkVertical(QPointF, QPointF, QPointF&);
  bool checkHorizontal(QPointF, QPointF, QPointF&);
  double intersect(QPointF, QPointF);
  void intersect(QPointF, double, double&, double&);
};

#endif // BALLS_H_INCLUDED
