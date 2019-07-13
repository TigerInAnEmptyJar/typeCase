#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
#ifdef USEROOT
Vector::Vector(const Vector& w) : point(0) // v.getDimension())
#else
Vector::Vector(const Vector& w) : point(0) // v.getDimension())
#endif
{
  reDimension(w.getDimension());
  kind = w.isLine();
  if (w.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        point::v[i] = w.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        point::v[i] = w.getValue(i);
      for (int i = 20; i < dimension; i++)
        point::value[i - 20] = w.getValue(i);
    }
    return;
  }
  if (w.getState() == _zero_) {
    point::Zero();
    return;
  }
  if (w.getState() == _infinite_) {
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        point::v[i] = w.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        point::v[i] = w.getValue(i);
      for (int i = 20; i < dimension; i++)
        point::value[i - 20] = w.getValue(i);
    }
    for (int i = 0; i < dimension; i++)
      if (w.getValue(i) == nan("nan"))
        infinityPos = i;
    Infinite();
    return;
  }
  if (w.getState() == _undefined_)
    Undefined();
}
//#ifdef USEQT
string Vector::toString(int) const
{
  string ret = point::toString();
  if (!kind)
    ret += "T";
  return ret;
}
//#endif
void Vector::normalize()
{
  if (point::getState() != _regular_)
    return;
  double tmp = length();
  if (dimension <= 20)
    for (int i = 0; i < dimension; i++)
      point::v[i] = point::v[i] / tmp;
  else {
    for (int i = 0; i < 20; i++)
      point::v[i] = point::v[i] / tmp;
    for (int i = 20; i < dimension; i++)
      point::value[i - 20] = point::value[i - 20] / tmp;
  }
}

double Vector::length() const
{
  if (point::getState() == _zero_)
    return 0;
  if (point::getState() != _regular_)
    return -1;
  double tmp = 0;
  for (int i = 0; i < point::dimension; i++)
    tmp = tmp + point::getValue(i) * point::getValue(i);
  return sqrt(tmp);
}

void Vector::operator=(const point& p)
{
  if (point::getDimension() != p.getDimension())
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
    if (p.getValue(i) == nan("nan"))
      infinityPos = i;
}

void Vector::operator=(const Vector& w)
{
  if (point::dimension != w.getDimension())
    return;
  kind = w.isLine();
  if (w.getState() == _zero_) {
    Zero();
    return;
  }
  if (w.getState() == _undefined_) {
    Zero();
    Undefined();
    return;
  }
  if (w.getState() == _regular_) {
    Regular();
    if (dimension <= 20)
      for (int i = 0; i < dimension; i++)
        point::v[i] = w.getValue(i);
    else {
      for (int i = 0; i < 20; i++)
        point::v[i] = w.getValue(i);
      for (int i = 20; i < dimension; i++)
        point::value[i - 20] = w.getValue(i);
    }
    return;
  }
  Infinite();
  if (dimension <= 20)
    for (int i = 0; i < dimension; i++)
      point::v[i] = w.getValue(i);
  else {
    for (int i = 0; i < 20; i++)
      point::v[i] = w.getValue(i);
    for (int i = 20; i < dimension; i++)
      point::value[i - 20] = w.getValue(i);
  }
  for (int i = 0; i < dimension; i++)
    if (w.getValue(i) == nan("nan"))
      infinityPos = i;
}

ostream& operator<<(ostream& o, const Vector& v)
{
  switch (v.getState()) {
  case _zero_:
  case _regular_: {
    o << "(";
    for (int i = 0; i < v.getDimension() - 1; i++)
      o << v.getValue(i) << ", ";
    o << v.getValue(v.getDimension() - 1) << ")";
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
  if (v.isColumn())
    o << "T";
  return o;
}

// Vector::Vector (int dim):point(dim)
// {
//     kind = 1;
// }

// Vector::Vector (const point &p):point(p)
// {
//     kind = 1;
// }

// void Vector::transpose()
// {
//     kind = !kind;
// }

// int Vector::isLine()const
// {
//     return kind;
// }

// int Vector::isColumn()const
// {
//     return (!kind);
// }

// void Vector::Undefined()
// {
//     point::Undefined();
// }
Vector Vector::operator+(const Vector& w) const
{
  if (point::dimension != w.getDimension())
    return Vector(1);
  Vector temp(dimension);
  if (isColumn())
    temp.transpose();
  for (int i = 0; i < point::dimension; i++)
    temp.setValue(i, point::getValue(i) + w.getValue(i));
  return temp;
}
Vector Vector::operator%(const Vector& w) const
{
  if (point::dimension != w.getDimension())
    return Vector(1);
  Vector temp(dimension);
  if (isColumn())
    temp.transpose();
  for (int i = 0; i < point::dimension; i++)
    temp.setValue(i, point::getValue(i) * w.getValue(i));
  return temp;
}

Vector Vector::operator-(const Vector& w) const
{
  if (point::dimension != w.getDimension())
    return Vector(1);
  Vector temp(dimension);
  if (isColumn())
    temp.transpose();
  for (int i = 0; i < point::dimension; i++)
    temp.setValue(i, point::getValue(i) - w.getValue(i));
  return temp;
}

Vector Vector::operator*(double f) const
{
  Vector temp(point::dimension);
  if (isColumn())
    temp.transpose();
  for (int i = 0; i < point::dimension; i++)
    temp.setValue(i, f * point::getValue(i));
  return temp;
}

double Vector::operator*(const Vector& w) const
{
  if (w.getDimension() != point::dimension)
    return 0;
  double temp = 0;
  for (int i = 0; i < point::dimension; i++)
    temp = temp + w.getValue(i) * point::getValue(i);
  return temp;
}
Vector operator*(double f, const Vector& v)
{
  Vector temp(v.getDimension());
  if (v.isColumn())
    temp.transpose();
  for (int i = 0; i < v.getDimension(); i++)
    temp.setValue(i, f * v[i]);
  return temp;
}
Vector operator/(const Vector& v, double f)
{
  Vector temp(v.getDimension());
  if (v.isColumn())
    temp.transpose();
  for (int i = 0; i < v.getDimension(); i++)
    temp.setValue(i, v[i] / f);
  return temp;
}
// Vector Vector::operator !()
// {
//     Vector v(*this);
//     v.transpose();
//     return v;
// }

int Vector::operator==(const Vector& p) const
{
  if (point::dimension != p.getDimension())
    return 0;
  int j = 1;
  for (int i = 0; i < point::dimension; i++)
    j = (j && (point::getValue(i) - p.getValue(i) < __prec));
  return j;
}
