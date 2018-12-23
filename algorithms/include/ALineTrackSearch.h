#include "algorithm.h"
#include "track.h"
#include "cluster.h"
#include "setup.h"
#include "fitAlgorithm.h"
#include "algorithmparameter.h"
#include "fittingRoutines.h"
using namespace std;

class ALineTrackSearch:public AAlgorithm
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
 lineFit myLineFit;//!
 int maxEqualElements;
 int numStop;
 int numStopCluster;
 int numStart;
 int *stopIDs;//[numStop]
 int *stopClusterIDs;//[numStopCluster]
 int *numEssentials;//[numStop]
 int *numClusterEssentials;//[numStopCluster]
 int **essentials;//!
 int **clusterEssentials;//!
 int *minElements;//[numStop]
 int *minClusterElements;//[numStopCluster]
 int *startIDs;//[numStart]
 int numMid;
 int *midIDs;//[numMid]
 int numDetectors;
 int *detectorIDs;//[numDetectors]
 int *conventional;//[numdetectors]
 int *away;//[numDetectors*(numStop+numStopCluster)]
 float *maxChi;//[numStop]
 float *maxClusterChi;//[numStopCluster]
 float *restrictionT;//[(numStop+numStopCluster)*2]
 float *restrictionP;//[(numStop+numStopCluster)*2]
 float *maxDistance;//[numStop+numStopCluster]
 bool useVertexAsStart;
 bool angularDistance;
 bool ownFitRoutine;//!
 bool searchATrack(int tracknum, int stopI, TBase *pix, TBase *spix=NULL);
 public:
 ALineTrackSearch(TSetup &setupIn, TTrack **tracksIn, TCluster ***clustersIn, TPixel ***pixelsIn, TCalibHit ***hitsIn, int maxTracksIn, int &numberOfTracksIn,int **numberOfClustersIn, int **numberOfPixelsIn, int **numberOfHitsIn, const algorithm_parameter &descr);
 // ALineTrackSearch();
 virtual ~ALineTrackSearch();
 virtual void *process(void*);
 static algorithm_parameter getDescription();
};
