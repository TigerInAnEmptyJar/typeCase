#include "detector.h"
TDetector::TDetector(TMaterial& matIn, int detNumIn) : TBase("TDetector"), detNum(detNumIn)
{
  mat = (&matIn);
  numElements = 0;
  stackType = -1;
  defined = true;
  v_shape = NULL;
  circular = false;
  //   for(int i=0;i<numElements;i++)
  //     v_shape[i]=new volumeShape("none");
}
TDetector::TDetector(const TDetector& det) : TBase("TDetector"), detNum(det.getDetectorNumber())
{
  mat = det.getMaterial();
  numElements = det.getNumberOfElements();
  stackType = det.getStackType();
  defined = true;
  circular = det.isCircular();
  v_shape = new volumeShape*[numElements];
  for (int i = 0; i < numElements; i++) {
    v_shape[i] = NULL;
    if (det.getShape(i) != NULL)
      v_shape[i] = det.getShape(i)->getClone();
  }
}
TDetector::TDetector() : TBase("TDetector"), detNum(-1), v_shape(NULL)
{
  numElements = 0;
  stackType = -1;
  defined = false;
}

TDetector::~TDetector()
{
  if (v_shape != NULL) {
    for (int i = 0; i < numElements; i++) {
      delete v_shape[i];
    }
    delete[] v_shape;
  }
}

int TDetector::getDetectorNumber() const { return detNum; }

int TDetector::getStackType() const { return stackType; }

TMaterial* TDetector::getMaterial() const { return mat; }

void TDetector::setNumberOfElements(int num)
{
  defined = true;
  volumeShape* tmp = NULL;
  if (num < 0)
    return;
  if (v_shape != NULL) {
    for (int i = 1; i < numElements; i++) {
      if (v_shape[i] != NULL)
        delete v_shape[i];
    }
    tmp = v_shape[0];
    delete[] v_shape;
  }
  numElements = num;
  v_shape = new volumeShape*[numElements];
  if (!tmp) {
    for (int i = 0; i < numElements; i++) {
      v_shape[i] = NULL;
    }
  } else {
    for (int i = 0; i < numElements; i++) {
      v_shape[i] = tmp->getNext(i, stackType);
    }
  }
}

void TDetector::setStackType(int st)
{
  defined = true;
  stackType = st;
}

void TDetector::setMaterial(TMaterial& m)
{
  if (m.isDefined())
    defined = true;
  mat = &m;
}

void TDetector::setShapeFirstElement(volumeShape* sh)
{
  defined = true;
  if (numElements) {
    for (int i = 0; i < numElements; i++) {
      if (v_shape[i] != NULL)
        delete v_shape[i];
    }
  }
  v_shape[0] = sh;
  for (int i = 1; i < numElements; i++) {
    v_shape[i] = sh->getNext(i, stackType);
  }
}
void TDetector::setShapes(int n, volumeShape** shapes)
{
  defined = true;
  if (numElements) {
    for (int i = 0; i < numElements; i++) {
      if (v_shape[i] != NULL)
        delete v_shape[i];
    }
    delete[] v_shape;
  }
  numElements = n;
  v_shape = new volumeShape*[numElements];
  for (int i = 0; i < numElements; i++) {
    v_shape[i] = NULL;
    if (shapes[i] != NULL)
      v_shape[i] = shapes[i]->getClone();
  }
}

volumeShape* TDetector::getOverallShape() const
{
  if (v_shape == NULL)
    return NULL;
  if (v_shape[0] == NULL)
    return NULL;
  volumeShape* ret = v_shape[0]->getEnvelope(numElements, stackType);
  return ret;
}

volumeShape* TDetector::getShape(int ElementNumber) const
{
  volumeShape* sh;
  if ((ElementNumber > numElements) || (ElementNumber < 0) || v_shape == NULL) {
    return NULL;
  }
  sh = v_shape[ElementNumber];
  return sh;
}

bool TDetector::isDefined() const { return defined; }

void TDetector::operator=(const TDetector& d)
{
  if (!d.isDefined()) {
    defined = false;
    return;
  }
  defined = true;
  if (numElements) {
    for (int i = 0; i < numElements; i++)
      if (v_shape[i] != NULL)
        delete v_shape[i];
    delete[] v_shape;
  }
  numElements = d.getNumberOfElements();
  stackType = d.getStackType();
  for (int i = 0; i < numElements; i++)
    v_shape[i] = d.getShape(i)->getClone();
  mat = d.getMaterial();
}

int TDetector::getNumberOfElements() const { return numElements; }

int TDetector::getID() const { return detNum; }
void TDetector::setCircular(bool circ) { circular = circ; }
bool TDetector::isCircular() const { return circular; }
