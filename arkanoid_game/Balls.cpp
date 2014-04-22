#include "Balls.h"

Ball::Ball()
{
  totalScore = 0;
  timeMove = NORMAL_TIME_ONE_MOVE;
  lenMove = LENGTH_MOVE_BALL;
  //gr.setColorAt(0.5, "#CD69FB");
  //gr.setColorAt(1, "#541870");
  //gr.setColorAt(0.5, "#423438");
  //gr.setColorAt(1, "#0C0C0C");
}

Ball::Ball(int sx, int sy, int rd, double cs, double sn)
{
  totalScore = 0;
  timeMove = NORMAL_TIME_ONE_MOVE;
  lenMove = LENGTH_MOVE_BALL;
  r = rd;

  pCirc = new QGraphicsEllipseItem(sx - r, sy - r, 2 * r, 2 * r);
  gr.setCenter(sx, sy);
  gr.setRadius(r);
  gr.setFocalPoint(QPoint(sx, sy) + QPoint(-4, 4));
  gr.setColorAt(0.1, Qt::white);
  gr.setColorAt(0.5, "#FB6996");
  gr.setColorAt(1, "#A84362");
  pCirc->setBrush(gr);
  pCirc->setPen(Qt::NoPen);
  vecMove = QPointF(cs, sn);
}

QBrush Ball::brush() const {return gr;}
QPointF Ball::centr() const {return QPointF(pCirc->x() + START_X_CIRCLE, pCirc->y() + START_Y_CIRCLE);}
double Ball::vect(QPointF v1, QPointF v2) const {return v1.x() * v2.y() - v1.y() * v2.x();}
double Ball::scal(QPointF v1, QPointF v2) const {return v1.x() * v2.x() + v1.y() * v2.y();}
double Ball::distToPoint(QPointF a, QPointF b) const
{
  return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}
double Ball::distToSegment(QPointF a, QPointF b, QPointF c) const
{
  double d = distToPoint(a, b);
  if (scal(b - a, c - a) > -EPS && scal(a - b, c - b) > -EPS) return fabs(vect(b - a, c - a)) / d;
  return min(distToPoint(a, c), distToPoint(b, c));
}

bool Ball::checkCollisionWithLine(QPointF str, QPointF fin) const
{
  if (distToSegment(str, fin, centr()) > r + EPS) return false;
  return distToPoint(centr(), str) <= r + EPS || distToPoint(centr(), fin) <= r + EPS ||
    (scal(fin - str, centr() - str) > -EPS && scal(str - fin, centr() - fin) > -EPS);
}

double Ball::intersect(QPointF c, QPointF d2)
{
  QPointF a = centr();
  QPointF d1 = vecMove * lenMove;
  double cp = vect(d1, d2);
  if (fabs(cp) < EPS) return 1;
  double t1 = vect(c - a, d2) / cp;
  double t2 = vect(c - a, d1) / cp;
  if (t1 < -EPS || t1 > 1 || t2 < -EPS || t2 > 1) return 1;
  return t1;
}
void Ball::intersect(QPointF c, double rd, double& t1, double& t2)
{
  t1 = t2 = 1;
  QPointF a = centr();
  QPointF d = vecMove * lenMove;
  a -= c;
  double A = d.x() * d.x() + d.y() * d.y();
  double B = 2 * (a.x() * d.x() + a.y() * d.y());
  double C = a.x() * a.x() + a.y() * a.y() - rd * rd;
  double D = B * B - 4 * A * C;
  if (D < -EPS) return;
  t1 = (-B - sqrt(D)) / (2 * A);
  t2 = (-B + sqrt(D)) / (2 * A);
  if (t1 < -EPS || t1 > 1) t1 = 1;
  if (t2 < -EPS || t2 > 1) t2 = 1;
}

double Ball::distToVertical(QPointF a, QPointF b) //a.x() == b.x()
{
  //qDebug() << "in vert\n";
  if (a.y() > b.y()) swap(a, b);
  double cf = 1, t1, t2;
  cf = min(cf, intersect(QPointF(a.x() - r, a.y()), QPointF(0, b.y() - a.y())));
  cf = min(cf, intersect(QPointF(a.x() + r, a.y()), QPointF(0, b.y() - a.y())));
  //cf = min(cf, intersect(QPointF(a.x() - r, a.y()), QPointF(2 * r, 0)));
  //cf = min(cf, intersect(QPointF(b.x() - r, b.y()), QPointF(2 * r, 0)));
  intersect(a, r, t1, t2);
  if ((centr() + vecMove * lenMove * t1).y() <= a.y() + EPS) cf = min(cf, t1);
  if ((centr() + vecMove * lenMove * t2).y() <= a.y() + EPS) cf = min(cf, t2);
  intersect(b, r, t1, t2);
  if ((centr() + vecMove * lenMove * t1).y() + EPS>= b.y()) cf = min(cf, t1);
  if ((centr() + vecMove * lenMove * t2).y() + EPS>= b.y()) cf = min(cf, t2);
  //qDebug() << "out vert\n";
  return lenMove * cf;
}

double Ball::distToHorizontal(QPointF a, QPointF b)//a.y() == b.y()
{
  //qDebug() << "in vert\n";
  if (a.x() > b.x()) swap(a, b);
  double cf = 1, t1, t2;
  cf = min(cf, intersect(QPointF(a.x(), a.y() - r), QPointF(b.x() - a.x(), 0)));
  cf = min(cf, intersect(QPointF(a.x(), a.y() + r), QPointF(b.x() - a.x(), 0)));
  //cf = min(cf, intersect(QPointF(a.x(), a.y() - r), QPointF(0, 2 * r)));
  //cf = min(cf, intersect(QPointF(b.x(), b.y() - r), QPointF(0, 2 * r)));
  intersect(a, r, t1, t2);
  if ((centr() + vecMove * lenMove * t1).x() <= a.x() + EPS) cf = min(cf, t1);
  if ((centr() + vecMove * lenMove * t2).x() <= a.x() + EPS) cf = min(cf, t2);
  intersect(b, r, t1, t2);
  if ((centr() + vecMove * lenMove * t1).x() + EPS >= b.x()) cf = min(cf, t1);
  if ((centr() + vecMove * lenMove * t2).x() + EPS >= b.x()) cf = min(cf, t2);
  return lenMove * cf;
}

bool Ball::checkHorizontal(QPointF p1, QPointF p2, QPointF& curMotionVec)
{
  if (checkCollisionWithLine(p1, p2) && checkCollisionWithLine(p1 - vecMove, p2 - vecMove)) return true;
  if (checkCollisionWithLine(p1 - curMotionVec, p2 - curMotionVec))
  {
    double distToBound = distToHorizontal(p1, p2);
    //qDebug() << centr() << " " << p1 << " " << p2 << " " << "dist to horiz = " << distToBound << endl;
    curMotionVec = vecMove * distToBound;
    //return true;
  }
  return false;
}

bool Ball::checkVertical(QPointF p1, QPointF p2, QPointF& curMotionVec)
{
  if (checkCollisionWithLine(p1, p2) && checkCollisionWithLine(p1 - vecMove, p2 - vecMove)) return true;
  if (checkCollisionWithLine(p1 - curMotionVec, p2 - curMotionVec))
  {
    double distToBound = distToVertical(p1, p2);
    //qDebug() << centr() << " " << p1 << " " << p2 << " " << "dist to vertical = " << distToBound << endl;
    curMotionVec = vecMove * distToBound;
    //return true;
  }
  return false;
}

int Ball::checkRectangle(double x, double y, double x1, double y1, QPointF& curMotionVec, bool needBottom)
{
  QPoint p00 = QPoint(x, y);
  QPoint pw0 = QPoint(x1, y);
  QPoint p0h = QPoint(x, y1);
  QPoint pwh = QPoint(x1, y1);
  bool yb = checkHorizontal(p00, pw0, curMotionVec) | (needBottom && checkHorizontal(p0h, pwh, curMotionVec));
  bool xb = checkVertical(p00, p0h, curMotionVec) | checkVertical(pw0, pwh, curMotionVec);
  return xb | (yb<<1);
}
void Ball::shift()
{
  pCirc->moveBy(vecMove.x() * lenMove, vecMove.y() * lenMove);
}
void Ball::shift(QPointF v)
{
  pCirc->moveBy(v.x(), v.y());
}

