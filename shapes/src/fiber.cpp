#include "fiber.h"
#include "hexPrism.h"
#include "planeShape.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {

const boost::uuids::uuid fiber_id = {{0xff, 0x39, 0x63, 0x7e, 0x11, 0xd9, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid fiber_id2 = {{0x06, 0x45, 0x30, 0xbc, 0x11, 0xda, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct FiberShapeProvider : public FactoryShapeProvider,
                            public std::enable_shared_from_this<FiberShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    auto fiberCreation = [](shape_parameter const& param) -> std::shared_ptr<volumeShape> {
      if (!checkParameter(param)) {
        return {};
      }
      vector3D shift = 0.5 * param.value(1).value<vector3D>() +
                       param.value(2).value<vector3D>() *
                           (param.value(5).value<int>() + 0.5 * param.value(4).value<int>());
      auto corner = (param.value(6).value<bool>() ? param.value(0).value<point3D>()
                                                  : param.value(0).value<point3D>() - shift);
      std::cout << "Fiber creation: " << shift << " " << corner << param.value(1).value<vector3D>()
                << param.value(2).value<vector3D>() << std::endl;
      return std::shared_ptr<volumeShape>(
          new fiber(corner, param.value(1).value<vector3D>(), param.value(2).value<vector3D>(),
                    param.value(3).value<vector3D>(), param.value(4).value<int>(),
                    param.value(5).value<int>()));
    };

    auto fiberNext2 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      if (!checkParameter(parameter)) {
        return {};
      }
      auto result = parameter;
      result.value(6) = true;
      auto halvedAt = static_cast<size_t>(parameter.value(5).value<int>());
      auto numberOfHalvedElements = static_cast<size_t>(parameter.value(4).value<int>());
      auto corner = parameter.value(0).value<point3D>();
      auto dir1 = parameter.value(1).value<vector3D>();
      auto dir2 = parameter.value(2).value<vector3D>();
      if (!parameter.value(6).value<bool>()) {
        vector3D shift = 0.5 * dir1 + dir2 * (halvedAt + 0.5 * numberOfHalvedElements);
        corner = corner - shift;
      }

      if (times < halvedAt) {
        result.value(0) = corner + (dir2 * times);
      } else if (times >= 2 * numberOfHalvedElements + halvedAt) {
        result.value(0) = corner + (dir2 * (times - numberOfHalvedElements));
      } else {
        if (times < halvedAt + numberOfHalvedElements) {
          result.value(0) = corner + (dir2 * times);
        } else {
          result.value(0) = corner + dir1 * 0.5 + dir2 * (times - numberOfHalvedElements);
        }
        result.value(1) = dir1 * 0.5;
      }
      return result;
    };
    auto fiberNext17 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      if (!checkParameter(parameter)) {
        return {};
      }
      auto result = parameter;
      result.value(6) = true;
      auto halvedAt = static_cast<size_t>(parameter.value(5).value<int>());
      auto numberOfHalvedElements = static_cast<size_t>(parameter.value(4).value<int>());
      auto corner = parameter.value(0).value<point3D>();
      auto dir1 = parameter.value(1).value<vector3D>();
      auto dir2 = parameter.value(2).value<vector3D>();
      if (!parameter.value(6).value<bool>()) {
        vector3D shift = 0.5 * dir1 + dir2 * (halvedAt + 0.5 * numberOfHalvedElements);
        corner = corner - shift;
      }

      if (times < halvedAt) {
        auto dr1 = dir1 * ((dir1.R() + dir2.R() * times * 2) / dir1.R());
        result.value(0) = corner + dir2 * times + (dir1 - dr1) * 0.5;
        result.value(1) = dr1;
      } else if (times < halvedAt + numberOfHalvedElements) {
        auto dr1 =
            dir1 * ((dir1.R() + dir2.R() * (halvedAt * 2 + numberOfHalvedElements - times) * 2) /
                    dir1.R());
        result.value(0) = corner + (dir2 * (times - numberOfHalvedElements)) + (dir1 - dr1) * 0.5;
        result.value(1) = dr1;
      } else {
        auto dr1 =
            dir1 * ((dir1.R() * 0.5 + dir2.R() * (times - numberOfHalvedElements)) / dir1.R());
        result.value(0) = corner + dir1 * 0.5 - dr1 + (dir2 * (times - numberOfHalvedElements));
        result.value(1) = dr1;
      }
      return result;
    };
    auto fiberEnvelope2 = [](shape_parameter const& parameter, size_t number) -> shape_parameter {
      if (!checkParameter((parameter))) {
        return {};
      }
      auto result = parameter;
      result.value(6) = true;
      auto halvedAt = static_cast<size_t>(parameter.value(5).value<int>());
      auto numberOfHalvedElements = static_cast<size_t>(parameter.value(4).value<int>());
      auto corner = parameter.value(0).value<point3D>();
      auto dir1 = parameter.value(1).value<vector3D>();
      auto dir2 = parameter.value(2).value<vector3D>();
      if (!parameter.value(6).value<bool>()) {
        vector3D shift = 0.5 * dir1 + dir2 * (halvedAt + 0.5 * numberOfHalvedElements);
        result.value(0) = parameter.value(0).value<point3D>() - shift;
      }

      if (number > halvedAt) {
        result.value(2) = dir2 * (number - numberOfHalvedElements);
      } else {
        result.value(2) = dir2 * number;
      }
      std::cout << number << " " << result.value(2).value<vector3D>() << " " << halvedAt
                << std::endl;
      return result;
    };
    auto fiberEnvelope17 = [](shape_parameter const& parameter, size_t number) -> shape_parameter {
      if (!checkParameter(parameter)) {
        return {};
      }
      auto halvedAt = static_cast<size_t>(parameter.value(5).value<int>());
      auto numberOfHalvedElements = static_cast<size_t>(parameter.value(4).value<int>());
      auto corner = parameter.value(0).value<point3D>();
      auto dir1 = parameter.value(1).value<vector3D>();
      auto dir2 = parameter.value(2).value<vector3D>();
      if (!parameter.value(6).value<bool>()) {
        vector3D shift = 0.5 * dir1 + dir2 * (halvedAt + 0.5 * numberOfHalvedElements);
        corner = corner - shift;
      }

      // well, the envelope of this shape-stack is a hexPrism
      auto keyWidth = dir2 * (halvedAt + 0.5 * numberOfHalvedElements);
      auto centerOfFront = corner + 0.5 * dir1 + keyWidth;
      auto thickness = parameter.value(3).value<vector3D>();
      auto toFirstPoint = corner - centerOfFront;

      auto result = hexPrism::getDescription();
      result.value(0) = centerOfFront;
      result.value(1) = thickness;
      result.value(2) = toFirstPoint;
      result.value(3) = keyWidth;
      return result;
    };
    auto description = fiber::getDescription();
    factory.addShapeToFactory(description, ShapeType::VolumeShape, fiberCreation, fiberNext2,
                              fiberEnvelope2);
    description.setId(fiber_id2);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, fiberCreation, fiberNext17,
                              fiberEnvelope17);
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(fiber_id);
    factory.removeShapeFromFactory(fiber_id2);
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
             param.value(6).valueType() != ParameterValue::ValueType::BOOLEAN);
  }
};
volatile static std::shared_ptr<FiberShapeProvider> prov = [] {
  auto r = std::make_shared<FiberShapeProvider>();
  r->install();
  return r;
}();
}

fiber::fiber(point3D cor, vector3D d1, vector3D d2, vector3D d3, int halved, int halvedat)
    : volumeShape("fiber")
{
  numberOfHalvedElements = halved;
  halvedAt = halvedat;
  corner.setValues(cor.X(), cor.Y(), cor.Z());
  dir1.setValues(d1.X(), d1.Y(), d1.Z());
  dir2.setValues(d2.X(), d2.Y(), d2.Z());
  dir3.setValues(d3.X(), d3.Y(), d3.Z());
  center = corner + dir1 * 0.5 + dir2 * 0.5 + dir3 * 0.5;
  if (dir1.R() > dir2.R() && dir1.R() > dir3.R())
    res = dir2;
  else if (dir2.R() > dir3.R())
    res = dir3;
  else
    res = dir1;
  res.normalize();
}

fiber::fiber(const fiber& s) : volumeShape("fiber")
{
  dir1 = s.getDirection(0);
  dir2 = s.getDirection(1);
  dir3 = s.getDirection(2);
  corner = s.getCorner();
  numberOfHalvedElements = s.getHalved();
  halvedAt = s.getHalvedAt();
  setMaxDistance(s.getMaxDistance());
  setCenter(s.getCenter());
  if (dir1.R() > dir2.R() && dir1.R() > dir3.R())
    res = dir2;
  else if (dir2.R() > dir3.R())
    res = dir3;
  else
    res = dir1;
  res.normalize();
}

fiber::fiber(const volumeShape& s) : volumeShape("fiber")
{
  if ((s.getName() != "fiber") && (s.getName() != "rectangular"))
    return;
  if (s.getName() == "fiber") {
    dir1 = ((fiber)s).getDirection(0);
    dir2 = ((fiber)s).getDirection(1);
    dir3 = ((fiber)s).getDirection(2);
    corner = ((fiber)s).getCorner();
    numberOfHalvedElements = ((fiber)s).getHalved();
    halvedAt = ((fiber)s).getHalvedAt();
  } else {
    // 	dir1=s.getDirection(0);
    // 	dir2=s.getDirection(1);
    // 	dir3=s.getDirection(2);
    // 	corner=s.getCorner();
    // 	numberOfHalvedElements=s.getHalved();
    // 	halvedAt=s.getHalvedAt();
  }
  setMaxDistance(s.getMaxDistance());
  setCenter(s.getCenter());
  if (dir1.R() > dir2.R() && dir1.R() > dir3.R())
    res = dir2;
  else if (dir2.R() > dir3.R())
    res = dir3;
  else
    res = dir1;
  res.normalize();
}
fiber::fiber(const shape_parameter& description) : volumeShape("fiber")
{
  if (description.getName() != "fiber")
    return;
  if (!FiberShapeProvider::checkParameter(description)) {
    return;
  }
  dir1 = description.value(1).value<vector3D>();
  dir2 = description.value(2).value<vector3D>();
  dir3 = description.value(3).value<vector3D>();
  numberOfHalvedElements = description.value(4).value<int>();
  halvedAt = description.value(5).value<int>();
  corner = description.value(0).value<point3D>() - dir1 * 0.5 -
           dir2 * (halvedAt + (numberOfHalvedElements % 2) * 0.5 + numberOfHalvedElements);
}
void fiber::operator=(const volumeShape& s)
{
  if ((s.getName() != "fiber") && (s.getName() != "rectangular"))
    return;
  if (s.getName() == "fiber") {
    dir1 = ((fiber)s).getDirection(0);
    dir2 = ((fiber)s).getDirection(1);
    dir3 = ((fiber)s).getDirection(2);
    corner = ((fiber)s).getCorner();
    numberOfHalvedElements = ((fiber)s).getHalved();
    halvedAt = ((fiber)s).getHalvedAt();
  } else {
    // 	dir1=s.getDirection(0);
    // 	dir2=s.getDirection(1);
    // 	dir3=s.getDirection(2);
    // 	corner=s.getCorner();
    // 	numberOfHalvedElements=s.getHalved();
    // 	halvedAt=s.getHalvedAt();
  }
  setMaxDistance(s.getMaxDistance());
  setCenter(s.getCenter());
  if (dir1.R() > dir2.R() && dir1.R() > dir3.R())
    res = dir2;
  else if (dir2.R() > dir3.R())
    res = dir3;
  else
    res = dir1;
  res.normalize();
}

Vector fiber::HitParams(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D coord(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D pitch;
  vector3D npc;
  vector3D atm;
  vector3D dist_(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  float sigma = hit.getValue(9);
  npc = coord - dist_;
  res = dir2 / dir2.R();
  pitch = res; // atm;
  if (((corner + 0.5 * dir1 + 0.5 * dir2 + 0.5 * dir3) - coord) * res < 0)
    pitch = pitch * (-1);
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, coord.X());
  ret.setValue(2, coord.Y());
  ret.setValue(3, coord.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist_.X());
  ret.setValue(11, dist_.Y());
  ret.setValue(12, dist_.Z());
  return ret;
}
vector3D fiber::distance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  //  cout<<"fiber::distance()"<<dist<<endl;
  return dist;
}
volumeShape* fiber::getEnvelope(int times, int stackType)
{
  switch (stackType) {
  case 2: {
    fiber* sh = new fiber(*this);
    if (times > halvedAt)
      sh->setDirection(1, dir2 * (times - numberOfHalvedElements / 2));
    else
      sh->setDirection(1, dir2 * times);
    sh->setMaxDistance(getMaxDistance());
    return sh;
  }
  case 17: {

    point3D cor = corner;
    vector3D dr1 = dir1 + dir2 * dir1.R() / dir2.R();
    vector3D dr2 = dr1 - dir1 * 2;
    cor = cor + dir1 * 0.5 - dir2 * halvedAt;
    fiber* sh = new fiber(*this);
    sh->setDirection(0, dr1);
    sh->setDirection(1, dr2);
    sh->setMaxDistance(getMaxDistance());
    sh->setCorner(cor);
    return sh;
  }
  }
  return this->getClone();
}
float fiber::getFlightPathInShape(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  if (dist.R() > __prec)
    return 0;
  Vector hit2(Hitting(sLine3D(line.Foot(), line.Direction() * -1)));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  point3D out(hit2.getValue(0), hit2.getValue(1), hit2.getValue(2));
  return (out - enter).R();
}
sLine3D fiber::Normal(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return sLine3D(enter, norm);
  return sLine3D();
}
bool fiber::cut(const fiber& f)
{
  vector3D cTc = f.getCorner() - corner;
  float xp, yp, zp, xl, yl, zl;
  xl = dir1.R();
  yl = dir2.R();
  zl = dir3.R();
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc + f.getDirection(0); // x
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc + f.getDirection(1); // xy
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc + f.getDirection(2); // xyz
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc - f.getDirection(0); // yz
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc - f.getDirection(1); // z
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc + f.getDirection(0); // zx
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  cTc = cTc - f.getDirection(2) - f.getDirection(0) + f.getDirection(1); // y
  xp = (cTc * dir1) / xl / xl;
  yp = (cTc * dir2) / yl / yl;
  zp = (cTc * dir3) / zl / zl;
  if ((abs(xp) < 1) || (abs(yp) < 1) || (abs(zp) < 1))
    return true;
  return false;
}

point3D fiber::entrance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return enter;
  return point3D();
}

int fiber::getHalved() const { return numberOfHalvedElements; }

void fiber::setHalved(int num) { numberOfHalvedElements = num; }

int fiber::getHalvedAt() const { return halvedAt; }

void fiber::setHalvedAt(int num) { halvedAt = num; }

point3D fiber::getCorner() const { return corner; }

vector3D fiber::getDirection(int num) const
{
  switch (num) {
  case 0:
    return dir1;
  case 1:
    return dir2;
  case 2:
    return dir3;
  }
  return vector3D();
}

void fiber::setCorner(point3D p)
{
  corner.setValues(p.X(), p.Y(), p.Z());
  center = corner + dir1 * 0.5 + dir2 * 0.5 + dir3 * 0.5;
}

void fiber::setDirection(int num, vector3D v)
{
  switch (num) {
  case 0:
    dir1.setValues(v.X(), v.Y(), v.Z());
    return;
  case 1:
    dir2.setValues(v.X(), v.Y(), v.Z());
    return;
  case 2:
    dir3.setValues(v.X(), v.Y(), v.Z());
    return;
  }
  center = corner + dir1 * 0.5 + dir2 * 0.5 + dir3 * 0.5;
  if (dir1.R() > dir2.R() && dir1.R() > dir3.R())
    res = dir2;
  else if (dir2.R() > dir3.R())
    res = dir3;
  else
    res = dir1;
  res.normalize();
}

volumeShape* fiber::getNext(int times, int stackType)
{
  fiber* sh = new fiber(*this);
  switch (stackType) {
  case 2: {
    if (times < halvedAt) // times<halvedAt-numberOfHalvedElements)
      sh->setCorner(corner + (dir2 * times));
    else if (times >=
             2 * numberOfHalvedElements + halvedAt) // times>=halvedAt+numberOfHalvedElements)
      sh->setCorner(corner + (dir2 * (times - numberOfHalvedElements)));
    else {
      point3D c = corner;
      if (times < halvedAt + numberOfHalvedElements)
        c = c + (dir2 * times);
      else
        c = c + (dir1 * 0.5) + (dir2 * (times - numberOfHalvedElements));
      sh->setCorner(c);
      sh->setDirection(0, dir1 * 0.5);
    }
    sh->setMaxDistance(getMaxDistance());
    break;
  }
  case 17: {
    point3D cor = corner;
    vector3D dr1;
    int nd2;
    if (times < halvedAt) {
      nd2 = times * 2;
      dr1 = dir1 * ((dir1.R() + dir2.R() * nd2) / dir1.R());
      cor = cor + (dir2 * times) + (dir1 - dr1) * 0.5;
    } else if (times >= 2 * numberOfHalvedElements + halvedAt) {
      nd2 = (halvedAt * 2 + numberOfHalvedElements - times) * 2;
      dr1 = dir1 * ((dir1.R() + dir2.R() * nd2) / dir1.R());
      cor = cor + (dir2 * (times - numberOfHalvedElements)) + (dir1 - dr1) * 0.5;
    } else {
      if (times < halvedAt + numberOfHalvedElements) {
        nd2 = times;
        dr1 = dir1 * ((dir1.R() * 0.5 + dir2.R() * nd2) / dir1.R());
        cor = cor + dir1 * 0.5 + (dir2 * times);
      } else {
        nd2 = times - numberOfHalvedElements;
        dr1 = dir1 * ((dir1.R() * 0.5 + dir2.R() * nd2) / dir1.R());
        cor = cor + dir1 * 0.5 - dr1 + (dir2 * (times - numberOfHalvedElements));
      }
    }
    sh->setCorner(cor);
    sh->setDirection(0, dr1);
    sh->setMaxDistance(getMaxDistance());
  }
  }
  return sh;
}

volumeShape* fiber::getClone()
{
  fiber* sh = new fiber(*this);
  sh->setMaxDistance(getMaxDistance());
  return sh;
}

//#include <TPolyLine.h>

Vector fiber::Hitting(const sLine3D& line)
{
  //  cout<<"\nfiber::Hitting()";
  Vector ret(10);
  float /*length=dir1.R(),*/ width = dir2.R() /*,thickness=dir3.R()*/;
  float /*sigma1=length*0.5,*/ sigma2 = width * 0.5 /*,sigma3=thickness*0.5*/;
  // make first guess
  vector3D diag = (dir2 + dir3) * 0.5;
  lLine3D testLine(corner + dir2 * 0.5 + dir3 * 0.5, dir1);
  lLine3D poca = line - testLine;
  if (poca.length() > diag.R() * 10) // far away, make approximation
  {
    //      cout<<"far away"<<endl;
    point3D p = poca.P() + poca.Direction() * (diag * poca.Direction());
    vector3D q = p - poca.Q();
    ret.setValues(p.X(), p.Y(), p.Z(), q.X(), q.Y(), q.Z(), q.X(), q.Y(), q.Z(), sigma2);
    return ret;
  }
  //  else if(poca.length()>diag.R())//out of volume
  {
    vector3D direct = line.Direction();
    // first hit with plane 3 -> most likely
    point3D c;
    if (dir3 * direct < 0)
      c = corner;
    else
      c = corner + dir3;
    plane3D plane(c, dir3);
    point3D hit1 = plane - line;
    vector3D l = hit1 - c;
    vector3D n = dir3 / dir3.R();
    float ll1 = (l * dir1) / dir1.R() / dir1.R();
    float ll2 = (l * dir2) / dir2.R() / dir2.R();
    if (ll1 >= 0 && ll1 <= 1 && ll2 >= 0 && ll2 <= 1) {
      ret.setValues(hit1.X(), hit1.Y(), hit1.Z(), 0, 0, 0, n.X(), n.Y(), n.Z(), sigma2);
      return ret;
    }
    vector3D d1 = dir1 * (ll1 < 0 ? ll1 : (ll1 < 1 ? 0 : ll1 - 1)) +
                  dir2 * (ll2 < 0 ? ll2 : (ll2 < 1 ? 0 : ll2 - 1));

    // hit with plane 2 -> likely
    if (dir2 * direct < 0)
      c = corner;
    else
      c = corner + dir2;
    plane = plane3D(c, dir2);
    point3D hit2 = plane - line;
    l = hit2 - c;
    n = dir2 / dir2.R();
    ll1 = (l * dir1) / dir1.R() / dir1.R();
    ll2 = (l * dir3) / dir3.R() / dir3.R();
    if (ll1 >= 0 && ll1 <= 1 && ll2 >= 0 && ll2 <= 1) {
      ret.setValues(hit2.X(), hit2.Y(), hit2.Z(), 0, 0, 0, n.X(), n.Y(), n.Z(), sigma2);
      return ret;
    }
    vector3D d2 = dir1 * (ll1 < 0 ? ll1 : (ll1 < 1 ? 0 : ll1 - 1)) +
                  dir3 * (ll2 < 0 ? ll2 : (ll2 < 1 ? 0 : ll2 - 1));
    // last hit with plane 1 -> least likely
    if (dir2 * direct < 0)
      c = corner;
    else
      c = corner + dir1;
    plane = plane3D(c, dir1);
    point3D hit3 = plane - line;
    l = hit3 - c;
    n = dir1 / dir1.R();
    ll1 = (l * dir2) / dir2.R() / dir2.R();
    ll2 = (l * dir3) / dir3.R() / dir3.R();
    if (ll1 >= 0 && ll1 <= 1 && ll2 >= 0 && ll2 <= 1) {
      ret.setValues(hit3.X(), hit3.Y(), hit3.Z(), 0, 0, 0, n.X(), n.Y(), n.Z(), sigma2);
      return ret;
    }
    vector3D d3 = dir2 * (ll1 < 0 ? ll1 : (ll1 < 1 ? 0 : ll1 - 1)) +
                  dir3 * (ll2 < 0 ? ll2 : (ll2 < 1 ? 0 : ll2 - 1));
    if (d1.R() <= d2.R() && d1.R() <= d3.R()) {
      ret.setValues(hit1.X(), hit1.Y(), hit1.Z(), d1.X(), d1.Y(), d1.Z(), dir3.X(), dir3.Y(),
                    dir3.Z(), sigma2);
      return ret;
    }
    if (d3.R() > d2.R()) {
      ret.setValues(hit2.X(), hit2.Y(), hit2.Z(), d2.X(), d2.Y(), d2.Z(), dir2.X(), dir2.Y(),
                    dir2.Z(), sigma2);
      return ret;
    }
    ret.setValues(hit3.X(), hit3.Y(), hit3.Z(), d3.X(), d3.Y(), d3.Z(), dir1.X(), dir1.Y(),
                  dir1.Z(), sigma2);
    return ret;
  }

  /*     vector3D direct=line.Direction();
  double ang[3];
  ang[0]=acos(dir1*direct/(dir1.R()*direct.R()));
  ang[1]=acos(dir2*direct/(dir2.R()*direct.R()));
  ang[2]=acos(dir3*direct/(dir3.R()*direct.R()));
  int pl=1;
  if(fabs(ang[1])>fabs(ang[0]))pl=2;
  if(fabs(ang[2])>fabs(ang[pl-1]))pl=3;
  pl=pl*(ang[pl-1]>0?1:-1);
  plane3D plane;
  switch(pl)
    {
    case -1:plane=plane3D(corner+dir1,dir1);break;
    case -2:plane=plane3D(corner+dir2,dir2);break;
    case -3:plane=plane3D(corner+dir3,dir3);break;
    case 1:plane=plane3D(corner,dir1*-1);break;
    case 2:plane=plane3D(corner,dir2*-1);break;
    case 3:plane=plane3D(corner,dir3*-1);break;
    }
  point3D hitP=plane-line;
  //      cout<<plane<<pl<<"
"<<plane.Normal()*line.Direction()<<line<<hitP<<endl;
  //      cout<<(plane&line.Foot())<<endl;
  float dx,dy,dz;
  vector3D dist=hitP-plane.Foot();
  dx=dist*dir1/(dir1.R()*dir1.R());
  dy=dist*dir2/(dir2.R()*dir2.R());
  dz=dist*dir3/(dir3.R()*dir3.R());
  //      cout<<dx<<" "<<dy<<" "<<dz<<endl;
  //	  cout<<dist<<hitP;
  if(dx>=0&&dx<=1&&dy<=1&&dy>=0&&dz<=1&&dz>=0)
    ret.setValues(hitP.X(),hitP.Y(),hitP.Z(),0,0,0,plane.Normal().X(),plane.Normal().Y(),plane.Normal().Z(),sigma2);
  else
    {
// 	  if((dist-dir1).R()<dist.R())dist=dist-dir1;
// 	  if((dist-dir2).R()<dist.R())dist=dist-dir2;
// 	  if((dist-dir3).R()<dist.R())dist=dist-dir3;
// 	  if((dist+dir1).R()<dist.R())dist=dist+dir1;
// 	  if((dist+dir2).R()<dist.R())dist=dist+dir2;
// 	  if((dist+dir3).R()<dist.R())dist=dist+dir3;
      if(dx>1)
        dist=dist-dir1;
      if(dy>1)
        dist=dist-dir2;
      if(dz>1)
        dist=dist-dir3;
      hitP=hitP-dist;
      ret.setValues(hitP.X(),hitP.Y(),hitP.Z(),dist.X(),dist.Y(),dist.Z(),plane.Normal().X(),plane.Normal().Y(),plane.Normal().Z(),sigma2);
    }
  //      cout<<ret<<endl;
  return ret;
}
//Hit with 3 planes only with planes that can have an entrance =>
normal_to_plane*line_direction<0
plane3D p1;
plane3D p2;
plane3D p3;
point3D c1,c2,c3;//corner of rectangles in each plane
vector3D d1=dir1,d2=dir2,d3=dir3;
d1.normalize();
d2.normalize();
d3.normalize();
vector3D v1_1,v1_2,v2_1,v2_2,v3_1,v3_2;//components of hit1-corner1 in each
fiber direction
//    float d11,d12,d21,d22,d31,d32;//length of these components
float dr11,dr12,dr21,dr22,dr31,dr32;//direction of components >0 ->point inwards
int outcode1=0,outcode2=0,outcode3=0;
//calculate planes
//first plane
if(line.Direction()*dir1>0)
  {
    p1=plane3D(corner,dir1*(-1));
    c1=corner;
  }
else
  {
    p1=plane3D(corner+dir1,dir1);
    c1=corner+dir1;
  }
//second plane
if(line.Direction()*dir2>0)
  {
    p2=plane3D(corner,dir2*(-1));
    c2=corner;
  }
else
  {
    p2=plane3D(corner+dir2,dir2);
    c2=corner+dir2;
  }
//third plane
if(line.Direction()*dir3>0)
  {
    p3=plane3D(corner,dir3*(-1));
    c3=corner;
  }
else
  {
    p3=plane3D(corner+dir3,dir3);
    c3=corner+dir3;
  }
//calculate hit variables
//first plane
point3D hit1=p1-line;
if(hit1.getState()==_undefined_)
  {
    dr11=-1;
    dr12=-1;
  }
else
  {
    v1_1=hit1-c1;
    v1_2=v1_1;
    dr11=v1_1*d2;
    dr12=v1_2*d3;
  }
outcode1=(dr11<0?1:0)*1+(dr11>width?1:0)*2
  +(dr12<0?1:0)*4+(dr12>thickness?1:0)*8;
//second plane
point3D hit2=p2-line;
if(hit2.getState()==_undefined_)
  {
    dr21=-1;
    dr22=-1;
  }
else
  {
    v2_1=hit2-c2;
    v2_2=v2_1;
    dr21=v2_1*d3;
    dr22=v2_2*d1;
  }
outcode2=(dr21<0?1:0)*1+(dr21>thickness?1:0)*2
  +(dr22<0?1:0)*4+(dr22>length?1:0)*8;
//third plane
point3D hit3=p3-line;
if(hit3.getState()==_undefined_)
  {
    dr31=-1;
    dr32=-1;
  }
else
  {
    v3_1=hit3-c3;
    v3_2=v3_1;
    dr31=v3_1*d1;
    dr32=v3_2*d2;
  }
//    v3_1=(dr31)*d1;
//    v3_2=(dr32)*d2;
//    d31=v3_1.R();
//    d32=v3_2.R();
outcode3=(dr31<0?1:0)*1+(dr31>length?1:0)*2
  +(dr32<0?1:0)*4+(dr32>width?1:0)*8;
//check planes
//first plane
if(outcode1==0)
  {
    ret.setValues(hit1.X(),hit1.Y(),hit1.Z(),0,0,0,p1.Normal().X(),p1.Normal().Y(),p1.Normal().Z(),sigma1);
    return ret;
  }
//second plane
if(outcode2==0)
  {
    ret.setValues(hit2.X(),hit2.Y(),hit2.Z(),0,0,0,p2.Normal().X(),p2.Normal().Y(),p2.Normal().Z(),sigma2);
    return ret;
  }
//third plane
if(outcode3==0)
  {
    ret.setValues(hit3.X(),hit3.Y(),hit3.Z(),0,0,0,p3.Normal().X(),p3.Normal().Y(),p3.Normal().Z(),sigma3);
    return ret;
  }
//no hit point calculate distances
vector3D dist1,dist2,dist3;
//first plane
switch(outcode1)
  {
  case 5:dist1=c1-hit1;break;
  case 6:dist1=(c1+dir2)-hit1;break;
  case 9:dist1=(c1+dir3)-hit1;break;
  case 10:dist1=(c1+dir2+dir3)-hit1;break;
  case 4:dist1=lLine3D(c1,dir2)-hit1;break;
  case 1:dist1=lLine3D(c1,dir3)-hit1;break;
  case 2:dist1=lLine3D(c1+dir2,dir3)-hit1;break;
  case 8:dist1=lLine3D(c1+dir3,dir2)-hit1;break;
  }
//second plane
switch(outcode2)
  {
  case 5:dist2=c2-hit2;break;
  case 6:dist2=(c2+dir3)-hit2;break;
  case 9:dist2=(c2+dir1)-hit2;break;
  case 10:dist2=(c2+dir3+dir1)-hit2;break;
  case 4:dist2=lLine3D(c2,dir3)-hit2;break;
  case 1:dist2=lLine3D(c2,dir1)-hit2;break;
  case 2:dist2=lLine3D(c2+dir3,dir1)-hit2;break;
  case 8:dist2=lLine3D(c2+dir1,dir3)-hit2;break;
  }

//third plane
switch(outcode3)
  {
  case 5:dist3=c3-hit3;break;
  case 6:dist3=(c3+dir1)-hit3;break;
  case 9:dist3=(c3+dir2)-hit3;break;
  case 10:dist3=(c3+dir1+dir2)-hit3;break;
  case 4:dist3=lLine3D(c3,dir1)-hit3;break;
  case 1:dist3=lLine3D(c3,dir2)-hit3;break;
  case 2:dist3=lLine3D(c3+dir1,dir2)-hit3;break;
  case 8:dist3=lLine3D(c3+dir2,dir1)-hit3;break;
  }
if((dist1.R()<=dist2.R())&&(dist1.R()<=dist3.R()))
  {
    ret.setValues(hit1.X(),hit1.Y(),hit1.Z(),dist1.X(),dist1.Y(),dist1.Z(),0,0,0,sigma1);
    return ret;
  }
if(dist2.R()<dist3.R())
  {
    ret.setValues(hit2.X(),hit2.Y(),hit2.Z(),dist2.X(),dist2.Y(),dist2.Z(),0,0,0,sigma2);
    return ret;
  }
ret.setValues(hit3.X(),hit3.Y(),hit3.Z(),dist3.X(),dist3.Y(),dist3.Z(),0,0,0,sigma3);
return ret;*/
}

Vector fiber::HitParams(const planeShape& shape, point3D origin)
{
  Vector hit(Hitting(sLine3D(origin, shape.getCenter() - origin)));
  point3D coord; //(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D pitch;
  point3D npc;
  //  vector3D atm;
  vector3D dist_(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  float sigma = hit.getValue(9);

  if (dist_.R() < __prec) {
    coord.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    npc = coord + dist_;
  } else {
    float dp = shape.angularRange_Phi(origin);
    float dt = shape.angularRange_Theta(origin);
    vector3D e(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    vector3D n = e + dist_;
    e = e - origin;
    n = n - origin;
    float dp1 = abs(e.Phi() - n.Phi()), dt1 = abs(e.Theta() - n.Theta());
    if (dp1 < dp && dt1 < dt) {
      int numCorners = shape.getNumberOfPoints();
      point3D corners[numCorners];
      vector3D dists[numCorners];
      point3D npcs[numCorners];
      point3D enters[numCorners];
      bool init = false;
      for (int i = 0; i < numCorners; i++) {
        corners[i] = shape.getPoint(i);
      }
      for (int i = 0; i < numCorners; i++) {
        hit = Vector(Hitting(sLine3D(origin, corners[i] - origin)));
        dists[i].setValues(hit.getValue(3), hit.getValue(4), hit.getValue(5));
        enters[i].setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
        npcs[i] = enters[i] + dists[i];
        if (dists[i].R() < __prec) {
          coord = enters[i];
          npc = npcs[i];
          dist_ = dists[i];
          init = true;
          break;
        }
      }
      if (!init) {
        bool passing = false;
        for (int i = 0; i < numCorners; i++)
          for (int j = 0; j < numCorners; j++) {
            if (dists[i] * dists[j] < 0)
              passing = true;
          }
        for (int i = 0; i < numCorners; i++) {
          if (dist_.R() > dists[i].R()) {
            dist_ = dists[i];
            coord = enters[i];
            npc = npcs[i];
          }
        }
        if (passing) {
          dist_.setValues(0, 0, 0);
        }
      }
    } else {
      coord.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
      npc = coord + dist_;
    }
  }

  pitch = res; // atm;
  if (((corner + 0.5 * dir1 + 0.5 * dir2 + 0.5 * dir3) - coord) * res < 0)
    pitch = pitch * (-1);
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, coord.X());
  ret.setValue(2, coord.Y());
  ret.setValue(3, coord.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist_.X());
  ret.setValue(11, dist_.Y());
  ret.setValue(12, dist_.Z());
  return ret;
}
#include "quadrangle.h"
void fiber::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, TObject** ident,
                 int lColor, int fColor, int fStyle) const
{
  vector3D viewDir = plane.Foot() - eye;
  point3D cornersi[3][4], c[3];
  cornersi[0][0] = corner;
  cornersi[0][1] = corner + dir3;
  cornersi[0][2] = corner + dir3 + dir2;
  cornersi[0][3] = corner + dir2;
  cornersi[1][0] = corner;
  cornersi[1][1] = corner + dir3;
  cornersi[1][2] = corner + dir3 + dir1;
  cornersi[1][3] = corner + dir1;
  cornersi[2][0] = corner;
  cornersi[2][1] = corner + dir2;
  cornersi[2][2] = corner + dir2 + dir1;
  cornersi[2][3] = corner + dir1;
  if (dir1 * viewDir < 0)
    for (int i = 0; i < 4; i++)
      cornersi[0][i] = cornersi[0][i] + dir1;
  if (dir2 * viewDir < 0)
    for (int i = 0; i < 4; i++)
      cornersi[1][i] = cornersi[1][i] + dir2;
  if (dir3 * viewDir < 0)
    for (int i = 0; i < 4; i++)
      cornersi[2][i] = cornersi[2][i] + dir3;
  for (int i = 0; i < 3; i++)
    c[i] = point3D(0, 0, 0) +
           (vector3D(cornersi[i][0]) + vector3D(cornersi[i][1]) + vector3D(cornersi[i][2]) +
            vector3D(cornersi[i][3])) *
               0.25;

  quadrangle s1(c[0], cornersi[0][0], cornersi[0][1], cornersi[0][2], cornersi[0][3]);
  quadrangle s2(c[1], cornersi[1][0], cornersi[1][1], cornersi[1][2], cornersi[1][3]);
  quadrangle s3(c[2], cornersi[2][0], cornersi[2][1], cornersi[2][2], cornersi[2][3]);
  vector4D bb1, bb[2];
  s1.Draw(eye, plane, (boundingBox == NULL ? NULL : &bb1), lColor, fColor, fStyle);
  s2.Draw(eye, plane, (boundingBox == NULL ? NULL : &bb[0]), lColor, fColor, fStyle);
  s3.Draw(eye, plane, (boundingBox == NULL ? NULL : &bb[1]), ident, lColor, fColor, fStyle);
  if (boundingBox != NULL) {
    boundingBox->setValues(bb1[0], bb1[1], bb1[2], bb1[3]);
    for (int i = 0; i < 2; i++) {
      if (boundingBox->X() > bb[i].X())
        boundingBox->setValue(0, bb[i].X());
      if (boundingBox->Y() > bb[i].Y())
        boundingBox->setValue(1, bb[i].Y());
      if (boundingBox->Z() < bb[i].Z())
        boundingBox->setValue(2, bb[i].Z());
      if (boundingBox->W() < bb[i].W())
        boundingBox->setValue(3, bb[i].W());
    }
  }
}
void fiber::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                 int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
int fiber::suspect(const sLine3D& line, int stackType)
{
  switch (stackType) {
  case 2: {
    int nE = halvedAt * 2; //+numberOfHalvedElements;
    point3D c3;
    if (dir3 * line.Direction() < 0)
      c3 = corner + dir3;
    else
      c3 = corner;
    plane3D p3(c3, dir1, dir2);
    point3D h3 = p3 - line;
    vector3D l3 = h3 - c3;
    float l3_1 = l3 * dir1 / (dir1.R() * dir1.R());
    float l3_2 = l3 * dir2 / (dir2.R() * dir2.R());
    // cout<<"fiber::suspect(rect) "<<l3_1<<" "<<l3_2<<endl;
    bool has3 = (l3_1 <= 1 && l3_1 >= 0 && l3_2 <= nE && l3_2 >= 0);
    if (has3) {
      if (l3_2 < halvedAt) //-0.5*numberOfHalvedElements)
        return ((int)l3_2);
      if (l3_2 > halvedAt + numberOfHalvedElements) //+0.5*numberOfHalvedElements)
        return ((int)l3_2) + numberOfHalvedElements;
      if (l3_1 < 0.5)
        return ((int)l3_2);
      return ((int)l3_2) + numberOfHalvedElements;
    }

    point3D c1;
    point3D c2;
    if (dir1 * line.Direction() < 0)
      c1 = corner + dir1;
    else
      c1 = corner;
    if (dir2 * line.Direction() < 0)
      c2 = corner + dir2 * (numberOfHalvedElements * 2 + halvedAt);
    else
      c2 = corner;
    plane3D p1(c1, dir2, dir3), p2(c2, dir3, dir1);
    point3D h1 = p1 - line, h2 = p2 - line;
    vector3D l1 = h1 - c1, l2 = h2 - c2;
    float l1_2 = l1 * dir2 / (dir2.R() * dir2.R());
    float l1_3 = l1 * dir3 / (dir3.R() * dir3.R());
    float l2_3 = l2 * dir3 / (dir3.R() * dir3.R());
    float l2_1 = l2 * dir1 / (dir1.R() * dir1.R());
    bool has1 = (l1_2 <= nE && l1_2 >= 0 && l1_3 <= 1 && l1_3 >= 0);
    bool has2 = (l2_3 <= 1 && l2_3 >= 0 && l2_1 <= 1 && l2_1 >= 0);
    if (!(has1 || has2 || has3))
      return -1;
    if (has1) {
      return ((int)l1_2);
    }
    if (has2) // only first or last element
    {
      if ((c2 - center).R() < dir2.R() / 10)
        return 0;
      return nE - 1;
    }
    return -1;
  }
  case 17: {
    vector3D v1, v2;
    v1 = dir2 + dir1 / (dir1.R() * dir2.R());
    v2 = dir2 - dir1 / (dir1.R() * dir2.R());
    int nE = halvedAt * 2;
    // with dir1 being longest and dir2 being stack direction,
    // plane3 is largest in area! Try first
    point3D c3;
    if (dir3 * line.Direction() < 0)
      c3 = corner + dir3;
    else
      c3 = corner;
    plane3D p3(c3, dir1, dir2);
    point3D h3 = p3 - line;
    vector3D l3 = h3 - c3;
    float l3_1 = l3 * dir1 / (dir1.R() * dir1.R()); // front
    float l3_2 = l3 * dir2 / (dir2.R() * dir2.R());
    bool has3 = (l3_1 <= 1 + l3_2 * dir2.R() / dir1.R() && l3_1 >= -l3_2 / dir1.R() * dir2.R() &&
                 l3_2 <= nE && l3_2 >= 0);
    // cout<<"fiber::suspect(ext) "<<l3_1<<" "<<l3_2<<endl;
    if (has3) {
      if (l3_2 < halvedAt) //-0.5*numberOfHalvedElements)
        return ((int)l3_2);
      if (l3_2 > halvedAt + numberOfHalvedElements) //+0.5*numberOfHalvedElements)
        return ((int)l3_2) + numberOfHalvedElements;
      if (l3_1 < 0.5)
        return ((int)l3_2);
      return ((int)l3_2) + numberOfHalvedElements;
    }

    point3D c1;
    point3D c2;
    point3D c4;
    if (v2 * line.Direction() < 0)
      c1 = corner + v1 * (halvedAt + numberOfHalvedElements);
    else
      c1 = corner;
    if (v1 * line.Direction() < 0)
      c4 = corner + v2 * (halvedAt + numberOfHalvedElements);
    else
      c4 = corner;
    if (dir2 * line.Direction() < 0)
      c2 = corner + dir2 * (numberOfHalvedElements * 2 + halvedAt) + dir1;
    else
      c2 = corner;
    plane3D p2(c2, dir3, dir1);
    point3D h2 = p2 - line;
    vector3D l2 = h2 - c2;
    float l2_3 = l2 * dir3 / (dir3.R() * dir3.R()); // uniform side
    float l2_1 = l2 * dir1 / (dir1.R() * dir1.R());
    bool has2 = (l2_3 <= 1 && l2_3 >= 0 && l2_1 <= 1 && l2_1 >= 0);
    plane3D p1(c1, v1, dir3);
    point3D h1 = p1 - line;
    vector3D l1 = h1 - c1;
    float l1_2 = l1 * v1 / (v1.R() * v1.R()); // segmented side 1
    float l1_3 = l1 * dir3 / (dir3.R() * dir3.R());
    bool has1 = (l1_2 <= halvedAt && l1_2 >= 0 && l1_3 <= 1 && l1_3 >= 0);
    plane3D p4(c4, v2, dir3);
    point3D h4 = p4 - line;
    vector3D l4 = h4 - c4;
    float l4_1 = l4 * dir3 / (dir3.R() * dir3.R()); // segmented side 2
    float l4_2 = l4 * v2 / (v2.R() * v2.R());
    bool has4 = (l4_1 <= 1 && l4_1 >= 0 && l4_2 <= halvedAt && l4_2 >= 0);
    if (!(has1 || has2 || has3 || has4))
      return -1;
    if (has1) {
      return ((int)l1_2);
    }
    if (has4) {
      return ((int)l4_2);
    }
    if (has2) // only first or last element
    {
      if ((c2 - center).R() < dir2.R() / 10)
        return 0;
      return nE - 1;
    }
    return -1;
  }
  }
  return -1;
}
shape_parameter fiber::description() const
{
  shape_parameter sh;
  sh.setName("fiber");
  sh.addValue("center of envelope",
              corner + dir1 * 0.5 +
                  dir2 * (halvedAt + (numberOfHalvedElements % 2) * 0.5 + numberOfHalvedElements));
  sh.addValue("length", dir1);
  sh.addValue("width", dir2);
  sh.addValue("thickness", dir3);
  sh.addValue("number of halved elements", numberOfHalvedElements);
  sh.addValue("halved at", halvedAt);
  sh.addValue("useCorner", false);
  sh.setCompleteWrite(true);
  return sh;
}
shape_parameter fiber::getDescription()
{
  shape_parameter sh;
  sh.setName("fiber");
  sh.setId(fiber_id);
  sh.addValue("center of envelope", point3D());
  sh.addValue("length", vector3D());
  sh.addValue("width", vector3D());
  sh.addValue("thickness", vector3D());
  sh.addValue("number of halved elements", static_cast<int>(0));
  sh.addValue("halved at", static_cast<int>(0));
  sh.addValue("useCorner", false);
  sh.setCompleteWrite(false);
  return sh;
}
