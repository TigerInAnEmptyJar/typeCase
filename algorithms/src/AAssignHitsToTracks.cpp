#include "AAssignHitsToTracks.h"
#include "algorithmparameter.h"
#include "fittingRoutines.h"
#include "setup.h"
#include <float.h>
#include <stdlib.h>
AAssignHitsToTracks::AAssignHitsToTracks(TSetup& setupIn, TTrack** tracksIn, TCalibHit*** hitIn,
                                         int** numberOfHitsIn, int& numberOfTracksIn,
                                         const algorithm_parameter& descr)
    : AAlgorithm("Assign hits in remaining detectors to defined tracks"), setup(setupIn),
      numberOfTracks(numberOfTracksIn)
{
  hits = hitIn;
  tracks = tracksIn;
  numberOfHits = numberOfHitsIn;
  numberOfDetectors = descr.value(4).value<vector<int>>().size();
  detectorIDs = new int[numberOfDetectors];
  mode = new int[numberOfDetectors];
  maxElementDistance = new int[numberOfDetectors];
  mustCheck = false;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = descr.value(4).value<vector<int>>().at(i);
    mode[i] = descr.value(5).value<vector<int>>().at(i);
    maxElementDistance[i] = descr.value(6).value<vector<int>>().at(i);
    mustCheck = (mustCheck && (mode[i] != 3 || mode != 0));
  }
  doFit = descr.value(0).value<bool>();
  maxChi = descr.value(1).value<float>();
  maxDistT = descr.value(2).value<float>();
  maxDistV = descr.value(3).value<float>();
}
AAssignHitsToTracks::~AAssignHitsToTracks()
{
  delete[] detectorIDs;
  delete[] mode;
  delete[] maxElementDistance;
}
void AAssignHitsToTracks::process()
{
  bool isPrompt, suspectmode;
  ;
  sLine3D path;
  volumeShape* shape;
  int elem, eldis, closest;
  Vector hitParam(13), bestHit(13);
  float bestD = FLT_MAX;
  vector3D distance;
  TDetector* det;
  Vector params[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++)
    params[i].reDimension(13);
  int nEl;
  lineFit myFit(numberOfDetectors);
  for (int i = 0; i < numberOfTracks; i++) {
    nEl = 0;
    path = tracks[i]->getPath();
    isPrompt = true;
    if (mustCheck)
      isPrompt = ((path - point3D(0, 0, 0)).R() <= maxDistT);
    for (int j = 0; j < numberOfDetectors; j++) {
      det = &setup.getDetectorr(detectorIDs[j]);
      if ((*(numberOfHits[detectorIDs[j]])) <= 0)
        continue;
      suspectmode = true;
      if (isPrompt)
        suspectmode = ((mode[j] & 1) > 0);
      else
        suspectmode = ((mode[j] & 2) > 0);
      if (suspectmode) {
        shape = det->getShape(0);
        elem = shape->suspect(path, det->getStackType());
        if (elem == -1 || elem >= static_cast<int>(det->getNumberOfElements()))
          continue;
        eldis = det->getNumberOfElements();
        closest = -1;
        for (int k = 0; k < (*(numberOfHits[detectorIDs[j]])); k++) {
          if (!hits[detectorIDs[j]][k]->isValid())
            continue;
          if (abs(hits[detectorIDs[j]][k]->getElement() - elem) < eldis) {
            eldis = (int)abs(hits[detectorIDs[j]][k]->getElement() - elem);
            closest = k;
          } else if (det->isCircular()) {
            if (abs(static_cast<int>(
                    abs(static_cast<int>(hits[detectorIDs[j]][k]->getElement() - elem)) -
                    det->getNumberOfElements()))) {
              eldis = static_cast<int>(
                  abs(static_cast<int>(hits[detectorIDs[j]][k]->getElement() - elem)));
              closest = k;
            }
          }
        }
        if (closest >= 0 && closest < (*(numberOfHits[detectorIDs[j]])) &&
            eldis < maxElementDistance[j]) {
          tracks[i]->addCalibSrc(hits[detectorIDs[j]][closest]);
          if (doFit) {
            params[nEl] = hits[detectorIDs[j]][closest]->getHitShape()->HitParams(path);
            nEl++;
          }
        }
      } else {
        elem = -1;
        bestD = FLT_MAX;
        for (int k = 0; k < (*(numberOfHits[detectorIDs[j]])); k++) {
          if (!hits[detectorIDs[j]][k]->isValid())
            continue;
          shape = hits[detectorIDs[j]][k]->getHitShape();
          if (shape == NULL)
            continue;
          hitParam = shape->HitParams(path);
          distance.setValues(hitParam[10], hitParam[11], hitParam[12]);
          if (distance.R() < __prec) {
            params[nEl] = hitParam;
            elem = k;
            bestD = distance.R();
            break;
          }
          if (distance.R() < bestD) {
            params[nEl] = hitParam;
            elem = k;
            bestD = distance.R();
          }
        }
        if (elem >= 0 && elem < static_cast<int>(det->getNumberOfElements()) && bestD < maxDistT) {
          tracks[i]->addCalibSrc(hits[detectorIDs[j]][elem]);
          nEl++;
        }
      }
    }
    if (doFit) {
      myFit.clear();
      //////////  Add ingoing path ////////////////
      for (int j = 0; j < nEl; j++) {
        myFit.addPoint(point3D(params[i][7], params[i][8], params[i][9]),
                       vector3D(params[i][4], params[i][5], params[i][6]), params[i][0]);
      }
      float chi = myFit.fit();
      if (chi >= 0 && chi < maxChi) {
        tracks[i]->setPath(sLine3D(myFit.Foot(), myFit.Direction()));
      }
    }
  }
}

algorithm_parameter AAssignHitsToTracks::getDescription()
{
  algorithm_parameter ret("Assign hits to defined tracks", algorithm_parameter::Category::INPUT, 0);
  string des = "This algorithm uses defined tracks and assigns hits "
               "in the given detectors to this track. There are two different "
               "ways of searching the corresponding detector elements: check "
               "on each hit or do the suspect for the detector envelope. For "
               "each detector you can specify the search mode for prompt and "
               "secondary tracks. first bit is prompt second is secondary"
               "tracks and set means use suspect search (3: all tracks suspect "
               "search, 1: prompt do suspect secondary do conventional).\n"
               "The secondary tracks are defined by distance of the track "
               "to the origin.\n"
               "You may take the track directions as given or do an additional "
               "line fit.";
  ret.setDescription(des);
  ret.addValue("Refit track directions", false);
  ret.addValue("maximum chi-squared for refit", 1.f);
  ret.addValue("max dist prompt to origin", 10.f);
  ret.addValue("max distance track-element", 1.f);
  vector<int> tmp;
  ret.addValue("detector IDs", tmp);
  ret.addValue("search mode", tmp);
  ret.addValue("max element distance", tmp);
  return ret;
}

bool AAssignHitsToTracks::checkParameter(algorithm_parameter const& a)
{
  return a.numberOfValues() == 7 && a.value(0).valueType() == ParameterValue::ValueType::BOOLEAN &&
         a.value(1).valueType() == ParameterValue::ValueType::FLOAT &&
         a.value(2).valueType() == ParameterValue::ValueType::FLOAT &&
         a.value(3).valueType() == ParameterValue::ValueType::FLOAT &&
         a.value(4).valueType() == ParameterValue::ValueType::VECTOR_INT &&
         a.value(5).valueType() == ParameterValue::ValueType::VECTOR_INT &&
         a.value(6).valueType() == ParameterValue::ValueType::VECTOR_INT;
}
