#include "volumeShape.h"
#include "planeShape.h"
Vector volumeShape::HitParams(const sLine3D& line) { return Vector(line.Foot().getDimension()); }
Vector volumeShape::HitParams(const planeShape& shape, point3D origin)
{
  if (shape.getCenter() == origin)
    return Vector(0);
  return Vector(0);
}

volumeShape::volumeShape(string n) : base_shape(n), res(1, 0, 0) {}

volumeShape::volumeShape(const volumeShape& s) : base_shape(s.getName()), res(1, 0, 0)
{
  setName(s.getName());
  setCenter(s.getCenter());
  setMaxDistance(s.getMaxDistance());
}

volumeShape::~volumeShape() {}

float volumeShape::getMaxDistance() const { return maxDist; }

void volumeShape::setMaxDistance(float value) { maxDist = value; }

volumeShape* volumeShape::getClone()
{
  volumeShape* sh = new volumeShape(*this);
  return sh;
}

// void volumeShape::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	volumeShape::Class()->WriteBuffer(b, this);
//     }
//     else
//     {
// 	volumeShape::Class()->ReadBuffer(b, this);
//     }
// }

volumeShape* volumeShape::getNext(int times, int st)
{
  volumeShape* ret = new volumeShape(*this);
  if (times == st)
    return ret;
  return ret;
}
volumeShape* volumeShape::getEnvelope(int times, int st)
{
  volumeShape* ret = new volumeShape(*this);
  if (times == st)
    return ret;
  return ret;
}
Vector volumeShape::Hitting(const sLine3D& line)
{
  Vector ret(10);
  ret.setValue(0, line.Foot().x());
  return ret;
}
float volumeShape::getFlightPathInShape(const sLine3D& line)
{
  if (line.Foot().x() == 0)
    return 0;
  return 0;
}
int volumeShape::suspect(const sLine3D& line, int stackType)
{
  if (line.Foot().x() < stackType)
    return -1;
  return -1;
}
