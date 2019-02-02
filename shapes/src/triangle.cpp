#include "planarShapes.h"
#include "shapeparameter.h"

#include "provideShapes.h"

namespace {
// f2040ab6-143f-11e9-ab14-d663bd873d93
const boost::uuids::uuid triangle_id{{0xf2, 0x04, 0x0a, 0xb6, 0x14, 0x3f, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct ShapeProvider : public FactoryShapeProvider,
                       public std::enable_shared_from_this<ShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        triangle::getDescription(), ShapeType::PlanarShape,
        [](shape_parameter const& param) -> std::shared_ptr<planeShape> {
          return std::shared_ptr<planeShape>(new triangle(
              param.getParam<point3D>(0), param.getParam<point3D>(1), param.getParam<point3D>(2)));
        },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(triangle_id);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
};
std::shared_ptr<ShapeProvider> prov = [] {
  auto r = std::make_shared<ShapeProvider>();
  r->install();
  return r;
}();
}

triangle::triangle(const planeShape& p) : planeShape("triangle")
{
  center = p.getCenter();
  normal = p.getNormal();
  circRadius = -1;
}

triangle::triangle(point3D a, point3D b, point3D c) : planeShape("triangle")
{
  A_ = a;
  B_ = b;
  C_ = c;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (a - b) ^ (b - c);
  normal.normalize();
  vector3D v1(a - center), v2(b - center), v3(c - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

triangle::triangle(point3D a, vector3D v1, vector3D v2) : planeShape("triangle")
{
  A_ = a;
  B_ = a + v1;
  C_ = a + v2;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v_1(A_ - center), v_2(B_ - center), v_3(C_ - center);
  circRadius = v_1.R();
  if (v_2.R() > circRadius)
    circRadius = v_2.R();
  if (v_3.R() > circRadius)
    circRadius = v_3.R();
}
triangle::triangle(const shape_parameter& description) : planeShape("triangle")
{
  if (description.getName() != "triangle")
    return;
  if (description.NumberOfParams<point3D>() < 3)
    return;
  A_ = description.getParam<point3D>(0);
  B_ = description.getParam<point3D>(1);
  C_ = description.getParam<point3D>(2);
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v_1(A_ - center), v_2(B_ - center), v_3(C_ - center);
  circRadius = v_1.R();
  if (v_2.R() > circRadius)
    circRadius = v_2.R();
  if (v_3.R() > circRadius)
    circRadius = v_3.R();
}
float triangle::area() const
{
  float base = (B_ - A_).length();
  vector3D AC = C_ - A_;
  vector3D AB = B_ - A_;
  AC.normalize();
  AB.normalize();
  float cosAlpha = AC * AB;
  float height = ((C_ - A_) * sin(acos(cosAlpha))).length();
  return 0.5 * base * height;
}

// plane3D planeShape::getPlane()const
//{
//  return plane3D();//getCenter(),getNormal());
//}

// void planeShape::setPlane(const plane3D &p)
//{
//  setCenter(p.Foot());
// normal=p.Normal();
//}

triangle::~triangle() {}

point3D triangle::getPoint(int num) const
{
  switch (num) {
  case 0:
    return A_;
  case 1:
    return B_;
  case 2:
    return C_;
  }
  return point3D();
}

void triangle::setPoint(int num, point3D p)
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
  }
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

void triangle::setPoints(point3D a, point3D b, point3D c)
{
  A_ = a;
  B_ = b;
  C_ = c;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (a - b) ^ (b - c);
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

point3D triangle::A() const { return A_; }

point3D triangle::B() const { return B_; }

point3D triangle::C() const { return C_; }

void triangle::setA(point3D p)
{
  A_ = p;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

void triangle::setB(point3D p)
{
  B_ = p;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

void triangle::setC(point3D p)
{
  C_ = p;
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = (A_ - B_) ^ (B_ - C_);
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

float triangle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v1 = A() - origin, v2 = B() - origin, v3 = C() - origin;
  vector3D vc = center - origin;
  vector3D n1 = v1 - zDirection * (v1 * zDirection);
  vector3D n2 = v2 - zDirection * (v2 * zDirection);
  vector3D n3 = v3 - zDirection * (v3 * zDirection);
  vector3D nc = vc - zDirection * (vc * zDirection);
  n1.normalize();
  n2.normalize();
  n3.normalize();
  nc.normalize();
  float t1 = acos(n1 * nc), t2 = acos(n2 * nc), t3 = acos(n3 * nc);
  float min = t1, max = t1;
  if (min > t2)
    min = t2;
  if (min > t3)
    min = t3;
  if (max < t2)
    max = t2;
  if (max < t3)
    max = t3;
  return max - min;
}
float triangle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v1 = A() - origin, v2 = B() - origin, v3 = C() - origin;
  v1.normalize();
  v2.normalize();
  v3.normalize();
  float t1 = acos(v1 * zDirection), t2 = acos(v2 * zDirection), t3 = acos(v3 * zDirection);
  float min = t1, max = t1;
  if (min > t2)
    min = t2;
  if (min > t3)
    min = t3;
  if (max < t2)
    max = t2;
  if (max < t3)
    max = t3;
  return max - min;
}

void triangle::operator=(const triangle& t)
{
  A_ = t.A();
  B_ = t.B();
  C_ = t.C();
  center = t.getCenter();
  normal = t.getNormal();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}

void triangle::operator=(const sphericTriangle& t)
{
  A_ = t.A();
  B_ = t.B();
  C_ = t.C();
  point3D H = A_ + ((B_ - A_) * 0.5);
  center = H + ((C_ - H) * (1 / 3.0));
  normal = t.getNormal();
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
}
vector3D triangle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v1 = p & A(), v2 = p & B(), v3 = p & C();
  if (v1 * v2 < 0)
    return vector3D(0, 0, 0);
  if (v2 * v3 < 0)
    return vector3D(0, 0, 0);
  if (v3 * v1 < 0)
    return vector3D(0, 0, 0);
  if (v1.length() < v2.length() && v1.length() < v3.length())
    return v1;
  if (v2.length() < v3.length() && v1.length() > v2.length())
    return v2;
  return v3;
}
vector3D triangle::distance(const sLine3D& line)
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
// #include <TClass.h>
// void triangle::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	triangle::Class()->WriteBuffer(b, this);
// 	b<<A_.X()<<A_.Y()<<A_.Z();
// 	b<<B_.X()<<B_.Y()<<B_.Z();
// 	b<<C_.X()<<C_.Y()<<C_.Z();
//     }
//     else
//     {
// 	triangle::Class()->ReadBuffer(b, this);
// 	float f1,f2,f3,f4,f5,f6,f7,f8,f9;
// 	b>>f1>>f2>>f3>>f4>>f5>>f6>>f7>>f8>>f9;
// 	A_.setValues(f1,f2,f3);
// 	B_.setValues(f4,f5,f6);
// 	C_.setValues(f7,f8,f9);
//     }
// }
int triangle::getNumberOfPoints() const { return 3; }
//#include <TPolyLine.h>
extern void DrawPoints(int nPoints, point3D* points, const point3D& eye, const plane3D& plane,
                       vector4D* boundingBox, TObject** ident, int lColor, int fColor, int fStyle);
void triangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                    TObject** ident, int lColor, int fColor, int fStyle) const
{
  point3D ps[3] = {getPoint(0), getPoint(1), getPoint(2)};
  DrawPoints(3, ps, eye, plane, boundingBox, ident, lColor, fColor, fStyle);
}
void triangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
shape_parameter triangle::description() const
{
  shape_parameter sh;
  sh.setName("triangle");
  sh.setId(triangle_id);
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(A_, "A");
  sh.addParam<point3D>(B_, "B");
  sh.addParam<point3D>(C_, "C");
  return sh;
}
shape_parameter triangle::getDescription()
{
  shape_parameter sh;
  sh.setName("triangle");
  sh.setId(triangle_id);
  sh.addParam<point3D>(point3D(), "A");
  sh.addParam<point3D>(point3D(), "B");
  sh.addParam<point3D>(point3D(), "C");
  sh.setCompleteWrite(false);
  return sh;
}
