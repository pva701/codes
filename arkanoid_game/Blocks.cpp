#include "Blocks.h"
#include <iostream>//for debug IO
using namespace std;
////SimpleBlock

SimpleBlock::SimpleBlock(double x, double y, int wid, int heig, int scr):QRectF(x, y, wid, heig)
{
  setRect(x - THICKNESS_PEN_BLOCK / 2.0, y - THICKNESS_PEN_BLOCK / 2.0, wid + THICKNESS_PEN_BLOCK, heig + THICKNESS_PEN_BLOCK);
  pRect = new QGraphicsRectItem(x, y, wid, heig);
  score = scr;
  gr.setStart(x + wid / 2, y);
  gr.setFinalStop(x + wid / 2, y + heig);
  gr.setColorAt(0.4, QColor("#EDE742"));//желтый
  gr.setColorAt(1, QColor("#7E7E1A"));
  initBrush();
  //pbrTimeActive = new QProgressBar;
}
void SimpleBlock::initBrush()
{
  pRect->setBrush(gr);
  QPen myPen(gr, THICKNESS_PEN_BLOCK, Qt::SolidLine);
  myPen.setJoinStyle(Qt::RoundJoin);
  pRect->setPen(myPen);
}
void SimpleBlock::changeSateBallandPlatform(Ball& b, Platform& p)
{
}
string SimpleBlock::name() {return "SimpleBlock";}

///////TimeMoveBlock
TimeMoveBlock::TimeMoveBlock(int x, int y, int wid, int heig, int scr, double cf):SimpleBlock(x, y, wid, heig, scr)
{
  coef = cf;
  gr.setColorAt(0.4, QColor("#00E82D"));//зеленый
  gr.setColorAt(1, QColor("#247412"));
  initBrush();
}
void TimeMoveBlock::changeSateBallandPlatform(Ball& b, Platform& p)
{
}

string TimeMoveBlock::name() {return "TimeMoveBlock";}

///////FireBallBlock
FireBallBlock::FireBallBlock(int x, int y, int wid, int heig, int scr):SimpleBlock(x, y, wid, heig, scr)
{
  gr.setColorAt(0.4, QColor("#CE4A56"));//красный
  gr.setColorAt(1, QColor("#5F181F"));
  initBrush();
}

void FireBallBlock::changeSateBallandPlatform(Ball& b, Platform& p){}
string FireBallBlock::name() {return "FireBallBlock";}


///////LengthPlatformBlock
LengthPlatformBlock::LengthPlatformBlock(int x, int y, int wid, int heig, int scr, double cf):SimpleBlock(x, y, wid, heig, scr)
{
  coef = cf;
  gr.setColorAt(0.4, QColor("#5E53E9"));//синий
  gr.setColorAt(1, QColor("#141238"));
  initBrush();
}

void LengthPlatformBlock::changeSateBallandPlatform(Ball& b, Platform&)
{
}
string LengthPlatformBlock::name() {return "LengthPlatformBlock";}

///////HeightPlatformBlock
HeightPlatformBlock::HeightPlatformBlock(int x, int y, int wid, int heig, int scr, double cf):SimpleBlock(x, y, wid, heig, scr)
{
  coef = cf;
  gr.setColorAt(0.4, QColor("#AE40C4"));//фиолетовый
  gr.setColorAt(1, QColor("#3E0849"));
  initBrush();
}

void HeightPlatformBlock::changeSateBallPlatform(Ball& b, Platform&)
{
}
string HeightPlatformBlock::name() {return "HeightPlatformBlock";}
