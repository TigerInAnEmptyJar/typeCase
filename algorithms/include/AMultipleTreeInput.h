#include "algorithm.h"
#include "container.h"
#include <TFile.h>
#include <TLeaf.h>
#include <TLeaf.h>
#include <TObject.h>
#include <TTree.h>
#include <TTree.h>
using namespace std;
class run_parameter;
class AMultipleTreeInput : public AAlgorithm
{
private:
  int& numberOfTracks;
  int& eventNr; //!
  int& runNr;   //!
  int& trgg;    //!
  TTrack** tracks;
  int** numberOfHits;
  TCalibHit*** hits;
  TLeaf**** leaves;
  TTree*** trees;
  TTree* countTree;
  TLeaf** entryNumber;
  TFile* inputFile;
  int nTracksPerTree;
  int nTrees[3];
  Int_t evtNr;
  Int_t numberOf[3];
  bool read_given_event;
  bool& readInValid;
  int currentEntry;
  void readATrack(TTrack* out, TLeaf** in);
  void setTree(const string& filename);
  void cleanUp();

public:
  AMultipleTreeInput(int& eventNumber, int& runNumber, int& trigger, int** numberOfHitsIn,
                     TTrack** tracksIn, int& numberOfTracksIn, TCalibHit*** hitsIn,
                     bool& readInValidIn, const algorithm_parameter& param);
  ~AMultipleTreeInput() override;
  void process() override;
  void OnNewRun(run_parameter& rp);

public:
  static algorithm_parameter getDescription();
};
