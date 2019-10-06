#include "TTree.h"
#include "algorithm.h"
#include "container.h"

#include <boost/signals2.hpp>

class AConversion : public AAlgorithm
{
private:
  int actualEvent;
  TTree* convertedData; //!
  TEvent& event;        //!
  void* input_mutex;    //!
  bool& validInput;     //!
  boost::signals2::signal<void()> _eventReadSignal;

public:
  AConversion(const string& filename, TEvent& ev, void* input_mutexIn, bool& validInputIn);
  ~AConversion() override;
  void process() override;
  boost::signals2::connection connectEventReadSignal(std::function<void()> subscriber);
  void newEvent();
  void getNewRun(const string& filename);
};
