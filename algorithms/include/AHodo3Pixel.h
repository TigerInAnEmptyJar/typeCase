#include "algorithm.h"
#include "hitCluster.h"
#include "pixel.h"
#include "setup.h"
using namespace std;
class AHodo3Pixel : public AAlgorithm
{
private:
  TPixel** pixel;      //!
  int& numberOfPixels; //!
  const int maxOfPixels;
  int* numberOfHitsX; //!
  int* numberOfHitsY; //!
  int* numberOfHitsD; //!
  TCalibHit** hitsX;  //!
  TCalibHit** hitsY;  //!
  TCalibHit** hitsD;  //!
  vector3D dir1;
  vector3D dir2;
  vector3D normal;
  point3D corner;

public:
  AHodo3Pixel(TPixel** pixelIn, int& numberOfPixelsIn, int maxOfPixelsIn, int& numberOfHitsXIn,
              int& numberOfHitsYIn, int& numberOfHitsDIn, TCalibHit** hitsXIn, TCalibHit** hitsYIn,
              TCalibHit** hitsDIn, TDetector& dx, TDetector& dy, TDetector& dz);
  AHodo3Pixel(TPixel** pixelIn, int& numberOfPixelsIn, int maxOfPixelsIn, int** numberOfHitsIn,
              int** numberOfHitClustersIn, TCalibHit*** hitsIn, THitCluster*** hitClusters,
              TSetup& setup, const algorithm_parameter& ap);
  virtual ~AHodo3Pixel();
  virtual void* process(void* ptr);
};
