#include <TObject.h>
typedef struct
{
  Double_t vertex[3];
  Double_t fitted[3];
  Int_t el[30];
  Int_t det[30];
  Double_t phi;
  Double_t theta;
  Double_t beta;
  Double_t energy;
  Double_t momentum;
  Int_t ID;
  Int_t nEl;
} trackT;
#include "algorithm.h"
#include "container.h"
#include <TTree.h>
class run_parameter;
using namespace std;

class AMultipleTreeOutput : public AAlgorithm
{
private:
  const int maxtrees;   //!
  TTrack** tracks;      //!
  int& numberOfTracks;  //!
  int& eventNr;         //!
  int& runNr;           //!
  int& trgg;            //!
  TTree* countTree;     //!
  TTree*** trees;       //!
  trackT*** trackOut;   //!
  TFile* outFile;       //!
  int nTrees[3];        //!
  Int_t nTracks;        //!
  Int_t eventNumber;    //!
  Int_t runNumber;      //!
  Int_t trigger;        //!
  Int_t nPTracks;       //!
  Int_t nVTracks;       //!
  Int_t nKTracks;       //!
  Int_t** entryNumbers; //!
  int nTracks_per_tree; //!
  bool newFileEachRun;  //!
  string filePattern;   //!
  void initNumTree();
  void cleanTree();
  void addATree(int tp);
  void setTree(TTrack* in, trackT* out);
  void setDefault(trackT* tr);
  void writeTracks();

public:
  AMultipleTreeOutput(int max, int& evtNr, int& rnNr, int& triggerIn, int& nTrack,
                      TTrack** tracksIn, const algorithm_parameter& param);
  ~AMultipleTreeOutput() override;
  void process() override;
  static algorithm_parameter getDescription();
  void newRun(run_parameter& rp);
};
