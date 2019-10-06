#include "ATofStrawTrackFinder.h"
//#include "tofTaskManager.h"
//#include "strawTube.h"
#include "algorithmparameter.h"
ATofStrawTrackFinder::ATofStrawTrackFinder(TTrack** tracksIn, int& numberOfTracksIn, TSetup& setup,
                                           TCalibHit*** hitsIn, int** numberOfHitsIn,
                                           const algorithm_parameter& descr)
    : AAlgorithm("interface to tofStraws::tofTrackFinder"), numberOfTracks(numberOfTracksIn)
{
  if (setup.getNumberOfDetectors() == 0)
    return;
  tracks = tracksIn;
  numDets = descr.getParam<vector<int>>(0).getData().size();
  numberOfHits = new int*[numDets];
  hits = new TCalibHit**[numDets];
  planeCenter = new point3D[numDets];
  trackFinder = tofTaskManager::Instance();
  vector3D tmpV = descr.getParam<vector3D>(0).getData();
  tofGeomVector axis(tmpV.X(), tmpV.Y(), tmpV.Z());
  tofGeomRotationMatrix rotationMatrix(axis, descr.getParam<float>(0).getData());
  for (int i = 0; i < numDets; i++) {
    numberOfHits[i] = numberOfHitsIn[descr.getParam<vector<int>>(0).getData().at(i)];
    hits[i] = hitsIn[descr.getParam<vector<int>>(0).getData().at(i)];
    tofPlaneOrientation myPlane(rotationMatrix, descr.getParam<vector<int>>(1).getData().at(i));
    trackFinder->AddPlane(myPlane);
  }
  trackFinder->Initialize();
  resetTrackNumber = descr.getParam<bool>(0).getData();
}
ATofStrawTrackFinder::~ATofStrawTrackFinder()
{
  delete[] hits;
  delete[] numberOfHits;
  delete[] planeCenter;
}
void ATofStrawTrackFinder::process()
{
  if (resetTrackNumber)
    numberOfTracks = 0;
  trackFinder->ClearHits();
  volumeShape* shape;
  point3D center;
  for (int i = 0; i < numDets; i++)
    for (int j = 0; j < (*numberOfHits[i]); j++) {
      if (!hits[i][j]->isValid())
        continue;
      shape = hits[i][j]->getHitShape();
      if (shape == NULL)
        continue;
      //	if(shape->IsA()!=strawTube::Class())continue;
      center = shape->getCenter();
      // get the hit coordinates and isochrone radius ...

      /** - A straw type hit consists of the z and x coordinate of the
          wire and a isochrone radius. Here the x coordinate is taken
          within the straw plane, e.g. it is the distance of the straw
          center to the center of the plane. In addition, the number
          of the plane in which the hit was measured is part of the
          straw-type hit.

          For the straw type hit the x and z coordinates are the wire
         coordinates
          taken in the projection along the straw wire*/
      tofHit newHit((center - planeCenter[i]).R(), 0., vector3D(planeCenter[i]) * beamDirection,
                    10 /*((strawTube*)shape)->getIsoChroneRadius()*/, 0., 0., 0., i);

      trackFinder->AddHit(newHit);
    }
  trackFinder->Execute();
  tofHit* tmpHit;
  //  TCalibHit* tmpH;
  for (int i = 0; i < trackFinder->GetNumberOfTracks(); i++) {
    tofTrack newTrack = trackFinder->GetTrack(i);
    float /*x,y,z,*/ p, t;
    p = newTrack.GetPhi();
    t = newTrack.GetTheta();
    tracks[numberOfTracks + i]->setPath(
        sLine3D(point3D(newTrack.GetFootX(), newTrack.GetFootY(), 0.),
                vector3D(cos(p) * sin(t), sin(p) * sin(t), cos(t))));
  }

  for (int i = 0; i < numDets; i++)
    for (int j = 0; j < (*numberOfHits[i]); j++) {
      if (!hits[i][j]->isValid())
        continue;
      shape = hits[i][j]->getHitShape();
      if (shape == NULL)
        continue;
      //	if(shape->IsA()!=strawTube::Class())continue;
      center = shape->getCenter();
      for (int k = 0; k < trackFinder->GetNumberOfTracks(); k++) {
        tofTrack newTrack = trackFinder->GetTrack(k);
        for (int l = 0; l < newTrack.GetNumberOfHits(); l++) {
          tmpHit = (tofHit*)tofDataManager::Instance()->At(newTrack.GetHitIndex(l));
          if (tmpHit->GetX() == (center - planeCenter[i]).R() && tmpHit->GetY() == 0 &&
              tmpHit->GetZ() == vector3D(planeCenter[i]) * beamDirection)
            tracks[numberOfTracks + k]->addCalibSrc(hits[i][j]);
        }
      }
    }
  numberOfTracks = numberOfTracks + trackFinder->GetNumberOfTracks();
}

algorithm_parameter ATofStrawTrackFinder::getDescription()
{
  algorithm_parameter ret("TOF straw-track-finder", algorithm_parameter::Category::TRACKING, 0, 0);
  string des =
      "This algorithm is the interface to the straw track finder implemented "
      "by Ralph Castelijns at the Forschungszentrum Juelich for the COSY-TOF-Detector. "
      "The adjustments made are tiny and do not extend so far as the tofStraw-library."
      "The geometry used for this library is quite fixed though I tried to widen the "
      "possibilities a bit.\n"
      "Assumptions are: the straws come in planar double-layers. The layers can be "
      "tilted with respect to each other around some axis. Though there is some axis "
      "you can specify as rotation axis I hope, the z-axis (as used for the COSY-TOF-"
      "Detector) is not hard coded. There are three possible angular alignments, a tilt "
      "of 60deg each (give here 1.047 == pi/3 as rotation angle), again I don't know "
      "whether other alignments, peraps as pentagram or so, would work."
      "The documentation for this trackfinder is given in "
      "http://www.fz-juelich.de/ikp/COSY-TOF/manuals/Manuals/straws/html/index.html, "
      "but try also my version "
      "http://www.pit.physik.uni-tuebingen.de/~ehrhardt/KTOF/classes/ATofStrawTrackFinder.html.";
  ret.setDescription(des);
  ret.addParam<bool>(single_parameter<bool>("start with numberOfTracks=0", true));
  ret.addParam<float>(single_parameter<float>("rotation angle", 1.047197551196597631));
  ret.addParam<vector3D>(single_parameter<vector3D>("axis", vector3D(0, 0, 1)));
  vector<int> tmpI;
  ret.addParam<vector<int>>(single_parameter<vector<int>>("detector IDs of straw layers", tmpI));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("orientation of layer", tmpI));
  return ret;
}
