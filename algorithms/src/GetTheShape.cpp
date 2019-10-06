#include "GetTheShape.h"
#include "logger.h"
AGetTheShape::AGetTheShape(TCalibHit** calibIn, int* numIn, TDetector* detIn)
    : AAlgorithm("Get the Shapes")
{
  calibs = calibIn;
  numberOfHits = numIn;
  det = detIn;
}

AGetTheShape::~AGetTheShape() {}

void AGetTheShape::process()
{
  int nEl;
  int maxEl = det->getNumberOfElements();
  if (maxEl == 0)
    return;
  for (int i = 0; i < *numberOfHits; i++) {
    nEl = calibs[i]->getElement();
    if ((nEl < 0) || (nEl >= maxEl) || (!(calibs[i]->isValid()))) {
      calibs[i]->setValid(false);
      continue;
    }
    volumeShape* tmp;
    tmp = det->getShape(nEl);
    if (tmp->getName() != "none")
      calibs[i]->setValid(calibs[i]->isValid());
    else
      calibs[i]->setValid(false);
    calibs[i]->setHitShape(tmp);
  }
}
