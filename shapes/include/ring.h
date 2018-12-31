#ifndef RING
#define RING
#include "volumeShape.h"

class ring : public volumeShape
{
private:
  vector3D normal;
  float innerR;
  float outerR;
  float thickness;

public:
  ring(point3D centerIn = point3D(0, 0, 0), vector3D normalIn = vector3D(0, 0, 1),
       float innerRadius = 0, float outerRadius = 100, float thicknessIn = 1);
  ring(const volumeShape& s);
  ring(const shape_parameter& description);
  virtual ~ring();
  virtual vector3D getNormal() const;
  virtual float getInnerRadius() const;
  virtual float getOuterRadius() const;
  virtual float getThickness() const;
  virtual volumeShape* getClone();
  virtual volumeShape* getNext(int times, int stackType);
  virtual volumeShape* getEnvelope(int times, int stackType);
  virtual Vector Hitting(const sLine3D& line);
  virtual point3D entrance(const sLine3D& line);
  virtual vector3D distance(const sLine3D& line);
  virtual sLine3D Normal(const sLine3D& line);
  virtual bool cut(const fiber& f);
  virtual Vector HitParams(const sLine3D& line);
  virtual Vector HitParams(const planeShape& shape, point3D origin);
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
