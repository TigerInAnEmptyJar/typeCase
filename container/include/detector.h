#ifndef DETECTOR
#define DETECTOR
#include "material.h"
#include "volumeShape.h"
#include "TBase.h"
class TDetector:public TBase
{
 private:
  int numElements;
  int stackType;
  bool circular;
  const int detNum;
  volumeShape** v_shape;// volumeShape **v_shape;//[numElements]->
  TMaterial* mat;//TMaterial *mat;//->
  bool defined;//!
 public:
  TDetector(TMaterial &matIn, int detNumIn); 
  TDetector(const TDetector &det); 
  TDetector();
  ~TDetector();
  int getDetectorNumber() const;
  int getStackType() const;
  int getNumberOfElements() const;
  int getID()const;
  bool isCircular()const;
  TMaterial *getMaterial() const;
  volumeShape *getOverallShape() const;
  volumeShape *getShape(int ElementNumber) const;
  void setNumberOfElements(int num);
  void setStackType(int st);
  void setMaterial(TMaterial &m);
  void setShapeFirstElement(volumeShape *sh);
  void setShapes(int n, volumeShape**shapes);
  void setCircular(bool circ=true);
  void operator=(const TDetector &d);
  bool isDefined() const;
};
#endif
