#ifndef ATDCCALIBRATION
#define ATDCCALIBRATION
#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
class TCanvas;
#include "CommonCalibrationParser.h"
#include "algorithm.h"
#include "beamtimeparameter.h"
#include <TPostScript.h>
using namespace std;
/************************************************
void ATDCcalibration::doCalibrationOffset()
  bool printAbsolute=(printPattern &1);
  bool printIndividual=(printPattern &2);
void ATDCcalibration::doCalibrationWALK()
  bool printIteration=(printPattern &4);
  bool printBefore=(printPattern &8);
  bool printAfter=(printPattern &16);
  bool printLastIteration=(printPattern &128);
void ATDCcalibration::doCalibrationBeam()
  bool printMe=(printPattern &32);
void ATDCcalibration::doCalibrationGeo()
  bool printMe=(printPattern &64);
void ATDCcalibration::doCalibrationTwoSided()
  bool printMe=(printPattern &256);

 ************************************************/

typedef struct
{
  Float_t pointz[10];
  Float_t tdcs[10];
  Float_t qdcs[10];
  Float_t bt;
  Float_t theta;
  Float_t phi;
  Int_t el[10];
  Int_t det;
  Float_t vt[3];
} trackies_t;
typedef struct
{
  Float_t pointz[2];
  Float_t tdcs[2];
  Float_t qdcs[2];
  Float_t theta;
  Float_t phi;
  Int_t el[2];
  Int_t det;
} track_el_t;
typedef struct
{
  Float_t tdcs[6];
  Float_t qdcs[6];
  Float_t z[5];
  Float_t theta;
  Int_t el[5];
} overlapp_t;
typedef struct
{
  Float_t tdcs[3];
  Float_t qdcs[3];
  Float_t pointx;
  Float_t pointy;
  Float_t pointz;
  Int_t el[3];
  Int_t ID;
} pixies_t;

class ATDCcalibration : public AAlgorithm
{
  Q_OBJECT
private:
  int& eventNumber;
  int& runNumber;
  TTrack** tracks;      //!
  TPixel*** pixels;     //!
  TCalibHit*** hits;    //!
  int& numberOfTracks;  //!
  int** numberOfPixels; //!
  int** numberOfHits;   //!
  TSetup& setup;        //!
  int nCalibDets;
  int* numCalibrations; //[nCalibDets]
  int* calibPos[4];     //
  int* detectors;       //[nCalibDets]
  int* numElements;     //[nCalibDets]
  int* pixelBased;      //[nCalibDets]
  int* twoSided;        //[nCalibDets]
  float* min_maxLRP;    //[nCalibDets*2]
  int* measuresAgainst; //[nCalibDets]
  float* meanOff;       //[nCalibDets*2]
  float* mimaqdc;       //[nCalibDets*2]
  bool* isStop;         //[nCalibDets]
  bool* doLRT;          //[nCalibDets]
  int* pixelDets;       //[nCalibDets]
  int* referencesTo;    //[nCalibDets]
  int* single;          //[nCalibDets]
  int nRefPix;
  int* refPixels;               //[nRefPix]
  string calibrationOutputPath; //!
  string author;                //!
  string beamtime;              //!
  bool eventBased;
  int sumEvents;
  bool killFileOnEnd;                 //!
  CommonCalibrationParser**** params; //!
  int** numRanges;                    //!
  int** actualRanges;                 //!
  int numberOfIterations;
  int printPattern;        //!
  TPostScript* postscript; //!
  bool doWalk;
  bool doOffset;
  bool doGeometry;
  bool doBeam;
  bool doTwoSided;
  bool hasTrack;
  bool hasPix;
  bool readFile;
  string filename; //!
  int nTwoSided;
  int* twoDets;           //!
  TH2F** geoHistos;       //!
  TH1F*** beamHistos;     //!
  TH1*** offsetHistos;    //!
  TH2F*** twoSidedHisto;  //!
  TTree* pixTree;         //!
  TTree* trackTree;       //!
  TTree* elTree;          //!
  TFile* outFile;         //!
  TH2F**** offsetHistos2; //!
  float*** offsets;
  float*** slopes;
  TH1*** walkHistos;       //!
  TH2F** resolutionHisto;  //!
  TH1F** resolutionHisto1; //!
  TF1** walkFitFunctions;  //!
  TF1*** walkFunctions;    //!
  TF1*** walkLines;        //!
  pixies_t outpix;         //!
  trackies_t outtrack[2];  //!
  track_el_t outtrack_el;  //!
  overlapp_t outoverlapp;  //!
  Int_t evtNr;
  int ntrackElstructs;
  int* trackEls[2];
  int runCounter;   //!
  int eventCounter; //!
  int noffsetRanges;
  int* offsetRanges;
  CommonCalibrationParser** offsetParser; //!
  CommonCalibrationParser** outParser;    //!
  void resetWalkHistos();
  void defineWalkStuff(bool justClean, bool redefine, bool mkehistos, int histoptt,
                       bool mkefunctions, float rngmod);
  void defineOffsetStuff(bool justClean, bool redefine, bool mkehistos, bool mkeparser,
                         float rngmod);
  float thMax(int id);
  float thMin(int id);
  void searchParser(const run_parameter& rp);
  float calculateWALK(int detectorNumber, int ElementNumber, float qdc);
  float calculateLRT(int detectorNumber, int ElementNumber, float theta);
  void fillGeoHisto();
  void fillBeamHisto();
  void fillPixTree();
  void fillTrackTree();
  void fillElTree();
  void fillOnlineOffsetHisto();
  void fillOnlineTwoSidedHisto();
  void doCalibrationGeo();
  void doCalibrationBeam();
  void doCalibrationWALK();
  void doCalibrationOffset();
  void doCalibrationTwoSided();
  void calculateOffsets(int ptt);   // float ***offs,float ***slop,int ptt);
  void calculateLines();            // float ***offs,float*** slop);
  void fillTwoSidedHisto(int ptt);  //, float ***offsets, float ***slopes);
  void fillWalkHistos(int pattern); //, float ***offsets, float ***slopes);
  void fillOffsetHistos(/*float ***offsets,float ***slopes,*/ TH1*** abOffsetHistos, int pattern);
  void finalAndReset();
  float getCalibratedTDC(const TRawHit& h, int dn, const point3D& p = point3D());
  float getTDC(int evtnr, int detectorNumber, int elementNumber, float tdc, float qdc, float path,
               int corr /*float **offsets, float **offsetSlope*/, bool hwalk, bool hoff, bool hlrt);
  void findRanges();
  void printWalkHistos(TCanvas* canv, int npads, int& ncanv, int begin, int end);
  void printOffsetHistos(TCanvas* canv, int npads, int& ncanv); //,float*** offs, float ***slop);
  void writeCalibration(CommonCalibrationParser* pa, ostream& out);
  point3D getPoint(volumeShape* sh, const sLine3D& path);
  TH1* getProfile(TH2* histo, string functionName, int parnumber);
  void getProfileFit(TH2* histo, const string& functionName, int parnum, TF1& function,
                     float& value, float& value2, int parnum1, int parnum2);
  void fitTheHisto(TH2* h2d, TH1* h1d, TF1* fitfunc, CommonCalibrationParser* output, int element,
                   bool fixAtMax, int ptt, string opt);

public:
  ATDCcalibration(int& evtnr, int& rnr, TTrack** trIn, TPixel*** pixIn, TCalibHit*** hitIn,
                  int& numTrack, int** numPix, int** numHits, TSetup& setin,
                  const algorithm_parameter& descr);
  ~ATDCcalibration();
  virtual void* process(void*);
  virtual vector<string> treeNames();
  virtual TTree* tree(string treename);
  static algorithm_parameter getDescription();
public slots:
  virtual void onNewRun(run_parameter& rp);
};
#endif
