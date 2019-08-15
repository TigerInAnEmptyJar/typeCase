#pragma once
#include "planeShape.h"

class sphericTriangle;

/*!
 * \brief The triangle class
 */
class triangle : public planeShape
{
protected:
  /*!
 * \brief A_
 * a point of the triangle
 */
  point3D A_, B_, C_;

public:
  /*!
   * \brief triangle
   *  Constructor. Sets the three points the triangle consists of. Center and normal are calculated.
   * \param a
   * \param b
   * \param c
   */
  triangle(point3D a = point3D(), point3D b = point3D(), point3D c = point3D());

  /*!
   * \brief triangle
   *  Constructor. Sets one of the points and the two other points are a + v1 and a + v2. Center and
   * Normal are calculated.
   * \param a
   * \param v1
   * \param v2
   */
  triangle(point3D a, vector3D v1, vector3D v2);

  /*!
   * \brief triangle
   *  Copy constructor. Sets the center and the normal of the triangle. If the s is a triangle or a
   * sphericTriangle, the points are also set.
   * \param s
   */
  triangle(const planeShape& s);

  /*!
   * \brief triangle
   *  Constructor. Creates a triangle acording to the given description.
   * \param description
   */
  triangle(const shape_parameter& description);

  /*!
   * \brief ~triangle
   */
  virtual ~triangle();

  /*!
   * \brief getPoint
   *  Returns the numth point of the triangle.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points of this shape: 3.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief setPoint
   *  Sets the numth point of the triangle. Center and normal are recalculated.
   * \param num
   * \param p
   */
  void setPoint(int num, point3D p);

  /*!
   * \brief setPoints
   *  Sets the points of the triangle. Center and normal are recalculated.
   * \param a
   * \param b
   * \param c
   */
  void setPoints(point3D a, point3D b, point3D c);

  /*!
   * \brief A
   *  Returns the first, the A point of the triangle.
   * \return
   */
  point3D A() const;

  /*!
   * \brief B
   *  Returns the second, the B point of the triangle.
   * \return
   */
  point3D B() const;

  /*!
   * \brief C
   *  Returns the third, the C point of the triangle.
   * \return
   */
  point3D C() const;

  /*!
   * \brief setA
   *  Set the first point of the triangle. Center and normal are recalculated.
   * \param p
   */
  void setA(point3D p);

  /*!
   * \brief setB
   *  Set the second point of the triangle. Center and normal are recalculated.
   * \param p
   */
  void setB(point3D p);

  /*!
   * \brief setC
   *  Set the third point of the triangle. Center and normal are recalculated.
   * \param p
   */
  void setC(point3D p);

  /*!
   * \brief area
   *  Returns the area of the triangle.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of a triangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of the triangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance vector for a plane to the triangle.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance vector from a line to the triangle.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param t
   */
  void operator=(const triangle& t);

  /*!
   * \brief operator =
   *  Copy operator. Copies the first three points from a spheric triangle.
   * \param t
   */
  void operator=(const sphericTriangle& t);

  /*!
   * \brief description
   *  Returns a description of this triangle.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of a triangle defining the necessary variables in
   * the parameter.
   * \return
   */
  static shape_parameter getDescription();
};

/*!
 * \brief The sphericTriangle class
 */
class sphericTriangle : public planeShape
{
protected:
  /*!
 * \brief realTriangle
 *  true if points A, B, and C are used, false if the others are used too
 */
  bool realTriangle;

  /*!
   * \brief A_
   *  point of the triangle
   */
  point3D A_, B_, C_, D_, E_, F_;

public:
  /*!
   * \brief sphericTriangle
   *  Constructor. Sets the corners of the triangle. If d, e and f are undefined, a real triangle
   * remains.
   * \param cen
   * \param a
   * \param b
   * \param c
   * \param d
   * \param e
   * \param f
   */
  sphericTriangle(point3D cen, point3D a = point3D(), point3D b = point3D(), point3D c = point3D(),
                  point3D d = point3D(), point3D e = point3D(), point3D f = point3D());

  /*!
   * \brief sphericTriangle
   *  Copy constructor. Copies the points. Works for s being a triangle or a sphericTriangle.
   * \param s
   */
  sphericTriangle(const planeShape& s);

  /*!
   * \brief sphericTriangle
   *  Constructor. Creates a spheric triangle a specified in the description.
   * \param description
   */
  sphericTriangle(const shape_parameter& description);

  /*!
   * \brief ~sphericTriangle
   */
  virtual ~sphericTriangle();

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points for this spheric triangle, can be 3 or 6.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numthpoint of the triangle.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief setPoint
   *  Sets the points of the spheric triangle. If the points d, e and f are undefined (as by
   * default) a real triangle remains.
   * \param num
   * \param p
   */
  void setPoint(int num, point3D p);

  /*!
   * \brief setPoints
   *
   * \param cen
   * \param a
   * \param b
   * \param c
   * \param d
   * \param e
   * \param f
   */
  void setPoints(point3D cen, point3D a, point3D b, point3D c, point3D d = point3D(),
                 point3D e = point3D(), point3D f = point3D());

  /*!
   * \brief A
   *  Returns the A point of the spheric triangle.
   * \return
   */
  point3D A() const;

  /*!
   * \brief B
   *  Returns the B point of the spheric triangle.
   * \return
   */
  point3D B() const;

  /*!
   * \brief C
   *  Returns the C point of the spheric triangle.
   * \return
   */
  point3D C() const;

  /*!
   * \brief D
   *  Returns the D point of the spheric triangle.
   * \return
   */
  point3D D() const;

  /*!
   * \brief E
   *  Returns the E point of the spheric triangle.
   * \return
   */
  point3D E() const;

  /*!
   * \brief F
   *  Returns the F point of the spheric triangle.
   * \return
   */
  point3D F() const;

  /*!
   * \brief setA
   *  Sets the A point of the spheric triangle.
   * \param p
   */
  void setA(point3D p);

  /*!
   * \brief setB
   *  Sets the B point of the spheric triangle.
   * \param p
   */
  void setB(point3D p);

  /*!
   * \brief setC
   *  Sets the C point of the spheric triangle.
   * \param p
   */
  void setC(point3D p);

  /*!
   * \brief setD
   *  Sets the D point of the spheric triangle.
   * \param p
   */
  void setD(point3D p);

  /*!
   * \brief setE
   *  Sets the E point of the spheric triangle.
   * \param p
   */
  void setE(point3D p);

  /*!
   * \brief setF
   *  Sets the F point of the spheric triangle.
   * \param p
   */
  void setF(point3D p);

  /*!
   * \brief area
   *  Returns the area of the spheric triangle.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of the spheric triangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of the spheric triangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance vector for a plane to the shperic triangle.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance from a line to the speric triangle.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief isRealTriangle
   *  Returns true if the points D, E and F are undefined.
   * \return
   */
  bool isRealTriangle();

  /*!
   * \brief getTriangle
   *  Returns the first three points of the spheric triangle as a "real" triangle.
   * \return
   */
  triangle getTriangle() const;

  /*!
   * \brief operator =
   *  Copy operator.
   * \param t
   */
  void operator=(const sphericTriangle& t);

  /*!
   * \brief description
   *  Returns a description of this spheric triangle.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of the spheric triangle with all necessary
   * variables defined.
   * \return
   */
  static shape_parameter getDescription();
};

/*!
 * \brief The rectangle class
 */
class rectangle : public planeShape
{
protected:
  /*!
 * \brief left
 *  the other edge of the rectangle
 */
  lLine3D left;

  /*!
   * \brief right
   *  one of the edges of the rectangle
   */
  lLine3D right;

public:
  /*!
   * \brief rectangle
   *  Constructor. Creates a rectangle with points A and B. Together with the normal vector norm
   * this defines a plane. The other two points are calculates using the rectangle restriction (two
   * opposing edges have the same length, the angle between to edges is PI/2) and the width
   * parameter.
   * \param A
   * \param B
   * \param norm
   * \param width
   */
  rectangle(point3D A = point3D(), point3D B = point3D(), vector3D norm = vector3D(),
            float width = 0);

  /*!
   * \brief rectangle
   *  Constructor. Creates a rectangle with center Ce, normal N and the points A, B, C and D.
   * \param Ce
   * \param N
   * \param A
   * \param B
   * \param C
   * \param D
   */
  rectangle(point3D Ce, vector3D N, point3D A = point3D(), point3D B = point3D(),
            point3D C = point3D(), point3D D = point3D());

  /*!
   * \brief rectangle
   *  Copy constructor. Copies center and normal. If s is a rectangle, also the points are copied.
   * \param s
   */
  rectangle(const planeShape& s);

  /*!
   * \brief rectangle
   *  Constructor. Creates a rectangle out of the values of the parameter.
   * \param description
   */
  rectangle(const shape_parameter& description);

  /*!
   * \brief ~rectangle
   */
  virtual ~rectangle();

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points of the rectangle: 4.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numth point of the rectangle.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief A
   *  Return point A of the rectangle.
   * \return
   */
  point3D A() const;

  /*!
   * \brief B
   *  Return point B of the rectangle.
   * \return
   */
  point3D B() const;

  /*!
   * \brief C
   *  Return point C of the rectangle.
   * \return
   */
  point3D C() const;

  /*!
   * \brief D
   *  Return point D of the rectangle.
   * \return
   */
  point3D D() const;

  /*!
   * \brief area
   *  Returns the area of the rectangle.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief getWidth
   *  Returns the width, the distance between points B and C.
   * \return
   */
  float getWidth() const; // distance AD

  /*!
   * \brief getLength
   *  Returns the length, the distance between points A and B.
   * \return
   */
  float getLength() const; // distance AB

  /*!
   * \brief setPoints
   *  Set the points A and B. Together with the normal vector norm, the width and the rectangle
   * restriction(two opposing edges have the same length, the angle between to edges is PI/2), the
   * other points are calculated.
   * \param A
   * \param B
   * \param norm
   * \param width
   */
  void setPoints(point3D A, point3D B, vector3D norm, float width);

  /*!
   * \brief AB
   *  Returns the edge between the points A and B.
   * \return
   */
  lLine3D AB() const;

  /*!
   * \brief BC
   *  Returns the edge between the points B and C.
   * \return
   */
  lLine3D BC() const;

  /*!
   * \brief CD
   *  Returns the edge between the points C and D.
   * \return
   */
  lLine3D CD() const;

  /*!
   * \brief DA
   *  Returns the edge between the points D and A.
   * \return
   */
  lLine3D DA() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of a rectangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of a rectangle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance vector for a plane to the rectangle.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance vector from a line to the rectangle
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param r
   */
  void operator=(const rectangle& r);

  /*!
   * \brief description
   *  Returns a description of this rectangle.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of a rectangle with all necessary variables
   * defined.
   * \return
   */
  static shape_parameter getDescription();
};

/*!
 * \brief The hexagon class
 */
class hexagon : public planeShape
{
protected:
  /*!
 * \brief A_
 *  one of the corners of the hexagon
 */
  point3D A_;

public:
  /*!
   * \brief hexagon
   *  Constructor. Sets the points, the center and the normal.
   * \param cen
   * \param a
   * \param norm
   */
  hexagon(point3D cen = point3D(), point3D a = point3D(), vector3D norm = point3D());

  /*!
   * \brief hexagon
   *  Copy constructor. Copies center and normal. If s is a hexagon it also copies the point.
   * \param s
   */
  hexagon(const planeShape& s);

  /*!
   * \brief hexagon
   *  Constructor. Creates a hexagon out of the values in the parameter.
   * \param description
   */
  hexagon(const shape_parameter& description);

  /*!
   * \brief ~hexagon
   */
  virtual ~hexagon();

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points of the hexagon: 6.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numth point of the hexagon.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief setA
   *  Sets the point A. The other points are generated by rotation of the point A around the axis
   * center-norm by PI/3.
   * \param p
   */
  void setA(point3D p);

  /*!
   * \brief setPoints
   *  Sets the points of the hexagon. The other points are generated by rotation of the point a
   * around the axis center-norm by PI/3.
   * \param cen
   * \param a
   * \param norm
   */
  void setPoints(point3D cen, point3D a, vector3D norm);

  /*!
   * \brief A
   *  Returns point A of the hexagon.
   * \return
   */
  point3D A() const;

  /*!
   * \brief B
   *  Returns point B of the hexagon.
   * \return
   */
  point3D B() const;

  /*!
   * \brief C
   *  Returns point C of the hexagon.
   * \return
   */
  point3D C() const;

  /*!
   * \brief D
   *  Returns point D of the hexagon.
   * \return
   */
  point3D D() const;

  /*!
   * \brief E
   *  Returns point E of the hexagon.
   * \return
   */
  point3D E() const;

  /*!
   * \brief F
   *  Returns point F of the hexagon.
   * \return
   */
  point3D F() const;

  /*!
   * \brief area
   *  Returns the area of the hexagon.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of a hexagon, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of a hexagon, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance vector for a plane to the hexagon.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance from a line to the hexagon.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief AB
   *  Returns the edge between point A and B.
   * \return
   */
  lLine3D AB() const;

  /*!
   * \brief BC
   *  Returns the edge between point B and C.
   * \return
   */
  lLine3D BC() const;

  /*!
   * \brief CD
   *  Returns the edge between point C and D.
   * \return
   */
  lLine3D CD() const;

  /*!
   * \brief DE
   *  Returns the edge between point D and E.
   * \return
   */
  lLine3D DE() const;

  /*!
   * \brief EF
   *  Returns the edge between point E and F.
   * \return
   */
  lLine3D EF() const;

  /*!
   * \brief FA
   *  Returns the edge between point F and A.
   * \return
   */
  lLine3D FA() const;

  /*!
   * \brief operator =
   *  Copy operator.
   * \param h
   */
  void operator=(const hexagon& h);

  /*!
   * \brief description
   *  Returns a description of this hexagon.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static Method. Returns a default description of a hexagon with all necessary variables
   * defined.
   * \return
   */
  static shape_parameter getDescription();
};
