#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
sLine3D::sLine3D(const point3D& p, const point3D& q) : geomObject()
{
  d = (q - p);
  setRep(p.getRep());
  if ((p.getState() == _undefined_) || (q.getState() == _undefined_)) {
    Undefined();
    return;
  }
  if (d.getState() == _zero_) {
    Zero();
    return;
  }
  foot = p;
  d.normalize();
}

sLine3D::sLine3D(const point3D& p, const vector3D& v) : geomObject()
{
  Regular();
  setRep(v.getRep());
  if ((p.getState() == _undefined_) || (v.getState() == _undefined_)) {
    Undefined();
    return;
  }
  if (v.getState() == _zero_) {
    Zero();
    return;
  }
  d = v;
  foot = p;
  d.normalize();
}

sLine3D::sLine3D(const sLine3D& l) : geomObject()
{
  foot = l.Foot();
  d = l.Direction();
  d.normalize();
  setRep(l.getRep());
  if (l.getState() == _regular_) {
    Regular();
    return;
  }
  if (l.getState() == _zero_)
    Zero();
  if (l.getState() == _undefined_)
    Undefined();
  if (l.getState() == _infinite_)
    Infinite();
}

void sLine3D::newLine(const point3D& p, const vector3D& v)
{
  d = v;
  foot = p;
  d.normalize();
  setRep(v.getRep());
  if ((p.getState() == _undefined_) || (v.getState() == _undefined_)) {
    Undefined();
    return;
  }
  if (v.getState() == _zero_) {
    Zero();
    return;
  }
  if (v.isLine() != d.isLine())
    d.transpose();
}

int sLine3D::isIn(const point3D& pIn) const
{
  if ((getState() == _undefined_) || (getState() == _infinite_) ||
      (foot.getState() == _undefined_) || (foot.getState() == _infinite_))
    return 0;
  if (getState() == _zero_)
    return (pIn == foot);
  vector3D fp = pIn - foot;
  vector3D n = fp - d * (d * fp);
  if (n.length() <= -__prec)
    return 1;
  else
    return 0;
}

// geomState sLine3D::getState() const
// {
//   return geomObject::getState();
// }

ostream& operator<<(ostream& o, const sLine3D& l)
{
  switch (l.getState()) {
  case _zero_: {
    o << "(p = ";
    o << l.Foot();
    o << " v = ZERO )";
    break;
  }
  case _undefined_: {
    o << "(undefined)";
    break;
  }
  case _regular_:
  case _infinite_: {
    o << "(p = ";
    o << l.Foot();
    o << " v = ";
    o << l.Direction();
    o << ")";
  }
  }
  return o;
}

// vector3D sLine3D::operator -(const point3D &p)const
// {
//   vector3D v=p-foot;
//   vector3D n=v-d*(d*v);
//   return n;
// }

point3D sLine3D::operator&&(const sLine3D& l) const
{
  vector3D d0 = Direction();
  vector3D d1 = l.Direction();
  vector3D f0 = Foot();
  vector3D f1 = l.Foot();
  vector3D d0Kreuzd1 = d0 ^ d1;
  if ((d0Kreuzd1.length() <= 0 + __prec) && (d0Kreuzd1.length() >= 0 - __prec)) {
    if (((f0 - f1).length() <= 0 + __prec) && (f0 - f1).length() >= 0 - __prec) {
      return f0;
    }
    return point3D();
  }
  double l0, l1;
  vector3D v0 = (f0 - f1) ^ d1;
  vector3D v1 = (f1 - f0) ^ d0;
  double r0r1 = d0Kreuzd1.length();
  l0 = d0Kreuzd1 * v0 / r0r1 / r0r1 * (-1);
  l1 = d0Kreuzd1 * v1 / r0r1 / r0r1;

  point3D p = Foot() + Direction() * l0;
  point3D q = l.Foot() + l.Direction() * l1;
  if ((p - q).length() <= __prec) {
    return p;
  }
  return point3D();
}

lLine3D sLine3D::operator-(const sLine3D& l) const
{
  vector3D d0 = Direction();
  vector3D d1 = l.Direction();
  vector3D d0Kreuzd1 = d0 ^ d1;
  double d0xd1length = d0Kreuzd1.length();
  if ((d0xd1length <= __prec)) {
    vector3D tmp = (sLine3D)l - Foot();
    return lLine3D(Foot(), Foot() - tmp);
  }
  double l0, l1;
  vector3D f0 = Foot();
  vector3D f1 = l.Foot();
  vector3D v0 = (f0 - f1) ^ d1;
  vector3D v1 = (f1 - f0) ^ d0;
  double r0r1 = d0Kreuzd1.length();
  l0 = d0Kreuzd1 * v0 / r0r1 / r0r1 * (-1);
  l1 = d0Kreuzd1 * v1 / r0r1 / r0r1;
  point3D p = f0 + (d0 * l0);
  point3D q = f1 + (d1 * l1);
  lLine3D ret(p, q);
  return ret; // lLine3D(p,q);
}

lLine3D sLine3D::operator-(const lLine3D& l) const
{
  vector3D d0 = Direction();
  vector3D d1 = l.Direction();
  vector3D d0Kreuzd1 = d0 ^ d1;
  double d0xd1length = d0Kreuzd1.length();
  if ((d0xd1length <= 0 + __prec) && (d0xd1length >= 0 - __prec)) {
    vector3D tmp = (lLine3D)l - Foot();
    return lLine3D(Foot(), Foot() - tmp);
  }
  double l0, l1;
  vector3D f0 = Foot();
  vector3D f1 = l.P();
  vector3D v0 = (f0 - f1) ^ d1;
  vector3D v1 = (f1 - f0) ^ d0;
  double r0r1 = d0Kreuzd1.length();
  l0 = d0Kreuzd1 * v0 / r0r1 / r0r1 * (-1);
  l1 = d0Kreuzd1 * v1 / r0r1 / r0r1;

  point3D p = Foot() + Direction() * l0;
  point3D q = l.P() + l.Direction() * l1;
  if (l.isIn(q)) {
    return lLine3D(p, q);
  }
  vector3D dis1((*this) - l.P()), dis2((*this) - l.Q());
  if (dis1.length() < dis2.length()) {
    return lLine3D(l.P(), (dis1 * (-1)));
  }
  return lLine3D(l.Q(), (dis2 * (-1)));
}

double sLine3D::operator&(const sLine3D& l) const
{
  if (Direction() == l.Direction()) {
    vector3D tmp = l - Foot();
    return -1;
  }
  double l0, l1;
  vector3D f0 = Foot();
  vector3D f1 = l.Foot();
  vector3D d0 = Direction();
  vector3D d1 = l.Direction();
  vector3D d0Kreuzd1 = d0 ^ d1;
  vector3D v0 = (f0 - f1) ^ d1;
  vector3D v1 = (f1 - f0) ^ d0;
  double r0r1 = d0Kreuzd1.length();
  l0 = d0Kreuzd1 * v0 / r0r1 / r0r1 * (-1);
  l1 = d0Kreuzd1 * v1 / r0r1 / r0r1;

  point3D p = Foot() + Direction() * l0;
  point3D q = l.Foot() + l.Direction() * l1;
  return ((p - q).length());
}
// sLine3D::sLine3D(): geomObject()
// {
// }
//#ifdef USEQT
string sLine3D::toString(int pt) const
{
  if (getState() != _regular_)
    return geomObject::toString();
  int ptt = -1;
  if (ptt <= 0) {
    ptt = (pt & 3);
    pt = pt << 2;
  }
  return (string("( p = ") + foot.toString(pt) + ", v = " + d.toString(ptt) + ")");
}
//#endif
void sLine3D::operator=(const sLine3D& l)
{
  foot = l.Foot();
  d = l.Direction();
  d.normalize();
  setRep(l.getRep());
  if (l.getState() == _regular_) {
    Regular();
    return;
  }
  if (l.getState() == _zero_)
    Zero();
  if (l.getState() == _undefined_)
    Undefined();
  if (l.getState() == _infinite_)
    Infinite();
  //  newLine(l.Foot(),l.Direction());
}

// point3D sLine3D::Foot() const
// {
//     return foot;
// }

// vector3D sLine3D::Direction() const
// {
//     return d;
// }

// void sLine3D::shiftFoot(double t)
// {
//     foot = foot + d*t;
// }

// rep3D sLine3D::getRep() const
// {
//     return d.getRep();
// }

// void sLine3D::setRep(rep3D r)
// {
//     d.setRep(r);
//     foot.setRep(r);
// }

// int sLine3D::isLine() const
// {
//     return d.isLine();
// }

// int sLine3D::isColumn() const
// {
//     return d.isColumn();
// }

// void sLine3D::transpose()
// {
//     d.transpose();
// }

// int sLine3D::getDimension() const
// {
//     return 3;
// }

// void sLine3D::Zero()
// {
//   geomObject::Zero();
//     d.setValues(0,0,0);
//     foot.Zero();
// }

// void sLine3D::Undefined()
// {
//   geomObject::Undefined();
//   foot=point3D();
//   d=vector3D();
// }

// void sLine3D::Regular()
// {
//   geomObject::Regular();
// }

// void sLine3D::Infinite()
// {
//   geomObject::Infinite();
// }
