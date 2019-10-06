#include "ACalibrationGeneration.h"
#include "Calibrations.h"
#include "logger.h"
#include <QObject>
#include <fstream>
#include <iostream>
#include <qdatetime.h>
ACalibrationGeneration::ACalibrationGeneration(TEvent& eventIn, TSetup& setupIn,
                                               const algorithm_parameter& descr)
    : AAlgorithm("Generate calibration constants"), event(eventIn), setup(setupIn)
{
  author = descr.value(4).value<string>();
  beamtime = descr.value(5).value<string>();
  fromRun = descr.value(1).value<int>();
  uptoRun = descr.value(2).value<int>();
  useEventBase = descr.value(0).value<bool>();
  if (useEventBase) {
    eventBase = 10000;
    eventBase = descr.value(3).value<int>();
    eventCounter = 0;
  }
  outFileName = descr.value(6).value<string>();
  detectorNames.push_back("QUIRL_GERADE");
  detectorNames.push_back("QUIRL_LINKS");
  detectorNames.push_back("QUIRL_RECHTS");
  detectorNames.push_back("RING_GERADE");
  detectorNames.push_back("RING_LINKS");
  detectorNames.push_back("RING_RECHTS");
  detectorNames.push_back("BARREL_VORNE");
  detectorNames.push_back("BARREL_HINTEN");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("");
  detectorNames.push_back("START_A");
  detectorNames.push_back("START_B");
  detectorNames.push_back("HODO_X");
  detectorNames.push_back("HODO_Y");
  detectorNames.push_back("large_HODO_X");
  detectorNames.push_back("large_HODO_Y");
  detectorNames.push_back("MICRO_PHI");
  detectorNames.push_back("MICRO_R");
  detectorNames.push_back("HODO_D");
  detectorNames.push_back("");
  detectorNames.push_back("CALORIMETER");
  calibrationNames.push_back("pedestal");
  calibrationNames.push_back("tdcFactor");
  calibrationNames.push_back("walk");
  calibrationNames.push_back("cuts");
  calibrationNames.push_back("qdcFactor");
  calibrationNames.push_back("tdcOffset");
  calibrationNames.push_back("zBarrel");
  calibrationNames.push_back("tdcRadialPix");
  calibrationNames.push_back("tdcRadialPol");
  calibrationNames.push_back("tdcRadialPix");
  calibrationNames.push_back("tdcRadialPol");
  calibrationNames.push_back("qdcRadialPix");
  calibrationNames.push_back("qdcRadialPol");
  declareCalibs(descr);
}
ACalibrationGeneration::~ACalibrationGeneration()
{
  //  cout<<"emit toEvaluate()"<<endl;
  // emit toEvaluate();
  //  cout<<"delete"<<endl;
  for (int i = 0; i < numberOfCalibrationAlgorithms; i++)
    if (calibrationAlgorithms[i] != NULL) {
      //	((AtdcRadialPixCalibration*)calibrationAlgorithms[i])->toEvaluate();
      delete calibrationAlgorithms[i];
    }
  delete[] calibrationAlgorithms;
}

void ACalibrationGeneration::toEvaluate()
{
  for (int i = 0; i < numberOfCalibrationAlgorithms; i++) {
    if (calibrationAlgorithms[i] != nullptr) {
      calibrationAlgorithms[i]->toEvaluate();
      writeCalibrationToFile(calibrationAlgorithms[i]->getCalibration().get());
    }
  }
}

void ACalibrationGeneration::process()
{
  for (int i = 0; i < numberOfCalibrationAlgorithms; i++)
    if (calibrationAlgorithms[i] != NULL)
      calibrationAlgorithms[i]->process();
  if (useEventBase) {
    if (eventCounter >= eventBase) {
      toEvaluate();
      eventCounter = -1;
    }
    eventCounter++;
  }
}
void ACalibrationGeneration::getNewRun(run_parameter& r) {}
void ACalibrationGeneration::writeCalibrationToFile(CommonCalibrationParser* parameter)
{
  ofstream out;
  out.open(outFileName.data(), ios_base::app);
  out << "#======================new calibration type==========================" << endl;
  out << endl;
  out << "==>               " << detectorNames[parameter->getDetector()].data() << " "
      << parameter->getDetector() << " " << parameter->getNumElements() << "  "
      << calibrationNames[parameter->getParamID()].data() << "  " << parameter->getParamID() << "  "
      << parameter->getNumParameters() << endl;
  out << endl;
  out << "#---------------------------------------------------" << endl;
  out << "author            " << author.data() << endl;
  out << "date              " << QDate::currentDate().toString().data() << endl;
  if (useEventBase) {
    out << "fromEvent         " << parameter->getRange(2) << endl;
    out << "uptoEvent         " << parameter->getRange(3) << endl;
    out << "fromRun           " << event.getRunNumber() << endl;
    out << "uptoRun           " << event.getRunNumber() << endl;
  } else {
    out << "fromRun           " << fromRun << endl;
    out << "uptoRun           " << uptoRun << endl;
  }
  out << "beamTime          " << beamtime.data() << endl;
  out << "gotFromFile       newCalDat.data" << endl;
  out << "#----------------------------------------------------" << endl;
  for (int i = 0; i < parameter->getNumElements(); i++) {
    out << parameter->getDetector() << "\t" << i << "\t";
    for (int j = 0; j < parameter->getNumParameters(); j++)
      out << parameter->getParameter(i, j) << "\t";
    out << endl;
  }
  out << endl;
  out.close();
  anaLog << "Calibration \"" << calibrationNames[parameter->getParamID()] << "\" for detector-ID-"
         << parameter->getDetector() << " written to file \"" << outFileName << "\"" << endli;
}
void ACalibrationGeneration::declareCalibs(const algorithm_parameter& descr)
{
  //  algorithm_parameter tmp;
  int algoNum = 0;
  std::vector<std::shared_ptr<algorithm_parameter>> algorithms;
  for (size_t i = 0; i < descr.numberOfValues(); i++) {
    if (descr.value(i).valueType() == ParameterValue::ValueType::ALGORITHM) {
      algorithms.push_back(descr.value(i).value<std::shared_ptr<algorithm_parameter>>());
    }
  }
  numberOfCalibrationAlgorithms = algorithms.size();
  calibrationAlgorithms = new CalibrationAlgorithm*[numberOfCalibrationAlgorithms];
  for (int i = 0; i < numberOfCalibrationAlgorithms; i++)
    calibrationAlgorithms[i] = NULL;

  for (size_t i = 0; i < algorithms.size(); i++) {
    calibrationAlgorithms[algoNum] = NULL;
    switch (algorithms[i]->getID()) {
    case 100:
      calibrationAlgorithms[algoNum] = new APedestalCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 101:
      calibrationAlgorithms[algoNum] = new AtdcFactorCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 102:
      calibrationAlgorithms[algoNum] = new AWalkCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 103:
      calibrationAlgorithms[algoNum] = new ACutsCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 104:
      calibrationAlgorithms[algoNum] = new AqdcFactorCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 105:
      calibrationAlgorithms[algoNum] = new AtdcOffsetCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 106:
      calibrationAlgorithms[algoNum] = new AzBarrelCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 107:
      calibrationAlgorithms[algoNum] = new AtdcRadialPixCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 108:
      calibrationAlgorithms[algoNum] = new AtdcRadialPolCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 109:
      calibrationAlgorithms[algoNum] = new AtdcRadialPixCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 110:
      calibrationAlgorithms[algoNum] = new AtdcRadialPolCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 111:
      calibrationAlgorithms[algoNum] = new AqdcRadialPixCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    case 112:
      calibrationAlgorithms[algoNum] = new AqdcRadialPolCalibration(event, setup, *algorithms[i]);
      algoNum++;
      break;
    }
  }
}
