#include "Interface.h"

#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

const char* VERSION_ARKANOID = "Арканоид 1.3.3";

/////class GameForm

GameForm::GameForm(Interface* mw, QWidget* par): QWidget(par)
{
  setWindowIcon(QIcon(":/icon.png"));
  setWindowTitle(VERSION_ARKANOID);
  scene = new QGraphicsScene(QRectF(0, 0, WIDTH_SCENE, HEIGHT_SCENE), this);
  view = new QGraphicsView(scene, this);
  view->setRenderHint(QPainter::Antialiasing);
  widMainWindow = mw;
  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);///тут убрать, добавить
  move(widMainWindow->pos());

  lytPanelInformation = new QVBoxLayout;
  widPanelInformation = new QWidget(this);
  butPause = new QPushButton("Старт");
  butMainMenu = new QPushButton("В меню");
  butPause->setMinimumSize(START_WIDTH_WIN - WIDTH_SCENE - 3 * SHIFT_WIN_SCENE, 50);
  butMainMenu->setMinimumSize(START_WIDTH_WIN - WIDTH_SCENE - 3 * SHIFT_WIN_SCENE, 50);
  labDebugInformation = new QLabel;
  lytPanelInformation->addWidget(butPause);
  lytPanelInformation->addWidget(butMainMenu);
  lytPanelInformation->addWidget(labDebugInformation);
  widPanelInformation->setLayout(lytPanelInformation);
  widPanelInformation->move(WIDTH_SCENE + SHIFT_WIN_SCENE, 0);
  connect(butPause, SIGNAL(clicked()), SLOT(clickPause()));
  connect(&tmrOneMotion, SIGNAL(timeout()), SLOT(oneMotionBall()));
  connect(butMainMenu, SIGNAL(clicked()), SLOT(returnMainMenu()));
}

void GameForm::startGame(Ball bl, Platform pl, QVector <SimpleBlock* > fld, QSize szField)
{
  ball = bl;
  plat = pl;
  blocks = fld;
  //ball.pCirc->moveBy(615.32 - START_X_CIRCLE + 6.20278 / 11.0924 * 12, 570.804 - START_Y_CIRCLE - 9.196 / 11.0924 * 12);
  sizeField = szField;
  scene->addItem(ball.pCirc);
  scene->addItem(plat.pRect);
  //scene->addItem(new QGraphicsRectItem(plat.x(), plat.y(), plat.len, plat.heig));
  for (int i = 0; i < blocks.size(); ++i) scene->addItem(blocks[i]->pRect);

  ///debug section
  stringstream debug;
  char ss[90];
  sprintf(ss, "Start vector = (%.3f, %.3f)\n",(double)ball.vecMove.x(), (double)ball.vecMove.y());
  debug << "Size = " << sizeField.height() << "\n" << ss << endl;
  string st = debug.str();
  QString blya;
  for (int i = 0; i < st.size(); ++i) blya += st[i];

  labDebugInformation->setText(blya);
  butPause->setText("Старт");
  //move(START_POSX, START_POSY);
  //setMinimumSize(START_WIDTH_WIN, START_HEIGHT_WIN);
  setFixedSize(START_WIDTH_WIN, START_HEIGHT_WIN);
  currentWindowSize = QSize(START_WIDTH_WIN, START_HEIGHT_WIN);
  int numPict = rand() % 3;
  if (numPict == 0) backgr.load(":/background_round1.jpg");
  else if (numPict == 1) backgr.load(":/background_round2.jpg");
  else backgr.load(":/background_round3.jpg");
  resizeScene(WIDTH_SCENE, HEIGHT_SCENE);
  show();
}

void GameForm::returnMainMenu()
{
  if (tmrOneMotion.isActive()) clickPause();
  QMessageBox *msgBackMenu = new QMessageBox(QMessageBox::Warning, "Арканоид", "<b>Вернуться в главное меню?</b>",
                                         QMessageBox::Ok | QMessageBox::Cancel, this);
  msgBackMenu->setWindowIcon(QIcon(":/icon.png"));
  int res = msgBackMenu->exec();
  delete msgBackMenu;
  if (res == QMessageBox::Ok)
  {
    scene->clear();
    hide();
    widMainWindow->show();
    widMainWindow->move(widMainWindow->pos());
  }
}

void GameForm::stopGame(bool win)
{
  if (tmrOneMotion.isActive()) clickPause();
  QString txt = win ? "Вы выиграли!" : "Вы проиграли";
  QMessageBox *msgStopGame = new QMessageBox(QMessageBox::Information, "Игра окончена", txt, QMessageBox::Ok | QMessageBox::Cancel, this);
  msgStopGame->setButtonText(QMessageBox::Ok, "Новая игра");
  msgStopGame->setButtonText(QMessageBox::Cancel, "В меню");
  int r = msgStopGame->exec();
  delete msgStopGame;
  scene->clear();
  if (r == QMessageBox::Ok) widMainWindow->newGame();
  else
  {
    hide();
    widMainWindow->show();
    widMainWindow->move(widMainWindow->pos());
  }
}

void GameForm::resizeEvent(QResizeEvent *)
{
  int curw = width();
  int curh = height();
  if (curw != currentWindowSize.width() && curh != currentWindowSize.height())
    currentWindowSize = QSize(curw, curw *  START_HEIGHT_WIN / START_WIDTH_WIN);
  else if (curw == currentWindowSize.width())
    currentWindowSize = QSize(START_WIDTH_WIN * curh / START_HEIGHT_WIN, curh);
  else
    currentWindowSize = QSize(curw, START_HEIGHT_WIN * curw / START_WIDTH_WIN);
  resizeScene(WIDTH_SCENE * currentWindowSize.width() / START_WIDTH_WIN, currentWindowSize.height());
  resize(currentWindowSize);
}

void GameForm::resizeScene(int w, int h)
{
  QImage backgrCur = backgr.scaled(w, h);
  scene->setSceneRect(0, 0, w, h);
  scene->setBackgroundBrush(QBrush(backgrCur));
  view->resize(w + 2, h + 2);
}

void GameForm::keyPressEvent(QKeyEvent* p)
{
  if (p->modifiers()) return;
  if (p->key() == Qt::Key_Escape || p->key() == Qt::Key_Space || p->key() == Qt::Key_Enter) clickPause();
  else if (p->key() == Qt::Key_A) shiftPlatform(-1);
  else if (p->key() == Qt::Key_D)  shiftPlatform(1);
}

void GameForm::clickPause()
{
  if (tmrOneMotion.isActive())
  {
    tmrOneMotion.stop();
    butPause->setText("Продолжить");
  } else
  {
    butPause->setText("Пауза");
    oneMotionBall();
    tmrOneMotion.start(ball.timeMove);
  }
}

void GameForm::oneMotionBall()
{
  if (ball.centr().y() - ball.r >= scene->height())
  {
    stopGame(false);
    return;
  }
  QPointF curMotionVec = ball.vecMove * ball.lenMove;
  int msk = ball.checkRectangle(0, 0, scene->width(), scene->height(), curMotionVec, true);///bll
  msk |= ball.checkRectangle(plat.x(), plat.y(), plat.x() + plat.len, plat.y() + plat.heig, curMotionVec, true);
  /*qDebug() << ball.centr() << endl;
  qDebug() << curMotionVec << endl;
  qDebug() << "msk = " << msk << endl;
  qDebug() << "len = " << curMotionVec.x() / ball.vecMove.x() << endl;
  qDebug() << "plat = " << plat.x() << " " << plat.y() << endl;*/
  QVector <int> idDelete;
  for(int i = 0; i < blocks.size(); ++i)
  {
    SimpleBlock *curBlock = blocks[i];
    int rectMsk = ball.checkRectangle(curBlock->x(), curBlock->y(), curBlock->x() + curBlock->width(), curBlock->y() + curBlock->height(), curMotionVec, true);
    if (rectMsk)
    {
      idDelete.push_back(i);
      msk |= rectMsk;
    }
  }
  QVector <SimpleBlock* > tmpBlocks;
  for (int i = 0, j = 0; i < blocks.size(); ++i)
    if (j < idDelete.size() && i == idDelete[j])
    {
      scene->removeItem(blocks[i]->pRect);
      ++j;
    } else
      tmpBlocks.push_back(blocks[i]);
  blocks = tmpBlocks;
  if (msk&1)
  {
    curMotionVec.rx() *= -1;
    ball.vecMove.rx() *= -1;
  }
  if (msk>>1)
  {
    curMotionVec.ry() *= -1;
    ball.vecMove.ry() *= -1;
  }
  if (!blocks.size()) stopGame(true);
  if (!msk) ball.shift(curMotionVec);
  else oneMotionBall();
}

void GameForm::shiftPlatform(int vc)
{
  if (!tmrOneMotion.isActive()) return;
  int tmplen, godx = 0;
  if (vc == 1) tmplen = std::min(plat.lenShift, int(scene->width() - plat.x() - plat.len + 1));
  else tmplen = std::min(plat.lenShift, (int)plat.x());
  for (int dx = tmplen; dx >= 0; --dx)
  {
    QPointF tmp;
    bool coll = ball.checkRectangle(plat.x() + dx * vc, plat.y(), plat.x() + dx * vc + plat.len, plat.y() + plat.heig, tmp, true);
    if (!coll)
    {
      godx = dx;
      break;
    }
  }
  //plat.setX(plat.x() + vc * godx);
  plat.pRect->setX(plat.pRect->x() + vc * godx);
}

////class Interface
Interface::Interface(QApplication& app, QWidget* par = 0):QWidget(par)
{
  QPushButton *butNewGame, *butLoadGame, *butConstructor, *butExit, *butChangeLog;
  app.setKeyboardInputInterval(0);
  setWindowIcon(QIcon(":/icon.png"));
  setWindowTitle(VERSION_ARKANOID);
  QVBoxLayout *rectMainMenu;
  widMainMenu = new QWidget(this);
  rectMainMenu = new QVBoxLayout;
  butNewGame = new QPushButton("Новая игра");
  butLoadGame = new QPushButton("Загрузить игру");
  butConstructor = new QPushButton("Конструктор");
  butChangeLog = new QPushButton("История изменений");
  butExit = new QPushButton("Выход");

  butNewGame->setMinimumSize(150, 50);
  butLoadGame->setMinimumSize(150, 50);
  butConstructor->setMinimumSize(150, 50);
  butChangeLog->setMinimumSize(150, 50);
  butExit->setMinimumSize(150, 50);

  rectMainMenu->addWidget(butNewGame);
  //rectMainMenu->addWidget(butLoadGame);
  //rectMainMenu->addWidget(butConstructor);
  rectMainMenu->addWidget(butChangeLog);
  rectMainMenu->addWidget(butExit);

  widMainMenu->setLayout(rectMainMenu);
  widMainMenu->setGeometry(300, 0, W_MAIN_MENU, H_MAIN_MENU);///change

  QPixmap backgrPict(":/meteorit2.jpg");
  QPalette palMainWindow;
  palMainWindow.setBrush(backgroundRole(), QBrush(backgrPict));
  setPalette(palMainWindow);
  move(START_POSX, START_POSY);
  setFixedSize(W_MAIN, H_MAIN);
  QObject::connect(butNewGame, SIGNAL(clicked()), SLOT(newGame()));
  QObject::connect(butChangeLog, SIGNAL(clicked()), SLOT(changeLog()));
  QObject::connect(butExit, SIGNAL(clicked()), &app, SLOT(quit()));

  //QObject::connect(&butNewGame, SIGNAL(clicked()), &mainWindow, SLOT(hide()));
  frmGameplay = new GameForm(this);
  diChangeLog = new QMessageBox(this);

  srand(time(NULL));
}

void Interface::newGame()
{
  hide();
  int ang = rand() % 40 + 20;
  if (rand()&1) ang = 180 - ang;
  Ball ball(START_X_CIRCLE, START_Y_CIRCLE, START_R_CIRCLE, cos(M_PI * ang / 180.0), -sin(M_PI * ang / 180.0));
  Platform plat(START_X_PLATFORM, START_Y_PLATFORM, WIDTH_PLATFORM, HEIGHT_PLATFORM);
  //Platform plat(0, 600 - 13, WIDTH_PLATFORM, HEIGHT_PLATFORM);
  QVector <SimpleBlock* > field;
  int XLeng = 25 + THICKNESS_PEN_BLOCK;
  int YLeng = 20 + THICKNESS_PEN_BLOCK;
  int hrnd = rand() % 5 + 1;
  for (int x = 5, i = 0; i <= MAX_NUMBER_BLOCKS_IN_ROW; x += XLeng + 1, i++)
    for (int y = 5, j = 1; y + YLeng <= HEIGHT_SCENE / 2 && j <= hrnd; y += YLeng + 1, j++)
    {
      int p = rand();
      if (p % 5 == 0) field.push_back(new SimpleBlock(x, y, WIDTH_BLOCK, HEIGHT_BLOCK, 20));
      else if (p % 5 == 1) field.push_back(new FireBallBlock(x, y, WIDTH_BLOCK, HEIGHT_BLOCK, 20));
      else if (p % 5 == 2) field.push_back(new LengthPlatformBlock(x, y, WIDTH_BLOCK, HEIGHT_BLOCK, 20, 1.5));
      else if (p % 5 == 3) field.push_back(new HeightPlatformBlock(x, y, WIDTH_BLOCK, HEIGHT_BLOCK, 20, 1.5));
      else field.push_back(new TimeMoveBlock(x, y, WIDTH_BLOCK, HEIGHT_BLOCK, 20, 1.5));
    }
  QSize sizeField(20, hrnd);
  frmGameplay->startGame(ball, plat, field, sizeField);
}

void Interface::changeLog()
{
  QString msg = "<h3><tt>Версия 1.3.3</tt></h3>"
                "<ol><li>Увеличина скорость движения шарика"
                "<li>Немного улучшена реакция платформы на нажатие клавиш"
                "<li>Исправлены ошибки, связанные с платформой"
                "<li>Незначительно улучшен интерфейс"
                "<li>Добалвена история изменений";
                //"<li>Решена проблема с отображением фона. Добавлены новые фоны"

  diChangeLog->setWindowTitle("История изменений");
  diChangeLog->setWindowIcon(QIcon(":/icon.png"));
  diChangeLog->setText(msg);
  diChangeLog->setModal(false);
  diChangeLog->show();
}
