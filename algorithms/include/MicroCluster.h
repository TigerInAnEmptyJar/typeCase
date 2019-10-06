#ifndef MICROCLUSTER
#define MICROCLUSTER
#include "algorithm.h"
#include "cluster.h"
class AMicroCluster : public AAlgorithm
{
private:
  int ID;
  const int maxCluster;
  int& numberOfClusters; //!
  TCluster** cluster;    //!
  int& numberOfPixels;   //!
  TPixel** pixel;        //!
  int pattern;
  int searchRing(TCluster* clust, float& normer_f, vector3D& mean_f, bool* used_f, float* energy_f,
                 int size, int centerP);

public:
  AMicroCluster(int IDIn, int& numberOfPixelsIn, TPixel** pixelIn, int maxClusterIn,
                int& numberOfClustersIn, TCluster** clusterIn, int patternIn);
  ~AMicroCluster() override;
  void process() override;
};
#endif
