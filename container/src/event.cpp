#include "event.h"
//#include "logger.h"
#include "RbaseReaction.h"
#include "detector.h"
#include <typeinfo>
TEvent::TEvent()
    : TBase("TEvent"), maxHit(0), maxPixel(0), maxCluster(0), maxTrack(0), maxDet(0),
      maxReactions(0)
{
  hits = NULL;
  reDimension(1, 1, 1, 1, 1);
}

void TEvent::reDimension(int maxH, int maxP, int maxC, int maxT, int maxD)
{
  if (hits != NULL) {
    for (int i = 0; i < maxDet; i++) {
      delete[] hits[i];
      delete[] pix[i];
      delete[] clust[i];
      delete[] hitClus[i];
    }
    delete[] hits;
    delete[] trck;
    delete[] pix;
    delete[] clust;
    delete[] hitClus;
    delete[] numberOfHits;
    delete[] numberOfPixels;
    delete[] numberOfCluster;
    delete[] numberOfHitCluster;
  }
  maxHit = maxH;
  maxPixel = maxP;
  maxCluster = maxC;
  maxTrack = maxT;
  maxDet = maxD;
  maxReactions = maxT * maxT;
  hits = new TCalibHit*[maxDet];
  trck = new TTrack[maxTrack];
  pix = new TPixel*[maxDet];     //(maxP,0);//TPixel*[maxDet];
  clust = new TCluster*[maxDet]; //(maxC,0);//TCluster*[maxDet];
  hitClus = new THitCluster*[maxDet];
  numberOfHits = new int[maxDet];
  numberOfTracks = 0;
  numberOfPixels = new int[maxDet];
  numberOfCluster = new int[maxDet];
  numberOfHitCluster = new int[maxDet];
  for (int i = 0; i < maxDet; i++) {
    hits[i] = new TCalibHit[maxHit];
    pix[i] = new TPixel[maxPixel];
    clust[i] = new TCluster[maxCluster];
    hitClus[i] = new THitCluster[maxHit];
    numberOfHits[i] = 0;
    numberOfPixels[i] = 0;
    numberOfCluster[i] = 0;
    numberOfHitCluster[i] = 0;
  }
  numReactions = 0;
  numberOfDets = maxDet;
  reactions = new RbaseReaction*[maxReactions];
  for (int i = 0; i < maxReactions; i++)
    reactions[i] = NULL;
}

TEvent::~TEvent()
{
  if (hits == NULL)
    return;
  for (int i = 0; i < maxDet; i++) {
    delete[] hits[i];
    delete[] pix[i];
    delete[] clust[i];
    delete[] hitClus[i];
  }
  delete[] hits;
  delete[] trck;
  delete[] pix;
  delete[] clust;
  delete[] hitClus;
  delete[] numberOfHits;
  delete[] numberOfPixels;
  delete[] numberOfCluster;
  delete[] numberOfHitCluster;
  for (int i = 0; i < maxReactions; i++)
    if (reactions[i] != NULL)
      delete reactions[i];
  delete[] reactions;
}

void TEvent::setEventNumber(int evtNr) { eventNumber = evtNr; }

void TEvent::setRunNumber(int RnNr) { runNumber = RnNr; }

template int TEvent::getMaxNumber<TRawHit>();
template int TEvent::getMaxNumber<TCalibHit>();
template int TEvent::getMaxNumber<THitCluster>();
template int TEvent::getMaxNumber<TPixel>();
template int TEvent::getMaxNumber<TCluster>();
template int TEvent::getMaxNumber<TDetector>();
template int TEvent::getMaxNumber<TTrack>();
template <class X>
int TEvent::getMaxNumber()
{
  if (typeid(X) == typeid(TRawHit) || typeid(X) == typeid(TCalibHit) ||
      typeid(X) == typeid(THitCluster))
    return maxHit;
  if (typeid(X) == typeid(TPixel))
    return maxPixel;
  if (typeid(X) == typeid(TCluster))
    return maxCluster;
  if (typeid(X) == typeid(TDetector))
    return maxDet;
  if (typeid(X) == typeid(TTrack))
    return maxTrack;
  return 0;
}
template int TEvent::getNumberOf<TRawHit>(int num);
template int TEvent::getNumberOf<TCalibHit>(int num);
template int TEvent::getNumberOf<THitCluster>(int num);
template int TEvent::getNumberOf<TPixel>(int num);
template int TEvent::getNumberOf<TCluster>(int num);
template int TEvent::getNumberOf<TDetector>(int num);
template int TEvent::getNumberOf<TTrack>(int num);
template <class X>
int TEvent::getNumberOf(int num)
{
  if (typeid(X) == typeid(TRawHit) || typeid(X) == typeid(TCalibHit))
    return getNumberOfHits(num);
  if (typeid(X) == typeid(TPixel))
    return getNumberOfPixels(num);
  if (typeid(X) == typeid(TCluster))
    return getNumberOfClusters(num);
  if (typeid(X) == typeid(THitCluster))
    return getNumberOfHitClusters(num);
  if (typeid(X) == typeid(TTrack))
    return getNumberOfTracks();
  return 0;
}
template int& TEvent::numberOf<TRawHit>(int num);
template int& TEvent::numberOf<TCalibHit>(int num);
template int& TEvent::numberOf<THitCluster>(int num);
template int& TEvent::numberOf<TPixel>(int num);
template int& TEvent::numberOf<TCluster>(int num);
template int& TEvent::numberOf<TDetector>(int num);
template int& TEvent::numberOf<TTrack>(int num);
template <class X>
int& TEvent::numberOf(int num)
{
  if (typeid(X) == typeid(TRawHit) || typeid(X) == typeid(TCalibHit))
    return getNumberOfHits(num);
  if (typeid(X) == typeid(TPixel))
    return getNumberOfPixels(num);
  if (typeid(X) == typeid(TCluster))
    return getNumberOfClusters(num);
  if (typeid(X) == typeid(THitCluster))
    return getNumberOfHitClusters(num);
  if (typeid(X) == typeid(TTrack))
    return getNumberOfTracks();
  return getTrigger();
}
int& TEvent::getNumberOfHitClusters(int det)
{
  //   int ret=maxHit;
  //   if(det==-1) return ret;
  //   ret=maxDet;
  //   if(det==-2) return ret;
  //   ret=-1;
  if ((det < 0) || (det > maxDet))
    return numberOfHitCluster[0];
  return numberOfHitCluster[det];
}
int& TEvent::getNumberOfHits(int det)
{
  //   int ret=maxHit;
  //   if(det==-1) return ret;
  //   ret=maxDet;
  //   if(det==-2) return ret;
  //   ret=-1;
  if ((det < 0) || (det > maxDet))
    return numberOfHits[0];
  return numberOfHits[det];
}
int& TEvent::getNumberOfClusters(int det)
{
  //   int ret=maxCluster;
  //   if(det==-1)return ret;
  //   ret=-1;
  if ((det < 0) || (det > maxDet))
    return numberOfCluster[0];
  return numberOfCluster[det];
}

int& TEvent::getNumberOfPixels(int det)
{
  //   int ret=maxPixel;
  //   if(det==-1)return ret;
  //   ret=-1;
  if ((det < 0) || (det >= maxDet))
    return numberOfPixels[0];
  return numberOfPixels[det];
}
int TEvent::getTotalNumberOfHitClusters() const
{
  int ret = 0;
  for (int i = 0; i < maxDet; i++)
    ret = ret + numberOfHitCluster[i];
  return ret;
}

int& TEvent::getNumberOfTracks() { return numberOfTracks; }

int& TEvent::getEventNumber() { return eventNumber; }

int& TEvent::getRunNumber() { return runNumber; }

TPixel& TEvent::getPixel(int det, int num)
{
  if (((det < 0) || (det > maxDet)) || ((num < 0) || (num > maxPixel))) {
    return (pix[0][0]);
  }
  return (pix[det][num]);
}

int TEvent::getTotalNumberOfHits() const
{
  int ret = 0;
  for (int i = 0; i < maxDet; i++)
    ret = ret + numberOfHits[i];
  return ret;
}

int TEvent::getTotalNumberOfClusters() const
{
  int ret = 0;
  for (int i = 0; i < maxDet; i++)
    ret = ret + numberOfCluster[i];
  return ret;
}

int TEvent::getTotalNumberOfPixels() const
{
  int ret = 0;
  for (int i = 0; i < maxDet; i++)
    ret = ret + numberOfPixels[i];
  return ret;
}

TCluster& TEvent::getCluster(int det, int num)
{
  if (((det < 0) || (det > maxDet)) || ((num < 0) || (num > maxCluster))) {
    return clust[0][0];
  }
  return clust[det][num];
}

TCalibHit& TEvent::getHit(int det, int num)
{
  if (((det < 0) || (det > maxDet)) || ((num < 0) || (num > maxHit))) {
    return hits[0][0];
  }
  return hits[det][num];
}

TTrack& TEvent::getTrack(int num)
{
  if ((num < 0) || (num > maxTrack)) {
    cerr << "dummy track returned" << endl;
    return trck[0];
  }
  return trck[num];
}
THitCluster& TEvent::getHitCluster(int det, int num)
{
  if (((det < 0) || (det > maxDet)) || ((num < 0) || (num > maxHit))) {
    return hitClus[0][0];
  }
  return hitClus[det][num];
}
void TEvent::reset()
{
  for (int i = 0; i < numberOfDets; i++) {
    for (int j = 0; j < numberOfHits[i]; j++)
      (hits[i][j]).reset();
    for (int j = 0; j < numberOfHitCluster[i]; j++)
      (hitClus[i][j]).reset();
    for (int j = 0; j < numberOfPixels[i]; j++)
      (pix[i][j]).reset();
    for (int j = 0; j < numberOfCluster[i]; j++)
      (clust[i][j]).reset();
    numberOfHits[i] = 0;
    numberOfPixels[i] = 0;
    numberOfCluster[i] = 0;
    numberOfHitCluster[i] = 0;
  }
  for (int i = 0; i < numberOfTracks; i++)
    trck[i].reset();
  numberOfTracks = 0;
  clearReactions();
}
void TEvent::clearReactions()
{
  for (int i = 0; i < numReactions; i++) {
    delete reactions[i];
    reactions[i] = NULL;
  }
  numReactions = 0;
}
int& TEvent::getTrigger() { return trigger; }

vector3D& TEvent::getPolarisation() { return polarisation; }
void TEvent::addReaction(RbaseReaction* r)
{
  if (r == NULL)
    return;
  if (numReactions == maxReactions)
    return;
  if (numReactions == 0) {
    reactions[0] = r;
  } else {
    int pos = numReactions - 1;
    while (pos > 0 && reactions[pos]->getChiSquared() > r->getChiSquared()) {
      reactions[pos + 1] = reactions[pos];
      pos--;
    }
    reactions[pos] = r;
  }
  numReactions++;
}
RbaseReaction* TEvent::getReaction(int num)
{
  if (num < 0 || num >= numReactions)
    return NULL;
  return (reactions[num]);
}
int TEvent::getNumberOfReactions() { return numReactions; }
