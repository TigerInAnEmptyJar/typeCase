#ifndef ELOSSAL
#define ELOSSAL
#include "algorithm.h"
#include "geometry.h"

/*!
 * \brief The AELossAlgorithm class
 */
class AELossAlgorithm : public AAlgorithm
{
private:
  int num;

public:
  /*!
   * \brief Constructor
   */
  AELossAlgorithm();

  /*!
   * \brief Destructor
   */
  virtual ~AELossAlgorithm();

  /*!
   * \brief process
   * Though this is the base algorithm-method, the main method for the energy-loss-algorithms
   * is energyLoss(). But you can also use it by defining setters and getters for the
   * properties you want to use.
   * \param ptr ignored
   * \return nullptr
   */
  virtual void* process(void* ptr);

  /*!
   * \brief energyLoss
   * Calculates the energy loss for a particle passing a material described by this algorithm
   * on a length of path and returns the result.
   * \param particle the particle at entering the material.
   * \param path the distance taking through the material.
   * \return the energy loss of the particle on its path through the material.
   */
  virtual float energyLoss(momentum4D particle, float path);

  /*!
   * \brief energyLoss
   * \param particle
   * \param path
   * \param charge
   * \return
   */
  virtual float energyLoss(momentum4D particle, float path, float charge);

  /*!
   * \brief energyLossM
   * \param geantId
   * \param momentum
   * \param path
   * \return
   */
  virtual float energyLossM(int geantId, float momentum, float path);

  /*!
   * \brief energyLossB
   * \param geantId
   * \param beta
   * \param path
   * \return
   */
  virtual float energyLossB(int geantId, float beta, float path);

  /*!
   * \brief energyLossE
   * \param geantId
   * \param energy
   * \param path
   * \return
   */
  virtual float energyLossE(int geantId, float energy, float path);

  /*!
   * \brief energyLoss
   * \param path
   * \param mass
   * \param beta
   * \param energy
   * \param charge2
   * \return
   */
  virtual float energyLoss(float path, float mass, float beta, float energy, float charge2);

  /*!
   * \brief energyLoss_photon
   * \param geantId
   * \param energy
   * \param path
   * \return
   */
  virtual float energyLoss_photon(int geantId, float energy, float path);

  /*!
   * \brief penetrationDepth
   * \param particle
   * \param charge
   * \return
   */
  virtual float penetrationDepth(momentum4D particle, double charge);

  /*!
   * \brief betaAfter
   * \param path
   * \param before
   * \param charge
   * \return
   */
  virtual float betaAfter(float path, momentum4D before, float charge);

  /*!
   * \brief betaAfter
   * \param path
   * \param geantId
   * \param betaIn
   * \return
   */
  virtual float betaAfter(float path, int geantId, float betaIn);

  /*!
   * \brief parameters
   * \return
   */
  virtual Vector parameters() const;

  /*!
   * \brief setParameters
   * \param parameter
   */
  virtual void setParameters(const Vector& parameter);

  /*!
   * \brief getClone
   * \return
   */
  virtual AELossAlgorithm* getClone() const;
};

#endif
