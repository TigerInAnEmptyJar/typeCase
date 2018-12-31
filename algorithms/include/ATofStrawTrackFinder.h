#include "algorithm.h"
#include "setup.h"
#include "track.h"

using namespace std;
class tofTaskManager;
class ATofStrawTrackFinder : public AAlgorithm
{
private:
  tofTaskManager* trackFinder; //!
  TTrack** tracks;             //!
  int& numberOfTracks;         //!
  int** numberOfHits;          //!
  TCalibHit*** hits;           //!
  int numDets;
  bool resetTrackNumber;
  point3D* planeCenter;
  vector3D beamDirection;

public:
  ATofStrawTrackFinder(TTrack** tracksIn, int& numberOfTracksIn, TSetup& setup, TCalibHit*** hitsIn,
                       int** numberOfHitsIn, const algorithm_parameter& descr);
  virtual ~ATofStrawTrackFinder();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
};
