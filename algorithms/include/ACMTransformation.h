#include "algorithm.h"
#include "container.h"
#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TTree.h>

class ACMTransformation : public AAlgorithm
{
private:
  momentum4D inParticle1;
  momentum4D inParticle2;
  const int maxOutParticles;
  int& numOutParticles;      //!
  momentum4D** outParticles; //!
  momentum4D CM;
  momentum4D* transformedOutParticles; //!
  TTree* outTree;
  TDirectory** rootDirectory; //!
  int &eventNr, &runNr;
  TList* particleList;
  int reactionType;
  float angleCM1;
  float angleCM2;
  float angleCM2_b;
  float angleCM12;
  float anglePlab1;
  float angleTlab1;
  float anglePlab2;
  float angleTlab2;
  vector3D beta;
  double M;
  double gamma;
  int numHistos;
  TH1** histos; //!
public:
  ACMTransformation(TSetup& setup, momentum4D** particles, int maxPart, int& numPart,
                    TDirectory** rootfile, int& evNr, int& runNrIn);
  ~ACMTransformation() override;
  void process() override;
  static algorithm_parameter getDescription();
};
