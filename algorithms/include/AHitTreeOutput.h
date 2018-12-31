#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
using namespace std;
const int MAX_HITS = 100000;
typedef struct
{
  Float_t qdcc[MAX_HITS];
  Float_t tdcc[MAX_HITS];
  Int_t qdc[MAX_HITS];
  Int_t tdc[MAX_HITS];
  Short_t det[MAX_HITS];
  Short_t el[MAX_HITS];
} hit_struct;

class AHitTreeOutput : public AAlgorithm
{
private:
  TCalibHit*** hits;   //!
  int** numberOfHits;  //!
  int& EventNumber;    //!
  int& RunNumber;      //!
  int& Trigger;        //!
  int maxDet;          //!
  TTree* hitTree;      //!
  hit_struct outHits;  //!
  bool localDirectory; //!
  string directory;    //!
  string outFile;      //!
  string copyFile;     //!
  Int_t Event;         //!
  Int_t Run;           //!
  Int_t Trig;          //!
  Int_t nHit;          //!
  void initialize(const string& filename);
  void finalize();

public:
  AHitTreeOutput(TCalibHit*** hitIn, int** nHits, int& evtNr, int& rnNr, int& trg, int maxD,
                 const algorithm_parameter& param);
  virtual ~AHitTreeOutput();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
};
