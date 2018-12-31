#ifndef SPHERICRECTANGLE
#define SPHERICRECTANGLE

#include "planeShape.h"

class sphericRectangle : public planeShape
{
private:
  point3D A_, B_, C_, D_;

public:
  sphericRectangle(point3D a = point3D(), point3D b = point3D(), point3D c = point3D(),
                   point3D d = point3D());
  sphericRectangle(point3D mid, vector3D lp, vector3D nor, float ra, float ri, float dp);
  sphericRectangle(point3D mid, vector3D lp, vector3D hp, float ra, float ri);
  sphericRectangle(const shape_parameter& description);
  ~sphericRectangle();
  virtual int getNumberOfPoints() const;
  virtual point3D getPoint(int num) const;
  void setPoint(int num, point3D p);
  void setPoints(point3D a, point3D b, point3D c, point3D d);
  void setPoints(point3D mid, vector3D lp, vector3D nor, float ra, float ri, float dp);
  point3D A() const;
  point3D B() const;
  point3D C() const;
  point3D D() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};
#endif
