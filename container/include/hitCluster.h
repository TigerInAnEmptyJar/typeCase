#ifndef HITCLUSTER
#define HITCLUSTER
#include "geometry.h"
#include "calibHit.h"
using namespace std;
class THitCluster:public TCalibHit
{
 private:
  const int numElem;
  int accElem;
  TCalibHit **hits;//!
 public:
  THitCluster(int maxElements=50);
  THitCluster(const THitCluster& cls);
  THitCluster(TCalibHit& input, int maxElements=50);
  virtual ~THitCluster();
  int getNumberOfHits() const;
  int getMaxNumberOfHits() const;
  TCalibHit* getHit(int num) const;
  void addHit(TCalibHit *h);
  void resetHits();
  virtual void reset();
  THitCluster &operator =(const THitCluster &hc);
  THitCluster &operator =(const TCalibHit &hc);
  void operator+=(TCalibHit &hc);
};
#endif
