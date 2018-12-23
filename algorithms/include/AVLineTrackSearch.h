#include "algorithm.h"
#include "track.h"
#include "cluster.h"
#include "setup.h"
#include "algorithmparameter.h"
using namespace std;
class trackVertexTrack;
const int MAX=1000;
class AVLineTrackSearch:public AAlgorithm
{
  private:
  TSetup &setup;//!
  int **numberOfHits;//!
  int **numberOfPixels;//!
  int **numberOfClusters;//!
  int maxTracks;//!
  int &numberOfTracks;//!
  int numTracks;//!
  TTrack **tmpTracks;//!
  TCalibHit ***hits;//!
  TPixel ***pixels;//!
  TCluster ***clusters;//!
  TTrack **tracks;//!
  //stop points
  int numStop;
  int *stopIDs;//[numStop]
  int *numEssentials;//[numStop]
  int **essentials;//!
  int *minElements;//[numStop]
  float *maxChi;//[numStop]
  int numStopCluster;
  int *stopClusterIDs;//[numStopCluster]
  int *numClusterEssentials;//[numStopCluster]
  int **clusterEssentials;//!
  int *minClusterElements;//[numStopCluster]
  float *maxClusterChi;//[numStopCluster]
  point3D pts[MAX];
  TBase *pto[MAX];
  //start points
  int numStart;
  int *startIDs;//[numStart]
  int numStartCluster;
  int *startClusterIDs;//[numStartCluster]
  int *minStarts;//[numStart+numStartCluster]
  //detectors
  int numDetectors;
  int *detectorIDs;//[numDetectors]
  int *conventional;//[numdetectors]
  int *away;//[(numDetectors+numNoGo)*(numStop+numStopCluster)]
  float *maxDistance;//[numStop+numStopCluster]
  int numNoGo;
  int *noGoDetectors;//[numNoGo]
  int *awayNoGo;//[numNoGo]
  //parameter
  float maxDistanceVertexPlane;
  float maxVertexChi;
  float maxAngle;
  int minElementsPerVee;
  int maxEqualElements;
  float maxDistanceToPlane;
  int numZDists;
  float *Zdists;//[numZDists*2]
  bool checkForPromptElements;
  int maxPixelPrompt;
  //working structures
  int maxEl;
  int nDet;
  int *nEl;
  TCalibHit ***chels;
  int maxPlanes;
  TTrack ***planeTrack;//!
  TTrack ***pointTracks;//!
  plane3D *plane;//!
  int maxPlanePoints;
  int *numPlanePoints;//!
  TBase ***planePoints;//!
  Vector ***hitParameter;//!
  int numVees;//!
  trackVertexTrack **vee;//!
  //fit routines
  bool checkWithPrompt(TCalibHit *hit);
  int checkWithPrompt(TPixel *pix);
  int checkWithPrompt(TCluster *clu);
  bool searchATrack(int tracknum, int basis,int stopI, TBase *pix, TBase *spix=NULL);
  bool assignPointsToPlane(int planenum);
  bool checkPlane(int planenum,int l,int k);
  bool checkWithPrompt(TBase *p1, TBase *p2, int i, int j);
  bool trackAPlane(int planenum, int id[2]);
  TBase *s_pto[MAX];
  point2D stopPoints[MAX];
  point2D startPoints[MAX];
  float zBuffer[MAX];
  int s_id[MAX];
  int nStartP;
  int nStopP;
  float alternateDist;
  int alternatePlaneSearch();
  bool doAlternateSearch;
 public:
  AVLineTrackSearch(TSetup &setupIn, TTrack **tracksIn, TCluster ***clustersIn, TPixel ***pixelsIn, TCalibHit ***hitsIn, int maxTracksIn, int &numberOfTracksIn,int **numberOfClustersIn, int **numberOfPixelsIn, int **numberOfHitsIn, const algorithm_parameter &descr);
  // AVLineTrackSearch();
  virtual ~AVLineTrackSearch();
  virtual void *process(void*);
  static algorithm_parameter getDescription();
};
