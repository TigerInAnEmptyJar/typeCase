#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
lLine3D::lLine3D(const point3D& p, const point3D& q) : geomObject()
{
  pPoint = p;
  qPoint = q;
  Regular();
  if ((p.getState() == _infinite_) || (q.getState() == _infinite_))
    Infinite();
  else if ((q.getState() == _undefined_) || (p.getState() == _undefined_))
    Undefined();
  else if (((point3D)q - (point3D)p).getState() == _zero_)
    Zero();
  else
    Regular();
}

lLine3D::lLine3D(const point3D& p, const vector3D& v) : geomObject()
{
  pPoint = p;
  qPoint = p + v;
  Regular();
  if ((p.getState() == _infinite_) || (v.getState() == _infinite_))
    Infinite();
  else if ((p.getState() == _undefined_) || (v.getState() == _undefined_))
    Undefined();
  else if ((v.getState() == _zero_))
    Zero();
  else
    Regular();
  setRep(p.getRep());
}
int lLine3D::isIn(const point3D& pIn) const
{
  if ((getState() == _undefined_) || (getState() == _infinite_) ||
      (qPoint.getState() == _undefined_) || (qPoint.getState() == _infinite_))
    return 0;
  vector3D pq = qPoint - pPoint;
  vector3D pInq = Q() - pIn;
  if ((abs((pq ^ pInq).X()) > __prec) && (abs((pq ^ pInq).Y()) > __prec) &&
      (abs((pq ^ pInq).Z()) > __prec))
    return 0;
  double d = pInq * pq / pq.length() / pq.length();
  return ((d <= 1) && (d >= 0));
}

void lLine3D::newLine(const point3D& p, const point3D& q)
{
  pPoint = p;
  qPoint = q;
  Regular();
  if ((p.getState() == _infinite_) || (q.getState() == _infinite_))
    Infinite();
  else if ((p.getState() == _undefined_) || (q.getState() == _undefined_))
    Undefined();
  else if (((q - p).getState() == _zero_))
    Zero();
  else
    Regular();
  setRep(p.getRep());
}

ostream& operator<<(ostream& o, const lLine3D& l)
{
  switch (l.getState()) {
  case _zero_: {
    o << "(P = Q = " << l.P() << ")";
    break;
  }
  case _undefined_: {
    o << "(undefined)";
    break;
  }
  case _regular_:
  case _infinite_: {
    o << "(P = " << l.P() << " Q = " << l.Q() << " length = " << l.Direction().R() << ")";
  }
  }
  return o;
}

vector3D lLine3D::operator-(const point3D& p) const
{
  vector3D w(P()), x(p);
  double vQ = Direction().R();
  double t = (x * Direction() - w * Direction()) / (vQ * vQ);
  if (t <= 0)
    return (p - P());
  if (t >= 1)
    return (p - Q());
  point3D q = P() + Direction() * t;
  return (p - q);
}

point3D lLine3D::operator&&(const lLine3D& l) const
{
  double t1 =
      (pPoint.Y() - l.P().Y()) - ((pPoint.X() - l.P().X()) * l.Direction().Y() / l.Direction().X());
  double t2 =
      (pPoint.Z() - l.P().Z()) - ((pPoint.X() - l.P().X()) * l.Direction().Z() / l.Direction().X());
  if ((t1 - t2 > __prec) || ((t1 < 0) || (t1 > 1)) || ((t2 < 0) && (t2 > 1)))
    return point3D();
  point3D p = pPoint + Direction() * t1;
  return p;
}

double lLine3D::operator&(const lLine3D& l) const
{
  double t1 =
      (pPoint.Y() - l.P().Y()) - ((pPoint.X() - l.P().X()) * l.Direction().Y() / l.Direction().X());
  double t2 =
      (pPoint.Z() - l.P().Z()) - ((pPoint.X() - l.P().X()) * l.Direction().Z() / l.Direction().X());
  if (t1 - t2 < __prec)
    return 0;
  if (Direction() == l.Direction()) {
    vector3D tmp = l - pPoint;
    return sqrt(tmp * tmp);
  }
  double b1, b2, as1, as2, at1, at2;
  b1 = (pPoint - l.P()) * Direction();
  b2 = (pPoint - l.P()) * l.Direction();
  as1 = Direction() * l.Direction();
  as2 = l.Direction() * l.Direction();
  at1 = -(Direction() * Direction());
  at2 = -as1;
  double t = (b2 - b1 * as2 / as1) / (at2 - at1 * as2 / as1);
  double s = (b1 - t * at1) / as1;
  point3D p;
  point3D q;
  if (t < 0)
    p = pPoint;
  else if (t > 1)
    p = Q();
  else
    p = pPoint + Direction() * t;
  if (s < 0)
    q = l.P();
  else if (s > 1)
    q = l.Q();
  else
    q = l.P() + l.Direction() * s;
  vector3D pq = p - q;
  return sqrt(pq * pq);
}
// lLine3D::lLine3D(): geomObject(),qPoint()
// {
//   Undefined();
// }

lLine3D::lLine3D(const lLine3D& l) : geomObject()
{
  pPoint = l.P();
  qPoint = l.Q();
  Regular();
  switch (l.getState()) {
  case _undefined_:
    Undefined();
    break;
  case _zero_:
    Zero();
    break;
  case _regular_:
    Regular();
    break;
  case _infinite_:
    Infinite();
    break;
  }
  setRep(l.getRep());
}

// point3D lLine3D::P() const
// {
//     return pPoint;
// }

// point3D lLine3D::Q() const
// {
//     return qPoint;
// }

// vector3D lLine3D::Direction() const
// {
//     return (qPoint-pPoint);
// }

// point3D lLine3D::Mid()const
// {
//   return pPoint+((qPoint-pPoint)*0.5);
// }
// double lLine3D::length()const
// {
//     vector3D v=qPoint-pPoint;
//     return sqrt(v*v);
// }

// rep3D lLine3D::getRep() const
// {
//     return pPoint.getRep();
// }

// void lLine3D::setRep(rep3D r)
// {
//     pPoint.setRep(r);
//     qPoint.setRep(r);
// }

// geomState lLine3D::getState() const
// {
//   return geomObject::getState();
// }

// int lLine3D::getDimension() const
// {
//     return 3;
// }

// void lLine3D::Zero()
// {
//     geomObject::Zero();
//     pPoint.Zero();
//     qPoint.Zero();
// }

// void lLine3D::Undefined()
// {
//   pPoint.Undefined();
//   qPoint=point3D();
//   geomObject::Undefined();
//     //   newLine(point3D(), point3D());
// }

void lLine3D::operator=(const lLine3D& l)
{
  pPoint = l.P();
  qPoint = l.Q();
  Regular();
  if ((pPoint.getState() == _infinite_) || (qPoint.getState() == _infinite_))
    Infinite();
  else if ((qPoint.getState() == _undefined_) || (pPoint.getState() == _undefined_))
    Undefined();
  else if (((point3D)qPoint - (point3D)pPoint).getState() == _zero_)
    Zero();
  else
    Regular();
}
//#ifdef USEQT
string lLine3D::toString(int pt) const
{
  if (getState() != _regular_ && getState() != _zero_)
    return geomObject::toString();
  int ptt = -1;
  if (pt >= 0) {
    ptt = pt & 3;
    pt = pt << 2;
  }
  if (getState() == _zero_)
    return (string("( P = Q = ") + pPoint.toString(ptt) + ")");
  return (string("( P = ") + pPoint.toString(ptt) + ", Q = " + qPoint.toString(pt) + ", length = " +
          string_number((pPoint - qPoint).R()) + ")");
}
//#endif
