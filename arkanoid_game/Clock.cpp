#include "Clock.h"
MClock::MClock(QWidget *par):QProgressBar(par)
{
  setAlignment(Qt::AlignCenter);
  id = 100;
  setFixedWidth(300);
  setFixedHeight(30);
  //setFixedSize(300, 60);
  ptimer = new QTimer(this);
  ptimer->start(100);
  connect(ptimer, SIGNAL(timeout()), SLOT(updateBar()));
}

void MClock::updateBar()
{
  setValue(--id);
  if (id == 0) ptimer->stop();
}

