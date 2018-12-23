#include "calibHit.h"
#include "algorithm.h"
#include "detector.h"

class AGetTheShape:public AAlgorithm
{
 private:
 TCalibHit** calibs;//! [maxDet]->
 int *numberOfHits;//! ->
 TDetector *det;//! ->
 public:
  AGetTheShape(TCalibHit **calibIn, int *numIn, TDetector *detIn);
  virtual ~AGetTheShape();
  virtual void *process(void*ptr);
};
