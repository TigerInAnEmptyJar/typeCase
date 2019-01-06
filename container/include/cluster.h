#ifndef CLUSTER
#define CLUSTER
#include "TBase.h"
#include "geometry.h"
#include "pixel.h"

/*!
 * \brief The TCluster class
 * This is the class defining a cluster of pixels. It contains an array for storing the
 * pixels it is made up, an ID, a center point and a radius, that defines in what circle
 * the pixels are located.
 */
class TCluster : public TBase
{
private:
  const int numElem;
  int accElem;
  TPixel** pix; // TPixel **pix;//[numElem]->
  point3D cpoint;
  int id;
  float radius;
  bool defined; //!
public:
  /*!
   * \brief TCluster
   * Constructor. Sets the maximum number of pixels to be stored for this cluster (default = 50).
   * \param maxElements
   */
  TCluster(int maxElements = 50);

  /*!
   * \brief Destructor
   */
  ~TCluster();

  /*!
   * \brief getNumberOfPixels
   * Returns the number of pixels stored for this cluster.
   * \return
   */
  int getNumberOfPixels() const;

  /*!
   * \brief getMaxNumberOfPixels
   * Returns the maximum number of pixels to be stored for this cluster.
   * \return
   */
  int getMaxNumberOfPixels() const;

  /*!
   * \brief getCenter
   * Returns the center point of the cluster.
   * \return
   */
  point3D getCenter() const;

  /*!
   * \brief getPixel
   * Returns a copy of the numth pixel. May be buggy and cause crashs. Use getPixelr() instead.
   * \param num
   * \return
   */
  TPixel getPixel(int num) const;

  /*!
   * \brief setCenter
   * Sets the center point of the cluster.
   * \param p
   */
  void setCenter(point3D p);

  /*!
   * \brief addPixel
   * Adds a pixel to the list of pixels. Take care, that no copy but a pointer to
   * this pixel is stored, after use you will have to see that it is deleted properly.
   * \param p
   */
  void addPixel(TPixel& p);

  /*!
   * \brief resetPixel
   * Sets the number of pixels to zero.
   */
  void resetPixel();

  /*!
   * \brief getCenterr
   * Returns a reference to the centerpoint of the cluster.
   * \return
   */
  point3D& getCenterr();

  /*!
   * \brief getPixelr
   * Returns a reference to the numth pixel stored for this cluster. Returns a "new"
   * TPixel if num is non-valid, that has to be taken care of (potential memory leak).
   * \param num
   * \return
   */
  TPixel& getPixelr(int num);

  /*!
   * \brief setID
   * Sets the cluster ID.
   * \param IDin
   */
  void setID(int IDin);

  /*!
   * \brief getID
   * Returns the cluster ID.
   * \return
   */
  int getID() const;

  /*!
   * \brief ID
   * Returns the cluster ID.
   * \return
   */
  int ID() const;

  /*!
   * \brief isDefined
   * Returns true if the cluster has been modified since creation.
   * \return
   */
  bool isDefined();

  /*!
   * \brief getClusterRadius
   * Returns the radius within which there are pixels to be found.
   * \return
   */
  vector3D getClusterRadius() const;

  /*!
   * \brief setClusterRadius
   * Sets the cluster radius. If you don't want to calculate it yourself, a simple version
   * is used in calculateClusterRadius().
   * \param value
   */
  void setClusterRadius(float value);

  /*!
   * \brief calculateClusterRadius
   * Calculates the cluster radius. For every pixel the distance of its center to the
   * cluster center is calculated; the maximum here is taken. This maximum is increased
   * by the first pixels shapes circumscribed radius.
   */
  void calculateClusterRadius();

  /*!
   * \brief reset
   * Resets the cluster. The number of pixels is set to zero, also the radius and the
   * center point is set to an undefined point.
   */
  void reset();
};
#endif
