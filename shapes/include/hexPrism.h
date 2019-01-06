#ifndef HEXPRISM
#define HEXPRISM
#include "volumeShape.h"

/*!
 * \brief The hexPrism class
 */
class hexPrism : public volumeShape
{
private:
  vector3D dir1;
  vector3D dir2;
  vector3D dir3;
  int stacki;

public:
  /*!
   * \brief hexPrism
   * Constructor. Sets the properties.
parameter	description
c	center of the front hexagon
thickness	distance vector front to back hexagon
toFirstPoint	vector from the center to the first corner of the hexagon.
keyWidth	vector from the center to the middle of one of the edges of the hexagon.
st	stacking parameter
Note, that the three vectors have to be orthogonal.
   * \param c
   * \param thickness
   * \param toFirstPoint
   * \param keyWidth
   * \param st
   */
  hexPrism(point3D c, vector3D thickness, vector3D toFirstPoint, vector3D keyWidth, int st);

  /*!
   * \brief hexPrism
   *  Copy constructor. Works, if the shape is a hexprism.
   * \param s
   */
  hexPrism(const volumeShape& s);

  /*!
   * \brief hexPrism
   *  Constructor. Creates a hexPrism out of the values of the parameter.
   * \param description
   */
  hexPrism(const shape_parameter& description);

  /*!
   * \brief ~hexPrism
   */
  virtual ~hexPrism();

  /*!
   * \brief getDirection
   * Returns the numth direction of the hexPrism:
num=0: thickness
num=1: center to first point
num=2: key width
   * \param num
   * \return
   */
  virtual vector3D getDirection(int num) const;

  /*!
   * \brief setDirection
   * Sets the numth direction of the hexPrism:
num=0: thickness
num=1: center to first point
num=2: key width
   * \param num
   * \param v
   */
  void setDirection(int num, vector3D v);

  /*!
   * \brief getStackingOrder
   *  Returns the stacking parameter. If it is <0, the center shape will be missing. Its absolute
   * number is the number of rings around the center shape, that are thaken into account.
   * \return
   */
  int getStackingOrder() const;

  /*!
   * \brief setStackingOrder
   *  Sets the stacking parameter. If it is <0, the center shape will be missing. Its absolute
   * number is the number of rings around the center shape, that are thaken into account.
   * \param st
   */
  void setStackingOrder(int st);

  /*!
   * \brief getNext
   * Returns a new volume shape, that is shifted, rotated, shrunk, enlarged, or else modified
according to the parameter stackType, generating the timesth element in the stack. Take care, that
you delete this shape correctly after use (potential memory leak).
The following stackTypes have been defined:
type
2	line stacking. The new hexprism is shifted in thickness direction by times the thickness.
3	Snake stacking. The envelope is a hexprism, the key width depends on the stacking parameter
With the stacking parameter <0 the 6 elements that are farest are missing.
-3	Snake stacking. The envelope is a hexprism without the center element, the key width
depends on the stacking parameter
With the stacking parameter <0 the 6 elements that are farest are missing.

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
The following stackTypes have been defined:
type
2	line stacking. The thickness is increased in length by times the thickness.
3, -3	Snake stacking. The envelope is a hexprism, the key width depends on the number of elements
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getEnvelope(int times, int stackType);

  /*!
   * \brief entrance
   *  Returns the entrance point of the straight line into the shape. This point is undefined if the
   * line does not hit the shape.
   * \param line
   * \return
   */
  virtual point3D entrance(const sLine3D& line);

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
   * \brief cut
   *  Returns true, if the shape and the fiber occupy some space commonly.
   * \param f
   * \return
   */
  virtual bool cut(const fiber& f);

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
   * \brief getFlightPathInShape
   *  Calculates the entrance point and the exit point of the line in the shape and returns the
   * length of the distance of the two points.
   * \param line
   * \return
   */
  virtual float getFlightPathInShape(const sLine3D& line);

  /*!
   * \brief getClone
   *  Creates a new instance of the same volume shape with the same properties. Take care, that you
   * delete this shape correctly after use (potential memory leak).
   * \return
   */
  virtual volumeShape* getClone();

  /*!
   * \brief suspect
   *  Assuming, this volume shape is the first one in a stack of shapes generated with the
   * getNext-method in stacking type stackType. Then the straight line line will hit the stack in
   * the element number, that is returned or -1 if it doesn't hit the stack. This doesn't know about
   * the actual number of elements in this stack, so you'll have to check if the return value
   * exceeds this.
   * \param line
   * \param stackType
   * \return
   */
  virtual int suspect(const sLine3D& line, int stackType);

  /*!
   * \brief Draw
   *  This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
3D shape from the eye-point to a plane. The corners of the drawn shape in canvas coordinates are
stored in boundingBox.
It is drawn using lColor as line color, fColor as fill color and fStyle as fill style. See
root-colors and -styles.
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
   * This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
3D shape from the eye-point to a plane. The corners of the drawn shape in canvas coordinates are
stored in boundingBox.
It is drawn using lColor as line color, fColor as fill color and fStyle as fill style. See
root-colors and -styles.
The parameter ident will return a pointer to the drawn root-primitive to provide the
distance-to-primitive functionality.
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
   *  Returns a parameter describing this hexPrism.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static Method. Returns a default description of a hexPrism with all necessary variables
   * defined.
   * \return
   */
  static shape_parameter getDescription();
};

#endif
