#ifndef HEXPRISM
#define HEXPRISM
#include "volumeShape.h"
class hexPrism:public volumeShape
{
 private:
    vector3D dir1;
    vector3D dir2;
    vector3D dir3;
  int stacki;
 public:
  hexPrism(point3D c, vector3D thickness,vector3D toFirstPoint, vector3D keyWidth, int st);
  hexPrism(const volumeShape &s); 
  hexPrism(const shape_parameter &description);
  virtual ~hexPrism();
  virtual vector3D getDirection(int num) const;
  void setDirection(int num,vector3D v);
  int getStackingOrder()const;
  void setStackingOrder(int st);
  virtual volumeShape *getNext(int times,int stackType);
  virtual volumeShape *getEnvelope(int times,int stackType);
  virtual point3D entrance(const sLine3D &line);
  virtual vector3D distance(const sLine3D &line);
  virtual sLine3D Normal(const sLine3D &line);
  virtual bool cut(const fiber &f);
  virtual Vector HitParams(const sLine3D &line);
  virtual Vector HitParams(const planeShape &shape, point3D origin);
  virtual Vector Hitting(const sLine3D &line);
  virtual float getFlightPathInShape(const sLine3D &line);
  virtual volumeShape* getClone();
  virtual int suspect(const sLine3D &line, int stackType);
  virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
  virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
  virtual shape_parameter description()const;
  static shape_parameter getDescription();
};

#endif
