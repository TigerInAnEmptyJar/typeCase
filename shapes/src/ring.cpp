#include "ring.h"
#include "cylinder.h"
#include "planeShape.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {
// 9be4d892-11ef-11e9-ab14-d663bd873d93
const boost::uuids::uuid ring_id1 = {{0x9b, 0xe4, 0xd8, 0x92, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
// a0a951f0-11ef-11e9-ab14-d663bd873d93
const boost::uuids::uuid ring_id2 = {{0xa0, 0xa9, 0x51, 0xf0, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct ShapeProvider : public FactoryShapeProvider,
                       public std::enable_shared_from_this<ShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    auto shapeCreation = [](shape_parameter const& param) -> std::shared_ptr<volumeShape> {
      return std::shared_ptr<volumeShape>(new ring(
          param.getParam<point3D>(0), param.getParam<vector3D>(0), param.getParam<float>(0),
          param.getParam<float>(0) + param.getParam<float>(1), param.getParam<float>(2)));
    };
    auto next1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      result.setParam<float>(0,
                             parameter.getParam<float>(0) + times * parameter.getParam<float>(1));
      return result;
    };
    auto next2 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      result.setParam<point3D>(0, parameter.getParam<point3D>(0) -
                                      parameter.getParam<vector3D>(0) *
                                          parameter.getParam<float>(2) * times);
      return result;
    };
    auto envelope1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      result.setParam<float>(1, parameter.getParam<float>(1) * times);
      return result;
    };
    auto envelope2 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      result.setParam<float>(2, parameter.getParam<float>(2) * times);
      return result;
    };

    auto description = ring::getDescription();
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next1, envelope1);
    description.setId(ring_id2);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next2, envelope2);
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(ring_id1);
    factory.removeShapeFromFactory(ring_id2);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
};
volatile static std::shared_ptr<ShapeProvider> prov = [] {
  auto r = std::make_shared<ShapeProvider>();
  r->install();
  return r;
}();
}

ring::ring(point3D centerIn, vector3D normalIn, float innerRadius, float outerRadius,
           float thicknessIn)
    : volumeShape("ring")
{
  center = centerIn;
  normal = normalIn;
  normal.normalize();
  innerR = innerRadius;
  outerR = outerRadius;
  thickness = thicknessIn;
}

ring::ring(const volumeShape& s) : volumeShape("ring")
{
  if (s.getName() != "ring")
    return;
  center = ((ring)s).getCenter();
  normal = ((ring)s).getNormal();
  innerR = ((ring)s).getInnerRadius();
  outerR = ((ring)s).getOuterRadius();
  thickness = ((ring)s).getThickness();
  setMaxDistance(((ring)s).getMaxDistance());
}
ring::ring(const shape_parameter& description) : volumeShape("ring")
{
  if (description.getName() != "ring")
    return;
  if (description.NumberOfParams<point3D>() < 1 || description.NumberOfParams<vector3D>() < 1 ||
      description.NumberOfParams<float>() < 3)
    return;
  center = description.getParam<point3D>(0);
  normal = description.getParam<vector3D>(0);
  normal.normalize();
  innerR = description.getParam<float>(0);
  outerR = description.getParam<float>(1) + description.getParam<float>(0);
  thickness = description.getParam<float>(2);
}

ring::~ring() {}

vector3D ring::getNormal() const { return normal; }

float ring::getInnerRadius() const { return innerR; }

float ring::getOuterRadius() const { return outerR; }

float ring::getThickness() const { return thickness; }

volumeShape* ring::getClone() { return (new ring(*this)); }

volumeShape* ring::getNext(int times, int stackType)
{
  volumeShape* sh = NULL;
  switch (stackType) {
  case 1: {
    float add = (outerR - innerR) * times;
    sh = new ring(center, normal, innerR + add, outerR + add, thickness);
    sh->setMaxDistance(getMaxDistance());
    //	cout<<"ring: "<<times<<" :
    //"<<atan((innerR+add)/center.Z())*180./M_PI<<"deg ->
    //"<<innerR+add<<endl;
    break;
  }
  case 2: {
    sh = new ring(center - normal * thickness * times, normal, innerR, outerR, thickness);
    sh->setMaxDistance(getMaxDistance());
    break;
  }
  }
  return sh;
}

volumeShape* ring::getEnvelope(int times, int stackType)
{
  volumeShape* sh = NULL;
  switch (stackType) {
  case 1: {
    float add = (outerR - innerR) * times;
    sh = new ring(center, normal, innerR, outerR + add, thickness);
    sh->setMaxDistance(getMaxDistance());
    break;
  }
  case 2: {
    sh = new ring(center, normal, innerR, outerR, thickness * times);
    sh->setMaxDistance(getMaxDistance());
    break;
  }
  }
  return sh;
}

point3D ring::entrance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return enter;
  return point3D();
}

vector3D ring::distance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  return dist;
}

sLine3D ring::Normal(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return sLine3D(enter, norm);
  return sLine3D();
}
float ring::getFlightPathInShape(const sLine3D& line)
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
#include "fiber.h"
bool ring::cut(const fiber& f)
{
  if (f.getCorner() == center)
    return false;
  return false;
}

Vector ring::HitParams(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  //  if(dist.length()<=__prec)cout<<"ring hit"<<endl;
  vector3D pitch;
  vector3D npc;
  vector3D dist_;
  float sigma = hit.getValue(9);
  //   pitch=enter-center;
  if (abs(dist * normal) < __prec) {
    if (dist.getState() != _zero_)
      pitch = dist;
    else {
      pitch = enter - center;
      pitch = pitch - normal * (pitch * normal);
      if (pitch.R() > (innerR + outerR) / 2.)
        pitch = pitch * -1;
    }
  } else
    pitch = normal;
  //   if(norm*normal<0)
  //     pitch=enter-(center-normal*(thickness*0.5));
  //   else if(norm*normal==0)
  //     pitch=pitch-normal*(pitch*normal);
  //   if(pitch.length()<(outerR-innerR)/2.0+innerR)
  //     pitch=pitch*(-1);
  pitch.normalize();
  npc = enter + dist;
  //  cout<<"ring: "<<enter<<dist<<norm<<npc<<pitch<<endl;
  //  cout<<"ring: "<<pitch<<endl;
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, enter.X());
  ret.setValue(2, enter.Y());
  ret.setValue(3, enter.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist.X());
  ret.setValue(11, dist.Y());
  ret.setValue(12, dist.Z());
  return ret;
}

Vector ring::Hitting(const sLine3D& line)
{
  Vector ret(10);
  float sigma;
  // check front / back plane
  plane3D plate;
  point3D c;
  if (normal * line.Direction() < 0) {
    plate = plane3D(center, normal);
    c = center;
  } else {
    plate = plane3D(center - normal * thickness, normal * (-1));
    c = center - normal * thickness;
  }
  point3D platePoint = plate - line;
  vector3D ctopp = platePoint - c;
  float r = ctopp.R();
  sigma = (outerR - innerR) / 2;
  if ((r <= outerR) && (r >= innerR)) {
    ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), 0, 0, 0, plate.Normal().X(),
                  plate.Normal().Y(), plate.Normal().Z(), sigma);
    return ret;
  }
  // acceleration
  vector3D dis;
  dis = platePoint - center;
  if (r > outerR)
    dis = dis * ((dis.R() - outerR) / dis.R());
  else
    dis = -1 * dis * ((dis.R() - innerR) / dis.R());
  ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), dis.X(), dis.Y(), dis.Z(),
                plate.Normal().X(), plate.Normal().Y(), plate.Normal().Z(), sigma);
  return ret;
  // end acceleration
  if (r < innerR) // check if hit from inside
  {
    float n = (c - line.Foot()) * (c - line.Foot()) - innerR * innerR;
    float m = -2 * (vector3D(c) * line.Direction() + vector3D(line.Foot()) * line.Direction());
    float o = (line.Direction() * line.Direction()) * (-1);
    float l = m * m - 4 * o * n;
    if (l > 0) {
      float t1 = -m + sqrt(m * m - 4 * o * n) / 2 / o, t2 = -m - sqrt(m * m - 4 * o * n) / 2 / o;
      point3D p;
      if (t1 < t2)
        p = line.Foot() + t2 * line.Direction();
      else
        p = line.Foot() + t1 * line.Direction();
      vector3D v = center - p;
      vector3D v1_ = v - (v * normal) * normal;
      vector3D v2_ = v - v1_;
      v1_.normalize();
      if (v2_ * normal < 0 && v2_.length() <= thickness) {
        ret.setValues(p.X(), p.Y(), p.Z(), 0, 0, 0, v1_.X(), v1_.Y(), v1_.Z(), sigma);
        return ret;
      }
    }
  } else // check if hit from outside
  {
    float n = (c - line.Foot()) * (c - line.Foot()) - outerR * outerR;
    float m = -2 * (vector3D(c) * line.Direction() + vector3D(line.Foot()) * line.Direction());
    float o = (line.Direction() * line.Direction()) * (-1);
    float l = m * m - 4 * o * n;
    if (l > 0) {
      float t1 = -m + sqrt(l) / 2 / o, t2 = -m - sqrt(l) / 2 / o;
      point3D p;
      if (t1 < t2)
        p = line.Foot() + t2 * line.Direction();
      else
        p = line.Foot() + t1 * line.Direction();
      vector3D v = center - p;
      vector3D v1_ = v - (v * normal) * normal;
      vector3D v2_ = v - v1_;
      v1_.normalize();
      if (v2_ * normal < 0 && v2_.length() <= thickness) {
        ret.setValues(p.X(), p.Y(), p.Z(), 0, 0, 0, v1_.X(), v1_.Y(), v1_.Z(), sigma);
        return ret;
      }
    }
  }
  // no hit => calculate distance
  vector3D dist1;
  if (r == 0) {
    ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), innerR * normal.X(),
                  innerR * normal.Y(), innerR * normal.Z(), 0, 0, 0, sigma);
    return ret;
  } else if (r < innerR)
    dist1 = ctopp * ((innerR - r) / r);
  else
    dist1 = ctopp * ((outerR - r) / r);
  ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), dist1.X(), dist1.Y(), dist1.Z(), 0,
                0, 0, sigma);
  return ret;
}
Vector ring::HitParams(const planeShape& shape, point3D origin)
{
  Vector hit(Hitting(sLine3D(origin, shape.getCenter() - origin)));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  //  if(dist.length()<=__prec)cout<<"ring hit"<<endl;
  vector3D pitch;
  vector3D npc;
  float sigma = hit.getValue(9);

  if (dist.length() < __prec) {
    enter.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    norm.setValues(hit.getValue(6), hit.getValue(7), hit.getValue(8));
    npc = enter + dist;
  } else {
    float dp = shape.angularRange_Phi(origin);
    float dt = shape.angularRange_Theta(origin);
    vector3D e(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    vector3D n = e + dist;
    e = e - origin;
    n = n - origin;
    float dp1 = abs(e.Phi() - n.Phi()), dt1 = abs(e.Theta() - n.Theta());
    if (dp1 < dp && dt1 < dt) {
      int numCorners = shape.getNumberOfPoints();
      point3D corners[numCorners];
      vector3D dists[numCorners];
      point3D npcs[numCorners];
      point3D enters[numCorners];
      vector3D norms[numCorners];
      bool init = false;
      for (int i = 0; i < numCorners; i++) {
        corners[i] = shape.getPoint(i);
      }
      for (int i = 0; i < numCorners; i++) {
        hit = Vector(Hitting(sLine3D(origin, corners[i] - origin)));
        dists[i].setValues(hit.getValue(3), hit.getValue(4), hit.getValue(5));
        enters[i].setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
        npcs[i] = enters[i] + dists[i];
        norms[i].setValues(hit.getValue(6), hit.getValue(7), hit.getValue(8));
        if (dists[i].length() < __prec) {
          enter = enters[i];
          npc = npcs[i];
          norm = norms[i];
          dist = dists[i];
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
          if (dist.length() > dists[i].length()) {
            dist = dists[i];
            enter = enters[i];
            norm = norms[i];
            npc = npcs[i];
          }
        }
        if (passing) {
          dist.setValues(0, 0, 0);
        }
      }
    } else {
      enter.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
      norm.setValues(hit.getValue(6), hit.getValue(7), hit.getValue(8));
      npc = enter + dist;
    }
  }

  if (abs(dist * normal) < __prec) {
    if (dist.getState() != _zero_)
      pitch = dist;
    else {
      pitch = enter - center;
      pitch = pitch - normal * (pitch * normal);
    }
  } else
    pitch = normal;

  pitch.normalize();
  npc = enter + dist;
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, enter.X());
  ret.setValue(2, enter.Y());
  ret.setValue(3, enter.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist.X());
  ret.setValue(11, dist.Y());
  ret.setValue(12, dist.Z());
  return ret;
}
#include "wedge.h"
void ring::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, TObject** ident,
                int lColor, int fColor, int fStyle) const
{
  vector3D beg;
  if (!(plane.Normal() == normal || plane.Normal() * -1 == normal))
    beg = plane.Normal() ^ normal;
  else {
    beg = vector3D(1, 0, 0);
    if (beg * normal < __prec)
      beg = vector3D(0, 1, 0);
    beg = beg - normal * (normal * beg);
  }
  beg.normalize();
  wedge w(innerR, outerR, 2 * M_PI, thickness, center, normal, beg);
  w.Draw(eye, plane, boundingBox, ident, lColor, fColor, fStyle);
}
void ring::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
int ring::suspect(const sLine3D& line, int stackType)
{
  switch (stackType) {
  case 1: {
    point3D c;
    if (normal * line.Direction() < 0)
      c = center;
    else
      c = center - normal * thickness;
    plane3D pl(c, normal);
    point3D hitp = pl - line;
    vector3D l = hitp - c;
    if (l.R() < innerR)
      return -1;
    return ((int)((l.R() - innerR) / (outerR - innerR)));
  }
  case 2: {
    plane3D front(center, normal);
    point3D hit = front - line;
    vector3D l = hit - center;
    if (l.R() <= outerR && l.R() >= innerR)
      return 0;
    sLine3D centerLine(center, normal);
    lLine3D poca = centerLine - line;
    if (poca.length() > outerR)
      return -1;
    //////ka
  }
  }
  return -1;
}
shape_parameter ring::description() const
{
  shape_parameter sh;
  sh.setName("ring");
  sh.setId(ring_id1);
  sh.addParam<point3D>(center, "center");
  sh.addParam<vector3D>(normal, "normal");
  sh.addParam<float>(innerR, "inner radius");
  sh.addParam<float>(outerR - innerR, "delta radius");
  sh.addParam<float>(thickness, "thickness");
  sh.setCompleteWrite(true);
  return sh;
}
shape_parameter ring::getDescription()
{
  shape_parameter sh;
  sh.setName("ring");
  sh.setId(ring_id1);
  sh.addParam<point3D>(point3D(), "center");
  sh.addParam<vector3D>(vector3D(), "normal");
  sh.addParam<float>(0, "inner radius");
  sh.addParam<float>(0, "delta radius");
  sh.addParam<float>(0, "thickness");
  sh.setCompleteWrite(false);
  return sh;
}
