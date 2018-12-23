#include"rawHit.h"

TRawHit::TRawHit():TBase("TRawHit")
{
  adc=-1;
  tdc=-1;
  detNum=-1;
  elNum=-1;
  defined=false;
}

TRawHit::TRawHit(int detector, int element, int ADC, int TDC):TBase("TRawHit")
{
  adc=ADC;
  tdc=TDC;
  detNum=detector;
  elNum=element;
}

ostream &operator <<(ostream &o,const TRawHit &r)
{
  if (o==cout)
    {
      o<<"Det: ";
      o.width(4);
      o<<r.getDetector();
      o<<" Ele: ";
      o.width(4);
      o<<r.getElement();
      o<<" ADC: ";
      o.width(4);
      o<<r.getADC();
      o<<" TDC: ";
      o.width(4);
      o<<r.getTDC(); 
    }
  else 
    {
      o.width(4);
      o<<r.getDetector();
      o.width(4);
      o<<r.getElement();
      o.width(4);
      o<<r.getADC();
      o.width(4);
      o<<r.getTDC(); 
    }
  return o;
}

TRawHit::~TRawHit()
{
}

int TRawHit::getADC() const
{
  return adc;
}

int TRawHit::getTDC() const
{
  return tdc;
}

int TRawHit::getDetector() const
{
  return detNum;
}

int TRawHit::getElement() const
{
  return elNum;
}

void TRawHit::setADC(int adcIn)
{
  adc=adcIn;
  defined=true;
}

void TRawHit::setTDC(int tdcIn)
{
  tdc=tdcIn;
  defined=true;
}

void TRawHit::setDetector(int detIn)
{
  detNum=detIn;
  defined=true;
}

void TRawHit::setElement(int elIn)
{
  elNum=elIn;
  defined=true;
}

int& TRawHit::getADCr()
{
  return adc;
  defined=true;
}

int& TRawHit::getTDCr()
{
  return tdc;
  defined=true;
}

int& TRawHit::getDetectorr()
{
  return detNum;
  defined=true;
}

int& TRawHit::getElementr()
{
  return elNum;
  defined=true;
}

bool TRawHit::isDefined()
{
  return defined;
}

