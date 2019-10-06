#include "algorithm.h"
#include "setup.h"
#include "track.h"

class AVirtualMachine : public AAlgorithm
{
private:
  int& numberOfTracks; //!
  TTrack** tracks;     //!
  int** numberOfHits;  //!
  TCalibHit*** hits;   //!
  TSetup& setup;       //!
  int numberOfDetectors;
  int* detectorIDs;    //[numberOfDetectors]
  int* mode;           //[numberOfDetectors]
  int* maxHitPerTrack; //[numberOfDetectors]
  float promptThetaRange[2];
  bool commonStart;
  int startTDC;
  int particleID[200]; //!
  vector3D addHit(int detNum, int elNum, volumeShape* shape, const sLine3D& path, TTrack* source);
  float calculateEloss(float pathlength, TMaterial* mat, int particleID, float beta);
  int searchADetector(int detNum, int detID, vector3D& distance, sLine3D path, TTrack* source);

public:
  AVirtualMachine(TTrack** tracksIn, TCalibHit*** hitsIn, TSetup& setupIn, int& numberOfTracksIn,
                  int** numberOfHitsIn, const algorithm_parameter& descr);
  ~AVirtualMachine() override;
  void process() override;
  static algorithm_parameter getDescription();
};
