#include "algorithm.h"
#include "pixel.h"
#include "fiber.h"
#include "hitCluster.h"
#include <TF1.h>
class AHodoPixel:public AAlgorithm
{
 private:
  int &numPixel;
  const int maxPixel;
  TPixel **Pixels;//[maxPixel]->
  const int maxHits;
  float maxChi;
  int *numHits1;//!
  int *numHits2;//!
  TCalibHit **hit1;//[maxHits]->
  TCalibHit **hit2;//[maxHits]->
  point3D projectionPoint;//!
  point3D planePoint;//!
  vector3D planeNormal;//!
  bool useModulation;
  TF1 *modulationFunction;
  bool useMiddlePlane;
  int ID;
 public:
  AHodoPixel(int IDIn,float maxChiIn, int maxPix, int& numPix, TPixel **pixIn, int maxHit, int &numHit1, int &numHit2, TCalibHit **hits1, TCalibHit **hits2, bool middlePlane=true, point3D planeP=point3D(0,0,17.45), point3D ProjectionPoint=point3D(0,0,0), vector3D normal=vector3D(0,0,1)); 
  AHodoPixel(int maxPix, int& numPix, TPixel **pixIn, int **numberOfHits, TCalibHit ***hits,int **numberOfHitsClusters, THitCluster ***hitClusters,const algorithm_parameter& ap); 
  virtual ~AHodoPixel();
  virtual void *process(void*ptr);
};
