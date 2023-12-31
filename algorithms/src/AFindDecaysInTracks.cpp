#include "AFindDecaysInTracks.h"
#include "algorithmparameter.h"
AFindDecayInTracks::AFindDecayInTracks(TSetup& setupIn, int& numberOfTracksIn, TTrack** tracksIn,
                                       const algorithm_parameter& descr)
    : AAlgorithm("Find decays in tracks"), setup(setupIn), numberOfTracks(numberOfTracksIn)
{
  tracks = tracksIn;
  vertexFitTarget = true;
  vertexFitDecay = true;
  maxDistanceToTarget = 1;
  maxDistanceToCommon = 10;
  vertexFitDecay = descr.value(1).value<bool>();
  vertexFitTarget = descr.value(0).value<bool>();
  maxDistanceToCommon = descr.value(3).value<float>();
  maxDistanceToTarget = descr.value(2).value<float>();
}
AFindDecayInTracks::~AFindDecayInTracks() = default;
void AFindDecayInTracks::process()
{
  bool isSecondary[numberOfTracks];
  int decayedOf[numberOfTracks];
  sLine3D paths[numberOfTracks];
  volumeShape* target = setup.getTargetr().getShape().getClone();
  point3D origin(0, 0, 0);
  vector3D distance;
  lLine3D poca;
  int nPrompt = 0;
  for (int i = 0; i < numberOfTracks; i++) {
    isSecondary[i] = false;
    decayedOf[i] = -1;
    paths[i] = tracks[i]->getPath();
  }
  if (target == NULL)
    for (int i = 0; i < numberOfTracks; i++) {
      distance = paths[i] - origin;
      isSecondary[i] = (distance.R() > maxDistanceToTarget);
      nPrompt++;
    }
  else
    for (int i = 0; i < numberOfTracks; i++) {
      distance = target->distance(paths[i]);
      isSecondary[i] = (distance.R() > maxDistanceToTarget);
      nPrompt++;
    }
  if (vertexFitTarget) {
    // fit the prompt vertex
  }
  int nVertices = numberOfTracks - nPrompt;
  if (nVertices > 0) {
    int prompts[nPrompt];
    int secs[numberOfTracks - nPrompt];
    int index = 0, indexS = 0;
    for (int i = 0; i < numberOfTracks; i++)
      if (isSecondary[i])
        secs[indexS++] = i;
      else
        prompts[index++] = i;
    int vertices[numberOfTracks];
    for (int i = 0; i < numberOfTracks; i++)
      vertices[i] = -1;
    point3D vertexPoints[nVertices];
    //      int vertexParticle[nVertices];
    int num;
    index = 0;
    // search kinks and decays with one found primary track
    for (int i = 0; i < nPrompt; i++) {
      for (int j = 0; j < nVertices; j++) {
        poca = paths[prompts[i]] - paths[secs[j]];
        if (poca.length() < maxDistanceToCommon) {
          point3D tmpP = poca.Mid();
          num = -1;
          for (int k = 0; k < index; k++)
            if ((tmpP - vertexPoints[k]).R() < maxDistanceToCommon)
              num = k;
          if (num < 0) {
            vertexPoints[index] = tmpP;
            num = index++;
          }
          vertices[secs[j]] = num;
          decayedOf[secs[j]] = i;
        }
      }
    }
    int nModifications = 1;
    while (nModifications > 0) {
      nModifications = 0;
      for (int i = 0; i < nVertices; i++) {
        for (int j = 0; j < nVertices; j++) {
          poca = paths[secs[i]] - paths[secs[j]];
          point3D tmpP = poca.Mid();
          if (poca.length() < maxDistanceToCommon) {
            num = -1;
            for (int k = 0; k < index; k++)
              if ((tmpP - vertexPoints[k]).R() < maxDistanceToCommon)
                num = k;
            if (num < 0) {
              vertexPoints[index] = tmpP;
              num = index++;
            }
            if ((decayedOf[secs[i]] >= 0) xor (decayedOf[secs[j]] >= 0)) {
              if (decayedOf[secs[i]] >= 0) {
                decayedOf[secs[j]] = decayedOf[secs[i]];
                vertices[secs[j]] = num;
              } else {
                decayedOf[secs[i]] = decayedOf[secs[j]];
                vertices[secs[i]] = num;
              }
              nModifications++;
            }
          }
        }
      }
    }
    TTrack* vertexTrack[index];
    for (int i = 0; i < index; i++)
      vertexTrack[i] = NULL;
    int nN = 0;
    for (int i = 0; i < numberOfTracks; i++) {
      tracks[i]->setOrder(isSecondary[i]);
      if (isSecondary[i] && decayedOf[i] >= 0 && decayedOf[i] < numberOfTracks) {
        tracks[i]->setPrimary(tracks[decayedOf[i]]);
        tracks[decayedOf[i]]->addSecondary(tracks[i]);
        vertexTrack[vertices[i]] = tracks[decayedOf[i]];
      } else if (isSecondary[i]) {
        if (vertexTrack[vertices[i]] != NULL) {
          tracks[i]->setPrimary(vertexTrack[vertices[i]]);
          vertexTrack[vertices[i]]->addSecondary(tracks[i]);
        } else if (vertices[i] >= 0) {
          tracks[numberOfTracks + nN]->setOrder(false);
          tracks[numberOfTracks + nN]->setCharge(0);
          tracks[numberOfTracks + nN]->addSecondary(tracks[i]);
          vertexTrack[vertices[i]] = tracks[numberOfTracks + nN];
          nN++;
        }
      }
    }
    if (vertexFitDecay) {
      for (int i = 0; i < numberOfTracks; i++) {
        if (tracks[i]->getNumberOfSecondaryTracks() > 0) {
          ///////////// fit the secondary vertex
        }
      }
    }
    numberOfTracks = numberOfTracks + nN;
  }
  return;
}
algorithm_parameter AFindDecayInTracks::getDescription()
{
  algorithm_parameter ret("Find decays in tracks", algorithm_parameter::Category::TRACKING, 0);
  string des = "This algorithm takes a set of tracks and defines for "
               "each track if it is a prompt or secondary track. Criterium "
               "here is the distance to the target [mm]. Then the point of "
               "closest approach of any two tracks is calculated and if it "
               "is below a certain limit, these two tracks are to have a "
               "common vertex point. For both primary and secondary "
               "vertex you can separately decide to do a vertex fit.";
  ret.setDescription(des);
  ret.addValue("fit primary vertex", true);
  ret.addValue("fit secondary vertices", true);
  ret.addValue("max distance to target volume", 1.f);
  ret.addValue("max dist of lines: sec vertex", 10.f);
  return ret;
}
