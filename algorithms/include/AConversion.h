#include "TTree.h"
#include "algorithm.h"
#include "container.h"

class AConversion : public AAlgorithm
{
  Q_OBJECT
private:
  int actualEvent;
  TTree* convertedData; //!
  TEvent& event;        //!
  void* input_mutex;    //!
  bool& validInput;     //!
public:
  AConversion(const string& filename, TEvent& ev, void* input_mutexIn, bool& validInputIn);
  virtual ~AConversion();
  virtual void* Process(void* ptr);
signals:
  void eventRead();
public slots:
  virtual void newEvent();
  virtual void getNewRun(const string& filename);
};
