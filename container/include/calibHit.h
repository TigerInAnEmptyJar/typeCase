#ifndef CALIBHIT
#define CALIBHIT
#include "volumeShape.h"
#include "rawHit.h"
#include "TBase.h"

class TCalibHit:public TBase
{
 private:
  TRawHit raws;
  float adc;
  float tdc;
  volumeShape *hitShape;//!
  volumeShape *hitShape_clone;//!
  bool defined;//!
  bool valid;
 public:
  TCalibHit();
  TCalibHit(float ADC, float TDC, const TRawHit &hit);
  TCalibHit(const TRawHit &hit, float ADC=-1, float TDC=-1, volumeShape *sh=NULL);
  TCalibHit(const TCalibHit &hit);
  ~TCalibHit();
  float getADC() const;
  float getTDC() const;
  int getRawADC() const;
  int getRawTDC() const;
  int getDetector() const;
  int getElement() const;
  volumeShape *getHitShape();
  void setADC(float adcIn);
  void setTDC(float tdcIn);
  void setRaw(const TRawHit &h);
  void setHitShape(volumeShape *shapeIn);
  TCalibHit &operator=(const TCalibHit& hit);
  float& getADCr();
  float& getTDCr();
  int &getRawADCr();
  int &getRawTDCr();
  int& getDetectorr();
  int &getElementr();
  volumeShape *getHitShaper()const;
  bool isDefined();
  TRawHit &getRaw();
  bool isValid() const;
  void setValid(bool val=true);
  void reset();
};
ostream &operator<<(ostream &o,const TCalibHit &h);
#endif
