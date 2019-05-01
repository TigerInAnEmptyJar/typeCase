#include "setup.h"

void TTarget::operator=(const TTarget& t)
{
  temperature_ = t.getTemperature();
  particle_ = t.getParticle();
  //  shape_=t.getShape();
}

void TBeam::operator=(const TBeam& b)
{
  particle_ = b.getParticle();
  emmitance_ = b.getEmmitance();
  beamSpot_ = b.getSpot();
}

TSetup::TSetup(int max, bool col) : TBase("TSetup"), maxDets(max)
{
  collider = col;
  numDets = 0;
  dets = new TDetector*[maxDets];
}

TSetup::TSetup(const TSetup& s) : TBase("TSetup"), maxDets(s.getMaxDetectors())
{
  dets = new TDetector*[maxDets];
  numDets = 0;
  for (int i = 0; i < s.getNumberOfDetectors(); i++)
    addDetector(s.getDetector(i));
  collider = !(s.hasTarget());
  beam1 = s.getBeam();
  if (collider)
    beam2 = s.getBeam(1);
  else
    tar = s.getTarget();
}

void TSetup::addDetector(const TDetector& det)
{
  if (numDets > maxDets - 1)
    return;
  dets[numDets] = new TDetector(det);
  //  bool isin=false;
  //   for(int i=0;i<numMats;i++)
  //     {
  //       if(mats[i]==dets[num]->getMaterial())
  // 	isin=true;
  //     }
  //   if(!isin)
  //     {
  //       mats[numMats]=dets[num]->getMaterial();
  //       numMats++;
  //     }
  numDets++;
}

void TSetup::operator=(const TSetup& s)
{
  while (numDets > 0)
    popDetector();
  for (int i = 0; i < s.getNumberOfDetectors(); i++)
    addDetector(s.getDetector(i));
  collider = !s.hasTarget();
  beam1 = s.getBeam();
  if (collider)
    beam2 = s.getBeam(1);
  else
    tar = s.getTarget();
}
TTarget::TTarget() : TBase("TTarget") {}

TTarget::TTarget(momentum4D particle, std::shared_ptr<volumeShape> shape)
    : TBase("TTarget"), shape_(shape)
{
  particle_ = particle;
}

TTarget::TTarget(const TTarget& t) : TBase("TTarget"), particle_(t.getParticle())
{
  temperature_ = t.getTemperature();
}

volumeShape TTarget::getShape() const { return *shape_; }

volumeShape& TTarget::getShaper() { return *shape_; }

void TTarget::setShape(std::shared_ptr<volumeShape> shape) { shape_ = shape; }

momentum4D TTarget::getParticle() const { return particle_; }

momentum4D& TTarget::getParticler() { return particle_; }

void TTarget::setParticle(const momentum4D& p) { particle_ = p; }

float TTarget::getTemperature() const { return temperature_; }

float& TTarget::getTemperature() { return temperature_; }

void TTarget::setTemperature(float t) { temperature_ = t; }

TBeam::TBeam() : TBase("TBeam"), beamSpot_("none") {}

TBeam::TBeam(momentum4D particle) : TBase("TBeam"), beamSpot_("none") { particle_ = particle; }

TBeam::TBeam(const TBeam& b) : TBase("TBeam"), beamSpot_(b.getSpot())
{
  particle_ = b.getParticle();
  emmitance_ = b.getEmmitance();
}

TBeam::~TBeam() {}

momentum4D TBeam::getParticle() const { return particle_; }

momentum4D& TBeam::getParticler() { return particle_; }

void TBeam::setParticle(const momentum4D& p) { particle_ = p; }

planeShape TBeam::getSpot() const { return beamSpot_; }

planeShape& TBeam::getSpotr() { return beamSpot_; }

void TBeam::setSpot(const planeShape& beamSpot) { beamSpot_ = beamSpot; }

float TBeam::getEmmitance() const { return emmitance_; }

float& TBeam::getEmmitance() { return emmitance_; }

void TBeam::setEmmitance(float emm) { emmitance_ = emm; }

TSetup::~TSetup()
{
  for (int i = 0; i < numDets; i++)
    delete dets[i];
  delete[] dets;
}

TDetector TSetup::getDetector(int num) const
{
  TDetector d;
  if ((num < 0) || (num > numDets))
    return d;
  return *(dets[num]); //*((TDetector*)dets[num].GetObject());
}

TDetector& TSetup::getDetectorr(int num)
{
  if ((num < 0) || (num > maxDets)) {
    return *dets[0];
  }
  return *(dets[num]); //*((TDetector*)dets[num].GetObject());
}

void TSetup::popDetector()
{
  if (numDets > 0) {
    if (dets[numDets - 1] != NULL)
      delete dets[numDets - 1];
    dets[numDets - 1] = NULL;
    numDets--;
  }
}

int TSetup::getNumberOfDetectors() const { return numDets; }

int& TSetup::getNumberOfDetectorsr() { return numDets; }

int TSetup::getMaxDetectors() const { return maxDets; }

int TSetup::getNumberOfBeams() const { return (collider ? 2 : 1); }

void TSetup::setNumberOfBeams(int b)
{
  if (b == 1)
    collider = false;
  if (b == 2)
    collider = true;
}

bool TSetup::hasTarget() const { return !(collider); }

TBeam TSetup::getBeam(int num) const
{
  if (num == 0)
    return beam1;
  if ((!collider) || (num > 1))
    return TBeam();
  return beam2;
}

TBeam& TSetup::getBeamr(int num)
{
  if (num == 0)
    return beam1;
  if ((!collider) || (num > 1)) {
    return beam2;
  }
  return beam2;
}

void TSetup::setBeam(TBeam& b, int num)
{
  if (num == 0) {
    beam1 = b;
    return;
  }
  if (!collider)
    return;
  if (num > 1)
    return;
  beam2 = b;
}

TTarget TSetup::getTarget() const { return tar; }

TTarget& TSetup::getTargetr() { return tar; }

void TSetup::setTarget(TTarget& t)
{
  if (collider)
    return;
  tar = t;
}

int TSetup::getNumberOfMaterials() const { return numMats; }

TMaterial& TSetup::getMaterialr(int num)
{
  if ((num < 0) || (num > numMats)) {
    return *mats[0];
  }
  return *(mats[num]); //*((TMaterial*)mats[num].GetObject());
}
