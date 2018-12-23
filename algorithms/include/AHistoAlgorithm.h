#ifndef HISTOALGORITHMS
#define HISTOALGORITHMS
#include "algorithm.h"
#include <TH2.h>
#include <string>
#include "TDirectory.h"
#include "container.h"
using namespace std;
class AHistoAlgorithm:public AAlgorithm
{
 protected:
  int numOD;
  int numTD;
  int startO;
  int startT;
  TH1 **ODhistos;//!
  TH2 **TDhistos;//!
  string options;
  TDirectory *dir;//!
  bool justFill;
 public:
  AHistoAlgorithm();
  virtual ~AHistoAlgorithm();
  virtual int getNumberOfHistos(int dim);
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};

class ARawHitHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  int numD;
  int **numberOfHits;//!
  TRawHit ***raws;//!
  int *maxEl;//!
  int *dID;//!
 public:
  ARawHitHistoAlgorithm(TRawHit ***rawHits, int **numHits, TSetup &setup, TDirectory *parent, const string &opt,bool jf);
  virtual ~ARawHitHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
class ARawHitCutHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  int numD;
  int **numberOfHits;//!
  TCalibHit ***hits;//!
  int *maxEl;//!
  int *dID;//!
 public:
  ARawHitCutHistoAlgorithm(TCalibHit ***Hits, int **numHits, TSetup &setup, TDirectory *parent, const string &opt,bool jf);
  virtual ~ARawHitCutHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
class ACalibHitHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  int numD;
  int **numberOfHits;//!
  TCalibHit ***hits;//!
  int *maxEl;//!
  int *dID;//!
 public:
  ACalibHitHistoAlgorithm(TCalibHit ***Hits, int **numHits, TSetup &setup, TDirectory *parent, const string &opt,bool jf);
  virtual ~ACalibHitHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
class APixelHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  int numD;
  int **numberOfPixels;//!
  TPixel ***pixels;//!
  int *pID;//!
 public:
  APixelHistoAlgorithm(TPixel ***Pixel, int **numPix, int maxDet, TDirectory *parent, const string &opt,bool jf);
  virtual ~APixelHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
class ATrackHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  int &numberOfTracks;
  TTrack **tracks;//!
  int *nTracks;//!
  int numD;
  int *st1;//!
  int *st2;//!
 public:
  ATrackHistoAlgorithm(TTrack **Tracks, int &numTracks, TDirectory *parent, const string &opt,bool jf);
  virtual ~ATrackHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
class AReactionHistoAlgorithm:public AHistoAlgorithm
{
 protected:
  TEvent *event;//!
  int *reactions;//!
  int numD;
  int *st1;//!
  int *st2;//!
 public:
  AReactionHistoAlgorithm(TEvent *EventIn, TDirectory *parent, const string &opt,bool jf);
  virtual ~AReactionHistoAlgorithm();
  virtual int declareHistos(TH1 **histos,int start);
  virtual int declare2DHistos(TH2 **histos,int start);
  virtual void *process(void*ptr);
  virtual void write();
};
#endif
