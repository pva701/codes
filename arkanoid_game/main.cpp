#include "Interface.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Interface Run(app, 0);
  Run.show();
  return app.exec();
}
