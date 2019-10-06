#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TChain.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
#include <boost/signals2.hpp>

using namespace std;
class run_parameter;
class AHitTreeInput : public AAlgorithm
{
private:
  TCalibHit*** hits;       //!
  int** numberOfHits;      //!
  int& EventNumber;        //!
  int& RunNumber;          //!
  int& Trigger;            //!
  int maxHit;              //!
  int maxDet;              //!
  bool localDirectory;     //!
  string directory;        //!
  vector<string> copyFile; //!
  bool& valid;             //!
  bool searchForEvent;     //!
  TSetup& setup;           //!
  TChain* hitChain;        //!
  TTree* hitTree;          //!
  TLeaf* tdcRLeaf;         //!
  TLeaf* qdcRLeaf;         //!
  TLeaf* tdcCLeaf;         //!
  TLeaf* qdcCLeaf;         //!
  TLeaf* elLeaf;           //!
  TLeaf* detLeaf;          //!
  TLeaf* eh_en;            //!
  TLeaf* eh_rn;            //!
  TLeaf* eh_tr;            //!
  TLeaf* eh_nh;            //!
  int currentEntry;        //!
  void clearTree();
  void getHeader();
  void getMain();
  bool useChain;
  boost::signals2::signal<void(int)> _eventReadSignal;

public:
  AHitTreeInput(TSetup& setupIn, TCalibHit*** hitsIn, int** nH, int& evtNr, int& rnNr, int& trggr,
                int maxH, int MaxD, bool& readInValid, const algorithm_parameter& param);
  ~AHitTreeInput() override;
  void process() override;
  static algorithm_parameter getDescription();
  void readEvent(int EvtNr);
  void readEntry(int entry);
  void newRun(run_parameter& r);
  boost::signals2::connection connectEventRead(std::function<void(int)> subscriber);
};
