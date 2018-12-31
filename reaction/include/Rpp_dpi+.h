#include "Eparticles.h"
#include "RbaseReaction.h"

using namespace std;
class Rpp_dpiPlus : public RbaseReaction
{
private:
  double thetaDmax;
  double thetaPimax;
  double coplanarity;
  double angleLimit;
  double crossover;

public:
  Rpp_dpiPlus();
  Rpp_dpiPlus(AFitAlgorithm* kinFitIn, momentum4D p1,
              momentum4D p2 = momentum4D(0, 0, 0, Eparticles::getMassG(14)));
  Rpp_dpiPlus(const Rpp_dpiPlus& rIn);
  virtual ~Rpp_dpiPlus();
  virtual int getNumberOfPrimaries() const;
  virtual int getNumberOfSecondaries() const;
  /*   virtual float check(TTrack **tracks, int num); */
  /*   virtual float justCheck(TTrack **tracks, int num, int
   * *indexes=NULL,momentum4D
   * *results=NULL); */
  virtual float check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                      TTrack** tracks_2s, int num_2s);
  virtual float justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                          TTrack** tracks_2s, int num_2s, int* indexes = NULL,
                          momentum4D* results = NULL);
  virtual RbaseReaction* checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                                  TTrack** tracks_2s, int num_2s);
  virtual void setInitParticles(int num, momentum4D* p);
  /*   virtual RbaseReaction* checkNew(TTrack **tracks,int num); */
  virtual RbaseReaction* getClone();
  void setCoplanarityDifference(double value);
  void setAngleLimit(double value);
  void setCrossoverMomentum(double value);
  virtual void calculate(int justGeometry = 1);
};
