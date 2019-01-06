#ifndef RAWHIT
#define RAWHIT
#include <iostream>
using namespace std;
#include "TBase.h"

/*!
 * \brief The TRawHit class
 */
class TRawHit : public TBase
{
private:
  int adc;
  int tdc;
  int detNum;
  int elNum;
  bool defined; //!
public:
  /*!
   * \brief TRawHit
   * Default constructor.
   */
  TRawHit();

  /*!
   * \brief TRawHit
   * Constructor. Sets the properies.
   * \param detector
   * \param element
   * \param ADC
   * \param TDC
   */
  TRawHit(int detector, int element, int ADC, int TDC);

  /*!
   * \brief Destructor
   */
  ~TRawHit();

  /*!
   * \brief getADC
   * Retruns the ADC-value of the raw hit.
   * \return
   */
  int getADC() const;

  /*!
   * \brief getTDC
   * Returns the TDC-value of the raw hit.
   * \return
   */
  int getTDC() const;

  /*!
   * \brief getDetector
   * Returns the detector ID of the hit.
   * \return
   */
  int getDetector() const;

  /*!
   * \brief getElement
   * Returns the element number of the hit.
   * \return
   */
  int getElement() const;

  /*!
   * \brief setADC
   * Sets the ADC-value of the raw hit.
   * \param adcIn
   */
  void setADC(int adcIn);

  /*!
   * \brief setTDC
   * Sets the TDC-value of the raw hit.
   * \param tdcIn
   */
  void setTDC(int tdcIn);

  /*!
   * \brief setDetector
   * Sets the detector ID of the hit.
   * \param detIn
   */
  void setDetector(int detIn);

  /*!
   * \brief setElement
   * Sets the element number of the hit.
   * \param elIn
   */
  void setElement(int elIn);

  /*!
   * \brief getADCr
   * Retruns a reference to the ADC-value of the raw hit.
   * \return
   */
  int& getADCr();

  /*!
   * \brief getTDCr
   * Returns a reference to the TDC-value of the raw hit.
   * \return
   */
  int& getTDCr();

  /*!
   * \brief getDetectorr
   * Returns a reference to the detector ID of the raw hit.
   * \return
   */
  int& getDetectorr();

  /*!
   * \brief getElementr
   * Returns a reference to the element number of the hit.
   * \return
   */
  int& getElementr();

  /*!
   * \brief isDefined
   * Returns true, if the hit was modified after creation.
   * \return
   */
  bool isDefined();
};

/*!
 * \brief operator <<
 * \param o
 * \param r
 * \return
 */
ostream& operator<<(ostream& o, const TRawHit& r);

#endif
