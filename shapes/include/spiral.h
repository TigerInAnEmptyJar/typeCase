#ifndef SPIRAL
#define SPIRAL
#include "volumeShape.h"

/*!
 * \brief The spiral class
 */
class spiral : public volumeShape
{
protected:
  /*!
 * \brief innerR
 *  inner radius.
 */
  float innerR;

  /*!
   * \brief outerR
   *  outer radius.
   */
  float outerR;

  /*!
   * \brief bending
   *  bending parameter : d(phi) / d(rho)
   */
  float bending;

  /*!
   * \brief dphi
   *  angular range.
   */
  float dphi;

  /*!
   * \brief thickness
   *  thickness
   */
  float thickness;

  /*!
   * \brief normal
   *  normal vector.
   */
  vector3D normal;

  /*!
   * \brief lpe
   *  direction vector for the lower-phi-edge.
   */
  vector3D lpe;

  /*!
   * \brief hpe
   *  direction vector for the higher-phi-edge.
   */
  vector3D hpe;

public:
  /*!
   * \brief spiral
   *  Constructor. Sets the spirals properties:
   * The starting direction for the second edge (the higher-phi-edge) will be calculated by rotating
   * the lower-phi-edge around the normal vector by the angular range.
   * \param i inner radius
   * \param a outer radius
   * \param b bending: d(phi) / d(rho)
   * \param d angular range
   * \param t thickness
   * \param c center point
   * \param n normal vector to the front plane
   * \param l	starting direction for one of the edges
   */
  spiral(float i = 0, float a = 1000, float b = 1, float d = 1, float t = 1,
         point3D c = point3D(0, 0, 0), vector3D n = vector3D(0, 0, 1),
         vector3D l = vector3D(1, 0, 0));

  /*!
   * \brief spiral
   *  Copy constructor. Works if the shape is a spiral or a wedge.
   * \param s
   */
  spiral(const volumeShape& s);

  /*!
   * \brief spiral
   *  Constructor. Creates a archimedian spiral out of the values in the description.
   * \param description
   */
  spiral(const shape_parameter& description);

  /*!
   * \brief Destructor
   */
  ~spiral();

  /*!
   * \brief getInnerRadius
   *  Returns the inner radius of the spiral.
   * \return
   */
  virtual float getInnerRadius() const;

  /*!
   * \brief getOuterRadius
   *  Returns the outer radius of the spiral.
   * \return
   */
  virtual float getOuterRadius() const;

  /*!
   * \brief getBending
   *  Returns the bending: d(phi) / d(rho)
   * \return
   */
  virtual float getBending() const;

  /*!
   * \brief getPhiRange
   *  Returns the angular range.
   * \return
   */
  virtual float getPhiRange() const;

  /*!
   * \brief getThickness
   *  Returns the thickness of the spiral.
   * \return
   */
  virtual float getThickness() const;

  /*!
   * \brief getNormal
   *  Returns the normal vector to the front plane.
   * \return
   */
  virtual vector3D getNormal() const;

  /*!
   * \brief getLowerPhiEdge
   *  Returns the starting direction of the lower-phi-edge.
   * \return
   */
  virtual vector3D getLowerPhiEdge() const;

  /*!
   * \brief setInnerRadius
   *  Sets the inner radius of the spiral.
   * \param v
   */
  void setInnerRadius(float v);

  /*!
   * \brief setOuterRadius
   *  Sets the outer radius of the spiral.
   * \param v
   */
  void setOuterRadius(float v);

  /*!
   * \brief setBending
   *  Sets the bending: d(phi)/d(rho) of the spiral.
   * \param v
   */
  void setBending(float v);

  /*!
   * \brief setPhiRange
   *  Sets the angular range of the spiral.
   * \param v
   */
  void setPhiRange(float v);

  /*!
   * \brief setThickness
   *  Sets the thickness of the spiral.
   * \param t
   */
  void setThickness(float t);

  /*!
   * \brief setCenter
   *  Sets the center point of the shape.
   * \param c
   */
  void setCenter(point3D c);

  /*!
   * \brief setNormal
   *  Sets the normal vector.
   * \param v
   */
  void setNormal(vector3D v);

  /*!
   * \brief setLowerPhiEdge
   *  Sets the starting direction for the lower phi edge.
   * \param v
   */
  void setLowerPhiEdge(vector3D v);

  /*!
   * \brief getNext
   *  Returns a new volume shape, that is shifted, rotated, shrunk, enlarged, or else modified
according to the parameter stackType, generating the timesth element in the stack. Take care, that
you delete this shape correctly after use (potential memory leak).
The following stackTypes have been defined:
2	a clone of the shape is returned
else	the new spiral is rotated times times around the normal vector by the angular range
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getNext(int times, int stackType);

  /*!
   * \brief getEnvelope
   *  Returns a volume shape, that will be the envelope of times shapes, generated with the
getNext-method in stack mode stackType. Take care, that you delete this shape correctly after use
(potential memory leak).
The following stackTypes have been defined:
2	a clone of the shape is returned
else	the envelope is eighther a ring or a spiral, if the angular range * times is smaller
than 2*Pi.
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
   * \brief cut
   *  Returns true, if the shape and the fiber occupy some space commonly.
   * \param f
   * \return
   */
  virtual bool cut(const fiber& f);

  /*!
   * \brief operator =
   *  Copy operator. Works only if the shape is a spiral or a wedge.
   * \param s
   */
  virtual void operator=(volumeShape* s);

  /*!
   * \brief getClone
   *  Creates a new instance of the same volume shape with the same properties. Take care, that you
   * delete this shape correctly after use (potential memory leak).
   * \return
   */
  virtual volumeShape* getClone();

  /*!
   * \brief HitParams
   *  Returns the parameters for a hit of a straight line with the shape. The pattern will be:
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
   *  Returns the parameters for a hit of a pyramid like shape with the tip at origin and the shape
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
   * \brief entrance
   *  Returns the entrance point of the straight line into the shape. This point is undefined if the
   * line does not hit the shape.
   * \param line
   * \return
   */
  virtual point3D entrance(const sLine3D& line);

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
   *  This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
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
   *  Returns a description of this spiral.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of a spiral with all necessary variables defined.
   * \return
   */
  static shape_parameter getDescription();
};
#endif
