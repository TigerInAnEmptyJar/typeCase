#include "planeShape.h"

/*!
 * \brief The circle class
 */
class circle : public planeShape
{
private:
  float rad;

public:
  /*!
   * \brief circle
   *  Constructor. Sets the center point, the normal vector and the radius.
   * \param cen
   * \param nor
   * \param rIn
   */
  circle(point3D cen, vector3D nor, float rIn);

  /*!
   * \brief circle
   *  Constructor creates a circle from parameter.
   * \param description
   */
  circle(const shape_parameter& description);

  /*!
   * \brief ~circle
   */
  virtual ~circle();

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points of the shape: 1.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numth point of the shape: only num=0 is valid, this gives the center point. Else
   * it returns an undefined point.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief area
   *  Returns the area of the circle.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of a circle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of a circle, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief radius
   *  Returns the radius.
   * \return
   */
  float radius() const;

  /*!
   * \brief setRadius
   *  Sets the radius.
   */
  void setRadius(float);

  /*!
   * \brief distancePlane
   *  Returns the distance vector from the plane to the shape.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance vector from the straight line to the shape.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

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
   *  Returns a description of this shape, that would produce a shape like this.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default shape-parameter with all necessary variables defined. Check
   * contens of parameter for sensible values.
   * \return
   */
  static shape_parameter getDescription();
};
