#ifndef CALIBHIT
#define CALIBHIT
#include "TBase.h"
#include "rawHit.h"
#include "volumeShape.h"

/*!
 * \brief The TCalibHit class
 */
class TCalibHit : public TBase
{
private:
  TRawHit raws;
  float adc;
  float tdc;
  volumeShape* hitShape;       //!
  volumeShape* hitShape_clone; //!
  bool defined;                //!
  bool valid;

public:
  /*!
   * \brief TCalibHit
   * Default constructor.
   */
  TCalibHit();

  /*!
   * \brief TCalibHit
   * uctor. Sets the properties, raw-hit (element, detectorID, QDC, TDC) calibrated QDC and TDC.
   * \param ADC
   * \param TDC
   * \param hit
   */
  TCalibHit(float ADC, float TDC, const TRawHit& hit);

  /*!
   * \brief TCalibHit
   * Constructor. Sets the properties, raw-hit (element, detectorID, QDC, TDC) calibrated
   * QDC and TDC and the shape of the detector element.
   * \param hit
   * \param ADC
   * \param TDC
   * \param sh
   */
  TCalibHit(const TRawHit& hit, float ADC = -1, float TDC = -1, volumeShape* sh = NULL);

  /*!
   * \brief TCalibHit
   * Copy constructor.
   * \param hit
   */
  TCalibHit(const TCalibHit& hit);

  /*!
   * \brief Destructor
   */
  ~TCalibHit();

  /*!
   * \brief getADC
   * Returns the calibrated ADC-value of the hit.
   * \return
   */
  float getADC() const;

  /*!
   * \brief getTDC
   * Returns the TDC of the hit.
   * \return
   */
  float getTDC() const;

  /*!
   * \brief getRawADC
   * Returns the raw, uncalibrated ADC of the hit.
   * \return
   */
  int getRawADC() const;

  /*!
   * \brief getRawTDC
   * Returns the raw, uncalibrated TDC of the hit.
   * \return
   */
  int getRawTDC() const;

  /*!
   * \brief getDetector
   * Retruns the detector ID of the hit.
   * \return
   */
  int getDetector() const;

  /*!
   * \brief getElement
   * Retruns the element number of the hit.
   * \return
   */
  int getElement() const;

  /*!
   * \brief getHitShape
   * Returns a pointer to the shape of the hit detector element. May be modified. Do
   * not delete. It is a copy of the shape stored in every detector. Note: this may
   * be NULL.
   * \return
   */
  volumeShape* getHitShape();

  /*!
   * \brief setADC
   * Sets the ADC-value of the calibrated hit.
   * \param adcIn
   */
  void setADC(float adcIn);

  /*!
   * \brief setTDC
   * Sets the TDC-value of the calibrated hit.
   * \param tdcIn
   */
  void setTDC(float tdcIn);

  /*!
   * \brief setRaw
   * Sets the properties of the raw component, such as raw ADC, raw TDC, detector iD and
   * element number.
   * \param h
   */
  void setRaw(const TRawHit& h);

  /*!
   * \brief setHitShape
   * Sets the hit shape. A copy of this shape is produced, when getHitShape() is called.
   * This copy is a property of the hit and will be deleted, when necessary.
   * \param shapeIn
   */
  void setHitShape(volumeShape* shapeIn);

  /*!
   * \brief operator =
   * Copy operator.
   * \param hit
   * \return
   */
  TCalibHit& operator=(const TCalibHit& hit);

  /*!
   * \brief getADCr
   * Returns a reference to the calibrated ADC-value of the hit.
   * \return
   */
  float& getADCr();

  /*!
   * \brief getTDCr
   * Returns a reference to the TDC of the hit.
   * \return
   */
  float& getTDCr();

  /*!
   * \brief getRawADCr
   * Returns a reference to the raw, uncalibrated ADC of the hit.
   * \return
   */
  int& getRawADCr();

  /*!
   * \brief getRawTDCr
   * Returns a reference to the raw, uncalibrated TDC of the hit.
   * \return
   */
  int& getRawTDCr();

  /*!
   * \brief getDetectorr
   * Retruns a reference to the detector ID of the hit.
   * \return
   */
  int& getDetectorr();

  /*!
   * \brief getElementr
   * Retruns a reference to the element number of the hit.
   * \return
   */
  int& getElementr();

  /*!
   * \brief getHitShaper
   * Returns a pointer to the shape of the hit detector element.
   * May be modified. Note: this may be NULL.
   * \return
   */
  volumeShape* getHitShaper() const;

  /*!
   * \brief isDefined
   * Returns true if the hit was modified after creation.
   * \return
   */
  bool isDefined();

  /*!
   * \brief getRaw
   * Returns the raw hit component of the hit.
   * \return
   */
  TRawHit& getRaw();

  /*!
   * \brief isValid
   * Returns the valid property. This may be set if the hit shape is defined, or the hit
   * is valid in terms of analysis cases.
   * \return
   */
  bool isValid() const;

  /*!
   * \brief setValid
   * Set the valid-property. If called without parameter, by default it is set to true.
   * \param val
   */
  void setValid(bool val = true);

  /*!
   * \brief reset
   * Resets the hit. The valid property is set to false.
   */
  void reset();
};
ostream& operator<<(ostream& o, const TCalibHit& h);
#endif
