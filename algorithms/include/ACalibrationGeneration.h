#include "CommonCalibrationParser.h"
#include "algorithm.h"
#include "algorithmparameter.h"
#include "event.h"
#include "setup.h"
class CalibrationAlgorithm;
class algorithm_parameter;
class run_parameter;
using namespace std;

class ACalibrationGeneration : public AAlgorithm
{
private:
  int numberOfCalibrationAlgorithms;
  TEvent& event;
  TSetup& setup;
  string outFileName; //!
  string author;      //!
  string beamtime;    //!
  bool useEventBase;
  int eventBase;
  int eventCounter;
  int fromRun;
  int uptoRun;
  vector<string> detectorNames;       //!
  vector<string> calibrationNames;    //!
  CalibrationAlgorithm** calibrationAlgorithms; //!
  void declareCalibs(const algorithm_parameter& descr);

public:
  ACalibrationGeneration(TEvent& eventIn, TSetup& setupIn, const algorithm_parameter& descr);
  ~ACalibrationGeneration() override;
  void process() override;
  void toEvaluate();

  virtual void getNewRun(run_parameter& r);
  virtual void writeCalibrationToFile(CommonCalibrationParser* parameter);
};
