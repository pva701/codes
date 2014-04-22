#ifndef INTERFACE_H
#define INTERFACE_H

#include <QVector>
#include "Blocks.h"
#include "Platform.h"
#include "Balls.h"
#include "Constants.h"
#include <ctime>
class Interface;
class GameForm;

class GameForm:public QWidget
{
Q_OBJECT
  QSize currentWindowSize, sizeField;
  QGraphicsScene *scene;
  QGraphicsView *view;
  QVector <SimpleBlock* > blocks;
  Ball ball;
  Platform plat;
  QImage backgr;
  QVBoxLayout *lytPanelInformation;
  QPushButton *butPause, *butMainMenu;
  QWidget *widPanelInformation;
  Interface *widMainWindow;
  QTimer tmrOneMotion;
  QLabel *labDebugInformation;
public:
  friend class Interface;
  GameForm(Interface*, QWidget* par = 0);
protected:
  void startGame(Ball, Platform, QVector <SimpleBlock* >, QSize);
  void resizeEvent(QResizeEvent *);
  void resizeScene(int, int);
  //void  keyReleaseEvent(QKeyEvent*);
  void keyPressEvent(QKeyEvent*);
  void shiftPlatform(int);
  void stopGame(bool);
private slots:
  void clickPause();
  void oneMotionBall();
  void returnMainMenu();
};

class Interface: public QWidget
{
Q_OBJECT
  friend class GameForm;
  QWidget *widMainMenu;
  QMessageBox *diChangeLog;
  GameForm *frmGameplay;
public:
  Interface(QApplication&, QWidget*);
private slots:
  void newGame();
  void changeLog();
};

#endif // INTERFACE_H
