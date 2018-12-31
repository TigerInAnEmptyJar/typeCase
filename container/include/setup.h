#include "detector.h"
#include "geometry.h"
#include "planeShape.h"
#ifndef SETUP
#define SETUP
#include "TBase.h"
class TTarget : public TBase
{
private:
  volumeShape* shape_;
  momentum4D particle_;
  float temperature_;

public:
  TTarget();
  TTarget(momentum4D particle, volumeShape& shape);
  TTarget(const TTarget& t);
  ~TTarget();
  volumeShape getShape() const;
  volumeShape& getShaper();
  void setShape(volumeShape& shape);
  momentum4D getParticle() const;
  momentum4D& getParticler();
  void setParticle(const momentum4D& p);
  float getTemperature() const;
  float& getTemperature();
  void setTemperature(float t);
  void operator=(const TTarget& t);
};

class TBeam : public TBase
{
private:
  momentum4D particle_;
  float emmitance_;
  planeShape beamSpot_;

public:
  TBeam();
  TBeam(momentum4D particle);
  TBeam(const TBeam& b);
  ~TBeam();
  momentum4D getParticle() const;
  momentum4D& getParticler();
  void setParticle(const momentum4D& p);
  planeShape getSpot() const;
  planeShape& getSpotr();
  void setSpot(const planeShape& beamSpot);
  float getEmmitance() const;
  float& getEmmitance();
  void setEmmitance(float emm);
  void operator=(const TBeam& b);
};

class TSetup : public TBase
{
private:
  int numDets;
  TDetector** dets; // TDetector **dets;//[maxDets]-> TRef *dets;
  int numMats;
  TMaterial** mats; // TMaterial **mats; //[maxDets]-> TRef *mats;
  TTarget tar;
  bool collider;
  TBeam beam1, beam2;
  const int maxDets;

public:
  TSetup(int max = 100, bool col = false);
  TSetup(const TSetup& s);
  ~TSetup();
  TDetector getDetector(int num) const;
  TDetector& getDetectorr(int num);
  void addDetector(const TDetector& det);
  void popDetector();
  int getNumberOfDetectors() const;
  int& getNumberOfDetectorsr();
  int getMaxDetectors() const;
  int getNumberOfBeams() const;
  void setNumberOfBeams(int b);
  bool hasTarget() const;
  TBeam getBeam(int num = 0) const;
  TBeam& getBeamr(int num = 0);
  void setBeam(TBeam& b, int num = 0);
  TTarget getTarget() const;
  TTarget& getTargetr();
  void setTarget(TTarget& t);
  int getNumberOfMaterials() const;
  TMaterial& getMaterialr(int num);
  void operator=(const TSetup& s);
};

#endif
