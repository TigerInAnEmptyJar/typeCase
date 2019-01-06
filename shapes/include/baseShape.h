#ifndef BASESHAPE
#define BASESHAPE
#include "geometry.h"
//#include <shapeparameter.h>

class TObject;
class fiber;
class shape_parameter;
class volumeShape;

/*!
 * \brief The base_shape class
 */
class base_shape //:public base_parameter
{
protected:
  /*!
 * \brief fname
 */
  string fname;

  /*!
   * \brief center
   *  center point of the base-shape
   */
  point3D center;

  /*!
   * \brief setName
   * \param nme
   */
  void setName(string nme);

public:
  /*!
   * \brief base_shape
   *  Constructor. Sets the name of the shape.
   * \param n
   */
  base_shape(string n);

  /*!
   * \brief base_shape
   *  Copy constructor.
   * \param s
   */
  base_shape(const base_shape& s);

  /*!
   * \brief ~base_shape
   */
  virtual ~base_shape() {}

  /*!
   * \brief getName
   *  Returns the name of the shape.
   * \return
   */
  string getName() const;

  /*!
   * \brief getCenter
   *  Returns the center point of the shape.
   * \return
   */
  point3D getCenter() const;

  /*!
   * \brief setCenter
   *  Sets the center point of the shape.
   * \param c
   */
  void setCenter(point3D c);

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
index of return	idescription
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
   * \brief Draw
   * This function is used for drawing a shape to a root-TCanvas. It will draw a projection of the
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
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   * \return
   */
  static shape_parameter getDescription();
};

#endif
