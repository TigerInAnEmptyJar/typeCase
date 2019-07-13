#include "geometry.h"
#include <typeinfo>
#ifdef USEROOT
#include <TObject.h>
#endif
point::point(int dim) : geomObject(), dimension(dim)
{
  if (dimension <= 20)
    value = NULL;
  else
    value = new double[dimension - 20];
  Zero();
  Undefined();
  infinityPos = -1;
}

point::point(const point& p) : geomObject(), dimension(p.getDimension())
{
  if (dimension <= 20)
    value = NULL;
  else
    value = new double[dimension - 20];
  if (p.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    return;
  }
  if (p.getState() == _zero_) {
    Zero();
    return;
  }
  if (p.getState() == _infinite_) {
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    for (int i = 0; i < dimension; i++)
      if (!(p[i] < 0 || p[i] >= 0))
        infinityPos = i;
    Infinite();
    return;
  }
  if (p.getState() == _undefined_)
    Undefined();
}

point::point(const point2D& p) : geomObject(), dimension(2)
{
  if (dimension <= 20)
    value = NULL;
  else
    value = new double[dimension - 20];
  if (p.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    return;
  }
  if (p.getState() == _zero_) {
    Zero();
    return;
  }
  if (p.getState() == _infinite_) {
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    for (int i = 0; i < dimension; i++)
      if (!(p[i] < 0 || p[i] >= 0))
        infinityPos = i;
    Infinite();
    return;
  }
  if (p.getState() == _undefined_)
    Undefined();
}

point::point(const point3D& p) : geomObject(), dimension(3)
{
  if (dimension <= 20)
    value = NULL;
  else
    value = new double[dimension - 20];
  if (p.getState() == _regular_) {
    Regular();
    if (dimension <= 20) {
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    } else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    return;
  }
  if (p.getState() == _zero_) {
    Zero();
    return;
  }
  if (p.getState() == _infinite_) {
    for (int i = 0; i < dimension; i++)
      if (dimension <= 20)
        for (int i = 0; i < dimension; i++)
          v[i] = p.getValue(i);
      else {
        for (int i = 0; i < 20; i++)
          v[i] = p.getValue(i);
        for (int i = 20; i < dimension; i++)
          value[i - 20] = p.getValue(i);
      }
    for (int i = 0; i < dimension; i++)
      if (!(p[i] < 0 || p[i] >= 0))
        infinityPos = i;
    Infinite();
    return;
  }
  if (p.getState() == _undefined_)
    Undefined();
}

point::point(const point4D& p) : geomObject(), dimension(4)
{
  if (dimension <= 20)
    value = NULL;
  else
    value = new double[dimension - 20];
  if (p.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    return;
  }
  if (p.getState() == _zero_) {
    Zero();
    return;
  }
  if (p.getState() == _infinite_) {
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    for (int i = 0; i < dimension; i++)
      if (!(p[i] < 0 || p[i] >= 0))
        infinityPos = i;
    Infinite();
    return;
  }
  if (p.getState() == _undefined_)
    Undefined();
}

void point::setValue(int pos, double valueIn)
{
  if (dimension == 0)
    return;
  if (getState() != _infinite_) {
    if ((pos < dimension) && (pos >= 0)) {
      if (pos < 20)
        v[pos] = valueIn;
      else
        value[pos - 20] = valueIn;
    }
    if (valueIn != 0)
      Regular();
    else {
      int test = 0;
      if (dimension <= 20) {
        for (int i = 0; i < dimension; i++)
          if (v[i] != 0)
            test = 1;
      } else {
        for (int i = 0; i < 20; i++)
          if (v[i] != 0)
            test = 1;
        for (int i = 20; i < dimension; i++)
          if (value[i - 20] != 0)
            test = 1;
      }
      if (!test)
        Zero();
    }
  } else {
    if ((pos < dimension) && (pos >= 0)) {
      if (pos < 20)
        v[pos] = valueIn;
      else
        value[pos - 20] = valueIn;
    }
    if (pos == infinityPos)
      Regular();
  }
}

void point::operator=(const point& p)
{
  if (dimension != p.getDimension() && dimension > 0)
    return;
  if (p.getState() == _zero_) {
    Zero();
    return;
  }
  if (p.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  if (p.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        v[i] = p.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        v[i] = p.getValue(i);
      for (int i = 20; i < dimension; i++)
        value[i - 20] = p.getValue(i);
    }
    return;
  }
  Infinite();
  if (dimension <= 20)
    for (int i = 0; i < dimension; i++)
      v[i] = p.getValue(i);
  else {
    for (int i = 0; i < 20; i++)
      v[i] = p.getValue(i);
    for (int i = 20; i < dimension; i++)
      value[i - 20] = p.getValue(i);
  }
  for (int i = 0; i < dimension; i++)
    if (!(p[i] < 0 || p[i] >= 0))
      infinityPos = i;
}
//#ifdef USEQT
string point::toString(int) const
{
  if (getState() == _undefined_ || getState() == _infinite_)
    return geomObject::toString();
  string ret = "(";
  for (int i = 0; i < dimension; i++)
    ret += string_number(getValue(i)) + ", ";
  ret = ret.substr(0, ret.length() - 2);
  ret += ")";
  return ret;
}
//#endif
void point::reDimension(int newDim)
{
  if (typeid(*this) == typeid(point) || typeid(*this) == typeid(Vector)) {
    if (dimension > 20)
      delete[] value;
    dimension = newDim;
    if (dimension <= 20)
      value = NULL;
    else
      value = new double[dimension - 20];
    Zero();
    Undefined();
    infinityPos = -1;
  }
}
ostream& operator<<(ostream& o, const point& p)
{
  switch (p.getState()) {
  case _zero_:
  case _regular_: {
    o << "(";
    for (int i = 0; i < p.getDimension() - 1; i++)
      o << p.getValue(i) << ", ";
    o << p.getValue(p.getDimension() - 1) << ")";
    break;
  }
  case _infinite_: {
    o << "(infinite)";
    break;
  }
  case _undefined_: {
    o << "(undefined)";
    break;
  }
  }
  return o;
}
// point::~point()
// {
//   if(dimension>=20)
//     delete []value;
// }

void point::setValues(double values[])
{
  Regular();
  if (dimension <= 20)
    for (int i = 0; i < dimension; i++)
      v[i] = values[i];
  else {
    for (int i = 0; i < 20; i++)
      v[i] = values[i];
    for (int i = 20; i < dimension; i++)
      value[i - 20] = values[i];
  }
}

void point::Zero()
{
  geomObject::Zero();
  if (dimension <= 20)
    for (int i = 0; i < dimension; i++)
      v[i] = 0;
  else {
    for (int i = 0; i < 20; i++)
      v[i] = 0;
    for (int i = 20; i < dimension; i++)
      value[i - 20] = 0;
  }
}

// double point::operator[](int pos)const
// {
//   if(pos>=dimension||pos<0)return 0;
//   if(pos<20)
//     return v[pos];
//   else
//     return value[pos-20];
// }

// double point::getValue(int pos)const
// {
//   if(pos>=dimension||pos<0)return 0;
//   if(pos<20)
//     return v[pos];
//   else
//     return value[pos-20];
// }

// int point::getDimension()const
// {
//     return dimension;
// }

// geomState point::getState()const
// {
//     return geomObject::getState();
// }
Vector point::operator-(const point& q)
{
  if (getDimension() != q.getDimension())
    return Vector(1);
  Vector temp(getDimension());
  for (int i = 0; i < getDimension(); i++)
    temp.setValue(i, getValue(i) - q.getValue(i));
  return temp;
}

point point::operator-(const Vector& w)
{
  if (getDimension() != w.getDimension())
    return point(1);
  point temp(w.getDimension());
  for (int i = 0; i < w.getDimension(); i++)
    temp.setValue(i, getValue(i) - w.getValue(i));
  return temp;
}

point point::operator+(const Vector& w)
{
  if (getDimension() != w.getDimension())
    return point(1);
  point temp(w.getDimension());
  for (int i = 0; i < w.getDimension(); i++)
    temp.setValue(i, getValue(i) + w.getValue(i));
  return temp;
}

int point::operator==(const point& p)
{
  if (getDimension() != p.getDimension())
    return 0;
  int j = 1;
  for (int i = 0; i < getDimension(); i++)
    j = (j && (getValue(i) == p.getValue(i)));
  return j;
}

void point::setValues(double f1, double f2, double f3, double f4, double f5, double f6, double f7,
                      double f8, double f9, double f10, double f11, double f12, double f13,
                      double f14, double f15, double f16, double f17, double f18, double f19,
                      double f20)
{
  Regular();
  if (dimension > 20) {
    v[0] = f1;
    v[1] = f2;
    v[2] = f3;
    v[3] = f4;
    v[4] = f5;
    v[5] = f6;
    v[6] = f7;
    v[7] = f8;
    v[8] = f9;
    v[9] = f10;
    v[10] = f11;
    v[11] = f12;
    v[12] = f13;
    v[13] = f14;
    v[14] = f15;
    v[15] = f16;
    v[16] = f17;
    v[17] = f18;
    v[18] = f19;
    v[19] = f20;
    return;
  }
  switch (dimension) {
  case 20:
    v[19] = f20;
  case 19:
    v[18] = f19;
  case 18:
    v[17] = f18;
  case 17:
    v[16] = f17;
  case 16:
    v[15] = f16;
  case 15:
    v[14] = f15;
  case 14:
    v[13] = f14;
  case 13:
    v[12] = f13;
  case 12:
    v[11] = f12;
  case 11:
    v[10] = f11;
  case 10:
    v[9] = f10;
  case 9:
    v[8] = f9;
  case 8:
    v[7] = f8;
  case 7:
    v[6] = f7;
  case 6:
    v[5] = f6;
  case 5:
    v[4] = f5;
  case 4:
    v[3] = f4;
  case 3:
    v[2] = f3;
  case 2:
    v[1] = f2;
  case 1:
    v[0] = f1;
  }
}
