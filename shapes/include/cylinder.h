#ifndef CYLINDER
#define CYLINDER
#include "volumeShape.h"

/*!
 * \brief The cylinder class
 */
class cylinder : public volumeShape
{
protected:
  /*!
 * \brief direction
 *  Direction from the center point to one of the circle like end shapes.
 */
  vector3D direction;

  /*!
   * \brief radius
   *  Radius of the cylinder.
   */
  float radius;

public:
  /*!
   * \brief cylinder
   *  Constructor. Creates a cylinder with center point c, direction vector d and radius v.
   * \param c
   * \param d
   * \param v
   */
  cylinder(point3D c = point3D(0, 0, 0), vector3D d = vector3D(0, 1, 0), float v = 5);

  /*!
   * \brief ~cylinder
   */
  virtual ~cylinder();

  /*!
   * \brief cylinder
   *  Copy constructor. Works only if the shape s is a cylinder, spiral or wedge.
   * \param s
   */
  cylinder(const volumeShape& s);

  /*!
   * \brief cylinder
   *  Constructor. Creates a cylinder as defined in the parameter.
   * \param description
   */
  cylinder(const shape_parameter& description);

  /*!
   * \brief getDirection
   *  Returns the direction vector from the center to the middle of one of the circle-surfaces.
   * \param num
   * \return
   */
  virtual vector3D getDirection(int num = 0) const;

  /*!
   * \brief getLine
   *  Returns the middle line of the cylinder.
   * \return
   */
  virtual lLine3D getLine() const;

  /*!
   * \brief getRadius
   *  Returns the radius of the cylinder.
   * \return
   */
  float getRadius() const;

  /*!
   * \brief setLine
   *  Sets the middle line of the cylinder.
   * \param l
   */
  void setLine(const lLine3D& l);

  /*!
   * \brief setLine
   *  Sets the two end points of the cylinder.
   * \param p
   * \param q
   */
  void setLine(point3D p, point3D q);

  /*!
   * \brief setLine
   *  Sets the center point and the direction of the middle line of the cylinder.
   * \param c
   * \param d
   */
  void setLine(point3D c, vector3D d);

  /*!
   * \brief setRadius
   *  Sets the radius of the cylinder.
   * \param r
   */
  void setRadius(float r);

  /*!
   * \brief getNext
   *  Returns a new volume shape, that is shifted, rotated, shrunk, enlarged, or else modified
   * according to the parameter stackType, generating the timesth element in the stack. Take care,
   * that you delete this shape correctly after use (potential memory leak).
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getNext(int times, int stackType);

  /*!
   * \brief getEnvelope
   *  Returns a volume that envelopes all n shapes generated with stacking type stackType. Take
   * care, that you delete this shape correctly after use (potential memory leak).
   * \param stackType
   * \return
   */
  virtual volumeShape* getEnvelope(int stackType);

  /*!
   * \brief distance
   *  Returns the distance vector from the straight line to the shape.
   * \param l
   * \return
   */
  virtual vector3D distance(const sLine3D& l);

  /*!
   * \brief Normal
   *  Returns the hit point of a straigt line with the shape and normal vector of the shape in that
   * point. This is stored in the straight line return value with the Foot-property being the hit
   * point and the Direction-property the normal vector.
   * \param line
   * \return
   */
  virtual sLine3D Normal(const sLine3D& line);

  /*!
   * \brief cut
   *  Returns true, if the cylinder and the fiber occupy some space commonly.
   * \param f
   * \return
   */
  virtual bool cut(const fiber& f);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param s
   */
  virtual void operator=(const volumeShape& s);

  /*!
   * \brief getClone
   *  Creates a new instance of the same cylinder with the same properties. Take care, that you
   * delete this shape correctly after use (potential memory leak).
   * \return
   */
  virtual volumeShape* getClone();

  /*!
   * \brief HitParams
   *  Returns the parameters for a hit of a straight line with the shape. The pattern will be: *
index of returndescription 0sigma value 1,2,3entrance point of line/point on line closest to the
volume 4,5,6resolution vector 7,8,9point on volume closest to the line 10,11,12minimum distance of
line to volume, points to volume Returns the parameters for a hit of a straight line with the shape.
The pattern will be:
index of return	description
0	sigma value
1,2,3	entrance point of line/point on line closest to the volume
4,5,6	resolution vector
7,8,9	point on volume closest to the line
10,11,12	minimum distance of line to volume, points to volume
   * \param line
   * \return
   */
  virtual Vector HitParams(const sLine3D& line);

  /*!
   * \brief HitParams
   * Returns the parameters for a hit of a pyramid like shape with the tip at origin and the shape
as base with the shape. The pattern will be:
index of return	description
0	sigma value
1,2,3	entrance point of line/point on line closest to the volume
4,5,6	resolution vector
7,8,9	point on volume closest to the line
10,11,12	minimum distance of line to volume, points to volume
   * \param shape
   * \param origin
   * \return
   */
  virtual Vector HitParams(const planeShape& shape, point3D origin);

  /*!
   * \brief entrance
   * Returns the entrance point of the straight line into the shape. This point is undefined if the
   * line does not hit the shape.
   * \param line
   * \return
   */
  virtual point3D entrance(const sLine3D& line);

  /*!
   * \brief Hitting
   * Calculates all characteristics of a hit of a straight line with the volume shape. The return
values components will be:
index 	value
0, 1, 2 	entrance point of line
3, 4, 5 	distance of line to volume, points to volume
6, 7, 8 	normal vector to volume in entrance point
9 	sigma value
   * \param line
   * \return
   */
  virtual Vector Hitting(const sLine3D& line);

  /*!
   * \brief Draw
   *  This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
   * 3D shape from the eye-point to a plane. The corners of the drawn shape in canvas coordinates
   * are stored in boundingBox. It is drawn using lColor as line color, fColor as fill color and
   * fStyle as fill style. See root-colors and -styles.
   * \param eye
   * \param plane
   * \param boundingBox
   * \param lColor
   * \param fColor
   * \param fStyle
   */
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;

  /*!
   * \brief Draw
   *  This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
   * 3D shape from the eye-point to a plane. The corners of the drawn shape in canvas coordinates
   * are stored in boundingBox. It is drawn using lColor as line color, fColor as fill color and
   * fStyle as fill style. See root-colors and -styles. The parameter ident will return a pointer to
   * the drawn root-primitive to provide the distance-to-primitive functionality.
   * \param eye
   * \param plane
   * \param boundingBox
   * \param ident
   * \param lColor
   * \param fColor
   * \param fStyle
   */
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;

  /*!
   * \brief description
   *  Returns a parameter describing this cylinder.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a parameter with all necessary default variables to create a cylinder.
   * \return
   */
  static shape_parameter getDescription();
};

#endif
