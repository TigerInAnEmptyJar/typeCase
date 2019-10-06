#include "algorithm.h"
#include "calibHit.h"
#include <string>
#include <vector>
class run_parameter;
using namespace std;
class ATeufelCorrection : public AAlgorithm
{
private:
  TCalibHit** hits;        //!
  const int& numberOfHits; //!
  const int nEl;
  const int& evtNr; //!
  const int& runNr; //!
  double** params;  //!
  double* shift;    //!
  int** sranges;    //!
  int numSranges;   //!
  int actualrange;  //!
  void getFromFile(vector<string> tmp);
  void cleanup();

public:
  ATeufelCorrection(TCalibHit** hitsIn, const int& numberOfHitsIn, const int detID, int nElIn,
                    int& evtNrIn, int& runNrIn, vector<string> calibrationFiles,
                    string referencefile);
  ~ATeufelCorrection() override;
  void process() override;
  void onNewRum(run_parameter& r);
};
