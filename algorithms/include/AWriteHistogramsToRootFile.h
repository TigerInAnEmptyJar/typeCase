#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include "AHistoAlgorithm.h"

class AWriteHistogramsToRootFile:public AAlgorithm
{
Q_OBJECT
 private:
    TFile **rootFile;//!
    TDirectory *currentDirectory;//!
    bool own;
    TEvent &event;//!
    TRawHit ***raws;//!
    TCalibHit ***hits;//! [maxDets][maxHits]->
    int **numberOfHits;//! [maxDets]->
    TPixel ***pixels;//! [maxDets][maxPixels]->
    int **numberOfPixels;//! [maxDets]->
    TTrack **tracks;//! [maxTracks]->
    int *numberOfTracks;//! ->
    TH1 **histos;//!
    TH2 **TDhistos;//!
    int numberOfHistos;
    int numberOf2DHistos;
    int *numberOfElements;//![maxDets]
    int numberOfDetectors;
    AHistoAlgorithm **f;//!
    int numF;
    bool server;
    void *histo_mutex;//!
    void declareHistos(TSetup &,const string &);
    void defineData(TEvent &);
 public:
  AWriteHistogramsToRootFile(const string &filename, const string &directory, TEvent &eventIn,TSetup &set,const string &option,bool serv,void *histo_mutexIn);
  AWriteHistogramsToRootFile(TFile **rootFileIn, const string &directory, TEvent &eventIn,TSetup &set,const string &option, bool serv,void *histo_mutexIn);
  virtual ~AWriteHistogramsToRootFile();
  virtual void *process(void*ptr);
  TFile *getFile();
  TH1 *getHisto(const string &name);
  TH1**getHistos1();
  TH2**getHistos2();
  virtual vector<string>histogramNames();
  virtual TH1* histogram(string histoName);
 signals:
  void destroyer(TH1**,TH2**);
  public slots:
    virtual void getDestroyHistos(TH1** ODH, TH2** TDH);
  void setHistos(TH1** h1,TH2 **h2);
};
