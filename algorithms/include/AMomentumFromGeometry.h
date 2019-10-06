#include "algorithm.h"
#include "setup.h"
#include "track.h"
class AMomentumFromGeometry : public AAlgorithm
{
private:
  TTrack** tracks;     //!
  int& numberOfTracks; //!
  momentum4D inParticle1;
  momentum4D inParticle2;
  momentum4D CM;

public:
  AMomentumFromGeometry(TSetup& setup, TTrack** tracksIn, int& numberOfTracksIn);
  ~AMomentumFromGeometry() override;
  void process() override;
};
