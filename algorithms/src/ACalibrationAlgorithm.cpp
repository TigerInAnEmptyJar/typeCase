#include "ACalibrationAlgorithm.h"

ACalibrationAlgorithm::ACalibrationAlgorithm(string name, int NCalibs): AAlgorithm(name), NCalTypes(NCalibs)
{
  calTypes=new int[NCalTypes];
  NParams=new int[NCalTypes];
  DetID=new int[NCalTypes];
  Nel=new int[NCalTypes];
  minRun=1000000000;
  maxRun=0;
}
ACalibrationAlgorithm::~ACalibrationAlgorithm()
{
  delete []calTypes;
  delete []NParams;
  delete []DetID;
  delete []Nel;
}
