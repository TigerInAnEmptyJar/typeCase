#include "algorithm.h"
#include "hitCluster.h"
#include "pixel.h"

class AMicroPixel : public AAlgorithm
{
private:
  const int maxPixel;
  int& numberOfPixels;
  TPixel** pixels; //![maxPixel]->
  int ID;
  int* numHits1;     //!
  int* numHits2;     //!
  TCalibHit** hits1; //![numHits1]->
  TCalibHit** hits2; //![numHits2]->
public:
  AMicroPixel(int IDIn, int maxPixelIn, int& numPixel, TPixel** pixelsIn, int& numHit1In,
              int& numHit2In, TCalibHit** h1, TCalibHit** h2);
  AMicroPixel(int maxPixelIn, int& numPixel, TPixel** pixelsIn, int** numHit, int** numHitC,
              TCalibHit*** hits, THitCluster*** hc, const algorithm_parameter& ap);
  virtual ~AMicroPixel();
  virtual void* process(void* ptr);
};
