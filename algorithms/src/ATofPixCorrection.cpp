#include "ATofPixCorrection.h"
#include "spiral.h"
#include "wedge.h"
TPixel tmpPixel(3);
#include "beamtimeparameter.h"
ATofPixCorrection::ATofPixCorrection(int& eventNr, int& runNr, TTrack** tracksIn,
                                     int& numberOfTracksIn, TSetup& setup,
                                     const algorithm_parameter& descr,
                                     const vector<string>& calibrationFiles)
    : AAlgorithm("TDC correction pixel wise"), numberOfTracks(numberOfTracksIn),
      numPixelTypes(descr.getParam<vector<int>>(0).getData().size()), EventNumber(eventNr),
      RunNumber(runNr)
{
  tracks = tracksIn;
  pixelTypes = new int[numPixelTypes];
  numRanges7 = new int[numPixelTypes];
  use9 = new bool[numPixelTypes];
  numRanges9 = new int[numPixelTypes];
  actualRange7 = new int[numPixelTypes];
  actualRange9 = new int[numPixelTypes];
  calibrations7 = new CommonCalibrationParser**[numPixelTypes];
  calibrations9 = new CommonCalibrationParser**[numPixelTypes];
  CinSzinti = new float[numPixelTypes];
  for (int i = 0; i < numPixelTypes; i++) {
    pixelTypes[i] = descr.getParam<vector<int>>(0).getData().at(i);
    numRanges7[i] = 0;
    numRanges9[i] = 0;
    use9[i] = false;
    if (setup.getDetectorr(pixelTypes[i]).getMaterial() != NULL)
      CinSzinti[i] = setup.getDetectorr(pixelTypes[i]).getMaterial()->getSpeedOfLight();
    else
      CinSzinti[i] = 0.7;
    if (setup.getDetectorr(pixelTypes[i]).getNumberOfElements() > 0)
      if (setup.getDetectorr(pixelTypes[i]).getShape(0)->getName() == "spiral")
        use9[i] = true;
  }
  getParameterFromFiles(calibrationFiles);
  if (descr.getNumberOfParam<bool>() > 0)
    runTime = descr.getParam<bool>(0).getData();
  else
    runTime = true;
  if (descr.getNumberOfParam<bool>() > 1)
    calib = descr.getParam<bool>(1).getData();
  else
    calib = true;
  //   for(int i=0;i<numPixelTypes;i++)
  //     cout<<"light velocity in detector: "<<pixelTypes[i]<<":
  //     "<<CinSzinti[i]<<endl;
}
ATofPixCorrection::ATofPixCorrection(int& eventNr, int& runNr, TTrack** tracksIn,
                                     int& numberOfTracksIn, TSetup& setup, vector<int> pixelTypesIn,
                                     const vector<string>& calibrationFiles)
    : AAlgorithm("TDC correction pixel wise"), numberOfTracks(numberOfTracksIn),
      numPixelTypes(pixelTypesIn.size()), EventNumber(eventNr), RunNumber(runNr)
{
  tracks = tracksIn;
  pixelTypes = new int[numPixelTypes];
  numRanges7 = new int[numPixelTypes];
  use9 = new bool[numPixelTypes];
  actualRange9 = new int[numPixelTypes];
  numRanges9 = new int[numPixelTypes];
  actualRange7 = new int[numPixelTypes];
  calibrations7 = new CommonCalibrationParser**[numPixelTypes];
  calibrations9 = new CommonCalibrationParser**[numPixelTypes];
  CinSzinti = new float[numPixelTypes];
  for (int i = 0; i < numPixelTypes; i++) {
    pixelTypes[i] = pixelTypesIn[i];
    numRanges7[i] = 0;
    numRanges9[i] = 0;
    if (setup.getDetectorr(pixelTypes[i]).getMaterial() != NULL)
      CinSzinti[i] = setup.getDetectorr(pixelTypes[i]).getMaterial()->getSpeedOfLight();
    else
      CinSzinti[i] = 0.7;
    use9[i] = false;
    if (setup.getDetectorr(pixelTypes[i]).getNumberOfElements() > 0)
      if (setup.getDetectorr(pixelTypes[i]).getShape(0)->getName() == "spiral")
        use9[i] = true;
  }
  getParameterFromFiles(calibrationFiles);
  runTime = true;
  calib = true;
}
ATofPixCorrection::~ATofPixCorrection()
{
  for (int i = 0; i < numPixelTypes; i++) {
    if (numRanges7[i] > 0) {
      for (int j = 0; j < numRanges7[i]; j++)
        delete calibrations7[i][j];
      delete[] calibrations7[i];
    }
    if (numRanges9[i] > 0) {
      for (int j = 0; j < numRanges9[i]; j++)
        delete calibrations9[i][j];
      delete[] calibrations9[i];
    }
  }
  delete[] calibrations9;
  delete[] numRanges9;
  delete[] use9;
  delete[] calibrations7;
  delete[] pixelTypes;
  delete[] numRanges7;
  delete[] actualRange7;
  delete[] actualRange9;
  delete[] CinSzinti;
}
bool formAPixel(TTrack* track)
{
  if (track->getNumberOfCalibHits(0) > 0 && track->getNumberOfCalibHits(1) > 0 &&
      track->getNumberOfCalibHits(2) > 0) {
    tmpPixel.reset();
    tmpPixel.addElement(*track->getCalibSrcr(0, 0));
    tmpPixel.addElement(*track->getCalibSrcr(1, 0));
    tmpPixel.addElement(*track->getCalibSrcr(2, 0));
    tmpPixel.setID(0);
    track->addPixel(&tmpPixel);
    return true;
  } else if (track->getNumberOfCalibHits(3) > 0 && track->getNumberOfCalibHits(4) > 0 &&
             track->getNumberOfCalibHits(5) > 0) {
    tmpPixel.reset();
    tmpPixel.addElement(*track->getCalibSrcr(3, 0));
    tmpPixel.addElement(*track->getCalibSrcr(4, 0));
    tmpPixel.addElement(*track->getCalibSrcr(5, 0));
    tmpPixel.setID(1);
    track->addPixel(&tmpPixel);
    return true;
  }
  return false;
}
void* ATofPixCorrection::process(void* ptr)
{
  if (calib)
    if (!checkRanges()) {
      cout << "Range not found!!! run: " << RunNumber << " event: " << EventNumber << endl;
      return ptr;
    }
  int pt;
  int pixpos;
  int left = 0, right = 0;
  TPixel* pix;
  float lightWay;
  float ra = 0;
  point3D hit, center;
  vector3D normal;
  plane3D plane;
  float tdc;
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->getNumberOfPixels() <= 0)
      if (!formAPixel(tracks[i]))
        continue;
    for (int j = 0; j < tracks[i]->getNumberOfPixels(); j++) {
      pix = &tracks[i]->getPixel(0, j);
      pixpos = 0;
      for (int k = 0; k < pix->getNumberOfActualElements(); k++) {
        if (pix->getElementr(k).getHitShape()->getName() == "spiral") {
          if (((spiral*)(pix->getElementr(k).getHitShape()))->getBending() > 0)
            left = pix->getElementr(k).getElement();
          else
            right = pix->getElementr(k).getElement();
        } else if (pix->getElementr(k).getHitShape()->getName() == "wedge") {
          center = ((wedge*)pix->getElementr(k).getHitShape())->getCenter();
          normal = ((wedge*)pix->getElementr(k).getHitShape())->getNormal();
          ra = ((wedge*)pix->getElementr(k).getHitShape())->getOuterRadius();
        }
      }
      pixpos = left - right;
      if (runTime) {
        plane = plane3D(center, normal);
        hit = plane - tracks[i]->getPath();
        lightWay = ra - (center - hit).R();
      } else
        lightWay = 0;
      for (int k = 0; k < pix->getNumberOfActualElements(); k++) {
        pt = -1;
        for (int l = 0; l < numPixelTypes; l++)
          if (pix->getElementr(k).getDetector() == pixelTypes[l])
            pt = l;
        if (pt == -1)
          continue;
        tdc = pix->getElementr(k).getTDC();
        if (calib)
          tdc += (calibrations7[pt][actualRange7[pt]]->getParameter(
                      pix->getElementr(k).getElement(), pixpos) +
                  (use9[pt]
                       ? calibrations9[pt][actualRange9[pt]]->getParameter(
                             pix->getElementr(k).getElement(), pixpos)
                       : 0)) /
                 10.;
        if (runTime)
          tdc += lightWay / CinSzinti[pt] / 299.792;
        pix->getElementr(k).setTDC(tdc);
      }
    }
  }
  return ptr;
}
bool ATofPixCorrection::checkRanges()
{
  int num;
  for (int i = 0; i < numPixelTypes; i++) {
    if (actualRange7[i] < 0 || numRanges7[i] <= actualRange7[i])
      return false;
    if (calibrations7[i][actualRange7[i]]->getRange(0) <= RunNumber &&
        calibrations7[i][actualRange7[i]]->getRange(1) >= RunNumber &&
        calibrations7[i][actualRange7[i]]->getRange(2) <= EventNumber &&
        calibrations7[i][actualRange7[i]]->getRange(3) >= EventNumber)
      continue;
    num = -1;
    for (int j = 0; j < numRanges7[i]; j++) {
      if (calibrations7[i][actualRange7[j]]->getRange(0) <= RunNumber &&
          calibrations7[i][actualRange7[j]]->getRange(1) >= RunNumber &&
          calibrations7[i][actualRange7[j]]->getRange(2) <= EventNumber &&
          calibrations7[i][actualRange7[j]]->getRange(3) >= EventNumber)
        num = j;
    }
    if (num == -1)
      return false;
    actualRange7[i] = num;
    if (!use9[i])
      continue;
    if ((actualRange9[i] < 0 || numRanges9[i] <= actualRange9[i]))
      return false;
    if (calibrations9[i][actualRange9[i]]->getRange(0) <= RunNumber &&
        calibrations9[i][actualRange9[i]]->getRange(1) >= RunNumber &&
        calibrations9[i][actualRange9[i]]->getRange(2) <= EventNumber &&
        calibrations9[i][actualRange9[i]]->getRange(3) >= EventNumber)
      continue;
    num = -1;
    for (int j = 0; j < numRanges9[i]; j++) {
      if (calibrations9[i][actualRange9[j]]->getRange(0) <= RunNumber &&
          calibrations9[i][actualRange9[j]]->getRange(1) >= RunNumber &&
          calibrations9[i][actualRange9[j]]->getRange(2) <= EventNumber &&
          calibrations9[i][actualRange9[j]]->getRange(3) >= EventNumber)
        num = j;
    }
    if (num == -1)
      return false;
    actualRange9[i] = num;
  }
  return true;
}
void ATofPixCorrection::getParameterFromFiles(const vector<string>& calibrationFiles)
{
  if (calibrationFiles.size() == 0)
    return;
  vector<CommonCalibrationParser*> tmp;
  for (int i = 0; i < numPixelTypes; i++) {
    if (numRanges7[i] != 0) {
      for (int j = 0; j < numRanges7[i]; j++)
        delete calibrations7[i][j];
      delete[] calibrations7[i];
    }
    numRanges7[i] = 0;
  }
  for (int i = 0; i < numPixelTypes; i++) {
    if (numRanges9[i] != 0) {
      for (int j = 0; j < numRanges9[i]; j++)
        delete calibrations9[i][j];
      delete[] calibrations9[i];
    }
    numRanges9[i] = 0;
  }
  for (unsigned int i = 0; i < calibrationFiles.size(); i++)
    CommonCalibrationParser::getFileContent(calibrationFiles[i], tmp);
  vector<CommonCalibrationParser*> tmp1;
  CommonCalibrationParser* tCP;
  while (!tmp.empty()) {
    tCP = tmp.back();
    tmp.pop_back();
    if (tCP->getParamID() == 7 || tCP->getParamID() == 9)
      tmp1.push_back(tCP);
    else
      delete tCP;
  }
  bool used;
  while (!tmp1.empty()) {
    used = false;
    tCP = tmp1.back();
    for (int j = 0; j < numPixelTypes; j++) {
      if (tCP != NULL) {
        if (tCP->getDetector() == pixelTypes[j] && tCP->getParamID() == 7) {
          used = true;
          numRanges7[j]++;
        } else if (tCP->getDetector() == pixelTypes[j] && tCP->getParamID() == 9 && use9[j]) {
          used = true;
          numRanges9[j]++;
        }
      }
    }
    if (used)
      tmp.push_back(tCP);
    else
      delete tCP;
    tmp1.pop_back();
  }
  for (int i = 0; i < numPixelTypes; i++) {
    if (numRanges7[i] != 0)
      calibrations7[i] = new CommonCalibrationParser*[numRanges7[i]];
    if (numRanges9[i] != 0)
      calibrations9[i] = new CommonCalibrationParser*[numRanges9[i]];
  }
  for (int i = 0; i < numPixelTypes; i++) {
    numRanges7[i] = 0;
    numRanges9[i] = 0;
  }
  for (unsigned int i = 0; i < tmp.size(); i++) {
    if (tmp[i] == NULL)
      continue;
    for (int j = 0; j < numPixelTypes; j++)
      if (tmp[i]->getDetector() == pixelTypes[j] && tmp[i]->getParamID() == 7) {
        calibrations7[j][numRanges7[j]] = tmp[i];
        numRanges7[j]++;
      }
    for (int j = 0; j < numPixelTypes; j++)
      if (tmp[i]->getDetector() == pixelTypes[j] && tmp[i]->getParamID() == 9) {
        calibrations9[j][numRanges9[j]] = tmp[i];
        numRanges9[j]++;
      }
  }
  for (int i = 0; i < numPixelTypes; i++) {
    actualRange7[i] = 0;
    actualRange9[i] = 0;
  }
}
void ATofPixCorrection::setNewRun(run_parameter& run)
{
  vector<string> tmp;
  if (run.hasOwnCalibration())
    for (int i = 0; i < run.getNumberOfCalibrationFiles(); i++)
      tmp.push_back(run.getCalibrationFile(i));
  else if (run.getParent() != NULL)
    for (int i = 0; i < run.getParent()->getNumberOfCalibrationFiles(); i++)
      tmp.push_back(run.getParent()->getCalibrationFile(i));
  getParameterFromFiles(tmp);
}
