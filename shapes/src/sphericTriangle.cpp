#include "planarShapes.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {
// 171f0f58-11ed-11e9-ab14-d663bd873d93
const boost::uuids::uuid sphericTriangle_id = {{0x17, 0x1f, 0x0f, 0x58, 0x11, 0xed, 0x11, 0xe9,
                                                0xab, 0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct SphericTriangleShapeProvider
    : public FactoryShapeProvider,
      public std::enable_shared_from_this<SphericTriangleShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        sphericTriangle::getDescription(), ShapeType::PlanarShape,
        [](shape_parameter const& param) -> std::shared_ptr<planeShape> {
          if (!checkParameter(param)) {
            return {};
          }
          return std::shared_ptr<planeShape>(new sphericTriangle(
              param.value(0).value<point3D>(), param.value(1).value<point3D>(),
              param.value(2).value<point3D>(), param.value(3).value<point3D>()));
        },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(sphericTriangle_id);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
  static bool checkParameter(shape_parameter const& param)
  {
    if (param.numberOfValues() != 8) {
      return false;
    }
    return !(param.value(0).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(1).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(2).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(3).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(4).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(5).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(6).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(7).valueType() != ParameterValue::ValueType::INT);
  }
};
volatile static std::shared_ptr<SphericTriangleShapeProvider> prov = [] {
  auto r = std::make_shared<SphericTriangleShapeProvider>();
  r->install();
  return r;
}();
}

sphericTriangle::sphericTriangle(const planeShape& p) : planeShape("spheric triangle")
{
  center = p.getCenter();
  normal = p.getNormal();
  circRadius = -1;
}
sphericTriangle::sphericTriangle(point3D cen, point3D a, point3D b, point3D c, point3D d, point3D e,
                                 point3D f)
    : planeShape("spheric triangle")
{
  A_ = a;
  B_ = b;
  C_ = c;
  D_ = d;
  E_ = e;
  F_ = f;
  if ((d.getState() == _undefined_) && (e.getState() == _undefined_) &&
      (f.getState() == _undefined_))
    realTriangle = true;
  else
    realTriangle = false;
  center = cen;
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  if (realTriangle) {
    vector3D v1(a - center), v2(b - center), v3(c - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

sphericTriangle::sphericTriangle(const shape_parameter& description)
    : planeShape("spheric triangle")
{
  if (description.getName() != "sphericTriangle")
    return;
  if (!SphericTriangleShapeProvider::checkParameter(description))
    return;
  A_ = description.value(1).value<point3D>();
  B_ = description.value(2).value<point3D>();
  C_ = description.value(3).value<point3D>();
  D_ = description.value(4).value<point3D>();
  E_ = description.value(5).value<point3D>();
  F_ = description.value(6).value<point3D>();
  if (description.value(7).value<int>() == 3)
    realTriangle = true;
  else
    realTriangle = false;
  center = description.value(0).value<point3D>();
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}
sphericTriangle::~sphericTriangle() {}

point3D sphericTriangle::getPoint(int num) const
{
  switch (num) {
  case 0:
    return A_;
  case 1:
    return B_;
  case 2:
    return C_;
  case 3:
    return D_;
  case 4:
    return E_;
  case 5:
    return F_;
  }
  return point3D();
}

void sphericTriangle::setPoint(int num, point3D p)
{
  switch (num) {
  case 0:
    A_ = p;
    break;
  case 1:
    B_ = p;
    break;
  case 2:
    C_ = p;
    break;
  case 3:
    D_ = p;
    break;
  case 4:
    E_ = p;
    break;
  case 5:
    F_ = p;
    break;
  }
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setPoints(point3D cen, point3D a, point3D b, point3D c, point3D d, point3D e,
                                point3D f)
{
  A_ = a;
  B_ = b;
  C_ = c;
  D_ = d;
  E_ = e;
  F_ = f;
  if ((d.getState() == _undefined_) && (e.getState() == _undefined_) &&
      (f.getState() == _undefined_))
    realTriangle = true;
  else
    realTriangle = false;
  center = cen;
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  if (realTriangle) {
    vector3D v1(a - center), v2(b - center), v3(c - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

point3D sphericTriangle::A() const { return A_; }

point3D sphericTriangle::B() const { return B_; }

point3D sphericTriangle::C() const { return C_; }

point3D sphericTriangle::D() const { return D_; }

point3D sphericTriangle::E() const { return E_; }

point3D sphericTriangle::F() const { return F_; }

void sphericTriangle::setA(point3D p)
{
  A_ = p;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setB(point3D p)
{
  B_ = p;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setC(point3D p)
{
  C_ = p;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setD(point3D p)
{
  D_ = p;
  if (D_.getState() != _undefined_) {
    realTriangle = false;
    return;
  }
  if ((E_.getState() == _undefined_) || (F_.getState() == _undefined_))
    realTriangle = true;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setE(point3D p)
{
  E_ = p;
  if (E_.getState() != _undefined_) {
    realTriangle = false;
    return;
  }
  if ((D_.getState() == _undefined_) || (F_.getState() == _undefined_))
    realTriangle = true;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

void sphericTriangle::setF(point3D p)
{
  F_ = p;
  if (F_.getState() != _undefined_) {
    realTriangle = false;
    return;
  }
  if ((E_.getState() == _undefined_) || (D_.getState() == _undefined_))
    realTriangle = true;
  if (realTriangle) {
    vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
    circRadius = v1.R();
    if (v2.R() > circRadius)
      circRadius = v2.R();
    if (v3.R() > circRadius)
      circRadius = v3.R();
  } else {
    vector3D v[6];
    v[0] = A_;
    v[1] = B_;
    v[2] = C_;
    v[3] = D_;
    v[4] = E_;
    v[5] = F_;
    for (int i = 0; i < 6; i++)
      v[i] = v[i] - center;
    circRadius = v[0].R();
    for (int i = 1; i < 6; i++)
      if (v[i].R() > circRadius)
        circRadius = v[i].R();
  }
}

float sphericTriangle::area() const { return -1; }

bool sphericTriangle::isRealTriangle() { return realTriangle; }

triangle sphericTriangle::getTriangle() const { return triangle(A_, B_, C_); }

void sphericTriangle::operator=(const sphericTriangle& t)
{
  setPoints(t.getCenter(), t.A(), t.B(), t.C(), t.D(), t.E(), t.F());
}
float sphericTriangle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  //  cout<<A()<<B()<<C()<<D()<<E()<<F()<<endl;
  vector3D v[6];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin, v[4] = E() - origin, v[5] = F() - origin;
  int nn = (realTriangle ? 3 : 6);
  vector3D vc = center - origin;
  vector3D n[nn], nc;
  nc = vc - zDirection * (vc * zDirection);
  nc.normalize();
  float t[nn];
  for (int i = 0; i < nn; i++) {
    v[i].normalize();
    n[i] = v[i] - zDirection * (v[i] * zDirection);
    n[i].normalize();
    t[i] = n[i] * nc;
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < nn; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
float sphericTriangle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[6];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin, v[4] = E() - origin, v[5] = F() - origin;
  vector3D vc = center - origin;
  int nn = (realTriangle ? 3 : 6);
  float t[nn];
  for (int i = 0; i < nn; i++) {
    v[i].normalize();
    t[i] = acos(v[i] * zDirection);
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < nn; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
vector3D sphericTriangle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v[(realTriangle ? 3 : 6)];
  v[0] = p & A();
  v[1] = p & B();
  v[2] = p & C();
  if (!realTriangle) {
    v[3] = p & D();
    v[4] = p & E();
    v[5] = p & F();
  }
  for (int i = 0; i < (realTriangle ? 3 : 6); i++)
    for (int j = i + 1; j < (realTriangle ? 3 : 6); j++)
      if (v[i] * v[j] < 0)
        return vector3D(0, 0, 0);
  int m = 0;
  for (int i = 1; i < (realTriangle ? 3 : 6); i++)
    if (v[i].length() < v[m].length())
      m = i;
  return v[m];
}
int sphericTriangle::getNumberOfPoints() const
{
  if (realTriangle)
    return 3;
  return 6;
}

vector3D sphericTriangle::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector3D aa = A_ - hit, bb = B_ - hit, cc = C_ - hit;
  float a = (cc * bb) / cc.R() / bb.R(), b = (aa * cc) / aa.R() / cc.R(),
        c = (aa * bb) / aa.R() / bb.R();
  if ((acos(a) + acos(b) + acos(c) - M_PI * 2.) > -0.01)
    return vector3D(0, 0, 0);
  if ((acos(a) + acos(b) + acos(c) - M_PI) < 0) {
    if (aa.R() < bb.R() && aa.R() < cc.R())
      return aa;
    if (bb.R() < cc.R())
      return bb;
    return cc;
  }
  if (a > b && a > c)
    return (bb + cc) * 0.5;
  if (b > c)
    return (aa + cc) * 0.5;
  return (aa + bb) * 0.5;
}
shape_parameter sphericTriangle::description() const
{
  shape_parameter sh;
  sh.setName("sphericTriangle");
  sh.setCompleteWrite(true);
  sh.setId(sphericTriangle_id);
  sh.addValue("center", center);
  sh.addValue("A", A_);
  sh.addValue("B", B_);
  sh.addValue("C", C_);
  sh.addValue("D", D_);
  sh.addValue("E", E_);
  sh.addValue("F", F_);
  sh.addValue("number of points", static_cast<int>(realTriangle ? 3 : 6));
  return sh;
}
shape_parameter sphericTriangle::getDescription()
{
  shape_parameter sh;
  sh.setName("sphericTriangle");
  sh.setId(sphericTriangle_id);
  sh.addValue("center", point3D());
  sh.addValue("A", point3D());
  sh.addValue("B", point3D());
  sh.addValue("C", point3D());
  sh.addValue("D", point3D());
  sh.addValue("E", point3D());
  sh.addValue("F", point3D());
  sh.addValue("number of points", static_cast<int>(3));
  sh.setCompleteWrite(false);
  return sh;
}
