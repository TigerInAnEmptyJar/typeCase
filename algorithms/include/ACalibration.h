#include "algorithm.h"
#include "calibHit.h"
#include "detector.h"
//#include "tofParam.h"
#include "CommonCalibrationParser.h"
class run_parameter;
class beamTime_parameter;
using namespace std;
class ACalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TCalibHit** hits;                     //![numberOfHits]->
  int& numberOfHits;                    //!
  CommonCalibrationParser*** parameter; //!
                                        /*   float
                                         * ****parameters;//![6][numberOfElements][numberOfParameters][numRanges]//parameters[parID][elemNum][parNum][Range]
                                         */
  /*   int **beginRunRange;//![6][numberOfRanges]//range[parID][rangeNum] */
  /*   int **beginEventRange;//![6][numberOfRanges] */
  /*   int **endRunRange;//![6][numberOfRanges] */
  /*   int **endEventRange;//![6][numberOfRanges] */
  int* numberOfRanges; //![6]
  int* actualRange;    //[6]
  int numberOfElements;
  int numberOfParameters;
  const int DetID;
  bool correctADC;
  bool correctTDC;
  int& runNumber;   //!
  int& eventNumber; //!
  void getFromFiles(const vector<string>& calibrationFiles);

public:
  ACalibration(TCalibHit** hitsIn, int& numberOfHitsIn, const TDetector& det,
               const vector<string>& filenames, int& runNr, int& evtNr, bool cA, bool cT);
  virtual ~ACalibration();
  virtual void* process(void* ptr);
  virtual bool checkRange();
public slots:
  void getNewRun(run_parameter& r); //, beamTime_parameter &b);
};
