#include "algorithm.h"
#include "container.h"
#include <iostream>
#include <pthread.h>
// class QProcess;//#include <qprocess.h>
#include <map>
#include <mutex>

class run_parameter;
using namespace std;
class AReadFromTade : public AAlgorithm
{
private:
    TEvent& _event;
    class reader;
    static std::shared_ptr<reader> _reader;

  TRawHit*** hit;     //! [maxDet][maxHit]->
  int** numberOfHits; //! [maxDet]->
  int& EventNumber;   //!
  int& trigger;       //!
  ifstream* input;    //!
  int maxHit;
  int maxDet;
  int nextEventNumber;
  int nextEventCount;
  int nextTrigger;
  bool init;
  // int& whichID; //!
  void* input_mutex;
  // bool& validInput; //!
  bool myInput;
  int filetype;      //! 0 regular ascii, 1 zipped, 2 bz2, +8 fifo
  int cleanupInput;  //!
  string fifoFile;   //!
  string scriptFile; //!
  void* feeder;      //!
public:
  AReadFromTade(TRawHit*** hitIn, int& eventNumberIn, int& triggerIn, int** numHits, int maxdet,
                int maxhit, int& readID, void* input_mutexIn, bool& validInput);
  AReadFromTade(TEvent &event, TSetup const& setup);
  virtual ~AReadFromTade();
  virtual void process() override;
  virtual void newInput(const string& filename);
  virtual void newInput(run_parameter& r);
  virtual void newInput(ifstream* in);
  // signals:
  //  void inputError();
  //  void eventRead(int, int, int);
  //  void newRun(ifstream*);
  virtual void newEvent(int evtNr, int count, int trigger);
};
