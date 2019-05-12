#include "detector.h"
#include "shapeFactory.h"
#include "shapeparameter.h"

TDetector::TDetector(TMaterial& matIn, int detNumIn) : TBase("TDetector"), detNum(detNumIn)
{
  mat = (&matIn);
  numElements = 0;
  stackType = -1;
  defined = true;
  circular = false;
}

TDetector::TDetector(const TDetector& det) : TBase("TDetector"), detNum(det.getDetectorNumber())
{
  mat = det.getMaterial();
  numElements = det.getNumberOfElements();
  stackType = det.getStackType();
  defined = true;
  circular = det.isCircular();
  _shapes.resize(numElements, {});
  for (size_t i = 0; i < numElements; i++) {
    if (det.getShape(i) != nullptr)
      _shapes[i] = std::shared_ptr<volumeShape>{det.getShape(i)->getClone()};
  }
}

TDetector::TDetector() : TBase("TDetector"), detNum(-1)
{
  numElements = 0;
  stackType = -1;
  defined = false;
}

TDetector::~TDetector() = default;

int TDetector::getDetectorNumber() const { return detNum; }

int TDetector::getStackType() const { return stackType; }

TMaterial* TDetector::getMaterial() const { return mat; }

void TDetector::setNumberOfElements(size_t num)
{
  defined = true;
  if (num == 0 || numElements == num)
    return;
  _shapes.resize(num, {});
  auto& shapeFactory = ShapeFactory::getInstance();
  if (descriptionOfFirstElement) {
    if (numElements > 0 && _shapes[0]) {
      for (size_t i = numElements; i < num; i++) {
        _shapes[i] = shapeFactory.createNext(*descriptionOfFirstElement, i);
      }
    }
  }
  numElements = num;
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

void TDetector::setShapeFirstElement(std::shared_ptr<volumeShape> sh)
{
  if (!sh) {
    return;
    }
  defined = true;
  descriptionOfFirstElement = std::make_unique<shape_parameter>(sh->getDescription());
  auto& shapeFactory = ShapeFactory::getInstance();
  if (numElements > 0) {
    _shapes.resize(numElements);
    _shapes[0] = sh;
    for (size_t i = 1; i < numElements; i++) {
      _shapes[i] = shapeFactory.createNext(*descriptionOfFirstElement, i);
    }
  }
}

std::shared_ptr<volumeShape> TDetector::getOverallShape() const
{
  if (numElements == 0 || !_shapes[0]) {
    return nullptr;
  }
  auto& shapeFactory = ShapeFactory::getInstance();
  return shapeFactory.createEnvelope(_shapes[0]->getDescription(), numElements);
}

volumeShape* TDetector::getShape(size_t ElementNumber) const
{
  if ((ElementNumber > numElements)) {
    return nullptr;
  }
  return _shapes[ElementNumber].get();
}

bool TDetector::isDefined() const { return defined; }

void TDetector::operator=(const TDetector& d)
{
  if (!d.isDefined()) {
    defined = false;
    return;
  }
  defined = true;
  _shapes.clear();
  numElements = d.getNumberOfElements();
  stackType = d.getStackType();
  if (numElements > 0) {
    auto& shapeFactory = ShapeFactory::getInstance();
    descriptionOfFirstElement = std::make_unique<shape_parameter>(d.getShape(0)->getDescription());
    for (size_t i = 0; i < numElements; i++)
      _shapes.push_back(shapeFactory.createNext(*descriptionOfFirstElement, i));
  }
  mat = d.getMaterial();
}

size_t TDetector::getNumberOfElements() const { return numElements; }

int TDetector::getID() const { return detNum; }
void TDetector::setCircular(bool circ) { circular = circ; }
bool TDetector::isCircular() const { return circular; }
