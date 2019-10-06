#ifndef HISTOALGORITHMS
#define HISTOALGORITHMS
#include "TDirectory.h"
#include "algorithm.h"
#include "container.h"
#include <TH2.h>
#include <string>
using namespace std;
class AHistoAlgorithm : public AAlgorithm
{
protected:
  int numOD;
  int numTD;
  int startO;
  int startT;
  TH1** ODhistos; //!
  TH2** TDhistos; //!
  string options;
  TDirectory* dir; //!
  bool justFill;

public:
  AHistoAlgorithm();
  ~AHistoAlgorithm() override;
  virtual int getNumberOfHistos(int dim);
  virtual int declareHistos(TH1** histos, int start);
  virtual int declare2DHistos(TH2** histos, int start);
  void process() override;
  virtual void write();
};

class ARawHitHistoAlgorithm : public AHistoAlgorithm
{
protected:
  int numD;
  int** numberOfHits; //!
  TRawHit*** raws;    //!
  int* maxEl;         //!
  int* dID;           //!
public:
  ARawHitHistoAlgorithm(TRawHit*** rawHits, int** numHits, TSetup& setup, TDirectory* parent,
                        const string& opt, bool jf);
  ~ARawHitHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
class ARawHitCutHistoAlgorithm : public AHistoAlgorithm
{
protected:
  int numD;
  int** numberOfHits; //!
  TCalibHit*** hits;  //!
  int* maxEl;         //!
  int* dID;           //!
public:
  ARawHitCutHistoAlgorithm(TCalibHit*** Hits, int** numHits, TSetup& setup, TDirectory* parent,
                           const string& opt, bool jf);
  ~ARawHitCutHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
class ACalibHitHistoAlgorithm : public AHistoAlgorithm
{
protected:
  int numD;
  int** numberOfHits; //!
  TCalibHit*** hits;  //!
  int* maxEl;         //!
  int* dID;           //!
public:
  ACalibHitHistoAlgorithm(TCalibHit*** Hits, int** numHits, TSetup& setup, TDirectory* parent,
                          const string& opt, bool jf);
  ~ACalibHitHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
class APixelHistoAlgorithm : public AHistoAlgorithm
{
protected:
  int numD;
  int** numberOfPixels; //!
  TPixel*** pixels;     //!
  int* pID;             //!
public:
  APixelHistoAlgorithm(TPixel*** Pixel, int** numPix, int maxDet, TDirectory* parent,
                       const string& opt, bool jf);
  ~APixelHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
class ATrackHistoAlgorithm : public AHistoAlgorithm
{
protected:
  int& numberOfTracks;
  TTrack** tracks; //!
  int* nTracks;    //!
  int numD;
  int* st1; //!
  int* st2; //!
public:
  ATrackHistoAlgorithm(TTrack** Tracks, int& numTracks, TDirectory* parent, const string& opt,
                       bool jf);
  ~ATrackHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
class AReactionHistoAlgorithm : public AHistoAlgorithm
{
protected:
  TEvent* event;  //!
  int* reactions; //!
  int numD;
  int* st1; //!
  int* st2; //!
public:
  AReactionHistoAlgorithm(TEvent* EventIn, TDirectory* parent, const string& opt, bool jf);
  ~AReactionHistoAlgorithm() override;
  int declareHistos(TH1** histos, int start) override;
  int declare2DHistos(TH2** histos, int start) override;
  void process() override;
  void write() override;
};
#endif
