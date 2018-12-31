#ifndef FIBER
#define FIBER
#include "volumeShape.h"
class fiber : public volumeShape
{
protected:
  point3D corner;
  vector3D dir1;
  vector3D dir2;
  vector3D dir3;
  int numberOfHalvedElements;
  int halvedAt;

public:
  fiber(point3D cor = point3D(0, 0, 0), vector3D d1 = vector3D(1, 0, 0),
        vector3D d2 = vector3D(0, 1, 0), vector3D d3 = vector3D(0, 0, 1), int halved = 0,
        int halvedat = 0);
  fiber(const volumeShape& s);
  fiber(const fiber& s);
  fiber(const shape_parameter& description);
  virtual point3D getCorner() const;
  virtual vector3D getDirection(int num) const;
  void setCorner(point3D p);
  void setDirection(int num, vector3D v);
  int getHalved() const;
  void setHalved(int num);
  int getHalvedAt() const;
  void setHalvedAt(int num);
  virtual volumeShape* getNext(int times, int stackType);
  virtual volumeShape* getEnvelope(int times, int stackType);
  virtual vector3D distance(const sLine3D& line);
  virtual sLine3D Normal(const sLine3D& line);
  virtual Vector Hitting(const sLine3D& line);
  virtual bool cut(const fiber& f);
  virtual void operator=(const volumeShape& s);
  virtual volumeShape* getClone();
  virtual Vector HitParams(const sLine3D& line);
  virtual Vector HitParams(const planeShape& shape, point3D origin);
  virtual point3D entrance(const sLine3D& line);
  virtual float getFlightPathInShape(const sLine3D& line);
  virtual int suspect(const sLine3D& line, int stackType);
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
