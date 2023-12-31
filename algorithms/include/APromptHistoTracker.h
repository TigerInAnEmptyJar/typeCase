#include "algorithm.h"
#include "fitAlgorithm.h"
#include "setup.h"
#include "track.h"
#include <TFormula.h>
#include <TH2.h>
class APromptHistoTracker : public AAlgorithm
{
private:
  const int numDets;
  int* nel; //[numDets]
  int nbinsT;
  int nbinsP;
  float minT;
  float maxT;
  float minP;
  float maxP;
  float* axisT;
  float* axisP;
  char**** Element; //!
  TTrack** tracks;  //!
  int& numberOfTracks;
  const int maxTracks;
  float maxChi;
  TCalibHit*** hits;  //!
  int** numHits;      //!
  TH2C* minElements;  //!
  float** workHisto;  //!
  AFitAlgorithm* fit; //!
  point3D vertex;

public:
  APromptHistoTracker(TSetup& setup, TTrack** tracksIn, int& numTracks, int maxTracksIn,
                      TCalibHit*** hitsIn, int** numHitsIn, bool flat, int minElementsIn,
                      bool outOfSetup, float percentage, bool function, TFormula* f,
                      AFitAlgorithm* fitIn, int binsP, int binsT, float minP, float minT,
                      float maxP, float maxT, bool readFromFile, bool writeToFile, string filename,
                      float macChiIn);
  ~APromptHistoTracker();
  void process();
};
