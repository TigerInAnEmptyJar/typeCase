#ifndef WEDGE
#define WEDGE
#include "volumeShape.h"

/*!
 * \brief The wedge class
 */
class wedge : public volumeShape
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
   * \brief dphi
   *  angular range of the wedge.
   */
  float dphi;

  /*!
   * \brief thickness
   *  thickness.
   */
  float thickness;

  /*!
   * \brief normal
   *  normal vector.
   */
  vector3D normal;

  /*!
   * \brief lpe
   *  The lower phi edge direction vector.
   */
  vector3D lpe;

  /*!
   * \brief hpe
   *  The higher phi edge direction vector.
   */
  vector3D hpe;

  /*!
   * \brief distBarrel
   *  The distance-to-barrel-center. It is optional. If it is -1 (default) a normal wegde is taken.
   * It is a parameter for stacking and gives the possibility to stack the elements to a cone stump
   * mantle, where it. gives the radius of the base circle.
   */
  float distBarrel;

public:
  /*!
   * \brief wedge
   * Constructor. Sets the properties.
   * dis	distance of the outer radius point to the straight line (center,normal to envelope)
The parameter dis is optional. If it is -1 (default) a normal wegde is taken. It is a parameter for
stacking and gives the possibility to stack the elements to a cone stump mantle, where dis gives the
radius of the base circle.
   * \param i inner radius of the wedge
   * \param a outer radius of the wedge
   * \param d angular range
   * \param t thickness
   * \param c center point
   * \param n normal vector
   * \param l direction of one edge
   * \param dis distance of the outer radius point to the straight line (center,normal to envelope)
   */
  wedge(float i = 0, float a = 1000, float d = 1, float t = 1, point3D c = point3D(0, 0, 0),
        vector3D n = vector3D(0, 0, 1), vector3D l = vector3D(1, 0, 0), float dis = -1);

  /*!
   * \brief ~wedge
   */
  virtual ~wedge();

  /*!
   * \brief wedge
   *  Copy constructor.
   * \param s
   */
  wedge(const wedge& s);

  /*!
   * \brief wedge
   *  Copy constructor. Works only if the shape is a ring, a wedge or a spiral.
   * \param s
   */
  wedge(const volumeShape& s);

  /*!
   * \brief wedge
   *  Constructor. Creates a wedge with the values taken from the description.
   * \param description
   */
  wedge(const shape_parameter& description);

  /*!
   * \brief getInnerRadius
   *  Returns the inner radius of the wegde.
   * \return
   */
  virtual float getInnerRadius() const;

  /*!
   * \brief getOuterRadius
   *  Returns the outer radius of the wedge.
   * \return
   */
  virtual float getOuterRadius() const;

  /*!
   * \brief getPhiRange
   *  Returns the angular range.
   * \return
   */
  virtual float getPhiRange() const;

  /*!
   * \brief getThickness
   *  Returns the thickness of the wedge.
   * \return
   */
  virtual float getThickness() const;

  /*!
   * \brief getNormal
   *  Returns the normal vector.
   * \return
   */
  virtual vector3D getNormal() const;

  /*!
   * \brief getLowerPhiEdge
   *  Returns the direction vector of the edge with the lower angle.
   * \return
   */
  virtual vector3D getLowerPhiEdge() const;

  /*!
   * \brief getHigherPhiEdge
   *  Returns the direction vector of the edge with the higher angle.
   * \return
   */
  virtual vector3D getHigherPhiEdge() const;

  /*!
   * \brief getDistToBarrelCenter
   *  Returns the distance-to-barrel-center. It is optional. If it is -1 (default) a normal wegde is
   * taken. It is a parameter for stacking and gives the possibility to stack the elements to a cone
   * stump mantle, where it. gives the radius of the base circle.
   * \return
   */
  float getDistToBarrelCenter() const;

  /*!
   * \brief setInnerRadius
   *  Sets the inner radius of the wedge.
   * \param v
   */
  void setInnerRadius(float v);

  /*!
   * \brief setOuterRadius
   *  Sets the outer radius of the wedge.
   * \param v
   */
  void setOuterRadius(float v);

  /*!
   * \brief setPhiRange
   *  Sets the angular range of the wedge.
   * \param v
   */
  void setPhiRange(float v);

  /*!
   * \brief setThickness
   *  Sets the thickness of the wedge.
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
   *  Sets the direction of the edge with the lower phi angle.
   * \param v
   */
  void setLowerPhiEdge(vector3D v);

  /*!
   * \brief setDistToBarrelCenter
   *  Sets the distance-to-barrel-center. It is optional. If it is -1 (default) a normal wegde is
   * taken. It is a parameter for stacking and gives the possibility to stack the elements to a cone
   * stump mantle, where it. gives the radius of the base circle.
   * \param v
   */
  void setDistToBarrelCenter(float v);

  /*!
   * \brief getNext
   *  Returns a new volume shape, that is shifted, rotated, shrunk, enlarged, or else modified
according to the parameter stackType, generating the timesth element in the stack. Take care, that
you delete this shape correctly after use (potential memory leak). The following stacking types have
been defined:
circular stacking (2): the new wedge is rotated times * angular range around the normal vector,
cone stacking (4): the new wedge is also rotated, but the tilt is taken into account.
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
The following stacking types have been defined:
circular stacking (2): results in a ring shaped envelope or a wedge if the angular range * times is
smaller than 2*Pi,
cone stacking (4): results in a cone shaped envelope. Doesn't return an appropriate envelope yet.
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
   * \brief getFlightPathInShape
   *  Calculates the entrance point and the exit point of the line in the shape and returns the
   * length of the distance of the two points.
   * \param line
   * \return
   */
  virtual float getFlightPathInShape(const sLine3D& line);

  /*!
   * \brief cut
   *  Returns true, if the shape and the fiber occupy some space commonly.
   * \param f
   * \return
   */
  virtual bool cut(const fiber& f);

  /*!
   * \brief operator =
   *  Copy operator. Works only if the shape is a wedge, spiral or ring.
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
   * For usual circular- and line-stacking the routine is tested and working, for cone-stacking it
   * depends on the line.
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
   *  Returns a description of this wedge.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of a wedge, defining all necessary variables in
   * the parameter.
   * \return
   */
  static shape_parameter getDescription();
};

#endif
