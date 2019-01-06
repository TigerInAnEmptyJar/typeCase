#ifndef EVENT
#define EVENT
#include "cluster.h"
#include "hitCluster.h"
#include "track.h"
class RbaseReaction;
#include "TBase.h"

/*!
 * \brief The TEvent class
 */
class TEvent : public TBase
{
private:
  int maxHit;
  int maxPixel;
  int maxCluster;
  int maxTrack;
  int maxDet;
  int eventNumber;
  int runNumber;
  int trigger;
  vector3D polarisation;
  int numberOfDets;
  int* numberOfHits; //[maxDet]
  int numberOfTracks;
  int* numberOfPixels;     //[maxDet]
  int* numberOfCluster;    //[maxDet]
  int* numberOfHitCluster; //[maxDet]
  TCalibHit** hits;        //[maxDet] //TCalibHit **hits;//[maxDet][maxHit]
  THitCluster** hitClus;   //[maxDet]
  TTrack* trck;            //[maxTrack]
  TPixel** pix;            //[maxDet] TPixel **pix;//[maxDet][maxPixel]
  TCluster** clust;        //[maxDet] TCluster **clust;//[maxDet][maxCluster]
  int maxReactions;
  int numReactions;
  RbaseReaction** reactions; //[maxReactions]
public:
  /*!
   * \brief TEvent
   * Default constructor.
   */
  TEvent();

  /*!
   * \brief Destructor.
   */
  ~TEvent();

  /*!
   * \brief reDimension
   * Sets the maximum number of hits to maxH, the maximum number of pixels to maxP, the maximum
   * number of clusters to maxC each per detector or ID, the maximum number of detectors or IDs to
   * maxD and the maximum number of tracks to maxT.
   * \param maxH
   * \param maxP
   * \param maxC
   * \param maxT
   * \param maxD
   */
  void reDimension(int maxH, int maxP, int maxC, int maxT, int maxD);

  /*!
   * \brief setEventNumber
   * Sets the event number.
   * \param evtNr
   */
  void setEventNumber(int evtNr);

  /*!
   * \brief setRunNumber
   * Sets the run number.
   * \param RnNr
   */
  void setRunNumber(int RnNr);

  /*!
   * Template function. Defined for the following types: TRawHit, TCalibHit: returns the maximum
   * number of hits storable for one detector. TPixel: returns the maximum number of pixels storable
   * for one ID. TCluster: returns the maximum number of clusters storable for one ID. TTrack:
   * returns the maximum number of tracks. TDetector: returns the maximum number of detectors and
   * IDs usable in the event. Returns zero else.
   */
  template <class X>
  int getMaxNumber();

  /*!
   * Template function. Returns one of the following variables:
|template class X	|variable|
|TDetector	|numberOfDetectors()|
|TCalibHit / TRawHit	|numberOfHits(num)|
|TPixel	|numberOfPixels(num)|
|THitCluser	|numberOfHitClusters(num)|
|TCluster	|numberOfClusters(num)|
|TTrack	|numberOfTracks()|
   */
  template <class X>
  int getNumberOf(int num = -1);

  /*!
   * Template function. Returns a reference to the following variables:
|template class X	|variable|
|TDetector	|numberOfDetectors()|
|TCalibHit / TRawHit	|numberOfHits(num)|
|TPixel	|numberOfPixels(num)|
|THitCluser	|numberOfHitClusters(num)|
|TCluster	|numberOfClusters(num)|
|TTrack	|numberOfTracks()|
   */
  template <class X>
  int& numberOf(int num);

  /*!
   * \brief getNumberOfHits
   * Returns the number of hits defined for the detector det. If det is -1, it returns the maximum
   * number of hits to be defined for one detector, if it is -2 it returns the maximum number of
   * detectors to be used. Returns -1 if det is non-valid.
   * \param det
   * \return
   */
  int& getNumberOfHits(int det = -1);

  /*!
   * \brief getTotalNumberOfHits
   * Returns the total number of hits defined in this event.
   * \return
   */
  int getTotalNumberOfHits() const;

  /*!
   * \brief getNumberOfHitClusters
   * Returns a reference to the number of hit clusters for detector det.
   * \param det
   * \return
   */
  int& getNumberOfHitClusters(int det = -1);

  /*!
   * \brief getTotalNumberOfHitClusters
   * Returns the total number of hit-clusters defined in this event.
   * \return
   */
  int getTotalNumberOfHitClusters() const;

  /*!
   * \brief getNumberOfClusters
   * Returns the number of cluster defined for the ID det. If det is -1, it returns the maximum
   * number of clusters to be defined for one ID, if it is -2 it returns the maximum number of IDs
   * to be used. Returns -1 if det is non-valid.
   * \param det
   * \return
   */
  int& getNumberOfClusters(int det = -1);

  /*!
   * \brief getTotalNumberOfClusters
   * Returns the total number of cluster defined in this event.
   * \return
   */
  int getTotalNumberOfClusters() const;

  /*!
   * \brief getNumberOfPixels
   * Returns the number of pixels defined for the ID det. If det is -1, it returns the maximum
   * number of pixels to be defined for one ID, if it is -2 it returns the maximum number of IDs to
   * be used. Returns -1 if det is non-valid.
   * \param det
   * \return
   */
  int& getNumberOfPixels(int det = -1);

  /*!
   * \brief getTotalNumberOfPixels
   * Returns the total number of pixels defined in this event.
   * \return
   */
  int getTotalNumberOfPixels() const;

  /*!
   * \brief getNumberOfTracks
   * Returns the number of tracks currently defined.
   * \return
   */
  int& getNumberOfTracks();

  /*!
   * \brief getNumberOfPromptTracks
   * Return the number of prompt tracks in this event.
   * \return
   */
  int getNumberOfPromptTracks();

  /*!
   * \brief getNumberOfVees
   * Returns the current number of neutral 2-track-decays in this event.
   * \return
   */
  int getNumberOfVees();

  /*!
   * \brief getNumberOfKinks
   * Returns the current number of charged decays in the event.
   * \return
   */
  int getNumberOfKinks();

  /*!
   * \brief getEventNumber
   * Returns a reference to the event number.
   * \return
   */
  int& getEventNumber();

  /*!
   * \brief getRunNumber
   * Returns a reference to the run number.
   * \return
   */
  int& getRunNumber();

  /*!
   * \brief getTrigger
   * Returns a reference to the trigger pattern of this event.
   * \return
   */
  int& getTrigger();

  /*!
   * \brief getPolarisation
   * Returns the polarisation vector of the event. Will have to be modified since incomplete.
   * \return
   */
  vector3D& getPolarisation();

  /*!
   * \brief getPixel
   * Returns a reference to the numth pixel of ID det of the event, also if this pixel has currently
   * not been defined.
   * \param det
   * \param num
   * \return
   */
  TPixel& getPixel(int det, int num);

  /*!
   * \brief getCluster
   * Returns a reference to the numth cluster of ID det of the event, also if this cluster has
   * currently not been defined.
   * \param det
   * \param num
   * \return
   */
  TCluster& getCluster(int det, int num);

  /*!
   * \brief getHit
   * Returns a reference to the numth hit in the detector det of the event, also if this hit has
   * currently not been defined.
   * \param det
   * \param num
   * \return
   */
  TCalibHit& getHit(int det, int num);

  /*!
   * \brief getHitCluster
   * Returns a reference to the numth hit-cluster of detector det.
   * \param det
   * \param num
   * \return
   */
  THitCluster& getHitCluster(int det, int num);

  /*!
   * \brief getTrack
   * Returns a reference to the numth track of the event, also if this track has currently not been
   * defined.
   * \param num
   * \return
   */
  TTrack& getTrack(int num);

  /*!
   * \brief getNumberOfReactions
   * Returns the number of reactions stored in this event.
   * \return
   */
  int getNumberOfReactions();

  /*!
   * \brief addReaction
   * Adds a reaction to the event. Does nothing if r is NULL. The reactions array is sorted
   * according to the RbaseReaction::getChiSquared() value of the reactions. The reaction is now a
   * property of the event and will be deleted at reset() or clearReactions()
   * \param r
   */
  void addReaction(RbaseReaction* r);

  /*!
   * \brief getReaction
   * Returns the numth reaction, that is defined for this event. Returns NULL, if num is non-valid.
   * \param num
   * \return
   */
  RbaseReaction* getReaction(int num = 0);

  /*!
   * \brief clearReactions
   * Deletes all reactions. The number of reactions is set to zero.
   */
  void clearReactions();

  /*!
   * \brief reset
   * Resets the event. All hits, pixels, clusters and tracks will be reset, the reactions deleted
   * and all numbers of properties set to zero.
   */
  void reset();
};
#endif
