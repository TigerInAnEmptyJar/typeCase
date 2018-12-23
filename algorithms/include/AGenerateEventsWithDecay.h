#include "algorithm.h"
#include "setup.h"
#include "event.h"
#include <TGenPhaseSpace.h>
#include <TF1.h>
using namespace std;
class AGenerateEventsWithDecay: public AAlgorithm
{
  private:
 TSetup &setup;//!
 TTrack **tracks;//!
 int &numberOfTracks;//!
 int &eventNumber;//!
 int numberOfDecays;
 TGenPhaseSpace **generator;//![numberOfDecays]
 int numberOfParticles;
 int *particleID;//[numberOfParticles]
 int *decayedOff;//[numberOfParticles]
 double *masses;//[numberOfParticles]
 double *halfLife;//[numberOfParticles]
 int *charge;//[numberOfParticles]
 int numberOfPrimary;
 int *primaries;//[numberOfPrimary]
 int *decayParticle;//[numberOfDecays]
 int *numDecayParticles;//[numberOfDecays]
 int **decayIDs;//[numberOfDecays]
 bool PhaseSpace;
 bool acceptance;
 int *requiredHits;//
 int numSums;//
 int *numSumDet;//[numSums]
 int **SumDetID;//!
 int *SumDetHits;//[numSums]
 // double *requiredQDC;//
 // double *requiredTDC;//
 TF1 **function;
 int nParameters;
 int *parameters;//[nParameters]
 double maxWeight;
 bool valid;
 bool &validInput;
 float modifyWeight(int decay);
 void setParameter(momentum4D *parts);
 bool accept(sLine3D *trajectories);
 public:
 AGenerateEventsWithDecay(TTrack **tracksIn, int &numberOfTracksIn, int &eventNumberIf, TSetup &setupIn,bool& validInputIn, const algorithm_parameter &descr);
 virtual ~AGenerateEventsWithDecay();
 virtual void *process(void *ptr);
 static algorithm_parameter getDescription();
};
