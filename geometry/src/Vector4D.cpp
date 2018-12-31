#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
// vector4D::vector4D():  point4D(), Vector(0)
// {
// }

// vector4D::vector4D(const point &p):  point4D(p), Vector(0)
// {
// }

// vector4D::vector4D(const point3D &p):  point4D(p), Vector(0)
// {
// }

// vector4D::vector4D(const point4D &p): point4D(p), Vector(0)
// {
// }

vector4D::vector4D(const Vector& v) : point4D(), Vector(0)
{
  if (isLine() != v.isLine())
    transpose();
  if ((v.getState() == _undefined_) || (v.getDimension() != 4)) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v[0], v[1], v[2], v[3]);
}

vector4D::vector4D(const vector3D& v) : point4D(), Vector(0)
{
  setRep(v.getRep());
  if (isLine() != v.isLine())
    transpose();
  if (v.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v.X(), v.Y(), v.Z(), 0);
}
#ifdef USEROOT
vector4D::vector4D(const vector4D& v) : point4D(), Vector(0)
#else
vector4D::vector4D(const vector4D& v) : point4D(), Vector(0)
#endif
{
  setRep(v.getRep());
  if (isLine() != v.isLine())
    transpose();
  if (v.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v.X(), v.Y(), v.Z(), v.W());
}

// vector4D::vector4D(double x, double y, double z): point4D(x,y,z,0), Vector(0)
// {
// }

// vector4D::vector4D(double x, double y, double z, double w): point4D(x,y,z,w),
// Vector(0)
// {
// }

void vector4D::operator=(const point& p)
{
  if ((p.getState() == _undefined_) || ((p.getDimension() != 4) && (p.getDimension() != 3))) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(p[0], p[1], p[2], ((p.getDimension() == 3) ? 0 : p[3]));
}

void vector4D::operator=(const Vector& v)
{
  if (v.isLine() != isLine())
    transpose();
  if ((v.getState() == _undefined_) || ((v.getDimension() != 4) && (v.getDimension() != 3))) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v[0], v[1], v[2], ((v.getDimension() == 3) ? 0 : v[3]));
}

void vector4D::operator=(const point4D& p)
{
  if ((p.getState() == _undefined_)) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(p[0], p[1], p[2], p[3]);
}

void vector4D::operator=(const vector4D& v)
{
  if (v.isLine() != isLine())
    transpose();
  if ((v.getState() == _undefined_)) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v[0], v[1], v[2], v[3]);
}

void vector4D::operator=(const point3D& p)
{
  if ((p.getState() == _undefined_)) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(p[0], p[1], p[2], 0);
}

void vector4D::operator=(const vector3D& v)
{
  if (v.isLine() != isLine())
    transpose();
  if ((v.getState() == _undefined_)) {
    Zero();
    Undefined();
    return;
  }
  point4D::setValues(v[0], v[1], v[2], 0);
}
//#ifdef USEQT
string vector4D::toString(int pt) const
{
  string ret = point4D::toString(pt);
  if (!isLine())
    ret += "T";
  return ret;
}
//#endif
ostream& operator<<(ostream& o, const vector4D& v)
{
  switch (v.getState()) {
  case _infinite_: {
    o << "(infinite)";
    break;
  }
  case _undefined_: {
    o << "(undefined)";
    break;
  }
  case _zero_:
  case _regular_: {
    switch (v.getRep()) {
    case _cartesic3_: {
      o << "(" << v.X() << ", " << v.Y() << ", " << v.Z() << ", " << v.W() << ")";
      break;
    }
    case _spheric_: {
      o << "(" << v.Phi() << ", " << v.Theta() << ", " << v.R() << ", " << v.W() << ")";
      break;
    }
    case _cylindric_: {
      o << "(" << v.Phi() << ", " << v.Rho() << ", " << v.Z() << ", " << v.W() << ")";
    }
    }
  }
  }
  if (v.isColumn())
    o << "T";
  return o;
}

// int vector4D::operator == (const vector4D &v)const
// {
//     return
//     ((point4D::value4-v.W()<__prec)&&(point4D::value1-v.X()<__prec)&&(point4D::value2-v.Y()<__prec)&&(point4D::value3-v.Z()<__prec));
// }

// vector4D vector4D::operator !()const
// {
//     vector4D v(*this);
//     v.transpose();
//     return v;
// }

// void vector4D::Zero()
// {
//     point4D::Zero();
// }

// void vector4D::Infinite()
// {
//     point4D::Infinite();
// }

// void vector4D::Undefined()
// {
//     point4D::Undefined();
// }

// void vector4D::Regular()
// {
//     point4D::Regular();
// }

// void vector4D::setValues(double x, double y, double z, double w)
// {
//     point4D::setValues(x,y,z,w);
// }

// void vector4D::setValues(double x, double y, double z)
// {
//     point4D::setValues(x,y,z);
// }

// double vector4D::getValue(int pos) const
// {
//     return point4D::getValue(pos);
// }

// geomState vector4D::getState() const
// {
//     return point4D::getState();
// }

// void vector4D::normalize()
// {
//     double t=length();
//     if(t==0)return;
//     setValues(point4D::value1/t,point4D::value2/t,point4D::value3/t,point4D::value4/t);
// }

// double vector4D::length() const
// {
//     return sqrt(point4D::f_r*point4D::f_r+point4D::value4*point4D::value4);
// }

// void vector4D::setValue(int pos, double valueIn)
// {
//     point4D::setValue(pos,valueIn);
// }
// double vector4D::operator[](int pos)const
// {
//   return point4D::operator[](pos);
// }
// int vector4D::getDimension() const{return 4;}
