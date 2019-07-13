#pragma once
#include "TBase.h"
#include "geometry.h"
#include <string>
class AELossAlgorithm;
using namespace std;

/*!
 * \brief The TMaterial class
 */
class TMaterial : public TBase
{
private:
  const int maxElements;
  int numElements;
  float* masses;  //[numElements] MeV
  float* charges; //[numElements]
  float* weights; //[numElements] -1 == equal parts
  float density;
  float radiationLength;
  float speedOfLight;
  AELossAlgorithm* energyloss; //->
  string name;                 //!
  bool defined;                //!
public:
  /*!
   * \brief TMaterial
   * Constructor. Defines the name of the material and the first element, the material consists of.
   * Default here is proton gas.
   * \param nameIn
   * \param max
   * \param mass
   * \param charge
   * \param weight
   */
  TMaterial(string nameIn = "proton", int max = 100, float mass = 0.93827231, float charge = 1,
            float weight = -1);

  /*!
   * \brief Destructor.
   */
  ~TMaterial();

  /*!
   * \brief getName
   * Returns the name of the material.
   * \return
   */
  string getName() const;

  /*!
   * \brief setName
   * Sets the name of the material.
   * \param n
   */
  void setName(string n);

  /*!
   * \brief getNumberOfElements
   * Returns the actual number of elements stored for this material.
   * \return
   */
  int getNumberOfElements() const;

  /*!
   * \brief getMaxNumberOfElements
   * Returns the maximal number of elements to be stored for this material.
   * \return
   */
  int getMaxNumberOfElements() const;

  /*!
   * \brief getRadiationLength
   * Returns the materials radiation length.
   * \return
   */
  float getRadiationLength() const;

  /*!
   * \brief getDensity
   * Returns the materials density in units of g/cm^3.
   * \return
   */
  float getDensity() const;

  /*!
   * \brief setRadiationLength
   * Sets the materials radiation length.
   * \param rad
   */
  void setRadiationLength(float rad);

  /*!
   * \brief setDensity
   * Sets the density of the material in units of g/cm^3.
   * \param d
   */
  void setDensity(float d);

  /*!
   * \brief getMass
   * Returns the mass (GeV) of the numth element the material consists of.
   * \param num
   * \return
   */
  float getMass(int num = 0) const;

  /*!
   * \brief getCharge
   * Returns the charge of the numth element the material consists of.
   * \param num
   * \return
   */
  float getCharge(int num = 0) const;

  /*!
   * \brief getWeight
   * Returns the relative abundance (0 <= weight <= 1) of an element in the material. If the weight
   * stored in this material is positive, this value is returned. It it is negative, the relative
   * abundance is calculated: (1 - sum(positive))/sum( abs(negative) ) * abs(weight) Returns -1 if
   * num is non valid.
   * \param num
   * \return
   */
  float getWeight(int num = 0) const;

  /*!
   * \brief getMeanMass
   * Returns the mean mass (GeV) of a nucleus in the material using the relative abundance of each
   * element.
   * \return
   */
  float getMeanMass() const;

  /*!
   * \brief getMeanCharge
   * Returns the mean charge of a nucleus in the material using the relative abundance of each
   * element.
   * \return
   */
  float getMeanCharge() const;

  /*!
   * \brief addElement
   * Adds an element to the list of elements with nuclear mass (Gev) and charge and a weight that
   * defines the abundance of the element in the material. If the weight is positive, it gives the
   * relative abundance of this element of all elements in the material. If it is negative, the rest
   * of the sum of the positive weights to one is distributed among all elements with negative
   * weight with the relative abundance = (1 - sum(positive))/sum( abs(negative) ) * abs(weight)
   * \param mass
   * \param charge
   * \param weight
   */
  void addElement(float mass, float charge, float weight = -1);

  /*!
   * \brief setElement
   * Sets the nuclear mass (GeV), charge and weight of the numth element of the material. If the
   * weight is positive, it gives the relative abundance of this element of all elements in the
   * material. If it is negative, the rest of the sum of the positive weights to one is distributed
   * among all elements with negative weight with the relative abundance = (1 - sum(positive))/sum(
   * abs(negative) ) * abs(weight) Does nothing if num is non valid.
   * \param num
   * \param mass
   * \param charge
   * \param weight
   */
  void setElement(int num, float mass, float charge, float weight = -1);

  /*!
   * \brief popElement
   * Removes the last element from the list of elements, the material consists of.
   */
  void popElement();

  /*!
   * \brief getAlgorithm
   * Returns the energy loss algorithm used to calculate the energy loss of some particle in the
   * material.
   * \return
   */
  AELossAlgorithm* getAlgorithm();

  /*!
   * \brief setAlgorithm
   * Sets the energy loss algorihm. This algorithm won't be deleted with the material. Take care it
   * is deleted properly after use.
   * \param energyLoss
   */
  void setAlgorithm(AELossAlgorithm* energyLoss);

  /*!
   * \brief EnergyLoss
   * If the energyloss algorithm is defined, it returns the energyloss of the particle passing the
   * material on a length of path. Not yet defined properly!
   * \param particle
   * \param path
   * \return
   */
  float EnergyLoss(momentum4D particle, float path);

  /*!
   * \brief getSpeedOfLight
   * Returns the speed of light in the material in units of the speed of light in vacuum.
   * \return
   */
  float getSpeedOfLight() const;

  /*!
   * \brief setSpeedOfLight
   * Sets the speed of light in the material in units of the speed of light in vacuum. Should not be
   * larger than 1, by definition.
   * \param val
   */
  void setSpeedOfLight(float val);

  /*!
   * \brief isDefined
   * Returns true, if this material has been defined.
   * \return
   */
  bool isDefined();
};
