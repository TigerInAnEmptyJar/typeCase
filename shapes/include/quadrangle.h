#ifndef SHAPE_QUADRANGLE
#define SHAPE_QUADRANGLE
#include"planeShape.h"

class quadrangle:public planeShape
{
 private:
  point3D A_, B_, C_, D_;
 public:
  quadrangle(point3D cent=point3D(), point3D a=point3D(), point3D b=point3D(), point3D c=point3D(), point3D d=point3D());
  quadrangle(const shape_parameter &description);
  virtual ~quadrangle();
  virtual int getNumberOfPoints()const;
  virtual point3D getPoint(int num)const;
  virtual float area()const;
  virtual float angularRange_Phi(const point3D &origin=point3D(0,0,0), const vector3D &zDirection=vector3D(0,0,1))const;
  virtual float angularRange_Theta(const point3D &origin=point3D(0,0,0), const vector3D &zDirection=vector3D(0,0,1))const;
  virtual vector3D distancePlane(const plane3D &p);
  virtual vector3D distance(const sLine3D& line);
  point3D A()const;
  point3D B()const;
  point3D C()const;
  point3D D()const;
  void setA(point3D p);
  void setB(point3D p);
  void setC(point3D p);
  void setD(point3D p);
  virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
  virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
  virtual shape_parameter description()const;
  static shape_parameter getDescription();
};


#endif
