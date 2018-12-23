#include "calibHit.h"

TCalibHit::TCalibHit():TBase("TCalibHit")
{
    adc = -1;
    tdc = -1;
    defined=false;
    hitShape_clone=NULL;
    hitShape=NULL;
}
TCalibHit::TCalibHit(const TCalibHit &hit):TBase("TCalibHit")
{
    adc = hit.getADC();
    tdc = hit.getTDC();
    raws.setADC(hit.getRawADC());
    raws.setTDC(hit.getRawTDC());
    raws.setElement(hit.getElement());
    raws.setDetector(hit.getDetector());
    defined=true;
    hitShape=hit.getHitShaper();
    hitShape_clone=NULL;
}
TCalibHit::TCalibHit(float ADC, float TDC, const TRawHit &hit):TBase("TCalibHit")
{
  adc=ADC;
  tdc=TDC;
  raws=hit;
  hitShape=NULL;
  hitShape_clone=NULL;
  defined=true;
  valid=true;
}
TCalibHit::TCalibHit(const TRawHit &hit, float ADC, float TDC, volumeShape *sh):TBase("TCalibHit")
{
  adc=ADC;
  tdc=TDC;
  raws=hit;
  hitShape=sh;
  hitShape_clone=sh;
  defined=true;
  valid=(sh!=NULL);
}

void TCalibHit::setHitShape(volumeShape *shapeIn)
{
  hitShape=shapeIn;
  if(hitShape_clone!=NULL) delete hitShape_clone;
  hitShape_clone=NULL;
  defined=true;
}
ostream &operator<<(ostream &o,const TCalibHit &r)
{
    if (o==cout)
    {
      o<<"Det: ";
      o.width(4);
      o<<r.getDetector();
      o<<" Ele: ";
      o.width(4);
      o<<r.getElement();
      o<<" rawADC: ";
      o.width(4);
      o<<r.getRawADC();
      o<<" ADC: ";
      o.width(4);
      o<<r.getADC();
      o<<" rawTDC: ";
      o.width(4);
      o<<r.getRawTDC(); 
      o<<" TDC: ";
      o.width(4);
      o<<r.getTDC(); 
      o<<" shape: "<<r.getHitShaper()->getName().data();
      o<<(r.isValid()?" valid":" not valid");
    }
  else 
    {
      o.width(4);
      o<<r.getDetector();
      o.width(4);
      o<<r.getElement();
      o.width(4);
      o<<r.getRawADC();
      o.width(4);
      o<<r.getADC();
      o.width(4);
      o<<r.getRawTDC(); 
      o.width(4);
      o<<r.getTDC(); 
      o<<(r.isValid()?r.getHitShaper()->getName().data():"none");
    }
    return o;
}
TCalibHit::~TCalibHit()
{
  if(hitShape_clone!=NULL)delete hitShape_clone;
}
float TCalibHit::getADC() const
{
    return adc;
}
float TCalibHit::getTDC() const
{
    return tdc;
}
volumeShape *TCalibHit::getHitShape() //const 
{
  if(hitShape==NULL)
    return NULL;
  else
    {
      if(hitShape_clone==NULL)
	hitShape_clone=hitShape->getClone();
      return hitShape_clone;//((volumeShape*)hitShape.GetObject());
    }
}
void TCalibHit::setADC(float adcIn)
{
    adc = adcIn;
  defined=true;
}
void TCalibHit::setTDC(float tdcIn)
{
    tdc = tdcIn;
  defined=true;
}
float& TCalibHit::getADCr()
{
  defined=true;
    return adc;
}
float& TCalibHit::getTDCr()
{
  defined=true;
    return tdc;
}

volumeShape *TCalibHit::getHitShaper()const
{
  return hitShape;
}

int TCalibHit::getRawADC() const
{
  return raws.getADC();
}

int TCalibHit::getRawTDC() const
{
  return raws.getTDC();
}
int TCalibHit::getDetector() const
{
  return raws.getDetector();
}
int TCalibHit::getElement() const
{
  return raws.getElement();
}
int &TCalibHit::getRawADCr()
{
  defined=true;
  return raws.getADCr();
}
int &TCalibHit::getRawTDCr()
{
  defined=true;
  return raws.getTDCr();
}
int& TCalibHit::getDetectorr()
{
  defined=true;
  return raws.getDetectorr();
}
int &TCalibHit::getElementr()
{
  defined=true;
  return raws.getElementr();
}
bool TCalibHit::isDefined()
{
  return defined;
}
TRawHit &TCalibHit::getRaw()
{
  return raws;
}
bool TCalibHit::isValid()const
{
  return valid;
}
void TCalibHit::setValid(bool val)
{
  valid=val;
}
void TCalibHit::reset()
{
  valid=false;
  raws.setADC(-1);
  raws.setTDC(-1);
  raws.setElement(-1);
  adc=-1;
  tdc=-1;
  setHitShape(NULL);
}
void TCalibHit::setRaw(const TRawHit &r)
{
    raws.setADC(r.getADC());
    raws.setTDC(r.getTDC());
    raws.setElement(r.getElement());
    raws.setDetector(r.getDetector());
}

TCalibHit &TCalibHit::operator=(const TCalibHit& hit)
{
    adc = hit.getADC();
    tdc = hit.getTDC();
    raws.setADC(hit.getRawADC());
    raws.setTDC(hit.getRawTDC());
    raws.setElement(hit.getElement());
    raws.setDetector(hit.getDetector());
    defined=true;
    hitShape=hit.getHitShaper();
    hitShape_clone=NULL;
    return *this;
}
