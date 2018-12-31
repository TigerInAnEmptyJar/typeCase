#include "CommonCalibrationParser.h"
#include "algorithm.h"
#include "setup.h"
#include "track.h"
class run_parameter;
using namespace std;

class AapplyLRC : public AAlgorithm
{
  Q_OBJECT
private:
  TTrack** tracks;     //!
  int& numberOfTracks; //!
  int& eventNumber;    //!
  int& runNumber;      //!
  int nCorrectingDets;
  int* correctingDets;                   //[nCorrectingDets]
  int* correctingType;                   //[nCorrectingDets]
  float speedOfLight;                    //!
  float* speedInMaterial;                //[nCorrectingDets]
  CommonCalibrationParser*** parameters; //!
  int* currentRange;                     //[nCorrectingDets]
  int* numRanges;                        //[nCorrectingDets]
  point3D getOnPoint(volumeShape* sh, const sLine3D& path);

public:
  AapplyLRC(TSetup& setup, TTrack** trackIn, int& numTracksIn, int& evtNr, int& rnr,
            const algorithm_parameter& descr);
  virtual ~AapplyLRC();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
public slots:
  void onNewRun(run_parameter& rp);

public:
};
