#include <algorithm>
#include <cmath>
#include <numeric>
#include <type_traits>

namespace geo {
namespace detail {

/*!
 * \brief Basic struct to define used types.
 */
template <typename T>
struct Types
{
  using value_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

  static value_type abs(constref_type v)
  {
    if (v < 0) {
      return -v;
    }
    return v;
  }
};

/*!
 * \brief Class Vector_base
 * This class contains all a vector needs. In the current implementation it could be easily
 * integrated into the Vector class.
 */
template <typename Object_t, typename T, size_t N>
class Vector_base
{
  using value_type = typename Types<T>::value_type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

public:
  /*!
   * \brief The VectorKind enum
   * Defines the two states a vector can have.
   */
  enum class VectorKind
  {
    /// a line-vector,
    Line,
    /// a column-vector.
    Column,
  };

  Vector_base(Object_t& object) : _object(object) {}
  Vector_base(Vector_base const& other) = delete;
  Vector_base(Vector_base&& other) = delete;
  Vector_base& operator=(Vector_base const& other) = delete;
  Vector_base& operator=(Vector_base&& other) = delete;

  /*!
   * \brief scalar_product
   * Do a scalar product of two vectors.
   * \param other the second vector to do the product with.
   * \return the scalar product of the two provided vectors.
   */
  Vector_base::value_type scalar_product(Object_t const& other) const
  {
    return std::inner_product(_object.begin(), _object.end(), other.begin(), value_type{0});
  }

  /*!
   * \brief length
   * \return the length of the vector.
   */
  Vector_base::value_type length() const
  {
    return sqrt(std::inner_product(_object.begin(), _object.end(), _object.begin(), value_type{0}));
  }

  /*!
   * \brief normalize
   * Normalize the vector. It's length will them be 1.
   */
  void normalize()
  {
    auto l = length();
    std::for_each(_object.begin(), _object.end(), [&l](auto& element) { element = element / l; });
  }

  /*!
   * \brief normalized
   * \return a normalized copy of this vector.
   */
  Object_t normalized() const
  {
    auto copy = _object;
    copy.normalize();
    return copy;
  }

  /*!
   * \brief kind
   * \return whether we have a line or column-vector.
   */
  VectorKind kind() const { return _kind; }

  /*!
   * \brief isLine
   * \return true if this vector is a line-vector.
   */
  bool isLine() const { return _kind == VectorKind::Line; }

  /*!
   * \brief isColumn
   * \return true if this vector is a column-vector.
   */
  bool isColumn() const { return _kind == VectorKind::Column; }

  /*!
   * \brief transpose
   * Transposes this vector. Switches from line-vector to column-vector and back.
   */
  void transpose() { _kind = (_kind == VectorKind::Line ? VectorKind::Column : VectorKind::Line); }

  /*!
   * \brief transposed
   * \return a transposed copy of this vector.
   */
  Object_t transposed() const
  {
    auto copy = _object;
    copy.transpose();
    return copy;
  }

  /*!
   * \brief projectOn
   * Do a projection of the provided vector on this vector.
   * The result will point in the direction of this vector, the length will be length of the other
   * vector multiplied by cosine of the angle between the two vectors.
   * \param other the vector to project.
   * \return the projected vector.
   */
  Object_t projectOn(Object_t other) const
  {
    auto inner = std::inner_product(_object.begin(), _object.end(), other.begin(), value_type{0});
    inner = inner / (length() * length());
    return (_object * inner);
  }

  /*!
   * \brief perpendicularToProject
   * This returns the component of the provided vector that is perpendicular to this vector.
   * \param other the vector to project.
   * \return the perpendicular vector.
   */
  Object_t perpendicularToProject(Object_t other) const { return other - projectOn(other); }

protected:
  /*!
   * \brief setKind
   * Sets whether this is a line or column-vector.
   * \param kind the type of vector.
   */
  void setKind(VectorKind kind) { _kind = kind; }

  /*!
   * \brief assign
   * Assigns the component that is vector-type.
   * \param other vector to assign.
   */
  void assign(Vector_base<Object_t, T, N> const& other) { _kind = other.kind(); }
private:
  Object_t& _object;
  VectorKind _kind{VectorKind::Line};
};

/*!
 * \brief Class Dim2_basic
 * This class contains convenience functions and helpers specific to 2D-space.
 * This means here a different representation from cartesic.
 */
template <typename Object_t, typename T>
class Dim2_basic
{
public:
  using value_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

  Dim2_basic(Object_t& object) : _object(object) {}

  /*!
   * \brief phi
   * Note: this is a reference and can be changed.
   * \return the phi component in polar coordinates of the vector.
   */
  ref_type phi() { return _phi; }

  /*!
   * \brief phi
   * \return the phi component in polar coordinates of the vector.
   */
  constref_type phi() const { return _phi; }

  /*!
   * \brief r
   * Note: this is a reference and can be changed.
   * \return the length of the vector, aka the r component in polar coordinates.
   */
  ref_type r() { return _r; }

  /*!
   * \brief r
   * \return the length of the vector, aka the r component in polar coordinates.
   */
  constref_type r() const { return _r; }

  /*!
   * \brief update
   * This calculates the polar coordinates.
   */
  void update()
  {
    _r = sqrt(_object[0] * _object[0] + _object[1] * _object[1]);
    if (_r != 0) {
      _phi = acos(_object[0] / _r);
      if (_object[1] < 0) {
        _phi = -_phi;
      }
    } else {
      _phi = 0;
    }
  }

protected:
  /*!
   * \brief assign
   * Copy 2D specific properties.
   * \param other the object to copy the 2D specific properties from.
   */
  void assign(Dim2_basic<Object_t, T> const& other)
  {
    _phi = other.phi();
    _r = other.r();
  }

  Object_t& _object;
  value_type _phi;
  value_type _r;
};

/*!
 * \brief Class Dim3_basic
 * This class contains convenience functions and helpers specific to 3D-space.
 * This means here different representations from cartesic.
 */
template <typename Object_t, typename T>
class Dim3_basic
{
public:
  using value_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

  Dim3_basic(Object_t& object) : _object(object) {}

  /*!
   * \brief phi
   * This is the phi coordinate in cylindric or spheric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the phi coordinate
   */
  ref_type phi() { return _phi; }

  /*!
   * \brief phi
   * This is the phi coordinate in cylindric or spheric coordinates.
   * \return the phi coordinate
   */
  constref_type phi() const { return _phi; }

  /*!
   * \brief theta
   * This is the theta coordinate in spheric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the theta coordinate
   */
  ref_type theta() { return _theta; }

  /*!
   * \brief theta
   * This is the theta coordinate in spheric coordinates.
   * \return the theta coordinate
   */
  constref_type theta() const { return _theta; }

  /*!
   * \brief r
   * This is the r coordinate in spheric coordinates or length.
   * Note: this returns a reference, which can be change outside the class.
   * \return the r coordinate
   */
  ref_type r() { return _r; }

  /*!
   * \brief r
   * This is the r coordinate in spheric coordinates or length.
   * \return the r coordinate
   */
  constref_type r() const { return _r; }

  /*!
   * \brief rho
   * This is the rho coordinate in cylindric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the rho coordinate
   */
  ref_type rho() { return _rho; }

  /*!
   * \brief rho
   * This is the rho coordinate in cylindric coordinates.
   * \return the rho coordinate
   */
  constref_type rho() const { return _rho; }

  /*!
   * \brief update
   * Calculates the coordinates that are different from cartesic.
   */
  void update()
  {
    _rho = sqrt(_object[0] * _object[0] + _object[1] * _object[1]);
    _r = sqrt(_rho * _rho + _object[2] * _object[2]);
    if (_rho != 0) {
      _phi = acos(_object[0] / _rho);
      if (_object[1] < 0) {
        _phi = -_phi;
      }
    } else {
      _phi = 0;
    }
    if (_r != 0) {
      _theta = acos(_object[2] / _r);
    } else {
      _theta = 0;
    }
  }

protected:
  /*!
   * \brief assign
   * Assign the 3D-specific properties from an other 3D object.
   * \param other the 3D object to copy from.
   */
  void assign(Dim3_basic<Object_t, T> const& other)
  {
    _phi = other.phi();
    _theta = other.theta();
    _r = other.r();
    _rho = other.rho();
  }

  Object_t& _object;
  value_type _phi;
  value_type _theta;
  value_type _r;
  value_type _rho;
};

/*!
 * \brief Class Dim3p1_basic
 * This class contains convenience functions and helpers specific to (3+1)D-space.
 * 3+1 dimensional space is often used in physics to describe space-time or momentum-space.
 * Most of the properties here are specific to 3 dimensional space (the spacial component).
 * The object to inherrit from this must be a 4D-Object.
 */
template <typename Object_t, typename T>
class Dim3p1_basic
{
public:
  using value_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
  using ref_type = value_type&;
  using constref_type = value_type const&;

  Dim3p1_basic(Object_t& object) : _object(object) {}

  /*!
   * \brief phi
   * This is the phi coordinate in cylindric or spheric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the phi coordinate
   */
  ref_type phi() { return _phi; }

  /*!
   * \brief phi
   * This is the phi coordinate in cylindric or spheric coordinates.
   * \return the phi coordinate
   */
  constref_type phi() const { return _phi; }

  /*!
   * \brief theta
   * This is the theta coordinate in spheric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the theta coordinate
   */
  ref_type theta() { return _theta; }

  /*!
   * \brief theta
   * This is the theta coordinate in spheric coordinates.
   * \return the theta coordinate
   */
  constref_type theta() const { return _theta; }

  /*!
   * \brief r
   * This is the r coordinate in spheric coordinates or length of the 3 dimensional part of the
   * object.
   * Note: this returns a reference, which can be change outside the class.
   * \return the r coordinate
   */
  ref_type r() { return _r; }

  /*!
   * \brief r
   * This is the r coordinate in spheric coordinates or length of the 3 dimensional part of the
   * object.
   * \return the r coordinate
   */
  constref_type r() const { return _r; }

  /*!
   * \brief rho
   * This is the rho coordinate in cylindric coordinates.
   * Note: this returns a reference, which can be change outside the class.
   * \return the rho coordinate
   */
  ref_type rho() { return _rho; }

  /*!
   * \brief rho
   * This is the rho coordinate in cylindric coordinates.
   * \return the rho coordinate
   */
  constref_type rho() const { return _rho; }

  /*!
   * \brief w
   * This is the w coordinate, the time-like component of the (3+1)D-object.
   * Note: this returns a reference, which can be change outside the class.
   * \return the w coordinate
   */
  ref_type w() { return _object[3]; }

  /*!
   * \brief w
   * This is the w coordinate, the time-like component of the (3+1)D-object.
   * \return the w coordinate
   */
  constref_type w() const { return _object[3]; }

  /*!
   * \brief update
   * Calculates all properties specific to (3+1) dimensional space.
   */
  void update()
  {
    _rho = sqrt(_object[0] * _object[0] + _object[1] * _object[1]);
    _r = sqrt(_rho * _rho + _object[2] * _object[2]);
    if (_rho != 0) {
      _phi = acos(_object[0] / _rho);
      if (_object[1] < 0) {
        _phi = -_phi;
      }
    } else {
      _phi = 0;
    }
    if (_r != 0) {
      _theta = acos(_object[2] / _r);
    } else {
      _theta = 0;
    }
  }

protected:
  /*!
   * \brief assign
   * Copies the (3+1)D specific properties from an other object.
   * \param other the object to copy.
   */
  void assign(Dim3p1_basic<Object_t, T> const& other)
  {
    _phi = other.phi();
    _theta = other.theta();
    _r = other.r();
    _rho = other.rho();
  }

  Object_t& _object;
  value_type _phi;
  value_type _theta;
  value_type _r;
  value_type _rho;
};

} // namespace detail
} // namespace geo
