#pragma once

#include "geometry_detail.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <type_traits>

#include <sstream>
#include <string>

namespace geo {

/*!
 * \brief GeomState
 * This enum class defines the state of a geometrical object. In this unit, the only not yet used
 * state is Infinite.
 */
enum class GeomState
{
  /// Defines a regular state of the geometrical object.
  Regular,
  /// This is used if the geometrical object is default constructed or if the operation that
  /// produced it failed.
  Undefined,
  /// This is used if at some point the geometrical object is infinite.
  Infinite,
  /// This is used if the geometrical object is in the special Null state. This could be that
  /// a vector has zero length or that a compond object has been non-default constructed but it is
  /// not really correct (example: line with zero length direction).
  Null,
};

/*!
 * \brief GeomObject
 * This class is the base class of all geometrical objects. It cannot be changed directly. The
 * derived classes will take care of that. But the state can be retrieved.
 */
class GeomObject
{
public:
  /*!
   * \brief GeomObject
   * Default constructor
   */
  GeomObject() {}

  /*!
   * \brief state
   * \return the state of the geometrical object.
   */
  GeomState state() const { return _state; }

  /*!
   * \brief isRegular
   * \return true if the geometrical object is in a regular state.
   */
  bool isRegular() const { return _state == GeomState::Regular; }

  /*!
   * \brief isUndefined
   * \return true if the geometrical object is default-constructed or otherwise mal-formed.
   */
  bool isUndefined() const { return _state == GeomState::Undefined; }

  /*!
   * \brief isInfinite
   * \return true if the geometrical object is infinite at some point.
   */
  bool isInfinite() const { return _state == GeomState::Infinite; }

  /*!
   * \brief isNull
   * \return true if the geometrical object is null.
   */
  bool isNull() const { return _state == GeomState::Null; }

protected:
  /*!
   * \brief GeomObject
   * Constructor for derived objects. Here you can set the state of your geometrical object
   * directly.
   * \param state the state of the object.
   */
  GeomObject(GeomState state) : _state(state) {}

  /*!
   * The state of the geometrical object.
   */
  GeomState _state{GeomState::Undefined};
};

/*!
 * \brief Class point.
 * This class is the base class for all objects in N-dimensional space with point-like properties.
 * It holds the component values.
 * There are two template parameters:
 * T, the type for the values to be stored for the point. This must be of arithmetic type.
 * N, the dimension in which the point is located.
 */
template <typename T, size_t N>
class Point : public GeomObject
{
public:
  using value_type = typename detail::Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;
  using iterator = typename std::array<T, N>::iterator;
  using const_iterator = typename std::array<T, N>::const_iterator;

public:
  Point() = default;

  /*!
   * \brief Constructor
   * Creates an object of type Point.
   * \param values a parameter pack. The number of parameters must be equal to the size_t template
   *          parameter.
   */
  template <typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0>
  Point(Args... values) : GeomObject(GeomState::Null)
  {
    setValues(values...);
  }
  Point(Point<T, N> const& other) = default;
  Point(Point<T, N>&& other) = default;
  Point<T, N>& operator=(Point<T, N> const& other) = default;
  Point<T, N>& operator=(Point<T, N>&& other) = default;

  /*!
   * \brief operator ==
   * Comparison operator
   * \param other the point to compare to.
   * \return true if the two points are equal.
   */
  bool operator==(Point<T, N> const& other) const
  {
    return std::equal(_data.begin(), _data.end(), other.begin());
  }

  /*!
   * \brief Accessor
   * Access one of the data elements of the point.
   * Throws if the index exceeds N.
   * \param n the index of the element to access.
   * \return a const reference to the requested element.
   */
  constref_type value(std::size_t n) const { return _data.at(n); }

  /*!
   * \brief Accessor
   * Access one of the data elements of the point.
   * Throws if the index exceeds N.
   * \param n the index of the element to access.
   * \return a reference to the requested element.
   */
  ref_type value(std::size_t n) { return _data.at(n); }

  /*!
   * Access one of the data elements of the point.
   * Throws if the index exceeds N.
   * \param n the index of the element to access.
   * \return a const reference to the requested element.
   */
  constref_type operator[](std::size_t n) const { return _data.at(n); }
  /*!
   * Access one of the data elements of the point.
   * Throws if the index exceeds N.
   * \param n the index of the element to access.
   * \return a reference to the requested element.
   */
  ref_type operator[](std::size_t n) { return _data.at(n); }

  /*!
   * \brief Setter
   * Sets all values of the point simultaneously.
   * \param values parameter pack containing the values. Must be of size N.
   */
  template <typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0>
  void setValues(Args... values)
  {
    _state = GeomState::Null;
    _setValues(0, values...);
  }

  /*!
   * \brief begin
   * Note: this is a non-const iterator. Values can be changed here.
   * \return an iterator to the individual elements.
   */
  iterator begin() { return _data.begin(); }

  /*!
   * \brief begin
   * \return a const-iterator to the individual elements.
   */
  const_iterator begin() const { return _data.begin(); }

  /*!
   * Note: this is a non-const iterator. Values can be changed here.
   * \return an iterator to the individual elements.
   */
  iterator end() { return _data.end(); }

  /*!
   * \return a const-iterator to the individual elements.
   */
  const_iterator end() const { return _data.end(); }

  /*!
   * \brief dimension of the point.
   * \return the dimension of the point.
   */
  constexpr size_t dimension() const { return N; }

  static Point<T, N> origin()
  {
    static Point<T, N> _origin;
    if (_origin.state() == GeomState::Undefined) {
      _origin._data.fill(value_type{0});
    }
    return _origin;
  }

private:
  template <typename... Args>
  void _setValues(size_t I, T t, Args... rest)
  {
    if (t != 0) {
      _state = GeomState::Regular;
    }
    _data[I] = t;
    _setValues(I + 1, rest...);
  }
  void _setValues(size_t I, T t)
  {
    if (t != 0) {
      _state = GeomState::Regular;
    }
    _data[I] = t;
  }

private:
  std::array<T, N> _data;
};

/*!
 * \brief Class Vector
 * This class, derived from the point, defines additional properties, all vectors have in common,
 * like a switch for column to line vector, scalar product or projections.
 */
template <typename T, size_t N>
class Vector : public Point<T, N>, public detail::Vector_base<Vector<T, N>, T, N>
{
public:
  using VectorKind = typename detail::Vector_base<Vector<T, N>, T, N>::VectorKind;

  template <typename... Args>
  Vector(Args... values) : Point<T, N>(values...), detail::Vector_base<Vector<T, N>, T, N>(*this)
  {
  }
  Vector() : detail::Vector_base<Vector<T, N>, T, N>(*this) {}
  Vector(Vector<T, N> const& other)
      : Point<T, N>(other), detail::Vector_base<Vector<T, N>, T, N>(*this)
  {
    this->setKind(other.kind());
  }
  Vector(Vector<T, N>&& other) : Point<T, N>(other), detail::Vector_base<Vector<T, N>, T, N>(*this)
  {
    this->setKind(other.kind());
  }
  Vector<T, N>& operator=(Vector<T, N> const& other)
  {
    Point<T, N>::operator=(other);
    this->setKind(other.kind());
    return *this;
  }
  Vector<T, N>& operator=(Vector<T, N>&& other)
  {
    Point<T, N>::operator=(other);
    this->setKind(other.kind());
    return *this;
  }
  /*!
   * \brief operator ==
   * Comparison operator
   * \param other the vector to compare to.
   * \return true if the two vectors are the same.
   */
  bool operator==(Vector<T, N> const& other) const
  {
    return Point<T, N>::operator==(other) && other.kind() == this->kind();
  }
};

/*!
 * \brief Class Point2D
 * Defines a point in 2 dimensional space with all the specialities a 2D-object can have.
 * Most of the functions defined here are conversion functions to make operations easily available.
 */
template <typename T>
class Point2D : public Point<T, 2>, public detail::Dim2_basic<Point2D<T>, T>
{
public:
  Point2D(T const& v1, T const& v2) : Point<T, 2>(v1, v2), detail::Dim2_basic<Point2D<T>, T>(*this)
  {
    this->update();
  }
  Point2D() : detail::Dim2_basic<Point2D<T>, T>(*this) {}
  Point2D(Point2D<T> const& other) : Point<T, 2>(other), detail::Dim2_basic<Point2D<T>, T>(*this)
  {
    detail::Dim2_basic<Point2D<T>, T>::assign(other);
  }
  Point2D(Point2D<T>&& other) : Point<T, 2>(other), detail::Dim2_basic<Point2D<T>, T>(*this)
  {
    detail::Dim2_basic<Point2D<T>, T>::assign(other);
  }
  Point2D(Point<T, 2> const& other) : Point<T, 2>(other), detail::Dim2_basic<Point2D<T>, T>(*this)
  {
    detail::Dim2_basic<Point2D<T>, T>::update();
  }
  Point2D(Point<T, 2>&& other) : Point<T, 2>(other), detail::Dim2_basic<Point2D<T>, T>(*this)
  {
    detail::Dim2_basic<Point2D<T>, T>::update();
  }
  Point2D<T>& operator=(Point2D<T> const& other)
  {
    Point<T, 2>::operator=(other);
    detail::Dim2_basic<Point2D<T>, T>::assign(other);
    return *this;
  }
  Point2D<T>& operator=(Point2D<T>&& other)
  {
    Point<T, 2>::operator=(other);
    detail::Dim2_basic<Point2D<T>, T>::assign(other);
    return *this;
  }
};

/*!
 * \brief Class Vector2D
 * Defines a vector in 2 dimensional space with all the specialities a 2D-object can have.
 * Most of the functions defined here are conversion functions to make operations easily available.
 */
template <typename T>
class Vector2D : public Vector<T, 2>, public detail::Dim2_basic<Vector2D<T>, T>
{
public:
  Vector2D(T const& v1, T const& v2)
      : Vector<T, 2>(v1, v2), detail::Dim2_basic<Vector2D<T>, T>(*this)
  {
    this->update();
  }
  Vector2D() : Vector<T, 2>(), detail::Dim2_basic<Vector2D<T>, T>(*this) {}
  Vector2D(Vector2D<T> const& other) : detail::Dim2_basic<Vector2D<T>, T>(*this)
  {
    Vector<T, 2>::operator=(other), detail::Dim2_basic<Vector2D<T>, T>::assign(other);
  }
  Vector2D(Vector2D<T>&& other) : Vector<T, 2>(other), detail::Dim2_basic<Vector2D<T>, T>(*this)
  {
    detail::Dim2_basic<Vector2D<T>, T>::assign(other);
  }
  Vector2D(Vector<T, 2> const& other)
      : Vector<T, 2>(other), detail::Dim2_basic<Vector2D<T>, T>(*this)
  {
    detail::Dim2_basic<Vector2D<T>, T>::update();
  }
  Vector2D(Vector<T, 2>&& other) : Vector<T, 2>(other), detail::Dim2_basic<Vector2D<T>, T>(*this)
  {
    detail::Dim2_basic<Vector2D<T>, T>::update();
  }
  Vector2D<T>& operator=(Vector2D<T> const& other)
  {
    Vector<T, 2>::operator=(other);
    detail::Dim2_basic<Vector2D<T>, T>::assign(other);
    return *this;
  }
  Vector2D<T>& operator=(Vector2D<T>&& other)
  {
    Vector<T, 2>::operator=(other);
    detail::Dim2_basic<Vector2D<T>, T>::assign(other);
    return *this;
  }
  Vector2D<T>& operator=(Vector<T, 2> const& other)
  {
    Vector<T, 2>::operator=(other);
    detail::Dim2_basic<Vector2D<T>, T>::update();
    return *this;
  }
  Vector2D<T>& operator=(Vector<T, 2>&& other)
  {
    Vector<T, 2>::operator=(other);
    detail::Dim2_basic<Vector2D<T>, T>::update();
    return *this;
  }
};

/*!
 * \brief Class Point3D
 * Defines a point in 3 dimensional space with all the specialities a 3D-object can have.
 * Most of the functions defined here are conversion functions to make operations easily available.
 */
template <typename T>
class Point3D : public Point<T, 3>, public detail::Dim3_basic<Point3D<T>, T>
{
public:
  Point3D(T const& v1, T const& v2, T const& v3)
      : Point<T, 3>(v1, v2, v3), detail::Dim3_basic<Point3D<T>, T>(*this)
  {
    this->update();
  }
  Point3D() : detail::Dim3_basic<Point3D<T>, T>(*this) {}
  Point3D(Point3D<T> const& other) : Point<T, 3>(other), detail::Dim3_basic<Point3D<T>, T>(*this)
  {
    detail::Dim3_basic<Point3D<T>, T>::assign(other);
  }
  Point3D(Point3D<T>&& other) : Point<T, 3>(other), detail::Dim3_basic<Point3D<T>, T>(*this)
  {
    detail::Dim3_basic<Point3D<T>, T>::assign(other);
  }
  Point3D(Point<T, 3> const& other) : Point<T, 3>(other), detail::Dim3_basic<Point3D<T>, T>(*this)
  {
    detail::Dim3_basic<Point3D<T>, T>::update();
  }
  Point3D(Point<T, 3>&& other) : Point<T, 3>(other), detail::Dim3_basic<Point3D<T>, T>(*this)
  {
    detail::Dim3_basic<Point3D<T>, T>::update();
  }
  Point3D<T>& operator=(Point3D<T> const& other)
  {
    Point<T, 3>::operator=(other);
    detail::Dim3_basic<Point3D<T>, T>::assign(other);
    return *this;
  }
  Point3D<T>& operator=(Point3D<T>&& other)
  {
    Point<T, 3>::operator=(other);
    detail::Dim3_basic<Point3D<T>, T>::assign(other);
    return *this;
  }
  Point3D<T>& operator=(Point<T, 3> const& other)
  {
    Point<T, 3>::operator=(other);
    detail::Dim3_basic<Point3D<T>, T>::update();
    return *this;
  }
  Point3D<T>& operator=(Point<T, 3>&& other)
  {
    Point<T, 3>::operator=(other);
    detail::Dim3_basic<Point3D<T>, T>::update();
    return *this;
  }
};

/*!
 * \brief Class Vector3D
 * Defines a vector in 3 dimensional space with all the specialities a 3D-object can have.
 * Most of the functions defined here are conversion functions to make operations easily available.
 */
template <typename T>
class Vector3D : public Vector<T, 3>, public detail::Dim3_basic<Vector3D<T>, T>
{
public:
  Vector3D(T const& v1, T const& v2, T const& v3)
      : Vector<T, 3>(v1, v2, v3), detail::Dim3_basic<Vector3D<T>, T>(*this)
  {
    this->update();
  }
  Vector3D() : Vector<T, 3>(), detail::Dim3_basic<Vector3D<T>, T>(*this) {}
  Vector3D(Vector3D<T> const& other) : Vector<T, 3>(), detail::Dim3_basic<Vector3D<T>, T>(*this)
  {
    Vector<T, 3>::operator=(other);
    detail::Dim3_basic<Vector3D<T>, T>::assign(other);
  }
  Vector3D(Vector3D<T>&& other) : Vector<T, 3>(other), detail::Dim3_basic<Vector3D<T>, T>(*this)
  {
    detail::Dim3_basic<Vector3D<T>, T>::assign(other);
  }
  Vector3D(Vector<T, 3> const& other)
      : Vector<T, 3>(other), detail::Dim3_basic<Vector3D<T>, T>(*this)
  {
    detail::Dim3_basic<Vector3D<T>, T>::update();
  }
  Vector3D(Vector<T, 3>&& other) : Vector<T, 3>(other), detail::Dim3_basic<Vector3D<T>, T>(*this)
  {
    detail::Dim3_basic<Vector3D<T>, T>::update();
  }
  Vector3D<T>& operator=(Vector3D<T> const& other)
  {
    Vector<T, 3>::operator=(other);
    detail::Dim3_basic<Vector3D<T>, T>::assign(other);
    return *this;
  }
  Vector3D<T>& operator=(Vector3D<T>&& other)
  {
    Vector<T, 3>::operator=(other);
    detail::Dim3_basic<Vector3D<T>, T>::assign(other);
    return *this;
  }
  Vector3D<T>& operator=(Vector<T, 3> const& other)
  {
    Vector<T, 3>::operator=(other);
    detail::Dim3_basic<Vector3D<T>, T>::update();
    return *this;
  }
  Vector3D<T>& operator=(Vector<T, 3>&& other)
  {
    Vector<T, 3>::operator=(other);
    detail::Dim3_basic<Vector3D<T>, T>::update();
    return *this;
  }
  /*!
   * \brief cross
   * Calculates the cross product of two 3D-vectors.
   * result = this x other
   * \param other the right-hand side vector on the cross product.
   * \return the cross product of two vectors.
   */
  Vector3D<T> cross(Vector3D<T> const& other) const
  {
    return {Point<T, 3>::value(1) * other[2] - Point<T, 3>::value(2) * other[1],
            Point<T, 3>::value(2) * other[0] - Point<T, 3>::value(0) * other[2],
            Point<T, 3>::value(0) * other[1] - Point<T, 3>::value(1) * other[0]};
  }
};

/*!
 * \brief Class Point4D
 * This class represents a point in (3+1) dimensional space as it is often used in physics to
 * describe space-time for example.
 * \see Dim3p1_basic for more details.
 */
template <typename T>
class Point4D : public Point<T, 4>, public detail::Dim3p1_basic<Point4D<T>, T>
{
public:
  Point4D(T const& v1, T const& v2, T const& v3)
      : Point<T, 4>(v1, v2, v3), detail::Dim3p1_basic<Point4D<T>, T>(*this)
  {
    this->update();
  }
  Point4D() : detail::Dim3p1_basic<Point4D<T>, T>(*this) {}
  Point4D(Point4D<T> const& other) : Point<T, 4>(other), detail::Dim3p1_basic<Point4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Point4D<T>, T>::assign(other);
  }
  Point4D(Point4D<T>&& other) : Point<T, 4>(other), detail::Dim3p1_basic<Point4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Point4D<T>, T>::assign(other);
  }
  Point4D(Point<T, 4> const& other) : Point<T, 4>(other), detail::Dim3p1_basic<Point4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Point4D<T>, T>::update();
  }
  Point4D(Point<T, 4>&& other) : Point<T, 4>(other), detail::Dim3p1_basic<Point4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Point4D<T>, T>::update();
  }
  Point4D<T>& operator=(Point4D<T> const& other)
  {
    Point<T, 4>::operator=(other);
    detail::Dim3p1_basic<Point4D<T>, T>::assign(other);
    return *this;
  }
  Point4D<T>& operator=(Point4D<T>&& other)
  {
    Point<T, 4>::operator=(other);
    detail::Dim3p1_basic<Point4D<T>, T>::assign(other);
    return *this;
  }
  Point4D<T>& operator=(Point<T, 4> const& other)
  {
    Point<T, 4>::operator=(other);
    detail::Dim3p1_basic<Point4D<T>, T>::update();
    return *this;
  }
  Point4D<T>& operator=(Point<T, 4>&& other)
  {
    Point<T, 4>::operator=(other);
    detail::Dim3p1_basic<Point4D<T>, T>::update();
    return *this;
  }
};

/*!
 * \brief Class Vector4D
 * This class represents a vector in (3+1) dimensional space as it is often used in physics to
 * describe space-time for example.
 * \see Dim3p1_basic for more details.
 */
template <typename T>
class Vector4D : public Vector<T, 4>, public detail::Dim3p1_basic<Vector4D<T>, T>
{
public:
  Vector4D(T const& v1, T const& v2, T const& v3)
      : Vector<T, 4>(v1, v2, v3), detail::Dim3p1_basic<Vector4D<T>, T>(*this)
  {
    this->update();
  }
  Vector4D() : Vector<T, 4>(), detail::Dim3p1_basic<Vector4D<T>, T>(*this) {}
  Vector4D(Vector4D<T> const& other) : Vector<T, 4>(), detail::Dim3p1_basic<Vector4D<T>, T>(*this)
  {
    Vector<T, 4>::operator=(other);
    detail::Dim3p1_basic<Vector4D<T>, T>::assign(other);
  }
  Vector4D(Vector4D<T>&& other) : Vector<T, 4>(other), detail::Dim3p1_basic<Vector4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Vector4D<T>, T>::assign(other);
  }
  Vector4D(Vector<T, 3> const& other)
      : Vector<T, 4>(other), detail::Dim3p1_basic<Vector4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Vector4D<T>, T>::update();
  }
  Vector4D(Vector<T, 3>&& other) : Vector<T, 4>(other), detail::Dim3p1_basic<Vector4D<T>, T>(*this)
  {
    detail::Dim3p1_basic<Vector4D<T>, T>::update();
  }
  Vector4D<T>& operator=(Vector4D<T> const& other)
  {
    Vector<T, 4>::operator=(other);
    detail::Dim3p1_basic<Vector4D<T>, T>::assign(other);
    return *this;
  }
  Vector4D<T>& operator=(Vector4D<T>&& other)
  {
    Vector<T, 4>::operator=(other);
    detail::Dim3p1_basic<Vector4D<T>, T>::assign(other);
    return *this;
  }
  Vector4D<T>& operator=(Vector<T, 4> const& other)
  {
    Vector<T, 4>::operator=(other);
    detail::Dim3p1_basic<Vector4D<T>, T>::update();
    return *this;
  }
  Vector4D<T>& operator=(Vector<T, 4>&& other)
  {
    Vector<T, 4>::operator=(other);
    detail::Dim3p1_basic<Vector4D<T>, T>::update();
    return *this;
  }
};

/*!
 * \brief Class Matrix
 * This class represents a matrix with N lines and M columns.
 * What is still missing is complete implementation of matrix inversion for at least square
 * matrices.
 */
template <typename T, size_t N, size_t M>
class Matrix : public GeomObject
{
public:
  using value_type = typename detail::Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;
  using iterator = typename std::array<std::array<T, M>, N>::iterator;
  using const_iterator = typename std::array<std::array<T, M>, N>::const_iterator;
  using line_iterator = typename std::array<T, M>::iterator;
  using const_line_iterator = typename std::array<T, M>::const_iterator;

public:
  Matrix() = default;

  /*!
   * \brief Constructor
   * Create and initialize a NxM-matrix.
   * \param lines a parameter pack containing N vectors of dimension M.
   */
  template <typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0>
  Matrix(Args... lines) : GeomObject(GeomState::Null)
  {
    setLines(lines...);
  }
  Matrix(Matrix<T, N, M> const& other) = default;
  Matrix(Matrix<T, N, M>&& other) = default;
  Matrix<T, N, M>& operator=(Matrix<T, N, M> const& other) = default;
  Matrix<T, N, M>& operator=(Matrix<T, N, M>&& other) = default;

  /*!
   * \brief operator ==
   * Comparison operator
   * \param other the matrix to compare to
   * \return true if both matrices are identical.
   */
  bool operator==(Matrix<T, N, M> const& other) const
  {
    return std::equal(_data.begin(), _data.end(), other.begin());
  }

  /*!
   * \brief Accessor
   * Return an element from the matrix.
   * \param n the line index of the element.
   * \param m the column index of the element.
   * \return the element at line n and column m.
   */
  constref_type value(std::size_t n, std::size_t m) const { return _data.at(n).at(m); }

  /*!
   * \brief Accessor
   * Return an element from the matrix.
   * Note: This returns a reference, which can be changed outside the class.
   * \param n the line index of the element.
   * \param m the column index of the element.
   * \return the element at line n and column m.
   */
  ref_type value(std::size_t n, std::size_t m) { return _data.at(n).at(m); }

  /*!
   * \brief Accessor
   * Return a line from the matrix.
   * \param n the line index of the element.
   * \return the line n.
   */
  constref_type operator[](std::size_t n) const { return _data.at(n); }

  /*!
   * \brief Accessor
   * Return a line from the matrix.
   * Note: This returns a reference, which can be changed outside the class.
   * \param n the line index of the element.
   * \return the line n.
   */
  ref_type operator[](std::size_t n) { return _data.at(n); }

  /*!
   * \brief Setter
   * Sets all elements of the matrix.
   * \param lines parameter pack of size N containing M-dimensional vectors.
   */
  template <typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0>
  void setLines(Args... lines)
  {
    _state = GeomState::Null;
    _setLines(0, lines...);
  }

  /*!
   * \brief begin
   * Begin iterator to iterate through the lines of the matrix.
   * Note: changes to the elements in this iterator will change the matrix as well.
   * \return the begin iterator for all lines.
   */
  iterator begin() { return _data.begin(); }

  /*!
   * \brief begin
   * Begin iterator to iterate through the lines of the matrix.
   * \return the begin iterator for all lines.
   */
  const_iterator begin() const { return _data.begin(); }

  /*!
   * \brief end
   * End iterator to iterate through the lines of the matrix.
   * Note: changes to the elements in this iterator will change the matrix as well.
   * \return the end iterator for all lines.
   */
  iterator end() { return _data.end(); }

  /*!
   * \brief end
   * End iterator to iterate through the lines of the matrix.
   * \return the end iterator for all lines.
   */
  const_iterator end() const { return _data.end(); }

  /*!
   * \brief begin
   * Begin iterator to iterate through the columns of line n of the matrix.
   * Note: changes to the elements in this iterator will change the matrix as well.
   * \param n the index of the line to iterate.
   * \return the begin iterator line n.
   */
  line_iterator begin(std::size_t n) { return _data[n].begin(); }

  /*!
   * \brief begin
   * Begin iterator to iterate through the columns of line n of the matrix.
   * \param n the index of the line to iterate.
   * \return the begin iterator line n.
   */
  const_line_iterator begin(std::size_t n) const { return _data[n].begin(); }

  /*!
   * \brief end
   * End iterator to iterate through the columns of line n of the matrix.
   * Note: changes to the elements in this iterator will change the matrix as well.
   * \param n the index of the line to iterate.
   * \return the end iterator line n.
   */
  line_iterator end(std::size_t n) { return _data[n].end(); }

  /*!
   * \brief end
   * End iterator to iterate through the columns of line n of the matrix.
   * \param n the index of the line to iterate.
   * \return the end iterator line n.
   */
  const_line_iterator end(std::size_t n) const { return _data[n].end(); }

  /*!
   * \brief columns
   * \return the number of columns in the matrix.
   */
  constexpr std::size_t columns() const { return M; }

  /*!
   * \brief lines
   * \return the number of lines in the matrix.
   */
  constexpr std::size_t lines() const { return N; }

  /*!
   * \brief isSquare
   * \return true if this is a square matrix.
   */
  constexpr bool isSquare() const { return N == M; }

  /*!
   * \brief submatix
   * Calculates and returns a sub-matrix with one line and one column less than the original
   * matrix. This is a helper-function for determinant calculation.
   * \param rowToSkip the row which should not be present in the result.
   * \param columnToSkip the column which should not be present in the result.
   * \return a matrix with one dimension less in both lines and columns.
   */
  Matrix<T, N - 1, M - 1> submatix(size_t rowToSkip, size_t columnToSkip) const
  {
    Matrix<T, N - 1, M - 1> result;
    size_t outL = 0;
    for (size_t i = 0; i < N; i++) {
      if (i == rowToSkip) {
        continue;
      }
      size_t outC = 0;
      for (size_t j = 0; j < M; j++) {
        if (j == columnToSkip) {
          continue;
        }
        result.value(outL, outC) = _data[i][j];
        outC++;
      }
      outL++;
    }
    return result;
  }

  /*!
   * \brief determinant
   * This function calculates and returns the determinant of the square matrix.
   * (Restricted to 2D-space)
   */
  template <typename U = T>
  typename std::enable_if<std::is_same<T, U>::value && N == M && N == 2, value_type>::type determinant()
      const
  {
    return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];
  }

  /*!
   * \brief determinant
   * This function calculates and returns the determinant of the square matrix.
   */
  template <typename U = T>
  typename std::enable_if<std::is_same<T, U>::value && N == M && N != 2, value_type>::type determinant()
      const
  {
    size_t counter = 0;
    return std::accumulate(_data[0].begin(), _data[0].end(), 0,
                           [&counter, this](value_type const& sum, T const& element) {
                             counter++;
                             return sum +
                                    element * submatix(0, counter - 1).determinant() *
                                        (counter % 2 == 0 ? +1 : -1);
                           });
  }

  /*!
   * \brief transpose
   * \return the transposed of the matrix.
   */
  Matrix<T, M, N> transpose() const
  {
    Matrix<T, M, N> result;
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        result.value(j, i) = _data[i][j];
      }
    }
    return result;
  }

  /*!
   * \brief Returns Inverse of matrix
   * For square 2x2 matrices.
   */
  template <typename U = T,
            typename std::enable_if<std::is_same<T, U>::value && N == M && N == 2, int>::type = 0>
  Matrix<T, N, M> invert() const
  {
    Matrix<T, N, M> result;
    auto det = determinant();
    result.value(0, 0) = _data[1][1] / det;
    result.value(0, 1) = _data[0][1] / det;
    result.value(1, 0) = _data[1][0] / det;
    result.value(1, 1) = _data[0][0] / det;
    return result;
  }

  /*!
   * \brief Returns Inverse of matrix
   * For square 3x3 matrices.
   */
  template <typename U = T,
            typename std::enable_if<std::is_same<T, U>::value && N == M && N == 3, int>::type = 0>
  Matrix<T, N, M> invert() const
  {
    Matrix<T, N, M> result;
    auto det = determinant();
    result.value(0, 0) = (_data[1][1] * _data[2][2] - _data[1][2] * _data[2][1]) / det;
    result.value(0, 1) = (_data[0][2] * _data[2][1] - _data[0][1] * _data[2][2]) / det;
    result.value(0, 2) = (_data[0][1] * _data[1][2] - _data[0][2] * _data[1][1]) / det;
    result.value(1, 0) = (_data[1][2] * _data[2][0] - _data[1][0] * _data[2][2]) / det;
    result.value(1, 1) = (_data[0][0] * _data[2][2] - _data[0][2] * _data[2][0]) / det;
    result.value(1, 2) = (_data[0][2] * _data[1][0] - _data[0][0] * _data[1][2]) / det;
    result.value(2, 0) = (_data[1][0] * _data[2][1] - _data[1][1] * _data[2][0]) / det;
    result.value(2, 1) = (_data[0][1] * _data[2][0] - _data[0][0] * _data[2][1]) / det;
    result.value(2, 2) = (_data[0][0] * _data[1][1] - _data[0][1] * _data[1][0]) / det;
    return result;
  }

  /*!
   * \brief Returns Inverse of matrix
   * Is currently only true for square orthogonal matrices.
   * I still have to figure out how to sensibly import the matrix decomposition algorithm here.
   */
  template <typename U = T,
            typename std::enable_if<std::is_same<T, U>::value && N == M && N != 3 && N != 2,
                                    int>::type = 0>
  Matrix<T, N, M> invert() const
  {
    return transpose();
  }

private:
  template <typename... Args>
  void _setLines(size_t I, Vector<T, M> t, Args... rest)
  {
    if (!t.isNull()) {
      _state = GeomState::Regular;
    }
    std::copy(t.begin(), t.end(), _data[I].begin());
    _setLines(I + 1, rest...);
  }
  void _setLines(size_t I, Vector<T, M> t)
  {
    if (!t.isNull()) {
      _state = GeomState::Regular;
    }
    std::copy(t.begin(), t.end(), _data[I].begin());
  }

  template <size_t I, typename std::enable_if<I<N, int>::type = 0> std::array<T, M - 1> _subspace(
                          size_t drop) const
  {
    std::array<T, M - 1> result;
    size_t counter = 0;
    std::copy_if(_data[I].begin(), _data[I].end(), result.begin(),
                 [&counter, drop](T const&) { return counter++ != drop; });
    return result;
  }

private:
  std::array<std::array<T, M>, N> _data;
};

/*!
 * \brief Class Matrix3D
 * This class is meant for rotations in 3-dimensional space.
 */
template <typename T>
class Matrix3D : public Matrix<T, 3, 3>
{
public:
  using Matrix<T, 3, 3>::Matrix;
  Matrix3D(Matrix3D<T> const&) = default;
  constexpr Matrix3D(Matrix3D<T>&&) = default;

  /*!
   * \brief Constructor
   * Creates and initializes a rotation matrix for a rotation around an axis by a specified angle.
   * \param axis the axis to rotate around.
   * \param angle the amount of rotation.
   */
  template <typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
  Matrix3D(Vector3D<T> const& axis, T angle)
  {
    auto l = axis.length();
    auto a = axis[0] / l;
    auto b = axis[1] / l;
    auto c = axis[2] / l;
    auto s = sin(angle);
    auto o = cos(angle);
    this->setLines(
        Vector<T, 3>{o + a * a * (1 - o), -c * s + a * b * (1 - o), b * s + a * c * (1 - o)},
        Vector<T, 3>{c * s + a * b * (1 - o), o + b * b * (1 - o), -a * s + b * c * (1 - o)},
        Vector<T, 3>{-b * s + a * c * (1 - o), a * s + b * c * (1 - o), o + c * c * (1 - o)});
  }
};

/*!
 * \brief Substraction
 * \param to point 1
 * \param from point 2
 * \returns the translation vector from one point to an other.
 */
template <typename T, size_t N>
Vector<T, N> operator-(Point<T, N> to, Point<T, N> const& from)
{
  std::inner_product(to.begin(), to.end(), from.begin(), 0,
                     [&to](size_t index, T value) {
                       to[index] = value;
                       return index + 1;
                     },
                     std::minus<>());
  // use constexpr if to update 2D and 3D
  return to;
}

/*!
 * \brief Vector subtraction
 * \param left vector 1
 * \param right vector 2
 * \return the difference of the two vectors.
 */
template <typename T, size_t N>
Vector<T, N> operator-(Vector<T, N> left, Vector<T, N> const& right)
{
  std::inner_product(left.begin(), left.end(), right.begin(), 0,
                     [&left](size_t index, T value) {
                       left[index] = value;
                       return index + 1;
                     },
                     std::minus<>());
  // use constexpr if to update 2D and 3D
  return left;
}

/*!
 * \brief Translation
 * Moves a point by a translation vector.
 * \param p the point to move.
 * \param shift the translation.
 * \returns the shifted point.
 */
template <typename T, size_t N>
Point<T, N> operator-(Point<T, N> p, Vector<T, N> const& shift)
{
  std::inner_product(p.begin(), p.end(), shift.begin(), 0,
                     [&p](size_t index, T value) {
                       p[index] = value;
                       return index + 1;
                     },
                     std::minus<>());
  // use constexpr if to update 2D and 3D
  return p;
}

/*!
 * \brief Vector addition
 * Adds two vectors.
 * \param left vector 1
 * \param right vector 2
 * \return sum of the two vectors
 */
template <typename T, size_t N>
Vector<T, N> operator+(Vector<T, N> left, Vector<T, N> const& right)
{
  std::inner_product(left.begin(), left.end(), right.begin(), 0,
                     [&left](size_t index, T value) {
                       left[index] = value;
                       return index + 1;
                     },
                     std::plus<>());
  // use constexpr if to update 2D and 3D
  return left;
}

/*!
 * \brief Translation
 * Moves a point by a translation vector.
 * \param p the point to move.
 * \param shift the translation.
 * \returns the shifted point.
 */
template <typename T, size_t N>
Point<T, N> operator+(Point<T, N> p, Vector<T, N> const& shift)
{
  std::inner_product(p.begin(), p.end(), shift.begin(), 0,
                     [&p](size_t index, T value) {
                       p[index] = value;
                       return index + 1;
                     },
                     std::plus<>());
  // use constexpr if to update 2D and 3D
  return p;
}

/*!
 * \brief Multiplication
 * Multiply a constant to a vector
 * \param v the vector
 * \param factor the factor
 * \return the product of the vector multiplication
 */
template <typename T, size_t N>
Vector<T, N> operator*(Vector<T, N> v, T const& factor)
{
  std::for_each(v.begin(), v.end(), [factor](T& value) { value = value * factor; });
  // use constexpr if to update 2D and 3D
  return v;
}

/*!
 * \brief Division
 * Divide a vector by a constant
 * \param v the vector
 * \param factor the divisor
 */
template <typename T, size_t N>
Vector<T, N> operator/(Vector<T, N> v, T const& factor)
{
  std::for_each(v.begin(), v.end(), [factor](T& value) { value = value / factor; });
  // use constexpr if to update 2D and 3D
  return v;
}

/*!
 * \brief Multiplication
 * Multiply a constant to a vector
 * \param factor the factor
 * \param v the vector
 * \return the product of the vector multiplication
 */
template <typename T, size_t N>
Vector<T, N> operator*(T const& factor, Vector<T, N> v)
{
  std::for_each(v.begin(), v.end(), [factor](T& value) { value = value * factor; });
  // use constexpr if to update 2D and 3D
  return v;
}

/*!
 * \brief Scalar product
 * Calculates and returns the scalar product (or inner product) of two vectors.
 * \param v1 first vector
 * \param v2 second vector
 * \return the result of the scalar product of the two vectors.
 */
template <typename T, size_t N>
typename detail::Types<T>::value_type operator*(Vector<T, N> const& v1, Vector<T, N> const& v2)
{
  return std::inner_product(v1.begin(), v1.end(), v2.begin(),
                            typename detail::Types<T>::value_type{0});
}

/*!
 * \brief Cosine of the angle between 2 vectors
 * Calculates the cosine of the angle between two vectors.
 * \param left first vector.
 * \param right second vector.
 * \return the cosine between the two vectors.
 */
template <typename T, size_t N>
typename detail::Types<T>::value_type cosA(Vector<T, N> const& left, Vector<T, N> const& right)
{
  auto length_l = typename detail::Types<T>::value_type{0};
  auto length_r = typename detail::Types<T>::value_type{0};

  auto prod_l = std::inner_product(left.begin(), left.end(), right.begin(),
                                   typename detail::Types<T>::value_type{0}, std::plus<>(),
                                   [&length_l, &length_r](T const& l, T const& r) {
                                     length_l += l * l;
                                     length_r += r * r;
                                     return l * r;
                                   });
  return prod_l / sqrt(length_l) / sqrt(length_r);
}

/*!
 * \brief Cross product
 * For 3-dimensional vectors, this calculates and returns the cross product of two vectors.
 * \param left the left-hand side vector.
 * \param right the right-hand side vector.
 * \return the result of the cross product.
 */
template <typename T, size_t N, typename std::enable_if<N == 3, int>::type = 0>
Vector<T, N> cross(Vector<T, N> const& left, Vector<T, N> const& right)
{
  return {left[1] * right[2] - left[2] * right[1], left[2] * right[0] - left[0] * right[2],
          left[0] * right[1] - left[1] * right[0]};
}

/*!
 * \brief Matrix multiplication
 * Multiply two matrices.
 * \param left the left hand side matrix
 * \param right the right hand side matrix
 * \return the result of the matrix multiplication
 */
template <typename T, size_t N, size_t M, size_t O>
Matrix<T, N, O> operator*(Matrix<T, N, M> const& left, Matrix<T, M, O> const& right)
{
  Matrix<T, N, O> result;
  auto rightT = right.transpose();
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < O; j++) {
      result.value(i, j) = std::inner_product(left.begin(i), left.end(i), rightT.begin(j), 0);
    }
  }
  return result;
}

/*!
 * \brief Matrix addition
 * Add two matrices.
 * \param left the left hand side matrix
 * \param right the right hand side matrix
 * \return the result of the matrix addition
 */
template <typename T, size_t N, size_t M>
Matrix<T, N, M> operator+(Matrix<T, N, M> left, Matrix<T, N, M> const& right)
{
  for (size_t i = 0; i < N; i++) {
    std::inner_product(left.begin(i), left.end(i), right.begin(i), 0,
                       [&left, i](size_t index, T value) {
                         left.value(i, index) = value;
                         return index + 1;
                       },
                       std::plus<>());
  }
  return left;
}

/*!
 * \brief Matrix subtraction
 * Subtract two matrices.
 * \param left the left hand side matrix
 * \param right the right hand side matrix
 * \return the result of the matrix subtraction
 */
template <typename T, size_t N, size_t M>
Matrix<T, N, M> operator-(Matrix<T, N, M> left, Matrix<T, N, M> const& right)
{
  for (size_t i = 0; i < N; i++) {
    std::inner_product(left.begin(i), left.end(i), right.begin(i), 0,
                       [&left, i](size_t index, T value) {
                         left.value(i, index) = value;
                         return index + 1;
                       },
                       std::minus<>());
  }
  return left;
}

/*!
 * \brief Matrix-vector multiplication
 * Multiply a vector to a matrix.
 * \param left the matrix
 * \param right the vector
 * \return the result of the multiplication
 */
template <typename T, size_t N, size_t M>
Vector<T, N> operator*(Matrix<T, N, M> const& left, Vector<T, M> const& right)
{
  Vector<T, N> result;
  for (size_t i = 0; i < N; i++) {
    result.value(i) = std::inner_product(left.begin(i), left.end(i), right.begin(), 0);
  }
  return result;
}

/*!
 * \brief Matrix-vector multiplication
 * Multiply a matrix to a vector.
 * \param left the vector
 * \param right the matrix
 * \return the result of the multiplication
 */
template <typename T, size_t N, size_t M>
Vector<T, N> operator*(Vector<T, M> const& left, Matrix<T, M, N> const& right)
{
  Vector<T, N> result;
  for (size_t i = 0; i < N; i++) {
    typename Matrix<T, N, M>::value_type tmp = 0;
    for (size_t j = 0; j < M; j++) {
      tmp += left[j] * right.value(j, i);
    }
    result.value(i) = tmp;
  }
  return result;
}

/*!
 * \brief Class LimitedLine
 * This class defines a line segment between two points start and stop.
 */
template <typename T, size_t N>
class LimitedLine : public GeomObject
{
public:
  using value_type = typename detail::Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

public:
  LimitedLine() = default;
  LimitedLine(LimitedLine const&) = default;
  LimitedLine(LimitedLine&&) = default;

  /*!
   * \brief LimitedLine
   * Create a line segment between two points start and stop.
   * \param start one of the endpoints of the line segment.
   * \param stop the other endpoint of the line segment.
   */
  LimitedLine(Point<T, N> start, Point<T, N> stop)
      : _start(std::move(start)), _stop(std::move(stop))
  {
    GeomObject::_state = GeomState::Regular;
    if (_start.state() == GeomState::Undefined || _stop.state() == GeomState::Undefined) {
      GeomObject::_state = GeomState::Undefined;
    } else if (_start == _stop) {
      GeomObject::_state = GeomState::Null;
    }
  }

  /*!
   * \brief start
   * Access one of the endpoints of the line segment.
   * \return an endpoint of the line segment.
   */
  Point<T, N> const& start() const { return _start; }

  /*!
   * \brief stop
   * Access one of the endpoints of the line segment.
   * \return an endpoint of the line segment.
   */
  Point<T, N> const& stop() const { return _stop; }

  /*!
   * \brief length
   * Calculates the vector between the two endpoints of the line segment.
   * \return the length vector of the line segment.
   */
  Vector<T, N> length() const { return _stop - _start; }

private:
  Point<T, N> _start;
  Point<T, N> _stop;
};

/*!
 * \brief Class StraightLine
 * This class represents a line. It has a direction and a footing point on the line.
 */
template <typename T, size_t N>
class StraightLine : public GeomObject
{
public:
  using value_type = typename detail::Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

public:
  StraightLine() = default;
  StraightLine(StraightLine const&) = default;
  StraightLine(StraightLine&&) = default;

  /*!
   * \brief StraightLine
   * Constructor. Creates a line with a footing point and a direction.
   * \param foot the footing point of the line.
   * \param direction the direction of the line.
   */
  StraightLine(Point<T, N> foot, Vector<T, N> direction)
      : _foot(std::move(foot)), _direction(std::move(direction))
  {
    GeomObject::_state = GeomState::Regular;
    if (_foot.isUndefined()) {
      GeomObject::_state = GeomState::Undefined;
    } else if (_direction.isNull()) {
      GeomObject::_state = GeomState::Null;
    }
  }

  /*!
   * \brief foot
   * Access the set footing point of the line.
   * \return the footing point of the line.
   */
  Point<T, N> const& foot() const { return _foot; }

  /*!
   * \brief direction
   * Access the direction of the line.
   * \return the direction of the line.
   */
  Vector<T, N> const& direction() const { return _direction; }

private:
  Point<T, N> _foot;
  Vector<T, N> _direction;
};

/*!
 * \brief Class Plane
 * This class represents a plane. Since we do not restrict to three dimensional space the only
 * possible representation is the parametric representation of a footing point and two directions.
 */
template <typename T, size_t N>
class Plane : public GeomObject
{
public:
  using value_type = typename detail::Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

public:
  Plane() = default;
  Plane(Plane const&) = default;
  Plane(Plane&&) = default;

  /*!
   * \brief Plane
   * Constructor. Creates a plane in parametric representation.
   * The state of the plane will be Null, if either of the directions is Null or if they are almost
   * parallel (threshold is 2.5 deg).
   * If the footing point is undefined, the whole plane is undefined.
   * \param foot the footing point of the plane.
   * \param direction1 the first direction of the plane.
   * \param direction2 the second direction of the plane.
   */
  Plane(Point<T, N> foot, Vector<T, N> direction1, Vector<T, N> direction2)
      : _foot(std::move(foot)), _direction1(std::move(direction1)),
        _direction2(std::move(direction2))
  {
    GeomObject::_state = GeomState::Regular;
    if (_foot.isUndefined()) {
      GeomObject::_state = GeomState::Undefined;
    } else if (_direction1.isNull() || _direction2.isNull() ||
               (cosA(_direction1, _direction2) > 0.999)) {
      GeomObject::_state = GeomState::Null;
    }
  }

  Point<T, N> foot() const { return _foot; }
  Vector<T, N> direction1() const { return _direction1; }
  Vector<T, N> direction2() const { return _direction2; }

private:
  Point<T, N> _foot;
  Vector<T, N> _direction1;
  Vector<T, N> _direction2;
};

const constexpr double angle_threshold = 0.999;

/*!
 * \brief distance
 * Calculates the distance between the line segment and a point. The distance vector points to the
 * provided point.
 * \param other the point to calculate the distance to.
 * \return the distance vector between the line segment and the point.
 */
template <typename T, size_t N>
Vector<T, N> distance(LimitedLine<T, N> const& section, Point<T, N> const& other)
{
  auto dir = section.stop() - section.start();
  auto d1 = other - section.start();
  auto para = dir.projectOn(d1);
  auto perp = d1 - para;
  if (para * dir <= 0) {
    return d1;
  }
  if (para.length() > dir.length()) {
    return other - section.stop();
  }
  return perp;
}

/*!
 * \brief distance
 * Calculate the distance of this line to a point.
 * \param other the point to calculate the distance to.
 * \return the distance vector from this line to the provided point.
 */
template <typename T, size_t N>
Vector<T, N> distance(StraightLine<T, N> const& line, Point<T, N> const& other)
{
  auto d1 = other - line.foot();
  auto perp = line.direction().perpendicularToProject(d1);
  return perp;
}

/*!
 * \brief intersection
 * Calculate the intersection of this line with an other line. One point of the returned section is
 * on this line, the stop point of the section is on the other line. The returned section is the
 * section with said definition with the minimal lenght.
 *
 * If the two lines are almost parallel (threshold is 2.5 deg) then an invalid section is returned.
 * The start point of this section will be undefined, in the stop point of the section, there will
 * be the distance between the two lines.
 * \param other the line to calculate the distance to.
 * \return the intersection segment with one point on this line, the other point on the other line.
 */
template <typename T, size_t N>
LimitedLine<T, N> intersection(StraightLine<T, N> const& line, StraightLine<T, N> const& other)
{
  auto r12 = other.foot() - line.foot();
  auto v1 = line.direction() / line.direction().length();
  auto v2 = other.direction() / other.direction().length();
  auto v1v2 = v1 * v2;
  // In case the two lines are almost parallel (2.5deg), let's return an invalid intersection,
  // with the distance as second point.
  if ((v1v2 * v1v2) > angle_threshold * angle_threshold) {
    return {{}, Point<T, N>::origin() + distance(other, line.foot())};
  }
  auto t = ((r12 * v1) - (r12 * v2) * v1v2) / (1 - v1v2 * v1v2);
  auto s = -((r12 * v2) - (r12 * v1) * v1v2) / (1 - v1v2 * v1v2);
  return {(line.foot() + t * v1), (other.foot() + s * v2)};
}

/*!
 * \brief intersection
 * Calculate the intersection of this line with a section. One point of the returned section is on
 * this line, the stop point of the section is on the other line. The returned section is the
 * section with said definition with minimal length.
 *
 * If the section is almost parallel (threshold is 2.5 deg) then an invalid section is returned. The
 * start point of this section will be undefined, in the stop point of the section, there will be
 * the distance between the line and the section.
 * \param other the section to calculate the distance to.
 * \return the intersection segment with one point on this line, the other point on the segment.
 */
template <typename T, size_t N>
LimitedLine<T, N> intersection(StraightLine<T, N> const& line, LimitedLine<T, N> const& other)
{
  auto r12 = other.start() - line.foot();
  auto v1 = line.direction() / line.direction().length();
  auto v2 = other.length() / other.length().length();
  auto v1v2 = v1 * v2;
  // In case the two lines are almost parallel (2.5deg), let's return an invalid intersection,
  // with the distance as second point.
  if ((v1v2 * v1v2) > angle_threshold * angle_threshold) {
    return {{}, Point<T, N>::origin() + distance(line, other.start())};
  }
  auto s = -((r12 * v2) - (r12 * v1) * v1v2) / (1 - v1v2 * v1v2);
  if (s < 0) {
    return {other.start() - distance(line, other.start()), other.start()};
  }
  if (s > other.length().length()) {
    return {other.stop() - distance(line, other.stop()), other.stop()};
  }
  auto t = ((r12 * v1) - (r12 * v2) * v1v2) / (1 - v1v2 * v1v2);
  return {(line.foot() + t * v1), (other.start() + s * v2)};
}

/*!
 * \brief Distance Plane - point
 * Calculates the distance of a plane to a point. The result points into the direction of the point.
 * Note: Since we are using cross product in the calculation, this function is only available in
 * 3D-space.
 * \param plane the plane to calculate the distance to.
 * \param other the point to calculate the distance to.
 * \return the distance between point and plane.
 */
template <typename T, size_t N, typename std::enable_if<N == 3, int>::type = 0>
Vector<T, N> distance(Plane<T, N> const& plane, Point<T, N> const& other)
{
  auto normal = cross(plane.direction1(), plane.direction2());
  normal.normalize();
  return ((other - plane.foot()) * normal) * normal;
}

/*!
 * \brief Intersection between plane and line
 * Calculates the intersection point between a line and a plane.
 * If the angle between the line and the plane is too small (threshold 2.5 deg), an undefined point
 * is returned.
 * \param plane the plane to calculate the intersection with.
 * \param other the line to calculate the intersection with.
 * \return the intersection point.
 */
template <typename T, size_t N, typename std::enable_if<N == 3, int>::type = 0>
Point<T, N> intersection(Plane<T, N> const& plane, StraightLine<T, N> const& other)
{
  auto normal = cross(plane.direction1(), plane.direction2());
  if (detail::Types<T>::abs(cosA(normal, other.direction())) < 1. - angle_threshold) {
    return {};
  }
  auto t = (normal * (other.foot() - plane.foot())) / -(other.direction() * normal);
  return other.foot() + t * other.direction();
}

/*!
 * \brief Intersection between plane and segment
 * Calculates the intersection point between a line segment and a plane.
 * Since the segment has a limited length, it can be that there is no intersection at all. So we
 * return a section with the closest connection between section and plane, where the start point is
 * on the plane and the stop point is on the given segment.
 * If the angle between the segment and the plane is too small (threshold 2.5 deg), an undefined
 * segment is returned.
 * We are using cross product for calculation here, so the function is only available in 3D-space.
 * \param plane the plane to calculate the intersection with.
 * \param other the segment to calculate the intersection with.
 * \return the intersection.
 */
template <typename T, size_t N, typename std::enable_if<N == 3, int>::type = 0>
LimitedLine<T, N> intersection(Plane<T, N> const& plane, LimitedLine<T, N> const& other)
{
  auto normal = cross(plane.direction1(), plane.direction2());
  if (detail::Types<T>::abs(cosA(normal, other.length())) < 1. - angle_threshold) {
    return {};
  }
  auto t = (normal * (other.start() - plane.foot())) / -(other.length() * normal);
  if (t < 0) {
    return {other.start() - distance(plane, other.start()), other.start()};
  }
  if (t > 1) {
    return {other.stop() - distance(plane, other.stop()), other.stop()};
  }
  return {other.start() + t * other.length(), other.start() + t * other.length()};
}

/*!
 * \brief Intersection between two planes
 * Calculates the intersection between two planes.
 * Since cross product is used for calculation, the function is only available in 3D-space.
 * If the two planes are almost parallel (threshold 2.5 deg), an undefined line is returned.
 * \param plane one of the planes to intersect.
 * \param other the other plane to intersect with.
 * \return the line that is on both lines.
 */
template <typename T, size_t N, typename std::enable_if<N == 3, int>::type = 0>
StraightLine<T, N> intersection(Plane<T, N> const& plane, Plane<T, N> const& other)
{
  auto normal1 = cross(plane.direction1(), plane.direction2()).normalized();
  auto normal2 = cross(other.direction1(), other.direction2()).normalized();
  if (detail::Types<T>::abs(cosA(normal1, normal2)) > angle_threshold) {
    return {};
  }
  auto d1 = -(normal1 * geo::Vector<T, N>{plane.foot()});
  auto d2 = -(normal2 * geo::Vector<T, N>{other.foot()});
  auto dir = cross(normal1, normal2);
  auto point = cross(-d1 * normal2 + d2 * normal1, dir) / dir.length() / dir.length();
  return {Point<T, N>{point}, dir};
}

/*!
 * Create a string representation of a point object.
 * \param point the point to create a string representation of.
 * \return a string representation of the point.
 */
template <typename T, size_t N>
std::string toString(Point<T, N> point)
{
  std::stringstream out;
  std::for_each(std::begin(point), std::end(point),
                [&out](auto const& element) { out << element << ", "; });
  auto result = out.str();
  if (!result.empty()) {
    result = result.substr(0, result.length() - 2);
  }
  return "(" + result + ")";
}

/*!
 * Create a string representation of a straight line.
 * \param line the line to create a string representation of.
 * \return a string representation of the line.
 */
template <typename T, size_t N>
std::string toString(StraightLine<T, N> line)
{
  std::stringstream out;
  out << "P = " << toString(line.foot()) << " + t * " << toString(line.direction());
  return "(" + out.str() + ")";
}

/*!
 * Create a string representation of a segment.
 * \param line the line-segment to create a string representation of.
 * \return a string representation of the segment.
 */
template <typename T, size_t N>
std::string toString(LimitedLine<T, N> line)
{
  std::stringstream out;
  out << toString(line.start()) << ", " << toString(line.stop());
  return "(" + out.str() + ")";
}

/*!
 * Create a string representation of a plane.
 * \param plane the plane to create a string representation of.
 * \return a string representation of the plane.
 */
template <typename T, size_t N>
std::string toString(Plane<T, N> plane)
{
  std::stringstream out;
  out << "P = " << toString(plane.foot()) << " + t * " << toString(plane.direction1()) << " + s * "
      << toString(plane.direction2());
  return "(" + out.str() + ")";
}
} // namespace geo
