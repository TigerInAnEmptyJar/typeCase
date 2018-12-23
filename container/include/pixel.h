#ifndef PIXEL
#define PIXEL
#include "planeShape.h"
#include "calibHit.h"
#include "TBase.h"

class TPixel:public TBase
{
 private:
  const int numDet;  //  TRef p_shape;
  planeShape* p_shape;//  TRef *elements;//TRef is a nice idea, if it worked
  TCalibHit **elements;//[accNum]->
  planeShape *emergencySave;//!
  int accNum;
  int ID;
  float maxChi;
  bool defined;//!
  bool valid;
 public:
  TPixel(int numDetIn=3);
  ~TPixel();
  planeShape *getShape();
  TCalibHit getElement(int num) const;
  TCalibHit &getElementr(int num);
  int getNumberOfElements() const;
  int getNumberOfActualElements() const;
  void setShape(planeShape *sh);
  void addElement(TCalibHit &hit);
  void resetElements();
  int getID() const;
  void setID(int IDIn);
  float getMaxChiSquared()const;
  void setMaxChiSquared(float value);
  bool isDefined();
  void reset();
  bool operator ==(TPixel &);
  bool isValid()const;
  void setValid(bool v=true);
};
#endif
