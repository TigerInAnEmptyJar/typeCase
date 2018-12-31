#include "APulsHeightCorrection.h"
#include "CommonCalibrationParser.h"
#include <iostream>

void getTheParameter(int& numberOfRanges, int** ranges, int DetID, int numberOfElements,
                     TF1*** correctionFunction, vector<string>& calibrationFiles)
{
  numberOfRanges = 0;
  ranges = new int*[4];
  vector<CommonCalibrationParser*> Parser;
  CommonCalibrationParser* tmpParser;
  for (unsigned int i = 0; i < calibrationFiles.size(); i++) {
    if (calibrationFiles[i] == "")
      continue;
    ifstream input;
    input.open(calibrationFiles[i].data());
    tmpParser = new CommonCalibrationParser(numberOfElements, DetID);
    while ((tmpParser->getFromFile(input))) {
      if (tmpParser->getParamID() != 12) {
        delete tmpParser;
        tmpParser = 0;
      } else {
        Parser.push_back(tmpParser);
      }
      tmpParser = new CommonCalibrationParser(numberOfElements, DetID);
    }
    if ((!(tmpParser->isValid())) || (tmpParser->getParamID() != 12))
      delete tmpParser;
    input.close();
  }
  numberOfRanges = Parser.size();
  for (int i = 0; i < 4; i++) {
    ranges[i] = new int[numberOfRanges];
    for (int j = 0; j < numberOfRanges; j++)
      ranges[i][j] = Parser[j]->getRange(i);
  }
  correctionFunction = new TF1**[numberOfRanges];
  for (int i = 0; i < numberOfRanges; i++) {
    correctionFunction[i] = new TF1*[numberOfElements];
    for (int j = 0; j < numberOfElements; j++) {
      correctionFunction[i][j] =
          new TF1("[0]+[1]*x+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",
                  (string("f") + string_number(i) + "/" + string_number(j)).data());
      for (int k = 0; k < 6; k++)
        correctionFunction[i][j]->SetParameter(k, Parser[i]->getParameter(j, k));
    }
  }
  while (!Parser.empty()) {
    tmpParser = Parser.back();
    Parser.pop_back();
    delete tmpParser;
  }
}

APulsHeightCorrectionWedge::APulsHeightCorrectionWedge(TTrack** tracksIn, int& numberOfTracksIn,
                                                       TDetector& det, int& runNumber,
                                                       int& eventNumber,
                                                       vector<string>& calibrationFiles)
    : AAlgorithm("Puls height correction for wedges"), numberOfTracks(numberOfTracksIn),
      runNr(runNumber), eventNr(eventNumber)
{
  DetID = det.getID();
  numberOfElements = det.getNumberOfElements();
  tracks = tracksIn;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}

APulsHeightCorrectionWedge::~APulsHeightCorrectionWedge()
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < numberOfRanges; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
}

void* APulsHeightCorrectionWedge::process(void* ptr)
{
  bool rangeFound = false;
  if (!((runNr >= ranges[0][actualRange]) && (runNr <= ranges[1][actualRange]) &&
        (eventNr <= ranges[3][actualRange]) && (eventNr >= ranges[2][actualRange]))) {
    rangeFound = false;
    for (int j = 0; j < numberOfRanges; j++) {
      if ((runNr >= ranges[0][j]) && (runNr <= ranges[1][j]) && (eventNr <= ranges[3][j]) &&
          (eventNr >= ranges[2][j])) {
        actualRange = j;
        rangeFound = true;
      }
    }
    if ((!rangeFound) && (numberOfRanges > 0)) {
      return ptr;
    }
  }
  TCalibHit* tmp = 0;
  sLine3D path;
  point3D enter;
  float r;
  for (int i = 0; i < numberOfTracks; i++) {
    path = tracks[i]->getPath();
    for (int j = 0; j < tracks[i]->getNumberOfCalibHits(DetID); j++) {
      enter = tmp->getHitShape()->entrance(path);
      r = (enter - tmp->getHitShape()->getCenter()).length();
      tmp = (tracks[i]->getCalibSrcr(DetID, j));
      tmp->setADC(tmp->getADC() * correctionFunction[tmp->getElement()][actualRange]->Eval(r));
    }
  }
  return ptr;
}

APulsHeightCorrectionSpiral::APulsHeightCorrectionSpiral(TTrack** tracksIn, int& numberOfTracksIn,
                                                         TDetector& det, int& runNumber,
                                                         int& eventNumber,
                                                         vector<string>& calibrationFiles)
    : AAlgorithm("Puls height correction for spirals"), numberOfTracks(numberOfTracksIn),
      runNr(runNumber), eventNr(eventNumber)
{
  DetID = det.getID();
  numberOfElements = det.getNumberOfElements();
  tracks = tracksIn;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}

APulsHeightCorrectionSpiral::~APulsHeightCorrectionSpiral()
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < numberOfRanges; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
}

void* APulsHeightCorrectionSpiral::process(void* ptr)
{
  bool rangeFound = false;
  if (!((runNr >= ranges[0][actualRange]) && (runNr <= ranges[1][actualRange]) &&
        (eventNr <= ranges[3][actualRange]) && (eventNr >= ranges[2][actualRange]))) {
    rangeFound = false;
    for (int j = 0; j < numberOfRanges; j++) {
      if ((runNr >= ranges[0][j]) && (runNr <= ranges[1][j]) && (eventNr <= ranges[3][j]) &&
          (eventNr >= ranges[2][j])) {
        actualRange = j;
        rangeFound = true;
      }
    }
    if ((!rangeFound) && (numberOfRanges > 0)) {
      return ptr;
    }
  }
  TCalibHit* tmp = 0;
  sLine3D path;
  point3D enter;
  float r;
  for (int i = 0; i < numberOfTracks; i++) {
    path = tracks[i]->getPath();
    for (int j = 0; j < tracks[i]->getNumberOfCalibHits(DetID); j++) {
      enter = tmp->getHitShape()->entrance(path);
      r = (enter - tmp->getHitShape()->getCenter()).length();
      tmp = tracks[i]->getCalibSrcr(DetID, j);
      tmp->setADC(tmp->getADC() * correctionFunction[tmp->getElement()][actualRange]->Eval(r));
    }
  }
  return ptr;
}

APulsHeightCorrectionTube::APulsHeightCorrectionTube(TTrack** tracksIn, int& numberOfTracksIn,
                                                     TDetector& det, int& runNumber,
                                                     int& eventNumber,
                                                     vector<string>& calibrationFiles)
    : AAlgorithm("Puls height correction for proportional chamber"),
      numberOfTracks(numberOfTracksIn), runNr(runNumber), eventNr(eventNumber)
{
  DetID = det.getID();
  numberOfElements = det.getNumberOfElements();
  tracks = tracksIn;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}

APulsHeightCorrectionTube::~APulsHeightCorrectionTube()
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < numberOfRanges; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
}

void* APulsHeightCorrectionTube::process(void* ptr)
{
  bool rangeFound = false;
  if (!((runNr >= ranges[0][actualRange]) && (runNr <= ranges[1][actualRange]) &&
        (eventNr <= ranges[3][actualRange]) && (eventNr >= ranges[2][actualRange]))) {
    rangeFound = false;
    for (int j = 0; j < numberOfRanges; j++) {
      if ((runNr >= ranges[0][j]) && (runNr <= ranges[1][j]) && (eventNr <= ranges[3][j]) &&
          (eventNr >= ranges[2][j])) {
        actualRange = j;
        rangeFound = true;
      }
    }
    if ((!rangeFound) && (numberOfRanges > 0)) {
      return ptr;
    }
  }
  TCalibHit* tmp = 0;
  sLine3D path;
  float r;
  for (int i = 0; i < numberOfTracks; i++) {
    path = tracks[i]->getPath();
    for (int j = 0; j < tracks[i]->getNumberOfCalibHits(DetID); j++) {
      tmp = tracks[i]->getCalibSrcr(DetID, j);
      r = (tmp->getHitShape()->entrance(path) - tmp->getHitShape()->getCenter()).length();
      tmp->setADC(tmp->getADC() * correctionFunction[tmp->getElement()][actualRange]->Eval(r));
    }
  }
  return ptr;
}

void APulsHeightCorrectionWedge::getNewCalibration(vector<string>& calibrationFiles)
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < 6; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}
void APulsHeightCorrectionSpiral::getNewCalibration(vector<string>& calibrationFiles)
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < 6; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}

void APulsHeightCorrectionTube::getNewCalibration(vector<string>& calibrationFiles)
{
  for (int j = 0; j < numberOfElements; j++) {
    for (int i = 0; i < 6; i++)
      delete correctionFunction[j][i];
    delete[] correctionFunction[j];
  }
  delete[] correctionFunction;
  for (int i = 0; i < 4; i++)
    delete[] ranges[i];
  delete[] ranges;
  getTheParameter(numberOfRanges, ranges, DetID, numberOfElements, correctionFunction,
                  calibrationFiles);
  actualRange = 0;
}
