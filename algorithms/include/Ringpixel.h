#ifndef RINGPIXEL
#define RINGPIXEL
#include "algorithm.h"
#include "detector.h"
#include "pixel.h"
class ARingpixel : public AAlgorithm
{
private:
  bool Parallelprojection;
  bool Specialprojectionplane;
  bool Originforprojectionpoint;
  bool Targetforprojectionpoint;
  bool Usetimedifference;
  bool Correctlightflighttime;
  bool Triangleapproximation;
  point3D projectionpoint;          //!
  point3D pointofprojectionplane;   //!
  vector3D normaltoprojectionplane; //!
  int minimumdeltaphi;
  int maximumdeltaphi;
  int minimumnumberofelements;
  int numberOfWedges;
  int numberOfSpirals;
  int numberOfSpiralSpiralCrossings;
  int pixelID;
  const float maxChi;

  TCalibHit** rightElements;    //[NumberOfRightElements]->
  TCalibHit** leftElements;     //[NumberOfLeftElements]->
  TCalibHit** straightElements; //[NumberOfStraightElements]->
  int& NumberOfRightElements;
  int& NumberOfLeftElements;
  int& NumberOfStraightElements;
  float deltaS;
  float deltaL;
  float deltaR;
  float bending;
  float outerR;
  float innerR;
  float coverage;
  float phiR;
  float maxDeltaT;
  float centerPhi;

  int* NumberOfPixels; //->
  TPixel** Pixels;     //[maxOfPixels]->
  const int maxOfPixels;
  vector3D zeroPhiDir; //!
public:
  ARingpixel(bool ParallelprojectionIn, bool SpecialprojectionplaneIn,
             bool OriginforprojectionpointIn, bool TargetforprojectionpointIn,
             bool UsetimedifferenceIn, bool CorrectlightflighttimeIn, bool TriangleapproximationIn,
             point3D projectionpointIn, point3D pointofprojectionplaneIn,
             vector3D normaltoprojectionplaneIn, TDetector& detS, TDetector& detL, TDetector& detR,
             int minimumdeltaphiIn, int maximumdeltaphiIn, int minimumnumberofelementsIn,
             int numberOfSpiralSpiralCrossingsIn, int pixelIDIn, float maxChiIn, float maxDeltaTIn,
             int* NumberOfPixelsIn, TCalibHit** straightE, TCalibHit** leftE, TCalibHit** rightE,
             int& nStraight, int& nLeft, int& nRight, TPixel** PixelsIn, int maxOfPixelsIn);
  virtual ~ARingpixel();
  planeShape* calculateCenter(int r, int l, int g, int dPhi, int rp);
  planeShape* triangleApprox(int r, int l, int g, int dPhi, int rp);
  virtual void* process(void* ptr);
};
#endif
