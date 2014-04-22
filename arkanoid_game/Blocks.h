#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include "Balls.h"
#include "Platform.h"
#include <cstring>
using namespace std;

class SimpleBlock:public QRectF
{
public:
  SimpleBlock() {}
  SimpleBlock(double, double, int, int, int);
  virtual string name();
protected:
  friend class Interface;
  friend class GameForm;
  int score;
  QLinearGradient gr;
  QGraphicsRectItem *pRect;
  void initBrush();
  //static QTimer tmrBlock;
  //static QProgressBar *pbrTimeActive;
  virtual void changeSateBallandPlatform(Ball&, Platform&);
};

class TimeMoveBlock:public SimpleBlock
{
public:
  TimeMoveBlock(int, int, int, int, int, double);
protected:
  double coef;
  //void initBrush();
  void changeSateBallandPlatform(Ball&, Platform&);
  string name();
};

class FireBallBlock: public SimpleBlock
{
public:
  FireBallBlock(int, int, int, int, int);
protected:
  //void initBrush();
  void changeSateBallandPlatform(Ball&, Platform&);
  string name();
};

class LengthPlatformBlock:public SimpleBlock
{
public:
  LengthPlatformBlock(int, int, int, int, int, double);
protected:
  double coef;
  //void initBrush();
  void changeSateBallandPlatform(Ball&, Platform&);
  string name();
};

class HeightPlatformBlock:public SimpleBlock
{
public:
  HeightPlatformBlock(int, int, int, int, int, double);
protected:
  double coef;
  //void initBrush();
  void changeSateBallPlatform(Ball&, Platform&);
  string name();
};

#endif // BLOCKS_H_INCLUDED
