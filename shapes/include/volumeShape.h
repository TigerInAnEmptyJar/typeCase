#ifndef VOLUMESHAPE
#define VOLUMESHAPE
#include "baseShape.h"
class planeShape;

/*!
 * \brief The volumeShape class
 */
class volumeShape : public base_shape
{
protected:
  vector3D res;

private:
  float maxDist;

public:
  /*!
   * \brief volumeShape
   *  Constructor. Sets the name of the shape.
   * \param n
   */
  volumeShape(string n = "undefined");

  /*!
   * \brief volumeShape
   *  Copy constructor.
   * \param s
   */
  volumeShape(const volumeShape& s);

  /*!
   * \brief ~volumeShape
   */
  virtual ~volumeShape();

  /*!
   * \brief getMaxDistance
   *  Returns the maxDistance property, that can be used to define the maximum distance, of a line
   * to be counted as hitting the volume.
   * \return
   */
  float getMaxDistance() const;

  /*!
   * \brief setMaxDistance
   *  Sets the maxDistance property, that can be used to define the maximum distance, of a line to
   * be counted as hitting the volume.
   * \param value
   */
  void setMaxDistance(float value);

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
   *  Returns a volume shape, that will be the envelope of times shapes, generated with the
   * getNext-method in stack mode stackType. Take care, that you delete this shape correctly after
   * use (potential memory leak).
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getEnvelope(int times, int stackType);

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
};
#endif
