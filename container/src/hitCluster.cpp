#include "hitCluster.h"

THitCluster::THitCluster(int maxElements):TCalibHit(),numElem(maxElements)
{
  hits=new TCalibHit*[maxElements];
  accElem=0;
}
THitCluster::THitCluster(const THitCluster& cls):TCalibHit((const TCalibHit&)cls),numElem(cls.getMaxNumberOfHits())
{
  accElem=0;
  hits=new TCalibHit*[numElem];
  setADC(cls.getADC());
  setTDC(cls.getTDC());
  setRaw(TRawHit(cls.getDetector(),cls.getElement(),cls.getRawADC(),cls.getRawTDC()));
  setHitShape(cls.getHitShaper());
  for(int i=0;i<cls.getNumberOfHits();i++)
    addHit(cls.getHit(i));
}
THitCluster::THitCluster(TCalibHit& input, int maxElements):TCalibHit(input),numElem(maxElements)
{
  hits=new TCalibHit*[maxElements];
  accElem=0;
  addHit(&input);
}
THitCluster::~THitCluster()
{
  delete []hits;
}
int THitCluster::getNumberOfHits() const
{
  return accElem;
}
int THitCluster::getMaxNumberOfHits() const
{
  return numElem;
}
TCalibHit* THitCluster::getHit(int num) const
{
  if(num<0||num>=accElem)return NULL;
  return hits[num];
}
void THitCluster::addHit(TCalibHit *h)
{
  if(h==NULL)return;
  if(accElem>=numElem)return;
  hits[accElem]=h;
  accElem++;
}
void THitCluster::resetHits()
{
  accElem=0;
  setADC(0);
  setTDC(0);
}
void THitCluster::reset()
{
  TCalibHit::reset();
  resetHits();
}
THitCluster &THitCluster::operator =(const THitCluster &hc)
{
  reset();
  setADC(hc.getADC());
  setTDC(hc.getTDC());
  setRaw(TRawHit(hc.getDetector(),hc.getElement(),hc.getRawADC(),hc.getRawTDC()));
  setHitShape(hc.getHitShaper());
  for(int i=0;i<hc.getNumberOfHits();i++)addHit(hc.getHit(i));
  return *this;
}
THitCluster &THitCluster::operator =(const TCalibHit &hc)
{
  
  setADC(hc.getADC());
  setTDC(hc.getTDC());
  setRaw(TRawHit(hc.getDetector(),hc.getElement(),hc.getRawADC(),hc.getRawTDC()));
  setHitShape(hc.getHitShaper());
  setValid(hc.isValid());
  return *this;
}
void THitCluster::operator+=(TCalibHit &hc)
{
  if(accElem>=numElem)return;
  hits[accElem]=&hc;
  accElem++;
}
