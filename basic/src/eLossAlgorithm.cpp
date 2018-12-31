#include "eLossAlgorithm.h"
#include "Eparticles.h"
AELossAlgorithm::AELossAlgorithm() : AAlgorithm("energy loss algorithm") {}
AELossAlgorithm::~AELossAlgorithm() {}
void* AELossAlgorithm::process(void* ptr) { return ptr; }
float AELossAlgorithm::energyLoss(momentum4D particle, float path)
{
  return energyLoss(particle, path, 1);
}
float AELossAlgorithm::energyLoss(momentum4D particle, float path, float charge)
{
  if (path == charge)
    return 0;
  if (particle.Energy() == 0)
    return 0;
  return 0;
}
float AELossAlgorithm::energyLossM(int geantId, float momentum, float path)
{
  momentum4D tmp;
  tmp.setPM(vector3D(0, 0, momentum), Eparticles::getMassG(geantId));
  return energyLoss(tmp, path, Eparticles::getChargeG(geantId));
}
float AELossAlgorithm::energyLossB(int geantId, float beta, float path)
{
  momentum4D tmp;
  tmp.setVM(vector3D(0, 0, beta), Eparticles::getMassG(geantId));
  return energyLoss(tmp, path, Eparticles::getChargeG(geantId));
}
float AELossAlgorithm::energyLossE(int geantId, float energy, float path)
{
  momentum4D tmp;
  float m = Eparticles::getMassG(geantId);
  tmp.setVE(vector3D(0, 0, sqrt(energy * energy - m * m)), m);
  return energyLoss(tmp, path, Eparticles::getChargeG(geantId));
}
float AELossAlgorithm::energyLoss(float path, float mass, float beta, float energy, float charge2)
{
  momentum4D tmp;
  if (energy == 0)
    tmp.setVM(vector3D(0, 0, beta), mass);
  else
    tmp.setVE(vector3D(0, 0, beta), mass + energy);
  return energyLoss(tmp, path, charge2);
}
float AELossAlgorithm::energyLoss_photon(int geantId, float energy, float path)
{
  momentum4D tmp;
  float m = Eparticles::getMassG(geantId);
  tmp.setVE(vector3D(0, 0, sqrt(energy * energy - m * m)), m);
  return energyLoss(tmp, path, 0);
}
float AELossAlgorithm::penetrationDepth(momentum4D particle, double charge)
{
  if (particle.Energy() == 0)
    return charge;
  return 0;
}
float AELossAlgorithm::betaAfter(float path, momentum4D before, float charge)
{
  float de = energyLoss(before, path, charge);
  float EA = before.Energy() - de;
  float p = sqrt(EA * EA - before.Mass() * before.Mass());
  before.setPE(before.Direction() * p, EA);
  return before.Beta();
}
float AELossAlgorithm::betaAfter(float path, int geantId, float betaIn)
{
  momentum4D before;
  before.setVM(vector3D(0, 0, betaIn), Eparticles::getMassG(geantId));
  float de = energyLoss(before, path, Eparticles::getChargeG(geantId));
  float EA = before.Energy() - de;
  float p = sqrt(EA * EA - before.Mass() * before.Mass());
  before.setPE(before.Direction() * p, EA);
  return before.Beta();
}
Vector AELossAlgorithm::parameters() const { return Vector(); }
void AELossAlgorithm::setParameters(const Vector& parameter)
{
  if (parameter.getDimension() == 0)
    return;
}
AELossAlgorithm* AELossAlgorithm::getClone() const { return new AELossAlgorithm(*this); }
