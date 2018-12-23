#ifndef TRACK
#define TRACK
#include "calibHit.h"
#include "rawHit.h"
#include "pixel.h"
#include "geometry.h"
#include "TBase.h"

class TTrack:public TBase
{
 private:
  const int maxhits;
  const int maxDets;
  int totnum;
  int *numhits;//[maxDets]
  sLine3D path;
  TCalibHit ***calibSrc;//!
  TPixel** pix;// TPixel **pix;//[maxDets]->
  TPixel** pixsv;// TPixel **pix;//[maxDets]->
  int numPix;
  momentum4D particle;
  float kineticEnergy;
  float speed;
  bool defined;//!
  float chiQ;
  bool order;
  TTrack* primary; //TTrack* primary;//->
  int numSec;
  TTrack** secondary;//TTrack **secondary;//[numHits]->
  bool valid;
  int charge;
  float errors[4];
  int partID;
 public:
  TTrack(int max=20,int mdet=50);
  ~TTrack();
  sLine3D getPath() const;
  TCalibHit getCalibSrc(int det, int num) const;
  momentum4D getParticle() const;
  void setPath(sLine3D pathIn);
  void addCalibSrc(TCalibHit *calibIn);
  void clearCalibSrc();
  void removeCalibSrc(TCalibHit *calibIn);
  int getNumberOfCalibHits(int num=-1)const;
  void setParticle(momentum4D partIn);
  void addPixel(TPixel *pix);
  void clearPixel();
  int getNumberOfPixels()const;
  TPixel &getPixel(int det, int num);
  sLine3D& getPathr();
  TCalibHit *getCalibSrcr(int det,int num);
  momentum4D& getParticler();
  TRawHit& getRawSrcr(int det, int num);
  bool isDefined();
  float getChiSquared();
  float &getChiSquaredr();
  void setChiSquared(float chi);
  void operator=(TTrack& t);
  bool isValid()const;
  void setValid(bool val=true);
  float getKineticEnergy()const;
  void setKineticEnergy(float val);
  float getSpeed()const;
  void setSpeed(float val);
  bool isSecondary()const;
  void setOrder(bool val=true);
  int getNumberOfSecondaryTracks()const;
  TTrack* getSecondary(int);
  void clearSecondary();
  void addSecondary(TTrack *);
  TTrack* getPrimary();
  void setPrimary(TTrack*);
  void setParticleID(int value);
  int getParticleID()const;
  void reset();
  int getCharge()const;
  int &getCharger();
  void setCharge(int c);
  float getErrorPhi()const;
  float getErrorTheta()const;
  float getErrorSpeed()const;
  float getErrorEnergy()const;
  void setErrorPhi(float v);
  void setErrorTheta(float v);
  void setErrorSpeed(float v);
  void setErrorEnergy(float v);
};
ostream &operator <<(ostream& o, const TTrack &t);

#endif
