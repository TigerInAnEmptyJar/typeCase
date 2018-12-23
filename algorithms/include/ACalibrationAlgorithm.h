#include "algorithm.h"

using namespace std;
class ACalibrationAlgorithm: public AAlgorithm
{
 protected:
  const int NCalTypes;
  int *calTypes;
  int *NParams;
  int *DetID;
  int *Nel;
  int minRun;
  int maxRun;
  string BeamTime;
  string Author;
 public:
  ACalibrationAlgorithm(string name, int NCalibs);
  ~ACalibrationAlgorithm();
  virtual void fill(void)=0;
  virtual void calculateCalibration(void)=0;
  virtual void writeToFile(string filename)=0;
};
