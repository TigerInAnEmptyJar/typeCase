#include <qapplication.h>
#include "cutsmw.h"

int main( int argc, char ** argv )
{
  QApplication a( argc, argv );
  cutsMW w;
  w.setCommandLineArguments(argc,argv);
  w.show();
  a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
  return a.exec();
}
