#include "strawTube.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {
// 299f8546-11f6-11e9-ab14-d663bd873d93
const boost::uuids::uuid strawtube_id = {{0x29, 0x9f, 0x85, 0x46, 0x11, 0xf6, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct StrawtubeShapeProvider : public FactoryShapeProvider,
                                public std::enable_shared_from_this<StrawtubeShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        strawTube::getDescription(), ShapeType::VolumeShape,
        [](shape_parameter const& param) -> std::shared_ptr<volumeShape> {
          if (!checkParameter(param)) {
            return {};
          }
          return std::shared_ptr<volumeShape>(
              new strawTube(param.value(0).value<point3D>(), param.value(1).value<vector3D>(),
                            param.value(6).value<float>(), param.value(2).value<vector3D>(),
                            param.value(3).value<vector3D>(), param.value(6).value<float>(),
                            param.value(4).value<int>(), param.value(5).value<int>()));
        },
        [](shape_parameter const&, size_t) -> shape_parameter {
          //! \todo implement me
          return {};
        },
        [](shape_parameter const&, size_t) -> shape_parameter {
          //! \todo implement me
          return {};
        });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(strawtube_id);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
  static bool checkParameter(shape_parameter const& param)
  {
    if (param.numberOfValues() != 7) {
      return false;
    }
    return !(param.value(0).valueType() != ParameterValue::ValueType::POINT3D ||
             param.value(1).valueType() != ParameterValue::ValueType::VECTOR3D ||
             param.value(2).valueType() != ParameterValue::ValueType::VECTOR3D ||
             param.value(3).valueType() != ParameterValue::ValueType::VECTOR3D ||
             param.value(4).valueType() != ParameterValue::ValueType::INT ||
             param.value(5).valueType() != ParameterValue::ValueType::INT ||
             param.value(6).valueType() != ParameterValue::ValueType::FLOAT);
  }
};
std::shared_ptr<StrawtubeShapeProvider> prov __attribute__((__used__)) = [] {
  auto r = std::make_shared<StrawtubeShapeProvider>();
  r->install();
  return r;
}();
}

strawTube::strawTube() : cylinder()
{
  shift = vector3D(0, 1, 1) * sqrt(2.);
  stackingDirection = vector3D(1, 0, 0);
  isoChrone = 5;
  halvedAt = 0;
  nHalvedElements = 0;
}
strawTube::strawTube(const strawTube& tube)
    : cylinder(tube.getCenter(), tube.getDirection(), tube.getRadius())
{
  stackingDirection = tube.getStackingDirection();
  shift = tube.getShift();
  isoChrone = tube.getIsoChroneRadius();
  halvedAt = tube.getHalvedAt();
  nHalvedElements = tube.getNumberOfHalvedElements();
  setMaxDistance(tube.getMaxDistance());
}
strawTube::strawTube(point3D centerIn, vector3D lineDirection, float rad, vector3D stackDirection,
                     vector3D shiftDirection, float isoChroneR, int halv, int halvAt)
    : cylinder(centerIn, lineDirection, rad)
{
  stackingDirection = stackDirection;
  shift = shiftDirection;
  isoChrone = isoChroneR;
  halvedAt = halvAt;
  nHalvedElements = halv;
}
strawTube::strawTube(const shape_parameter& description)
{
  if (!StrawtubeShapeProvider::checkParameter(description)) {
    return;
  }
  center = description.value(0).value<point3D>();
  direction = description.value(1).value<vector3D>();
  stackingDirection = description.value(2).value<vector3D>();
  shift = description.value(3).value<vector3D>();
  radius = description.value(6).value<float>();
  halvedAt = description.value(5).value<int>();
  nHalvedElements = description.value(4).value<int>();
}
strawTube::~strawTube() {}

float strawTube::getIsoChroneRadius() const { return isoChrone; }
vector3D strawTube::getStackingDirection() const { return stackingDirection; }
vector3D strawTube::getShift() const { return shift; }
void strawTube::setIsoChroneRadius(float value)
{
  if (value > radius)
    isoChrone = radius;
  else if (value < 0)
    isoChrone = 0;
  else
    isoChrone = value;
}
void strawTube::setStackingDirection(vector3D value) { stackingDirection = value; }
void strawTube::setShift(vector3D value) { shift = value; }
void strawTube::setNumberOfHalvedElements(int value) { nHalvedElements = value; }
void strawTube::setHalvedAt(int value) { halvedAt = value; }
int strawTube::getNumberOfHalvedElements() const { return nHalvedElements; }
int strawTube::getHalvedAt() const { return halvedAt; }
float strawTube::getFlightPathInShape(const sLine3D& line)
{
  point3D enter = entrance(line);
  point3D exit = entrance(sLine3D(line.Foot() + line.Direction() * 1e4, line.Direction() * -1));
  return (exit - enter).R();
}
void strawTube::operator=(const volumeShape& s)
{
  if (s.getName() == "cylinder") {
    radius = ((cylinder)s).getRadius();
    direction = ((cylinder)s).getDirection();
  } else if (s.getName() == "strawTube") {
    radius = ((strawTube*)&s)->getRadius();
    direction = ((strawTube*)&s)->getDirection();
    isoChrone = ((strawTube*)&s)->getIsoChroneRadius();
    stackingDirection = ((strawTube*)&s)->getStackingDirection();
  }
  setMaxDistance(s.getMaxDistance());
}
volumeShape* strawTube::getClone()
{
  strawTube* ret = new strawTube(*this);
  //   ret->setRadius(radius);
  //   ret->setDirection(direction);
  //   ret->setMaxDistance(getMaxDistance());
  return ret;
}
volumeShape* strawTube::getNext(int times, int stackType)
{
  strawTube* ret = ((strawTube*)this->getClone());
  switch (stackType) {
  case 0:
    ret->setCenter(ret->getCenter() + ret->getStackingDirection() * times);
    break;
  case 1: {
    if (times < halvedAt)
      ret->setCenter(ret->getCenter() + ret->getStackingDirection() * times);
    else if (times > halvedAt + 2 * nHalvedElements)
      ret->setCenter(ret->getCenter() + ret->getStackingDirection() * (times - nHalvedElements));
    else if (times < halvedAt + nHalvedElements) {
      ret->setLine(ret->getCenter() + ret->getStackingDirection() * times +
                       ret->getDirection() * 0.5,
                   ret->getDirection() * 0.5);
    } else {
      ret->setLine(ret->getCenter() + ret->getStackingDirection() * (times - nHalvedElements) -
                       ret->getDirection() * 0.5,
                   ret->getDirection() * 0.5);
    }
    break;
  }
  case 2: {
    if (times < halvedAt)
      ret->setCenter(ret->getCenter() + ret->getStackingDirection() * (times / 2.) +
                     ret->getShift() * (times % 2));
    else if (times > halvedAt + 2 * nHalvedElements)
      ret->setCenter(ret->getCenter() +
                     ret->getStackingDirection() * ((times - nHalvedElements) / 2.) +
                     ret->getShift() * (times % 2));
    else if (times < halvedAt + nHalvedElements) {
      ret->setLine(ret->getCenter() + ret->getStackingDirection() * (times / 2.) +
                       ret->getDirection() * 0.5 + ret->getShift() * (times % 2),
                   ret->getDirection() * 0.5);
    } else {
      ret->setLine(ret->getCenter() +
                       ret->getStackingDirection() * ((times - nHalvedElements) / 2.) -
                       ret->getDirection() * 0.5 + ret->getShift() * (times % 2),
                   ret->getDirection() * 0.5);
    }
    break;
  }
  }
  return ret;
}
#include "fiber.h"
volumeShape* strawTube::getEnvelope(int times, int stackType)
{
  vector3D d1 = direction, d2 = stackingDirection, d3 = d1 ^ d2;
  d3.normalize();
  d3 = d3 * radius * 2.;
  fiber* ret = new fiber(center - d1 * 0.5 - d2 * .5, d1, d2, d3);
  switch (stackType) {
  case 0: {
    ret->setCorner(center - d1 * 0.5 - d2 * .5); //+ret->getStackingDirection()*times);break;
    ret->setDirection(0, d1);
    ret->setDirection(1, d2 * times);
    ret->setDirection(2, d3);
    break;
  }
  case 1: {
    ret->setCorner(center - d1 * 0.5 - d2 * .5); //+ret->getStackingDirection()*times);break;
    ret->setDirection(0, d1);
    ret->setDirection(2, d3);
    if (times < halvedAt + nHalvedElements)
      ret->setDirection(1, d2 * times);
    else
      ret->setDirection(1, d2 * (times - nHalvedElements));
    break;
  }
  case 2: {
    ret->setCorner(center - d1 * 0.5 - d2 * .5); //+ret->getStackingDirection()*times);break;
    ret->setDirection(0, d1);
    ret->setDirection(2, d3 * 1.7);
    if (times < halvedAt + nHalvedElements)
      ret->setDirection(1, d2 * (times / 2. + 1));
    else
      ret->setDirection(1, d2 * ((times - nHalvedElements) / 2. + 1));
    break;
  }
  }
  return ret;
}
int strawTube::suspect(const sLine3D& line, int stackType)
{
  plane3D plane(center, direction ^ stackingDirection);
  point3D hit = plane - line;
  switch (stackType) {
  case 0: {
    vector3D d1 = direction, d2 = stackingDirection, d3 = d1 ^ d2;
    d3.normalize();
    d3 = d3 * radius * 2.;
    fiber tmpfiber(center - d1 * 0.5 - d2 * .5, d1, d2, d3);
    return tmpfiber.suspect(line, stackType);
  }
  case 1: {
    vector3D d1 = direction, d2 = stackingDirection, d3 = d1 ^ d2;
    d3.normalize();
    d3 = d3 * radius * 2.;
    fiber tmpfiber(center - d1 * 0.5 - d2 * .5, d1, d2, d3, nHalvedElements, halvedAt);
    return tmpfiber.suspect(line, stackType);
  }
  case 2: {
    vector3D d1 = direction, d2 = stackingDirection, d3 = d1 ^ d2;
    d3.normalize();
    d3 = d3 * radius * 2.;
    fiber tmpfiber(center - d1 * 0.5 - d2 * .5, d1, d2, d3, nHalvedElements, halvedAt);
    return 2 * tmpfiber.suspect(line, stackType);
  }
  }
  return -1;
}

void strawTube::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                     int fColor, int fStyle) const
{
  strawTube::Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
void strawTube::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                     TObject** ident, int lColor, int fColor, int fStyle) const
{
  cylinder tmp(center, direction, isoChrone);
  cylinder::Draw(eye, plane, boundingBox, ident, lColor, fColor, fStyle);
  tmp.Draw(eye, plane, boundingBox, NULL, fColor, fColor, fStyle);
}
shape_parameter strawTube::description() const
{
  shape_parameter sh;
  sh.setName("strawTube");
  sh.setId(strawtube_id);
  sh.addValue("center", center);
  sh.addValue("direction", direction);
  sh.addValue("stacking direction", stackingDirection);
  sh.addValue("shift", shift);
  sh.addValue("number of halved elements", nHalvedElements);
  sh.addValue("halved at", halvedAt);
  sh.addValue("radius", radius);
  sh.setCompleteWrite(true);
  return sh;
}
shape_parameter strawTube::getDescription()
{
  shape_parameter sh;
  sh.setName("strawTube");
  sh.setId(strawtube_id);
  sh.addValue("center", point3D());
  sh.addValue("direction", vector3D());
  sh.addValue("stacking direction", vector3D());
  sh.addValue("shift", vector3D());
  sh.addValue("number of halved elements", static_cast<int>(0));
  sh.addValue("halved at", static_cast<int>(0));
  sh.addValue("radius", static_cast<float>(0));
  sh.setCompleteWrite(false);
  return sh;
}
