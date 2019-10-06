#include "algorithm.h"
#include "cluster.h"
#include "fittingRoutines.h"
#include "track.h"
#include <vector>

class AKinkSearch : public AAlgorithm
{
private:
  int tmpNum;             //!
  TTrack** tmpTracks;     //!
  TTrack* tmpPrompt;      //!
  TTrack** tracks;        //!
  const int maxTrack;     //!
  int& numberOfTracks;    //!
  TPixel*** pixels;       //!
  int** numberOfPixels;   //!
  TCluster*** clusters;   //!
  int** numberOfClusters; //!
  TCalibHit*** hits;      //!
  int** numberOfHits;     //!
  int* stopPixelIDs;      //[numStopPixels]
  int numStopPixels;
  int* stopClusterIDs; //[numStopCluster]
  int numStopCluster;
  int* definePromptPixIDs; //[numPromptPix]
  int numPromptPix;
  int* definePromptClIDs; //[numPromptCl]
  int numPromptCl;
  int* defineDecPixIDs; //[numDecPix]
  int numDecPix;
  int* defineDecClIDs; //[numDecCl]
  int numDecCl;
  int* promptDetIDs; //[numPromptDet]
  int numPromptDet;
  int* decDetIDs; //[numDecDet]
  int numDecDet;
  int* interDetIDs; //[numInterDet]
  int numInterDet;
  int minPromPix;
  int minDecPix;
  int* minElementsP; //[numPromptPix+numPromptCl]
  int* minElementsD; //[numStopPixels+numStopCluster]
  float maxChiKink;
  vertexFit vFit;
  bool checkLine(const sLine3D& promptLine);
  bool searchTrack(int trNum, int* dets, int numDets, void* p1, void* p2, int d1, int d2);
  bool searchPrompt(TTrack* tmpPrompt, void* pix, int d2, Vector* PromptParams, int& numPromptP);
  int bestElement(TTrack* tracki, int det, float maxDistP, float maxDistT, bool cp,
                  Vector& bestHit);

public:
  AKinkSearch(TTrack** tracksI, int maxTrackI, int& numberOfTracksI, TPixel*** pixelsI,
              int** numberOfPixelsI, TCluster*** clustersI, int** numberOfClustersI,
              TCalibHit*** hitsI, int** numberOfHitsI, std::vector<int> stopPixelIDsI,
              std::vector<int> stopClusterIDsI, std::vector<int> definePromptPixIDsI,
              std::vector<int> definePromptClIDsI, std::vector<int> defineDecPixIDsI,
              std::vector<int> defineDecClIDsI, std::vector<int> promptDetIDsI,
              std::vector<int> decDetIDsI, std::vector<int> interDetIDsI, int minPromPixI,
              int minDecPixI, std::vector<int> minElementsPI, std::vector<int> minElementsDI,
              float maxChiKinkI);
  ~AKinkSearch() override;
  void process() override;
};
