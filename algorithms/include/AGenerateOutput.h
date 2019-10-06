#include "RbaseReaction.h"
#include <TTree.h>
#include <algorithm.h>
#include <container.h>
class run_parameter;
using namespace std;
#ifndef OUT_TRACK_
#define OUT_TRACK_
typedef struct
{
  Double_t phi;
  Double_t theta;
  Double_t beta;
  Int_t ID;
  Int_t nEl;
  Int_t ss[17];
  Float_t qdc[17];
  Float_t tdc[17];
  Double_t vertex[3];
  Double_t phiIn;
  Double_t thetaIn;
  Double_t betaIn;
  Int_t nElIn;
  Int_t ssIn[17];
  Double_t vertexIn[3];
  Double_t momentum;
} track;
typedef struct
{
  Float_t qdc[17];
  Float_t tdc[17];
  Int_t ss[17];
  Int_t ssIn[17];
  Double_t vertex[3];
  Double_t vertexIn[3];
  Double_t phi;
  Double_t theta;
  Double_t beta;
  Double_t momentum;
  Double_t phiIn;
  Double_t thetaIn;
  Double_t betaIn;
  Int_t ID;
  Int_t nEl;
  Int_t nElIn;
} track1;
typedef struct
{
  Int_t ID;
  Int_t numberOfElements;
  Int_t elementNumber[10];
  Double_t center[3];
} pixel;
typedef struct
{
  Int_t ID;
  Int_t numberOfPixels;
  Int_t pixelNumber[10];
  Double_t center[3];
} cluster;
typedef struct
{
  Int_t qdcR;
  Int_t tdcR;
  Double_t qdcC;
  Double_t tdcC;
  Int_t el;
  Int_t det;
} hit;
#endif
#include <fstream>
#ifndef AGENERATEOUTPUT
#define AGENERATEOUTPUT
class AGenerateOutput : public AAlgorithm
{
private:
  float* outp;
  bool useMC;
  bool useLocal;
  string localDir;
  string filepattern;
  ofstream outFile;
  ifstream mcFile;
  string outfileName; //!
  TEvent& event;      //!
  int maxTr;
  Int_t numTr;
  int eventNr;
  Int_t runNr;
  Float_t chisquared;
  int trigger;
  float **phi, **th, **beta; //!
                             /*   TVectorF *ss;  //! */
                             /*   TVectorF ***saves;//! */
                             /*   TVectorF *qdc,*tdc;//! */
  Int_t valid;
  float** vert; //!
  int* nEl;     //!
  int maxtr;
  bool writeHits;
  bool writePixel;
  bool writeCluster;
  bool writeTracks;
  bool writeReactions;
  TTree** reactTree; //!
  TTree* mcTree;     //!
  TTree** trackTree; //!
  TTree** pixTree;   //!
  TTree** cluTree;   //!
  TTree** hitTree;   //!
  int numT;
  int numC;
  int numP;
  int numH;
  int numReact;
  RbaseReaction** reactions; //!
  int** reactIDs;            //!
  TTree* numHitTree;         //!
  TTree* numPixTree;         //!
  TTree* numCluTree;         //!
  TTree* numTrackTree;       //!
  TFile* hitFile;            //!
  TFile* pixFile;            //!
  TFile* cluFile;            //!
  TFile* trackFile;          //!
  TFile* reactionFile;       //!
  track1** Track;            //!
  track* trackIn;            //!
  int numberOfHits;
  const int maxOfHits;
  hit** Hits; //!
  int numberOfPixels;
  const int maxOfPixel;
  pixel** Pix; //!
  int numberOfCluster;
  const int maxOfCluster;
  cluster** Clus;       //!
  Int_t** fromTrack;    //!
  Int_t*** hitsOfTrack; //!
  void expandTree(int which, int upto);
  int getNumTrees(int numEntr, int which);
  void clearData();
  void write_Tracks();
  void write_Pixels();
  void write_Hits();
  void write_Clusters();
  void getMCTree(string mcFile);
  void assignTrackTree();
  void assignPixelTree();
  void assignClusterTree();
  void assignHitTree();
  void write_Reactions();
  void writeLambda();
  void setReactions(vector<string> reacts, string outName);
  Int_t*** saver; //!
  void* outmutex; //!
  bool server;
  bool addFile;
  bool exchangeFile;

public:
  AGenerateOutput(TEvent& evIn, const string& mcFileName, const string& outFileName,
                  const string& localD, bool useLocalD, int maxt, bool umc, bool writeT,
                  bool writeC, bool writeP, bool writeH, bool writeR, vector<string> reacts,
                  int serverIn, bool addFileIn, bool exchangeFileIn, void* outmutexIn);
  ~AGenerateOutput() override;
  void process() override;
  void OnFinalizeTracks(vector<TTree*>* tTree, TTree* nTTree);
  void OnFinalizeCluster(vector<TTree*>* cTree, TTree* nCTree);
  void OnFinalizePixel(vector<TTree*>* pTree, TTree* nPTree);
  void OnFinalizeHits(vector<TTree*>* hTree, TTree* nHTree);
  void OnFinalizeReactions(vector<TTree*>* rTree);
  void OnNewRun(run_parameter& r);
};
#endif
