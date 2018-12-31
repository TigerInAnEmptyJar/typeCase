#ifndef __MATPARAM
#define __MATPARAM
#include "baseparameter.h"
using namespace std;
class element_parameter : public base_parameter
{
private:
  float mass;
  float charge;
  float weight;

public:
  element_parameter(float m = 0.938, float c = 1, float w = -1, string n = "proton");
  element_parameter(const element_parameter& e);
  virtual ~element_parameter();
  element_parameter& operator=(const element_parameter& e);
  bool operator==(const element_parameter& e);
  float getMass() const;
  float getCharge() const;
  float getWeight() const;
  void setMass(float m);
  void setCharge(float c);
  void setWeight(float w);
};
ostream& operator<<(ostream& o, const element_parameter& ep);

class material_parameter : public base_parameter
{
private:
  // particle p;
  bool active;
  float radiationLength; //  in mm
  float density;         //  in units of gramms per cubic centimeter
  float speed;
  vector<element_parameter> elements;
  vector<float> energyloss;

public:
  material_parameter();
  material_parameter(const material_parameter& m);
  ~material_parameter();
  material_parameter& operator=(const material_parameter& m);
  bool IsActive() const;
  float EnergyLoss(int num) const;
  float Density() const;
  float RadiationLength() const;
  element_parameter Element(int num) const;
  void addElement(const element_parameter& ep);
  void setElement(int pos, const element_parameter& ep);
  int NumberOfElements() const;
  void deleteElement(int num);
  int NumberOfEnergyLossParams() const;
  void ClearEnergyLoss();
  void setActive(bool is);
  void setEnergyLoss(int num, float value);
  void setDensity(float value);
  void setRadiationLength(float value);
  float Speed() const;
  void setSpeed(float val);
  bool operator<(const material_parameter& p);
  bool operator>(const material_parameter& p);
  bool operator==(const material_parameter& p);
  bool operator<=(const material_parameter& p);
  bool operator>=(const material_parameter& p);
};
istream& operator>>(istream& i, material_parameter& m);
ostream& operator<<(ostream& o, const material_parameter& m);
template <>
bool takeItemFromVector<element_parameter>(vector<element_parameter>& vect,
                                           element_parameter& item);
template <>
bool moveItemToPosVector<element_parameter>(vector<element_parameter>& vect,
                                            element_parameter& item, int pos);
template <>
bool takeItemFromVector<material_parameter>(vector<material_parameter>& vect,
                                            material_parameter& item);
template <>
bool moveItemToPosVector<material_parameter>(vector<material_parameter>& vect,
                                             material_parameter& item, int pos);
#endif
