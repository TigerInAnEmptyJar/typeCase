#include "Calibrations.h"
#include "fiber.h"
#include "wedge.h"
AzBarrelCalibration::AzBarrelCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr)
    : CalibrationAlgorithm("Generate zBarrel calibration"), event(eventIn),
      minEntriesPerHisto(descr.value(0).value<int>())
{
  vector<int> tmp(descr.value(5).value<vector<int>>());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  histograms = new TH1**[numberOfDetectors];
  string nn;
  int xmin = 0, xmax = 10000;
  xmin = descr.value(1).value<int>();
  xmax = descr.value(2).value<int>();
  int ymin = 0, ymax = 10000;
  xmin = descr.value(3).value<int>();
  xmax = descr.value(4).value<int>();
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i])
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
    else
      numberOfElements[i] = 0;
    histograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("tdc vs r: detID-") + string_number(detectorIDs[i]) + " el-" + string_number(j))
               .data();
      histograms[i][j] = new TH2F(nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
}
AzBarrelCalibration::AzBarrelCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                                         int minEntriesPerHistoIn)
    : CalibrationAlgorithm("Generate zBarrel calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  histograms = new TH1**[numberOfDetectors];
  string nn;
  int xmin = 0, xmax = 10000;
  int ymin = 0, ymax = 10000;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i])
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
    else
      numberOfElements[i] = 0;
    histograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("tdc vs r: detID-") + string_number(detectorIDs[i]) + " el-" + string_number(j))
               .data();
      histograms[i][j] = new TH2F(nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
}

AzBarrelCalibration::~AzBarrelCalibration()
{
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++)
      delete histograms[i][j];
    delete[] histograms[i];
  }
  delete[] histograms;
  delete[] detectorIDs;
  delete[] numberOfElements;
}
void AzBarrelCalibration::process()
{
  if (event.getNumberOfTracks() != 1)
    return;
  TTrack* track = &event.getTrack(0);
  float theta = track->getPath().Direction().Theta(), phi = track->getPath().Direction().Phi();
  float thetaSearched = M_PI / 2 - theta;
  float phiSearched = M_PI - phi;
  if (thetaSearched < 0) {
    thetaSearched = thetaSearched * -1;
    phiSearched = phi;
  }
  if (phiSearched < -M_PI)
    phiSearched = phiSearched + M_PI * 2;
  if (phiSearched > M_PI)
    phiSearched = phiSearched - M_PI * 2;
  TCalibHit* tmp;
  float ph, pr, r;
  sLine3D path(track->getPath().Foot(),
               vector3D(cos(phiSearched) * sin(thetaSearched),
                        sin(phiSearched) * sin(thetaSearched), cos(thetaSearched)));
  point3D onPoint;
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < event.getNumberOfHits(detectorIDs[i]); j++) {
      tmp = &event.getHit(detectorIDs[i], j);
      if (tmp->getHitShape() == NULL)
        continue;
      if (tmp->getHitShape()->getName() != "wedge")
        continue;
      ph = ((wedge*)tmp->getHitShape())->getLowerPhiEdge().Phi();
      pr = ((wedge*)tmp->getHitShape())->getPhiRange();
      if (phiSearched < ph || phiSearched > ph + pr)
        continue;
      onPoint = tmp->getHitShape()->Normal(path).Foot();
      if (onPoint.getState() == _undefined_)
        continue;
      r = (onPoint - ((wedge*)tmp->getHitShape())->getCenter()).R() -
          ((wedge*)tmp->getHitShape())->getInnerRadius();
      histograms[i][event.getHit(detectorIDs[i], j).getElement()]->Fill(
          tmp->getRawTDC(), r); // should be tdc difference!!!
    }
  }
}
void AzBarrelCalibration::toEvaluate()
{
  //  for (int i = 0; i < numberOfDetectors; i++)
  //    emit evaluated(evaluate(i));
  eventStart = event.getEventNumber();
}

CommonCalibrationParser* AzBarrelCalibration::evaluate(int num)
{
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(5, numberOfElements[num]);
  ret->setParamID(6);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[5] = {0, 0, 0, 0, 0};
  for (int i = 0; i < numberOfElements[num]; i++) {
    // insert calibration routine
    /*********************************

    **********************************/
    for (int j = 0; j < 5; j++)
      ret->setParameter(i, j, v[j]);
  }
  return ret;
}
