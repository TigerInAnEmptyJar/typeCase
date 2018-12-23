#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include "algorithmparameter.h"
#include "CommonCalibrationParser.h"
class algorithm_parameter;
class run_parameter;
using namespace std;

class ACalibrationGeneration: public AAlgorithm
{
Q_OBJECT
 private:
  int numberOfCalibrationAlgorithms;
  TEvent &event;
  TSetup &setup;
  string outFileName;//!
  string author;//!
  string beamtime;//!
  bool useEventBase;
  int eventBase;
  int eventCounter;
  int fromRun;
  int uptoRun;
  vector<string> detectorNames;//!
  vector<string> calibrationNames;//!
  AAlgorithm **calibrationAlgorithms;//!
  void declareCalibs(const algorithm_parameter &descr);
 public:
  ACalibrationGeneration(TEvent &eventIn, TSetup &setupIn, const algorithm_parameter &descr);
  virtual ~ACalibrationGeneration();
  virtual void *process(void *ptr);
 signals:
  void newRun(run_parameter &r);
  void toEvaluate();
  public slots:
    virtual void getNewRun(run_parameter& r);
  virtual void writeCalibrationToFile(CommonCalibrationParser* parameter);
};
