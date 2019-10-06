#include "Calibrations.h"
APedestalCalibration::APedestalCalibration(TEvent& eventIn, TSetup& setup,
                                           algorithm_parameter& descr)
    : CalibrationAlgorithm("Generate Pedestal calibration"), event(eventIn),
      minEntriesPerHisto(descr.value(0).value<int>())
{
  vector<int> tmp(descr.value(3).value<vector<int>>());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1**[numberOfDetectors];
  string nn;
  int xmin = 0, xmax = 10000;
  xmin = descr.value(1).value<int>();
  xmax = descr.value(2).value<int>();
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i])
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
    else
      numberOfElements[i] = 0;
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("qdc: detID-") + string_number(detectorIDs[i]) + " el-" + string_number(j))
               .data();
      qdcHistograms[i][j] = new TH1F(nn.data(), nn.data(), 1000, xmin, xmax);
    }
  }
}
APedestalCalibration::APedestalCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                                           int minEntriesPerHistoIn)
    : CalibrationAlgorithm("Generate Pedestal calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1**[numberOfDetectors];
  string nn;
  int xmin = 0, xmax = 10000;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i])
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
    else
      numberOfElements[i] = 0;
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("qdc: detID-") + string_number(detectorIDs[i]) + " el-" + string_number(j))
               .data();
      qdcHistograms[i][j] = new TH1F(nn.data(), nn.data(), 1000, xmin, xmax);
    }
  }
}
APedestalCalibration::~APedestalCalibration()
{
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++)
      delete qdcHistograms[i][j];
    delete[] qdcHistograms[i];
  }
  delete[] qdcHistograms;
  delete[] detectorIDs;
  delete[] numberOfElements;
}
void APedestalCalibration::process()
{
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < event.getNumberOfHits(detectorIDs[i]); j++)
      qdcHistograms[i][event.getHit(detectorIDs[i], j).getElement()]->Fill(
          event.getHit(detectorIDs[i], j).getRawADC());
  }
}
void APedestalCalibration::toEvaluate()
{
  //  for (int i = 0; i < numberOfDetectors; i++)
  //    emit evaluated(evaluate(i));
  eventStart = event.getEventNumber();
}
CommonCalibrationParser* APedestalCalibration::evaluate(int num)
{
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(4, numberOfElements[num]);
  ret->setParamID(0);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[4] = {0, 0, 0, 0};
  for (int i = 0; i < numberOfElements[num]; i++) {
    // insert calibration routine
    /*********************************

    **********************************/
    for (int j = 0; j < 4; j++)
      ret->setParameter(i, j, v[j]);
  }
  return ret;
}
