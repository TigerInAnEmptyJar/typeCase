#include "algorithm.h"
#include "event.h"
#include "geometry.h"
#include "setup.h"
#include <TF1.h>
#include <TGenPhaseSpace.h>
#include <TGraph.h>
using namespace std;
class AGenerateEvents : public AAlgorithm
{
private:
  momentum4D m1, m2;
  vector3D betaCM;
  int numberOfParticles;
  int* particleIDs;    //[numberOfParticles]
  TTrack** tracks;     //!
  int& numberOfTracks; //!
  int& eventNumber;    //!
  TGenPhaseSpace generator;
  TGraph* graph;
  TFormula* formula;
  TF1* function;
  int* parameters; //[nParameters]
  int nParameters;
  double maxWeight;
  bool useSAID;
  bool& validIn; //!
  float modifyWeight(momentum4D* parts);

public:
  AGenerateEvents(TTrack** tracksIn, int& numberOfTracksIn, int& eventNumberIn, TSetup& setupIn,
                  vector<int> IDs, string functionIn, vector<int> parameter, bool useSAIDIN,
                  bool& validInIn);
  ~AGenerateEvents() override;
  void process() override;
};
