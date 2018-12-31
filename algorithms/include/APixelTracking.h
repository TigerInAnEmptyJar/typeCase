#include "algorithm.h"
#include "cluster.h"
#include "setup.h"
#include "track.h"

using namespace std;

class APixelTracking : public AAlgorithm
{
private:
  TTrack** tracks;
  int& numberOfTracks;
  TCalibHit*** hits;
  TPixel*** pixels;
  TCluster*** clusters;
  int** numberOfHits;
  int** numberOfPixels;
  int** numberOfClusters;
  TSetup& setup;
  volumeShape* target;
  int maxTracks;
  int maxPromptEqual;
  int maxVeeCommon;
  int* maxDiff;
  int* minPoints;
  int minPointsVee;
  float* maxDistance;
  int maxDecayStart;
  float maxTargetDist;
  float maxVeeTargetDist;
  float maxDistVee;
  float* maxChiPrompt;
  float* maxChiDecay;
  float maxChiVee;
  int** detpos;
  int* ndetpos;
  int nndetpos;
  int* checkdets;
  int** pixpos;
  int* npixpos;
  int* checkpix;
  int nnpixpos;
  TTrack** tmptracks;
  bool checkWithPrompt(TCalibHit* hit);
  bool checkWithPrompt(TPixel* pix);
  void eliminateTooCloseToPrompt(int& n, TTrack** array, int max);
  float trackFit(TTrack* track, int ID, float maxdistance);
  float veeFit(TTrack* track1, TTrack* track2);
  TCalibHit* searchElement(const sLine3D& line, int ID, int trackID);
  bool searchPromptTrack(TTrack* track, int ID);
  bool searchDecayTrack(TTrack* track, int ID);
  void sortAndCopyPrompt(int& n, TTrack** array);
  void sortAndCopyVee(int& n, TTrack** array);
  void eliminateDoubleTracks(int& n, TTrack** array, int max);
  void eliminateDoubleTracks(int& n1, TTrack** array1, int& n2, TTrack** array2, int max);
  void setTrackErrors(TTrack* tr);
  int nElementsInCommon(TTrack* t1, TTrack* t2);

public:
  APixelTracking(TSetup& setupIn, TTrack** tracksIn, TPixel*** pixelsIn, TCluster*** clusterIn,
                 TCalibHit*** hitsIn, int& nTracksIn, int** nPixelsIn, int** nClusterIn,
                 int** nHitsIn, int maxTracksIn, const algorithm_parameter& param);
  virtual ~APixelTracking();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
};
