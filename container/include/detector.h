#ifndef DETECTOR
#define DETECTOR
#include "TBase.h"
#include "material.h"
#include "volumeShape.h"

/*!
 * \brief The TDetector class
 */
class TDetector : public TBase
{
private:
  int numElements;
  int stackType;
  bool circular;
  const int detNum;
  volumeShape** v_shape; // volumeShape **v_shape;//[numElements]->
  TMaterial* mat;        // TMaterial *mat;//->
  bool defined;          //!
public:
  /*!
   * \brief TDetector
   * Constructor. Defines the detectors material (Note that just a pointer and no copy of the
   * material is saved), the maximum number of elements and the detector ID.
   * \param matIn
   * \param detNumIn
   */
  TDetector(TMaterial& matIn, int detNumIn);

  /*!
   * \brief TDetector
   * Copy constructor.
   * \param det
   */
  TDetector(const TDetector& det);

  /*!
   * \brief TDetector
   * Default constructor.
   */
  TDetector();

  /*!
   * \brief Destructor
   */
  ~TDetector();

  /*!
   * \brief getDetectorNumber
   * Returns the detector ID.
   * \return
   */
  int getDetectorNumber() const;

  /*!
   * \brief getStackType
   * Returns the stack-type-property. It defines how the elements shapes are generated out of the
   * first one. See the getNext()-method for the corresponding shape.
   * \return
   */
  int getStackType() const;

  /*!
   * \brief getNumberOfElements
   * Returns the number of elements defined for the detector.
   * \return
   */
  int getNumberOfElements() const;

  /*!
   * \brief getID
   * Returns the detectors ID.
   * \return
   */
  int getID() const;

  /*!
   * \brief isCircular
   * Returns true if the first elements shape neighbours the last one. There is no check done if
   * this is really true and the property has to be set by the user.
   * \return
   */
  bool isCircular() const;

  /*!
   * \brief getMaterial
   * Returns a pointer to the material the detector is made up.
   * \return
   */
  TMaterial* getMaterial() const;

  /*!
   * \brief getOverallShape
   * Returns the overall shape of the detector if for the detectors shapes a getEnvelope-routine is
   * correctly implemented. Take care, that this pointer is correctly deleted after use.
   * \return
   */
  volumeShape* getOverallShape() const;

  /*!
   * \brief getShape
   * Returns a pointer to the shape of the ElementNumberth element. Returns NULL if ElementNumber is
   * non-valid. Do not delete this pointer, it is the only one we've got.
   * \param ElementNumber
   * \return
   */
  volumeShape* getShape(int ElementNumber) const;

  /*!
   * \brief setNumberOfElements
   * Sets the number of elements and creates new volume shapes for the detector elements. Advice,
   * first set the number of elements and later the shape of the first element to save new and
   * delete operations.
   * \param num
   */
  void setNumberOfElements(int num);

  /*!
   * \brief setStackType
   * Sets the stack type property of the detector elements. Defines how the shapes of the elements
   * of the detector are generated out of the first one. For additional information see the getNext
   * routine of the corresponding shape.
   * \param st
   */
  void setStackType(int st);

  /*!
   * \brief setMaterial
   * Sets the detectors material. Note that only a pointer is set and no copy is made: the material
   * should persist in memory as long as it is used in the detector.
   * \param m
   */
  void setMaterial(TMaterial& m);

  /*!
   * \brief setShapeFirstElement
   * Sets the shape of the first element. The pointer is saved and for all other elements the
   * element volume shape is generated using the getNext() routine and the stackType property.
   * \param sh
   */
  void setShapeFirstElement(volumeShape* sh);

  /*!
   * \brief setShapes
   * Sets all shapes of all detector-elements in one go. Use this if you don't want to use the
   * getNext()-method, the shapes provide.
   * \param n
   * \param shapes
   */
  void setShapes(int n, volumeShape** shapes);

  /*!
   * \brief setCircular
   * Sets the circular property. This tells weather the last elements neighbours the first one. It
   * is an additional property without any consequences on element shape generation.
   * \param circ
   */
  void setCircular(bool circ = true);

  /*!
   * \brief operator =
   * Copy operator.
   * \param d
   */
  void operator=(const TDetector& d);

  /*!
   * \brief isDefined
   * Returns true if the detector has been modified since its creation.
   * \return
   */
  bool isDefined() const;
};
#endif
