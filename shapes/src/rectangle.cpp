#include "planeShape.h"
#include "shapeparameter.h"
rectangle::rectangle(const planeShape& p) : planeShape("rectangle")
{
  center = p.getCenter();
  normal = p.getNormal();
  circRadius = -1;
}
rectangle::rectangle(point3D Ce, vector3D N, point3D A, point3D B, point3D C, point3D D)
    : planeShape("rectangle")
{
  center = Ce;
  normal = N;
  normal.normalize();
  plane3D p(center, normal);
  left.newLine(A - (p & A), B - (p & B));
  right.newLine(D - (p & D), C - (p & C));
}
rectangle::rectangle(point3D A, point3D B, vector3D norm, float width) : planeShape("rectangle")
{
  left.newLine(A, B);
  vector3D ADv = norm ^ (A - B);
  ADv.normalize();
  right.newLine(A + ADv * width, B + ADv * width);
  center = A + (B - A) * 0.5 + ((A + (ADv * width)) - A) * 0.5;
  normal = norm;
  normal.normalize();
  vector3D v1(A - center), v2(B - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}
rectangle::rectangle(const shape_parameter& description) : planeShape("rectangle")
{
  if (description.getName() != "rectangle")
    return;
  if (description.NumberOfParams<point3D>() < 4)
    return;
  left.newLine(description.getParam<point3D>(0), description.getParam<point3D>(1));
  right.newLine(description.getParam<point3D>(2), description.getParam<point3D>(3));
  center = left.Mid() + (right.Mid() - left.Mid()) * 0.5;
  normal = (description.getParam<point3D>(0) - description.getParam<point3D>(1)) ^
           (description.getParam<point3D>(0) - description.getParam<point3D>(2));
  normal.normalize();
  vector3D v1(description.getParam<point3D>(0) - center),
      v2(description.getParam<point3D>(1) - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}
void rectangle::setPoints(point3D A, point3D B, vector3D norm, float width)
{
  left.newLine(A, B);
  vector3D ADv = norm ^ (A - B);
  ADv.normalize();
  right.newLine(A + ADv * width, B + ADv * width);
  center = A + (B - A) * 0.5 + ((A + (ADv * width)) - A) * 0.5;
  normal = norm;
  normal.normalize();
  vector3D v1(A - center), v2(B - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}

rectangle::~rectangle() {}

point3D rectangle::getPoint(int num) const
{
  switch (num) {
  case 0:
    return left.P();
  case 1:
    return left.Q();
  case 2:
    return right.Q();
  case 3:
    return right.P();
  }
  return point3D();
}

point3D rectangle::A() const { return left.P(); }

point3D rectangle::B() const { return left.Q(); }

point3D rectangle::C() const { return right.Q(); }

point3D rectangle::D() const { return right.P(); }

float rectangle::area() const { return left.length() * (left.P() - right.P()).length(); }

float rectangle::getWidth() const { return left.length(); }

float rectangle::getLength() const { return (left.P() - right.P()).length(); }

lLine3D rectangle::AB() const { return left; }

lLine3D rectangle::BC() const { return lLine3D(left.Q(), right.Q()); }

lLine3D rectangle::CD() const { return lLine3D(right.Q(), right.P()); }

lLine3D rectangle::DA() const { return lLine3D(right.P(), left.P()); }

void rectangle::operator=(const rectangle& r)
{
  left.newLine(r.A(), r.B());
  right.newLine(r.D(), r.C());
}
float rectangle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  vector3D n[4], nc;
  nc = vc - zDirection * (vc * zDirection);
  nc.normalize();
  float t[4];
  for (int i = 0; i < 4; i++) {
    v[i].normalize();
    n[i] = v[i] - zDirection * (v[i] * zDirection);
    n[i].normalize();
    t[i] = n[i] * nc;
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
float rectangle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  float t[4];
  for (int i = 0; i < 4; i++) {
    v[i].normalize();
    t[i] = acos(v[i] * zDirection);
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
vector3D rectangle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v[4];
  v[0] = p & A();
  v[1] = p & B();
  v[2] = p & C();
  v[3] = p & D();
  for (int i = 0; i < 4; i++)
    for (int j = i + 1; j < 4; j++)
      if (v[i] * v[j] < 0)
        return vector3D(0, 0, 0);
  int m = 0;
  for (int i = 1; i < 4; i++)
    if (v[i].length() < v[m].length())
      m = i;
  return v[m];
}
int rectangle::getNumberOfPoints() const { return 4; }
extern void DrawPoints(int nPoints, point3D* points, const point3D& eye, const plane3D& plane,
                       vector4D* boundingBox, TObject** ident, int lColor, int fColor, int fStyle);
void rectangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                     TObject** ident, int lColor, int fColor, int fStyle) const
{
  point3D ps[4] = {getPoint(0), getPoint(1), getPoint(2), getPoint(3)};
  DrawPoints(4, ps, eye, plane, boundingBox, ident, lColor, fColor, fStyle);
}
void rectangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                     int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
vector3D rectangle::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector3D aa = A() - hit, bb = B() - hit, cc = C() - hit, dd = D() - hit;
  float a = acos((aa * bb) / aa.R() / bb.R()), b = acos((bb * cc) / bb.R() / cc.R()),
        c = acos((cc * dd) / cc.R() / dd.R()), d = acos((aa * dd) / aa.R() / dd.R());
  if ((a + b + c + d - M_PI * 2.) > -0.01)
    return vector3D(0, 0, 0);
  if ((a + b + c + d - M_PI) < 0) {
    if (aa.R() < bb.R() && aa.R() < cc.R() && aa.R() < dd.R())
      return aa;
    if (bb.R() < cc.R() && bb.R() < dd.R())
      return bb;
    if (cc.R() < dd.R())
      return cc;
    return dd;
  }
  if (a > b && a > c && a > d)
    return (bb + aa) * 0.5;
  if (b > c && b > d)
    return (bb + cc) * 0.5;
  if (c > d)
    return (cc + dd) * 0.5;
  return (dd + aa) * 0.5;
}
shape_parameter rectangle::description() const
{
  shape_parameter sh;
  sh.setName("rectangle");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(left.P(), "A");
  sh.addParam<point3D>(left.Q(), "B");
  sh.addParam<point3D>(right.Q(), "C");
  sh.addParam<point3D>(right.P(), "D");
  return sh;
}
shape_parameter rectangle::getDescription()
{
  shape_parameter sh;
  sh.setName("rectangle");
  sh.addParam<point3D>(point3D(), "A");
  sh.addParam<point3D>(point3D(), "B");
  sh.addParam<point3D>(point3D(), "C");
  sh.addParam<point3D>(point3D(), "D");
  sh.setCompleteWrite(false);
  return sh;
}
