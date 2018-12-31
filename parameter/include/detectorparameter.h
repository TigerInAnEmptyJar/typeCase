#ifndef __DETPARAM
#define __DETPARAM
#include "baseparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"
using namespace std;
class detector_parameter : public base_parameter
{
private:
  int numberOfElements;
  float maxDist;
  shape_parameter shape;
  int stackType;
  int fmaterial;
  int ID;
  bool circular;
  material_parameter* mat;

public:
  detector_parameter();
  ~detector_parameter();
  int getNumberOfElements() const;
  void setNumberOfElements(int n);
  int getStackType() const;
  void setStackType(int s);
  int getID() const;
  void setID(int id);
  bool isCircular() const;
  void setCircular(bool circ = true);
  float getMaxDistance() const;
  void setMaxDistance(float value);
  shape_parameter getShape() const;
  void setShape(const shape_parameter& sh);
  void setMaterial(int mat);
  int getMaterial() const;
  material_parameter* material();
  void setMaterial(material_parameter* matIn);
  void operator=(const detector_parameter& d);
};
ostream& operator<<(ostream& o, const detector_parameter& d);
istream& operator>>(istream& i, detector_parameter& d);

class reaction_parameter : public base_parameter
{
private:
  bool twoBeams;
  int firstMat;
  int secMat;
  float firstMom;
  float secMom;
  shape_parameter targetShape;

public:
  reaction_parameter(const shape_parameter& target = shape_parameter(), bool twoBeamIn = false,
                     int first = 0, int sec = 0);
  ~reaction_parameter();
  reaction_parameter(const reaction_parameter& r);
  reaction_parameter& operator=(const reaction_parameter& r);
  bool hasTwoBeams() const;
  void setTwoBeams(bool tb = true);
  int getMaterial(int num) const;
  int getTargetMaterial() const;
  void setMaterial(int num, int mat);
  void setTargetMaterial(int mat);
  float getBeamMomentum(int num = 0) const;
  void setBeamMomentum(float value, int num = 0);
  shape_parameter getTargetShape() const;
  void setTargetShape(const shape_parameter& s);
};
ostream& operator<<(ostream& o, const reaction_parameter& d);
istream& operator>>(istream& i, reaction_parameter& d);
template <>
bool takeItemFromVector<detector_parameter>(vector<detector_parameter>& vect,
                                            detector_parameter& item);
template <>
bool moveItemToPosVector<detector_parameter>(vector<detector_parameter>& vect,
                                             detector_parameter& item, int pos);
template <>
bool takeItemFromVector<reaction_parameter>(vector<reaction_parameter>& vect,
                                            reaction_parameter& item);
template <>
bool moveItemToPosVector<reaction_parameter>(vector<reaction_parameter>& vect,
                                             reaction_parameter& item, int pos);
#endif
