#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif

point3D::point3D(const point& p) : point(0)
{
  repre = _cartesic3_;
  if (p.getDimension() != 3)
    return;
  switch (p.getState()) {
  case _zero_:
  case _regular_:
  case _infinite_: {
    setValues(p[0], p[1], p[2]);
    break;
  }
  case _undefined_: {
    Zero();
    Undefined();
    break;
  }
  }
}

#ifdef USEROOT
point3D::point3D(const point3D& p) : point(0)
#else
point3D::point3D(const point3D& p) : point(0)
#endif
{
  repre = _cartesic3_;
  switch (p.getState()) {
  case _zero_:
  case _regular_:
  case _infinite_: {
    setValues(p.X(), p.Y(), p.Z());
    break;
  }
  case _undefined_: {
    Zero();
    Undefined();
    break;
  }
  }
  setRep(p.getRep());
}

void point3D::setValues(double x, double y, double z)
{
  Regular();
  value1 = x;
  value2 = y;
  value3 = z;
  if ((x == 0) && (y == 0) && (z == 0)) {
    Zero();
    f_r = 0;
    f_phi = 0;
    f_theta = 0;
    f_rho = 0;
    return;
  }
  if ((!(x < 0 || x >= 0)) || (!(y < 0 || y >= 0)) || (!(z < 0 || z >= 0))) {
    //  	value[0]= x;
    //  	value[1]= y;
    //  	value[2]= z;
    f_r = nanf("nan");
    infinityPos = ((!(x < 0 || x >= 0)) ? 0 : (!(y < 0 || y >= 0)) ? 1 : 2);
    f_rho = (infinityPos < 1 ? nanf("nan") : sqrt(value1 * value1 + value2 * value2));
    f_phi = (infinityPos == 0 ? 0 : M_PI / 2.);
    f_theta = (infinityPos == 2 ? 0 : M_PI / 2);
    Infinite();
    return;
  }
  //     value[0]= x;
  //     value[1]= y;
  //     value[2]= z;
  f_r = sqrt(value1 * value1 + value2 * value2 + value3 * value3);
  f_rho = sqrt(value1 * value1 + value2 * value2);
  if (f_rho == 0)
    f_phi = 0;
  else
    f_phi = acos(value1 / f_rho);
  if (value2 < 0)
    f_phi = -f_phi;
  if (f_r == 0)
    f_theta = 0;
  else
    f_theta = acos(value3 / f_r);
}

// double point3D::R()const
// {
//     return f_r;
// }

void point3D::operator=(const point& p)
{
  if (p.getDimension() != 3)
    return;
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  setValues(p[0], p[1], p[2]);
}

void point3D::operator=(const point3D& p)
{
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  setValues(p[0], p[1], p[2]);
}
//#ifdef USEQT
string point3D::toString(int pt) const
{
  if (getState() != _regular_)
    return point::toString();
  rep3D re = repre;
  switch (pt) {
  case _cartesic3_:
    re = _cartesic3_;
    break;
  case _spheric_:
    re = _spheric_;
    break;
  case _cylindric_:
    re = _cylindric_;
    break;
  }
  if (re == _cartesic3_)
    return (string("(") + string_number(value1) + ", " + string_number(value2) + ", " +
            string_number(value3) + ")");
  else if (re == _spheric_)
    return (string("(") + string_number(f_theta) + ", " + string_number(f_phi) + ", " +
            string_number(f_r) + ")");
  else
    return (string("(") + string_number(f_phi) + ", " + string_number(f_rho) + ", " +
            string_number(value3) + ")");
}
//#endif
ostream& operator<<(ostream& o, const point3D& p)
{
  switch (p.getState()) {
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
    switch (p.getRep()) {
    case _cartesic3_: {
      o << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";
      break;
    }
    case _spheric_: {
      o << "(" << p.Phi() << ", " << p.Theta() << ", " << p.R() << ")";
      break;
    }
    case _cylindric_: {
      o << "(" << p.Phi() << ", " << p.Rho() << ", " << p.Z() << ")";
    }
    }
  }
  }
  return o;
}

// point3D::point3D():point(0)
// {
//     repre = _cartesic3_;
// }

// point3D::point3D(double x, double y, double z):point(0)
// {
//     repre = _cartesic3_;
//     setValues(x, y, z);
// }

// double point3D::X()const
// {
//     return value1;
// }

// double point3D::Y()const
// {
//     return value2;
// }

// double point3D::Z()const
// {
//     return value3;
// }

// double point3D::Phi()const
// {
//     return f_phi;
// }

// double point3D::Theta()const
// {
//     return f_theta;
// }
// double point3D::Rho()const
// {
//     return f_rho;
// }

// rep3D point3D::getRep()const
// {
//     return repre;
// }

// void point3D::setRep(rep3D r)
// {
//     repre = r;
// }

// vector3D point3D::operator - (const point3D& q)const
// {
//     return vector3D(value1-q.X(), value2-q.Y(), value3-q.Z());
// }

// point3D point3D::operator - (const vector3D& v)const
// {
//     return point3D(value1-v.X(), value2-v.Y(), value3-v.Z());
// }

// point3D point3D::operator + (const vector3D &v)const
// {
//     return point3D(value1+v.X(), value2+v.Y(), value3+v.Z());
// }

int point3D::operator==(const point3D& p) const
{
  return ((value1 - p.X() < __prec) && (value2 - p.Y() < __prec) && (value3 - p.Z() < __prec));
}

// int point3D::getDimension() const{return 3;}
// double point3D::getValue(int pos) const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     case 2:return value3;
//     }
//   return 0;
// }
// double point3D::operator [](int pos)const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     case 2:return value3;
//     }
//   return -1;
// }
void point3D::setValue(int pos, double valueIn)
{
  switch (pos) {
  case 0:
    value1 = valueIn;
    break;
  case 1:
    value2 = valueIn;
    break;
  case 2:
    value3 = valueIn;
    break;
  }
}
// double point3D::x() const
// {
//   return value1;
// }
// double point3D::y() const
// {
//   return value2;
// }
// double point3D::z() const
// {
//   return value3;
// }
// double point3D::r() const
// {
//   return f_r;
// }
// double point3D::theta() const
// {
//   return f_theta;
// }
// double point3D::phi() const
// {
//   return f_phi;
// }
// double point3D::rho() const
// {
//   return f_rho;
// }
// void point3D::setThetaPhiR(double th, double ph, double rIN)
// {
//   value1=rIN*sin(th)*cos(ph);
//   value2=rIN*sin(th)*sin(ph);
//   value3=rIN*cos(th);
//   f_theta=th;
//   f_phi=ph;
//   f_r=rIN;
//   f_rho=rIN*sin(th);
// }
