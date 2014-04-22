#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QApplication>
#include <QProgressBar>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QBrush>
#include <QPointF>
#include <QBrush>
#include <QProgressBar>
#include <QVariant>
#include <QDebug>

const int W_MAIN = 800;
const int H_MAIN = 600;
const int W_MAIN_MENU = 200;
const int H_MAIN_MENU = 600;
const int START_WIDTH_WIN = 900;
const int START_HEIGHT_WIN = 600;
const int WIDTH_RIGHT_SHIFT = 200;
const int START_POSX = 200;
const int START_POSY = 50;
const int WIDTH_SCENE = 700;
const int HEIGHT_SCENE = 600;
const int WIDTH_PLATFORM = 120;
const int HEIGHT_PLATFORM = 10;
const int START_R_CIRCLE = 11;
const int THICKNESS_PEN_PLATFORM = 5;
const int START_X_PLATFORM = (WIDTH_SCENE - WIDTH_PLATFORM) / 2;
const int START_Y_PLATFORM = HEIGHT_SCENE - 2 * HEIGHT_PLATFORM;
const int START_X_CIRCLE = START_X_PLATFORM + WIDTH_PLATFORM / 2;
const int START_Y_CIRCLE = START_Y_PLATFORM - START_R_CIRCLE - (THICKNESS_PEN_PLATFORM + 1) / 2;
const int THICKNESS_PEN_BLOCK = 9;
const int NORMAL_TIME_ONE_MOVE = 25;//25 ms
const int LENGTH_MOVE_BALL = 12;//12
const int WIDTH_BLOCK = 25;
const int HEIGHT_BLOCK = 20;
const int XLeng = WIDTH_BLOCK + THICKNESS_PEN_BLOCK;
const int YLeng = HEIGHT_BLOCK + THICKNESS_PEN_BLOCK;
const int ITERATION_BIN_SEARCH = 50;
const int MAX_NUMBER_BLOCKS_IN_ROW = 20;
const int SHIFT_WIN_SCENE = 10;
const double EPS = 1e-7;
const double INF = 1e9;
const QRectF RECT_PLATFORM = QRect(0, START_Y_PLATFORM, WIDTH_SCENE, HEIGHT_PLATFORM + THICKNESS_PEN_PLATFORM);
#endif // CONSTANTS_H
