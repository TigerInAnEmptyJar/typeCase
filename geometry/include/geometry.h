#ifndef GEOMETRY
#define GEOMETRY
#include "definitions.h"
#include "string_utilities.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;

const double __prec = 1e-6;
// possible status for a geometrical object
enum geomState
{
  _zero_,
  _regular_,
  _infinite_,
  _undefined_
};
// representation of the 2D points in output
enum repTD
{
  _cartesic_,
  _polar_
};
// representation of the 3D points in output
enum rep3D
{
  _cartesic3_,
  _spheric_,
  _cylindric_
};
// representation of planes in 3D in output
enum planeRep
{
  _normal_,
  _parametric_
};

/*****Forward declarations*/
class Vector;
class point2D;
class vector2D;
class point3D;
class vector3D;
class point4D;
class vector4D;
class point4D;
class momentum4D;

/*****Base class for all geometric objects **************/
class geomObject
{
private:
  geomState status;

public:
  geomObject() { status = _undefined_; }
  virtual ~geomObject() {}
  virtual geomState getState() const { return status; }
  virtual void Zero() { status = _zero_; }
  virtual void Infinite() { status = _infinite_; }
  virtual void Undefined() { status = _undefined_; }
  virtual void Regular() { status = _regular_; }
  virtual string toString() const;
};

/*****Base point class - for points in n dimensional space*/

class point : public geomObject
{
protected:
  int dimension;
  int infinityPos;
  double v[20];  //[20]
  double* value; //[dimension]
public:
  point(int dim = 0);
  point(const point& p);
  point(const point2D& p);
  point(const point3D& p);
  point(const point4D& p);
  virtual ~point()
  {
    if (dimension >= 20)
      delete[] value;
  }
  virtual void reDimension(int newDim);
  void setValues(double values[]);
  void setValues(double f1 = 0, double f2 = 0, double f3 = 0, double f4 = 0, double f5 = 0,
                 double f6 = 0, double f7 = 0, double f8 = 0, double f9 = 0, double f10 = 0,
                 double f11 = 0, double f12 = 0, double f13 = 0, double f14 = 0, double f15 = 0,
                 double f16 = 0, double f17 = 0, double f18 = 0, double f19 = 0, double f20 = 0);
  virtual void setValue(int pos, double valueIn);
  virtual void Zero();
  virtual double getValue(int pos) const
  {
    if (pos >= dimension || pos < 0)
      return 0;
    if (pos < 20)
      return v[pos];
    else
      return value[pos - 20];
  }
  virtual int getDimension() const { return dimension; }
  virtual geomState getState() const { return geomObject::getState(); }
  void operator=(const point& p);
  Vector operator-(const point& q);
  point operator-(const Vector& v);
  point operator+(const Vector& v);
  int operator==(const point& p);
  virtual double operator[](int pos) const
  {
    if (pos >= dimension || pos < 0)
      return 0;
    if (pos < 20)
      return v[pos];
    else
      return value[pos - 20];
  }
  virtual string toString() const;
};

ostream& operator<<(ostream& o, const point& p);

/*****Base vector class - for vectors in n dimensional space*/

class Vector : public point
{
private:
  int kind;

protected:
  virtual void Undefined() { point::Undefined(); }

public:
  Vector(int dim = 0) : point(dim) { kind = 1; }
  Vector(const Vector& v);
  Vector(const point& p) : point(p) { kind = 1; }
  /*     Vector (const vector3D v); */
  /*     Vector (const vector2D v); */
  /*     Vector (const vector4D v); */
  /*     Vector (const momentum4D v); */
  virtual void normalize();
  void transpose() { kind = !kind; }
  int isLine() const { return kind; }
  int isColumn() const { return !kind; }
  virtual double length() const;
  void operator=(const point& p);
  void operator=(const Vector& v);
  Vector operator+(const Vector& w) const;
  Vector operator-(const Vector& w) const;
  Vector operator*(double f) const;
  Vector operator%(const Vector& w) const;
  double operator*(const Vector& w) const;
  //    Vector operator ^ (const Vector &w);
  int operator==(const Vector& v) const;
  Vector operator!()
  {
    Vector v(*this);
    v.transpose();
    return v;
  }
  virtual string toString() const;
};
Vector operator*(double f, const Vector& v);
Vector operator/(const Vector& v, double f);
ostream& operator<<(ostream& o, const Vector& v);

/*****Point class for points in 2-dimensional Space*/

class point2D : public point
{
private:
  repTD repre;

protected:
  double f_phi;
  double f_r;
  double value1, value2;

public:
  point2D() {}
  point2D(const point& p);
  point2D(const point2D& p);
  point2D(double x, double y);
  virtual void setValues(double x, double y);
  virtual void setValue(int pos, double valueIn)
  {
    switch (pos) {
    case 0:
      value1 = valueIn;
      break;
    case 1:
      value2 = valueIn;
      break;
    }
  }
  virtual double X() const { return value1; }
  virtual double Y() const { return value2; }
  virtual double R() const { return f_r; }
  virtual double Phi() const { return f_phi; }
  virtual double x() const { return value1; }
  virtual double y() const { return value2; }
  virtual double r() const { return f_r; }
  virtual double phi() const { return f_phi; }
  virtual int isCartesic() const { return (repre == _cartesic_); }
  virtual void toggleRep()
  {
    if (repre == _cartesic_)
      repre = _polar_;
    else
      repre = _cartesic_;
  }
  virtual geomState getState() const { return point::getState(); }
  virtual int getDimension() const { return 2; }
  virtual double getValue(int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    }
    return 0;
  }
  void operator=(const point& p);
  void operator=(const point2D& p);
  vector2D operator-(const point2D& q) const;
  /*       {return vector2D(value1-q.X(), value2-q.Y());} */
  point2D operator-(const vector2D& v) const;
  /*       {return vector2D(value1-v.X(), value2-v.Y());} */
  point2D operator+(const vector2D& v) const;
  /*       {return vector2D(v.X()+value1, v.Y()+value2);} */
  virtual double operator[](int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    }
    return 0;
  }
  int operator==(const point2D& p) const;
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const point2D& p);

/*****Vector class for vectors in 2-dimensional space */

class vector2D : public point2D, public Vector
{
protected:
  virtual void Zero() { point2D::Zero(); }
  virtual void Infinite() { point2D::Infinite(); }
  virtual void Undefined() { point2D::Undefined(); }
  virtual void Regular() { point2D::Regular(); }

public:
  vector2D() : point2D(), Vector(0) {}
  vector2D(const point2D& p) : point2D(p), Vector(0) {}
  vector2D(const point& p) : point2D(p), Vector(0) {}
  vector2D(const vector2D& v);
  vector2D(const Vector& v);
  vector2D(double x, double y) : point2D(x, y), Vector(0) {}
  virtual void setValues(double x, double y) { point2D::setValues(x, y); }
  virtual void setValue(int pos, double valueIn) { point2D::setValue(pos, valueIn); }
  virtual void normalize()
  {
    if (point2D::f_r == 0)
      return;
    point2D::setValues(point2D::value1 / point2D::f_r, point2D::value2 / point2D::f_r);
  }
  virtual double length() const { return point2D::f_r; }
  virtual geomState getState() const { return point2D::getState(); }
  virtual double getValue(int pos) const { return point2D::getValue(pos); }
  virtual int getDimension() const { return 2; }
  void operator=(const point& p);
  void operator=(const Vector& v);
  void operator=(const point2D& p);
  void operator=(const vector2D& v);
  vector2D operator+(const vector2D& w) const
  {
    return vector2D(point2D::value1 + w.X(), point2D::value2 + w.Y());
  }
  vector2D operator-(const vector2D& w) const
  {
    return vector2D(point2D::value1 - w.X(), point2D::value2 - w.Y());
  }
  vector2D operator*(double f) const { return vector2D(f * point2D::value1, f * point2D::value2); }
  double operator*(const vector2D& w) const
  {
    return (point2D::value1 * w.X() + point2D::value2 * w.Y());
  }
  int operator==(const vector2D& v) const
  {
    return ((point2D::value1 == v.X()) && (point2D::value2 == v.Y()));
  }
  virtual double operator[](int pos) const { return point2D::getValue(pos); }
  vector2D operator!() const
  {
    vector2D v(*this);
    v.transpose();
    return v;
  }
  virtual string toString(int pt = -1) const;
};
ostream& operator<<(ostream& o, const vector2D& v);
inline vector2D point2D::operator-(const point2D& q) const
{
  return vector2D(value1 - q.X(), value2 - q.Y());
}
inline point2D point2D::operator-(const vector2D& v) const
{
  return vector2D(value1 - v.X(), value2 - v.Y());
}
inline point2D point2D::operator+(const vector2D& v) const
{
  return vector2D(v.X() + value1, v.Y() + value2);
}

/*****Point class for points in 3-dimensional space */

class point3D : public point
{
private:
  rep3D repre;

protected:
  double f_phi;
  double f_theta;
  double f_r;
  double f_rho;
  double value1, value2, value3;

public:
  point3D() : point(0) { repre = _cartesic3_; }
  point3D(const point& p);
  point3D(const point3D& p);
  point3D(double x, double y, double z) : point(0)
  {
    repre = _cartesic3_;
    setValues(x, y, z);
  }
  virtual void setValues(double x, double y, double z);
  virtual void setThetaPhiR(double th, double ph, double rIN)
  {
    value1 = rIN * sin(th) * cos(ph);
    value2 = rIN * sin(th) * sin(ph);
    value3 = rIN * cos(th);
    f_theta = th;
    f_phi = ph;
    f_r = rIN;
    f_rho = rIN * sin(th);
  }
  virtual double X() const { return value1; }
  virtual double Y() const { return value2; }
  virtual double Z() const { return value3; }
  virtual double R() const { return f_r; }
  virtual double Phi() const { return f_phi; }
  virtual double Rho() const { return f_rho; }
  virtual double Theta() const { return f_theta; }
  virtual double x() const { return value1; }
  virtual double y() const { return value2; }
  virtual double z() const { return value3; }
  virtual double r() const { return f_r; }
  virtual double phi() const { return f_phi; }
  virtual double rho() const { return f_rho; }
  virtual double theta() const { return f_theta; }
  virtual int getDimension() const { return 3; }
  virtual double getValue(int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    case 2:
      return value3;
    }
    return 0;
  }
  virtual void setValue(int pos, double valueIn);
  virtual rep3D getRep() const { return repre; }
  virtual void setRep(rep3D r) { repre = r; }
  void operator=(const point& p);
  void operator=(const point3D& p);
  virtual double operator[](int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    case 2:
      return value3;
    }
    return 0;
  }
  vector3D operator-(
      const point3D& q) const; //{return vector3D(value1-q.X(), value2-q.Y(), value3-q.Z());}
  point3D operator-(
      const vector3D& v) const; //{return point3D(value1-v.X(), value2-v.Y(), value3-v.Z());}
  point3D operator+(
      const vector3D& v) const; //{return point3D(value1+v.X(), value2+v.Y(), value3+v.Z());}
  int operator==(const point3D& p) const;
  virtual string toString(int pt = -1) const;
};

// vector3D operator-(const point3D &p, const point3D &q);
// point3D operator-(const point3D &p, const vector3D &v);
// point3D operator+(const point3D &p, const vector3D &v);
ostream& operator<<(ostream& o, const point3D& p);

/*****Vector class for vectors in 3-dimensional space*/

class vector3D : public point3D, public Vector
{
protected:
  virtual void Zero() { point3D::Zero(); }
  virtual void Infinite() { point3D::Infinite(); }
  virtual void Undefined() { point3D::Undefined(); }
  virtual void Regular() { point3D::Regular(); }

public:
  vector3D() : point3D(), Vector(0) {}
  vector3D(const point& p);
  vector3D(const Vector& v);
  vector3D(const point3D& p) : point3D(p), Vector(0) {}
  vector3D(const vector3D& v);
  vector3D(double x, double y, double z) : point3D(x, y, z), Vector(0) {}
  vector3D(const momentum4D& m);
  virtual void setValues(double x, double y, double z) { point3D::setValues(x, y, z); }
  virtual double getValue(int pos) const { return point3D::getValue(pos); }
  virtual void setValue(int pos, double valueIn) { point3D::setValue(pos, valueIn); }
  virtual geomState getState() const { return point3D::getState(); }
  virtual void normalize()
  {
    if (point3D::f_r == 0)
      return;
    point3D::setValues(point3D::value1 / point3D::f_r, point3D::value2 / point3D::f_r,
                       point3D::value3 / point3D::f_r);
  }
  virtual double length() const { return point3D::f_r; }
  virtual int getDimension() const { return 3; }
  void operator=(const point& p);
  void operator=(const Vector& v);
  void operator=(const point3D& p);
  void operator=(const vector3D& v);
  vector3D operator+(const vector3D& w) const
  {
    return vector3D(point3D::value1 + w.X(), point3D::value2 + w.Y(), point3D::value3 + w.Z());
  }
  vector3D operator-(const vector3D& w) const
  {
    return vector3D(point3D::value1 - w.X(), point3D::value2 - w.Y(), point3D::value3 - w.Z());
  }
  double operator*(const vector3D& w) const
  {
    return (point3D::value1 * w.X() + point3D::value2 * w.Y() + point3D::value3 * w.Z());
  }
  vector3D operator^(const vector3D& w) const
  {
    return vector3D(point3D::value2 * w.Z() - point3D::value3 * w.Y(),
                    point3D::value3 * w.X() - point3D::value1 * w.Z(),
                    point3D::value1 * w.Y() - point3D::value2 * w.X());
  }
  vector3D operator*(double f) const
  {
    return vector3D(f * point3D::value1, f * point3D::value2, f * point3D::value3);
  }
  vector3D operator/(double f) const
  {
    return vector3D(point3D::value1 / f, point3D::value2 / f, point3D::value3 / f);
  }
  int operator==(const vector3D& v) const;
  virtual double operator[](int pos) const { return point3D::getValue(pos); }
  vector3D operator!() const
  {
    vector3D vec(*this);
    vec.transpose();
    return vec;
  }
  double angle(const vector3D& v, const vector3D& w) const;
  virtual string toString(int pt = -1) const;
};
vector3D operator*(double f, const vector3D& v);
// vector3D operator*(const vector3D &v, double f);

ostream& operator<<(ostream& o, const vector3D& v);
inline vector3D point3D::operator-(const point3D& q) const
{
  return vector3D(value1 - q.X(), value2 - q.Y(), value3 - q.Z());
}
inline point3D point3D::operator-(const vector3D& v) const
{
  return point3D(value1 - v.X(), value2 - v.Y(), value3 - v.Z());
}
inline point3D point3D::operator+(const vector3D& v) const
{
  return point3D(value1 + v.X(), value2 + v.Y(), value3 + v.Z());
}

/*****Point class for points in 3+1-dimensional space*/

class point4D : public point
{
private:
  rep3D repre;

protected:
  double f_r;
  double f_rho;
  double f_phi;
  double f_theta;
  double value1, value2, value3, value4;

public:
  point4D() : point(0) { repre = _cartesic3_; }
  point4D(const point3D& p, double w);
  point4D(const point4D& p);
  point4D(const point& p);
  point4D(const point& p, double w);
  point4D(double x, double y, double z, double w) : point(0)
  {
    repre = _cartesic3_;
    setValues(x, y, z, w);
  }
  virtual void setValues(double x, double y, double z, double w);
  virtual void setValues(double x, double y, double z);
  virtual void setValue(int pos, double valueIn)
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
    case 3:
      value4 = valueIn;
      break;
    }
  }
  virtual int getDimension() const { return 4; }
  virtual double getValue(int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    case 2:
      return value3;
    case 3:
      return value4;
    }
    return 0;
  }
  double W() const { return value4; }
  double X() const { return value1; }
  double Y() const { return value2; }
  double Z() const { return value3; }
  double Phi() const { return f_phi; }
  double Theta() const { return f_theta; }
  double Rho() const { return f_rho; }
  double R() const { return f_r; }
  double w() const { return value4; }
  double x() const { return value1; }
  double y() const { return value2; }
  double z() const { return value3; }
  double phi() const { return f_phi; }
  double theta() const { return f_theta; }
  double rho() const { return f_rho; }
  double r() const { return f_r; }
  rep3D getRep() const { return repre; }
  void setRep(rep3D r) { repre = r; }
  void operator=(const point& p);
  void operator=(const point3D& p) { setValues(p[0], p[1], p[2], W()); }
  void operator=(const point4D& p);
  int operator==(const point4D& p)
  {
    return ((value1 - p.X() < __prec) && (value2 - p.Y() < __prec) && (value3 - p.Z() < __prec) &&
            (W() - p.W() < __prec));
  }
  virtual double operator[](int pos) const
  {
    switch (pos) {
    case 0:
      return value1;
    case 1:
      return value2;
    case 2:
      return value3;
    case 3:
      return value4;
    }
    return 0;
  }
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const point4D& p);

/*****Vector class for vectors in 3+1-dimensional space*/

class vector4D : public point4D, public Vector
{
protected:
  virtual void Zero() { point4D::Zero(); }
  virtual void Infinite() { point4D::Infinite(); }
  virtual void Undefined() { point4D::Undefined(); }
  virtual void Regular() { point4D::Regular(); }

public:
  vector4D() : point4D(), Vector(0) {}
  vector4D(const point& p) : point4D(p), Vector(0) {}
  vector4D(const point3D& p) : point4D(p), Vector(0) {}
  vector4D(const point4D& p) : point4D(p), Vector(0) {}
  vector4D(const Vector& v);
  vector4D(const vector3D& v);
  vector4D(const vector4D& v);
  vector4D(double x, double y, double z) : point4D(x, y, z, 0), Vector(0) {}
  vector4D(double x, double y, double z, double w) : point4D(x, y, z, w), Vector(0) {}
  virtual void setValues(double x, double y, double z, double w) { point4D::setValues(x, y, z, w); }
  virtual void setValues(double x, double y, double z) { point4D::setValues(x, y, z); }
  virtual void setValue(int pos, double valueIn) { point4D::setValue(pos, valueIn); }
  virtual double getValue(int pos) const { return point4D::getValue(pos); }
  virtual geomState getState() const { return point4D::getState(); }
  virtual int getDimension() const { return 4; }
  virtual void normalize()
  {
    double t = length();
    if (t == 0)
      return;
    setValues(point4D::value1 / t, point4D::value2 / t, point4D::value3 / t, point4D::value4 / t);
  }
  virtual double length() const
  {
    return sqrt(point4D::f_r * point4D::f_r + point4D::value4 * point4D::value4);
  }
  void operator=(const point& p);
  void operator=(const Vector& v);
  void operator=(const point4D& p);
  void operator=(const vector4D& v);
  void operator=(const point3D& p);
  void operator=(const vector3D& v);
  int operator==(const vector4D& v) const
  {
    return ((point4D::value4 - v.W() < __prec) && (point4D::value1 - v.X() < __prec) &&
            (point4D::value2 - v.Y() < __prec) && (point4D::value3 - v.Z() < __prec));
  }
  vector4D operator!() const
  {
    vector4D v(*this);
    v.transpose();
    return v;
  }
  virtual double operator[](int pos) const { return point4D::operator[](pos); }
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const vector4D& v);

/*****Vector class for momentum in relativistic calculations*/

class momentum4D : public vector4D
{
protected:
  vector3D velocity;
  double mass;
  double gamma;

public:
  momentum4D() : vector4D() { Undefined(); }
  momentum4D(const point4D& p);
  momentum4D(const vector4D& v);
  momentum4D(const momentum4D& m);
  momentum4D(const point3D& p, double energy);
  momentum4D(double x, double y, double z, double e);
  void setPM(const vector3D& p, double m);
  void setVM(const vector3D& v, double m);
  void setVE(const vector3D& v, double e);
  void setPE(const vector3D& p, double e)
  {
    mass = sqrt(abs(e * e - p.R() * p.R()));
    velocity = p * (1 / e);
    gamma = 1 / sqrt(1 - velocity.R() * velocity.R());
    point4D::setValues(p.X(), p.Y(), p.Z(), e);
  }
  void setValues(double x, double y, double z, double e) { setPE(vector3D(x, y, z), e); }
  double Energy() const { return point4D::value4; }
  double Mass() const { return mass; }
  double Beta() const { return velocity.R(); }
  double Gamma() const { return gamma; }
  double Rapidity() const
  {
    return 0.5 * (log((point4D::value4 + point4D::value3) / (point4D::value4 - point4D::value3)));
  }
  double Rapidity(const vector3D& axis) const;
  double px() const { return point4D::value1; }
  double py() const { return point4D::value2; }
  double pz() const { return point4D::value3; }
  double e() const { return point4D::value4; }
  double Px() const { return point4D::value1; }
  double Py() const { return point4D::value2; }
  double Pz() const { return point4D::value3; }
  double E() const { return point4D::value4; }
  double Pt() const { return point4D::value3; }
  double Pt(const vector3D& axis) const
  {
    return (Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }
  vector3D Pt_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R());
  }
  double Pl() const { return f_rho; }
  double Pl(const vector3D& axis) const
  {
    return (axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }
  vector3D Pl_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return axis * (Momentum() * axis) / (axis.R() * axis.R());
  }
  double pt() const { return point4D::value3; }
  double pt(const vector3D& axis) const
  {
    return (Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }
  vector3D pt_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R());
  }
  double pl() const { return f_rho; }
  double pl(const vector3D& axis) const
  {
    return (axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }
  vector3D pl_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return axis * (Momentum() * axis) / (axis.R() * axis.R());
  }
  vector3D Velocity() const { return velocity; }
  vector3D Momentum() const { return vector3D(point4D::value1, point4D::value2, point4D::value3); }
  vector3D Direction() const
  {
    vector3D ret(point4D::value1, point4D::value2, point4D::value3);
    ret.normalize();
    return ret;
  }
  momentum4D boost(const vector3D& beta) const;
  static momentum4D CM(const momentum4D& v1, const momentum4D& v2,
                       const momentum4D& v3 = momentum4D(), const momentum4D& v4 = momentum4D(),
                       const momentum4D& v5 = momentum4D());
  int operator==(const momentum4D& v) const
  {
    return ((Velocity() == v.Velocity()) && (Energy() == v.Energy()));
  }
  void operator=(const momentum4D& v) { setPE(vector3D(v[0], v[1], v[2]), v[3]); }
  momentum4D operator!() const
  {
    momentum4D m(*this);
    m.transpose();
    return m;
  }
  momentum4D operator+(const momentum4D& v) const
  {
    return momentum4D(Momentum() + v.Momentum(), v.Energy() + Energy());
  }
  momentum4D operator-(const momentum4D& v) const
  {
    return momentum4D(Momentum() - v.Momentum(), Energy() - v.Energy());
  }
  double operator*(const momentum4D& v) const
  {
    return (v.Energy() * point4D::value4 - Momentum() * v.Momentum());
  }
  momentum4D operator*(double factor) const
  {
    return momentum4D(Momentum() * factor, point4D::value4 * factor);
  }
  virtual double operator[](int pos) const { return vector4D::operator[](pos); }
  virtual string toString(int pt = -1) const;
};
momentum4D operator*(double factor, const momentum4D& v);
ostream& operator<<(ostream& o, const momentum4D& m);

/*****Standard Variables*/

/* const point2D Origin2D(0, 0); */
/* const vector2D ZeroVector2D(0, 0); */
/* const point3D Origin3D(0, 0, 0); */
/* const vector3D ZeroVector3D(0, 0, 0); */
/* const point4D Origin4D(0, 0, 0, 0); */
/* const vector4D ZeroVector4D(0, 0, 0, 0); */
/* const momentum4D ZeroMomentum(0, 0, 0, 0); */

/************************************************
 *     composed objects                         *
 ************************************************/

/*****Straight Line in 2-Dimensions*/
class lLine2D;
class sLine2D;

class sLine2D : public geomObject
{
private:
  point2D foot;
  vector2D d;

protected:
  void Infinite();
  void Undefined();
  void Regular();

public:
  sLine2D() : geomObject() {}
  sLine2D(const point2D& p, const point2D& q);
  sLine2D(const point2D& p, const vector2D& v);
  sLine2D(const sLine2D& l);
  point2D Foot() const { return foot; }
  vector2D Direction() const { return d; }
  void shiftFoot(double t) { foot = foot + d * t; }
  void newLine(const point2D& p, const vector2D& v);
  int isIn(const point2D& p) const;
  repTD getRep() const { return (d.isCartesic() ? _cartesic_ : _polar_); }
  void setRep(repTD r);
  int isLine() const { return d.isLine(); }
  int isColumn() const { return d.isColumn(); }
  void transpose() { d.transpose(); }
  geomState getState() const { return geomObject::getState(); }
  int getDimension() const { return 2; }
  void Zero()
  {
    geomObject::Zero();
    d.setValues(0, 0);
    foot.Zero();
  }
  vector2D operator-(const point2D& p) const; // distance line - point
  point2D operator&&(const sLine2D& l) const; // common point of 2 lines
  double operator&(const sLine2D& l) const;   // distance of two lines
  lLine2D operator-(const sLine2D& l) const;  // closest approach of 2 lines
  lLine2D operator-(const lLine2D& l) const;  // closest approach of 2 lines
  void operator=(const sLine2D& l);
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const sLine2D& l);

/*****Line between 2 points in 2-Dimensions */

class lLine2D : public geomObject
{
private:
  point2D pPoint;
  point2D qPoint;

protected:
  void Infinite() { geomObject::Infinite(); }
  void Undefined() { geomObject::Undefined(); }
  void Regular() { geomObject::Regular(); }

public:
  lLine2D() : geomObject(), qPoint() { geomObject::Undefined(); }
  lLine2D(const point2D& p, const point2D& q);
  lLine2D(const point2D& p, const vector2D& v);
  lLine2D(const lLine2D& l);
  point2D P() const { return pPoint; }
  point2D Q() const { return qPoint; }
  vector2D Direction() const { return (qPoint - pPoint); }
  point2D Mid() const { return pPoint + ((qPoint - pPoint) * 0.5); }
  int isIn(const point2D& p) const
  {
    vector2D vv = (*this) - p;
    return ((vv.R() < __prec) ? 1 : 0);
  }
  void newLine(const point2D& p, const point2D& q);
  repTD getRep() const { return (pPoint.isCartesic() ? _cartesic_ : _polar_); }
  double length() const { return (pPoint - qPoint).R(); }
  void setRep(repTD r);
  geomState getState() const { return geomObject::getState(); }
  int getDimension() const { return 2; }
  void Zero()
  {
    geomObject::Zero();
    pPoint.Zero();
    qPoint.Zero();
  }
  void operator=(const lLine2D& l);
  vector2D operator-(const point2D& p) const;
  lLine2D operator-(const lLine2D& l) const;
  point2D operator&&(const lLine2D& l) const
  {
    lLine2D tmpc = (*this) - l;
    return tmpc.Mid();
  }
  double operator&(const lLine2D& l) const { return ((*this) - l).length(); }
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const lLine2D& l);

/*****Straight Line in 3-Dimensions*/
class lLine3D;
class sLine3D;

class sLine3D : public geomObject
{
private:
  point3D foot;
  vector3D d;

protected:
  void Infinite() { geomObject::Infinite(); }
  void Undefined()
  {
    geomObject::Undefined();
    foot = point3D();
    d = vector3D();
  }
  void Regular() { geomObject::Regular(); }

public:
  sLine3D() : geomObject() {}
  sLine3D(const point3D& p, const point3D& q);
  sLine3D(const point3D& p, const vector3D& v);
  sLine3D(const sLine3D& l);
  point3D Foot() const { return foot; }
  vector3D Direction() const { return d; }
  void shiftFoot(double t) { foot = foot + d * t; }
  void newLine(const point3D& p, const vector3D& v);
  int isIn(const point3D& p) const;
  rep3D getRep() const { return d.getRep(); }
  void setRep(rep3D r)
  {
    d.setRep(r);
    foot.setRep(r);
  }
  int isLine() const { return d.isLine(); }
  int isColumn() const { return d.isColumn(); }
  void transpose() { d.transpose(); }
  geomState getState() const { return geomObject::getState(); }
  int getDimension() const { return 3; }
  void Zero()
  {
    geomObject::Zero();
    d.setValues(0, 0, 0);
    foot.Zero();
  }
  vector3D operator-(const point3D& p) const // distance line - point
  {
    vector3D v = p - foot;
    vector3D n = v - d * (d * v);
    return n;
  }
  point3D operator&&(const sLine3D& l) const; // common point of 2 lines
  double operator&(const sLine3D& l) const;   // distance of two lines
  lLine3D operator-(const sLine3D& l) const;  // closest approach of 2 lines
  lLine3D operator-(const lLine3D& l) const;  // closest approach of 2 lines
  void operator=(const sLine3D& l);
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const sLine3D& l);

/*****Line between 2 points in 3-Dimensions */

class lLine3D : public geomObject
{
private:
  point3D pPoint;
  point3D qPoint;

protected:
  void Undefined()
  {
    pPoint.Undefined();
    qPoint = point3D();
    geomObject::Undefined();
  }

public:
  lLine3D() : geomObject(), qPoint() { Undefined(); }
  lLine3D(const point3D& p, const point3D& q);
  lLine3D(const point3D& p, const vector3D& v);
  lLine3D(const lLine3D& l);
  point3D P() const { return pPoint; }
  point3D Q() const { return qPoint; }
  vector3D Direction() const { return (qPoint - pPoint); }
  point3D Mid() const { return pPoint + ((qPoint - pPoint) * 0.5); }
  int isIn(const point3D& p) const;
  void newLine(const point3D& p, const point3D& q);
  rep3D getRep() const { return pPoint.getRep(); }
  double length() const { return (qPoint - pPoint).R(); }
  void setRep(rep3D r)
  {
    pPoint.setRep(r);
    qPoint.setRep(r);
  }
  geomState getState() const { return geomObject::getState(); }
  int getDimension() const { return 3; }
  void Zero()
  {
    geomObject::Zero();
    pPoint.Zero();
    qPoint.Zero();
  }
  void operator=(const lLine3D& l);
  vector3D operator-(const point3D& p) const;
  point3D operator&&(const lLine3D& l) const;
  double operator&(const lLine3D& l) const;
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const lLine3D& l);

class plane3D : public geomObject
{
private:
  point3D f;
  vector3D dir1;
  vector3D dir2;
  vector3D n_;

protected:
  void Infinite() { geomObject::Infinite(); }
  void Undefined() { geomObject::Undefined(); }
  void Regular() { geomObject::Regular(); }
  planeRep pr;

public:
  plane3D() : geomObject(), dir1(), dir2() { Undefined(); }
  plane3D(const plane3D& p);
  //  plane3D(triangle3D t);
  plane3D(const point3D& a, const point3D& b, const point3D& c);
  plane3D(const point3D& foot, const vector3D& dir1, const vector3D& dir2);
  plane3D(const point3D& foot, const vector3D& norm);
  point3D Foot() const { return f; }
  vector3D Direction(int i) const
  {
    switch (i) {
    case 0:
      return dir1;
    case 1:
      return dir2;
    }
    return vector3D();
  }
  vector3D Normal() const { return n_; }
  void operator=(const plane3D& p);
  int operator==(const plane3D& p) const;
  geomState getState() const { return geomObject::getState(); }
  void Zero()
  {
    geomObject::Zero();
    dir1.setValues(0, 0, 0);
    dir2.setValues(0, 0, 0);
  }
  rep3D getRep() const { return f.getRep(); }
  void setRep(rep3D r)
  {
    f.setRep(r);
    dir1.setRep(r);
    dir2.setRep(r);
    n_.setRep(r);
  }
  planeRep getPlaneRep() const { return pr; }
  void setPlaneRep(planeRep r) { pr = r; }
  void setPlane(const point3D& foot, const vector3D& dir1, const vector3D& dir2);
  vector3D operator&(const point3D& p) const; // Distance vector point-plane points to point
  point3D operator-(const sLine3D& l) const;  // hit point line-plane
  point3D operator-(const lLine3D& l) const;  // hit point line-plane
  sLine3D operator-(const plane3D& p) const;  // two planes hitting->straight line
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const plane3D& p);

/*
class spiralLine3D: private plane3D
{
 private:
  point3D center;
  double bending;
  double phiOffset;
  double outerR;
  double innerR;
 protected:
  void Infinite();
  void Undefined();
  void Regular();
  void Zero();
 public:
  spiralLine3D();
  spiralLine3D(const spiralLine3D & s);
  spiralLine3D(vector3D normal, point3D centerPoint, double innerRadius,
           double outerRadius, double phi_offset, double dr_dphi);
  ~spiralLine3D();
void operator
};

class spiralLine2D
{
};
*/

class matrix3D : public geomObject
{
private:
  //  vector3D v_0,v_1,v_2;
  double data[9];
  int unit, reel;

public:
  matrix3D();
  matrix3D(const matrix3D& m);
  matrix3D(const vector3D& axis, double angle);
  matrix3D(const vector3D& line1, const vector3D& line2, const vector3D& line3);
  matrix3D(const vector3D& v1, const vector3D& v2);
  virtual ~matrix3D();
  double getValue(int line, int column) const;
  double Determinant() const;
  void setValue(int line, int column, double value);
  void Zero();
  static matrix3D Unity();
  int Unitary() const;
  int Orthogonal() const;
  matrix3D Rotate(const vector3D& fromX, const vector3D& fromY, const vector3D& fromZ,
                  const vector3D& toX, const vector3D& toY, const vector3D& toZ);
  void operator=(const matrix3D& m);
  vector3D operator*(const vector3D& v) const;
  matrix3D operator*(const matrix3D& m) const;
  matrix3D operator*(double v) const;
  matrix3D operator+(const matrix3D& m) const;
  matrix3D operator-(const matrix3D& m) const;
  matrix3D operator!() const;                 // transpose
  matrix3D operator-() const;                 // invert
  virtual vector3D operator[](int num) const; // returns a line of the matrix
  virtual string toString() const;
};

ostream& operator<<(ostream& o, const matrix3D m);
/* #define MATRIX_T */
/* template<class elements> class matrixNxMT:public geomObject */
/* { */
/* private: */
/*     int numberOfColumns; */
/*     int numberOfLines; */
/*     elements *data;  //[numberOfLines*numberOfColumns] */
/* public: */
/*     matrixNxMT(int n=1, int m=1); */
/*     matrixNxMT(const matrixNxMT &mat); */
/*     matrixNxMT(int n, int m, elements *datas); */
/*     ~matrixNxMT(); */
/*     elements getValue(int line, int column) const; */
/*     void setValue(int line, int column, elements value); */
/*     template<class elem>void setValue(int line, int column, elem value); */
/*     int Columns()const; */
/*     int Lines()const; */
/*     void Unity(); */
/*     void transpose(); */
/*     elements Determinant()const; */
/*     bool Diagonal()const; */
/*     bool UpperTriangular()const; */
/*     bool LowerTriangular()const; */
/*     matrixNxMT<elements> subset(int lineBegin, int lineEnd, int columnBegin,
 * int columnEnd)
 * const; */
/*     void operator =(const matrixNxMT<elements> &mat); */
/*     Vector operator[](int num)const;// line of matrix */
/*     Vector operator()(int num)const;//column of matrix */
/*     Vector operator *(const Vector &v)const; */
/*     matrixNxMT<elements> operator *(const matrixNxMT<elements> &m)const; */
/*     matrixNxMT<elements> operator *(elements value)const; */
/*     template<class elem>matrixNxMT<elements> operator *(elem value)const; */
/*     matrixNxMT<elements> operator+(const matrixNxMT<elements> &m)const; */
/*     matrixNxMT<elements> operator-(const matrixNxMT<elements> &m)const; */
/*     matrixNxMT<elements> operator!()const; //transpose */
/*     matrixNxMT<elements> operator-()const; //invert */
/*     virtual string toString()const; */
/*     //    ClassDef(matrixNxMT,3) */
/* }; */

class matrixNxM : public geomObject
{
private:
  int numberOfColumns;
  int numberOfLines;
  double* data; //[numberOfLines*numberOfColumns]
public:
  matrixNxM(int n = 1, int m = 1);
  matrixNxM(const matrixNxM& mat);
#ifdef MATRIX_T
  matrixNxM(const matrixNxMT<double>& mat);
  matrixNxM(const matrixNxMT<float>& mat);
  matrixNxM(const matrixNxMT<long double>& mat);
#endif
  matrixNxM(int n, int m, double* datas);
  ~matrixNxM();
  double getValue(int line, int column) const;
  void setValue(int line, int column, double value);
  int Columns() const;
  int Lines() const;
  void Unity();
  void transpose();
  double Determinant() const;
  bool Diagonal() const;
  bool UpperTriangular() const;
  bool LowerTriangular() const;
  matrixNxM subset(int lineBegin, int lineEnd, int columnBegin, int columnEnd) const;
  void decomposeLU(matrixNxM& L, matrixNxM& U) const;
  void decomposeLDU(matrixNxM& L, matrixNxM& D, matrixNxM& U) const;
  void decomposeQR(matrixNxM& Q, matrixNxM& R) const;
  void operator=(const matrixNxM& mat);
#ifdef MATRIX_T
  void operator=(const matrixNxMT<double>& mat);
  void operator=(const matrixNxMT<long double>& mat);
  void operator=(const matrixNxMT<float>& mat);
#endif
  Vector operator[](int num) const; // line of matrix
  Vector operator()(int num) const; // column of matrix
  Vector operator*(const Vector& v) const;
  matrixNxM operator*(const matrixNxM& m) const;
  matrixNxM operator*(double value) const;
  matrixNxM operator+(const matrixNxM& m) const;
  matrixNxM operator-(const matrixNxM& m) const;
  matrixNxM operator!() const; // transpose
  matrixNxM operator-() const; // invert
  virtual string toString() const;
};
ostream& operator<<(ostream& o, const matrixNxM& m);
Vector operator*(const Vector& v, const matrixNxM& m);

class projector : public geomObject
{
  static plane3D fplane;
  static point3D feye;
  static vector3D fopticalAxis;
  static int fmode;

public:
  enum
  {
    parallel = 0,
    central = 1,
    fishEye = 2,
    predefined = 3
  };
  projector();
  ~projector();
  static void setProjector(const plane3D& plane, const point3D& point, int mode = central);
  static point2D project(const point3D& point, int mode = predefined);
  static point2D project(const plane3D& plane, const point3D& eye, const point3D& point,
                         int mode = predefined);
  static plane3D plane() { return fplane; };
  static point3D eye() { return feye; };
};
#endif
