#ifndef CLOCK_H
#define CLOCK_H

#include <QApplication>
//#include <QtWidgets>

class MClock:public QProgressBar
{
  Q_OBJECT
  QTimer *ptimer;
  int id;
public:
  MClock(QWidget* par = 0);
private slots:
  void updateBar();
};
#endif // CLOCK_H
