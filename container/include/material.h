#ifndef MATERIAL
#define MATERIAL
#include "TBase.h"
#include "geometry.h"
#include <string>
class AELossAlgorithm;
using namespace std;
class TMaterial : public TBase
{
private:
  const int maxElements;
  int numElements;
  float* masses;  //[numElements] MeV
  float* charges; //[numElements]
  float* weights; //[numElements] -1 == equal parts
  float density;
  float radiationLength;
  float speedOfLight;
  AELossAlgorithm* energyloss; //->
  string name;                 //!
  bool defined;                //!
public:
  TMaterial(string nameIn = "proton", int max = 100, float mass = 0.93827231, float charge = 1,
            float weight = -1);
  ~TMaterial();
  string getName() const;
  void setName(string n);
  int getNumberOfElements() const;
  int getMaxNumberOfElements() const;
  float getRadiationLength() const;
  float getDensity() const;
  void setRadiationLength(float rad);
  void setDensity(float d);
  float getMass(int num = 0) const;
  float getCharge(int num = 0) const;
  float getWeight(int num = 0) const;
  float getMeanMass() const;
  float getMeanCharge() const;
  void addElement(float mass, float charge, float weight = -1);
  void setElement(int num, float mass, float charge, float weight = -1);
  void popElement();
  AELossAlgorithm* getAlgorithm();
  void setAlgorithm(AELossAlgorithm* energyLoss);
  float EnergyLoss(momentum4D particle, float path);
  float getSpeedOfLight() const;
  void setSpeedOfLight(float val);
  bool isDefined();
};
#endif
