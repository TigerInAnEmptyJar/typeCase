#include "shape_utilities.h"
#include "planeShape.h"
#include "ring.h"
#include "shapeparameter.h"
#include "shapes.h"
#include "spiral.h"
#include "volumeShape.h"
#include "wedge.h"
vector3D getEntrance(base_shape* shape, const sLine3D& path)
{
  if (shape == NULL)
    return vector3D();
  if (shape->getName() == "wedge" || shape->getName() == "spiral" || shape->getName() == "ring" ||
      shape->getName() == "fiber" || shape->getName() == "hexPrism" ||
      shape->getName() == "cylinder" || shape->getName() == "strawTube" ||
      shape->getName() == "volumeShape")
    return getEntrance((volumeShape*)shape, path);
  if (shape->getName() == "triangle" || shape->getName() == "rectangle" ||
      shape->getName() == "circle" || shape->getName() == "sphericTriangle" ||
      shape->getName() == "quadrangle" || shape->getName() == "hexagon" ||
      shape->getName() == "sphericRectangle")
    return getEntrance((planeShape*)shape, path);
  return vector3D();
}
vector3D getEntrance(planeShape* shape, const sLine3D& path)
{
  if (shape == NULL)
    return vector3D();
  if (shape->getName() == "triangle" || shape->getName() == "rectangle" ||
      shape->getName() == "circle" || shape->getName() == "sphericTriangle" ||
      shape->getName() == "quadrangle" || shape->getName() == "hexagon" ||
      shape->getName() == "sphericRectangle") {
    vector3D n = shape->getNormal();
    vector3D c = shape->getCenter();
    plane3D p(c, n);
    return vector3D(p - path);
  }
  return vector3D();
}
vector3D getEntrance(volumeShape* shape, const sLine3D& path)
{
  if (shape == NULL)
    return vector3D();
  point3D foot(0, 0, 0);
  vector3D normal(0, 0, -1);
  if (shape->getName() == "wedge") {
    if (((wedge*)shape)->getDistToBarrelCenter() > 0) {
      wedge* w = (wedge*)shape;
      float alpha = asin(w->getDistToBarrelCenter() / w->getOuterRadius());
      //	  float
      // h=sqrt(w->getOuterRadius()*w->getOuterRadius()-w->getDistToBarrelCenter()*w->getDistToBarrelCenter());
      float s = w->getCenter().Z() * (w->getDistToBarrelCenter() / w->getOuterRadius() /
                                      sin(M_PI - alpha - path.Direction().Theta()));
      return path.Foot() + path.Direction() * s;
    }
    foot = ((wedge*)shape)->getCenter();
    normal = ((wedge*)shape)->getNormal();
  } else if (shape->getName() == "spiral") {
    foot = ((spiral*)shape)->getCenter();
    normal = ((spiral*)shape)->getNormal();
  } else if (shape->getName() == "ring") {
    foot = ((ring*)shape)->getCenter();
    normal = ((ring*)shape)->getNormal();
  } else if (shape->getName() == "fiber") {
    foot = ((fiber*)shape)->getCorner();
    normal = ((fiber*)shape)->getDirection(2);
  } else if (shape->getName() == "hexPrism") {
    foot = ((hexPrism*)shape)->getCenter();
    normal = ((hexPrism*)shape)->getDirection(2);
  } else if (shape->getName() == "cylinder") {
    normal = ((cylinder*)shape)->getDirection();
    foot = ((cylinder*)shape)->getCenter() + normal;
  } else if (shape->getName() == "strawTube") {
    normal = ((strawTube*)shape)->getDirection(1) ^ ((strawTube*)shape)->getDirection(0);
    foot =
        ((strawTube*)shape)->getCenter() + normal * (((strawTube*)shape)->getRadius() / normal.R());
  }
  plane3D pl(foot, normal);
  return (pl - path);

  if (shape->getName() == "wedge") {
    // return shape->entrance(path);
    if (((wedge*)shape)->getDistToBarrelCenter() > 0) {
      wedge* w = (wedge*)shape;
      float alpha = asin(w->getDistToBarrelCenter() / w->getOuterRadius());
      float h = sqrt(w->getOuterRadius() * w->getOuterRadius() -
                     w->getDistToBarrelCenter() * w->getDistToBarrelCenter());
      return ConeLine(w->getCenter(), vector3D(0, 0, -1), alpha, h, path);
    } else
      return shape->entrance(path);
  } else
    return shape->entrance(path);
  return vector3D();
}
vector3D ConeLine(const point3D& tip, const vector3D& rotaxis, const float& alpha,
                  const float& height, const sLine3D& path)
{
  double tana2 = tan(alpha) * tan(alpha);
  vector3D v = path.Direction();
  point3D f = path.Foot();
  point3D c = tip;
  double a = v.Z() * v.Z() - (v.Y() * v.Y() + v.X() * v.X()) / tana2;
  double b =
      2 * (v.X() * (c.X() - f.X()) + v.Y() * (c.Y() - f.Y())) / tana2 - 2 * v.Z() * (c.Z() - f.Z());
  double cc = (c.Z() - f.Z()) * (c.Z() - f.Z()) -
              (((c.X() - f.X()) * (c.X() - f.X())) + ((c.Y() - f.Y()) * (c.Y() - f.Y()))) / tana2;
  vector3D d(0, 0, 0);
  vector3D d1, d2, d3, d4;
  if (b * b - 4 * a * cc < 0)
    return vector3D();
  double t[2] = {(-b + sqrt(b * b - 4 * a * cc)) / 2 / a, (-b - sqrt(b * b - 4 * a * cc)) / 2 / a};
  // double z[2]={c.Z()-f.Z()-t[0]*v.Z(),c.Z()-f.Z()-t[1]*v.Z()};
  // double
  // ph[2]={acos((c.X()-f.X()-t[0]*v.X())/z[0]/tan(alpha)),acos((c.X()-f.X()-t[1]*v.X())/z[1]/tan(alpha))};
  point3D p[2] = {f + t[0] * v, f + t[1] * v};
  if (abs((tip - p[0]).Z()) > height)
    return p[1];
  if (abs((tip - p[1]).Z()) > height)
    return p[0];
  int outInd = 0;
  if (t[0] < 0 && t[1] < 0) {
    if (t[0] < t[1])
      outInd = 1;
    else
      outInd = 0;
  } else if (t[0] < 0 || t[1] < 0) {
    if (t[0] < 0)
      outInd = 1;
    else
      outInd = 0;
  } else {
    if (t[0] < t[1])
      outInd = 0;
    else
      outInd = 1;
  }
  return p[outInd];

  ////////////////////////////////
  //   double cosa=cos(alpha);
  //   vector3D d=rotaxis/rotaxis.R();
  //   double vd=path.Direction()*d;
  //   vector3D fc=path.Foot()-tip;
  //   double a=vd*vd-cosa*cosa*path.Direction().R();
  //   double b=2*(vd*(fc*d)-cosa*cosa*(fc*path.Direction()));
  //   double c=fc*d;
  //   c=c*c-cosa*cosa*fc.R();
  //   double t,t1,t2;
  //   double det=b*b-4*a*c;
  //   cout<<tip<<d<<alpha<<fc<<vd<<endl;
  //   if(det<0)return vector3D();
  //   if(a==0)
  //     t=-c/b;
  //   else
  //     {
  //       if(det==0)
  // 	t=-b/(2*a);
  //       else
  // 	{
  // 	  t1=(-b+sqrt(det))/(2*a);
  // 	  t2=(-b-sqrt(det))/(2*a);
  // 	  vector3D v1=(path.Foot()+t1*path.Direction());
  // 	  vector3D v2=(path.Foot()+t2*path.Direction());
  // 	  if(v1.Z()>=0&&v2.Z()>=0)
  // 	    {if(v1.Z()>v2.Z())t=t2;else t=t1;}
  // 	  else if(v1.Z()<=0&&v2.Z()<=0)
  // 	    {if(abs(v1.Z())>abs(v2.Z()))t=t2;else t=t1;}
  // 	  else
  // 	    {if(v1.Z()<v2.Z())t=t2;else t=t1;}
  // 	}
  //     }
  //   return vector3D(path.Foot()+t*path.Direction());
}
vector3D getPath(int nstart, base_shape** start, int nstop, base_shape** stop, const sLine3D& path)
{
  if (start == NULL || stop == NULL)
    return vector3D();
  if (nstart == 0 || nstop == 0)
    return vector3D();
  int ns = 0, nt = 0;
  vector3D sa, so;
  sa.setValues(0, 0, 0);
  so.setValues(0, 0, 0);
  vector3D tmp;
  for (int i = 0; i < nstart; i++) {
    if (start[i] == NULL)
      continue;
    tmp = getEntrance(start[i], path);
    if (tmp.getState() != _undefined_) {
      ns++;
      sa = sa + tmp;
    }
  }
  if (ns == 0)
    return vector3D();
  for (int i = 0; i < nstop; i++) {
    if (stop[i] == NULL)
      continue;
    tmp = getEntrance(stop[i], path);
    if (tmp.getState() != _undefined_) {
      nt++;
      so = so + tmp;
    }
  }
  if (nt == 0)
    return vector3D();
  sa = sa * (1. / ns);
  so = so * (1. / nt);
  return so - sa;
}

#include "fiber.h"
#include "spiral.h"
#include "wedge.h"
float getLightWay(volumeShape* sh, point3D onPoint, bool from)
{
  if (sh == NULL)
    return -1;
  if (sh->getName() == "wedge") {
    float r = (onPoint - ((wedge*)sh)->getCenter()).R();
    if (from)
      return r - ((wedge*)sh)->getInnerRadius();
    else
      return ((wedge*)sh)->getOuterRadius() - r;
  } else if (sh->getName() == "spiral") {
    float r = (onPoint - ((spiral*)sh)->getCenter()).R();
    if (from)
      return r - ((spiral*)sh)->getInnerRadius();
    else
      return ((spiral*)sh)->getOuterRadius() - r;
  } else if (sh->getName() == "fiber") {
    point3D p2;
    if (from)
      p2 = ((fiber*)sh)->getCorner();
    else
      p2 = ((fiber*)sh)->getCorner() + ((fiber*)sh)->getDirection(0);
    return (onPoint - p2).R();
  }
  return -1;
}
extern base_shape* getShape(const shape_parameter& sp);
float getLightWay(shape_parameter& sp, sLine3D& path, bool from)
{
  if (sp.getName() == "wedge" || sp.getName() == "spiral") {
    plane3D pl(sp.getParam<point3D>(0), sp.getParam<vector3D>(0));
    point3D p = pl - path;
    vector3D v = p - pl.Foot();
    return sp.getParam<float>(1) - v.R();
  } else if (sp.getName() == "fiber") {
    vector3D shift = 0.5 * sp.getParam<vector3D>(0) +
                     sp.getParam<vector3D>(1) * (sp.getParam<int>(1) + 0.5 * sp.getParam<int>(0));
    sLine3D middleLine(sp.getParam<point3D>(0) + shift, sp.getParam<vector3D>(0));
    lLine3D poca = middleLine - path;
    return (poca.Q() - (sp.getParam<point3D>(0) + shift)).R();
  } else if (sp.getName() == "wedge-cone") {
    float z = sp.getParam<float>(2) * sp.getParam<float>(0) /
              (sp.getParam<float>(0) - sp.getParam<float>(1));
    float oR = sqrt(z * z + sp.getParam<float>(0) * sp.getParam<float>(0));
    float alpha = atan(abs(sp.getParam<float>(0) - sp.getParam<float>(1)) / sp.getParam<float>(2));
    float lrp = (z * sin(path.Direction().Theta()) / sin(M_PI - alpha - path.Direction().Theta()));
    return oR - lrp;
  } else {
    volumeShape* sh = (volumeShape*)getShape(sp);
    if (sh == NULL)
      return -1;
    point3D op = sh->entrance(path);
    float ret = getLightWay(sh, op, from);
    delete sh;
    return ret;
  }
  return -1;
}
float getLightWay(volumeShape* sh, const sLine3D& path, bool from)
{
  if (sh == NULL)
    return -1;
  point3D onPoint = sh->entrance(path);
  if (onPoint.getState() != _regular_)
    return -1;
  return getLightWay(sh, onPoint, from);
}
