#ifndef SHAPE_QUADRANGLE
#define SHAPE_QUADRANGLE
#include "planeShape.h"

/*!
 * \brief The quadrangle class
 *  This class describes a shape with four corners within a plane without intersections.
 */
class quadrangle : public planeShape
{
private:
  point3D A_, B_, C_, D_;

public:
  /*!
   * \brief quadrangle
   *  Constructor. Creates a quadrangle with center cent and the four points a, b, c and d. The
   * plane is fixed using center, a and b. The other points are projected to this plane.
   * \param cent
   * \param a
   * \param b
   * \param c
   * \param d
   */
  quadrangle(point3D cent = point3D(), point3D a = point3D(), point3D b = point3D(),
             point3D c = point3D(), point3D d = point3D());

  /*!
   * \brief quadrangle
   *  Constructor. Creates a quadrangle as described in the parameter.
   * \param description
   */
  quadrangle(const shape_parameter& description);

  /*!
   * \brief ~quadrangle
   */
  virtual ~quadrangle();

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points which is four.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numth point of the shape.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief area
   *  Returns the area of the shape.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the phi-angular-range viewn from origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the theta-angular range of the shape viewn from origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance of the center of the shape to the plane.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance of the shape to the line.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief A
   *  Returns the first point of the quadrangle.
   * \return
   */
  point3D A() const;

  /*!
   * \brief B
   *  Returns the second point of the quadrangle.
   * \return
   */
  point3D B() const;

  /*!
   * \brief C
   *  Returns the third point of the quadrangle.
   * \return
   */
  point3D C() const;

  /*!
   * \brief D
   *  Returns the fourth point of the quadrangle.
   * \return
   */
  point3D D() const;

  /*!
   * \brief setA
   *  Sets the first point of the quadrangle.
   * \param p
   */
  void setA(point3D p);

  /*!
   * \brief setB
   *  Sets the second point of the quadrangle.
   * \param p
   */
  void setB(point3D p);

  /*!
   * \brief setC
   *  Sets the third point of the quadrangle.
   * \param p
   */
  void setC(point3D p);

  /*!
   * \brief setD
   *  Sets the fourth point of the quadrangle.
   * \param p
   */
  void setD(point3D p);

  /*!
   * \brief description
   *  Returns a description of this quadrangle.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of the quadrangle defining all necessary
   * variables.
   * \return
   */
  static shape_parameter getDescription();
};

#endif
