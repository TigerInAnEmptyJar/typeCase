#include "detectorparameter.h"
detector_parameter::detector_parameter()
{
  mat = NULL;
  circular = false;
}

detector_parameter::~detector_parameter() {}

float detector_parameter::getMaxDistance() const { return maxDist; }

void detector_parameter::setMaxDistance(float value) { maxDist = value; }

int detector_parameter::getNumberOfElements() const { return numberOfElements; }

void detector_parameter::setNumberOfElements(int n) { numberOfElements = n; }

int detector_parameter::getID() const { return ID; }

void detector_parameter::setID(int id) { ID = id; }

int detector_parameter::getStackType() const { return stackType; }

void detector_parameter::setStackType(int s) { stackType = s; }

shape_parameter detector_parameter::getShape() const { return shape; }

void detector_parameter::setShape(const shape_parameter& sh)
{
  shape = sh;
  shape.setCompleteWrite(true);
}

ostream& operator<<(ostream& o, const detector_parameter& d)
{
  o << d.getNumberOfElements() << " " << d.getStackType() << " " << d.getID() << " "
    << d.getMaterial() << " " << d.getMaxDistance() << d.getName().data() << endl;
  shape_parameter sh;
  sh = d.getShape();
  o << sh;
  return o;
}

void detector_parameter::setMaterial(int matIn) { fmaterial = matIn; }

int detector_parameter::getMaterial() const { return fmaterial; }

void detector_parameter::setMaterialId(boost::uuids::uuid id) { _materialId = id; }

boost::uuids::uuid detector_parameter::getMaterialId() const { return _materialId; }

material_parameter* detector_parameter::material() { return mat; }

void detector_parameter::setMaterial(material_parameter* matIn)
{
  mat = matIn;
  if (matIn != nullptr) {
    _materialId = matIn->id();
  }
}

bool detector_parameter::isCircular() const { return circular; }
void detector_parameter::setCircular(bool circ) { circular = circ; }

bool detector_parameter::operator==(detector_parameter const& other) const
{
  bool matEqual = true;
  if (other.mat != nullptr && mat != nullptr) {
    matEqual = (other.mat->id() == mat->id());
  } else if (_materialId.is_nil() || other._materialId.is_nil()) {
    matEqual = (other.fmaterial == fmaterial);
  } else {
    matEqual = (other._materialId == _materialId);
  }
  return other.numberOfElements == numberOfElements && matEqual && other.ID == ID &&
         other.maxDist == maxDist && other.circular == circular && other.stackType == stackType &&
         other.shape == shape && other.id() == id() && other.getName() == getName() &&
         other.getDescription() == getDescription();
}

reaction_parameter::reaction_parameter(const shape_parameter& target, bool twoBeamIn, int first,
                                       int sec)
{
  twoBeams = twoBeamIn;
  firstMat = first;
  secMat = sec;
  targetShape = target;
  firstMom = 0;
  secMom = 0;
}
reaction_parameter::reaction_parameter(const reaction_parameter& r)
{
  twoBeams = r.hasTwoBeams();
  firstMat = r.getMaterial(0);
  firstMom = r.getBeamMomentum(0);
  if (twoBeams) {
    secMat = r.getMaterial(1);
    secMom = r.getBeamMomentum(1);
  } else {
    secMat = r.getTargetMaterial();
    secMom = 0;
  }
  targetShape = r.getTargetShape();
}
reaction_parameter& reaction_parameter::operator=(const reaction_parameter& r)
{
  twoBeams = r.hasTwoBeams();
  firstMat = r.getMaterial(0);
  firstMom = r.getBeamMomentum(0);
  if (twoBeams) {
    secMat = r.getMaterial(1);
    secMom = r.getBeamMomentum(1);
  } else {
    secMat = r.getTargetMaterial();
    secMom = 0;
  }
  targetShape = r.getTargetShape();
  return *this;
}

bool reaction_parameter::operator==(reaction_parameter const& other) const
{
  return twoBeams == other.twoBeams && firstMat == other.firstMat && firstMom == other.firstMom &&
         targetShape == other.targetShape && secMat == other.secMat && secMom == other.secMom;
}

reaction_parameter::~reaction_parameter() {}

bool reaction_parameter::hasTwoBeams() const { return twoBeams; }

void reaction_parameter::setTwoBeams(bool tb) { twoBeams = tb; }

int reaction_parameter::getMaterial(int num) const
{
  if (num == 0)
    return firstMat;
  if (num == 1)
    return secMat;
  return -1;
}

int reaction_parameter::getTargetMaterial() const
{
  if (!twoBeams)
    return secMat;
  return -1;
}

void reaction_parameter::setMaterial(int num, int mat)
{
  if (num == 0)
    firstMat = mat;
  else if (num == 1)
    secMat = mat;
}

void reaction_parameter::setMaterialId(int num, boost::uuids::uuid id)
{
  if (num == 0) {
    _firstMaterialId = id;
  } else if (num == 1) {
    _secondMaterialId = id;
  }
}

boost::uuids::uuid reaction_parameter::getMaterialId(int num) const
{
  if (num == 0) {
    return _firstMaterialId;
  } else if (num == 1) {
    return _secondMaterialId;
  }
  return {};
}

void reaction_parameter::setTargetMaterial(int mat)
{
  if (!twoBeams)
    secMat = mat;
}

float reaction_parameter::getBeamMomentum(int num) const
{
  if (num == 0)
    return firstMom;
  if ((twoBeams) && (num == 1))
    return secMom;
  return -1;
}

void reaction_parameter::setBeamMomentum(float value, int num)
{
  if (num == 0)
    firstMom = value;
  else if ((num == 1) && twoBeams)
    secMom = value;
}

shape_parameter reaction_parameter::getTargetShape() const { return targetShape; }

void reaction_parameter::setTargetShape(const shape_parameter& s) { targetShape = s; }

ostream& operator<<(ostream& o, const reaction_parameter& d)
{
  o << ((d.hasTwoBeams()) ? 1 : 0) << " " << d.getMaterial(0) << " " << d.getMaterial(1) << " "
    << d.getBeamMomentum();
  if (d.hasTwoBeams())
    o << " " << d.getBeamMomentum(1);
  o << d.getName().data() << endl;
  o << d.getDescription() << endl;
  shape_parameter sh;
  sh = d.getTargetShape();
  o << sh;
  return o;
}
