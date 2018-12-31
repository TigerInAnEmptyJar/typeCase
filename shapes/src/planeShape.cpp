#include "planeShape.h"
#include "shapeparameter.h"
plane3D planeShape::getPlane() const { return plane3D(center, normal); }
void planeShape::setPlane(plane3D const& p)
{
  center = p.Foot();
  normal = p.Normal();
}
planeShape::planeShape(const shape_parameter& description) : base_shape(description.getName())
{
  setCenter(point3D(0, 0, 0));
  normal = vector3D(0, 0, 1);
}
shape_parameter planeShape::description() const
{
  shape_parameter sh;
  sh.setName(getName());
  return sh;
}
shape_parameter planeShape::getDescription()
{
  shape_parameter sh;
  sh.setName("planeshape");
  return sh;
}
float planeShape::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  if (origin[0] == zDirection[0])
    return 0;
  return 0;
}
float planeShape::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  if (origin[0] == zDirection[0])
    return 0;
  return 0;
}
point3D planeShape::getPoint(int num) const
{
  if (num == 0)
    return point3D();
  return point3D();
}
vector3D planeShape::distancePlane(const plane3D& p) { return vector3D(p.Foot()); }
int planeShape::getNumberOfPoints() const { return 0; }
vector3D planeShape::distance(const sLine3D& line) { return vector3D(line.Direction()); }
float planeShape::circumscribedRadius() const { return circRadius; }
