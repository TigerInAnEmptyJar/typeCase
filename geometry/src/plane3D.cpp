#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
plane3D::plane3D(const plane3D& p)
    : geomObject(), f(p.Foot()), dir1(p.Direction(0)), dir2(p.Direction(1))
{
  dir1 = p.Direction(0);
  dir2 = p.Direction(1);
  dir1.normalize();
  dir2.normalize();
  n_ = p.Normal();
  f = p.Foot();
  vector3D v(dir1), w(dir2);
  if (v == w)
    Undefined();
  switch (p.getState()) {
  case _undefined_:
    Undefined();
    break;
  case _regular_:
    Regular();
    break;
  case _infinite_:
    Infinite();
    break;
  case _zero_:
    Zero();
    break;
  }
}

/*plane3D::plane3D(triangle3D t):point(3),point3D(t.A()),
dir1(t.A()-t.B()),dir2(t.A()-t.C())
{
  dir1.normalize();
  dir2.normalize();
    vector3D v(dir1),w(dir2),n;
    v.normalize();
    w.normalize();
    if (v==w) {
        Undefined();
        return;
    }
    n=v^w;
    n.normalize();
    w=n^v;
    w.normalize();
    dir1=v;
    dir2=w;
    }*/

plane3D::plane3D(const point3D& a, const point3D& b, const point3D& c)
    : geomObject(), f(a), dir1(a - b), dir2(a - c)
{
  f = a;
  vector3D v(dir1), w(dir2);
  v.normalize();
  w.normalize();
  if (v == w) {
    Undefined();
    return;
  }
  n_ = v ^ w;
  n_.normalize();
  w = n_ ^ v;
  w.normalize();
  dir1 = v;
  dir2 = w;
  if ((f.getState() == _infinite_) || (dir1.getState() == _infinite_) ||
      (dir2.getState() == _infinite_))
    Infinite();
  else if ((f.getState() == _undefined_) || (dir1.getState() == _undefined_) ||
           (dir2.getState() == _undefined_))
    Undefined();
  else if ((dir1.getState() == _zero_) || (dir2.getState() == _zero_))
    Undefined();
  else
    Regular();
}

plane3D::plane3D(const point3D& foot, const vector3D& d1, const vector3D& d2)
    : geomObject(), f(foot), dir1(d1), dir2(d2)
{
  f = foot;
  dir1.normalize();
  dir2.normalize();
  if (dir1 == dir2) {
    Undefined();
    return;
  }
  n_ = dir1 ^ dir2;
  n_.normalize();
  dir2 = n_ ^ dir1;
  dir2.normalize();
  if ((f.getState() == _infinite_) || (dir1.getState() == _infinite_) ||
      (dir2.getState() == _infinite_))
    Infinite();
  else if ((f.getState() == _undefined_) || (dir1.getState() == _undefined_) ||
           (dir2.getState() == _undefined_))
    Undefined();
  else if ((dir1.getState() == _zero_) || (dir2.getState() == _zero_))
    Undefined();
  else
    Regular();
}

plane3D::plane3D(const point3D& foot, const vector3D& norm) : geomObject(), f(foot)
{
  f = foot;
  n_ = norm;
  n_.normalize();
  if (n_ * vector3D(0, 1, 0) < 1 - __prec) {
    dir1 = vector3D(0, 1, 0) ^ n_;
    dir1.normalize();
    dir2 = n_ ^ dir1;
    dir2.normalize();
  } else if (n_ * vector3D(1, 0, 0) < 1 - __prec) {
    dir1 = vector3D(1, 0, 0) ^ n_;
    dir1.normalize();
    dir2 = n_ ^ dir1;
    dir2.normalize();
  } else {
    dir1 = vector3D(0, 0, 1) ^ n_;
    dir1.normalize();
    dir2 = n_ ^ dir1;
    dir2.normalize();
  }
  if ((f.getState() == _infinite_) || (dir1.getState() == _infinite_) ||
      (dir2.getState() == _infinite_))
    Infinite();
  else if ((f.getState() == _undefined_) || (dir1.getState() == _undefined_) ||
           (dir2.getState() == _undefined_))
    Undefined();
  else if ((dir1.getState() == _zero_) || (dir2.getState() == _zero_))
    Undefined();
  else
    Regular();
}

void plane3D::operator=(const plane3D& p)
{
  f = p.Foot();
  dir1 = p.Direction(0);
  dir2 = p.Direction(1);
  n_ = p.Normal();
  switch (p.getState()) {
  case _undefined_:
    Undefined();
    break;
  case _regular_:
    Regular();
    break;
  case _infinite_:
    Infinite();
    break;
  case _zero_:
    Zero();
    break;
  }
}

int plane3D::operator==(const plane3D& p) const
{
  if (!(n_ == p.Normal()))
    return 0;
  if ((p & f) == vector3D(0, 0, 0))
    return 1;
  return 0;
}
vector3D plane3D::operator&(const point3D& p) const
{
  if ((p - f) * n_ == 0)
    return vector3D(0, 0, 0);
  vector3D temp = (p - f);
  temp = n_ * (temp * n_);
  return temp;
} // Distance vector point-plane

point3D plane3D::operator-(const sLine3D& l) const
{
  if (l.Direction() * n_ == 0)
    return point3D();
  vector3D dist1 = (*this) & l.Foot();
  if (dist1.R() < __prec)
    return l.Foot();
  double c = dist1 * l.Direction() / (dist1.R());
  vector3D to = l.Direction() * dist1.R() / c;
  return l.Foot() - to;
} // hit point line-plane

point3D plane3D::operator-(const lLine3D& l) const
{
  point3D p = *this - sLine3D(l.P(), l.Q());
  vector3D v = l.P() - p;
  if (v * l.Direction() > 0)
    v = l.Q() - p;
  if (v.length() > (l.P() - l.Q()).length())
    return point3D();
  return p;
} // hit point line-plane

/*point3D plane3D::operator - (spiralLine3D s)
{
    sLine3D temp;
    temp = *this-s.Plane();
    if (temp.getState()!=_regular_)return point3D();
    return s-temp;
}*/

sLine3D plane3D::operator-(const plane3D& p) const
{
  if (*this == p)
    return sLine3D(f, f);
  if (n_ == p.Normal())
    return sLine3D();
  vector3D temp1;
  if (n_ * p.Direction(0) != 0)
    temp1 = p.Direction(0) - (n_ * (n_ * p.Direction(0)));
  else
    temp1 = p.Direction(1) - (n_ * (n_ * p.Direction(1)));
  vector3D temp2 = p & f;
  temp2 = temp2 - n_ * (temp2 * n_);
  return sLine3D(f - temp2, temp1);
} // two planes hitting->straight line

void plane3D::setPlane(const point3D& foot, const vector3D& d1, const vector3D& d2)
{
  dir1 = d1;
  dir2 = d2;
  dir1.normalize();
  dir2.normalize();
  if (dir1 == dir2) {
    Undefined();
    return;
  }
  n_ = dir1 ^ dir2;
  n_.normalize();
  dir2 = n_ ^ dir1;
  dir2.normalize();
  f = foot;
  if ((f.getState() == _infinite_) || (dir1.getState() == _infinite_) ||
      (dir2.getState() == _infinite_))
    Infinite();
  else if ((f.getState() == _undefined_) || (dir1.getState() == _undefined_) ||
           (dir2.getState() == _undefined_))
    Undefined();
  else if ((dir1.getState() == _zero_) || (dir2.getState() == _zero_))
    Undefined();
  else
    Regular();
}
//#ifdef USEQT
string plane3D::toString(int pt) const
{
  if (getState() != _regular_)
    return geomObject::toString();
  int ptt1 = -1;
  int ptt2 = -1;
  int ptt3 = -1;
  planeRep plr = pr;
  if (pt >= 0) {
    ptt1 = pt & 3;
    pt = pt << 2;
    ptt2 = pt & 3;
    pt = pt << 2;
    ptt3 = pt & 3;
    pt = pt << 2;
    if (pt == _parametric_ || pt == _normal_)
      plr = (planeRep)pt;
  }
  if (plr == _parametric_)
    return (string("( P = ") + f.toString(ptt1) + " + t * " + dir1.toString(ptt2) + " * s * " +
            dir2.toString(ptt3) + ")");
  else
    return (string("( (P - ") + f.toString(ptt1) + ") * " + n_.toString(ptt2) + " = 0 )");
}
//#endif
ostream& operator<<(ostream& o, const plane3D& p)
{
  if (p.getPlaneRep() == _normal_)
    o << "((P - " << p.Foot() << ") * " << p.Normal() << " = 0 )";
  else
    o << "P = " << p.Foot() << " + s" << p.Direction(0) << " + t" << p.Direction(1) << " ";
  return o;
}
// void plane3D::Infinite()
// {
//     geomObject::Infinite();
// }

// void plane3D::Undefined()
// {
//     geomObject::Undefined();
// }

// void plane3D::Regular()
// {
//     geomObject::Regular();
// }

// plane3D::plane3D():geomObject(),dir1(),dir2()
// {
//   Undefined();
// }

// point3D plane3D::Foot()const
// {
//     return f;
// }

// vector3D plane3D::Direction(int i)const
// {
//     switch (i)
//     {
// 	case 0:return dir1;
// 	case 1:return dir2;
//     }
//     return vector3D();
// }

// vector3D plane3D::Normal()const
// {
//     return n_;
// }

// geomState plane3D::getState() const
// {
//   return geomObject::getState();
// }

// void plane3D::Zero()
// {
//     geomObject::Zero();
//     dir1.setValues(0,0,0);
//     dir2.setValues(0,0,0);
// }

// rep3D plane3D::getRep() const
// {
//     return f.getRep();
// }

// void plane3D::setRep(rep3D r)
// {
//     f.setRep(r);
//     dir1.setRep(r);
//     dir2.setRep(r);
//     n_.setRep(r);
// }

// planeRep plane3D::getPlaneRep() const
// {
//     return pr;
// }

// void plane3D::setPlaneRep(planeRep r)
// {
//     pr=r;
// }
