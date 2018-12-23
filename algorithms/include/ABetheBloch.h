#ifndef ABETHEBLOCH
#define ABETHEBLOCH
#include "eLossAlgorithm.h"


class ABetheBloch: public AELossAlgorithm
{
 private:
  double c1; //material independent constant
  double m_electron; 
  double c_light;
  float epsilon;
  float N_a;
  int nComponents;
  double density;
  double *c2; //material depenedent constant
  double *c3; //excitation energy
  double *Z;
  double *A;
  double *weight;
  double stepwidth;
  double radiationLength;
 public:
  ABetheBloch();
  ABetheBloch(const ABetheBloch &algo);
  ~ABetheBloch();
  virtual void setParameters(const Vector &parameter);
  virtual void *process(void*ptr);
  virtual float energyLoss(momentum4D particle, float path);
  virtual float energyLoss(momentum4D particle, float path,float charge);
  virtual float energyLossM(int geantId, float momentum, float path);
  virtual float energyLossB(int geantId, float beta, float path);
  virtual float energyLossE(int geantId, float energy, float path);
  virtual float energyLoss(float path, float mass, float beta, float energy, float charge2);
  virtual float energyLoss_photon(int geantId, float energy, float path);
  virtual float penetrationDepth(momentum4D particle, double charge);
  virtual float betaAfter(float path, momentum4D before, float charge);
  virtual float betaAfter(float path, int geantId, float betaIn);
  virtual Vector parameters()const;
  double stepSize()const;
  void setStepsize(double value);
  virtual AELossAlgorithm *getClone()const;
};
#endif
