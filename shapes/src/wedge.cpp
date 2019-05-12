#include "wedge.h"
#include "planeShape.h"
#include "provideShapes.h"
#include "ring.h"
#include "shapeparameter.h"
#include "spiral.h"

namespace {
// 2e501b2e-11f5-11e9-ab14-d663bd873d93
const boost::uuids::uuid wedge_id1 = {{0x2e, 0x50, 0x1b, 0x2e, 0x11, 0xf5, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
// 332cf48c-11f5-11e9-ab14-d663bd873d93
const boost::uuids::uuid wedge_id2 = {{0x33, 0x2c, 0xf4, 0x8c, 0x11, 0xf5, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct ShapeProvider : public FactoryShapeProvider,
                       public std::enable_shared_from_this<ShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    auto shapeCreation = [](shape_parameter const& param) -> std::shared_ptr<volumeShape> {
      if (param.NumberOfParams<float>() > 4)
        return std::shared_ptr<volumeShape>(new wedge(
            param.getParam<float>(0), param.getParam<float>(1), param.getParam<float>(2),
            param.getParam<float>(3), param.getParam<point3D>(0), param.getParam<vector3D>(0),
            param.getParam<vector3D>(1), param.getParam<float>(4)));
      return std::shared_ptr<volumeShape>(
          new wedge(param.getParam<float>(0), param.getParam<float>(1), param.getParam<float>(2),
                    param.getParam<float>(3), param.getParam<point3D>(0),
                    param.getParam<vector3D>(0), param.getParam<vector3D>(1), -1));
    };

    //! \todo there is still the wedge-cone stuff to do
    //        else if (sp.getName() == "wedge-cone") {
    //            point3D centerOfBase = sp.getParam<point3D>(0);
    //            vector3D symmetryAxis = sp.getParam<vector3D>(0);
    //            vector3D lpOnBase = sp.getParam<vector3D>(1);
    //            float baseRadius = sp.getParam<float>(0);
    //            float topRadius = sp.getParam<float>(1);
    //            float length = sp.getParam<float>(2);
    //            float dp = sp.getParam<float>(3);
    //            float thickness = sp.getParam<float>(4);
    //            float Ro, Ri, z = length * baseRadius / (baseRadius - topRadius);
    //            Ro = sqrt(z * z + baseRadius * baseRadius);
    //            Ri = Ro * topRadius / baseRadius;
    //            point3D center = centerOfBase - symmetryAxis * (z / symmetryAxis.R());
    //            vector3D lpe = symmetryAxis * (z / symmetryAxis.R()) + lpOnBase * (baseRadius /
    //            lpOnBase.R());
    //            lpOnBase = matrix3D(symmetryAxis, dp * 0.5) * lpOnBase;
    //            vector3D normal = (symmetryAxis * baseRadius - lpOnBase * z) / (Ro);
    //            lpe.normalize();
    //            normal.normalize();
    //            //	float alpha=2*asin(baseRadius/Ro*sin(dp*0.5));
    //            shape = new wedge(Ri, Ro, dp, thickness, center, normal, lpe, baseRadius);
    //          }

    auto next1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      matrix3D turn(parameter.getParam<vector3D>(0),
                    static_cast<double>(parameter.getParam<float>(2) * times));
      vector3D tmp = turn * parameter.getParam<vector3D>(1);

      auto result = parameter;
      result.setParam<vector3D>(1, tmp);

      return result;
    };
    auto next2 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      auto outerR = static_cast<double>(parameter.getParam<float>(1));
      auto distBarrel = static_cast<double>(parameter.getParam<float>(4));
      auto lpe = parameter.getParam<vector3D>(1);
      auto dphi = parameter.getParam<float>(2);
      auto normal = parameter.getParam<vector3D>(0);
      double l = sqrt(outerR * outerR - distBarrel * distBarrel);
      vector3D rot_vec = vector3D(0, 0, -1);
      vector3D lpeBase = lpe - rot_vec * (lpe * rot_vec);
      lpeBase.normalize();
      double angle = 2 * atan(l / outerR * tan(dphi / 2.));
      rot_vec.normalize();
      matrix3D rotm(rot_vec, angle * times);
      lpeBase = rotm * lpeBase;
      vector3D lpeNew =
          distBarrel * lpeBase + rot_vec * sqrt(outerR * outerR - distBarrel * distBarrel);
      lpeNew.normalize();

      result.setParam<vector3D>(0, rotm * normal);
      result.setParam<vector3D>(1, lpeNew);
      return result;
    };
    auto envelope1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      double totalAngle = static_cast<double>(parameter.getParam<float>(2)) * times;
      if (totalAngle - M_PI * 2 < 0) {
        auto result = parameter;
        result.setParam<float>(2, static_cast<float>(totalAngle));
        return result;
      }
      auto result = ring::getDescription();
      result.setParam<point3D>(0, parameter.getParam<point3D>(0));
      result.setParam<vector3D>(0, parameter.getParam<vector3D>(0));
      result.setParam<float>(0, parameter.getParam<float>(0));
      result.setParam<float>(1, parameter.getParam<float>(1) - parameter.getParam<float>(0));
      result.setParam<float>(2, parameter.getParam<float>(3));
      return result;
    };
    auto envelope2 = [](shape_parameter const&, size_t) -> shape_parameter {
      // no shape defined yet for this use-case.
      return {};
    };

    auto description = wedge::getDescription();
    description.setId(wedge_id1);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next1, envelope1);
    description.setId(wedge_id2);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next2, envelope2);
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(wedge_id1);
    factory.removeShapeFromFactory(wedge_id2);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
};
volatile static std::shared_ptr<ShapeProvider> prov = [] {
  auto r = std::make_shared<ShapeProvider>();
  r->install();
  return r;
}();
}

wedge::wedge(float i, float a, float d, float t, point3D c, vector3D n, vector3D l, float dis)
    : volumeShape("wedge")
{
  innerR = i;
  outerR = a;
  dphi = d;
  thickness = t;
  center = c;
  normal = n;
  normal.normalize();
  lpe = l;
  lpe.normalize();
  distBarrel = dis;
  hpe = matrix3D(normal, dphi) * lpe;
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    vector3D rot_vec =
        lpe * (outerR - distBarrel * distBarrel / l) + outerR * distBarrel / l * normal;
    double angle = 2 * atan(l / outerR * tan(dphi / 2.));
    rot_vec.normalize();
    matrix3D rotm(rot_vec, angle);
    hpe = rotm * lpe;
  }
  res = hpe;
  hpe.normalize();
  res = normal ^ res;
  res.normalize();
}
wedge::wedge(const shape_parameter& description) : volumeShape("wedge")
{
  if (description.getName() != "wedge")
    return;
  if (description.NumberOfParams<point3D>() < 1 || description.NumberOfParams<vector3D>() < 2 ||
      description.NumberOfParams<float>() < 5)
    return;
  innerR = description.getParam<float>(0);
  outerR = description.getParam<float>(1);
  distBarrel = description.getParam<float>(4);
  dphi = description.getParam<float>(2);
  thickness = description.getParam<float>(3);
  center = description.getParam<point3D>(0);
  normal = description.getParam<vector3D>(0);
  normal.normalize();
  lpe = description.getParam<vector3D>(1);
  hpe = matrix3D(normal, dphi) * lpe; //-
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    vector3D rot_vec =
        lpe * (outerR - distBarrel * distBarrel / l) + outerR * distBarrel / l * normal;
    double angle = 2 * atan(l / outerR * tan(dphi / 2.));
    rot_vec.normalize();
    matrix3D rotm(rot_vec, angle);
    hpe = rotm * lpe;
  }
  res = (hpe + lpe) * 0.5;
  hpe.normalize();
  res = normal ^ res;
  res.normalize();
}
wedge::wedge(const wedge& s) : volumeShape("wedge")
{
  innerR = s.getInnerRadius();
  outerR = s.getOuterRadius();
  dphi = s.getPhiRange();
  thickness = s.getThickness();
  lpe = s.getLowerPhiEdge();
  normal = s.getNormal();
  distBarrel = s.getDistToBarrelCenter();
  setMaxDistance(s.getMaxDistance());
  lpe.normalize();
  center = s.getCenter();
  hpe = matrix3D(normal, dphi) * lpe; //-
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    vector3D rot_vec =
        lpe * (outerR - distBarrel * distBarrel / l) + outerR * distBarrel / l * normal;
    double angle = 2 * atan(l / outerR * tan(dphi / 2.));
    rot_vec.normalize();
    matrix3D rotm(rot_vec, angle);
    hpe = rotm * lpe;
  }
  res = (hpe + lpe) * 0.5;
  hpe.normalize();
  res = normal ^ res;
  res.normalize();
}
wedge::wedge(const volumeShape& s) : volumeShape("wedge")
{
  if ((s.getName() != "spiral") && (s.getName() != "circular") && (s.getName() != "wedge"))
    return;
  if (s.getName() == "spiral") {
    innerR = ((spiral)s).getInnerRadius();
    outerR = ((spiral)s).getOuterRadius();
    dphi = ((spiral)s).getPhiRange();
    thickness = ((spiral)s).getThickness();
    lpe = ((spiral)s).getLowerPhiEdge();
    normal = ((spiral)s).getNormal();
    distBarrel = -1;
  } else if (s.getName() == "wedge") {
    innerR = ((wedge)s).getInnerRadius();
    outerR = ((wedge)s).getOuterRadius();
    dphi = ((wedge)s).getPhiRange();
    thickness = ((wedge)s).getThickness();
    lpe = ((wedge)s).getLowerPhiEdge();
    normal = ((wedge)s).getNormal();
    distBarrel = ((wedge)s).getDistToBarrelCenter();
  } else {
    // 	innerR=s.getInnerRadius();
    // 	outerR=s.getOuterRadius();
    // 	thickness=s.getThickness();
    // 	lpe=vector3D(1,0,0);
    // 	normal=s.getNormal();
    distBarrel = -1;
  }
  setMaxDistance(s.getMaxDistance());
  lpe.normalize();
  center = s.getCenter();
  hpe = matrix3D(normal, dphi) * lpe; //-
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    vector3D rot_vec =
        lpe * (outerR - distBarrel * distBarrel / l) + outerR * distBarrel / l * normal;
    double angle = 2 * atan(l / outerR * tan(dphi / 2.));
    rot_vec.normalize();
    matrix3D rotm(rot_vec, angle);
    hpe = rotm * lpe;
  }
  res = (hpe + lpe) * 0.5;
  hpe.normalize();
  res = normal ^ res;
  res.normalize();
}

void wedge::operator=(volumeShape* s)
{
  if ((s->getName() != "spiral") && (s->getName() != "circular") && (s->getName() != "wedge"))
    return;
  if (s->getName() == "spiral") {
    innerR = ((spiral*)s)->getInnerRadius();
    outerR = ((spiral*)s)->getOuterRadius();
    dphi = ((spiral*)s)->getPhiRange();
    thickness = ((spiral*)s)->getThickness();
    lpe = ((spiral*)s)->getLowerPhiEdge();
    normal = ((spiral*)s)->getNormal();
    distBarrel = -1;
  } else if (s->getName() == "wedge") {
    innerR = ((wedge*)s)->getInnerRadius();
    outerR = ((wedge*)s)->getOuterRadius();
    dphi = ((wedge*)s)->getPhiRange();
    thickness = ((wedge*)s)->getThickness();
    lpe = ((wedge*)s)->getLowerPhiEdge();
    normal = ((wedge*)s)->getNormal();
    distBarrel = ((wedge*)s)->getDistToBarrelCenter();
  } else {
    // 	innerR=(s)->getInnerRadius();
    // 	outerR=(s)->getOuterRadius();
    // 	thickness=(s)->getThickness();
    // 	normal=(s)->getNormal();
    distBarrel = -1;
  }
  setMaxDistance(s->getMaxDistance());
  lpe.normalize();
  center = s->getCenter();
  hpe = matrix3D(normal, dphi) * lpe; //-
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    double Calpha = l / outerR;
    double dp = asin(Calpha * tan(dphi / 2.)) * 2.;
    matrix3D t1(normal, dphi / 2.);
    vector3D axis = lpe;
    axis = axis * (Calpha / axis.R()) + normal * (sin(acos(Calpha)));
    matrix3D turn(axis, dp);
    hpe = turn * lpe;
  }
  res = (hpe + lpe) * 0.5;
  hpe.normalize();
  res = normal ^ res;
  res.normalize();
}

vector3D wedge::distance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  return dist;
}
float wedge::getFlightPathInShape(const sLine3D& line)
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
#include "ring.h"
volumeShape* wedge::getEnvelope(int times, int stackType)
{
  switch (stackType) {
  case 1: {
    if ((abs(times * dphi) - M_PI * 2) < -0.1 * M_PI) {
      wedge* ret = new wedge(*this);
      ret->setPhiRange(times * dphi);
      return ret;
    }
    ring* ret = new ring(center, normal, innerR, outerR, thickness);
    return ret;
  }
  case 4: {
    // no appropriate shape exists yet for this task!
    // 	double alpha=asin(distBarrel/outerR);
    // 	double l=sqrt(outerR*outerR-distBarrel*distBarrel);
    // 	double cosP=dphi;
    // 	cosP=cos(cosP);
    // 	double dp=acos(cosP*(l*l+2*l*cos(alpha)+1)-2*l*cos(alpha)-l*l);
    // 	//turn lpe to center of the element
    // 	vector3D lp=matrix3D(normal,dphi/2.0)*lpe;//-
    // 	//turn vectors to cone base
    // 	vector3D tv=normal^lp;
    // 	tv.normalize();
    // 	matrix3D torr(tv,-(M_PI*0.5-alpha));//-
    // 	vector3D lp1=torr*lp;
    // 	vector3D nor=torr*normal;
    // 	//turn vectors in cone base
    // 	matrix3D turn(nor,dp*times);//-
    // 	vector3D lp2=turn*lp1;
    // 	//turn vectors to cone mantle
    // 	tv=lp2^nor;
    // 	tv.normalize();
    // 	matrix3D torr1(tv,-(M_PI*0.5-alpha));//-
    // 	vector3D lp3=torr1*lp2;
    // 	vector3D nor1=torr1*nor;
    // 	//turn vector back to edge of element
    // 	lp3=matrix3D(nor1,-dphi/2.0)*lp3;//-

    // 	float cosa=cos(asin(distBarrel/outerR));
    // 	vector3D front=lpe*(outerR*cosa)+normal*(distBarrel/cosa);
    // 	//	float alpha=atan(outerR*sin(dphi/2.0)/distBarrel);
    // 	matrix3D turn1(front,-alpha);//-
    // 	wedge* ret=new wedge(*this);
    // 	ret->setNormal(turn1*normal);
    // 	ret->setLowerPhiEdge(turn1*lpe);
    // 	ret->setNormal(nor1);
    // 	ret->setLowerPhiEdge(lp3);
    return getClone();
  }
  }
  return getClone();
}
sLine3D wedge::Normal(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return sLine3D(enter, norm);
  return sLine3D();
}
#include "fiber.h"
bool wedge::cut(const fiber& f)
{
  if ((f.getCorner() - center).R() < 0)
    return false;
  return false;
}
Vector wedge::HitParams(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  vector3D npc = enter + dist;
  vector3D atm;
  atm = sLine3D(center, matrix3D(normal, dphi * 0.5) * lpe) - enter; //-
  float sigma = hit.getValue(9);
  if (atm.length() > 0.1)
    sigma = sigma * atm.length();
  atm = res;
  if (((enter - center) ^ (matrix3D(normal, dphi / 2) * lpe)) * normal > 0)
    atm = atm * (-1); //-
  atm.normalize();
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, enter.X());
  ret.setValue(2, enter.Y());
  ret.setValue(3, enter.Z());
  ret.setValue(4, atm.X());
  ret.setValue(5, atm.Y());
  ret.setValue(6, atm.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist.X());
  ret.setValue(11, dist.Y());
  ret.setValue(12, dist.Z());
  return ret;
}
point3D wedge::entrance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  return enter;
}

wedge::~wedge() {}

float wedge::getInnerRadius() const { return innerR; }

float wedge::getOuterRadius() const { return outerR; }

float wedge::getPhiRange() const { return dphi; }

float wedge::getThickness() const { return thickness; }

vector3D wedge::getNormal() const { return normal; }

vector3D wedge::getLowerPhiEdge() const { return lpe; }
vector3D wedge::getHigherPhiEdge() const { return hpe; }

float wedge::getDistToBarrelCenter() const { return distBarrel; }

void wedge::setDistToBarrelCenter(float v) { distBarrel = v; }

void wedge::setInnerRadius(float v) { innerR = (v < 0 ? -v : v); }

void wedge::setOuterRadius(float v) { outerR = (v < 0 ? -v : v); }

void wedge::setPhiRange(float v)
{
  dphi = v;
  res = matrix3D(normal, dphi / 2) * lpe; //-
  res = normal ^ res;
  res.normalize();
  hpe = matrix3D(normal, -dphi) * lpe;
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    double Calpha = l / outerR;
    double dp = asin(Calpha * tan(dphi / 2.)) * 2.;
    matrix3D t1(normal, dphi / 2.);
    vector3D axis = lpe;
    axis = axis * (Calpha / axis.R()) + normal * (sin(acos(Calpha)));
    matrix3D turn(axis, dp);
    hpe = turn * lpe;
  }
}

void wedge::setThickness(float v) { thickness = (v < 0 ? -v : v); }

void wedge::setCenter(point3D c) { center = c; }

void wedge::setNormal(vector3D v)
{
  normal = v;
  normal.normalize();
  res = matrix3D(normal, dphi / 2) * lpe; //-
  res = normal ^ res;
  res.normalize();
  hpe = matrix3D(normal, -dphi) * lpe;
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    double Calpha = l / outerR;
    double dp = asin(Calpha * tan(dphi / 2.)) * 2.;
    matrix3D t1(normal, dphi / 2.);
    vector3D axis = lpe;
    axis = axis * (Calpha / axis.R()) + normal * (sin(acos(Calpha)));
    matrix3D turn(axis, dp);
    hpe = turn * lpe;
  }
}

void wedge::setLowerPhiEdge(vector3D v)
{
  lpe = v;
  res = matrix3D(normal, dphi / 2) * lpe; //-
  res = normal ^ res;
  res.normalize();
  hpe = matrix3D(normal, -dphi) * lpe;
  if (distBarrel > 0) {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    double Calpha = l / outerR;
    double dp = asin(Calpha * tan(dphi / 2.)) * 2.;
    matrix3D t1(normal, dphi / 2.);
    vector3D axis = lpe;
    axis = axis * (Calpha / axis.R()) + normal * (sin(acos(Calpha)));
    matrix3D turn(axis, dp);
    hpe = turn * lpe;
  }
}

volumeShape* wedge::getNext(int times, int stackType)
{
  //  if(stackType==2) return(new wedge(*this));
  wedge* ret = new wedge(*this);
  switch (stackType) {
  case 1: {
    matrix3D turn(normal, dphi * times); //-
    vector3D tmp = lpe;
    tmp = turn * tmp;
    ret->setLowerPhiEdge(tmp);
    ret->setMaxDistance(getMaxDistance());
    return ret;
  }
  case 4: {
    double l = sqrt(outerR * outerR - distBarrel * distBarrel);
    vector3D rot_vec =
        vector3D(0, 0,
                 -1); // lpe*(outerR-distBarrel*distBarrel/l)+outerR*distBarrel/l*normal;
    vector3D lpeBase = lpe - rot_vec * (lpe * rot_vec);
    lpeBase.normalize();
    double angle = 2 * atan(l / outerR * tan(dphi / 2.));
    rot_vec.normalize();
    matrix3D rotm(rot_vec, angle * times);
    lpeBase = rotm * lpeBase;
    vector3D lpeNew =
        distBarrel * lpeBase + rot_vec * sqrt(outerR * outerR - distBarrel * distBarrel);
    lpeNew.normalize();
    ret->setNormal(rotm * normal); // turn*normal);
    ret->setLowerPhiEdge(lpeNew);  // turn*lpe);
    ret->setMaxDistance(getMaxDistance());
    return ret;
  }
  }
  return getClone();
}
volumeShape* wedge::getClone()
{
  wedge* sh = new wedge(*this);
  sh->setMaxDistance(getMaxDistance());
  return sh;
}

#include "sphericRectangle.h"
Vector wedge::Hitting(const sLine3D& line)
{
  Vector ret(10);
  float sigma;
  vector3D v1_, v2_;
  if (distBarrel <= 0)
    sigma = fabs(0.5 * dphi);
  else {
    sigma = fabs(M_PI / 96.); // arrg cannot reproduce!!!
    double alpha = asin(distBarrel / outerR);
    double tana2 = tan(alpha) * tan(alpha);
    double ca = cos(alpha);
    vector3D v = line.Direction();
    point3D f = line.Foot();
    point3D c = center;
    double a = v[2] * v[2] - (v[1] * v[1] + v[0] * v[0]) / tana2;
    double b = 2 * (v[0] * (c[0] - f[0]) + v[1] * (c[1] - f[1])) / tana2 - 2 * v[2] * (c[2] - f[2]);
    double cc = (c[2] - f[2]) * (c[2] - f[2]) -
                (((c[0] - f[0]) * (c[0] - f[0])) + ((c[1] - f[1]) * (c[1] - f[1]))) / tana2;
    vector3D d(0, 0, 0);
    vector3D d1, d2, d3, d4;
    if (b * b - 4 * a * cc < 0) {
      ret.setValues(0, 0, 0, 10000, 10000, 10000, normal.X(), normal.Y(), normal.Z(), 10000);
      return ret;
    } else //(b*b-4*a*cc>=0)
    {
      double t[2] = {(-b + sqrt(b * b - 4 * a * cc)) / 2 / a,
                     (-b - sqrt(b * b - 4 * a * cc)) / 2 / a};
      double z[2] = {c[2] - f[2] - t[0] * v[2], c[2] - f[2] - t[1] * v[2]};
      double ph[2] = {acos((c[0] - f[0] - t[0] * v[0]) / z[0] / tan(alpha)),
                      acos((c[0] - f[0] - t[1] * v[0]) / z[1] / tan(alpha))};
      point3D p[2] = {f + t[0] * v, f + t[1] * v};
      int outInd = 0;
      if (t[0] < 0 && t[1] < 0) {
        if ((ph[0] >= lpe.Phi() && ph[0] <= hpe.Phi() && z[0] <= outerR * ca &&
             z[0] >= innerR * ca) ||
            (ph[1] >= lpe.Phi() && ph[1] <= hpe.Phi() && z[1] <= outerR * ca &&
             z[1] >= innerR * ca)) {
          if (ph[0] >= lpe.Phi() && ph[0] <= hpe.Phi())
            outInd = 0;
          else if (ph[1] >= lpe.Phi() && ph[1] <= hpe.Phi())
            outInd = 1;
        } else {
          lLine3D ll1(center + lpe * innerR, center + lpe * outerR);
          lLine3D ll2(center + hpe * innerR, center + hpe * outerR);
          d1 = ll1 - p[0];
          d2 = ll2 - p[0];
          d3 = ll1 - p[1];
          d4 = ll2 - p[1];
          if (d1 * d2 < 0 || d3 * d4 < 0) {
            if (d1 * d2 < 0)
              outInd = 0;
            else {
              d1 = d3;
              d2 = d4;
              outInd = 1;
            }
          } else {
            if (d1.R() < d2.R() && d1.R() < d3.R() && d1.R() < d4.R()) {
              outInd = 0;
              d = d1;
            } else if (d2.R() < d1.R() && d2.R() < d3.R() && d2.R() < d4.R()) {
              outInd = 0;
              d = d2;
            } else if (d3.R() < d2.R() && d3.R() < d1.R() && d3.R() < d4.R()) {
              outInd = 1;
              d = d3;
            } else {
              outInd = 1;
              d = d4;
            }
          }
        }
        ret.setValues(p[outInd].X(), p[outInd].Y(), p[outInd].Z(), d.X(), d.Y(), d.Z(), normal.X(),
                      normal.Y(), normal.Z(), sigma * (center - p[outInd]).R());
        return ret;
      }
      if (t[0] < 0 || t[1] < 0) {
        if (t[0] < 0)
          outInd = 1;
        else
          outInd = 0;
        outInd = 0;
        if (z[outInd] < outerR * ca && z[outInd] > innerR * ca) {
          if (ph[outInd] < lpe.Phi() || ph[outInd] > hpe.Phi()) {
            sLine3D ll1(center, lpe);
            sLine3D ll2(center, hpe);
            d1 = ll1 - p[outInd];
            d2 = ll2 - p[outInd];
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          }
        } else {
          if (z[outInd] > outerR * ca) {
            point3D l1 = center + lpe * outerR, l2 = center + hpe * outerR;
            d1 = p[outInd] - l1;
            d1 = p[outInd] - l2;
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          } else {
            point3D l1 = center + lpe * innerR, l2 = center + hpe * innerR;
            d1 = p[outInd] - l1;
            d2 = p[outInd] - l2;
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          }
        }
        ret.setValues(p[outInd].X(), p[outInd].Y(), p[outInd].Z(), d.X(), d.Y(), d.Z(), normal.X(),
                      normal.Y(), normal.Z(), sigma * (center - p[outInd]).R());
        return ret;
      } else {
        if (t[0] < t[1])
          outInd = 0;
        else
          outInd = 1;
        if (z[outInd] < outerR * ca && z[outInd] > innerR * ca) {
          if (ph[outInd] < lpe.Phi() || ph[outInd] > hpe.Phi()) {
            sLine3D ll1(center, lpe);
            sLine3D ll2(center, hpe);
            d1 = ll1 - p[outInd];
            d2 = ll2 - p[outInd];
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          }
        } else {
          if (z[outInd] > outerR * ca) {
            point3D l1 = center + lpe * outerR, l2 = center + hpe * outerR;
            d1 = p[outInd] - l1;
            d2 = p[outInd] - l2;
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          } else {
            point3D l1 = center + lpe * innerR, l2 = center + hpe * innerR;
            d1 = p[outInd] - l1;
            d2 = p[outInd] - l2;
            if (d1 * d2 > 0) {
              if (d1.R() < d2.R())
                d = d1;
              else
                d = d2;
            }
          }
        }
        ret.setValues(p[outInd].X(), p[outInd].Y(), p[outInd].Z(), d.X(), d.Y(), d.Z(), normal.X(),
                      normal.Y(), normal.Z(), sigma * (center - p[outInd]).R());
        return ret;
      }
      if (z[0] > outerR * ca && z[1] > outerR * ca) {
        ret.setValues(p[0].X(), p[0].Y(), p[0].Z(), 0, 0, 0, normal.X(), normal.Y(), normal.Z(),
                      sigma * (center - p[0]).R());
        return ret;
      }
    }
  }

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
  //  cout<<"wedge::Hitting()"<<endl;
  vector3D ctopp;
  float pl = 0, ph = dphi, pph, ddphi = dphi;
  float r = innerR;
  if (!(platePoint.getState() == _undefined_)) {
    //      if(distBarrel>0)
    {
      v1_ = sLine3D(c, lpe) - platePoint;
      v2_ = sLine3D(c, hpe) - platePoint;
    }
    ctopp = platePoint - c;
    r = ctopp.length();
    pl = acos(ctopp * lpe / r);
    ph = acos(ctopp * hpe / r);
    pph = normal.angle(lpe, ctopp);
    if (distBarrel > 0) {
      //	  cout<<"wedge::Hitting "<<pph<<" "
      //	      <<ddphi<<" "<<r<<" "
      //	      <<acos(sqrt(outerR*outerR-distBarrel*distBarrel)/outerR)<<endl;
    }
    if (distBarrel <= 0) {
      if ((r <= outerR) && (r >= innerR) && pph <= ddphi && pph >= 0) //(pl<=dphi)&&(ph<=dphi))
      {
        ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), 0, 0, 0, plate.Normal().X(),
                      plate.Normal().Y(), plate.Normal().Z(), sigma * (center - platePoint).R());
        return ret;
      }
    } else {
      if ((r <= outerR) && (r >= innerR) && pph <= ddphi && pph >= 0) //(v1_*v2_<=0))
      {
        ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), 0, 0, 0, plate.Normal().X(),
                      plate.Normal().Y(), plate.Normal().Z(), sigma * platePoint.Rho());
        //	      cout<<ret<<endl;
        return ret;
      }
    }
  }
  // acceleration
  vector3D dis, dis1;
  dis = lLine3D(center + (lpe * innerR), center + (lpe * outerR)) - platePoint;
  dis1 = lLine3D(center + (hpe * innerR), center + (hpe * outerR)) - platePoint;
  //  cout<<center<<lpe<<center+(lpe*innerR)<<center+(lpe*outerR)<<dis<<dis1<<endl;
  if (dis.R() > dis1.R())
    dis = dis1;
  ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), dis.X(), dis.Y(), dis.Z(),
                plate.Normal().X(), plate.Normal().Y(), plate.Normal().Z(),
                sigma * (center - platePoint).R());
  return ret;
  // end acceleration
  // check side planes
  vector3D ns1 = normal ^ lpe;
  ns1.normalize();
  plane3D side;
  float d = 0;
  vector3D dir1;
  float t = 0;
  vector3D v1;
  vector3D v2;
  if (ns1 * line.Direction() < 0) {
    side = plane3D(center, ns1);
    dir1 = lpe;
  } else {
    side = plane3D(center, matrix3D(normal, dphi) * ns1); //-
    dir1 = hpe;
  }
  point3D sidePoint = side - line;
  if (sidePoint.getState() != _undefined_) {
    vector3D cTsp = sidePoint - center;
    v1 = dir1 * (dir1 * cTsp);
    v2 = cTsp - v1;
    d = v1.length();
    t = v2.length();
    if (d <= outerR && d >= innerR && v2 * normal < 0 && t <= thickness) {
      ret.setValues(sidePoint.X(), sidePoint.Y(), sidePoint.Z(), 0, 0, 0, side.Normal().X(),
                    side.Normal().Y(), side.Normal().Z(), 0.5 * thickness);
      return ret;
    }
  }
  // check if hit on curved areas
  if (((distBarrel <= 0) && (pl <= dphi) && (ph <= dphi)) ||
      ((distBarrel > 0) && (v1_ * v2_ <= 0))) {
    if (r < innerR) // check if hit from inside
    {
      float n = (center - line.Foot()) * (center - line.Foot()) - innerR * innerR;
      float m =
          -2 * (vector3D(center) * line.Direction() + vector3D(line.Foot()) * line.Direction());
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
        vector3D v1_a = v - (v * normal) * normal;
        vector3D v2_a = v - v1_a;
        v1_a.normalize();
        if (v2_a * normal < 0 && v2_a.length() <= thickness &&
            (acos(v1_a * lpe) < dphi && (acos(v1_a * hpe) < dphi))) {
          ret.setValues(p.X(), p.Y(), p.Z(), 0, 0, 0, v1_a.X(), v1_a.Y(), v1_a.Z(),
                        0.5 * thickness);
          return ret;
        }
      }
    } else if (r > outerR) // check if hit from outside
    {
      float n = (center - line.Foot()) * (center - line.Foot()) - outerR * outerR;
      float m =
          -2 * (vector3D(center) * line.Direction() + vector3D(line.Foot()) * line.Direction());
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
        vector3D v1_a = v - (v * normal) * normal;
        vector3D v2_a = v - v1_a;
        v1_a.normalize();
        if (v2_a * normal < 0 && v2_a.length() <= thickness &&
            (acos(v1_a * lpe) < dphi && (acos(v1_a * hpe) < dphi))) {
          ret.setValues(p.X(), p.Y(), p.Z(), 0, 0, 0, v1_a.X(), v1_a.Y(), v1_a.Z(),
                        0.5 * thickness);
          return ret;
        }
      }
    }
  }
  // no hit => calculate distance
  // distance to front / back plane shape
  vector3D dist1;
  if (platePoint.getState() == _undefined_)
    dist1 = vector3D();
  else {
    if (((distBarrel <= 0) && (pl <= dphi) && (ph <= dphi)) ||
        ((distBarrel > 0) && (v1_ * v2_ <= 0))) {
      if (r < innerR)
        dist1 = ctopp * ((innerR - r) / r);
      else
        dist1 = ctopp * ((outerR - r) / r);
    } else {
      if (pl > ph)
        dist1 = lLine3D(center + hpe * innerR, center + hpe * outerR) - platePoint;
      else
        dist1 = lLine3D(center + lpe * innerR, center + lpe * outerR) - platePoint;
    }
  }
  // distance to side plane shape
  vector3D dist2;
  if (sidePoint.getState() == _undefined_) {
    dist2 = vector3D();
  } else {
    if (d <= outerR && d >= innerR) {
      if (t <= thickness)
        dist2 = lLine3D(center + dir1 * innerR, center + dir1 * outerR) - sidePoint;
      else
        dist2 = lLine3D(center + dir1 * innerR - normal * thickness,
                        center + dir1 * outerR - normal * thickness) -
                sidePoint;
    } else {
      if (v2 * normal > 0) {
        if (d > outerR)
          dist2 = center + dir1 * outerR - sidePoint;
        else
          dist2 = center + dir1 * innerR - sidePoint;
      } else if (t > thickness) {
        if (d > outerR)
          dist2 = center + dir1 * outerR - normal * thickness - sidePoint;
        else
          dist2 = center + dir1 * innerR - normal * thickness - sidePoint;
      } else {
        if (d > outerR)
          dist2 = v1 - dir1 * outerR;
        else
          dist2 = v1 - dir1 * innerR;
      }
    }
  }
  if ((dist1.length() < dist2.length() && (!(platePoint.getState() == _undefined_))) ||
      (sidePoint.getState() == _undefined_)) {
    ret.setValues(platePoint.X(), platePoint.Y(), platePoint.Z(), dist1.X(), dist1.Y(), dist1.Z(),
                  0, 0, 0, sigma);
  } else {
    ret.setValues(sidePoint.X(), sidePoint.Y(), sidePoint.Z(), dist2.X(), dist2.Y(), dist2.Z(), 0,
                  0, 0, 0.5 * thickness);
  }
  return ret;
}
Vector wedge::HitParams(const planeShape& shape, point3D origin)
{
  sLine3D cline(origin, shape.getCenter() - origin);
  Vector hit(Hitting(cline));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  point3D enter;
  vector3D norm;
  point3D npc;
  vector3D atm(res);
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
        sLine3D lin(origin, corners[i] - origin);
        Vector hit1(Hitting(lin));
        dists[i].setValues(hit1.getValue(3), hit1.getValue(4), hit1.getValue(5));
        enters[i].setValues(hit1.getValue(0), hit1.getValue(1), hit1.getValue(2));
        npcs[i] = enters[i] + dists[i];
        norms[i].setValues(hit1.getValue(6), hit1.getValue(7), hit1.getValue(8));
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
  if (((enter - center) ^ (matrix3D(normal, dphi / 2) * lpe)) * normal > 0)
    atm = atm * (-1); //-

  atm.normalize();
  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, enter.X());
  ret.setValue(2, enter.Y());
  ret.setValue(3, enter.Z());
  ret.setValue(4, atm.X());
  ret.setValue(5, atm.Y());
  ret.setValue(6, atm.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist.X());
  ret.setValue(11, dist.Y());
  ret.setValue(12, dist.Z());
  return ret;
}
#include "quadrangle.h"
void wedge::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                 int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
#include "sphericRectangle.h"

void wedge::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, TObject** ident,
                 int lColor, int fColor, int fStyle) const
{
  /*  if(distBarrel>0)
      {
          double l=sqrt(outerR*outerR-distBarrel*distBarrel);
          vector3D
    rot_vec=lpe*(outerR-distBarrel*distBarrel/l)+outerR*distBarrel/l*normal;
          double angle=2*atan(l/outerR*tan(dphi/2.));
          rot_vec.normalize();
          matrix3D rotm(rot_vec,angle);
          sphericRectangle plate(center+lpe*innerR,center+lpe*outerR,
                               center+rotm*lpe*outerR,center+rotm*lpe*innerR);
        plate.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
        return;
      }
    vector3D s1,s2;
    s1=lpe^normal;
    s2=normal^hpe;
    if(plane.Normal()*s1<0)
      {
        quadrangle g(center,
                    center+(lpe*innerR),
                    center+(normal*(-thickness)+(lpe*innerR)),
                    center+(normal*(-thickness)+(lpe*outerR)),
                    center+(lpe*outerR));
        g.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
      }
    if(plane.Normal()*s2<0)
      {
        quadrangle g(center,
                    center+(hpe*innerR),
                    center+(normal*(-thickness)+(hpe*innerR)),
                    center+(normal*(-thickness)+(hpe*outerR)),
                    center+(hpe*outerR));
        g.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
      }

    int numPoints=200;
    point3D ps[numPoints];
    point2D dp[numPoints];
    Double_t pox[numPoints+1],poy[numPoints+1];
    matrix3D turn(normal,dphi/(((double)numPoints)/2.-1));//-
    point3D c=center;
    if(normal*plane.Normal()>0)c=c+normal*(-thickness);
    vector3D tmp=lpe;
    for(int i=0;i<numPoints/2;i++)
      {
        ps[i]=c+tmp*innerR;
        ps[numPoints-i-1]=c+tmp*outerR;
        tmp=turn*tmp;
      }
    for(int i=0;i<numPoints;i++)
      {
        dp[i]=projector::project(ps[i]);
        pox[i]=dp[i].X();
        poy[i]=dp[i].Y();
      }
    pox[numPoints]=dp[0].X();
    poy[numPoints]=dp[0].Y();
    if(boundingBox!=NULL)
      {
        boundingBox->setValues(pox[0],poy[0],pox[0],poy[0]);
        for(int i=1;i<numPoints;i++)
          {
            if(boundingBox->X()>pox[i])boundingBox->setValue(0,pox[i]);
            if(boundingBox->Y()>poy[i])boundingBox->setValue(1,poy[i]);
            if(boundingBox->Z()<pox[i])boundingBox->setValue(2,pox[i]);
            if(boundingBox->W()<poy[i])boundingBox->setValue(3,poy[i]);
          }
      }
    TPolyLine pl(numPoints+1,pox,poy);
    pl.SetFillColor(fColor);
    pl.SetLineColor(lColor);
    pl.SetFillStyle(fStyle);
    pl.DrawPolyLine(numPoints+1,pox,poy,"f");
    if(ident!=NULL)(*ident)=pl.Clone();
    return;
    if(lpe*(center-eye)<0)//outside
      {
        for(int i=0;i<numPoints/2;i++)
          {
            ps[i]=ps[numPoints-i-1]+normal*(-thickness);
            dp[i]=projector::project(ps[i]);
            pox[i]=dp[i].X();
            poy[i]=dp[i].Y();
          }
      }
    else
      {
        for(int i=0;i<numPoints/2;i++)
          {
            ps[numPoints-i-1]=ps[i]+normal*(-thickness);
            dp[numPoints-i-1]=projector::project(ps[numPoints-i-1]);
            pox[numPoints-i-1]=dp[numPoints-i-1].X();
            poy[numPoints-i-1]=dp[numPoints-i-1].Y();
          }
      }
    pox[numPoints]=dp[0].X();
    poy[numPoints]=dp[0].Y();
    TPolyLine pl2(numPoints+1,pox,poy);
    pl2.SetFillColor(fColor);
    pl2.SetLineColor(lColor);
    pl2.SetFillStyle(fStyle);
    */
}
int wedge::suspect(const sLine3D& line, int stackType)
{
  switch (stackType) {
  case 1: // circular
  {
    plane3D ps;
    if (line.Direction() * normal < 0)
      ps = plane3D(center, normal);
    else
      ps = plane3D(center - normal * thickness, -1 * normal);
    point3D hit = ps - line;
    vector3D len = hit - center;
    if (len.R() > outerR || len.R() < innerR)
      return -1;
    float a;
    a = normal.angle(lpe, len);
    // 	cout<<line.Direction();
    // 	cout<<a<<endl;
    if (a / dphi > 0)
      return ((int)(a / dphi));
    return ((int)(2 * M_PI / abs(dphi) + a / dphi));
  }
  case 4: // cone
  {
    double alpha = asin(distBarrel / outerR);
    double tana2 = tan(alpha) * tan(alpha);
    double ca = cos(alpha);
    vector3D v = line.Direction();
    point3D f = line.Foot();
    point3D c = center;
    double a = v[2] * v[2] - (v[1] * v[1] + v[0] * v[0]) / tana2;
    double b = 2 * (v[0] * (c[0] - f[0]) + v[1] * (c[1] - f[1])) / tana2 - 2 * v[2] * (c[2] - f[2]);
    double cc = (c[2] - f[2]) * (c[2] - f[2]) -
                (((c[0] - f[0]) * (c[0] - f[0])) + ((c[1] - f[1]) * (c[1] - f[1]))) / tana2;
    vector3D d(0, 0, 0);
    // 	cout<<line<<b*b-4*a*cc<<endl;
    if (b * b - 4 * a * cc < 0) {
      return -1;
    } else //(b*b-4*a*cc>=0)
    {
      double t[2] = {(-b + sqrt(b * b - 4 * a * cc)) / 2 / a,
                     (-b - sqrt(b * b - 4 * a * cc)) / 2 / a};
      double z[2] = {c[2] - f[2] - t[0] * v[2], c[2] - f[2] - t[1] * v[2]};
      double ph[2] = {(f + t[0] * v).Phi() // acos((c[0]-f[0]-t[0]*v[0])/z[0]/tan(alpha))
                      ,
                      (f + t[1] * v).Phi()}; // acos((c[0]-f[0]-t[1]*v[0])/z[1]/tan(alpha))};
      int s[2];
      double phii = lpe.Phi();
      if (phii < ph[0])
        s[0] = (int)(48. * (ph[0] - phii) / M_PI);
      else
        s[0] = (int)(48. * (ph[0] - phii + 2 * M_PI) / M_PI);
      if (phii < ph[1])
        s[1] = (int)(48. * (ph[1] - phii) / M_PI);
      else
        s[1] = (int)(48. * (ph[1] - phii + 2 * M_PI) / M_PI);
      if (t[0] < 0 && t[1] < 0)
        return -1;
      if ((z[0] < innerR * ca || z[0] > outerR * ca) && (z[1] < innerR * ca || z[1] > outerR * ca))
        return -1;
      if ((z[0] < innerR * ca || z[0] > outerR * ca) ||
          (z[1] < innerR * ca || z[1] > outerR * ca)) {
        if (z[0] < innerR * ca || z[0] > outerR * ca)
          return s[1];
        else
          return s[0];
      } else {
        if (t[1] < 0 || t[0] < 0) {
          if (t[0] >= 0)
            return s[0];
          else
            return s[1];
        } else {
          if (t[0] <= t[1])
            return s[0];
          else
            return s[1];
        }
      }
      return -1;
    }
    break;
  }
  }
  return -1;
}
shape_parameter wedge::description() const
{
  shape_parameter sh;
  sh.setName("wedge");
  sh.setId(wedge_id1);
  sh.addParam<point3D>(center, "center");
  sh.addParam<vector3D>(normal, "normal");
  sh.addParam<vector3D>(lpe, "lower phi edge");
  sh.addParam<float>(innerR, "inner radius");
  sh.addParam<float>(outerR, "outer radius");
  sh.addParam<float>(dphi, "angle between edges");
  sh.addParam<float>(thickness, "thickness");
  sh.addParam<float>(distBarrel, "distance to barrel center");
  sh.setCompleteWrite(false);
  return sh;
}
shape_parameter wedge::getDescription()
{
  shape_parameter sh;
  sh.setName("wedge");
  sh.setId(wedge_id1);
  sh.addParam<point3D>(point3D(), "center");
  sh.addParam<vector3D>(vector3D(), "normal");
  sh.addParam<vector3D>(vector3D(), "lower phi edge");
  sh.addParam<float>(0, "inner radius");
  sh.addParam<float>(0, "outer radius");
  sh.addParam<float>(0, "angle between edges");
  sh.addParam<float>(0, "thickness");
  sh.addParam<float>(0, "distance to barrel center");
  sh.setCompleteWrite(false);
  return sh;
}
