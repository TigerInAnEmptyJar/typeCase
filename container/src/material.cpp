#include "material.h"

TMaterial::TMaterial(string nameIn, int max, float mass, float charge, float weight)
    : TBase("TMaterial"), maxElements(max)
{
  name = nameIn;
  masses = new float[maxElements];
  charges = new float[maxElements];
  weights = new float[maxElements];
  if (mass > 0) {
    masses[0] = mass;
    charges[0] = charge;
    weights[0] = weight;
    numElements = 1;
    defined = true;
  } else {
    numElements = 0;
    defined = false;
  }
  energyloss = 0;
  density = 0;
  radiationLength = 0;
}

float TMaterial::getWeight(int num) const
{
  if ((num < 0) || (num >= numElements))
    return -1;
  if (weights[num] >= 0)
    return weights[num];
  float sum = 0;
  float dist = 0;
  for (int i = 0; i < numElements; i++) {
    if (weights[i] == -1)
      dist = dist - weights[i];
    else
      sum = sum + weights[i];
  }
  if (sum > 1)
    return 0;
  return (1 - sum) / dist * weights[num];
}
float TMaterial::getMeanMass() const
{
  float ret = 0;
  for (int i = 0; i < numElements; i++) {
    ret += masses[i] * getWeight(i);
  }
  return ret;
}
float TMaterial::getMeanCharge() const
{
  float ret = 0;
  for (int i = 0; i < numElements; i++) {
    ret += charges[i] * getWeight(i);
  }
  return ret;
}
float TMaterial::EnergyLoss(momentum4D particle, float path)
{
  return (particle * particle) * path;
}

bool TMaterial::isDefined() { return defined; }
TMaterial::~TMaterial()
{
  delete[] masses;
  delete[] charges;
  delete[] weights;
}

string TMaterial::getName() const { return name; }

void TMaterial::setName(string n) { name = n; }

int TMaterial::getNumberOfElements() const { return numElements; }

int TMaterial::getMaxNumberOfElements() const { return maxElements; }

float TMaterial::getRadiationLength() const { return radiationLength; }

float TMaterial::getDensity() const { return density; }

void TMaterial::setRadiationLength(float rad) { radiationLength = rad; }

void TMaterial::setDensity(float d) { density = d; }

float TMaterial::getMass(int num) const
{
  if ((num < 0) || (num >= numElements))
    return -1;
  return masses[num];
}
float TMaterial::getSpeedOfLight() const { return speedOfLight; }
void TMaterial::setSpeedOfLight(float val) { speedOfLight = val; }

float TMaterial::getCharge(int num) const
{
  if ((num < 0) || (num >= numElements))
    return -1;
  return charges[num];
}

void TMaterial::addElement(float mass, float charge, float weight)
{
  if (numElements >= maxElements)
    return;
  masses[numElements] = mass;
  charges[numElements] = charge;
  weights[numElements] = weight;
}

void TMaterial::setElement(int num, float mass, float charge, float weight)
{
  if ((num < 0) || (num > numElements))
    return;
  if (num == numElements) {
    addElement(mass, charge, weight);
    return;
  }
  masses[num] = mass;
  charges[num] = charge;
  weights[num] = weight;
}

void TMaterial::popElement() { numElements--; }

AELossAlgorithm* TMaterial::getAlgorithm()
{
  return energyloss; //(AELossAlgorithm*)energyloss.GetObject();
}

void TMaterial::setAlgorithm(AELossAlgorithm* energyLoss) { energyloss = energyLoss; }
