#include "baseShape.h"
#include "shapeparameter.h"

vector3D base_shape::distance(const sLine3D& line) { return vector3D(line.Foot()); }
void base_shape::setName(string nme) { fname = nme; }

sLine3D base_shape::Normal(const sLine3D& line) { return line; }

Vector base_shape::HitParams(const sLine3D& line) { return Vector(line.Foot()); }
#include "fiber.h"
bool base_shape::cut(const fiber& f)
{
  if (f.getCorner().getState() == _undefined_)
    return false;
  return false;
}
point3D base_shape::entrance(const sLine3D& line) { return point3D(line.Foot()); }
base_shape::base_shape(string n)
{
  center.setValues(0, 0, 0);
  setName(n);
}

base_shape::base_shape(const base_shape& s)
{
  setName(s.getName());
  center = s.getCenter();
}

void base_shape::setCenter(point3D p) { center = p; }

string base_shape::getName() const { return fname; }

point3D base_shape::getCenter() const { return center; }
void base_shape::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                      int fColor, int fStyle) const
{
  if (eye == plane.Foot())
    return;
  if (lColor == fColor && lColor == fStyle && boundingBox == NULL)
    return;
}
void base_shape::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                      TObject** ident, int lColor, int fColor, int fStyle) const
{
  if (eye == plane.Foot())
    return;
  if (lColor == fColor && lColor == fStyle && boundingBox == NULL)
    return;
  if (ident == NULL)
    return;
}
shape_parameter base_shape::description() const
{
  shape_parameter sh;
  sh.setName(fname);
  return sh;
}
shape_parameter base_shape::getDescription()
{
  shape_parameter sh;
  sh.setName("shape");
  return sh;
}
