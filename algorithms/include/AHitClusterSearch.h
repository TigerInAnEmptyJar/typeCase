#include "algorithm.h"
#include "hitCluster.h"
#include "setup.h"

using namespace std;

class AHitClusterSearch : public AAlgorithm
{
private:
  int** nClusters;         //!
  int** nHits;             //!
  TCalibHit*** hits;       //!
  THitCluster*** clusters; //!
  TSetup& setup;           //!
  bool* circular;          //[numDets]
  int* nEl;                //[numDets]
  int numDets;

public:
  AHitClusterSearch(TSetup& setupIn, THitCluster*** clustersIn, int** numberOfCluster,
                    TCalibHit*** hitsIn, int** numberOfHits, const algorithm_parameter& ap);
  ~AHitClusterSearch() override;
  void process() override;
  static algorithm_parameter getDescription();
};
