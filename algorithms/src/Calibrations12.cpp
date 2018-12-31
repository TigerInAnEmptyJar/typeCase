#include "Calibrations.h"
#include "TF1.h"
#include "fiber.h"
#include "reactions.h"
#include "spiral.h"
#include "wedge.h"
extern float getLightWay(volumeShape* sh, point3D onPoint, bool from);
AqdcRadialPolCalibration::AqdcRadialPolCalibration(TEvent& eventIn, TSetup& setup,
                                                   algorithm_parameter& descr)
    : AAlgorithm("Generate qdcRadialPol calibration"), event(eventIn),
      minEntriesPerHisto(descr.getParam<int>(0).getData())
{
  vector<int> tmp(descr.getParam<vector<int>>(0).getData());
  vector<int> tmp1(descr.getParam<vector<int>>(1).getData());
  vector<int> tmp3(descr.getParam<vector<int>>(2).getData());
  getReactions(descr.getParam<vector<string>>(0).getData());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1**[numberOfDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  numberOfStartDetectors = tmp1.size();
  startDetectors = new int[numberOfStartDetectors];
  for (unsigned int i = 0; i < tmp1.size(); i++)
    startDetectors[i] = tmp1[i];
  int xmin = 0, xmax = 1000;
  if (descr.getNumberOfParam<int>() > 1)
    xmin = descr.getParam<int>(1).getData();
  if (descr.getNumberOfParam<int>() > 2)
    xmax = descr.getParam<int>(2).getData();
  int ymin = -100, ymax = -100;
  if (descr.getNumberOfParam<int>() > 3)
    xmin = descr.getParam<int>(3).getData();
  if (descr.getNumberOfParam<int>() > 4)
    xmax = descr.getParam<int>(4).getData();
  eventStart = 0;
  string nn;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
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
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("qdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
            string_number(j))
               .data();
      qdcHistograms[i][j] = new TH2F(nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
}
AqdcRadialPolCalibration::AqdcRadialPolCalibration(TEvent& eventIn, TSetup& setup,
                                                   vector<int>& detectors, vector<int>& Stdetectors,
                                                   vector<int> readSide,
                                                   vector<string> reactionNames,
                                                   int minEntriesPerHistoIn)
    : AAlgorithm("Generate qdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  getReactions(reactionNames);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1**[numberOfDetectors];
  numberOfStartDetectors = Stdetectors.size();
  startDetectors = new int[numberOfStartDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  for (unsigned int i = 0; i < Stdetectors.size(); i++)
    startDetectors[i] = Stdetectors[i];
  string nn;
  int xmin = 0, xmax = 1000;
  int ymin = -100, ymax = 100;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
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
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("qdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
            string_number(j))
               .data();
      qdcHistograms[i][j] = new TH2F(nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
}
AqdcRadialPolCalibration::~AqdcRadialPolCalibration()
{
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++)
      delete qdcHistograms[i][j];
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
  delete[] startDetectors;
}
void* AqdcRadialPolCalibration::process(void* ptr)
{
  if (event.getNumberOfReactions() == 0)
    return ptr;
  // check for reactions:
  int found = -1;
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i]->name() == event.getReaction(0)->name())
      found = i;
  if (found < 0)
    return ptr;
  RbaseReaction* react = event.getReaction(0);
  int num;
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
  int nh;
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
    for (int j = 0; j < numberOfDetectors; j++) {
      if (track->getNumberOfCalibHits(detectorIDs[j]) == 0)
        continue;
      tmp = track->getCalibSrcr(detectorIDs[j], 0);
      stopPos = tmp->getHitShape()->entrance(track->getPath());
      lightPath = getLightWay(tmp->getHitShape(), stopPos, innerReadOut[j]);
      if (lightPath < 0)
        continue;
      flightPath = (stopPos - startPos).R();
      tdcCalculated = flightPath / particle.Beta() / 299.792;
      tdcCalculated += lightPath / CinSzinti[j] / 299.792;
      tdcCalculated += startTime;
      num = tmp->getElement();
      qdcHistograms[j][num]->Fill(lightPath, tdcCalculated - tmp->getTDC());
    }
  }
  return ptr;
}
void AqdcRadialPolCalibration::toEvaluate()
{
  for (int i = 0; i < numberOfDetectors; i++)
    emit evaluated(evaluate(i));
  eventStart = event.getEventNumber();
}
CommonCalibrationParser* AqdcRadialPolCalibration::evaluate(int num)
{
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(6, numberOfElements[num]);
  ret->setParamID(4);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[6];
  TF1 function("function", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x");
  TF1* f1;
  for (int i = 0; i < numberOfElements[num]; i++) {
    qdcHistograms[num][i]->Fit(&function, "Q");
    f1 = qdcHistograms[num][i]->GetFunction("function");
    for (int j = 0; j < 6; j++)
      v[j] = f1->GetParameter(j);
    qdcHistograms[num][i]->Reset();
    for (int j = 0; j < 6; j++)
      ret->setParameter(i, j, v[j]);
  }
  return ret;
}
void AqdcRadialPolCalibration::getReactions(const vector<string>& names)
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
