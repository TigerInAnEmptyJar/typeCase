#include "algorithm.h"
#include "track.h"
//#include <fstream>
#include <TH2.h>
using namespace std;

class ACompareToGIN : public AAlgorithm
{
private:
  ifstream* input;
  int& eventNumber;
  TTrack** RecTracks;
  const int maxTracks;
  int& numberOfTracks;
  TTrack** InTracks;
  int numberOfInTracks;
  string outFileName;
  int nHistos;
  TH1** histos;
  TH2F histoNTracks;
  TH1F histoDphi;
  TH1F histoDtheta;
  TH1F histoDbeta;
  TH1F histoNotFound;
  TH1F histoDphiPr;
  TH1F histoDthetaPr;
  TH1F histoDbetaPr;
  TH1F histoNotFoundPr;
  TH1F histoDphiN;
  TH1F histoDthetaN;
  TH1F histoDbetaN;
  TH1F histoNotFoundN;
  TH1F histoDphiD;
  TH1F histoDthetaD;
  TH1F histoDbetaD;
  TH1F histoNotFoundD;
  bool valid;
  int numSepDet;
  int* sepDets;
  float maxAngularDist;
  bool toCout;

public:
  ACompareToGIN(TTrack** tracksIn, int& numberOfTracksIn, int& eventNumberIn, int maxTracksIn,
                const algorithm_parameter& descr);
  virtual ~ACompareToGIN();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
  virtual vector<string> histogramNames();
  virtual TH1* histogram(string histoName);
};
