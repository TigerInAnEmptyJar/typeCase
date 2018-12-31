#include "documentation_mw.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication a(argc, argv);
  documentation_mw w;
  w.show();
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  return a.exec();
}
