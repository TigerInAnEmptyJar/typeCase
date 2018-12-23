#include "geometry.h"
#include "TBase.h"
using namespace std;

class RthreeParticleDecay:public TBase
{
 private:
  double masses[4];
  double mom_cm[4];
  double mom_lab[4];
  double E_cm[4];
  double E_lab[4];
  double beta_cm[4];
  double beta_lab[4];
  vector3D d_lab[4];
  vector3D d_cm[4];
  momentum4D P_lab[4];
  momentum4D P_cm[4];
  bool possible;
 public:
  RthreeParticleDecay();
  RthreeParticleDecay(const RthreeParticleDecay& dec);
  RthreeParticleDecay(double massI, double massO1, double massO2, double massO3);
  bool isPossible()const;
  void setMasses(double *massIn); //has to have 4 components!!!
  void setMasses(double massI, double massO1, double massO2, double massO3);
  double mass(int particle)const;//particle =-1 for InitParticle, 0, 1, 2, for decay particles
  double pInitLab()const;
  double betaInitLab()const;
  double eInitLab()const;
  double pLab(int particle)const;
  double betaLab(int particle)const;
  double eLab(int particle)const;
  double pCM(int particle)const;
  double betaCM(int particle)const;
  double eCM(int particle)const;
  momentum4D PLab(int particle)const;
  momentum4D PCM(int particle)const;
  void check(vector3D dirInit, vector3D dir1, vector3D dir2, vector3D dir3, double value, int type);//calculate properties of decay. Three particle decay needs the three direction vectors and an additional value, such as Momentum (t==1), Energy (t==2), KineticE (t==3), beta (t==4), with type==t*nParticles+(ParticleIdentifier+1)
};
