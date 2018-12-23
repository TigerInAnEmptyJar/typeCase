#include "track.h"

TTrack::TTrack(int max, int dets):TBase("TTrack"),maxhits(max), maxDets(dets),particle(0,0,0,0)
{
  defined=false;
  numhits=new int[maxDets];
  //  for(int i=0;i<maxDets;i++)numhits[i]=0;
  calibSrc=new TCalibHit**[maxDets];//TRefArray[maxDets];//(maxhits);
  for(int i=0;i<maxDets;i++)calibSrc[i]=new TCalibHit*[maxhits];
  pixsv=new TPixel*[maxDets];
  pix=new TPixel*[maxDets];
  primary=NULL;
  secondary=new TTrack*[maxDets];
  reset();
}
void TTrack::addCalibSrc(TCalibHit *calibIn)
{
  if(!calibIn)return;
  if(!(calibIn->isValid()))return;
  if(calibIn->getDetector()>=maxDets)return;
  if((numhits[calibIn->getDetector()]>maxhits-1)) return;
  defined=true;
  bool isIn=false;
  for(int i=0;i<numhits[calibIn->getDetector()];i++)
    if(calibSrc[calibIn->getDetector()][i]==calibIn)
      isIn=true;
  if(isIn)return;
  calibSrc[calibIn->getDetector()][numhits[calibIn->getDetector()]]=calibIn;//.Add(calibIn);
  numhits[calibIn->getDetector()]++;
  totnum++;
}
TCalibHit *TTrack::getCalibSrcr(int det, int num)
{
  defined=true;
  if(((det<0)||(det>maxDets))||(( num<0)||(num>numhits[det])))
    {
      TCalibHit *dummy=new TCalibHit();
      return dummy;
    }
  //  TObject *tmp=calibSrc[det].At(num);
  return calibSrc[det][num];//((TCalibHit*)tmp);//.GetObject());
}
TRawHit& TTrack::getRawSrcr(int det, int num)
{
  defined=true;
  if(((det<0)||(det>maxDets))||(( num<0)||(num>numhits[det])))
    {
      TRawHit *dummy=new TRawHit();
      return *dummy;
    }
  return calibSrc[det][num]->getRaw();//((TCalibHit*)calibSrc[det].At(num))->getRaw();//.GetObject())->getRaw();

}
int TTrack::getNumberOfCalibHits(int num) const
{
  if(num==-2)
    return maxDets;
  if(num==-1)
    {
      return totnum;
    }
  if((num<0)||(num>maxDets)) return -1;
  return numhits[num];
}

void TTrack::operator=(TTrack&t)
{
  reset();
  for(int i=0;i<t.getNumberOfCalibHits(-2);i++)
    {
    for(int j=0;j<t.getNumberOfCalibHits(i);j++)
      {
	addCalibSrc(t.getCalibSrcr(i,j));
      }
    }
  for(int i=0;i<t.getNumberOfPixels();i++)
    addPixel(&t.getPixel(0,i));
  setPath(t.getPath());
  setParticle(t.getParticle());
  setChiSquared(t.getChiSquared());
  setSpeed(t.getSpeed());
  setKineticEnergy(t.getKineticEnergy());
  setOrder(t.isSecondary());
  for(int i=0;i<t.getNumberOfSecondaryTracks();i++)
    addSecondary(t.getSecondary(i));
  if(order)setPrimary(t.getPrimary());
  setCharge(t.getCharge());
  setParticleID(t.getParticleID());
}

ostream &operator <<(ostream& o, const TTrack &t)
{
  o<<"("<<t.getPath().Direction().Theta()<<", "<<t.getPath().Direction().Phi()<<"),beta: "<<t.getSpeed()<<" Vertex "<<t.getPath().Foot()<<endl;
  for(int i=0;i<t.getNumberOfCalibHits(-2);i++)
    {
      for(int j=0;j<t.getNumberOfCalibHits(i);j++)
	{
      	  o<<t.getCalibSrc(i,j).getRaw()<<endl;
	}
    }
  return o;
}
TTrack::~TTrack()
{
  for(int i=0;i<maxDets;i++)delete []calibSrc[i];
  delete []calibSrc;
  delete []numhits;
  delete []pix;
  delete []secondary;
}
sLine3D TTrack::getPath() const
{
    return path;
}
TCalibHit TTrack::getCalibSrc(int det, int num) const
{
  if((det<0)||(det>=maxDets)) return TCalibHit();
  if(( num<0)||(num>=numhits[det])) return TCalibHit();
  return *(calibSrc[det][num]);//*((TCalibHit*)calibSrc[det].At(num));//.GetObject());
}
void TTrack::removeCalibSrc(TCalibHit *calibIn)
{
  if(calibIn==NULL)return;
  if(calibIn->getDetector()<0||calibIn->getDetector()>=maxDets)return;
  if(numhits[calibIn->getDetector()]<=0)return;
  int num=-1, det=calibIn->getDetector();
  for(int i=0;i<numhits[det];i++)
    if(calibIn==calibSrc[det][i])
      num=i;
  if(num==-1)return;
  if(num==numhits[det]-1)
    {
      calibSrc[det][num]=NULL;
      totnum--;
      numhits[det]--;
      return;
    }
  for(int i=num;i<numhits[det]-1;i++)
    {
      calibSrc[det][i]=calibSrc[det][i+1];
    }
  numhits[det]--;
  totnum--;
}
momentum4D TTrack::getParticle() const
{
    return particle;
}
void TTrack::setPath(sLine3D pathIn)
{
  defined=true;
    path= pathIn;
}
void TTrack::setParticle(momentum4D partIn)
{
  defined=true;
    particle = partIn;
}
sLine3D& TTrack::getPathr()
{
  defined=true;
    return path;
}
momentum4D& TTrack::getParticler()
{
  defined=true;
    return particle;
}
void TTrack::clearCalibSrc()
{
  for(int i=0;i<maxDets;i++)
    {
      //    calibSrc[i].Clear();
      numhits[i]=0;
    }
  totnum=0;
}

bool TTrack::isDefined()
{
  return defined;
}
void TTrack::addPixel(TPixel *p)
{
  if(numPix>=maxDets-1)return;
  pixsv[numPix]=p;
  pix[numPix]=p;//.Add(p);//[numPix]=p;
  numPix++;
}

int TTrack::getNumberOfPixels()const
{
  return numPix;
}

TPixel& TTrack::getPixel(int det, int num)
{
  if((det<0)||numPix<=num) 
    {
      TPixel *dummy=new TPixel();
      return *dummy;
    }
  return *pixsv[num];//*((TPixel*)pix.At(num));//.GetObject());
}

void TTrack::clearPixel()
{
  numPix=0;
  //  pix.Clear();
}

float TTrack::getChiSquared()
{
  return chiQ;
}

float& TTrack::getChiSquaredr()
{
  return chiQ;
}

void TTrack::setChiSquared(float chi)
{
  chiQ=chi;
}
bool TTrack::isValid()const
{
  return valid;
}

void TTrack::setValid(bool val)
{
  valid=val;
}

float TTrack::getKineticEnergy()const
{
  return kineticEnergy;
}

void TTrack::setKineticEnergy(float val)
{
  kineticEnergy=val;
}

float TTrack::getSpeed()const
{
  return speed;
}

void TTrack::setSpeed(float val)
{
  speed=val;
}
void TTrack::reset()
{
  clearCalibSrc();
  clearPixel();
//   path=sLine3D();
//   particle=momentum4D();
  setOrder(false);
  clearSecondary();
  kineticEnergy=0;
  speed=0;
  chiQ=10000000;
  valid=false;
  partID=-1;
}

bool TTrack::isSecondary()const
{
    return order;
}

void TTrack::setOrder(bool o)
{
    order=o;
}
int TTrack::getNumberOfSecondaryTracks()const
{
  return numSec;
}
TTrack* TTrack::getSecondary(int num)
{
  if(num>=numSec) return NULL;
  return secondary[num];//(TTrack*)secondary.At(num);
}
void TTrack::clearSecondary()
{
  //secondary.Clear();
  numSec=0;
}
void TTrack::addSecondary(TTrack *t)
{
  if(numSec>maxDets-1)return;
  secondary[numSec]=t;//.Add(t);
  numSec++;
}
TTrack* TTrack::getPrimary()
{
  return primary;//.GetObject();
}
void TTrack::setPrimary(TTrack*t)
{
  primary=t;//TRef(t);
}

int TTrack::getCharge()const
{
  return charge;
}
int &TTrack::getCharger()
{
  return charge;
}
void TTrack::setCharge(int c)
{
  charge=c;
}
float TTrack::getErrorPhi()const{return errors[0];}
float TTrack::getErrorTheta()const{return errors[1];}
float TTrack::getErrorSpeed()const{return errors[2];}
float TTrack::getErrorEnergy()const{return errors[3];}
void TTrack::setErrorPhi(float v){errors[0]=v;}
void TTrack::setErrorTheta(float v){errors[1]=v;}
void TTrack::setErrorSpeed(float v){errors[2]=v;}
void TTrack::setErrorEnergy(float v){errors[3]=v;}
void TTrack::setParticleID(int value){partID=value;}
int TTrack::getParticleID()const{return partID;}
