#include "CommonCalibrationParser.h"
#include "algorithm.h"
#include "algorithmparameter.h"
#include "setup.h"
#include "track.h"
using namespace std;
class run_parameter;
class ATofPixCorrection : public AAlgorithm
{
  Q_OBJECT
private:
  TTrack** tracks;     //!
  int& numberOfTracks; //!
  const int numPixelTypes;
  int& EventNumber;                         //!
  int& RunNumber;                           //!
  int* pixelTypes;                          //[numPixelTypes]
  CommonCalibrationParser*** calibrations7; //!
  int* numRanges7;                          //[numPixelTypes]
  CommonCalibrationParser*** calibrations9; //!
  int* numRanges9;                          //[numPixelTypes]
  bool* use9;                               //[numPixelTypes]
  int* actualRange7;                        //[numPixelTypes]
  int* actualRange9;                        //[numPixelTypes]
  float* CinSzinti;                         //[numPixelTypes]
  bool runTime;
  bool calib;

public:
  ATofPixCorrection(int& eventNr, int& runNr, TTrack** tracksIn, int& numberOfTracksIn,
                    TSetup& setup, const algorithm_parameter& descr,
                    const vector<string>& calibrationFiles);
  ATofPixCorrection(int& eventNr, int& runNr, TTrack** tracksIn, int& numberOfTracksIn,
                    TSetup& setup, vector<int> pixelTypes, const vector<string>& calibrationFiles);
  virtual ~ATofPixCorrection();
  virtual void* process(void* ptr);
  virtual void getParameterFromFiles(const vector<string>& calibrationFiles);
  virtual bool checkRanges();
public slots:
  virtual void setNewRun(run_parameter& run);
};
