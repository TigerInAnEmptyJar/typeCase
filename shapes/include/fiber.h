#ifndef FIBER
#define FIBER
#include "volumeShape.h"

/*!
 * \brief The fiber class
 */
class fiber : public volumeShape
{
protected:
  /*!
 * \brief corner
 *  The corner of the fiber.
 */
  point3D corner;

  /*!
   * \brief dir1
   *  One of the three direction vectors a fiber defines.
   */
  vector3D dir1;

  /*!
   * \brief dir2
   *  One of the three direction vectors a fiber defines.
   */
  vector3D dir2;

  /*!
   * \brief dir3
   *  One of the three direction vectors a fiber defines.
   */
  vector3D dir3;

  /*!
   * \brief numberOfHalvedElements
   *  The properties halved and halved at are for stacking purposes and define in a stack of n
   * fibers the number of fibers that are half the size of the other fibers (a second fiber is then
   * filling up the space) and the element number when the halved elements first occur.
   */
  int numberOfHalvedElements;

  /*!
   * \brief halvedAt
   *  The properties halved and halved at are for stacking purposes and define in a stack of n
   * fibers the number of fibers that are half the size of the other fibers (a second fiber is then
   * filling up the space) and the element number when the halved elements first occur.
   */
  int halvedAt;

public:
  /*!
   * \brief fiber
   *  Constructor. Sets a fiber. cor is one of the corner points and with the three vectors d1, d2
   * and d3 it defines a volume with any two opposing areas being parallel. The properties halved
   * and halved at are for stacking purposes and define in a stack of n fibers the number of fibers
   * that are half the size of the other fibers (a second fiber is then filling up the space) and
   * the element number when the halved elements first occur.
   * \param cor
   * \param d1
   * \param d2
   * \param d3
   * \param halved
   * \param halvedat
   */
  fiber(point3D cor = point3D(0, 0, 0), vector3D d1 = vector3D(1, 0, 0),
        vector3D d2 = vector3D(0, 1, 0), vector3D d3 = vector3D(0, 0, 1), int halved = 0,
        int halvedat = 0);

  /*!
   * \brief fiber
   *  Copy constructor. Works only if the shape is a fiber.
   * \param s
   */
  fiber(const volumeShape& s);

  /*!
   * \brief fiber
   *  Copy constructor.
   * \param s
   */
  fiber(const fiber& s);

  /*!
   * \brief fiber
   *  Constructor. Creates a fiber with the values from the parameter.
   * \param description
   */
  fiber(const shape_parameter& description);

  /*!
   * \brief getCorner
   *  Returns the corner of the fiber.
   * \return
   */
  virtual point3D getCorner() const;

  /*!
   * \brief getDirection
   *  Returns the numth direction vector of the fiber. Returns an undefined vector if num is non
   * valid.
   * \param num
   * \return
   */
  virtual vector3D getDirection(int num) const;

  /*!
   * \brief setCorner
   *  Sets the corner of the fiber.
   * \param p
   */
  void setCorner(point3D p);

  /*!
   * \brief setDirection
   *  Sets the numth direction vector. Does nothing if num is non valid.
   * \param num
   * \param v
   */
  void setDirection(int num, vector3D v);

  /*!
   * \brief getHalved
   *  Returns the halved property. The properties halved and halved at are for stacking purposes and
   * define in a stack of n fibers the number of fibers that are half the size of the other fibers
   * (a second fiber is then filling up the space) and the element number when the halved elements
   * first occur.
   * \return
   */
  int getHalved() const;

  /*!
   * \brief setHalved
   *  Sets the halved property. The properties halved and halved at are for stacking purposes and
   * define in a stack of n fibers the number of fibers that are half the size of the other fibers
   * (a second fiber is then filling up the space) and the element number when the halved elements
   * first occur.
   * \param num
   */
  void setHalved(int num);

  /*!
   * \brief getHalvedAt
   *  Returns the halved at property. The properties halved and halved at are for stacking purposes
   * and define in a stack of n fibers the number of fibers that are half the size of the other
   * fibers (a second fiber is then filling up the space) and the element number when the halved
   * elements first occur.
   * \return
   */
  int getHalvedAt() const;

  /*!
   * \brief setHalvedAt
   *  Sets the halved at property. The properties halved and halved at are for stacking purposes and
   * define in a stack of n fibers the number of fibers that are half the size of the other fibers
   * (a second fiber is then filling up the space) and the element number when the halved elements
   * first occur.
   * \param num
   */
  void setHalvedAt(int num);

  /*!
   * \brief getNext
   * Returns a new volume shape, that is shifted, rotated, shrunk, enlarged, or else modified
according to the parameter stackType, generating the timesth element in the stack. Take care, that
you delete this shape correctly after use (potential memory leak).
These stack types have been implemented:

rectangular stacking(2)	extended rectangular stacking (17)
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getNext(int times, int stackType);

  /*!
   * \brief getEnvelope
   * Returns a volume shape, that will be the envelope of times shapes, generated with the
getNext-method in stack mode stackType. Take care, that you delete this shape correctly after use
(potential memory leak).
These stack types have been implemented:

rectangular stacking(2)	extended rectangular stacking (17)
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getEnvelope(int times, int stackType);

  /*!
   * \brief distance
   *  Returns the distance vector from the straight line to the shape.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

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
   * \brief cut
   *  Returns true, if the shape and the fiber occupy some space commonly.
   * \param f
   * \return
   */
  virtual bool cut(const fiber& f);

  /*!
   * \brief operator =
   *  Copy operator. Only works if the shape is a fiber.
   * \param s
   */
  virtual void operator=(const volumeShape& s);

  /*!
   * \brief getClone
   *  Creates a new instance of the same volume shape with the same properties. Take care, that you
   * delete this shape correctly after use (potential memory leak).
   * \return
   */
  virtual volumeShape* getClone();

  /*!
   * \brief HitParams
   * Returns the parameters for a hit of a straight line with the shape. The pattern will be:
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
   *  Returns the entrance point of the straight line into the shape. This point is undefined if the
   * line does not hit the shape.
   * \param line
   * \return
   */
  virtual point3D entrance(const sLine3D& line);

  /*!
   * \brief getFlightPathInShape
   *  Calculates the entrance point and the exit point of the line in the shape and returns the
   * length of the distance of the two points.
   * \param line
   * \return
   */
  virtual float getFlightPathInShape(const sLine3D& line);

  /*!
   * \brief suspect
   *  Assuming, this fiber is the first one in a stack of shapes generated with the getNext-method
   * in stacking type stackType. Then the straight line line will hit the stack in the element
   * number, that is returned or -1 if it doesn't hit the stack. This doesn't know about the actual
   * number of elements in this stack, so you'll have to check if the return value exceeds this.
   * \param line
   * \param stackType
   * \return
   */
  virtual int suspect(const sLine3D& line, int stackType);

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
   *  Returns a parameter with the values of this shape.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static Method. Returns a default parameter with all necessary variables defined.
   * \return
   */
  static shape_parameter getDescription();
};

#endif
