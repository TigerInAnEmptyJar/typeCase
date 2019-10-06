#include "algorithm.h"

class ACalibrationAlgorithm : public AAlgorithm
{
protected:
  const int NCalTypes;
  int* calTypes;
  int* NParams;
  int* DetID;
  int* Nel;
  int minRun;
  int maxRun;
  std::string BeamTime;
  std::string Author;

public:
  ACalibrationAlgorithm(std::string name, int NCalibs);
  ~ACalibrationAlgorithm() override;
  virtual void fill() = 0;
  virtual void calculateCalibration() = 0;
  virtual void writeToFile(std::string filename) = 0;
};
