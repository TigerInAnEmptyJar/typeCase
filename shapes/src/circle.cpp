#include "circle.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace Shape {

const boost::uuids::uuid circle_id = {{0x14, 0x8b, 0xec, 0xc6, 0x11, 0xbf, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct CircleShapeProvider : public FactoryShapeProvider,
                             public std::enable_shared_from_this<CircleShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        circle::getDescription(), ShapeType::PlanarShape,
        [](shape_parameter const& param) -> std::shared_ptr<planeShape> {
          if (!CircleShapeProvider::checkParameter(param)) {
            return {};
          }
          return std::shared_ptr<planeShape>(new circle(param.value(0).value<point3D>(),
                                                        param.value(1).value<vector3D>(),
                                                        param.value(2).value<float>()));
        },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(Shape::circle_id);
  }
  void install() { innerShapeProviders.push_back(shared_from_this()); }
  static bool checkParameter(shape_parameter const& param)
  {
    if (param.numberOfValues() != 3) {
      return false;
    }
    return !(param.value(0).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(1).valueType() != ParameterValue::ValueType::VECTOR3D ||
             param.value(2).valueType() != ParameterValue::ValueType::FLOAT);
  }
};
auto prov __attribute__((__used__)) = [] {
  auto r = std::make_shared<CircleShapeProvider>();
  r->install();
  return r;
}();
}

circle::circle(point3D cen, vector3D nor, float rIn) : planeShape("circle")
{
  center = cen;
  normal = nor;
  rad = rIn;
  circRadius = rIn;
}
circle::circle(const shape_parameter& description) : planeShape("circle")
{
  if (description.getName() != "circle")
    return;
  if (!Shape::CircleShapeProvider::checkParameter(description)) {
    return;
  }
  center = description.value(0).value<point3D>();
  normal = description.value(1).value<vector3D>();
  rad = description.value(2).value<float>();
  circRadius = rad;
}
circle::~circle() {}
int circle::getNumberOfPoints() const { return 0; }
point3D circle::getPoint(int num) const
{
  if (num == 0)
    return center;
  return point3D();
}
float circle::area() const { return 2 * rad * M_PI; }
float circle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  float dist = ((origin - center) * zDirection);
  float xy = ((origin - center) - (zDirection * dist)).length();
  return atan((xy - rad) / dist) - atan((xy + rad) / dist);
}

float circle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  float dist = ((origin - center) * zDirection);
  float xy = ((origin - center) - (zDirection * dist)).length();
  return atan((xy - rad) / dist) - atan((xy + rad) / dist);
}
float circle::radius() const { return rad; }
void circle::setRadius(float rIn) { rad = rIn; }

vector3D circle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v = p & center;
  float m = v * normal;
  vector3D vv = normal * m;
  vector3D v2 = v - vv;
  if (v2.length() < rad)
    return vector3D(0, 0, 0);
  return v - (v * ((v2 * v) / v.length()));
}
vector3D circle::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector3D l = hit - center;
  if (l.R() <= rad)
    return vector3D(0, 0, 0);
  return l * (l.R() - rad);
}

shape_parameter circle::description() const
{
  shape_parameter sh;
  sh.setName("circle");
  sh.setId(Shape::circle_id);
  sh.setCompleteWrite(true);
  sh.addValue("center", center);
  sh.addValue("normal", normal);
  sh.addValue("radius", rad);
  return sh;
}
shape_parameter circle::getDescription()
{
  shape_parameter sh;
  sh.setName("circle");
  sh.setId(Shape::circle_id);
  sh.addValue("center", point3D{});
  sh.addValue("normal", vector3D{});
  sh.addValue("radius", static_cast<float>(0));
  return sh;
}
