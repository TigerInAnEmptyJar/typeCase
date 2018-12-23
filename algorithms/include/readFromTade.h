#include <iostream>
#include "algorithm.h"
#include "container.h"
#include <pthread.h>
//class QProcess;//#include <qprocess.h>
class run_parameter;
using namespace std;
class AReadFromTade: public AAlgorithm
{
  Q_OBJECT
    private:
  TRawHit*** hit;//! [maxDet][maxHit]->
  int **numberOfHits;//! [maxDet]->
  int &EventNumber;//!
  int &trigger;//!
  ifstream *input;//!
  int maxHit;
  int maxDet;
  int nextEventNumber;
  int nextEventCount;
  int nextTrigger;
  bool init;
  int &whichID;//!
  void *input_mutex;
  bool &validInput;//!
  bool myInput;
  int filetype;//! 0 regular ascii, 1 zipped, 2 bz2, +8 fifo
  int cleanupInput;//!
  string fifoFile;//!
  string scriptFile;//!
  void* feeder;//!
 public:
  AReadFromTade( TRawHit*** hitIn,int &eventNumberIn, int &triggerIn, int **numHits, int maxdet, int maxhit, int &readID,void *input_mutexIn,bool &validInput);
  virtual ~AReadFromTade();
  virtual void *process(void*ptr);
public slots:
  virtual void newInput(const string & filename);
 virtual void newInput(run_parameter&r);
 virtual void newInput(ifstream*in);
 signals:
  virtual void inputError();
  virtual void eventRead(int,int,int);
  virtual void newRun(ifstream*);
  public slots:
    virtual void newEvent(int evtNr, int count, int trigger);
};
