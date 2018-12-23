#include "planeShape.h"

class circle:public planeShape
{
 private:
  float rad;
 public:
  circle(point3D cen, vector3D nor, float rIn);
  circle(const shape_parameter &description);
  virtual ~circle();
  virtual int getNumberOfPoints()const;
  virtual point3D getPoint(int num)const;
  virtual float area()const;
  virtual float angularRange_Phi(const point3D &origin=point3D(0,0,0), const vector3D &zDirection=vector3D(0,0,1))const;
  virtual float angularRange_Theta(const point3D &origin=point3D(0,0,0), const vector3D &zDirection=vector3D(0,0,1))const;
  float radius()const;
  void setRadius(float);
  virtual vector3D distancePlane(const plane3D &p);
  virtual vector3D distance(const sLine3D &line);
  virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
  virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
  virtual shape_parameter description()const;
  static shape_parameter getDescription();
};
