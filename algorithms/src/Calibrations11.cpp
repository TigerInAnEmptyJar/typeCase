#include "Calibrations.h"
#include "reactions.h"
#include <stdlib.h>
extern float getLightWay(volumeShape* sh, point3D onPoint, bool from);
extern int getPixelPoint(TPixel* pix);
AqdcRadialPixCalibration::AqdcRadialPixCalibration(TEvent& eventIn, TSetup& setup,
                                                   algorithm_parameter& descr)
    : CalibrationAlgorithm("Generate qdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(descr.value(0).value<int>())
{
  vector<int> tmp(descr.value(3).value<vector<int>>());
  vector<int> tmp1(descr.value(4).value<vector<int>>());
  vector<int> tmp2(descr.value(5).value<vector<int>>());
  vector<int> tmp3(descr.value(6).value<vector<int>>());
  vector<int> tmp4(descr.value(7).value<vector<int>>());
  getReactions(descr.value(8).value<vector<string>>());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1***[numberOfDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  string nn;
  numberOfStartDetectors = tmp1.size();
  startDetectors = new int[numberOfStartDetectors];
  for (unsigned int i = 0; i < tmp1.size(); i++)
    startDetectors[i] = tmp1[i];
  numberOfPixels = tmp2.size();
  pixelIDs = new int[numberOfPixels];
  pixelpoints = new int[numberOfDetectors];
  for (int i = 0; i < numberOfPixels; i++) {
    pixelIDs[i] = tmp2[i];
  }
  int xmin = -100, xmax = -100;
  xmin = descr.value(1).value<int>();
  xmax = descr.value(2).value<int>();
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    pixelpoints[i] = tmp4[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i]) {
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
      innerReadOut[i] = (tmp3[i] == 1);
      CinSzinti[i] = 0.7;
      if (setup.getDetectorr(i).getMaterial() != NULL)
        CinSzinti[i] = setup.getDetectorr(i).getMaterial()->getSpeedOfLight();
    } else {
      numberOfElements[i] = 0;
      innerReadOut[i] = false;
      CinSzinti[i] = 0.7;
    }
    qdcHistograms[i] = new TH1**[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      qdcHistograms[i][j] = new TH1*[pixelpoints[i]];
      for (int k = 0; k < pixelpoints[i]; k++) {
        nn = (string("qdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
              string_number(j) + " pix-" + string_number(k))
                 .data();
        qdcHistograms[i][j][k] = new TH1F(nn.data(), nn.data(), 100, xmin, xmax);
      }
    }
  }
}
AqdcRadialPixCalibration::AqdcRadialPixCalibration(TEvent& eventIn, TSetup& setup,
                                                   vector<int>& detectors, vector<int>& Stdetectors,
                                                   vector<int> pixIDs, vector<int> readSide,
                                                   vector<int> pointsPerPixel,
                                                   vector<string> reactionNames,
                                                   int minEntriesPerHistoIn)
    : CalibrationAlgorithm("Generate qdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  getReactions(reactionNames);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1***[numberOfDetectors];
  numberOfStartDetectors = Stdetectors.size();
  startDetectors = new int[numberOfStartDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  for (unsigned int i = 0; i < Stdetectors.size(); i++)
    startDetectors[i] = Stdetectors[i];
  numberOfPixels = pixIDs.size();
  pixelIDs = new int[numberOfPixels];
  pixelpoints = new int[numberOfPixels];
  for (int i = 0; i < numberOfPixels; i++) {
    pixelIDs[i] = pixIDs[i];
  }
  string nn;
  int xmin = 0, xmax = 10000;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    pixelpoints[i] = pointsPerPixel[i];
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i]) {
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
      CinSzinti[i] = 0.7;
      innerReadOut[i] = (readSide[i] == 1);
      if (setup.getDetectorr(i).getMaterial() != NULL)
        CinSzinti[i] = setup.getDetectorr(i).getMaterial()->getSpeedOfLight();
    } else {
      numberOfElements[i] = 0;
      innerReadOut[i] = false;
      CinSzinti[i] = 0.7;
    }
    qdcHistograms[i] = new TH1**[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      qdcHistograms[i][j] = new TH1*[pixelpoints[i]];
      for (int k = 0; k < pixelpoints[i]; k++) {
        nn = (string("qdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
              string_number(j) + " pix-" + string_number(k))
                 .data();
        qdcHistograms[i][j][k] = new TH1F(nn.data(), nn.data(), 100, xmin, xmax);
      }
    }
  }
}
AqdcRadialPixCalibration::~AqdcRadialPixCalibration()
{
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++) {
      for (int k = 0; k < pixelpoints[i]; k++)
        delete qdcHistograms[i][j][k];
      delete[] qdcHistograms[i][j];
    }
    delete[] qdcHistograms[i];
  }
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i] != NULL)
      delete reactions[i];
  delete[] reactions;
  delete[] qdcHistograms;
  delete[] detectorIDs;
  delete[] innerReadOut;
  delete[] CinSzinti;
  delete[] numberOfElements;
  delete[] pixelIDs;
  delete[] startDetectors;
  delete[] pixelpoints;
}
void AqdcRadialPixCalibration::process()
{
  if (event.getNumberOfReactions() == 0)
    return;
  // check for reactions:
  int found = -1;
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i]->name() == event.getReaction(0)->name())
      found = i;
  if (found < 0)
    return;
  RbaseReaction* react = event.getReaction(0);
  int pos, num;
  TCalibHit* tmp;
  float flightPath;
  float lightPath;
  float tdcCalculated;
  float startTime;
  point3D startPos;
  point3D stopPos;
  point3D origin(0, 0, 0);
  TTrack* track;
  momentum4D particle;
  TPixel* pix;
  int nh;
  int pixpos;
  for (int i = 0; i < react->getNumberOfTracks(); i++) {
    track = react->getTrack(i);
    particle = react->getParticle(i);
    startTime = 0;
    startPos.setValues(0, 0, 0);
    nh = 0;
    for (int j = 0; j < numberOfStartDetectors; j++) {
      if (track->getNumberOfCalibHits(startDetectors[j]) > 0) {
        tmp = track->getCalibSrcr(startDetectors[j], 0);
        startTime += tmp->getTDC();
        startPos = startPos + (tmp->getHitShape()->entrance(track->getPath()) - origin);
        nh++;
      }
    }
    if (nh == 0)
      continue;
    startTime = startTime / nh;
    startPos = origin + (vector3D(startPos) * (1 / nh));
    for (int j = 0; j < numberOfPixels; j++)
      for (int k = 0; k < track->getNumberOfPixels(); k++) {
        pix = &track->getPixel(k, 0);
        if (pix->getID() != pixelIDs[j])
          continue;
        pixpos = getPixelPoint(pix);
        if (pixpos == -1)
          continue;
        for (int m = 0; m < pix->getNumberOfActualElements(); m++) {
          pos = -1;
          tmp = &pix->getElementr(m);
          for (int l = 0; l < numberOfDetectors; l++)
            if (tmp->getDetector() == detectorIDs[l])
              pos = l;
          if (pos < 0)
            continue;
          stopPos = tmp->getHitShape()->entrance(track->getPath());
          lightPath = getLightWay(tmp->getHitShape(), stopPos, innerReadOut[pos]);
          if (lightPath < 0)
            continue;
          flightPath = (stopPos - startPos).R();
          tdcCalculated = flightPath / particle.Beta() / 299.792;
          tdcCalculated += lightPath / CinSzinti[pos] / 299.792;
          tdcCalculated += startTime;
          num = tmp->getElement();
          qdcHistograms[pos][num][pixpos]->Fill(tdcCalculated - tmp->getTDC());
        }
      }
  }
}
void AqdcRadialPixCalibration::toEvaluate()
{
  if (abs(eventStart - event.getEventNumber()) < 10)
    return;
  //  for (int i = 0; i < numberOfDetectors; i++)
  //    emit evaluated(evaluate(i));
  eventStart = event.getEventNumber();
}
void AqdcRadialPixCalibration::getReactions(const vector<string>& names)
{
  int num = 0;
  for (unsigned int i = 0; i < names.size(); i++) {
    if (names[i].find("ppelastic") < names[i].npos)
      num++;
    else if (names[i].find("dpi+") < names[i].npos)
      num++;
  }
  numberOfReactions = num;
  reactions = new RbaseReaction*[numberOfReactions];
  for (int i = 0; i < num; i++)
    reactions[i] = NULL;
  num = 0;
  for (unsigned int i = 0; i < names.size(); i++) {
    if (names[i].find("ppelastic") < names[i].npos) {
      reactions[num] = new Rppelastic();
      num++;
    } else if (names[i].find("dpi+") < names[i].npos) {
      reactions[num] = new Rpp_dpiPlus();
      num++;
    }
  }
}
CommonCalibrationParser* AqdcRadialPixCalibration::evaluate(int num)
{
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(pixelpoints[num], numberOfElements[num]);
  ret->setParamID(4);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[pixelpoints[num]];
  for (int i = 0; i < numberOfElements[num]; i++) {
    for (int j = 0; j < pixelpoints[num]; j++) {
      v[j] = qdcHistograms[num][i][j]->GetMean();
      qdcHistograms[num][i][j]->Reset();
    }
    // insert calibration routine
    /*********************************

    **********************************/
    for (int j = 0; j < pixelpoints[num]; j++)
      ret->setParameter(i, j, v[j]);
  }
  return ret;
}
