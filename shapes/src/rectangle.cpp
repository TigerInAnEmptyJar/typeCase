#include "planarShapes.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {
// 389e9e10-11ec-11e9-ab14-d663bd873d93
const boost::uuids::uuid rectangle_id = {{0x38, 0x9e, 0x9e, 0x10, 0x11, 0xec, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct RectangleShapeProvider : public FactoryShapeProvider,
                                public std::enable_shared_from_this<RectangleShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        rectangle::getDescription(), ShapeType::PlanarShape,
        [](shape_parameter const& param) -> std::shared_ptr<planeShape> {
          if (!checkParameter(param)) {
            return {};
          }
          return std::shared_ptr<planeShape>(new rectangle(param));
        },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(rectangle_id);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
  static bool checkParameter(shape_parameter const& param)
  {
    if (param.numberOfValues() != 4) {
      return false;
    }
    return !(param.value(0).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(1).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(2).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(3).valueType() != ParameterValue::ValueType::POINT3D);
  }
};
auto prov __attribute__((__used__)) = [] {
  auto r = std::make_shared<RectangleShapeProvider>();
  r->install();
  return r;
}();
}

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
  if (!RectangleShapeProvider::checkParameter(description))
    return;
  left.newLine(description.value(0).value<point3D>(), description.value(1).value<point3D>());
  right.newLine(description.value(3).value<point3D>(), description.value(2).value<point3D>());
  center = left.Mid() + (right.Mid() - left.Mid()) * 0.5;
  normal = (description.value(0).value<point3D>() - description.value(3).value<point3D>()) ^
           (description.value(0).value<point3D>() - description.value(1).value<point3D>());
  normal.normalize();
  vector3D v1(description.value(0).value<point3D>() - center),
      v2(description.value(1).value<point3D>() - center);
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
  sh.setId(rectangle_id);
  sh.setCompleteWrite(true);
  sh.addValue("A", left.P());
  sh.addValue("B", left.Q());
  sh.addValue("C", right.Q());
  sh.addValue("D", right.P());
  return sh;
}
shape_parameter rectangle::getDescription()
{
  shape_parameter sh;
  sh.setName("rectangle");
  sh.setId(rectangle_id);
  sh.addValue("A", point3D());
  sh.addValue("B", point3D());
  sh.addValue("C", point3D());
  sh.addValue("D", point3D());
  sh.setCompleteWrite(false);
  return sh;
}
