#include "AGenerateOutput.h"
#include "algorithm.h"
#include "event.h"
#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

#include <boost/signals2.hpp>

class AReadFromSimpleTree : public AAlgorithm
{
private:
  bool localDirectory;
  string localDir;
  bool readHits;
  bool readPixels;
  bool readClusters;
  bool readTracks;
  bool resetUnread;
  Int_t evNT, evNP, evNC, evNH; //!
  Int_t tT, tP, tC, tH;         //!
  //  TTree *tree;//!
  TTrack** tracks; //!
  const int maxTrack;
  track1** Tracks;     //!
  Int_t** fromID;      //!
  int& numberOfTracks; //!
  Int_t ntr;
  Int_t*** hitsOfTrack;    //!
  TBranch** trackBranches; //!
  int maxTracks;           //!
  TLeaf*** trackLeaves;    //!
  TLeaf** trackHitLeaves;  //!
  TLeaf** trackFromLeaves; //!
  Int_t numberOfHits;
  const int maxOfHits;
  hit** Hits; //!
  Int_t numberOfPixels;
  const int maxOfPixel;
  vector<pixel*> Pix; //!
  Int_t numberOfCluster;
  const int maxOfCluster;
  vector<cluster*> Clus;   //!
  int* eventNumber;        //!
  TCalibHit*** oHits;      //!
  int** onumberOfHits;     //!
  TPixel*** oPixels;       //!
  int** onumberOfPixels;   //!
  TCluster*** oClusters;   //!
  int** onumberOfClusters; //!
  int numHitTrees;
  int numPixTrees;
  int numCluTrees;
  int numTrackTrees;
  TTree** pixTree;              //!
  TTree** cluTree;              //!
  TTree** hitTree;              //!
  TTree** trackTree;            //!
  TTree* numHitTree;            //!
  TTree* numPixTree;            //!
  TTree* numCluTree;            //!
  TTree* numTrackTree;          //!
  vector<Int_t> nextHitEntry;   //!
  vector<Int_t> nextPixEntry;   //!
  vector<Int_t> nextCluEntry;   //!
  vector<Int_t> nextTrackEntry; //!
  int* currentHitEntry;         //!
  int* currentPixEntry;         //!
  int* currentCluEntry;         //!
  int* currentTrackEntry;       //!
  TSetup* setup;                //!
  int counter;
  int c1, c2, c3, c4;
  TEvent* event;            //!
  TFile* trackf;            //!
  TFile* pixelf;            //!
  TFile* clusterf;          //!
  TFile* hitf;              //!
  bool& validInput;         //!
  vector<string> filenames; //!
  void* inputmutex;         //!
  boost::signals2::signal<void(int)> _hitsReadSignal;
  boost::signals2::signal<void(int)> _pixelsReadSignal;
  boost::signals2::signal<void(int)> _tracksReadSignal;
  boost::signals2::signal<void(int)> _clustersReadSignal;

  int searchDelayedEvent(TTree** thetree, int numTree, int eventNumber, int* theCounter,
                         int acounter);
  int getCommonEvent(int& iT, bool& ht, int& iC, bool& hc, int& iP, bool& hp, int& iH, bool& hh);
  void readTrack(int i);
  void readPixel(int i);
  void readCluster(int i);
  void readHit(int i);
  int getNumTrees(int numEntr, int which);
  bool setTrackTree(TFile* f);
  bool setPixelTree(TFile* f);
  bool setClusterTree(TFile* f);
  bool setHitTree(TFile* f);
  void setHitStruct();
  void setPixelStruct();
  void setClusterStruct();
  void cleanTracks();
  void cleanHits();
  void cleanPixels();
  void cleanCluster();

public:
  AReadFromSimpleTree(const string& fileName, const string& treename, const string& localDirec,
                      bool useLocalD, TTrack** tracksIn, int& numberOfTracksIn, int& eventNumberIn,
                      int maxTrackIn, TEvent& eventIn, int& maxEventsToRead, TSetup* setupIn,
                      bool readT, bool readC, bool readP, bool readH, string TrackFile,
                      string ClusterFile, string PixelFile, string HitFile, bool resetUnreadIn,
                      bool startWithDelayedEventIn, int StartWithEventIn, bool& validInputIn,
                      void* inputmutexIn);
  virtual ~AReadFromSimpleTree() override;
  void process() override;

  void getNewInput(string* files, int* type, int numFiles, bool grap);
  void getNewRun(run_parameter& rp);
  boost::signals2::connection connectHitRead(std::function<void(int)> subscriber);
  boost::signals2::connection connectPixelRead(std::function<void(int)> subscriber);
  boost::signals2::connection connectClusterRead(std::function<void(int)> subscriber);
  boost::signals2::connection connectTrackRead(std::function<void(int)> subscriber);
  void OnHitRead(int numberOfHits);
  void OnPixelRead(int numberOfPixels);
  void OnClusterRead(int numberOfClusters);
  void OnTrackRead(int numberOfTracks);
};
