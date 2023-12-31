#include "algorithm.h"
#include "track.h"
class TSetup;
using namespace std;
class AAssignHitsToTracks : public AAlgorithm
{
private:
  TSetup& setup;       //!
  TCalibHit*** hits;   //!
  int** numberOfHits;  //!
  int& numberOfTracks; //!
  TTrack** tracks;     //!
  int numberOfDetectors;
  int* detectorIDs;        //[numberOfDetectors]
  int* mode;               //[numberOfDetectors]
  int* maxElementDistance; //[numberOfDetectors]
  bool doFit;
  float maxChi;
  float maxDistT;
  float maxDistV;
  bool mustCheck;

public:
  AAssignHitsToTracks(TSetup& setupIn, TTrack** tracksIn, TCalibHit*** hitIn, int** numberOfHitsIn,
                      int& numberOfTracksIn, const algorithm_parameter& descr);
  ~AAssignHitsToTracks() override;
  void process() override;
  static algorithm_parameter getDescription();
  static bool checkParameter(algorithm_parameter const& a);
};
