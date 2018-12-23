#ifndef CLUSTER
#define CLUSTER
#include "geometry.h"
#include "pixel.h"
#include "TBase.h"
class TCluster:public TBase
{
 private:
  const int numElem;
  int accElem;
  TPixel **pix;//TPixel **pix;//[numElem]->
  point3D cpoint;
  int id;
  float radius;
  bool defined;//!
 public:
  TCluster(int maxElements=50);
  ~TCluster();
  int getNumberOfPixels() const;
  int getMaxNumberOfPixels() const;
  point3D getCenter() const;
  TPixel getPixel(int num) const;
  void setCenter(point3D p);
  void addPixel(TPixel &p);
  void resetPixel();
  point3D &getCenterr();
  TPixel &getPixelr(int num);
  void setID(int IDin);
  int getID()const;
  int ID()const;
  bool isDefined();
  vector3D getClusterRadius()const;
  void setClusterRadius(float value);
  void calculateClusterRadius();
  void reset();
};
#endif
