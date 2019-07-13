#ifndef __MATPARAM
#define __MATPARAM
#include "baseparameter.h"
using namespace std;

/*!
 * \brief The element_parameter class
 */
class element_parameter : public base_parameter
{
private:
  float mass;
  float charge;
  float weight;

public:
  /*!
   * \brief element_parameter
   *  Constructor. Sets the mass m (units of GeV), the charge c, the weight w and the name n of the
   * element. The proton is given as default.
   * \param m
   * \param c
   * \param w
   * \param n
   */
  element_parameter(float m = 0.938, float c = 1, float w = -1, string n = "proton");

  /*!
   * \brief element_parameter
   *  Copy constructor.
   * \param e
   */
  element_parameter(const element_parameter& e);

  /*!
   * \brief ~element_parameter
   */
  virtual ~element_parameter();

  /*!
   * \brief operator =
   *  Copy operator.
   * \param e
   * \return
   */
  element_parameter& operator=(const element_parameter& e);

  /*!
   * \brief operator ==
   *  Comparison operator
   * \param e
   * \return
   */
  bool operator==(const element_parameter& e) const;

  /*!
   * \brief getMass
   *  Returns the mass (units of GeV) of the element.
   * \return
   */
  float getMass() const;

  /*!
   * \brief getCharge
   *  Returns the charge of the element.
   * \return
   */
  float getCharge() const;

  /*!
   * \brief getWeight
   *  Returns the weight of the element in a material. If the weight is positive, this value is used
   * as relative abundance. It it is negative, the relative abundance is calculated: (1 -
   * sum(positive))/sum( abs(negative) ) * abs(weight)
   * \return
   */
  float getWeight() const;

  /*!
   * \brief setMass
   *  Sets the mass (units of GeV) of the element.
   * \param m
   */
  void setMass(float m);

  /*!
   * \brief setCharge
   *  Sets the charge of the element.
   * \param c
   */
  void setCharge(float c);

  /*!
   * \brief setWeight
   *  Sets the weight of the element in a material. If the weight is positive, this value is used as
   * relative abundance. It it is negative, the relative abundance is calculated: (1 -
   * sum(positive))/sum( abs(negative) ) * abs(weight)
   * \param w
   */
  void setWeight(float w);
};

/*!
 * \brief operator <<
 * \param o
 * \param ep
 * \return
 */
ostream& operator<<(ostream& o, const element_parameter& ep);

/*!
 * \brief The material_parameter class
 */
class material_parameter : public base_parameter
{
private:
  // particle p;
  bool active;
  float radiationLength; //  in mm
  float density;         //  in units of gramms per cubic centimeter
  float speed;
  vector<element_parameter> elements;
  vector<float> energyloss;

public:
  /*!
   * \brief material_parameter
   *  Default constructor.
   */
  material_parameter();

  /*!
   * \brief material_parameter
   *  Copy constructor.
   * \param m
   */
  material_parameter(const material_parameter& m);

  /*!
   * \brief Destructor
   */
  ~material_parameter();

  /*!
   * \brief operator =
   *  Copy operator.
   * \param m
   * \return
   */
  material_parameter& operator=(const material_parameter& m);

  /*!
   * \brief IsActive
   *  Returns true if this is an active detecting material.
   * \return
   */
  bool IsActive() const;

  /*!
   * \brief EnergyLoss
   *  Returns the numth parameters value for the energy loss algorithm.
   * \param num
   * \return
   */
  float EnergyLoss(int num) const;

  /*!
   * \brief Density
   *  Returns the density of the material in units of g/cm^3.
   * \return
   */
  float Density() const;

  /*!
   * \brief RadiationLength
   *  Returns the radiation length in the material.
   * \return
   */
  float RadiationLength() const;

  /*!
   * \brief Element
   *  Returns the numth element defined for the material. Returns a proton if num is non-valid.
   * \param num
   * \return
   */
  element_parameter Element(int num) const;

  /*!
   * \brief addElement
   *  Adds an element to the list of elements in the material.
   * \param ep
   */
  void addElement(const element_parameter& ep);

  /*!
   * \brief setElement
   *  Set the posth element to ep.
   * \param pos
   * \param ep
   */
  void setElement(int pos, const element_parameter& ep);

  /*!
   * \brief NumberOfElements
   *  Returns the number of elements used for the material.
   * \return
   */
  int NumberOfElements() const;

  /*!
   * \brief deleteElement
   *  Removes the numth element from the list of elements for the material.
   * \param num
   */
  void deleteElement(int num);

  /*!
   * \brief NumberOfEnergyLossParams
   *  Returns the number of parameters for the energy loss algorithm.
   * \return
   */
  int NumberOfEnergyLossParams() const;

  /*!
   * \brief ClearEnergyLoss
   *  Clears the parameters for the energy loss algorithm.
   */
  void ClearEnergyLoss();

  /*!
   * \brief setActive
   *  Set if the material is an active detector material. Usable for monte-carlo-simulations.
   * \param is
   */
  void setActive(bool is);

  /*!
   * \brief setEnergyLoss
   *  Sets the parameter num for the energy loss algorithm.
   * \param num
   * \param value
   */
  void setEnergyLoss(int num, float value);

  /*!
   * \brief setDensity
   *  Sets the density of the material in units of g/cm^3.
   * \param value
   */
  void setDensity(float value);

  /*!
   * \brief setRadiationLength
   *  Sets the radiation length in the material.
   * \param value
   */
  void setRadiationLength(float value);

  /*!
   * \brief Speed
   *  Returns the speed of light in the material in units of the speed of light in vacuum. This
   * should not exceed 1.
   * \return
   */
  float Speed() const;

  /*!
   * \brief setSpeed
   *  Sets the speed of light in the material in units of the speed of light in vacuum. This should
   * not exceed 1.
   * \param val
   */
  void setSpeed(float val);

  /*!
   * \brief operator <
   *  Coparison operator. Returns true if the name of this material is smaller the one of material
   * p. Needed for the sorting algorithm of the root-TObjArray.
   * \param p
   * \return
   */
  bool operator<(const material_parameter& p);

  /*!
   * \brief operator >
   *  Coparison operator. Returns true if the name of this material is larger the one of material p.
   * Needed for the sorting algorithm of the root-TObjArray.
   * \param p
   * \return
   */
  bool operator>(const material_parameter& p);

  /*!
   * \brief operator ==
   *  Coparison operator. Returns true if the name of this material is equal the one of material p.
   * Needed for the sorting algorithm of the root-TObjArray.
   * \param p
   * \return
   */
  bool operator==(const material_parameter& p) const;

  /*!
   * \brief operator <=
   *  Coparison operator. Returns true if the name of this material is smaller or equal the one of
   * material p. Needed for the sorting algorithm of the root-TObjArray.
   * \param p
   * \return
   */
  bool operator<=(const material_parameter& p);

  /*!
   * \brief operator >=
   *  Coparison operator. Returns true if the name of this material is larger or equal the one of
   * material p. Needed for the sorting algorithm of the root-TObjArray.
   * \param p
   * \return
   */
  bool operator>=(const material_parameter& p);
};

/*!
 * \brief operator >>
 * \param i
 * \param m
 * \return
 */
istream& operator>>(istream& i, material_parameter& m);

/*!
 * \brief operator <<
 * \param o
 * \param m
 * \return
 */
ostream& operator<<(ostream& o, const material_parameter& m);

#endif
