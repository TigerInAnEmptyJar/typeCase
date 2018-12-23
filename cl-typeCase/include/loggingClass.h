#include <qstring.h>
#include <qobject.h>
#include <qdatetime.h>
#include "beamtimeparameter.h"
#include "eventpainter.h"
class TCanvas;
using namespace std;
class loggingClass: public QObject
{
Q_OBJECT
private:
  string filename;
  string computerID;
  string dataAnalysed;
  TCanvas *canvas;
  EventPainter painter;
  QTime timer;
  QDate startingDate;
  int eventsTo;
  int startingEvent;
  int actuallyDone;
public:
  loggingClass(string fn="");
  ~loggingClass();
 public slots:
   void setData(string d);
 void newRun(run_parameter &r);
 void eventsDone(int num);
 void eventsDone(TEvent *e);
 void setStartingEvent(int value);
 void setEventsToAnalyse(int value);
 void finished(int num);
 void onInitState(const char *state);
 void onCleanUpState(const char *state);
};
