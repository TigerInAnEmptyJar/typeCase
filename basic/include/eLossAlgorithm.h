#ifndef ELOSSAL
#define ELOSSAL
#include "algorithm.h"
#include "geometry.h"

class AELossAlgorithm : public AAlgorithm
{
private:
  int num;

public:
  AELossAlgorithm();
  virtual ~AELossAlgorithm();
  virtual void* process(void* ptr);
  virtual float energyLoss(momentum4D particle, float path);
  virtual float energyLoss(momentum4D particle, float path, float charge);
  virtual float energyLossM(int geantId, float momentum, float path);
  virtual float energyLossB(int geantId, float beta, float path);
  virtual float energyLossE(int geantId, float energy, float path);
  virtual float energyLoss(float path, float mass, float beta, float energy, float charge2);
  virtual float energyLoss_photon(int geantId, float energy, float path);
  virtual float penetrationDepth(momentum4D particle, double charge);
  virtual float betaAfter(float path, momentum4D before, float charge);
  virtual float betaAfter(float path, int geantId, float betaIn);
  virtual Vector parameters() const;
  virtual void setParameters(const Vector& parameter);
  virtual AELossAlgorithm* getClone() const;
};

#endif
