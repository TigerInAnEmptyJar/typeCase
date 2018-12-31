#ifndef BASESHAPE
#define BASESHAPE
#include "geometry.h"
//#include <shapeparameter.h>

class TObject;
class fiber;
class shape_parameter;
class volumeShape;
class base_shape //:public base_parameter
{
protected:
  string fname;
  point3D center;
  void setName(string nme);

public:
  base_shape(string n);
  base_shape(const base_shape& s);
  virtual ~base_shape() {}
  string getName() const;
  point3D getCenter() const;
  void setCenter(point3D c);
  virtual point3D entrance(const sLine3D& line);
  virtual vector3D distance(const sLine3D& line);
  virtual sLine3D Normal(const sLine3D& line);
  virtual bool cut(const fiber& f);
  virtual Vector HitParams(const sLine3D& line);
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
