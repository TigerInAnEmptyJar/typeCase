#ifndef RAWHIT
#define RAWHIT 
#include <iostream>
using namespace std;
#include "TBase.h"
class TRawHit:public TBase
{
 private:
  int adc;
  int tdc;
  int detNum;
  int elNum;
  bool defined; //!
 public:
  TRawHit();
  TRawHit(int detector, int element, int ADC, int TDC);
  ~TRawHit();
  int getADC() const;
  int getTDC() const;
  int getDetector() const;
  int getElement() const;
  void setADC(int adcIn);
  void setTDC(int tdcIn);
  void setDetector(int detIn);
  void setElement(int elIn);
  int& getADCr();
  int& getTDCr();
  int& getDetectorr();
  int& getElementr();
  bool isDefined();
};
ostream &operator <<(ostream &o, const TRawHit &r); 

#endif
