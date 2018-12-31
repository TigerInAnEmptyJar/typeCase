#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif

vector3D::vector3D(const point& p) : point3D(), Vector(0)
{
  if ((p.getState() == _undefined_) || (p.getDimension() != 3)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(p[0], p[1], p[2]);
}

vector3D::vector3D(const Vector& vec) : point3D(), Vector(0)
{
  if (isLine() != vec.isLine())
    transpose();
  if ((vec.getState() == _undefined_) || (vec.getDimension() != 3)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(vec[0], vec[1], vec[2]);
}
#ifdef USEROOT
vector3D::vector3D(const vector3D& vec) : point3D(), Vector(0)
#else
vector3D::vector3D(const vector3D& vec) : point3D(), Vector(0)
#endif
{
  setRep(vec.getRep());
  if (isLine() != vec.isLine())
    transpose();
  if (vec.getState() == _undefined_) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(vec.X(), vec.Y(), vec.Z());
}

vector3D::vector3D(const momentum4D& m) : point3D(), Vector(0)
{
  setRep(m.getRep());
  if (isLine() != m.isLine())
    transpose();
  if (m.getState() == _undefined_) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(m.X() * m.Energy(), m.Y() * m.Energy(), m.Z() * m.Energy());
}

// vector3D vector3D::operator / (double f)const
// {
//   return vector3D(point3D::value1/f, point3D::value2/f, point3D::value3/f);
// }
void vector3D::operator=(const point& p)
{
  if ((p.getState() == _undefined_) || (p.getDimension() != 3)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(p[0], p[1], p[2]);
}

void vector3D::operator=(const Vector& vec)
{
  if (isLine() != vec.isLine())
    transpose();
  if ((vec.getState() == _undefined_) || (vec.getDimension() != 3)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(vec[0], vec[1], vec[2]);
}

void vector3D::operator=(const point3D& p)
{
  if ((p.getState() == _undefined_)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(p.X(), p.Y(), p.Z());
}

void vector3D::operator=(const vector3D& vec)
{
  if (isLine() != vec.isLine())
    transpose();
  if ((vec.getState() == _undefined_)) {
    Zero();
    Vector::Undefined();
    return;
  }
  setValues(vec.X(), vec.Y(), vec.Z());
}
//#ifdef USEQT
string vector3D::toString(int pt) const
{
  string ret = point3D::toString(pt);
  if (!isLine())
    ret += "T";
  return ret;
}
//#endif
ostream& operator<<(ostream& o, const vector3D& v)
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
      o << "(" << v.X() << ", " << v.Y() << ", " << v.Z() << ")";
      break;
    }
    case _spheric_: {
      o << "(" << v.Phi() << ", " << v.Theta() << ", " << v.R() << ")";
      break;
    }
    case _cylindric_: {
      o << "(" << v.Phi() << ", " << v.Rho() << ", " << v.Z() << ")";
    }
    }
  }
  }
  if (v.isColumn())
    o << "T";
  return o;
}

// vector3D::vector3D(): point3D(), Vector(0)
// {
// }

// vector3D::vector3D(const point3D &p): point3D(p), Vector(0)
// {
// }

// vector3D::vector3D(double x, double y, double z): point3D(x, y, z), Vector(0)
// {
// }

// vector3D vector3D::operator + (const vector3D &w)const
// {
//   return vector3D(point3D::value1+w.X(), point3D::value2+w.Y(),
//   point3D::value3+w.Z());
// }

// vector3D vector3D::operator - (const vector3D &w)const
// {
//   return vector3D(point3D::value1-w.X(), point3D::value2-w.Y(),
//   point3D::value3-w.Z());
// }

// vector3D vector3D::operator * (double f)const
// {
//   vector3D ret;
//   ret.setValues(f*point3D::value1, f*point3D::value2, f*point3D::value3);
//   return ret;
// }

// double vector3D::operator * (const vector3D &w)const
// {
//   return (point3D::value1*w.X()+point3D::value2*w.Y()+point3D::value3*w.Z());
// }

// double vector3D::operator[](int pos)const
// {
//   switch(pos)
//     {
//     case 0: return point3D::value1;
//     case 1: return point3D::value2;
//     case 2: return point3D::value3;
//     }
//   return -1;
// }

// vector3D vector3D::operator ^ (const vector3D &w)const
// {
//     return vector3D(point3D::value2*w.Z()-point3D::value3*w.Y(),
// 		    point3D::value3*w.X()-point3D::value1*w.Z(),
// 		    point3D::value1*w.Y()-point3D::value2*w.X());
// }

int vector3D::operator==(const vector3D& vec) const
{
  return ((point3D::value1 == vec.X()) && (point3D::value2 == vec.Y()) &&
          (point3D::value3 == vec.Z()));
}

// vector3D vector3D::operator !()const
// {
//     vector3D vec(*this);
//     vec.transpose();
//     return vec;
// }

vector3D operator-(const point3D& p, const point3D& q)
{
  return vector3D(p.X() - q.X(), p.Y() - q.Y(), p.Z() - q.Z());
}

point3D operator-(const point3D& p, const vector3D& v)
{
  return point3D(p.X() - v.X(), p.Y() - v.Y(), p.Z() - v.Z());
}

point3D operator+(const point3D& p, const vector3D& v)
{
  return point3D(p.X() + v.X(), p.Y() + v.Y(), p.Z() + v.Z());
}

vector3D operator*(double f, const vector3D& vec)
{
  return vector3D(vec.X() * f, vec.Y() * f, vec.Z() * f);
}

// vector3D operator*(const vector3D &v, double f)
// {
//   return vector3D(v.X()*f,v.Y()*f,v.Z()*f);
// }

// void vector3D::Zero()
// {
//     point3D::Zero();
// }

// void vector3D::Infinite()
// {
//     point3D::Infinite();
// }

// void vector3D::Undefined()
// {
//     point3D::Undefined();
// }

// void vector3D::Regular()
// {
//     point3D::Regular();
// }

// void vector3D::setValues(double x, double y, double z)
// {
//     point3D::setValues(x,y,z);
// }

// void vector3D::normalize()
// {
//     if(point3D::f_r==0)return;
//     point3D::setValues(point3D::value1/point3D::f_r,point3D::value2/point3D::f_r,point3D::value3/point3D::f_r);
// }

// double vector3D::length() const
// {
//     return point3D::f_r;
// }

// geomState vector3D::getState() const
// {
//     return point3D::getState();
// }

// double vector3D::getValue(int pos) const
// {
//     return point3D::getValue(pos);
// }

// void vector3D::setValue(int pos, double valueIn)
// {
//     point3D::setValue(pos,valueIn);
// }

double vector3D::angle(const vector3D& vec, const vector3D& w) const
{
  vector3D w1(w), v1(vec);
  vector3D ref(*this);
  ref.normalize();
  w1.normalize();
  v1.normalize();
  double cosA = (w1 * v1);
  if (cosA == 1)
    return 0;
  if (cosA == -1)
    return M_PI;
  vector3D u = w1 ^ v1;
  double ra = u * ref;
  if (ra < 0) {
    return -acos(cosA);
  } else if (ra > 0) {
    return acos(cosA);
  } else {
    return acos(cosA);
  }
}
// int vector3D::getDimension() const{return 3;}
