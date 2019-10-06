#include "RbaseReaction.h"
#include "algorithm.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TTree.h>

#ifndef OUT_TRACK_
#ifndef AGENERATEOUTPUT
#define AGENERATEOUTPUT
#include "AGenerateOutput.h"
#undef AGENERATEOUTPUT
#else
#include "AGenerateOutput.h"
#endif
#endif
using namespace std;
/***********************************************
 * This class can read whole-event information *
 * from several files of the simple-tree-format*
 * if there are several files defined for one  *
 * run, they are sorted according to event-    *
 * number and read in successively.            *
 * two modes ae defined:                       *
 *  read all events where at least one data-   *
 *   structure is defined                      *
 *  read only events where a reaction is de-   *
 *   fined                                     *
 * define which data-structures shall be read.  *
 * steers the input variable.                  *
 ***********************************************/
class AReadReactionFromRoot : public AAlgorithm
{
private:
  TSetup& setup;
  TEvent& event;
  bool readHits;
  bool reconstructHits;
  bool readPixel;
  bool readCluster;
  bool readTracks;
  bool readMinimum;
  const int maxHit;
  const int maxPix;
  const int maxClu;
  const int maxTra;
  int NHit, NPix, NClu, NTra, NRea;
  TTree* numHitTree;
  TTree* numPixTree;
  TTree* numCluTree;
  TTree* numTraTree;
  TTree** pixTree;                     //!
  TTree** cluTree;                     //!
  TTree** hitTree;                     //!
  TTree** traTree;                     //!
  TTree** reactionTree;                //!
  Int_t* currentHitEntry;              //!
  Int_t* currentPixEntry;              //!
  Int_t* currentCluEntry;              //!
  Int_t* currentTraEntry;              //!
  Int_t* currentReaEntry;              //!
  Int_t* nextHitEntry;                 //!
  Int_t* nextPixEntry;                 //!
  Int_t* nextCluEntry;                 //!
  Int_t* nextTraEntry;                 //!
  Int_t* nextReaEntry;                 //!
  TFile* traf;                         //!
  TFile* pixf;                         //!
  TFile* cluf;                         //!
  TFile* hitf;                         //!
  TFile** reactionf;                   //!
  bool& validInput;                    //!
  Int_t evNT, evNP, evNC, evNH, *evNR; //!
  Int_t tT, tP, tC, tH, *tR;           //!
  Int_t ruNT, ruNP, ruNC, ruNH, *ruNR; //!
  Int_t numberOfHits;
  hit** Hits; //!
  Int_t numberOfPixels;
  pixel** Pix; //!
  Int_t numberOfCluster;
  cluster** Clus; //!
  Int_t ntr;
  track1** Tracks;      //!
  Int_t** fromID;       //!
  Int_t*** hitsOfTrack; //!
  Int_t* nRtr;
  track1** RTra;               //!
  Int_t** RFrom;               //!
  Int_t*** Rhits;              //!
  RbaseReaction** theReaction; //!
  vector<vector<string>> files;
  int* filePos;
  string* reactionNames;
  vector<vector<string>> Rfiles;
  int* RfilePos;
  void init();
  void cleanHit();
  void cleanPix();
  void cleanClu();
  void cleanTra();
  void cleanARea(int num);
  void cleanRea();
  void sortFiles();
  void searchEvent(int eventNumber, int runNumber, int mode);
  void getNextEvent();
  void read_hit();
  void read_pix();
  void read_clu();
  void read_tra();
  void read_rea();
  void set_hit(string filename);
  void set_pix(string filename);
  void set_clu(string filename);
  void set_tra(string filename);
  void set_rea(string filename, int num, string reactionName);
  void read_a_track(TTrack* tr, track1 Track, Int_t* hi);
  void reconstruct_hits_from_track(track1 Track, TTrack* tr);

public:
  AReadReactionFromRoot(TSetup& setupIn, TEvent& eventIn, bool& validInputIn,
                        const algorithm_parameter& param);
  AReadReactionFromRoot(TSetup& setupIn, TEvent& eventIn, bool& validInputIn, bool readH,
                        bool readP, bool readC, bool readT, bool minimum, bool reconstruct,
                        vector<string> Rnames);
  ~AReadReactionFromRoot() override;
  void process() override;
  void OnNewRun(run_parameter& r);
};
