#ifndef __DETPARAM
#define __DETPARAM
#include "baseparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"
using namespace std;

/*!
 * \brief The detector_parameter class
 * This class defines the parameters of one detector, the number of elements, it contains, its ID,
 * stacking type, material, and shape.
 */
class detector_parameter : public base_parameter
{
public:
  /*!
   * \brief detector_parameter
   *  Default constructor.
   */
  detector_parameter();

  detector_parameter(detector_parameter const& other) = default;
  detector_parameter(detector_parameter&& other) = default;

  /*!
   * \brief Destructor
   */
  ~detector_parameter();

  /*!
   * \brief getNumberOfElements
   *  Returns the number of elements the detector is made up of.
   * \return
   */
  int getNumberOfElements() const;

  /*!
   * \brief setNumberOfElements
   *  Sets the number of elements used in the detector.
   * \param n
   */
  void setNumberOfElements(int n);

  /*!
   * \brief getStackType
   *  Returns the stacking type of the detectors shapes. This defines how all the other detector
   * elements are generated out of the first one. See the corresponding shapes getNext()-method for
   * details.
   * \return
   */
  int getStackType() const;

  /*!
   * \brief setStackType
   *  Set the stacking type of the detectors shapes. This defines how all the other detector
   * elements are generated out of the first one. See the corresponding shapes getNext()-method for
   * details.
   * \param s
   */
  void setStackType(int s);

  /*!
   * \brief getID
   *  Return the detectors ID.
   * \return
   */
  int getID() const;

  /*!
   * \brief setID
   *  Set the detectors ID.
   * \param id
   */
  void setID(int id);

  /*!
   * \brief isCircular
   *  Returns if the first element of the detector neighbours the last one.
   * \return
   */
  bool isCircular() const;

  /*!
   * \brief setCircular
   *  Sets if the first element of the detector neighbours the last one.
   * \param circ
   */
  void setCircular(bool circ = true);

  /*!
   * \brief getMaxDistance
   *  Returns the maximum distance of this detector to a track to count as hit.
   * \return
   */
  float getMaxDistance() const;

  /*!
   * \brief setMaxDistance
   *  Sets the maximal distance of a track to the detector, for this detector to count as hit.
   * \param value
   */
  void setMaxDistance(float value);

  /*!
   * \brief getShape
   *  Returns the shape-parameter of the first element of the detector.
   * \return
   */
  shape_parameter getShape() const;

  /*!
   * \brief setShape
   *  Sets the shape-parameter for the first elements shape.
   * \param sh
   */
  void setShape(const shape_parameter& sh);

  /*!
   * \brief setMaterial
   *  Sets the used materials number.
   * \param mat
   */
  void setMaterial(int mat);

  /*!
   * \brief getMaterial
   *  Returns the used materials number.
   * \return
   */
  int getMaterial() const;

  /*!
   * \brief setMaterialId
   * Set the uuid for the material the detector is made up.
   * Use this for persistency, when the actual material is not yet available.
   * \param id the id of the detector's material.
   */
  void setMaterialId(boost::uuids::uuid id);

  /*!
   * \brief getMaterialId
   * Returns the uuid of the material the detector is made up.
   * Use this for persistency, when the actual material is not yet available.
   * \return the id of the detector's material.
   */
  boost::uuids::uuid getMaterialId() const;

  /*!
   * \brief material
   *  Returns the pointer to the material-parameter used for this detector.
   * \return
   */
  material_parameter* material();

  /*!
   * \brief setMaterial
   *  Sets the pointer to the material-parameter used for this detector.
   * \param matIn
   */
  void setMaterial(material_parameter* matIn);

  /*!
   * \brief operator =
   *   Copy operator.
   * \param d
   */
  detector_parameter& operator=(const detector_parameter& d) = default;

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the parameter to compare to.
   * \return true if the parameters are identical.
   */
  bool operator==(detector_parameter const& other) const;

private:
  int numberOfElements;
  float maxDist;
  shape_parameter shape;
  int stackType;
  int fmaterial;
  boost::uuids::uuid _materialId;
  int ID;
  bool circular;
  material_parameter* mat;
};

/*!
 * \brief operator <<
 * \param o
 * \param d
 * \return
 */
ostream& operator<<(ostream& o, const detector_parameter& d);

/*!
 * \brief operator >>
 * \param i
 * \param d
 * \return
 */
istream& operator>>(istream& i, detector_parameter& d);

/*!
 * \brief The reaction_parameter class
 */
class reaction_parameter : public base_parameter
{
public:
  /*!
   * \brief reaction_parameter
   *  Constructor. Sets the targets shape and weather it is a collider experiment. The parameters
   * first and second give the material numbers for beam and beam/target material.
   * \param target
   * \param twoBeamIn
   * \param first
   * \param sec
   */
  reaction_parameter(const shape_parameter& target = shape_parameter(), bool twoBeamIn = false,
                     int first = 0, int sec = 0);

  /*!
   * \brief Destructor
   */
  ~reaction_parameter();

  /*!
   * \brief reaction_parameter
   *  Copy constructor.
   * \param r
   */
  reaction_parameter(const reaction_parameter& r);

  /*!
   * \brief operator =
   *  Copy operator
   * \param r
   * \return
   */
  reaction_parameter& operator=(const reaction_parameter& r);

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the parameter to compare to.
   * \return true if the two parameters are identical.
   */
  bool operator==(reaction_parameter const& other) const;

  /*!
   * \brief hasTwoBeams
   *  Returns true if this is a collider experiment.
   * \return
   */
  bool hasTwoBeams() const;

  /*!
   * \brief setTwoBeams
   *  Sets if the experiment is a collinder experiment. For a fixed target experiment do
   * setTwoBeams(false).
   * \param tb
   */
  void setTwoBeams(bool tb = true);

  /*!
   * \brief getMaterial
   *  Returns the material number of the interacting materials. num=0, will be the (first) beam and
   * num=1, will be the target/second beam.
   * \param num
   * \return
   */
  int getMaterial(int num) const;

  /*!
   * \brief getTargetMaterial
   *  Returns the targets material number or -1 if no target has been defined.
   * \return
   */
  int getTargetMaterial() const;

  /*!
   * \brief setMaterial
   *  Sets the material number of the interacting materials. num=0, will be the (first) beam and
   * num=1, will be the target/second beam.
   * \param num
   * \param mat
   */
  void setMaterial(int num, int mat);

  /*!
   * \brief setMaterialId
   * Sets the material uuid of the interacting materials.
   * \param num the index of the beam(s)/target,
   * \param id the material used for the beam(s)/target.
   */
  void setMaterialId(int num, boost::uuids::uuid id);

  /*!
   * \brief getMaterialId
   * Returns the material's uuid of one of the interacting materials.
   * \param num the index of the beam(s)/target.
   * \return the id of the material used for the beam(s)/target.
   */
  boost::uuids::uuid getMaterialId(int num) const;

  /*!
   * \brief setTargetMaterial
   *  Sets the material number of the target if a target is defined.
   * \param mat
   */
  void setTargetMaterial(int mat);

  /*!
   * \brief getBeamMomentum
   *  Returns the momentum of the numth beam/target particle. Returns -1 if num is non-valid.
   * \param num
   * \return
   */
  float getBeamMomentum(int num = 0) const;

  /*!
   * \brief setBeamMomentum
   *  Sets the momentum (units of GeV) of the numth beam. Does nothing, if num is smaller than 0 or
   * larger than 1.
   * \param value
   * \param num
   */
  void setBeamMomentum(float value, int num = 0);

  /*!
   * \brief getTargetShape
   *  Returns the shape parameter for the targets shape.
   * \return
   */
  shape_parameter getTargetShape() const;

  /*!
   * \brief setTargetShape
   *  Sets the shape parameter for the targets shape.
   * \param s
   */
  void setTargetShape(const shape_parameter& s);

private:
  bool twoBeams;
  int firstMat;
  int secMat;
  boost::uuids::uuid _firstMaterialId;
  boost::uuids::uuid _secondMaterialId;
  float firstMom;
  float secMom;
  shape_parameter targetShape;
};

/*!
 * \brief operator <<
 * \param o
 * \param d
 * \return
 */
ostream& operator<<(ostream& o, const reaction_parameter& d);

/*!
 * \brief operator >>
 * \param i
 * \param d
 * \return
 */
istream& operator>>(istream& i, reaction_parameter& d);

#endif
