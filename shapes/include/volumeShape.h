#ifndef VOLUMESHAPE
#define VOLUMESHAPE
#include "baseShape.h"
class planeShape;
class volumeShape : public base_shape
{
protected:
  vector3D res;

private:
  float maxDist;

public:
  volumeShape(string n = "undefined");
  volumeShape(const volumeShape& s);
  virtual ~volumeShape();
  float getMaxDistance() const;
  void setMaxDistance(float value);
  virtual float getFlightPathInShape(const sLine3D& line);
  virtual volumeShape* getClone();
  virtual Vector HitParams(const sLine3D& line);
  virtual Vector Hitting(const sLine3D& line);
  virtual Vector HitParams(const planeShape& shape, point3D origin);
  virtual volumeShape* getNext(int times, int stackType);
  virtual volumeShape* getEnvelope(int times, int stackType);
  virtual int suspect(const sLine3D& line, int stackType);
};
#endif
