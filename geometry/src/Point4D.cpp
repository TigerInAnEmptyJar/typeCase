#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
// point4D::point4D(): point(0)
// {
//     repre = _cartesic3_;
// }

point4D::point4D(const point3D& p, double w) : point(0)
{
  setRep(p.getRep());
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  setValues(p.X(), p.Y(), p.Z(), w);
}

#ifdef USEROOT
point4D::point4D(const point4D& p) : point(0)
#else
point4D::point4D(const point4D& p) : point(0)
#endif
{
  setRep(p.getRep());
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  setValues(p.X(), p.Y(), p.Z(), p.W());
}

point4D::point4D(const point& p) : point(0)
{
  repre = _cartesic3_;
  if ((p.getState() == _undefined_) || ((p.getDimension() != 4) && (p.getDimension() != 3))) {
    Zero();
    Undefined();
    return;
  }
  setValues(p[0], p[1], p[2], (p.getDimension() == 3 ? 0 : p[3]));
}
point4D::point4D(const point& p, double w) : point(0)
{
  repre = _cartesic3_;
  if ((p.getState() == _undefined_) || (p.getDimension() != 3)) {
    Zero();
    Undefined();
    return;
  }
  setValues(p[0], p[1], p[2], w);
}

// point4D::point4D(double x, double y, double z, double w): point(0)
// {
//     repre = _cartesic3_;
//     setValues(x, y, z, w);
// }

void point4D::setValues(double x, double y, double z, double w)
{
  Regular();
  value1 = x;
  value2 = y;
  value3 = z;
  value4 = w;
  if ((x == 0) && (y == 0) && (z == 0) && (w == 0)) {
    Zero();
    f_theta = 0;
    f_phi = 0;
    f_rho = 0;
    f_r = 0;
    return;
  }
  if ((!(x < 0 || x >= 0)) || (!(y < 0 || y >= 0)) || (!(z < 0 || z >= 0)) ||
      (!(w < 0 || w >= 0))) {
    Infinite();
    infinityPos =
        ((!(x < 0 || x >= 0)) ? 0 : ((!(y < 0 || y >= 0)) ? 1 : ((!(z < 0 || z >= 0)) ? 2 : 3)));

    f_phi = (infinityPos == 0 ? 0 : M_PI / 2.);
    f_r = (infinityPos == 3 ? sqrt(value1 * value1 + value2 * value2 + value3 * value3)
                            : nanf("nan"));
    f_rho = (infinityPos == 0 || infinityPos == 1 ? nanf("nan")
                                                  : sqrt(value1 * value1 + value2 * value2));
    f_theta = ((infinityPos == 2) ? 0 : M_PI / 2);
    return;
  }
  f_r = sqrt(value1 * value1 + value2 * value2 + value3 * value3);
  f_rho = sqrt(value1 * value1 + value2 * value2);
  f_phi = (f_rho == 0 ? 0 : acos(value1 / f_rho));
  f_theta = (f_r == 0 ? 0 : acos(value3 / f_r));
}

void point4D::setValues(double x, double y, double z)
{
  double w = 0;
  Regular();
  value1 = x;
  value2 = y;
  value3 = z;
  value4 = w;
  if ((x == 0) && (y == 0) && (z == 0) && (w == 0)) {
    Zero();
    f_theta = 0;
    f_phi = 0;
    f_rho = 0;
    f_r = 0;
    return;
  }
  if ((!(x < 0 || x >= 0)) || (!(y < 0 || y >= 0)) || (!(z < 0 || z >= 0)) ||
      (!(w < 0 || w >= 0))) {
    Infinite();
    infinityPos =
        ((!(x < 0 || x >= 0)) ? 0 : ((!(y < 0 || y >= 0)) ? 1 : ((!(z < 0 || z >= 0)) ? 2 : 3)));

    f_phi = (infinityPos == 0 ? 0 : M_PI / 2.);
    f_r = (infinityPos == 3 ? sqrt(value1 * value1 + value2 * value2 + value3 * value3)
                            : nanf("nan"));
    f_rho = (infinityPos == 0 || infinityPos == 1 ? nanf("nan")
                                                  : sqrt(value1 * value1 + value2 * value2));
    f_theta = ((infinityPos == 2) ? 0 : M_PI / 2);
    return;
  }
  f_r = sqrt(value1 * value1 + value2 * value2 + value3 * value3);
  f_rho = sqrt(value1 * value1 + value2 * value2);
  f_phi = (f_rho == 0 ? 0 : acos(value1 / f_rho));
  f_theta = (f_r == 0 ? 0 : acos(value3 / f_r));
}

// double point4D::W()const
// {
//     return value4;
// }

// double point4D::X()const
// {
//     return value1;
// }

// double point4D::Y()const
// {
//     return value2;
// }

// double point4D::Z()const
// {
//     return value3;
// }

// double point4D::R()const
// {
//   return f_r;
// }

// double point4D::Phi()const
// {
//   return f_phi;
// }

// double point4D::Theta()const
// {
//   return f_theta;
// }

// rep3D point4D::getRep()const
// {
//     return repre;
// }

// void point4D::setRep(rep3D r)
// {
//     repre = r;
// }

void point4D::operator=(const point& p)
{
  if ((p.getDimension() != 4) || (p.getDimension() != 3))
    return;
  setValues(p[0], p[1], p[2], (p.getDimension() == 3 ? W() : p.getValue(3)));
}

// void point4D::operator = (const point3D &p)
// {
//   setValues(p[0], p[1], p[2], W());
// }

void point4D::operator=(const point4D& p)
{
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  setValues(p.X(), p.Y(), p.Z(), p.W());
}
//#ifdef USEQT
string point4D::toString(int pt) const
{
  if (getState() != _regular_ && getState() != _zero_)
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
  string ret = point3D(value1, value2, value3).toString(re);
  ret = ret.substr(0, ret.length() - 1) + ", ";
  ret += string_number(W()) + ")";
  return ret;
}
//#endif
ostream& operator<<(ostream& o, const point4D& p)
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
      o << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ", " << p.W() << ")";
      break;
    }
    case _spheric_: {
      o << "(" << p.Phi() << ", " << p.Theta() << ", " << p.R() << ", " << p.W() << ")";
      break;
    }
    case _cylindric_: {
      o << "(" << p.Phi() << ", " << p.Rho() << ", " << p.Z() << ", " << p.W() << ")";
    }
    }
  }
  }
  return o;
}

// int point4D::operator == (const point4D & p)
// {
//     return
//     ((value1-p.X()<__prec)&&(value2-p.Y()<__prec)&&(value3-p.Z()<__prec)&&(W()-p.W()<__prec));
// }

// double point4D::Rho() const
// {
//   return f_rho;
// }
// int point4D::getDimension() const{return 4;}
// double point4D::getValue(int pos) const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     case 2:return value3;
//     case 3:return value4;
//     }
//   return 0;
// }
// double point4D::operator [](int pos)const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     case 2:return value3;
//     case 3:return value4;
//     }
//   return -1;
// }
// void point4D::setValue(int pos, double valueIn)
// {
//   switch(pos)
//     {
//     case 0:value1=valueIn;break;
//     case 1:value2=valueIn;break;
//     case 2:value3=valueIn;break;
//     case 3:value4=valueIn;break;
//     }
// }
// double point4D::x() const
// {
//   return value1;
// }
// double point4D::y() const
// {
//   return value2;
// }
// double point4D::z() const
// {
//   return value3;
// }
// double point4D::w() const
// {
//   return value4;
// }
// double point4D::r() const
// {
//   return f_r;
// }
// double point4D::theta() const
// {
//   return f_theta;
// }
// double point4D::phi() const
// {
//   return f_phi;
// }
// double point4D::rho() const
// {
//   return f_rho;
// }
