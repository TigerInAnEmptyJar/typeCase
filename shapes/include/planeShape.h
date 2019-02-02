#ifndef PLANESHAPE
#define PLANESHAPE
#include "baseShape.h"
#include "geometry.h"

/*!
 * \brief The planeShape class
 */
class planeShape : public base_shape
{
protected:
  /*!
 * \brief normal
 *  normal vector to the plane
 */
  vector3D normal;

  /*!
   * \brief circRadius
   *  circumscribed radius
   */
  float circRadius;

public:
  /*!
   * \brief planeShape
   *  Constructor. Sets the name of the shape.
   * \param n
   */
  planeShape(const string& n);

  /*!
   * \brief planeShape
   *  Constructor. Creates a shape as defined in the parameter. Please overwrite in derived classes.
   * \param description
   */
  planeShape(const shape_parameter& description);

  /*!
   * \brief ~planeShape
   */
  virtual ~planeShape() = default;

  /*!
   * \brief getNormal
   *  Returns the vormal vector of the planar shape.
   * \return
   */
  vector3D getNormal() const;

  /*!
   * \brief setNormal
   *  Set the normal vector of the planar shape.
   * \param v
   */
  void setNormal(const vector3D& v);

  /*!
   * \brief getPlane
   *  Returns the plane the planar shape is defined along.
   * \return
   */
  plane3D getPlane() const;

  /*!
   * \brief setPlane
   *  Set the plane for the planar shape. The normal vector will be set.
   * \param p
   */
  void setPlane(const plane3D& p);

  /*!
   * \brief getNumberOfPoints
   *  Returns the number of points the planar shape defines.
   * \return
   */
  virtual int getNumberOfPoints() const;

  /*!
   * \brief getPoint
   *  Returns the numth point of the planar shape.
   * \param num
   * \return
   */
  virtual point3D getPoint(int num) const;

  /*!
   * \brief area
   *  Returns the area of the planar shape.
   * \return
   */
  virtual float area() const;

  /*!
   * \brief angularRange_Phi
   *  Returns the polar angular range of a planar shape, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief angularRange_Theta
   *  Returns the azimuthal angular range of a planar shape, for any z direction and origin.
   * \param origin
   * \param zDirection
   * \return
   */
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;

  /*!
   * \brief distancePlane
   *  Returns the distance vector for a plane to the planar shape.
   * \param p
   * \return
   */
  virtual vector3D distancePlane(const plane3D& p);

  /*!
   * \brief distance
   *  Returns the distance from a line to the planar shape.
   * \param line
   * \return
   */
  virtual vector3D distance(const sLine3D& line);

  /*!
   * \brief circumscribedRadius
   *  Returns the circumscribed radius of a planar shape.
   * \return
   */
  virtual float circumscribedRadius() const;

  /*!
   * \brief description
   *  Returns a description of this shape. Please overwrite in derived classes.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of the shape with all necessary variables
   * defined. Please overwrite in derived classes.
   * \return
   */
  static shape_parameter getDescription();
};

inline vector3D planeShape::getNormal() const { return normal; }

inline void planeShape::setNormal(const vector3D& v) { normal = v; }

inline float planeShape::area() const { return -1; }

inline planeShape::planeShape(const string& n) : base_shape(n)
{
  setCenter(point3D(0, 0, 0));
  normal = vector3D(0, 0, 1);
}

#endif
