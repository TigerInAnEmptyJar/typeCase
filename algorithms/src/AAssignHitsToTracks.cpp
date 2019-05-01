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
  numberOfDetectors = 0;
  if (descr.getNumberOfParam<vector<int>>() >= 1)
    numberOfDetectors = descr.getParam<vector<int>>(0).getData().size();
  detectorIDs = new int[numberOfDetectors];
  mode = new int[numberOfDetectors];
  maxElementDistance = new int[numberOfDetectors];
  mustCheck = false;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = descr.getParam<vector<int>>(0).getData().at(i);
    if (descr.getNumberOfParam<vector<int>>() > 1)
      mode[i] = descr.getParam<vector<int>>(1).getData().at(i);
    else
      mode[i] = 3;
    if (descr.getNumberOfParam<vector<int>>() > 2)
      maxElementDistance[i] = descr.getParam<vector<int>>(3).getData().at(i);
    else
      maxElementDistance[i] = 0;
    mustCheck = (mustCheck && (mode[i] != 3 || mode != 0));
  }
  doFit = false;
  if (descr.getNumberOfParam<bool>() > 0)
    doFit = descr.getParam<bool>(0).getData();
  maxChi = 1;
  if (descr.getNumberOfParam<float>() > 0)
    maxChi = descr.getParam<float>(0).getData();
  if (descr.getNumberOfParam<float>() > 1)
    maxDistT = descr.getParam<float>(1).getData();
  if (descr.getNumberOfParam<float>() > 2)
    maxDistV = descr.getParam<float>(2).getData();
}
AAssignHitsToTracks::~AAssignHitsToTracks()
{
  delete[] detectorIDs;
  delete[] mode;
  delete[] maxElementDistance;
}
void* AAssignHitsToTracks::process(void* ptr)
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
            if (abs(abs(hits[detectorIDs[j]][k]->getElement() - elem) -
                    det->getNumberOfElements())) {
              eldis = (int)abs(hits[detectorIDs[j]][k]->getElement() - elem);
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
  return ptr;
}

algorithm_parameter AAssignHitsToTracks::getDescription()
{
  algorithm_parameter ret("Assign hits to defined tracks", 0, 0);
  vector<string> des;
  des.push_back("This algorithm uses defined tracks and assigns hits");
  des.push_back("in the given detectors to this track. There are two different");
  des.push_back("ways of searching the corresponding detector elements: check");
  des.push_back("on each hit or do the suspect for the detector envelope. For");
  des.push_back("each detector you can specify the search mode for prompt and");
  des.push_back("secondary tracks. first bit is prompt second is secondary");
  des.push_back("tracks and set means use suspect search (3: all tracks suspect");
  des.push_back("search, 1: prompt do suspect secondary do conventional).");
  des.push_back("The secondary tracks are defined by distance of the track");
  des.push_back("to the origin.");
  des.push_back("You may take the track directions as given or do an additional");
  des.push_back("line fit.");
  ret.setDescription(des);
  ret.addParam<bool>(single_parameter<bool>("Refit track directions", false));
  ret.addParam<float>(single_parameter<float>("maximum chi-squared for refit", 1));
  ret.addParam<float>(single_parameter<float>("max dist prompt to origin", 10));
  ret.addParam<float>(single_parameter<float>("max distance track-element", 1));
  vector<int> tmp;
  ret.addParam<vector<int>>(single_parameter<vector<int>>("detector IDs", tmp));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("search mode", tmp));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("max element distance", tmp));
  return ret;
}
