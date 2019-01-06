#include "detector.h"
#include "geometry.h"
#include "planeShape.h"
#ifndef SETUP
#define SETUP
#include "TBase.h"

/*!
 * \brief The TTarget class
 * This is the target class. A target here is defined by a volume shape, a temperature and a
 * 4-momentum for the particle. This class is still incomplete and will be modified, when needed.
 * Like all the other container classes, this class provides no functionality for itself, it has to
 * be filled during the analysis.
 */
class TTarget : public TBase
{
private:
  volumeShape* shape_;
  momentum4D particle_;
  float temperature_;

public:
  /*!
   * \brief TTarget
   * Default constructor
   */
  TTarget();

  /*!
   * \brief TTarget
   * Constructor. Sets the target particles 4-momentum and the volume shape. As shape for the
   * target, a pointer of shape and no copy is saved. So take care, that the object persists in
   * memory and that you delete it after use, if necessary.
   * \param particle
   * \param shape
   */
  TTarget(momentum4D particle, volumeShape& shape);

  /*!
   * \brief TTarget
   * Copy constructor.
   * \param t
   */
  TTarget(const TTarget& t);

  /*!
   * \brief Destructor.
   */
  ~TTarget();

  /*!
   * \brief getShape
   * Returns a copy of the target volume shape.
   * \return
   */
  volumeShape getShape() const;

  /*!
   * \brief getShaper
   * Returns a reference to the target volume shape.
   * \return
   */
  volumeShape& getShaper();

  /*!
   * \brief setShape
   * Sets the target volume shape. Since only a pointer to shape is stored and no copy is made, make
   * sure, that this object persists in memory while it is needed and that it is correctly deleted
   * after use.
   * \param shape
   */
  void setShape(volumeShape& shape);

  /*!
   * \brief getParticle
   * Returns the target-particles 4-momentum.
   * \return
   */
  momentum4D getParticle() const;

  /*!
   * \brief getParticler
   * Returns a reference to the target-particles 4-momentum.
   * \return
   */
  momentum4D& getParticler();

  /*!
   * \brief setParticle
   * Sets the target-particles 4-momentum.
   * \param p
   */
  void setParticle(const momentum4D& p);

  /*!
   * \brief getTemperature
   *
   * \return
   */
  float getTemperature() const;

  /*!
   * \brief getTemperature
   * Returns a reference to the temperature of the target in units of kelvin.
   * \return
   */
  float& getTemperature();

  /*!
   * \brief setTemperature
   * Sets the temperature of the target in units of kelvin.
   * \param t
   */
  void setTemperature(float t);

  /*!
   * \brief operator =
   * Copy operator.
   * \param t
   */
  void operator=(const TTarget& t);
};

/*!
 * \brief The TBeam class
 * This class is describing a particle beam made of particles with a definite 4-momentum. It has am
 * emmitance parameter and a planar shape that describes the beam spot on the target.
 */
class TBeam : public TBase
{
private:
  momentum4D particle_;
  float emmitance_;
  planeShape beamSpot_;

public:
  /*!
   * \brief TBeam
   * Default constructor.
   */
  TBeam();

  /*!
   * \brief TBeam
   * Constructor. Sets the beam particles 4-momentum.
   * \param particle
   */
  TBeam(momentum4D particle);

  /*!
   * \brief TBeam
   * Copy constructor.
   * \param b
   */
  TBeam(const TBeam& b);

  /*!
   * \brief Destructor
   */
  ~TBeam();

  /*!
   * \brief getParticle
   * Returns the beam particles 4-momentum.
   * \return
   */
  momentum4D getParticle() const;

  /*!
   * \brief getParticler
   * Returns a reference to the beam particles 4-momentum.
   * \return
   */
  momentum4D& getParticler();

  /*!
   * \brief setParticle
   * Set the beam particles 4-momentum.
   * \param p
   */
  void setParticle(const momentum4D& p);

  /*!
   * \brief getSpot
   * Returns the planar shape that defines the interaction area of the beam with another beam or
   * target material.
   * \return
   */
  planeShape getSpot() const;

  /*!
   * \brief getSpotr
   * Returns a reference to the beam spot.
   * \return
   */
  planeShape& getSpotr();

  /*!
   * \brief setSpot
   * Sets the planar shape defining the beam spot in the target or the interaction area of two
   * beams.
   * \param beamSpot
   */
  void setSpot(const planeShape& beamSpot);

  /*!
   * \brief getEmmitance
   * Returns a reference to the emmitance parameter.
   * \return
   */
  float getEmmitance() const;

  /*!
   * \brief getEmmitance
   *
   * \return
   */
  float& getEmmitance();

  /*!
   * \brief setEmmitance
   * Sets the emmitance parameter.
   * \param emm
   */
  void setEmmitance(float emm);

  /*!
   * \brief operator =
   * Copy operator.
   * \param b
   */
  void operator=(const TBeam& b);
};

/*!
 * \brief The TSetup class
 */
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
  /*!
   * \brief TSetup
   * Constructor. Sets the maximum number of detectors and materials to be stored (default =100) and
   * weather it is a collider or a fixed target experiment (default = false = fixed target).
   * \param max
   * \param col
   */
  TSetup(int max = 100, bool col = false);

  /*!
   * \brief TSetup
   * Copy constructor.
   * \param s
   */
  TSetup(const TSetup& s);

  /*!
   * \brief Destructor
   */
  ~TSetup();

  /*!
   * \brief getDetector
   * Returns the numth detector of the setup. Returns an undefined detector if num is non-valid.
   * \param num
   * \return
   */
  TDetector getDetector(int num) const;

  /*!
   * \brief getDetectorr
   * Returns a reference to the numth detector of the setup. Returns a reference to a "new" detector
   * if num is non-valid, that has to be taken care of (possible memory leak).
   * \param num
   * \return
   */
  TDetector& getDetectorr(int num);

  /*!
   * \brief addDetector
   * Adds a detector to the list of detectors. Take care, that the detector structure det persists
   * in memory since only a pointer and no copy of det is stored. Has to be deleted after use or
   * handled equivalently.
   * \param det
   */
  void addDetector(const TDetector& det);

  /*!
   * \brief popDetector
   * Removes the last detector from the setup.
   */
  void popDetector();

  /*!
   * \brief getNumberOfDetectors
   * Returns the number of detectors defined for this setup.
   * \return
   */
  int getNumberOfDetectors() const;

  /*!
   * \brief getNumberOfDetectorsr
   * Returns a reference to the number of detectors defined for this setup.
   * \return
   */
  int& getNumberOfDetectorsr();

  /*!
   * \brief getMaxDetectors
   * Returns the maximum number of detectors storable in this setup.
   * \return
   */
  int getMaxDetectors() const;

  /*!
   * \brief getNumberOfBeams
   * Returns the number of beams defined for this setup.
   * \return
   */
  int getNumberOfBeams() const;

  /*!
   * \brief setNumberOfBeams
   * Sets the number of beams.
   * \param b
   */
  void setNumberOfBeams(int b);

  /*!
   * \brief hasTarget
   * Returns true if a target and just one beam have been defined.
   * \return
   */
  bool hasTarget() const;

  /*!
   * \brief getBeam
   * Returns the numth beam on the setup. num is 0 by default. If it is 1 and a target is defined,
   * or it is larger than 1, rubbish is returned.
   * \param num
   * \return
   */
  TBeam getBeam(int num = 0) const;

  /*!
   * \brief getBeamr
   * Returns a reference to the numth beam on the setup. num is 0 by default. If it is 1 and a
   * target is defined, or it is larger than 1, a reference to a "new" beam is returned, that has to
   * be taken care of (possible memory leak).
   * \param num
   * \return
   */
  TBeam& getBeamr(int num = 0);

  /*!
   * \brief setBeam
   * Sets the beam of the setup. The parameter num corresponds to the beam number. This can be 0
   * (default) or 1. It it is 1, the number of beams is set to 2 and the target is undefined.
   * \param b
   * \param num
   */
  void setBeam(TBeam& b, int num = 0);

  /*!
   * \brief getTarget
   * Returns the target defined in the setup. Returns an undefined target, if no target is defined
   * for the setup.
   * \return
   */
  TTarget getTarget() const;

  /*!
   * \brief getTargetr
   * Returns a reference to the target. Returns rubbish if no target is defined.
   * \return
   */
  TTarget& getTargetr();

  /*!
   * \brief setTarget
   * Sets the target of the experiment. The number of beams is set to 1.
   * \param t
   */
  void setTarget(TTarget& t);

  /*!
   * \brief getNumberOfMaterials
   * Returns the number of materials defined for the setup.
   * \return
   */
  int getNumberOfMaterials() const;

  /*!
   * \brief getMaterialr
   * Returns a reference to the numth material defined for the setup.
   * \param num
   * \return
   */
  TMaterial& getMaterialr(int num);

  /*!
   * \brief operator =
   * Copy operator.
   * \param s
   */
  void operator=(const TSetup& s);
};

#endif
