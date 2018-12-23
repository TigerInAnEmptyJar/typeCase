#include "RbaseReaction.h"
#include "Eparticles.h"

using namespace std;
class Rppelastic:public RbaseReaction
{
  bool useTiming;
  float CoplanarityDifference;
  float GammaDifference;
 public:
  Rppelastic();
  Rppelastic(AFitAlgorithm *kinFitIn,momentum4D p1, momentum4D p2=momentum4D(0,0,0,Eparticles::getMassG(14)));
  Rppelastic(const Rppelastic &rIn);
  virtual ~Rppelastic();
  virtual int getNumberOfPrimaries() const;
  virtual int getNumberOfSecondaries() const;
/*   virtual float check(TTrack **tracks, int num); */
/*   virtual float justCheck(TTrack **tracks, int num, int *indexes=NULL, momentum4D *results=NULL); */
  virtual float check(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s);
  virtual float justCheck(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s, int *indexes=NULL, momentum4D *results=NULL);
  virtual RbaseReaction* checkNew(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s);
  virtual void setInitParticles(int num, momentum4D *p);
  virtual void setCoplanarityDifference(float val);
  virtual void setGammaDifference(float val);
  float getCoplanarityDifference()const;
  float getGammaDifference()const;
/*   virtual RbaseReaction* checkNew(TTrack **tracks,int num); */
  virtual RbaseReaction* getClone();
  virtual void calculate(int justGeometry=1);
};
