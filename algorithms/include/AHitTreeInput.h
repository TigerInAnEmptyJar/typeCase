#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TTree.h>
#include <TChain.h>
#include <TLeaf.h>
#include <TFile.h>
using namespace std;
class run_parameter;
class AHitTreeInput:public AAlgorithm
{
Q_OBJECT
  private:
 TCalibHit ***hits;//!
 int **numberOfHits;//!
 int &EventNumber;//!
 int &RunNumber;//!
 int &Trigger;//!
 int maxHit;//!
 int maxDet;//!
 bool localDirectory;//!
 string directory;//!
 vector<string> copyFile;//!
 bool &valid;//!
 bool searchForEvent;//!
 TSetup &setup;//!
 TChain *hitChain;//!
 TTree *hitTree;//!
 TLeaf *tdcRLeaf;//!
 TLeaf *qdcRLeaf;//!
 TLeaf *tdcCLeaf;//!
 TLeaf *qdcCLeaf;//!
 TLeaf *elLeaf;//!
 TLeaf *detLeaf;//!
 TLeaf *eh_en;//!
 TLeaf *eh_rn;//!
 TLeaf *eh_tr;//!
 TLeaf *eh_nh;//!
 int currentEntry;//!
 void clearTree();
 void getHeader();
 void getMain();
 bool useChain;
 public:
 AHitTreeInput(TSetup& setupIn, TCalibHit ***hitsIn, int **nH, int &evtNr, int &rnNr, int &trggr, int maxH,int MaxD, bool &readInValid,const algorithm_parameter &param);
 virtual ~AHitTreeInput();
 virtual void* process(void* ptr);
 static algorithm_parameter getDescription();
 public slots:
 void readEvent(int EvtNr);
 void readEntry(int entry);
 void newRun(run_parameter &r);
 signals:
 void eventRead(int EvtNr);
};
