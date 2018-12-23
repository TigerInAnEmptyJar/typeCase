#include "algorithm.h"
#include "setup.h"
#include "track.h"

using namespace std;

class AFindDecayInTracks: public AAlgorithm
{
 private:
  TSetup &setup;
  TTrack **tracks;
  int &numberOfTracks;
  bool vertexFitTarget;
  float maxDistanceToTarget;
  bool vertexFitDecay;
  float maxDistanceToCommon;
 public:
  AFindDecayInTracks(TSetup &setupIn,int &numberOfTracksIn, TTrack **tracksIn, const algorithm_parameter &descr);
  virtual ~AFindDecayInTracks();
  virtual void *process(void *ptr);
  static algorithm_parameter getDescription();
};
