#ifndef RBASEREACTION
#define RBASEREACTION
#include "geometry.h"
#include "track.h"
class AFitAlgorithm;

class RbaseReaction : public TBase
{
protected:
  const int numberOfInitParticles;
  momentum4D* initParticles; //[numberOfInitParticles]
  const int numberOfTracks;
  TTrack** tracks; //[numberOfTracks]
  int numberOfParticles;
  momentum4D* particles; //[numberOfParticles]
  momentum4D cm;
  int* particleIDs;
  float excessEnergy;
  momentum4D* cmparticles; //[numberOfParticles]
  AFitAlgorithm* fkinFit;  //!
  float chiLimit;
  float chi2;
  void calculateCM();

public:
  RbaseReaction(AFitAlgorithm* kinFitIn, int nTracks = 2, int nInits = 2);
  RbaseReaction(const RbaseReaction& rIn);
  virtual ~RbaseReaction();
  int getNumberOfTracks() const;
  virtual int getNumberOfPrimaries() const;
  virtual int getNumberOfSecondaries() const;
  virtual momentum4D getParticle(int num) const;
  int getParticleID(int num) const;
  virtual void setTrack(int num, TTrack* track);
  TTrack* getTrack(int num);
  //  virtual float check(TTrack **tracks, int num);
  //  virtual float justCheck(TTrack **tracks, int num, int *indexes=NULL,
  //  momentum4D
  //  *results=NULL);
  //  virtual RbaseReaction* checkNew(TTrack **tracks,int num);
  virtual float check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                      TTrack** tracks_2s, int num_2s);
  virtual float justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                          TTrack** tracks_2s, int num_2s, int* indexes = NULL,
                          momentum4D* results = NULL);
  virtual RbaseReaction* checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                                  TTrack** tracks_2s, int num_2s);
  void setChiLimit(float chi);
  float getChiLimit() const;
  float getChiSquared() const;
  virtual void setInitParticles(int num, momentum4D* p);
  momentum4D getInitParticle(int num = 0) const;
  float getExcessEnergy() const;
  momentum4D getCenterOfMass() const;
  momentum4D getCMParticle(int num) const;
  AFitAlgorithm* getKinFit() const;
  virtual RbaseReaction* getClone();
  virtual void calculate(int justGeometry = 1);
  //  bool operator<(const RbaseReaction &r);
  //  bool operator<=(const RbaseReaction &r);
  //  bool operator>(const RbaseReaction &r);
  //  bool operator>=(const RbaseReaction &r);
  //  virtual Bool_t IsSortable();
  //  ClassDef(RbaseReaction,2)
};

RbaseReaction* getReactionFromName(const string& name);
#endif
