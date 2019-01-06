#ifndef HITCLUSTER
#define HITCLUSTER
#include "calibHit.h"
#include "geometry.h"
using namespace std;

/*!
 * \brief The THitCluster class
 */
class THitCluster : public TCalibHit
{
private:
  const int numElem;
  int accElem;
  TCalibHit** hits; //!
public:
  /*!
   * \brief THitCluster
   * Default constructor. Sets the maximal number of hits for this cluster.
   * \param maxElements
   */
  THitCluster(int maxElements = 50);

  /*!
   * \brief THitCluster
   * Copy Constructor.
   * \param cls
   */
  THitCluster(const THitCluster& cls);

  /*!
   * \brief THitCluster
   * Constructor. Sets the maximum number of hits in this cluster and initializes the base hit.
   * \param input
   * \param maxElements
   */
  THitCluster(TCalibHit& input, int maxElements = 50);

  /*!
   * \brief ~THitCluster
   * Destructor.
   */
  virtual ~THitCluster();

  /*!
   * \brief getNumberOfHits
   * Returns the current number of hits in this hit-cluster.
   * \return
   */
  int getNumberOfHits() const;

  /*!
   * \brief getMaxNumberOfHits
   * Returns the maximum number of hits for this hit-cluster.
   * \return
   */
  int getMaxNumberOfHits() const;

  /*!
   * \brief getHit
   * Returns the numth hit of the cluster (or NULL if num is invalid).
   * \param num
   * \return
   */
  TCalibHit* getHit(int num) const;

  /*!
   * \brief addHit
   * Adds a hit to the hit-cluster. If it is already contained in the hit-cluster, it is not added a
   * second time.
   * \param h
   */
  void addHit(TCalibHit* h);

  /*!
   * \brief resetHits
   * Resets the hits in this hit cluster. But not the inherrited properties.
   */
  void resetHits();

  /*!
   * \brief reset
   * Resets the hit-cluster.
   */
  virtual void reset();

  /*!
   * \brief operator =
   * Copy operator.
   * \param hc
   * \return
   */
  THitCluster& operator=(const THitCluster& hc);

  /*!
   * \brief operator =
   * Copy operator. Copies the base properties of hc, the number of hits in this cluster is 1.
   * \param hc
   * \return
   */
  THitCluster& operator=(const TCalibHit& hc);

  /*!
   * \brief operator +=
   * Addition operator. Adds a hit to the hit-cluster. If this hit is already contained, it is not
   * added a second time.
   * \param hc
   */
  void operator+=(TCalibHit& hc);
};
#endif
