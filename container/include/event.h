#ifndef EVENT
#define EVENT
#include "cluster.h"
#include "hitCluster.h"
#include "track.h"
class RbaseReaction;
#include "TBase.h"

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
  TEvent();
  ~TEvent();
  void reDimension(int maxH, int maxP, int maxC, int maxT, int maxD);
  void setEventNumber(int evtNr);
  void setRunNumber(int RnNr);
  template <class X>
  int getMaxNumber();
  template <class X>
  int getNumberOf(int num = -1);
  template <class X>
  int& numberOf(int num);
  int& getNumberOfHits(int det = -1);
  int getTotalNumberOfHits() const;
  int& getNumberOfHitClusters(int det = -1);
  int getTotalNumberOfHitClusters() const;
  int& getNumberOfClusters(int det = -1);
  int getTotalNumberOfClusters() const;
  int& getNumberOfPixels(int det = -1);
  int getTotalNumberOfPixels() const;
  int& getNumberOfTracks();
  int getNumberOfPromptTracks();
  int getNumberOfVees();
  int getNumberOfKinks();
  int& getEventNumber();
  int& getRunNumber();
  int& getTrigger();
  vector3D& getPolarisation();
  TPixel& getPixel(int det, int num);
  TCluster& getCluster(int det, int num);
  TCalibHit& getHit(int det, int num);
  THitCluster& getHitCluster(int det, int num);
  TTrack& getTrack(int num);
  int getNumberOfReactions();
  void addReaction(RbaseReaction* r);
  RbaseReaction* getReaction(int num = 0);
  void clearReactions();
  void reset();
};
#endif
