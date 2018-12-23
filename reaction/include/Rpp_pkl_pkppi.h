#include "RbaseReaction.h"
#include "Eparticles.h"

using namespace std;
class Rpp_pkl_pkppi:public RbaseReaction
{
  RbaseReaction *Ldecay;
  float missMassLimit;
  float angleLimit;
  float momentumLimit;
  float maximumProtonAngle;
  bool exactRequirement;
  bool useMultiLineFit;
  bool useKinfit;
  bool useDecay;
  float angleRanges[4];
  float reso[4];
  int useRep[4];
  float multiLineFit(TTrack &v1,TTrack &v2, TTrack &v3, TTrack & v4, vector3D& r1,vector3D &r2,vector3D& r3,vector3D &r4,point3D &p1,point3D &p2);
  float applyKinfit(const vector3D &v1,const vector3D &v2, const vector3D &v3, const vector3D& v4, float beta1, float beta2, float beta3, float beta4, int &combination, momentum4D *res,vector3D *errors);
  float applyAKinfit(const vector3D &v1, const vector3D &v2, const vector3D &v3, const vector3D &v4, float beta1, float beta2, float beta3, float beta4, momentum4D *res,vector3D *errors);
 public:
  Rpp_pkl_pkppi();
  Rpp_pkl_pkppi(AFitAlgorithm *kinFitIn,momentum4D p1, momentum4D p2);
  Rpp_pkl_pkppi(const Rpp_pkl_pkppi &rIn);
  virtual ~Rpp_pkl_pkppi();
  virtual int getNumberOfPrimaries() const;
  virtual int getNumberOfSecondaries() const;
  momentum4D getPromptProton()const;
  momentum4D getDecayProton()const;
  momentum4D getKaon()const;
  momentum4D getDecayPion()const;
  momentum4D getLambda()const;
  int getPromptProtonID()const;
  int getDecayProtonID()const;
  int getKaonID()const;
  int getLambdaID()const;
  int getDecayPionID()const;
  RbaseReaction *getLambdaDecay()const;
  void setLambdaDecay(RbaseReaction &reaction);
  void setMissingMassLimit(float v);
  void setAngleLimit(float v);
  void setExact(bool e=true);
  void setMomentumLimit(float v);
  void setAngleRangesPK(float mn, float mx);
  void setAngleRangesDecay(float mn, float mx);
  void setMaximumProtonAngle(float ang);
  float getMaximumProtonAngle()const;
  bool getExactRequirement()const;
  bool getMultiLineFit()const;
  bool getUseKinfit()const;
  void setMultiLineFit(bool value);
  void setUseKinfit(bool value);
  void setUseDecay(bool value);
  bool UseDecay()const;
  float getTOFResolution(int n)const;
  void setTOFResolution(float f1, float f2, float f3, float f4);
  void setTOFResolution(int n, float f);
  int getRepresentation(int n)const;
  void setRepresentation(int n, int rep);
  void setRepresentation(int rep1, int rep2, int rep3, int rep4);
  bool calculateDecay(TTrack *t1, TTrack *t2, TTrack *t3, float *m, momentum4D *results);
  bool calculatePrimary(TTrack *t1, TTrack *t2, momentum4D lambda, float *m, momentum4D *results);
/*   virtual float check(TTrack **tracks, int num); */
/*   virtual float justCheck(TTrack **tracks, int num, int* indexes=NULL, momentum4D *results=NULL); */
  virtual void setInitParticles(int num, momentum4D *p);
/*   virtual RbaseReaction* checkNew(TTrack **tracks,int num); */
  virtual float check(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s);
  virtual float justCheck(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s, int *indexes=NULL, momentum4D *results=NULL);
  virtual RbaseReaction* checkNew(TTrack **tracks_nd, int num_nd, TTrack **tracks_1s, int num_1s, TTrack **tracks_2s, int num_2s);
  virtual RbaseReaction* getClone();
};
