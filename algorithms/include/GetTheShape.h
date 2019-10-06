#include "algorithm.h"
#include "calibHit.h"
#include "detector.h"

class AGetTheShape : public AAlgorithm
{
private:
  TCalibHit** calibs; //! [maxDet]->
  int* numberOfHits;  //! ->
  TDetector* det;     //! ->
public:
  AGetTheShape(TCalibHit** calibIn, int* numIn, TDetector* detIn);
  virtual ~AGetTheShape();
  void process() override;
};
