#include "cylinder.h"
class shape_parameter;
using namespace std;
class strawTube:public cylinder
{
 private:
  vector3D stackingDirection;
  vector3D shift;
  float isoChrone;
  int halvedAt;
  int nHalvedElements;
 public:
  strawTube();
  strawTube(point3D centerIn, vector3D lineDirection, float rad, vector3D stackDirection, vector3D shiftDirection, float isoChroneR, int halv, int halvAt);
  strawTube(const shape_parameter &description);
  strawTube(const strawTube& tube);
  virtual ~strawTube();
  virtual float getIsoChroneRadius()const;
  virtual vector3D getStackingDirection()const;
  virtual vector3D getShift()const;
  virtual int getNumberOfHalvedElements()const;
  virtual int getHalvedAt()const;
  virtual void setIsoChroneRadius(float value);
  virtual void setStackingDirection(vector3D value);
  virtual void setShift(vector3D value);
  virtual void setNumberOfHalvedElements(int value);
  virtual void setHalvedAt(int value);
  virtual float getFlightPathInShape(const sLine3D &line);
  virtual volumeShape* getClone();
  virtual void operator=(const volumeShape &s);
  virtual volumeShape *getNext(int times, int stackType);
  virtual volumeShape *getEnvelope(int times,int stackType);
  virtual int suspect(const sLine3D &line, int stackType);
  virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
  virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
  virtual shape_parameter description()const;
  static shape_parameter getDescription();
};
