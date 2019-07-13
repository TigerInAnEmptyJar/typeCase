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
/*!
 * \brief The geomObject class
 * This is the base class for gemetrical objects. It contains a status variable and a toString
 * method that can be overwritten by derived classes.
 */
class geomObject
{
private:
  geomState status;

public:
  /*!
   * \brief geomObject
   * Default constructor.
   */
  geomObject() { status = _undefined_; }

  /*!
   * \brief ~geomObject
   * Destructor
   */
  virtual ~geomObject() {}

  /*!
   * \brief getState
   * Returns the state of the geometrical object.
   * \return
   */
  virtual geomState getState() const { return status; }

  /*!
   * \brief Zero
   * Sets the state of the geometrical object to _zero_.
   */
  virtual void Zero() { status = _zero_; }

  /*!
   * \brief Infinite
   * Sets the state of the geometrical object to _infinite_.
   */
  virtual void Infinite() { status = _infinite_; }

  /*!
   * \brief Undefined
   * Sets the state of the geometrical object to _undefined_.
   */
  virtual void Undefined() { status = _undefined_; }

  /*!
   * \brief Regular
   * Sets the state of the geometrical object to _regular_.
   */
  virtual void Regular() { status = _regular_; }

  /*!
   * \brief toString
   * Returns a string representation of the geometrical object.
   * \return
   */
  virtual string toString(int pt = -1) const;
};

/*****Base point class - for points in n dimensional space*/
/*!
 * \brief The point class
 * This class represents a point in n-dimensional space.
 */
class point : public geomObject
{
protected:
  /*!
   * \brief dimension
   * dimension of the space the point is located in
   */
  int dimension;

  /*!
   * \brief infinityPos
   * position where there is a first inf-value. obsolete.
   */
  int infinityPos;

  /*!
   * \brief v
   * values for dimension 0-20
   */
  double v[20]; //[20]

  /*!
   * \brief value
   * values for dimension 20+
   */
  double* value; //[dimension]
public:
  /*!
   * \brief point
   * Constructor set the dimension of the space the point is located in (default =0).
   * \param dim
   */
  point(int dim = 0);

  /*!
   * \brief point
   * Copy constructor.
   * \param p
   */
  point(const point& p);

  /*!
   * \brief point
   * Copy constructor from 2D.
   * \param p
   */
  point(const point2D& p);

  /*!
   * \brief point
   * Copy constructor from 3D.
   * \param p
   */
  point(const point3D& p);

  /*!
   * \brief point
   * Copy constructor from 4D.
   * \param p
   */
  point(const point4D& p);

  /*!
   * \brief ~point
   */
  virtual ~point()
  {
    if (dimension >= 20)
      delete[] value;
  }

  /*!
   * \brief reDimension
   * Erase all information about position of the point and re-allocate it in newDim-dimensional
   * space.
   * \param newDim
   */
  virtual void reDimension(int newDim);

  /*!
   * \brief setValues
   * Sets all elements of the point in cartesic coordinates. Take care that values is correctly
   * dimensioned.
   * \param values
   */
  void setValues(double values[]);

  /*!
   * \brief setValues
   * Sets up to dimension elements in cartesic coordinates. If the dimension is larger than 20 it
   * sets only the 20 first elements.
   * \param f1
   * \param f2
   * \param f3
   * \param f4
   * \param f5
   * \param f6
   * \param f7
   * \param f8
   * \param f9
   * \param f10
   * \param f11
   * \param f12
   * \param f13
   * \param f14
   * \param f15
   * \param f16
   * \param f17
   * \param f18
   * \param f19
   * \param f20
   */
  void setValues(double f1 = 0, double f2 = 0, double f3 = 0, double f4 = 0, double f5 = 0,
                 double f6 = 0, double f7 = 0, double f8 = 0, double f9 = 0, double f10 = 0,
                 double f11 = 0, double f12 = 0, double f13 = 0, double f14 = 0, double f15 = 0,
                 double f16 = 0, double f17 = 0, double f18 = 0, double f19 = 0, double f20 = 0);

  /*!
   * \brief setValue
   * Setter. Sets the posth element of the point in cartesic coordinates to value.
   * \param pos
   * \param valueIn
   */
  virtual void setValue(int pos, double valueIn);

  /*!
   * \brief Zero
   * Sets the state of the objet to _zero_.
   */
  virtual void Zero();

  /*!
   * \brief getValue
   * Getter. Returns the posth element of the point in cartesic coordinates.
   * \param pos
   * \return
   */
  virtual double getValue(int pos) const
  {
    if (pos >= dimension || pos < 0)
      return 0;
    if (pos < 20)
      return v[pos];
    else
      return value[pos - 20];
  }

  /*!
   * \brief getDimension
   * Returns the dimension of the space the point is located in.
   * \return
   */
  virtual int getDimension() const { return dimension; }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  virtual geomState getState() const { return geomObject::getState(); }

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimension of this point is equal to the dimension of p.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator -
   * Subtraction operator. Return the distance vector between point q and this point.
   * \param q
   * \return
   */
  Vector operator-(const point& q);

  /*!
   * \brief operator -
   * Subtraction operator. Add the vector v multiplied by -1 to the point and return the result.
   * \param v
   * \return
   */
  point operator-(const Vector& v);

  /*!
   * \brief operator +
   * Addition operator. Add a vector to a point and return the result.
   * \param v
   * \return
   */
  point operator+(const Vector& v);

  /*!
   * \brief operator ==
   * Comparison operator. Returns 1 if the dimension as well as the individual elements of the two
   * points are equal.
   * \param p
   * \return
   */
  int operator==(const point& p);

  /*!
   * \brief operator []
   * Access-operator. Returns the posth element of the point in cartesic coordinates.
   * \param pos
   * \return
   */
  virtual double operator[](int pos) const
  {
    if (pos >= dimension || pos < 0)
      return 0;
    if (pos < 20)
      return v[pos];
    else
      return value[pos - 20];
  }

  /*!
   * \brief toString
   * Returns a cartesic string representation of the point.
   * \return
   */
  virtual string toString(int pt = -1) const;
};

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
ostream& operator<<(ostream& o, const point& p);

/*****Base vector class - for vectors in n dimensional space*/

/*!
 * \brief The Vector class
 */
class Vector : public point
{
private:
  int kind;

protected:
  /*!
   * \brief Undefined
   * Sets the state of the object to _undefined_.
   */
  virtual void Undefined() { point::Undefined(); }

public:
  /*!
   * \brief Vector
   * Constructor. Sets the dimension of the vector (default =0).
   * \param dim
   */
  Vector(int dim = 0) : point(dim) { kind = 1; }

  /*!
   * \brief Vector
   * Copy constructor.
   * \param v
   */
  Vector(const Vector& v);

  /*!
   * \brief Vector
   * Copy constructor from point.
   * \param p
   */
  Vector(const point& p) : point(p) { kind = 1; }
  /*     Vector (const vector3D v); */
  /*     Vector (const vector2D v); */
  /*     Vector (const vector4D v); */
  /*     Vector (const momentum4D v); */

  /*!
   * \brief normalize
   * Normalizes the vector. All components in cartesian representation are devided by the length of
   * the vector.
   */
  virtual void normalize();

  /*!
   * \brief transpose
   * Transposes the vector. Converts a line vector into a column vector and vice versa.
   */
  void transpose() { kind = !kind; }

  /*!
   * \brief isLine
   * Returns true if this vector is a line vector.
   * \return
   */
  int isLine() const { return kind; }

  /*!
   * \brief isColumn
   * Returns true if this vector is a column vector.
   * \return
   */
  int isColumn() const { return !kind; }

  /*!
   * \brief length
   * Returns the length of the vector.
   * \return
   */
  virtual double length() const;

  /*!
   * \brief operator =
   * Copy operator. Used is the point vector of the origin to point p. Works only if the dimensions
   * of this vector and p are the same.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimensions of this vector and v are the same.
   * \param v
   */
  void operator=(const Vector& v);

  /*!
   * \brief operator +
   * Addition operator. Returns the sum of this vector and vector w. Works only if the dimensions of
   * this vector and w are the same.
   * \param w
   * \return
   */
  Vector operator+(const Vector& w) const;

  /*!
   * \brief operator -
   * Subtraction operator. Subtracts w from this vector and returns the result. This vector remains
   * unchanged. Works only if the dimensions of this vector and w are the same.
   * \param w
   * \return
   */
  Vector operator-(const Vector& w) const;

  /*!
   * \brief operator *
   * Multiplication operator. Multiplies this vector with f and returns the result. This vector
   * remains unchanged.
   * \param f
   * \return
   */
  Vector operator*(double f) const;

  /*!
   * \brief operator %
   * A multiplication operator. The cartesic component of the return vector at position i is the
   * product of the component of this vector at position i and the one of vector w at position i.
   * \param w
   * \return
   */
  Vector operator%(const Vector& w) const;

  /*!
   * \brief operator *
   * Multiplication operator. Returns the scalar product of vector w wit this vector. Works only if
   * the dimensions of this vector and w are the same.
   * \param w
   * \return
   */
  double operator*(const Vector& w) const;
  //    Vector operator ^ (const Vector &w);

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the dimension of this vector is the same as of vector v a
   * if the components are the same.
   * \param v
   * \return
   */
  int operator==(const Vector& v) const;

  /*!
   * \brief operator !
   * Transposition operator. Converts a line vector into a column vector and vice versa and returns
   * the result. This vector remains unchanged.
   * \return
   */
  Vector operator!()
  {
    Vector v(*this);
    v.transpose();
    return v;
  }

  /*!
   * \brief toString
   * Returns a string representation of the vector.
   * \return
   */
  virtual string toString(int pt = -1) const;
};

/*!
 * \brief operator *
 * \param f
 * \param v
 * \return
 */
Vector operator*(double f, const Vector& v);

/*!
 * \brief operator /
 * \param v
 * \param f
 * \return
 */
Vector operator/(const Vector& v, double f);

/*!
 * \brief operator <<
 * \param o
 * \param v
 * \return
 */
ostream& operator<<(ostream& o, const Vector& v);

/*****Point class for points in 2-dimensional Space*/
/*!
 * \brief The point2D class
 * Point in 2 dimensional space.
 */
class point2D : public point
{
private:
  repTD repre;

protected:
  /*!
   * \brief f_phi
   * phi-coordinate
   */
  double f_phi;

  /*!
   * \brief f_r
   * r-coordinate: distance to origin.
   */
  double f_r;

  /*!
   * \brief value1
   * x-coordinate. y-coordinate.
   */
  double value1, value2;

public:
  /*!
   * \brief point2D
   * Default constructor.
   */
  point2D() {}

  /*!
   * \brief point2D
   * Copy constructor. Works only if the dimension of point p is 2.
   * \param p
   */
  point2D(const point& p);

  /*!
   * \brief point2D
   * Copy constructor.
   * \param p
   */
  point2D(const point2D& p);

  /*!
   * \brief point2D
   * Constructor. Defines a 2D-point with the cartesian coordinates x and y.
   * \param x
   * \param y
   */
  point2D(double x, double y);

  /*!
   * \brief setValues
   * Sets the components in cartesic coordinates.
   * \param x
   * \param y
   */
  virtual void setValues(double x, double y);

  /*!
   * \brief setValue
   * Setter sets the posth component in cartesic coordinates to valueIn
   * \param pos
   * \param valueIn
   */
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

  /*!
   * \brief X
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  virtual double X() const { return value1; }

  /*!
   * \brief Y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  virtual double Y() const { return value2; }

  /*!
   * \brief R
   * Returns the points distance to the origin.
   * \return
   */
  virtual double R() const { return f_r; }

  /*!
   * \brief Phi
   * Returns the phi-component in polar coordinates.
   * \return
   */
  virtual double Phi() const { return f_phi; }

  /*!
   * \brief x
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  virtual double x() const { return value1; }

  /*!
   * \brief y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  virtual double y() const { return value2; }

  /*!
   * \brief r
   * Returns the points distance to the origin.
   * \return
   */
  virtual double r() const { return f_r; }

  /*!
   * \brief phi
   * Returns the phi-component in polar coordinates.
   * \return
   */
  virtual double phi() const { return f_phi; }

  /*!
   * \brief isCartesic
   * Returns true if the points representation is _cartesic_.
   * \return
   */
  virtual int isCartesic() const { return (repre == _cartesic_); }

  /*!
   * \brief toggleRep
   * Switches the points representation fron _cartesic_ to _polar_, or vice versa.
   */
  virtual void toggleRep()
  {
    if (repre == _cartesic_)
      repre = _polar_;
    else
      repre = _cartesic_;
  }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  virtual geomState getState() const { return point::getState(); }

  /*!
   * \brief getDimension
   * Returns the dimension of the point which is 2.
   * \return
   */
  virtual int getDimension() const { return 2; }

  /*!
   * \brief getValue
   * Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
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

  /*!
   * \brief operator =
   * Copy operator. Works only if p is two-dimensional.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator
   * \param p
   */
  void operator=(const point2D& p);

  /*!
   * \brief operator -
   * Subtraction operator. Returns the vector to come from point q to this point.
   * \param q
   * \return
   */
  vector2D operator-(const point2D& q) const;
  /*       {return vector2D(value1-q.X(), value2-q.Y());} */

  /*!
   * \brief operator -
   * Subtraction operator. Returns a point that is shifted by minus v from this point.
   * \param v
   * \return
   */
  point2D operator-(const vector2D& v) const;
  /*       {return vector2D(value1-v.X(), value2-v.Y());} */

  /*!
   * \brief operator +
   * Addition operator. Returns a point that is shifted by vector v from this point.
   * \param v
   * \return
   */
  point2D operator+(const vector2D& v) const;
  /*       {return vector2D(v.X()+value1, v.Y()+value2);} */

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
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

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if this point and p refer to the same point.
   * \param p
   * \return
   */
  int operator==(const point2D& p) const;

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt the representation is eigther in
   * cartesic coordinates (_cartesic_) or in polar coordinates (_polar_). if pt is -1, the points
   * own representation is used.
   * \param pt
   * \return
   */
  virtual string toString(int pt = -1) const;
};

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
ostream& operator<<(ostream& o, const point2D& p);

/*****Vector class for vectors in 2-dimensional space */
/*!
 * \brief The vector2D class
 * Vector in 2-dimensional space.
 */
class vector2D : public point2D, public Vector
{
protected:
  /*!
   * \brief Zero
   * Sets the state of the objet to _zero_.
   */
  void Zero() override { point2D::Zero(); }

  /*!
   * \brief Infinite
   * Sets the state of the objet to _infinite_.
   */
  void Infinite() override { point2D::Infinite(); }

  /*!
   * \brief Undefined
   * Sets the state of the objet to _undefined_.
   */
  void Undefined() override { point2D::Undefined(); }

  /*!
   * \brief Regular
   * Sets the state of the objet to _regular_.
   */
  void Regular() override { point2D::Regular(); }

public:
  /*!
   * \brief vector2D
   * Default constructor
   */
  vector2D() : point2D(), Vector(0) {}

  /*!
   * \brief vector2D
   * Copy constructor. Sets a point vector from the origin to point p.
   * \param p
   */
  vector2D(const point2D& p) : point2D(p), Vector(0) {}

  /*!
   * \brief vector2D
   *  Copy constructor. Sets a point vector from the origin to point p. Works only if p is
   * 2-dimensional.
   * \param p
   */
  vector2D(const point& p) : point2D(p), Vector(0) {}

  /*!
   * \brief vector2D
   * Copy constructor.
   * \param v
   */
  vector2D(const vector2D& v);

  /*!
   * \brief vector2D
   * Copy constructor. Works only if v is 2-dimensional.
   * \param v
   */
  vector2D(const Vector& v);

  /*!
   * \brief vector2D
   * Constructor. Sets the cartesic coordinates.
   * \param x
   * \param y
   */
  vector2D(double x, double y) : point2D(x, y), Vector(0) {}

  /*!
   * \brief setValues
   * Sets the cartesic coordinates.
   * \param x
   * \param y
   */
  void setValues(double x, double y) override { point2D::setValues(x, y); }

  /*!
   * \brief setValue
   * Setter sets the posth component in cartesic coordinates to valueIn.
   * \param pos
   * \param valueIn
   */
  void setValue(int pos, double valueIn) override { point2D::setValue(pos, valueIn); }

  /*!
   * \brief normalize
   * Normalizes the vector. Each component in cartesic coordinates is devided by the vectors length.
   */
  void normalize() override
  {
    if (point2D::f_r == 0)
      return;
    point2D::setValues(point2D::value1 / point2D::f_r, point2D::value2 / point2D::f_r);
  }

  /*!
   * \brief length
   * Returns the length of the vector.
   * \return
   */
  virtual double length() const override { return point2D::f_r; }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return point2D::getState(); }

  /*!
   * \brief getValue
   * Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double getValue(int pos) const override { return point2D::getValue(pos); }

  /*!
   * \brief getDimension
   * Returns the dimension of the vector which is 2.
   * \return
   */
  int getDimension() const override { return 2; }

  /*!
   * \brief operator =
   * Copy operator. Sets the vector to a point-vector from the origin to point p. Works only if p is
   * 2-dimensional.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator. Works only if v is 2-dimensional.
   * \param v
   */
  void operator=(const Vector& v);

  /*!
   * \brief operator =
   * Copy operator. Sets the vector to a point-vector from the origin to point p.
   * \param p
   */
  void operator=(const point2D& p);

  /*!
   * \brief operator =
   * Copy operator.
   * \param v
   */
  void operator=(const vector2D& v);

  /*!
   * \brief operator +
   * Addition operator. Return the sum of this vector and vector w.
   * \param w
   * \return
   */
  vector2D operator+(const vector2D& w) const
  {
    return vector2D(point2D::value1 + w.X(), point2D::value2 + w.Y());
  }

  /*!
   * \brief operator -
   * Subtraction operator. Returns the difference vector of this vector minus w.
   * \param w
   * \return
   */
  vector2D operator-(const vector2D& w) const
  {
    return vector2D(point2D::value1 - w.X(), point2D::value2 - w.Y());
  }

  /*!
   * \brief operator *
   * Multiplication operator. Returns a vector with f times the length of this vector.
   * \param f
   * \return
   */
  vector2D operator*(double f) const { return vector2D(f * point2D::value1, f * point2D::value2); }

  /*!
   * \brief operator *
   * Multiplication operator. Returns the scalar product of this vector with w.
   * \param w
   * \return
   */
  double operator*(const vector2D& w) const
  {
    return (point2D::value1 * w.X() + point2D::value2 * w.Y());
  }

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if this vector and v are the same.
   * \param v
   * \return
   */
  int operator==(const vector2D& v) const
  {
    return ((point2D::value1 == v.X()) && (point2D::value2 == v.Y()));
  }

  /*!
   * \brief operator []
   * Access operator- Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override { return point2D::getValue(pos); }

  /*!
   * \brief operator !
   * Transposes the vector and returns the result. A line vector returns a column vector, a column
   * vector a line vector. This vector remains unchanged.
   * \return
   */
  vector2D operator!() const
  {
    vector2D v(*this);
    v.transpose();
    return v;
  }

  /*!
   * \brief toString
   * Returns a string representation of this vector. This is depending on pt eigther _cartesic_,
   * _polar_ or the vectors own representation, when pt is -1 (default).
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 *
 * \param o
 * \param v
 * \return
 */
ostream& operator<<(ostream& o, const vector2D& v);

/*!
 * \brief point2D::operator -
 *
 * \param q
 * \return
 */
inline vector2D point2D::operator-(const point2D& q) const
{
  return vector2D(value1 - q.X(), value2 - q.Y());
}

/*!
 * \brief point2D::operator -
 * \param v
 * \return
 */
inline point2D point2D::operator-(const vector2D& v) const
{
  return vector2D(value1 - v.X(), value2 - v.Y());
}

/*!
 * \brief point2D::operator +
 * \param v
 * \return
 */
inline point2D point2D::operator+(const vector2D& v) const
{
  return vector2D(v.X() + value1, v.Y() + value2);
}

/*****Point class for points in 3-dimensional space */
/*!
 * \brief The point3D class
 * Point in 3 dimensional space.
 */
class point3D : public point
{
private:
  rep3D repre;

protected:
  /*!
   * \brief f_phi
   * phi-component in eigther polar or spheric coordinates, angle of the point vector projected to
   * x-y-plane to the x-axis.
   */
  double f_phi;

  /*!
   * \brief f_theta
   * theta-component in spheric coordinates, angle of the point vector to the z-axis.
   */
  double f_theta;

  /*!
   * \brief f_r
   * r-component in spheric coordinates, distance to the origin.
   */
  double f_r;

  /*!
   * \brief f_rho
   * rho-component in cylindric coordinates, length of the point vector projected on the x-y-plane.
   */
  double f_rho;

  /*!
   * \brief value1
   * x,y,z-components in cartesic coordinates.
   */
  double value1, value2, value3;

public:
  /*!
   * \brief point3D
   * Default constructor.
   */
  point3D() : point(0) { repre = _cartesic3_; }

  /*!
   * \brief point3D
   * Copy constructor. Works only if the dimension of p is 3.
   * \param p
   */
  point3D(const point& p);

  /*!
   * \brief point3D
   * Copy constructor.
   * \param p
   */
  point3D(const point3D& p);

  /*!
   * \brief point3D
   * Constructor. Sets the cartesic coordinates.
   * \param x
   * \param y
   * \param z
   */
  point3D(double x, double y, double z) : point(0)
  {
    repre = _cartesic3_;
    setValues(x, y, z);
  }

  /*!
   * \brief setValues
   * Set the cartesic coordinates.
   * \param x
   * \param y
   * \param z
   */
  virtual void setValues(double x, double y, double z);

  /*!
   * \brief setThetaPhiR
   * \param th
   * \param ph
   * \param rIN
   */
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

  /*!
   * \brief X
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  virtual double X() const { return value1; }

  /*!
   * \brief Y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  virtual double Y() const { return value2; }

  /*!
   * \brief Z
   * Returns the z-component in cartesic coordinates.
   * \return
   */
  virtual double Z() const { return value3; }

  /*!
   * \brief R
   * Returns the r-component in spheric coordinates, distance to the origin.
   * \return
   */
  virtual double R() const { return f_r; }

  /*!
   * \brief Phi
   * Returns the phi-component in eigther polar or spheric coordinates, angle of the point vector
   * projected to x-y-plane to the x-axis.
   * \return
   */
  virtual double Phi() const { return f_phi; }

  /*!
   * \brief Rho
   * Returns the rho-component in cylindric coordinates, length of the point vector projected on the
   * x-y-plane.
   * \return
   */
  virtual double Rho() const { return f_rho; }

  /*!
   * \brief Theta
   * Returns the theta-component in spheric coordinates, angle of the point vector to the z-axis.
   * \return
   */
  virtual double Theta() const { return f_theta; }

  /*!
   * \brief x
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  virtual double x() const { return value1; }

  /*!
   * \brief y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  virtual double y() const { return value2; }

  /*!
   * \brief z
   * Returns the z-component in cartesic coordinates.
   * \return
   */
  virtual double z() const { return value3; }

  /*!
   * \brief r
   * Returns the r-component in spheric coordinates, distance to the origin.
   * \return
   */
  virtual double r() const { return f_r; }

  /*!
   * \brief phi
   * Returns the phi-component in eigther polar or spheric coordinates, angle of the point vector
   * projected to x-y-plane to the x-axis.
   * \return
   */
  virtual double phi() const { return f_phi; }

  /*!
   * \brief rho
   * Returns the rho-component in cylindric coordinates, length of the point vector projected on the
   * x-y-plane.
   * \return
   */
  virtual double rho() const { return f_rho; }

  /*!
   * \brief theta
   * Returns the theta-component in spheric coordinates, angle of the point vector to the z-axis.
   * \return
   */
  virtual double theta() const { return f_theta; }

  /*!
   * \brief getDimension
   * Returns the point's dimension which is 3.
   * \return
   */
  int getDimension() const override { return 3; }

  /*!
   * \brief getValue
   * Return the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double getValue(int pos) const override
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

  /*!
   * \brief setValue
   * Set the posth component in cartesic coordinates to valueIn.
   * \param pos
   * \param valueIn
   */
  void setValue(int pos, double valueIn) override;

  /*!
   * \brief getRep
   * Returns the points representation.
   * \return
   */
  virtual rep3D getRep() const { return repre; }

  /*!
   * \brief setRep
   * Set the points representation to r. This can be eigther _cartesic3_, _cylindric_ or _spheric_
   * \param r
   */
  virtual void setRep(rep3D r) { repre = r; }

  /*!
   * \brief operator =
   * Copy operator. Works only if p is 2-dimensional.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator.
   * \param p
   */
  void operator=(const point3D& p);

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override
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

  /*!
   * \brief operator -
   * Subtraction operator. Returns the vector needed to come from q to this point.
   * \param q
   * \return
   */
  vector3D operator-(
      const point3D& q) const; //{return vector3D(value1-q.X(), value2-q.Y(), value3-q.Z());}

  /*!
   * \brief operator -
   * Subtraction operator. Returns the point which is shifted by minus v from this point.
   * \param v
   * \return
   */
  point3D operator-(
      const vector3D& v) const; //{return point3D(value1-v.X(), value2-v.Y(), value3-v.Z());}

  /*!
   * \brief operator +
   * Addition operator. Returns the point which is shifted by v from this point.
   * \param v
   * \return
   */
  point3D operator+(
      const vector3D& v) const; //{return point3D(value1+v.X(), value2+v.Y(), value3+v.Z());}

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the distance between this point and p is smaller __prec
   * (1e-6).
   * \param p
   * \return
   */
  int operator==(const point3D& p) const;

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the points own representation (pt=-1, default).
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

// vector3D operator-(const point3D &p, const point3D &q);
// point3D operator-(const point3D &p, const vector3D &v);
// point3D operator+(const point3D &p, const vector3D &v);

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
ostream& operator<<(ostream& o, const point3D& p);

/*****Vector class for vectors in 3-dimensional space*/
/*!
 * \brief The vector3D class
 * Vector in 3-dimensional space.
 */
class vector3D : public point3D, public Vector
{
protected:
  /*!
   * \brief Zero
   * Sets the state of the objet to _zero_.
   */
  void Zero() override { point3D::Zero(); }

  /*!
   * \brief Infinite
   * Sets the state of the objet to _infinite_.
   */
  void Infinite() override { point3D::Infinite(); }

  /*!
   * \brief Undefined
   * Sets the state of the objet to _undefined_.
   */
  void Undefined() override { point3D::Undefined(); }

  /*!
   * \brief Regular
   * Sets the state of the objet to _regular_.
   */
  void Regular() override { point3D::Regular(); }

public:
  /*!
   * \brief vector3D
   * Default constructor.
   */
  vector3D() : point3D(), Vector(0) {}

  /*!
   * \brief vector3D
   * Copy constructor. Creates a point vector from the origin to point p. Works only if the
   * dimension of p is 3.
   * \param p
   */
  vector3D(const point& p);

  /*!
   * \brief vector3D
   * Copy constructor. Works only if the dimension of v is 3.
   * \param v
   */
  vector3D(const Vector& v);

  /*!
   * \brief vector3D
   * Copy constructor. Sets a point vector from the origin to point p.
   * \param p
   */
  vector3D(const point3D& p) : point3D(p), Vector(0) {}

  /*!
   * \brief vector3D
   * Copy constructor.
   * \param v
   */
  vector3D(const vector3D& v);

  /*!
   * \brief vector3D
   * Constructor. Sets the cartesic coordinates.
   * \param x
   * \param y
   * \param z
   */
  vector3D(double x, double y, double z) : point3D(x, y, z), Vector(0) {}

  /*!
   * \brief vector3D
   * Copy constructor. Takes the 3-momentum of the 4D-momentum vector (without the energy
   * component).
   * \param m
   */
  vector3D(const momentum4D& m);

  /*!
   * \brief setValues
   * Sets the cartesic coordinates.
   * \param x
   * \param y
   * \param z
   */
  void setValues(double x, double y, double z) override { point3D::setValues(x, y, z); }

  /*!
   * \brief getValue
   * Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double getValue(int pos) const override { return point3D::getValue(pos); }

  /*!
   * \brief setValue
   * Set the posth component in cartesic coordinates to valueIn.
   * \param pos
   * \param valueIn
   */
  void setValue(int pos, double valueIn) override { point3D::setValue(pos, valueIn); }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return point3D::getState(); }

  /*!
   * \brief normalize
   * Set the length of the vector to 1.
   */
  void normalize() override
  {
    if (point3D::f_r == 0)
      return;
    point3D::setValues(point3D::value1 / point3D::f_r, point3D::value2 / point3D::f_r,
                       point3D::value3 / point3D::f_r);
  }

  /*!
   * \brief length
   * Return the length of the vector.
   * \return
   */
  double length() const override { return point3D::f_r; }

  /*!
   * \brief getDimension
   * Return the vectors dimension which is 3.
   * \return
   */
  int getDimension() const override { return 3; }

  /*!
   * \brief operator =
   * Copy operator. Sets a point vector from the origin to point p. Works only if the dimension of p
   * is 3.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimension of v is 3.
   * \param v
   */
  void operator=(const Vector& v);

  /*!
   * \brief operator =
   * Copy operator. Sets a point vector from the origin to point p.
   * \param p
   */
  void operator=(const point3D& p);

  /*!
   * \brief operator =
   * Copy operator.
   * \param v
   */
  void operator=(const vector3D& v);

  /*!
   * \brief operator +
   * Addition operation. Returns the sum of this vector and w.
   * \param w
   * \return
   */
  vector3D operator+(const vector3D& w) const
  {
    return vector3D(point3D::value1 + w.X(), point3D::value2 + w.Y(), point3D::value3 + w.Z());
  }

  /*!
   * \brief operator -
   * Subtraction operator. Returns the difference vector between this vector and w.
   * \param w
   * \return
   */
  vector3D operator-(const vector3D& w) const
  {
    return vector3D(point3D::value1 - w.X(), point3D::value2 - w.Y(), point3D::value3 - w.Z());
  }

  /*!
   * \brief operator *
   * Multiplication operator. Returns the scalar product of this vector and w.
   * \param w
   * \return
   */
  double operator*(const vector3D& w) const
  {
    return (point3D::value1 * w.X() + point3D::value2 * w.Y() + point3D::value3 * w.Z());
  }

  /*!
   * \brief operator ^
   * Returns the cross-product of (this x w).
   * \param w
   * \return
   */
  vector3D operator^(const vector3D& w) const
  {
    return vector3D(point3D::value2 * w.Z() - point3D::value3 * w.Y(),
                    point3D::value3 * w.X() - point3D::value1 * w.Z(),
                    point3D::value1 * w.Y() - point3D::value2 * w.X());
  }

  /*!
   * \brief operator *
   * Multiplication operator. Returns a vector with f times the length of this vector.
   * \param f
   * \return
   */
  vector3D operator*(double f) const
  {
    return vector3D(f * point3D::value1, f * point3D::value2, f * point3D::value3);
  }

  /*!
   * \brief operator /
   * Division operator. Divides each component in cartesic coordinates by f and returns the result.
   * This vector remains unchanged.
   * \param f
   * \return
   */
  vector3D operator/(double f) const
  {
    return vector3D(point3D::value1 / f, point3D::value2 / f, point3D::value3 / f);
  }

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the difference between the vectors is less than __prec
   * (1e-6).
   * \param v
   * \return
   */
  int operator==(const vector3D& v) const;

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override { return point3D::getValue(pos); }

  /*!
   * \brief operator !
   * Transposes the vector and returns the result. A line vector returns a column vector, a column
   * vector a line vector. This vector remains unchanged.
   * \return
   */
  vector3D operator!() const
  {
    vector3D vec(*this);
    vec.transpose();
    return vec;
  }

  /*!
   * \brief angle
   * Returns a signed angle between the vectors v and w. This vector acts as sign defining axis. If
   * the cross-product (v x w)*this is negative, so is the sign of the result.
   * \param v
   * \param w
   * \return
   */
  double angle(const vector3D& v, const vector3D& w) const;

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the points own representation (pt=-1, default).
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator *
 * \param f
 * \param v
 * \return
 */
vector3D operator*(double f, const vector3D& v);
// vector3D operator*(const vector3D &v, double f);

/*!
 * \brief operator <<
 * \param o
 * \param v
 * \return
 */
ostream& operator<<(ostream& o, const vector3D& v);

/*!
 * \brief point3D::operator -
 * \param q
 * \return
 */
inline vector3D point3D::operator-(const point3D& q) const
{
  return vector3D(value1 - q.X(), value2 - q.Y(), value3 - q.Z());
}

/*!
 * \brief point3D::operator -
 * \param v
 * \return
 */
inline point3D point3D::operator-(const vector3D& v) const
{
  return point3D(value1 - v.X(), value2 - v.Y(), value3 - v.Z());
}

/*!
 * \brief point3D::operator +
 * \param v
 * \return
 */
inline point3D point3D::operator+(const vector3D& v) const
{
  return point3D(value1 + v.X(), value2 + v.Y(), value3 + v.Z());
}

/*****Point class for points in 3+1-dimensional space*/
/*!
 * \brief The point4D class
 * Point in 3+1-dimensional space.
 */
class point4D : public point
{
private:
  rep3D repre;

protected:
  /*!
   * \brief f_r
   * r-component of the 3D-part in spheric coordinates, distance to the origin
   */
  double f_r;

  /*!
   * \brief f_rho
   * rho-component of the 3D-part in cylindric coordinates, length of the point vector projected on
   * the x-y-plane
   */
  double f_rho;

  /*!
   * \brief f_phi
   * phi-component in eigther polar or spheric coordinates of the 3D-part, angle of the point vector
   * projected to x-y-plane to the x-axis
   */
  double f_phi;

  /*!
   * \brief f_theta
   * theta-component of the 3D part in spheric coordinates, angle of the point vector to the z-axis
   */
  double f_theta;

  /*!
   * \brief value1
   * x, y, z, t-component in cartesic coordinates.
   */
  double value1, value2, value3, value4;

public:
  /*!
   * \brief point4D
   * Default constructor.
   */
  point4D() : point(0) { repre = _cartesic3_; }

  /*!
   * \brief point4D
   * Copy constructor. Sets the 3D-part p and the w-component.
   * \param p
   * \param w
   */
  point4D(const point3D& p, double w);

  /*!
   * \brief point4D
   * Copy constructor.
   * \param p
   */
  point4D(const point4D& p);

  /*!
   * \brief point4D
   * Copy constructor. Works only if p is 4-dimensional.
   * \param p
   */
  point4D(const point& p);

  /*!
   * \brief point4D
   * Copy constructor. Works only if p is 3-dimensional. Sets the 3D-part p, and the w-component.
   * \param p
   * \param w
   */
  point4D(const point& p, double w);

  /*!
   * \brief point4D
   * Constructor. Sets the cartesic components.
   * \param x
   * \param y
   * \param z
   * \param w
   */
  point4D(double x, double y, double z, double w) : point(0)
  {
    repre = _cartesic3_;
    setValues(x, y, z, w);
  }

  /*!
   * \brief setValues
   * Set the cartesic coordinates.
   * \param x
   * \param y
   * \param z
   * \param w
   */
  virtual void setValues(double x, double y, double z, double w);

  /*!
   * \brief setValues
   * Set the cartesic coordinates of the 3D-part.
   * \param x
   * \param y
   * \param z
   */
  virtual void setValues(double x, double y, double z);

  /*!
   * \brief setValue
   * Set the posth component in cartesic coordinates to valueIn.
   * \param pos
   * \param valueIn
   */
  void setValue(int pos, double valueIn) override
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

  /*!
   * \brief getDimension
   * Returns the point's dimension which is 4.
   * \return
   */
  int getDimension() const override { return 4; }

  /*!
   * \brief getValue
   * Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double getValue(int pos) const override
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

  /*!
   * \brief W
   * Returns the w-component in cartesic coordinates.
   * \return
   */
  double W() const { return value4; }

  /*!
   * \brief X
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  double X() const { return value1; }

  /*!
   * \brief Y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  double Y() const { return value2; }

  /*!
   * \brief Z
   * Returns the z-component in cartesic coordinates.
   * \return
   */
  double Z() const { return value3; }

  /*!
   * \brief Phi
   * Returns the phi-component of the 3D-part in eigther polar or spheric coordinates, angle of the
   * point vector projected to x-y-plane to the x-axis.
   * \return
   */
  double Phi() const { return f_phi; }

  /*!
   * \brief Theta
   * Returns the theta-component of the 3D-part in spheric coordinates, angle of the point vector to
   * the z-axis.
   * \return
   */
  double Theta() const { return f_theta; }

  /*!
   * \brief Rho
   * Returns the rho-component of the 3D-part in cylindric coordinates, length of the point vector
   * projected on the x-y-plane.
   * \return
   */
  double Rho() const { return f_rho; }

  /*!
   * \brief R
   * Returns the r-component of the 3D-part in spheric coordinates, distance to the origin.
   * \return
   */
  double R() const { return f_r; }

  /*!
   * \brief w
   * Returns the w-component in cartesic coordinates.
   * \return
   */
  double w() const { return value4; }

  /*!
   * \brief x
   * Returns the x-component in cartesic coordinates.
   * \return
   */
  double x() const { return value1; }

  /*!
   * \brief y
   * Returns the y-component in cartesic coordinates.
   * \return
   */
  double y() const { return value2; }

  /*!
   * \brief z
   * Returns the z-component in cartesic coordinates.
   * \return
   */
  double z() const { return value3; }

  /*!
   * \brief phi
   * Returns the phi-component of the 3D-part in eigther polar or spheric coordinates, angle of the
   * point vector projected to x-y-plane to the x-axis.
   * \return
   */
  double phi() const { return f_phi; }

  /*!
   * \brief theta
   * Returns the theta-component of the 3D-part in spheric coordinates, angle of the point vector to
   * the z-axis.
   * \return
   */
  double theta() const { return f_theta; }

  /*!
   * \brief rho
   * Returns the rho-component of the 3D-part in cylindric coordinates, length of the point vector
   * projected on the x-y-plane.
   * \return
   */
  double rho() const { return f_rho; }

  /*!
   * \brief r
   * Returns the r-component of the 3D-part in spheric coordinates, distance to the origin.
   * \return
   */
  double r() const { return f_r; }

  /*!
   * \brief getRep
   * Returns the representation of the 3D-part.
   * \return
   */
  rep3D getRep() const { return repre; }

  /*!
   * \brief setRep
   * Set the representation of the 3D-part.
   * \param r
   */
  void setRep(rep3D r) { repre = r; }

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimension of the point is 4.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator. Sets the 3D-part.
   * \param p
   */
  void operator=(const point3D& p) { setValues(p[0], p[1], p[2], W()); }

  /*!
   * \brief operator =
   * Copy operator.
   * \param p
   */
  void operator=(const point4D& p);

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the distance between the points is less than __prec
   * (1e-6).
   * \param p
   * \return
   */
  int operator==(const point4D& p)
  {
    return ((value1 - p.X() < __prec) && (value2 - p.Y() < __prec) && (value3 - p.Z() < __prec) &&
            (W() - p.W() < __prec));
  }

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override
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

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the points own representation (pt=-1, default) for the 3D-part,
   * the w-component is added.
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
ostream& operator<<(ostream& o, const point4D& p);

/*****Vector class for vectors in 3+1-dimensional space*/
/*!
 * \brief The vector4D class
 * Vector in 3+1-dimensional space.
 */
class vector4D : public point4D, public Vector
{
protected:
  /*!
   * \brief Zero
   * Sets the state of the objet to _zero_.
   */
  void Zero() override { point4D::Zero(); }

  /*!
   * \brief Infinite
   * Sets the state of the objet to _infinite_.
   */
  void Infinite() override { point4D::Infinite(); }

  /*!
   * \brief Undefined
   * Sets the state of the objet to _undefined_.
   */
  void Undefined() override { point4D::Undefined(); }

  /*!
   * \brief Regular
   * Sets the state of the objet to _regular_.
   */
  void Regular() override { point4D::Regular(); }

public:
  /*!
   * \brief vector4D
   * Default constructor.
   */
  vector4D() : point4D(), Vector(0) {}

  /*!
   * \brief vector4D
   * Copy constructor. Sets a point vector from the origin to point p. Works only if p is
   * 4-dimensional.
   * \param p
   */
  vector4D(const point& p) : point4D(p), Vector(0) {}

  /*!
   * \brief vector4D
   * Copy constructor. Sets a point vector from the origin to point p for the 3D-part.
   * \param p
   */
  vector4D(const point3D& p) : point4D(p), Vector(0) {}

  /*!
   * \brief vector4D
   * Copy constructor. Sets a point vector from the origin to point p.
   * \param p
   */
  vector4D(const point4D& p) : point4D(p), Vector(0) {}

  /*!
   * \brief vector4D
   * Copy constructor. Works only if v is 4-dimensional.
   * \param v
   */
  vector4D(const Vector& v);

  /*!
   * \brief vector4D
   * Copy constructor. Sets the 3D-part.
   * \param v
   */
  vector4D(const vector3D& v);

  /*!
   * \brief vector4D
   * Copy constructor.
   * \param v
   */
  vector4D(const vector4D& v);

  /*!
   * \brief vector4D
   * Constructor. Sets the cartesic components of the 3D-part.
   * \param x
   * \param y
   * \param z
   */
  vector4D(double x, double y, double z) : point4D(x, y, z, 0), Vector(0) {}

  /*!
   * \brief vector4D
   * Constructor. Sets the cartesic components.
   * \param x
   * \param y
   * \param z
   * \param w
   */
  vector4D(double x, double y, double z, double w) : point4D(x, y, z, w), Vector(0) {}

  /*!
   * \brief setValues
   * Set the cartesic components.
   * \param x
   * \param y
   * \param z
   * \param w
   */
  void setValues(double x, double y, double z, double w) override
  {
    point4D::setValues(x, y, z, w);
  }

  /*!
   * \brief setValues
   * Set the cartesic components of the 3D-part.
   * \param x
   * \param y
   * \param z
   */
  void setValues(double x, double y, double z) override { point4D::setValues(x, y, z); }

  /*!
   * \brief setValue
   * Set the posth component in cartesic coordinates to valueIn.
   * \param pos
   * \param valueIn
   */
  void setValue(int pos, double valueIn) override { point4D::setValue(pos, valueIn); }

  /*!
   * \brief getValue
   * Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double getValue(int pos) const override { return point4D::getValue(pos); }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return point4D::getState(); }

  /*!
   * \brief getDimension
   * Returns the vector's dimension which s 4.
   * \return
   */
  int getDimension() const override { return 4; }

  /*!
   * \brief normalize
   * Sets the length of the vector to 1.
   */
  void normalize() override
  {
    double t = length();
    if (t == 0)
      return;
    setValues(point4D::value1 / t, point4D::value2 / t, point4D::value3 / t, point4D::value4 / t);
  }

  /*!
   * \brief length
   * Returns the length of the vector.
   * \return
   */
  double length() const override
  {
    return sqrt(point4D::f_r * point4D::f_r + point4D::value4 * point4D::value4);
  }

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimension of p is 4. Sets a point vector from the origin to
   * point p.
   * \param p
   */
  void operator=(const point& p);

  /*!
   * \brief operator =
   * Copy operator. Works only if the dimension of v is 4
   * \param v
   */
  void operator=(const Vector& v);

  /*!
   * \brief operator =
   * Copy operator. Sets a point vector from the origin to point p.
   * \param p
   */
  void operator=(const point4D& p);

  /*!
   * \brief operator =
   * Copy operator.
   * \param v
   */
  void operator=(const vector4D& v);

  /*!
   * \brief operator =
   * Copy operator. Sets the 3D-part. Sets a point vector from the origin to point p.
   * \param p
   */
  void operator=(const point3D& p);

  /*!
   * \brief operator =
   * Copy operator. Sets the 3D-part.
   * \param v
   */
  void operator=(const vector3D& v);

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the difference between the vectors is less than __prec
   * (1e-6).
   * \param v
   * \return
   */
  int operator==(const vector4D& v) const
  {
    return ((point4D::value4 - v.W() < __prec) && (point4D::value1 - v.X() < __prec) &&
            (point4D::value2 - v.Y() < __prec) && (point4D::value3 - v.Z() < __prec));
  }

  /*!
   * \brief operator !
   * Transposes the vector and returns the result. A line vector returns a column vector, a column
   * vector a line vector. This vector remains unchanged.
   * \return
   */
  vector4D operator!() const
  {
    vector4D v(*this);
    v.transpose();
    return v;
  }

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override { return point4D::operator[](pos); }

  /*!
   * \brief toString
   * Returns a string representation of the vector. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the vector's own representation (pt=-1, default) for the 3D-part.
   * The w-component is added.
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param v
 * \return
 */
ostream& operator<<(ostream& o, const vector4D& v);

/*****Vector class for momentum in relativistic calculations*/
/*!
 * \brief The momentum4D class
 * This class represents a 4D-momentum vector as used in physics.
 */
class momentum4D : public vector4D
{
protected:
  /*!
   * \brief velocity
   * Velocity vector of the particle.
   */
  vector3D velocity;

  /*!
   * \brief mass
   * Mass of the particle.
   */
  double mass;

  /*!
   * \brief gamma
   * Gamma value of the particle this is 1/sqrt(1-beta²).
   */
  double gamma;

public:
  /*!
   * \brief momentum4D
   * Default constructor.
   */
  momentum4D() : vector4D() { Undefined(); }

  /*!
   * \brief momentum4D
   * Copy constructor. Interpret the first 3 values of p as momentum components and p[3] as energy.
   * \param p
   */
  momentum4D(const point4D& p);

  /*!
   * \brief momentum4D
   * Copy constructor. Interpret the first 3 values of v as momentum components and v[3] as energy.
   * \param v
   */
  momentum4D(const vector4D& v);

  /*!
   * \brief momentum4D
   * Copy constructor.
   * \param m
   */
  momentum4D(const momentum4D& m);

  /*!
   * \brief momentum4D
   * Constructor. Set the momentum in 3D-space and the energy of the particle.
   * \param p
   * \param energy
   */
  momentum4D(const point3D& p, double energy);

  /*!
   * \brief momentum4D
   * Constructor. Set the cartesic momentum components and the energy of the particle.
   * \param x
   * \param y
   * \param z
   * \param e
   */
  momentum4D(double x, double y, double z, double e);

  /*!
   * \brief setPM
   * Sets the particles properties. Vector is interpreted as momentum components, scalar as mass.
   * \param p
   * \param m
   */
  void setPM(const vector3D& p, double m);

  /*!
   * \brief setVM
   * Sets the particles properties. Vector is interpreted as velocity components, scalar as mass.
   * \param v
   * \param m
   */
  void setVM(const vector3D& v, double m);

  /*!
   * \brief setVE
   * Sets the particles properties. Vector is interpreted as velocity components, scalar as energy.
   * \param v
   * \param e
   */
  void setVE(const vector3D& v, double e);

  /*!
   * \brief setPE
   * Sets the particles properties. Vector is interpreted as momentum components, scalar as energy.
   * \param p
   * \param e
   */
  void setPE(const vector3D& p, double e)
  {
    mass = sqrt(abs(e * e - p.R() * p.R()));
    velocity = p * (1 / e);
    gamma = 1 / sqrt(1 - velocity.R() * velocity.R());
    point4D::setValues(p.X(), p.Y(), p.Z(), e);
  }

  /*!
   * \brief setValues
   * \param x
   * \param y
   * \param z
   * \param e
   */
  void setValues(double x, double y, double z, double e) override { setPE(vector3D(x, y, z), e); }

  /*!
   * \brief Energy
   * Return the energy of the particle.
   * \return
   */
  double Energy() const { return point4D::value4; }

  /*!
   * \brief Mass
   * Returns the mass of the particle.
   * \return
   */
  double Mass() const { return mass; }

  /*!
   * \brief Beta
   * Returns the scalar velocity of the particle.
   * \return
   */
  double Beta() const { return velocity.R(); }

  /*!
   * \brief Gamma
   * Returns the gamma value of the particle: 1/sqrt(1-beta^2).
   * \return
   */
  double Gamma() const { return gamma; }

  /*!
   * \brief Rapidity
   * Return the Rapidity of the particle along axis. 1/2*ln((e+pt) / (e-pt))
   * \return
   */
  double Rapidity() const
  {
    return 0.5 * (log((point4D::value4 + point4D::value3) / (point4D::value4 - point4D::value3)));
  }

  /*!
   * \brief Rapidity
   * Return the Rapidity of the particle along axis. 1/2*ln((e+pt) / (e-pt))
   * \param axis
   * \return
   */
  double Rapidity(const vector3D& axis) const;

  /*!
   * \brief px
   * Return the x-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double px() const { return point4D::value1; }

  /*!
   * \brief py
   * Return the y-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double py() const { return point4D::value2; }

  /*!
   * \brief pz
   * Return the z-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double pz() const { return point4D::value3; }

  /*!
   * \brief e
   * Return the particles energy.
   * \return
   */
  double e() const { return point4D::value4; }

  /*!
   * \brief Px
   * Return the x-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double Px() const { return point4D::value1; }

  /*!
   * \brief Py
   * Return the y-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double Py() const { return point4D::value2; }

  /*!
   * \brief Pz
   * Return the z-component of the momentum of the particle in cartesic coordinates.
   * \return
   */
  double Pz() const { return point4D::value3; }

  /*!
   * \brief E
   * Return the particles energy.
   * \return
   */
  double E() const { return point4D::value4; }

  /*!
   * \brief Pt
   * Return the transverse momentum of the particle perpendicular to z-axis.
   * \return
   */
  double Pt() const { return point4D::value3; }

  /*!
   * \brief Pt
   * Return the transverse momentum of the particle perpendicular to axis.
   * \param axis
   * \return
   */
  double Pt(const vector3D& axis) const
  {
    return (Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }

  /*!
   * \brief Pt_v
   * Return the transverse momentum-vector perpendicular to axis.
   * \param axis
   * \return
   */
  vector3D Pt_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R());
  }

  /*!
   * \brief Pl
   * Return the longitudinal momentum of the particle along z-axis.
   * \return
   */
  double Pl() const { return f_rho; }

  /*!
   * \brief Pl
   * Return the longitudinal momentum of the particle along axis.
   * \param axis
   * \return
   */
  double Pl(const vector3D& axis) const
  {
    return (axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }

  /*!
   * \brief Pl_v
   * Return the longitudinal momentum-vector of the particle along axis.
   * \param axis
   * \return
   */
  vector3D Pl_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return axis * (Momentum() * axis) / (axis.R() * axis.R());
  }

  /*!
   * \brief pt
   * Return the transverse momentum of the particle perpendicular to z-axis.
   * \return
   */
  double pt() const { return point4D::value3; }

  /*!
   * \brief pt
   * Return the transverse momentum of the particle perpendicular to axis.
   * \param axis
   * \return
   */
  double pt(const vector3D& axis) const
  {
    return (Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }

  /*!
   * \brief pt_v
   * Return the transverse momentum-vector perpendicular to axis.
   * \param axis
   * \return
   */
  vector3D pt_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return Momentum() - axis * (Momentum() * axis) / (axis.R() * axis.R());
  }

  /*!
   * \brief pl
   * Return the longitudinal momentum of the particle along axis.
   * \return
   */
  double pl() const { return f_rho; }

  /*!
   * \brief pl
   * \param axis
   * \return
   */
  double pl(const vector3D& axis) const
  {
    return (axis * (Momentum() * axis) / (axis.R() * axis.R())).R();
  }

  /*!
   * \brief pl_v
   * Return the longitudinal momentum-vector of the particle along axis.
   * \param axis
   * \return
   */
  vector3D pl_v(const vector3D& axis = vector3D(0, 0, 1)) const
  {
    return axis * (Momentum() * axis) / (axis.R() * axis.R());
  }

  /*!
   * \brief Velocity
   * Return the velocity of the particle.
   * \return
   */
  vector3D Velocity() const { return velocity; }

  /*!
   * \brief Momentum
   * Return the momentum of the particle.
   * \return
   */
  vector3D Momentum() const { return vector3D(point4D::value1, point4D::value2, point4D::value3); }

  /*!
   * \brief Direction
   * Return the direction of the momentum of the particle.
   * \return
   */
  vector3D Direction() const
  {
    vector3D ret(point4D::value1, point4D::value2, point4D::value3);
    ret.normalize();
    return ret;
  }

  /*!
   * \brief boost
   * Returns a particle that is Lorentz-boosted by beta.
   * \param beta
   * \return
   */
  momentum4D boost(const vector3D& beta) const;

  /*!
   * \brief CM
   * Static method to calculate the center of momentum for up to 5 particles.
   * \param v1
   * \param v2
   * \param v3
   * \param v4
   * \param v5
   * \return
   */
  static momentum4D CM(const momentum4D& v1, const momentum4D& v2,
                       const momentum4D& v3 = momentum4D(), const momentum4D& v4 = momentum4D(),
                       const momentum4D& v5 = momentum4D());

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the difference between the particles is less than __prec
   * (1e-6).
   * \param v
   * \return
   */
  int operator==(const momentum4D& v) const
  {
    return ((Velocity() == v.Velocity()) && (Energy() == v.Energy()));
  }

  /*!
   * \brief operator =
   * Copy operator.
   * \param v
   */
  void operator=(const momentum4D& v) { setPE(vector3D(v[0], v[1], v[2]), v[3]); }

  /*!
   * \brief operator !
   * Transposes the vector and returns the result. A line vector returns a column vector, a column
   * vector a line vector. This vector remains unchanged. Inherrited from superclass.
   * \return
   */
  momentum4D operator!() const
  {
    momentum4D m(*this);
    m.transpose();
    return m;
  }

  /*!
   * \brief operator +
   * Addition operation. Returns the sum of this particle and v.
   * \param v
   * \return
   */
  momentum4D operator+(const momentum4D& v) const
  {
    return momentum4D(Momentum() + v.Momentum(), v.Energy() + Energy());
  }

  /*!
   * \brief operator -
   * Subtraction operator. Returns the difference between this particle and v.
   * \param v
   * \return
   */
  momentum4D operator-(const momentum4D& v) const
  {
    return momentum4D(Momentum() - v.Momentum(), Energy() - v.Energy());
  }

  /*!
   * \brief operator *
   * Multiplication operator. Returns the scalar product of this particle and v. result =
   * e*ev-(p*pv)
   * \param v
   * \return
   */
  double operator*(const momentum4D& v) const
  {
    return (v.Energy() * point4D::value4 - Momentum() * v.Momentum());
  }

  /*!
   * \brief operator *
   * Multiplication operator. Returns a particle with f times the momentum and f times the energy of
   * this particle.
   * \param factor
   * \return
   */
  momentum4D operator*(double factor) const
  {
    return momentum4D(Momentum() * factor, point4D::value4 * factor);
  }

  /*!
   * \brief operator []
   * Access operator. Returns the posth component in cartesic coordinates.
   * \param pos
   * \return
   */
  double operator[](int pos) const override { return vector4D::operator[](pos); }

  /*!
   * \brief toString
   * Returns a string representation of the particle. pt is 4*ptparticle+pt3D. pt_particle is here
   * |momentum|veclocity ||energy|mass|
|0||	2|
|1||	3|
pt3D can be eigther _cartesic3_, _cylindric_, _spheric_ or in the momentum's own representation
(pt=-1, momentum-energy, default).
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator *
 * \param factor
 * \param v
 * \return
 */
momentum4D operator*(double factor, const momentum4D& v);

/*!
 * \brief operator <<
 * \param o
 * \param m
 * \return
 */
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

/*!
 * \brief The sLine2D class
 * Straight unsimited line in 2 dimensions.
 */
class sLine2D : public geomObject
{
private:
  point2D foot;
  vector2D d;

protected:
  /*!
   * \brief Infinite
   * Sets the state of the object to _infinite_.
   */
  void Infinite() override;

  /*!
   * \brief Undefined
   * Sets the state of the object to _undefined_.
   */
  void Undefined() override;

  /*!
   * \brief Regular
   * Sets the state of the object to _regular_.
   */
  void Regular() override;

public:
  /*!
   * \brief sLine2D
   * Default constructor.
   */
  sLine2D() : geomObject() {}

  /*!
   * \brief sLine2D
   * Constructor. Sets p as footing point and uses the vector q-p as direction.
   * \param p
   * \param q
   */
  sLine2D(const point2D& p, const point2D& q);

  /*!
   * \brief sLine2D
   * Constructor. Sets a footing point and the direction.
   * \param p
   * \param v
   */
  sLine2D(const point2D& p, const vector2D& v);

  /*!
   * \brief sLine2D
   * Copy constructor.
   * \param l
   */
  sLine2D(const sLine2D& l);

  /*!
   * \brief Foot
   * Returns the footing point of the line.
   * \return
   */
  point2D Foot() const { return foot; }

  /*!
   * \brief Direction
   * Return the direction-vector of the line.
   * \return
   */
  vector2D Direction() const { return d; }

  /*!
   * \brief shiftFoot
   * The new foot is fnew=fold+t*v.
   * \param t
   */
  void shiftFoot(double t) { foot = foot + d * t; }

  /*!
   * \brief newLine
   * Assigns a new line with footing point p and direction v.
   * \param p
   * \param v
   */
  void newLine(const point2D& p, const vector2D& v);

  /*!
   * \brief isIn
   * Returns true if the distance of point p to the line is smaller than __prec (1e-6).
   * \param p
   * \return
   */
  int isIn(const point2D& p) const;

  /*!
   * \brief getRep
   * Returns the representation of the line.
   * \return
   */
  repTD getRep() const { return (d.isCartesic() ? _cartesic_ : _polar_); }

  /*!
   * \brief setRep
   * Sets the representation of the line. This can be _cartesic_ or _polar_.
   * \param r
   */
  void setRep(repTD r);

  /*!
   * \brief isLine
   * Returns true if the direction vector is a line vector.
   * \return
   */
  int isLine() const { return d.isLine(); }

  /*!
   * \brief isColumn
   * Returns true if the direction vector is a column vector.
   * \return
   */
  int isColumn() const { return d.isColumn(); }

  /*!
   * \brief transpose
   * Transposes the direction vector. A line vector returns a column vector, a column vector a line
   * vector.
   */
  void transpose() { d.transpose(); }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return geomObject::getState(); }

  /*!
   * \brief getDimension
   * Returns the dimension of the line which is 2.
   * \return
   */
  int getDimension() const { return 2; }

  /*!
   * \brief Zero
   * Sets the state of the object to _regular_.
   */
  void Zero() override
  {
    geomObject::Zero();
    d.setValues(0, 0);
    foot.Zero();
  }

  /*!
   * \brief operator -
   * Returns the distance of point p to this line.
   * \param p
   * \return
   */
  vector2D operator-(const point2D& p) const; // distance line - point

  /*!
   * \brief operator &&
   * Returns the intersection point of two straight lines.
   * \param l
   * \return
   */
  point2D operator&&(const sLine2D& l) const; // common point of 2 lines

  /*!
   * \brief operator &
   * Returns the minimal distance of the line l and this line. It is zero if the two lines
   * intersect, or non-zero if the lines are parallel.
   * \param l
   * \return
   */
  double operator&(const sLine2D& l) const; // distance of two lines

  /*!
   * \brief operator -
   * Subtraction operator. Returns a limited straight line with length zero. The begin and end point
   * is the intersection point of the line l and this line.
   * \param l
   * \return
   */
  lLine2D operator-(const sLine2D& l) const; // closest approach of 2 lines

  /*!
   * \brief operator -
   * Subtraction operator. Returns a limited straight line with length zero. The begin and end point
   * is the intersection point of the line l and this line. If the l doesn't intersect with this
   * line the return value is the line of closest approach between the limited line and this line.
   * \param l
   * \return
   */
  lLine2D operator-(const lLine2D& l) const; // closest approach of 2 lines

  /*!
   * \brief operator =
   * Copy operator.
   * \param l
   */
  void operator=(const sLine2D& l);

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt this can be eigther _cartesic_,
   * _polar_ or in the points own representation (pt=-1, default) for the direction vector.
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param l
 * \return
 */
ostream& operator<<(ostream& o, const sLine2D& l);

/*****Line between 2 points in 2-Dimensions */
/*!
 * \brief The lLine2D class
 * This is a limited straight line in 2-dimensions. In eukledean space the shortest connection
 * between two points P and Q.
 */
class lLine2D : public geomObject
{
private:
  point2D pPoint;
  point2D qPoint;

protected:
  /*!
   * \brief Infinite
   * Sets the state of the object to _infinite_.
   */
  void Infinite() override { geomObject::Infinite(); }

  /*!
   * \brief Undefined
   * Sets the state of the object to _undefined_.
   */
  void Undefined() override { geomObject::Undefined(); }

  /*!
   * \brief Regular
   * Sets the state of the object to _regular_.
   */
  void Regular() override { geomObject::Regular(); }

public:
  /*!
   * \brief lLine2D
   * Default constructor.
   */
  lLine2D() : geomObject(), qPoint() { geomObject::Undefined(); }

  /*!
   * \brief lLine2D
   * Constructor. Sets the end points.
   * \param p
   * \param q
   */
  lLine2D(const point2D& p, const point2D& q);

  /*!
   * \brief lLine2D
   * Constructor. First end point is p, second end point is p+v.
   * \param p
   * \param v
   */
  lLine2D(const point2D& p, const vector2D& v);

  /*!
   * \brief lLine2D
   * Copy constructor.
   * \param l
   */
  lLine2D(const lLine2D& l);

  /*!
   * \brief P
   * Returns the first end point of the limited straight line.
   * \return
   */
  point2D P() const { return pPoint; }

  /*!
   * \brief Q
   * Returns the second end point of the limited straight line.
   * \return
   */
  point2D Q() const { return qPoint; }

  /*!
   * \brief Direction
   * Returns the direction from P to Q.
   * \return
   */
  vector2D Direction() const { return (qPoint - pPoint); }

  /*!
   * \brief Mid
   * Return the mid of the line.
   * \return
   */
  point2D Mid() const { return pPoint + ((qPoint - pPoint) * 0.5); }

  /*!
   * \brief isIn
   * Returns true if the distance of p to the line is smaller than __prec (1e-6)
   * \param p
   * \return
   */
  int isIn(const point2D& p) const
  {
    vector2D vv = (*this) - p;
    return ((vv.R() < __prec) ? 1 : 0);
  }

  /*!
   * \brief newLine
   * Assign a new line.
   * \param p
   * \param q
   */
  void newLine(const point2D& p, const point2D& q);

  /*!
   * \brief getRep
   * Returns the representation of the line.
   * \return
   */
  repTD getRep() const { return (pPoint.isCartesic() ? _cartesic_ : _polar_); }

  /*!
   * \brief length
   * Returns the length of the line.
   * \return
   */
  double length() const { return (pPoint - qPoint).R(); }

  /*!
   * \brief setRep
   * Set the representation of the line, this can be eigther _cartesic_ or _polar_.
   * \param r
   */
  void setRep(repTD r);

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return geomObject::getState(); }

  /*!
   * \brief getDimension
   * Returns the dimension of the line which is 2.
   * \return
   */
  int getDimension() const { return 2; }

  /*!
   * \brief Zero
   * Sets the state of the object to _zero_.
   */
  void Zero() override
  {
    geomObject::Zero();
    pPoint.Zero();
    qPoint.Zero();
  }

  /*!
   * \brief operator =
   * Copy operator.
   * \param l
   */
  void operator=(const lLine2D& l);

  /*!
   * \brief operator -
   * Returns the distance of point p to the line.
   * \param p
   * \return
   */
  vector2D operator-(const point2D& p) const;

  /*!
   * \brief operator -
   * Returns the line of closest approach between two straight lines. If the lines intersect, both
   * endpoints of the resulting line are the intersection point. If they don't intersect, the first
   * endpoint lies on this line the second on l.
   * \param l
   * \return
   */
  lLine2D operator-(const lLine2D& l) const;

  /*!
   * \brief operator &&
   * Returns the intersection point of two limited straight lines.
   * \param l
   * \return
   */
  point2D operator&&(const lLine2D& l) const
  {
    lLine2D tmpc = (*this) - l;
    return tmpc.Mid();
  }

  /*!
   * \brief operator &
   * Returns the distance between two limited straight lines.
   * \param l
   * \return
   */
  double operator&(const lLine2D& l) const { return ((*this) - l).length(); }

  /*!
   * \brief toString
   * Returns a string representation of the point. Depending on pt this can be eigther _cartesic_,
   * _polar_ or in the points own representation (pt=-1, default) for the direction vector.
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param l
 * \return
 */
ostream& operator<<(ostream& o, const lLine2D& l);

/*****Straight Line in 3-Dimensions*/
class lLine3D;
class sLine3D;

/*!
 * \brief The sLine3D class
 * This class describes a straight line in 3 dimensional space.
 */
class sLine3D : public geomObject
{
private:
  point3D foot;
  vector3D d;

protected:
  /*!
   * \brief Infinite
   * Sets the state of the object to _infinite_.
   */
  void Infinite() override { geomObject::Infinite(); }

  /*!
   * \brief Undefined
   * Sets the state of the object to _undefined_.
   */
  void Undefined() override
  {
    geomObject::Undefined();
    foot = point3D();
    d = vector3D();
  }

  /*!
   * \brief Regular
   * Sets the state of the object to _regular_.
   */
  void Regular() override { geomObject::Regular(); }

public:
  /*!
   * \brief sLine3D
   * Default constructor.
   */
  sLine3D() : geomObject() {}

  /*!
   * \brief sLine3D
   * Constructor. Sets the footing point to p and the direction to p-q.
   * \param p
   * \param q
   */
  sLine3D(const point3D& p, const point3D& q);

  /*!
   * \brief sLine3D
   * Constructor. Sets the footing point and the direction of the line.
   * \param p
   * \param v
   */
  sLine3D(const point3D& p, const vector3D& v);

  /*!
   * \brief sLine3D
   * Copy constructor.
   * \param l
   */
  sLine3D(const sLine3D& l);

  /*!
   * \brief Foot
   * Returns the footing point of the line.
   * \return
   */
  point3D Foot() const { return foot; }

  /*!
   * \brief Direction
   * Returns the direction of the line.
   * \return
   */
  vector3D Direction() const { return d; }

  /*!
   * \brief shiftFoot
   * Shifts the footing point along the line: fnew = fold + t*v
   * \param t
   */
  void shiftFoot(double t) { foot = foot + d * t; }

  /*!
   * \brief newLine
   * Assign new properties to the line.
   * \param p
   * \param v
   */
  void newLine(const point3D& p, const vector3D& v);

  /*!
   * \brief isIn
   * Returns true if the distance of the point p to the line is smaller than __prec (1e-6).
   * \param p
   * \return
   */
  int isIn(const point3D& p) const;

  /*!
   * \brief getRep
   * Returns the line's representation.
   * \return
   */
  rep3D getRep() const { return d.getRep(); }

  /*!
   * \brief setRep
   * Set the representation of the line, this can be eigther _cartesic3_, _cylindric_ or _spheric_.
   * \param r
   */
  void setRep(rep3D r)
  {
    d.setRep(r);
    foot.setRep(r);
  }

  /*!
   * \brief isLine
   * Returns true if the direction vector is a line vector.
   * \return
   */
  int isLine() const { return d.isLine(); }

  /*!
   * \brief isColumn
   * Returns true if the direction vector is a column vector.
   * \return
   */
  int isColumn() const { return d.isColumn(); }

  /*!
   * \brief transpose
   * Transposes the direction vector. A line vector will lead to a column vector, a column vector to
   * a line vector.
   */
  void transpose() { d.transpose(); }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const override { return geomObject::getState(); }

  /*!
   * \brief getDimension
   * Returns the line's dimension which is 3.
   * \return
   */
  int getDimension() const { return 3; }

  /*!
   * \brief Zero
   * Sets the state of the object to _zero_.
   */
  void Zero() override
  {
    geomObject::Zero();
    d.setValues(0, 0, 0);
    foot.Zero();
  }

  /*!
   * \brief operator -
   * Returns the distance vector of p to this line. Vector points to line.
   * \param p
   * \return
   */
  vector3D operator-(const point3D& p) const // distance line - point
  {
    vector3D v = p - foot;
    vector3D n = v - d * (d * v);
    return n;
  }

  /*!
   * \brief operator &&
   * Returns the intersection point of this line with <l, if it exists.
   * \param l
   * \return
   */
  point3D operator&&(const sLine3D& l) const; // common point of 2 lines

  /*!
   * \brief operator &
   * Returns the minimal distance between this line and l.
   * \param l
   * \return
   */
  double operator&(const sLine3D& l) const; // distance of two lines

  /*!
   * \brief operator -
   * Returns the line of closest approach between this line and l. The first end point will lie on
   * this line the second one on l.
   * \param l
   * \return
   */
  lLine3D operator-(const sLine3D& l) const; // closest approach of 2 lines

  /*!
   * \brief operator -
   * Returns the line of closest approach between this line and l. The first end point will lie on
   * this line the second one on l.
   * \param l
   * \return
   */
  lLine3D operator-(const lLine3D& l) const; // closest approach of 2 lines

  /*!
   * \brief operator =
   * Copy operator.
   * \param l
   */
  void operator=(const sLine3D& l);

  /*!
   * \brief toString
   * Returns a string representation of the line. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the lines own representation (pt=-1, default). There are two
   * representations possible: one for the vector and one for the point: pt=4*ppoint+pvector.
   * \param pt
   * \return
   */
  string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param l
 * \return
 */
ostream& operator<<(ostream& o, const sLine3D& l);

/*****Line between 2 points in 3-Dimensions */
/*!
 * \brief The lLine3D class
 * This class represents a limited straight line. In eukledean space the shortest connection between
 * the points P and Q.
 */
class lLine3D : public geomObject
{
private:
  point3D pPoint;
  point3D qPoint;

protected:
  /*!
   * \brief Undefined
   * Sets the state of the object to _undefined_.
   */
  void Undefined()
  {
    pPoint.Undefined();
    qPoint = point3D();
    geomObject::Undefined();
  }

public:
  /*!
   * \brief lLine3D
   * Default constructor.
   */
  lLine3D() : geomObject(), qPoint() { Undefined(); }

  /*!
   * \brief lLine3D
   * Constructor. Sets the end points.
   * \param p
   * \param q
   */
  lLine3D(const point3D& p, const point3D& q);

  /*!
   * \brief lLine3D
   * Constructor. Sets the first end point to p and the second to p+v.
   * \param p
   * \param v
   */
  lLine3D(const point3D& p, const vector3D& v);

  /*!
   * \brief lLine3D
   * Copy constructor.
   * \param l
   */
  lLine3D(const lLine3D& l);

  /*!
   * \brief P
   * Returns the first endpoint.
   * \return
   */
  point3D P() const { return pPoint; }

  /*!
   * \brief Q
   * Returns the second endpoint.
   * \return
   */
  point3D Q() const { return qPoint; }

  /*!
   * \brief Direction
   * Returns the direction from the first endpoint P to the second endpoint Q.
   * \return
   */
  vector3D Direction() const { return (qPoint - pPoint); }

  /*!
   * \brief Mid
   * Returns the middle point of the limited straight line.
   * \return
   */
  point3D Mid() const { return pPoint + ((qPoint - pPoint) * 0.5); }

  /*!
   * \brief isIn
   * Returns true if the distance of the point p to this line is smaller than __prec (1e-6).
   * \param p
   * \return
   */
  int isIn(const point3D& p) const;

  /*!
   * \brief newLine
   * Assigns new properties to the line.
   * \param p
   * \param q
   */
  void newLine(const point3D& p, const point3D& q);

  /*!
   * \brief getRep
   * Returns the representation of the endpoints.
   * \return
   */
  rep3D getRep() const { return pPoint.getRep(); }

  /*!
   * \brief length
   * Returns the length of the limited straight line.
   * \return
   */
  double length() const { return (qPoint - pPoint).R(); }

  /*!
   * \brief setRep
   * Set the representation of the endpoints this can be eigther _cartesic3_, _cylindric_ or
   * _spheric_.
   * \param r
   */
  void setRep(rep3D r)
  {
    pPoint.setRep(r);
    qPoint.setRep(r);
  }

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const { return geomObject::getState(); }

  /*!
   * \brief getDimension
   * Returns the lines dimension which is 3.
   * \return
   */
  int getDimension() const { return 3; }

  /*!
   * \brief Zero
   * Sets the state of the object to _zero_.
   */
  void Zero()
  {
    geomObject::Zero();
    pPoint.Zero();
    qPoint.Zero();
  }

  /*!
   * \brief operator =
   * Copy operator.
   * \param l
   */
  void operator=(const lLine3D& l);

  /*!
   * \brief operator -
   * Returns the distance of a point to this line. Vector points to this line.
   * \param p
   * \return
   */
  vector3D operator-(const point3D& p) const;

  /*!
   * \brief operator &&
   * Returns the intersection point of this line with l, if it exists.
   * \param l
   * \return
   */
  point3D operator&&(const lLine3D& l) const;

  /*!
   * \brief operator &
   * Returns the minimal distance between the two lines.
   * \param l
   * \return
   */
  double operator&(const lLine3D& l) const;

  /*!
   * \brief toString
   * Returns a string representation of the line. Depending on pt this can be eigther _cartesic3_,
   * _cylindric_, _spheric_ or in the points' own representation (pt=-1, default).
   * \param pt
   * \return
   */
  virtual string toString(int pt = -1) const;
};

ostream& operator<<(ostream& o, const lLine3D& l);

/*!
 * \brief The plane3D class
 * This is a plane in 3-dimensional space.
 */
class plane3D : public geomObject
{
private:
  point3D f;
  vector3D dir1;
  vector3D dir2;
  vector3D n_;

protected:
  /*!
   * \brief Infinite
   *  Sets the state of the object to _infinite_.
   */
  void Infinite() { geomObject::Infinite(); }

  /*!
   * \brief Undefined
   *  Sets the state of the object to _undefined_.
   */
  void Undefined() { geomObject::Undefined(); }

  /*!
   * \brief Regular
   *  Sets the state of the object to _regular_.
   */
  void Regular() { geomObject::Regular(); }

  /*!
   * \brief pr
   *  Representation of the plane: _normal_ or _parametric_.
   */
  planeRep pr;

public:
  /*!
   * \brief plane3D
   * Default constructor.
   */
  plane3D() : geomObject(), dir1(), dir2() { Undefined(); }

  /*!
   * \brief plane3D
   * Copy constructor.
   * \param p
   */
  plane3D(const plane3D& p);
  //  plane3D(triangle3D t);

  /*!
   * \brief plane3D
   * Constructor. Sets the footing point to a, the two directions are calculated from a to b or c
   * respectively.
   * \param a
   * \param b
   * \param c
   */
  plane3D(const point3D& a, const point3D& b, const point3D& c);

  /*!
   * \brief plane3D
   *  Constructor. Sets the footing point and the two directions making up the plane. Take care that
   * the angle between both directions is not too small.
   * \param foot
   * \param dir1
   * \param dir2
   */
  plane3D(const point3D& foot, const vector3D& dir1, const vector3D& dir2);

  /*!
   * \brief plane3D
   * Constructor. Set the footing point and the normal vector.
   * \param foot
   * \param norm
   */
  plane3D(const point3D& foot, const vector3D& norm);

  /*!
   * \brief Foot
   * Returns the footing point.
   * \return
   */
  point3D Foot() const { return f; }

  /*!
   * \brief Direction
   * Returns the ith direction of the plane.
   * \param i
   * \return
   */
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

  /*!
   * \brief Normal
   * Returns the normal vector of the plane.
   * \return
   */
  vector3D Normal() const { return n_; }

  /*!
   * \brief operator =
   * Copy operator.
   * \param p
   */
  void operator=(const plane3D& p);

  /*!
   * \brief operator ==
   * Comparison operator. Returns true if the difference between p's footing point and this plane is
   * less than __prec (1e-6).
   * \param p
   * \return
   */
  int operator==(const plane3D& p) const;

  /*!
   * \brief getState
   * Returns the state of the geometrical object. This can be _regular_, _undefined_, _infinite_ or
   * _zero_.
   * \return
   */
  geomState getState() const { return geomObject::getState(); }

  /*!
   * \brief Zero
   *  Sets the state of the object to _zero_.
   */
  void Zero()
  {
    geomObject::Zero();
    dir1.setValues(0, 0, 0);
    dir2.setValues(0, 0, 0);
  }

  /*!
   * \brief getRep
   *  Return the representation of plane's directions.
   * \return
   */
  rep3D getRep() const { return f.getRep(); }

  /*!
   * \brief setRep
   *  Set the representation of the plane's directions to eigther _cartesic3_, _spheric_ or
   * _cylindric_.
   * \param r
   */
  void setRep(rep3D r)
  {
    f.setRep(r);
    dir1.setRep(r);
    dir2.setRep(r);
    n_.setRep(r);
  }

  /*!
   * \brief getPlaneRep
   *  Returns the planes representation.
   * \return
   */
  planeRep getPlaneRep() const { return pr; }

  /*!
   * \brief setPlaneRep
   *  Set the planes representation to eigther _normal_ or _parametric_.
   * \param r
   */
  void setPlaneRep(planeRep r) { pr = r; }

  /*!
   * \brief setPlane
   *  Set the plane's footing point to foot, and its directions. Take care that the angle between
   * both directions is sufficiently large.
   * \param foot
   * \param dir1
   * \param dir2
   */
  void setPlane(const point3D& foot, const vector3D& dir1, const vector3D& dir2);

  /*!
   * \brief operator &
   *  Returns the distance of point p to this plane. Vector points to plane.
   * \param p
   * \return
   */
  vector3D operator&(const point3D& p) const; // Distance vector point-plane points to point

  /*!
   * \brief operator -
   *  Returns the intersection point of this plane with line l, if it exists.
   * \param l
   * \return
   */
  point3D operator-(const sLine3D& l) const; // hit point line-plane

  /*!
   * \brief operator -
   *  Returns the intersection point of this plane with line l, if it exists.
   * \param l
   * \return
   */
  point3D operator-(const lLine3D& l) const; // hit point line-plane

  /*!
   * \brief operator -
   *  Returns the intersection line of plane p with this plane.
   * \param p
   * \return
   */
  sLine3D operator-(const plane3D& p) const; // two planes hitting->straight line

  /*!
   * \brief toString
   *  Returns a string representation of the plane dependent on pt = 16*pplane + 4*pvector + pfoot
   * \param pt
   * \return
   */
  virtual string toString(int pt = -1) const;
};

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
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

/*!
 * \brief The matrix3D class
 * This class describes a 3-dimensional rotation matrix. Note that the inversion sometimes doesn't
 * work that well. Use matrixNxM instead.
 */
class matrix3D : public geomObject
{
private:
  //  vector3D v_0,v_1,v_2;
  double data[9];
  int unit, reel;

public:
  /*!
   * \brief matrix3D
   * Default constructor.
   */
  matrix3D();

  /*!
   * \brief matrix3D
   * Copy constructor.
   * \param m
   */
  matrix3D(const matrix3D& m);

  /*!
   * \brief matrix3D
   * Constructor. Define a rotation matrix to rotate around an axis by an angle.
   * \param axis
   * \param angle
   */
  matrix3D(const vector3D& axis, double angle);

  /*!
   * \brief matrix3D
   *  Constructor. Sets the elements of the matrix.
   * \param line1
   * \param line2
   * \param line3
   */
  matrix3D(const vector3D& line1, const vector3D& line2, const vector3D& line3);

  /*!
   * \brief matrix3D
   *  Constructor. The matrix is set as the dyadic product of the two vectors v1 and v2.
   * \param v1
   * \param v2
   */
  matrix3D(const vector3D& v1, const vector3D& v2);

  /*!
   * \brief ~matrix3D
   */
  virtual ~matrix3D();

  /*!
   * \brief getValue
   *  Returns the matrix element in (line,column).
   * \param line
   * \param column
   * \return
   */
  double getValue(int line, int column) const;

  /*!
   * \brief Determinant
   *  Returns the matrix's determinant.
   * \return
   */
  double Determinant() const;

  /*!
   * \brief setValue
   *  Set the value in lineth line and columnth colum to value.
   * \param line
   * \param column
   * \param value
   */
  void setValue(int line, int column, double value);

  /*!
   * \brief Zero
   *  Sets the state of the object to _zero_.
   */
  void Zero() override;

  /*!
   * \brief Unity
   *  Static method. Returns a unit -matrix.
   * \return
   */
  static matrix3D Unity();

  /*!
   * \brief Unitary
   *  Returns true if matrix is unitary.
   * \return
   */
  int Unitary() const;

  /*!
   * \brief Orthogonal
   *  Returns true if matrix is orthogonal.
   * \return
   */
  int Orthogonal() const;

  /*!
   * \brief Rotate
   *  Define a rotation matrix. Note this method will need some later work!
   * \param fromX
   * \param fromY
   * \param fromZ
   * \param toX
   * \param toY
   * \param toZ
   * \return
   */
  matrix3D Rotate(const vector3D& fromX, const vector3D& fromY, const vector3D& fromZ,
                  const vector3D& toX, const vector3D& toY, const vector3D& toZ);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param m
   */
  void operator=(const matrix3D& m);

  /*!
   * \brief operator *
   *  Multiplication operator. Multiply a vector v to this matrix and return the result. This works
   * only for column vectors.
   * \param v
   * \return
   */
  vector3D operator*(const vector3D& v) const;

  /*!
   * \brief operator *
   *  Multipliclation operator. Multiplies this matrix to matrix m and returns the result. This
   * matrix is left unchanged. Note this is genuine matrix multiplication. Commutation law doesn't
   * apply here.
   * \param m
   * \return
   */
  matrix3D operator*(const matrix3D& m) const;

  /*!
   * \brief operator *
   *  Multiplication operator. Multiplies v to each element of the matrix and returns the result.
   * This matrix is left unchanged.
   * \param v
   * \return
   */
  matrix3D operator*(double v) const;

  /*!
   * \brief operator +
   *  Addition operator. Adds m to this matrix and returns the result. This matrix is left
   * unchanged.
   * \param m
   * \return
   */
  matrix3D operator+(const matrix3D& m) const;

  /*!
   * \brief operator -
   *  Subtraction operator. Subtracts matrix m from this matrix and returns the result. This matrix
   * is left unchanged.
   * \param m
   * \return
   */
  matrix3D operator-(const matrix3D& m) const;

  /*!
   * \brief operator !
   *  Transposition operator. Returns the transposed matrix. This matrix is left unchanged.
   * \return
   */
  matrix3D operator!() const; // transpose

  /*!
   * \brief operator -
   *  Inversion operator. Return the inverse of this matrix. This matrix is left unchanged.
   * \return
   */
  matrix3D operator-() const; // invert

  /*!
   * \brief operator []
   *  Access operator. Returns the numth line of the matrix. To access the individual element you
   * can use: m[line][column].
   * \param num
   * \return
   */
  virtual vector3D operator[](int num) const; // returns a line of the matrix

  /*!
   * \brief toString
   *  Returns a string representation of the matrix. Contains line breaks.
   * \return
   */
  virtual string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param m
 * \return
 */
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

/*!
 * \brief The matrixNxM class
 * General matrix. Note that inversion only works for square matrices.
 */
class matrixNxM : public geomObject
{
private:
  int numberOfColumns;
  int numberOfLines;
  double* data; //[numberOfLines*numberOfColumns]
public:
  /*!
   * \brief matrixNxM
   * Constructor. Sets the number of lines to n, and the number of columns to m.
   * \param n
   * \param m
   */
  matrixNxM(int n = 1, int m = 1);

  /*!
   * \brief matrixNxM
   * Copy constructor.
   * \param mat
   */
  matrixNxM(const matrixNxM& mat);
#ifdef MATRIX_T
  matrixNxM(const matrixNxMT<double>& mat);
  matrixNxM(const matrixNxMT<float>& mat);
  matrixNxM(const matrixNxMT<long double>& mat);
#endif

  /*!
   * \brief matrixNxM
   * Constructor. Sets the number of lines to n, the number of columns to m. The elements are filled
   * from the array datas. Take care that datas is correctly dimensioned. The elements are read
   * line-wise.
   * \param n
   * \param m
   * \param datas
   */
  matrixNxM(int n, int m, double* datas);

  /*!
   * \brief Destructor.
   */
  ~matrixNxM();

  /*!
   * \brief getValue
   *  Return the element a position (line,column).
   * \param line
   * \param column
   * \return
   */
  double getValue(int line, int column) const;

  /*!
   * \brief setValue
   *  Set the element at position (line,column) to value.
   * \param line
   * \param column
   * \param value
   */
  void setValue(int line, int column, double value);

  /*!
   * \brief Columns
   * Returns the number of columns of this matrix.
   * \return
   */
  int Columns() const;

  /*!
   * \brief Lines
   * Returns the number of lines of the matrix.
   * \return
   */
  int Lines() const;

  /*!
   * \brief Unity
   *  Makes a unit matrix out of this matrix.
   */
  void Unity();

  /*!
   * \brief transpose
   *  Transposes the matrix.
   */
  void transpose();

  /*!
   * \brief Determinant
   *  Returns the determinant of the matrix.
   * \return
   */
  double Determinant() const;

  /*!
   * \brief Diagonal
   *  Returns true if the matrix is diagonal.
   * \return
   */
  bool Diagonal() const;

  /*!
   * \brief UpperTriangular
   *  Returns true if the matrix is a upper triangular matrix.
   * \return
   */
  bool UpperTriangular() const;

  /*!
   * \brief LowerTriangular
   *  Returns true if the matrix is a lower triangular matrix.
   * \return
   */
  bool LowerTriangular() const;

  /*!
   * \brief subset
   *  Return a subset of the matrix.
   * \param lineBegin
   * \param lineEnd
   * \param columnBegin
   * \param columnEnd
   * \return
   */
  matrixNxM subset(int lineBegin, int lineEnd, int columnBegin, int columnEnd) const;

  /*!
   * \brief decomposeLU
   *  Decompose the sqare matrix into a lower triangular matrix (saved in L) and an upper triangular
   * matrix (saved in R). A pre step for inversion.
   * \param L
   * \param U
   */
  void decomposeLU(matrixNxM& L, matrixNxM& U) const;

  /*!
   * \brief decomposeLDU
   *  Decompose the sqare matrix into a lower triangular matrix (saved in L), an upper triangular
   * matrix (saved in R) and a diagonal matrix. A pre step for inversion.
   * \param L
   * \param D
   * \param U
   */
  void decomposeLDU(matrixNxM& L, matrixNxM& D, matrixNxM& U) const;

  /*!
   * \brief decomposeQR
   *  Do a QR-decomposition of the matrix and save the results in the matrices Q and R. This is not
   * implemented yet, giving the reason why inversion is not working yet for non-square matrices.
   * \param Q
   * \param R
   */
  void decomposeQR(matrixNxM& Q, matrixNxM& R) const;

  /*!
   * \brief operator =
   *  Copy operator.
   * \param mat
   */
  void operator=(const matrixNxM& mat);
#ifdef MATRIX_T
  void operator=(const matrixNxMT<double>& mat);
  void operator=(const matrixNxMT<long double>& mat);
  void operator=(const matrixNxMT<float>& mat);
#endif

  /*!
   * \brief operator []
   *  Access operator. Return the numth line of the matrix. To access an individual element use
   * m[line][column].
   * \param num
   * \return
   */
  Vector operator[](int num) const; // line of matrix

  /*!
   * \brief operator ()
   *  Access operator. Return the numth column of the matrix.
   * \param num
   * \return
   */
  Vector operator()(int num) const; // column of matrix

  /*!
   * \brief operator *
   *  Multiplication operator. Multiplies this matrix to a vector v and returns the result. Works
   * only if v is a column vector and has 'number of columns' elements.
   * \param v
   * \return
   */
  Vector operator*(const Vector& v) const;

  /*!
   * \brief operator *
   *  Matrix multiplication. Multiplies this matrix to m and returns the result. This matrix is left
   * unchanged. Note this is genuine matrix multiplication. This does not commute. Works only if
   * this' number of columns equals m's number of lines.
   * \param m
   * \return
   */
  matrixNxM operator*(const matrixNxM& m) const;

  /*!
   * \brief operator *
   *  Multiplication operator. Multiplies value to all elements of the matrix and returns the
   * result. This matrix is left unchanged.
   * \param value
   * \return
   */
  matrixNxM operator*(double value) const;

  /*!
   * \brief operator +
   *  Addition operator. Adds m to this matrix and returns the result. This matrix is left
   * unchanged. Works only if this matrix and m have the same number of columns and number of lines.
   * \param m
   * \return
   */
  matrixNxM operator+(const matrixNxM& m) const;

  /*!
   * \brief operator -
   *  Suptraction operator. Subtracts m from this matrix and returns the result. This matrix is left
   * unchanged. Works only if this matrix and m have the same number of columns and number of lines.
   * \param m
   * \return
   */
  matrixNxM operator-(const matrixNxM& m) const;

  /*!
   * \brief operator !
   *  Transposition operator. Returns the transposed of the matrix.
   * \return
   */
  matrixNxM operator!() const; // transpose

  /*!
   * \brief operator -
   *  Inversion operator. Works reliably but only for square matrices. Returns the inverse of the
   * matrix and leaves the original matrix unchanged.
   * \return
   */
  matrixNxM operator-() const; // invert

  /*!
   * \brief toString
   *  Returns a string representation of the matrix. Note this contains line breaks.
   * \return
   */
  virtual string toString(int pt = -1) const override;
};

/*!
 * \brief operator <<
 * \param o
 * \param m
 * \return
 */
ostream& operator<<(ostream& o, const matrixNxM& m);

/*!
 * \brief operator *
 * \param v
 * \param m
 * \return
 */
Vector operator*(const Vector& v, const matrixNxM& m);

/*!
 * \brief The projector class
 * Static class to do projections from 3D to 2D. Used for drawing the more complicated shapes.
 */
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
