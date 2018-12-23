#include "algorithm.h"
#include "track.h"
#include"setup.h"
class AMomentumFromGeometry:public AAlgorithm
{
 private:
  TTrack **tracks;//!
  int &numberOfTracks;//!
  momentum4D inParticle1;
  momentum4D inParticle2;
  momentum4D CM;
 public:
  AMomentumFromGeometry(TSetup &setup, TTrack **tracksIn, int &numberOfTracksIn);
  virtual ~AMomentumFromGeometry();
  virtual void *process(void*ptr);
};
