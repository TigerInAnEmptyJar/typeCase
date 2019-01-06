#ifndef PIXEL
#define PIXEL
#include "TBase.h"
#include "calibHit.h"
#include "planeShape.h"

/*!
 * \brief The TPixel class
 */
class TPixel : public TBase
{
private:
  const int numDet;          //  TRef p_shape;
  planeShape* p_shape;       //  TRef *elements;//TRef is a nice idea, if it worked
  TCalibHit** elements;      //[accNum]->
  planeShape* emergencySave; //!
  int accNum;
  int ID;
  float maxChi;
  bool defined; //!
  bool valid;

public:
  /*!
   * \brief TPixel
   * Constructor. Sets the maximum number of elements this pixel consists of. This is 3 by default.
   * \param numDetIn
   */
  TPixel(int numDetIn = 3);

  /*!
   * \brief Destructor.
   */
  ~TPixel();

  /*!
   * \brief getShape
   * Returns a pointer to the planar shape of the pixel.
   * \return
   */
  planeShape* getShape();

  /*!
   * \brief getElement
   * Returns the numth element of the pixel.
   * \param num
   * \return
   */
  TCalibHit getElement(int num) const;

  /*!
   * \brief getElementr
   * Returns a reference tor the numth element of the pixel. This routine is prefered.
   * \param num
   * \return
   */
  TCalibHit& getElementr(int num);

  /*!
   * \brief getNumberOfElements
   * Returns the maximum number of elements storable for this pixel.
   * \return
   */
  int getNumberOfElements() const;

  /*!
   * \brief getNumberOfActualElements
   * Returns the number of detector elements in this pixel.
   * \return
   */
  int getNumberOfActualElements() const;

  /*!
   * \brief setShape
   * Sets the planar shape of the pixel. Warning: Only the pointer is set; no memory of previously
   * devlared shapes is freed.
   * \param sh
   */
  void setShape(planeShape* sh);

  /*!
   * \brief addElement
   * Add a detector element to the pixel. Routine is skipped, if the number of elements exceeds the
   * maximum.
   * \param hit
   */
  void addElement(TCalibHit& hit);

  /*!
   * \brief resetElements
   * Sets the number of elements to zero.
   */
  void resetElements();

  /*!
   * \brief getID
   * Returns the ID of the pixel.
   * \return
   */
  int getID() const;

  /*!
   * \brief setID
   * Sets the pixel ID.
   * \param IDIn
   */
  void setID(int IDIn);

  /*!
   * \brief getMaxChiSquared
   * Retruns a number usable as maximum chi squared in further analysis. obsolete
   * \return
   */
  float getMaxChiSquared() const;

  /*!
   * \brief setMaxChiSquared
   * Sets the maximum chi squared value.
   * \param value
   */
  void setMaxChiSquared(float value);

  /*!
   * \brief isDefined
   * Returns true if the pixel has been modified after creation or reset.
   * \return
   */
  bool isDefined();

  /*!
   * \brief reset
   * Resets the pixel. The valid property is set to false, the number of elements is set to zero.
   * Warning: No pointers are deleted!! This, the user of this class has to do himself.
   */
  void reset();

  /*!
   * \brief operator ==
   * Returns true if this pixel and the other one consist of the same hits.
   * \return
   */
  bool operator==(TPixel&);

  /*!
   * \brief isValid
   * Returns the valid property.
   * \return
   */
  bool isValid() const;

  /*!
   * \brief setValid
   * Sets the valid property of the pixel. If used without parameter, the method sets it to true
   * \param v
   */
  void setValid(bool v = true);
};
#endif
