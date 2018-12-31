#ifndef BARRELPIXEL
#define BARRELPIXEL
#include "algorithm.h"
#include "detector.h"
#include "pixel.h"
class run_parameter;
#include "CommonCalibrationParser.h"
using namespace std;

class ABarrelPixel : public AAlgorithm
{
  Q_OBJECT
private:
  const int maxPixels;
  int ID;
  int& numberOfPixels; //!
  TPixel** pixels;     //![maxPixels]->
  int& numberOfHits1;  //!
  int& numberOfHits2;  //!
  TCalibHit** hits1;   //![numberOfHits1]->
  TCalibHit** hits2;   //![numberOfHits2]->
  planeShape* getShape(int, float, float);
  float lightSpeed, ps;
  int &eventNumber, &runNumber, NumEl; //!
  CommonCalibrationParser** zParams;   //![maxPixels][6][numRanges]
  int numRanges;
  int actualRange;
  int** range;    //![4][numRanges]
  point3D center; //
  vector3D lpe;   //
  vector3D norm;  //
  float phiRange;
  float radius, radiusIn, radiusMid;
  float dB;
  float coneLength;

public:
  ABarrelPixel(int maxPixelsIn, int IDIn, int& numPixels, TPixel** pixelsIn, int& numHits1,
               int& numHits2, TCalibHit** h1, TCalibHit** h2, TDetector* front, float pixelSizeIn,
               int& eventNr, int& runNr, const vector<string>& fileNames);
  virtual ~ABarrelPixel();
  virtual void* process(void* ptr);
public slots:
  void onNewRun(run_parameter& rp);
};
#endif
