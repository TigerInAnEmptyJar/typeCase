#include "ALineTrackSearch.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "wedge.h"
#include <iostream>
#include <stdlib.h>
//#define SHOW_PARAMETERS
//#define DEBUGINFO
//#define DEBUGSEARCH
//#define MICROPIXELCHECK
//#define MICROPIXELOUT
#ifdef MICROPIXELCHECK
#include <TFile.h>
#include <TTree.h>
TTree* outTreeTracking;
Float_t theta_tracking[2];
Float_t phi_tracking[2];
Int_t el_tracking[2];
int pixid = 5;
#endif
#ifndef MICROPIXELCHECK
#ifdef MICROPIXELOUT
int pixid = 5;
#endif
#endif
#include "ring.h"
#include "spiral.h"
#include "wedge.h"
void setTrackErrors(TTrack* tr)
{
  if (tr->isSecondary()) {
    tr->setErrorTheta(2. * M_PI / 180.);
    tr->setErrorPhi(4. * M_PI / 180.);
    return;
  }
  float minPhi = 3, minth = 3;
  float tmp, f1, f2, f3, p, b, d;
  for (int i = 0; i < tr->getNumberOfCalibHits(-2); i++)
    for (int j = 0; j < tr->getNumberOfCalibHits(i); j++) {
      if (tr->getCalibSrcr(i, j) == NULL)
        continue;
      if (tr->getCalibSrcr(i, j)->getHitShape() == NULL)
        continue;
      if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "wedge") {
        tmp = abs(((wedge*)tr->getCalibSrcr(i, j)->getHitShape())->getPhiRange() / 2.);
        if (minPhi > tmp &&
            ((wedge*)tr->getCalibSrcr(i, j)->getHitShape())->getDistToBarrelCenter() < 0)
          minPhi = tmp;
      } else if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "spiral") {
        tmp = ((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getPhiRange() / 2.;
        if (minPhi > tmp)
          minPhi = tmp;
        d = tmp;
        b = abs(((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getBending());
        p = tr->getPath().Direction().Phi();
        f1 = ((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getCenter().Z();
        f2 = b * p;
        f3 = b * (p + d);
        tmp = atan(((b * d) / f1) / (1 + f2 * f3 / f1 / f1)) / 2.;
        if (minth > tmp)
          minth = tmp;
      } else if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "ring") {
        f1 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getCenter().Z();
        f2 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getInnerRadius();
        f3 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getOuterRadius();
        tmp = atan((f3 - f2) / f1 / (1 + f2 * f3 / f1 / f1)) / 2.;
        if (minth > tmp)
          minth = tmp;
      }
    }
  tr->setErrorTheta(minth);
  tr->setErrorPhi(minPhi);
}
bool ALineTrackSearch::searchATrack(int tracknum, int stopI, TBase* pix, TBase* spix)
{
  if (tracknum >= maxTracks)
    return false;
  if (pix == NULL)
    return false;
  if (!(pix->name() == "TPixel" || pix->name() == "TCluster"))
    return false;
  if (spix != NULL)
    if (!(spix->name() == "TPixel" || spix->name() == "TCluster"))
      return false;
  sLine3D path;
  point3D vertex(0, 0, 0), p1, p2(0, 0, 0);
  int offset = 0;
  int typ = 0;
  if (pix->name() == "TCluster") {
    offset = numStop;
    typ = 1;
    p1 = ((TCluster*)pix)->getCenter();
  } else // pix->name()=="TCluster"
  {
    p1 = ((TPixel*)pix)->getShape()->getCenter();
  }
  if (spix == NULL)
    p2 = vertex;
  else {
    if (spix->name() == "TCluster")
      p2 = ((TCluster*)spix)->getCenter();
    else
      p2 = ((TPixel*)spix)->getShape()->getCenter();
  }
  path = sLine3D(p2, p1 - p2);
  path.setRep(_spheric_);
#ifdef DEBUGINFO
  string out = string(typ == 1 ? "c" : "p") + string_number(stopI) + " " + path.toString();
#endif
  bool found;
  TDetector* det;
  volumeShape* shape;
  int suspect;
  Vector hitParam(13), bestHit(13);
  float bestD = 1000000;
  int bestEl;
  int pixelDists[setup.getNumberOfDetectors()];
  float pd;
  vector3D dis;
  for (int i = 0; i < numMid; i++) {
    if ((*(numberOfPixels[midIDs[i]])) == 0)
      continue;
    found = false;
    for (int j = 0; j < tmpTracks[tracknum]->getNumberOfPixels(); j++)
      if (tmpTracks[tracknum]->getPixel(0, j).getID() == midIDs[i])
        found = true;
    if (found)
      continue;
    pd = 1000000;
    pixelDists[i] = -1;
    for (int j = 0; j < (*(numberOfPixels[midIDs[i]])); j++) {
      if (!pixels[midIDs[i]][j]->isValid())
        continue;
      dis = pixels[midIDs[i]][j]->getShape()->distance(path);
      if (dis.R() < __prec) {
        pixelDists[i] = j;
        break;
      }
      if (dis.R() < maxDistance[offset + stopI] && pd > dis.R())
        pixelDists[i] = j;
    }
    if (pixelDists[i] >= 0)
      tmpTracks[tracknum]->addPixel(pixels[midIDs[i]][pixelDists[i]]);
  }
  TPixel* ppix;
  int closest;
  vector3D dist, v1;
  float adist;
  int pos = -1, con;
  myLineFit.clear();
  /*****************************************************
   * Essential detectors
   *
   * for every pixel- / cluster-type there is a number of
   * detectors defined that have to be found in order to form
   * a regular track (e.g. to make sure that there are start-
   * as well as stop-signal defining hits on the track).
   *****************************************************/
  int tak = (typ == 0 ? numEssentials[stopI] : numClusterEssentials[stopI]),
      *tak2 = (typ == 0 ? essentials[stopI] : clusterEssentials[stopI]);
  for (int i = 0; i < tak; i++) {
    for (int j = 0; j < numDetectors; j++)
      if (tak2[i] == detectorIDs[j])
        pos = j;
#ifdef DEBUGSEARCH
    cout << "Essential detector: " << tak2[i] << " position: " << pos << endl << " (";
    for (int j = 0; j < (*(numberOfHits[tak2[i]])) - 1; j++)
      cout << hits[tak2[i]][j]->getElement() << ",";
    if ((*(numberOfHits[tak2[i]])) > 0)
      cout << hits[tak2[i]][(*(numberOfHits[tak2[i]])) - 1]->getElement() << ")" << endl;
#endif
    if (pos >= 0)
      con = conventional[pos];
    else
      con = 0;
    found = false;
    for (int k = 0; k < tmpTracks[tracknum]->getNumberOfPixels(); k++) {
      ppix = &tmpTracks[tracknum]->getPixel(0, k);
      /*search for essential detector in pixels*/
      for (int j = 0; j < ppix->getNumberOfActualElements(); j++)
        if (tak2[i] == ppix->getElementr(j).getDetector()) {
          tmpTracks[tracknum]->addCalibSrc(&ppix->getElementr(j));
          if (con) {
            bestHit = ppix->getElementr(j).getHitShape()->HitParams(path);
            myLineFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                               vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0]);
          }
          found = true;
        }
    }
    if (!found) {
      /*search other hits*/
      if (con == 1 && pos >= 0) // conventional search
      {
        bestEl = -1;
        bestD = 10000000;
        for (int j = 0; j < (*(numberOfHits[tak2[i]])); j++) {
          if (!hits[tak2[i]][j]->isValid())
            continue;
          shape = hits[tak2[i]][j]->getHitShape();
          if (shape == NULL)
            continue;
          hitParam = shape->HitParams(path);
          dist.setValues(hitParam[10], hitParam[11], hitParam[12]);
          if (angularDistance) {
            v1.setValues(hitParam[1], hitParam[2], hitParam[3]);
            adist = acos((v1 * path.Direction()) / v1.R());
            if (adist < __prec) {
              bestEl = j;
              bestHit = hitParam;
              break;
            }
            if (adist > maxDistance[offset + stopI])
              continue;
            if (bestD > adist) {
              bestEl = j;
              bestD = adist;
              bestHit = hitParam;
            }
          } else {
            if (dist.R() < __prec) {
              bestEl = j;
              bestHit = hitParam;
              break;
            }
            if (dist.R() > maxDistance[offset + stopI])
              continue;
            if (bestD > dist.R()) {
              bestEl = j;
              bestD = dist.R();
              bestHit = hitParam;
            }
          }
        }
        if (bestEl < 0)
          continue;
        tmpTracks[tracknum]->addCalibSrc(hits[tak2[i]][bestEl]);
        myLineFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                           vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0]);
      } else // suspect search
      {
        det = &setup.getDetectorr(tak2[i]);
        shape = det->getShape(0);
        suspect = shape->suspect(path, det->getStackType());
        closest = 0;
#ifdef DEBUGSEARCH
        cout << "Suspect element:" << suspect << " detector: " << tak2[i]
             << " away: " << away[(offset + stopI) * numDetectors + pos] << " "
             << shape->getName().ascii() << endl;
#endif
#ifdef MICROPIXELOUT
        if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid)
          cout << "suspect:" << tak2[i] << " " << suspect << flush;
#endif
        if (suspect >= 0 && suspect < static_cast<int>(det->getNumberOfElements())) {
          for (int j = 0; j < (*(numberOfHits[tak2[i]])); j++) {
            if (!hits[tak2[i]][j]->isValid())
              continue;
            if (hits[tak2[i]][j]->getHitShape() == NULL)
              continue;
            if (abs(hits[tak2[i]][closest]->getElement() - suspect) >
                abs(hits[tak2[i]][j]->getElement() - suspect))
              closest = j;
            if (hits[tak2[i]][j]->getElement() == suspect)
              tmpTracks[tracknum]->addCalibSrc(hits[tak2[i]][j]);
            else if (pos >= 0) {
              if (abs(hits[tak2[i]][j]->getElement() - suspect) <=
                  away[(offset + stopI) * numDetectors + pos])
                tmpTracks[tracknum]->addCalibSrc(hits[tak2[i]][j]);
              else if (det->isCircular()) {
                if (abs(static_cast<int>(
                        abs(static_cast<int>(hits[tak2[i]][j]->getElement() - suspect)) -
                        det->getNumberOfElements())) <=
                    away[(offset + stopI) * numDetectors + pos]) {
#ifdef DEBUGSEARCH
                  cout << "circular detector: element taken " << hits[tak2[i]][j]->getElement()
                       << endl;
#endif
                  tmpTracks[tracknum]->addCalibSrc(hits[tak2[i]][j]);
                }
              }
            }
          }
        }
#ifdef MICROPIXELOUT
        if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid)
          cout << " closest:" << hits[tak2[i]][closest]->getElement() << endl;
#endif
      }
    }
    if (tmpTracks[tracknum]->getNumberOfCalibHits(tak2[i]) <= 0) {
#ifdef DEBUGINFO
      cout << out.ascii() << " essential Detetector not found: " << tak2[i] << endl;
#endif
#ifdef MICROPIXELOUT
      if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid) {
        cout << "micro-Pixel: det not found: " << path.Direction().toString(_spheric_).ascii()
             << tak2[i] << endl;
        for (int iii = 0; iii < (*(numberOfHits[tak2[i]])); iii++)
          cout << hits[tak2[i]][iii]->getElement() << "\t" << flush;
        cout << endl;
      }
#endif
      return false; // has not essential detector
    }
  }
  /***********************************************
   * search other detectors for hits for this track
   ***********************************************/
  for (int i = 0; i < numDetectors; i++) {
    if (tmpTracks[tracknum]->getNumberOfCalibHits(detectorIDs[i]) > 0)
      continue;
    found = false;
    /*search pixel*/
    for (int k = 0; k < tmpTracks[tracknum]->getNumberOfPixels(); k++) {
      ppix = &tmpTracks[tracknum]->getPixel(0, k);
      for (int j = 0; j < ppix->getNumberOfActualElements(); j++)
        if (detectorIDs[i] == ppix->getElementr(j).getDetector()) {
          tmpTracks[tracknum]->addCalibSrc(&ppix->getElementr(j));
          if (conventional[i]) {
            bestHit = ppix->getElementr(j).getHitShape()->HitParams(path);
            myLineFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                               vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0]);
          }
          found = true;
        }
    }
    if (!found) {
      /*search other detectors*/
      if (conventional[i] == 1) {
        bestEl = -1;
        bestD = 10000000;
        for (int j = 0; j < (*(numberOfHits[detectorIDs[i]])); j++) {
          if (!hits[detectorIDs[i]][j]->isValid())
            continue;
          shape = hits[detectorIDs[i]][j]->getHitShape();
          if (shape == NULL)
            continue;
          hitParam = shape->HitParams(path);
          dist.setValues(hitParam[10], hitParam[11], hitParam[12]);
          if (angularDistance) {
            v1.setValues(hitParam[1], hitParam[2], hitParam[3]);
            adist = acos((v1 * path.Direction()) / v1.R());
            if (adist < __prec) {
              bestEl = j;
              bestHit = hitParam;
              break;
            }
            if (adist > maxDistance[offset + stopI])
              continue;
            if (bestD > adist) {
              bestEl = j;
              bestD = adist;
              bestHit = hitParam;
            }
          } else {
            if (dist.R() < __prec) {
              bestEl = j;
              bestHit = hitParam;
              break;
            }
            if (dist.R() > maxDistance[offset + stopI])
              continue;
            if (bestD > dist.R()) {
              bestEl = j;
              bestD = dist.R();
              bestHit = hitParam;
            }
          }
        }
        if (bestEl < 0)
          continue;
        tmpTracks[tracknum]->addCalibSrc(hits[detectorIDs[i]][bestEl]);
        myLineFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                           vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0]);
      } else // suspect search
      {
        det = &setup.getDetectorr(detectorIDs[i]);
        shape = det->getShape(0);
        suspect = shape->suspect(path, det->getStackType());
#ifdef MICROPIXELOUT
        if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid)
          cout << "suspect:" << detectorIDs[i] << " " << suspect << flush;
#endif
        closest = 0;
        if (suspect >= 0 && suspect < static_cast<int>(det->getNumberOfElements())) {
          for (int j = 0; j < (*(numberOfHits[detectorIDs[i]])); j++) {
            if (!hits[detectorIDs[i]][j]->isValid())
              continue;
            if (hits[detectorIDs[i]][j]->getHitShape() == NULL)
              continue;
            if (abs(hits[detectorIDs[i]][closest]->getElement() - suspect) >
                abs(hits[detectorIDs[i]][j]->getElement() - suspect))
              closest = j;
            if (hits[detectorIDs[i]][j]->getElement() == suspect)
              tmpTracks[tracknum]->addCalibSrc(hits[detectorIDs[i]][j]);
            else {
              if (det->isCircular()) {
                if (abs(hits[detectorIDs[i]][j]->getElement() - suspect) <=
                    away[(offset + stopI) * numDetectors + i])
                  tmpTracks[tracknum]->addCalibSrc(hits[detectorIDs[i]][j]);
                else if (abs(static_cast<int>(
                             abs(static_cast<int>(hits[detectorIDs[i]][j]->getElement() -
                                                  suspect)) -
                             det->getNumberOfElements())) <=
                         away[(offset + stopI) * numDetectors + i])
                  tmpTracks[tracknum]->addCalibSrc(hits[detectorIDs[i]][j]);
              } else if (abs(static_cast<int>(hits[detectorIDs[i]][j]->getElement() - suspect)) <=
                         away[(offset + stopI) * numDetectors + i])
                tmpTracks[tracknum]->addCalibSrc(hits[detectorIDs[i]][j]);
            }
          }
        }
#ifdef MICROPIXELOUT
        if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid)
          cout << " closest:" << hits[detectorIDs[i]][closest]->getElement() << endl;
#endif
      }
    }
  }
  int nhits = 0;
  for (int i = 0; i < tmpTracks[tracknum]->getNumberOfCalibHits(-2); i++)
    if (tmpTracks[tracknum]->getNumberOfCalibHits(i) > 0)
      nhits++;
  //  bool
  //  condition=tmpTracks[tracknum]->getNumberOfCalibHits(-1)>=(typ==0?minElements[stopI]:minClusterElements[stopI]);
  bool condition = nhits >= (typ == 0 ? minElements[stopI] : minClusterElements[stopI]);
  if (!condition) {
#ifdef DEBUGINFO
    cout << out.ascii()
         << " too less elements: " << nhits /*tmpTracks[tracknum]->getNumberOfCalibHits(-1)*/ << "("
         << (typ == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
#endif
#ifdef MICROPIXELOUT
    if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid) {
      cout << "Micropixel too less elements: " << path.Direction().toString(_spheric_).ascii()
           << tmpTracks[tracknum]->getNumberOfCalibHits(-1) << "("
           << (typ == 0 ? minElements[stopI] : minClusterElements[stopI]) << ") (";
      for (int i = 0; i < tmpTracks[tracknum]->getNumberOfCalibHits(-2); i++)
        if (tmpTracks[tracknum]->getNumberOfCalibHits(i) > 0)
          cout << i << "," << flush;
      cout << ")" << endl;
    }
#endif
    return false;
  }
  TCalibHit* hit;
  float chi;
  point3D enter, htt;
  vector3D tmp, tmp2;
  tmp.setRep(_spheric_);
  tmp2.setRep(_spheric_);
  v1.setRep(_spheric_);
  if (tmpTracks[tracknum]->getNumberOfCalibHits(-1) > 4) {
    for (int i = 0; i < numDetectors; i++) {
      if (conventional[i])
        continue;
      for (int j = 0; j < tmpTracks[tracknum]->getNumberOfCalibHits(detectorIDs[i]); j++) {
        hit = tmpTracks[tracknum]->getCalibSrcr(detectorIDs[i], j);
        bestHit = hit->getHitShape()->HitParams(path);
        enter.setValues(bestHit[1], bestHit[2], bestHit[3]);
        htt.setValues(bestHit[7], bestHit[8], bestHit[9]);
        tmp = enter - vertex;
        tmp2 = htt - vertex;
        vector3D dist(bestHit[10], bestHit[11], bestHit[12]);
        if (angularDistance) {
          v1.setValues(bestHit[7], bestHit[8], bestHit[9]);
          adist = (v1 * path.Direction()) / v1.R();
          if (adist > 1)
            adist = 1;
          if (adist < -1)
            adist = -1;
          adist = acos(adist);
          if (adist > maxDistance[offset + stopI]) {
            tmpTracks[tracknum]->removeCalibSrc(hit);
            continue;
          }
        } else {
          if (dist.R() > maxDistance[offset + stopI]) {
            tmpTracks[tracknum]->removeCalibSrc(hit);
            continue;
          }
        }
        myLineFit.addPoint(vector3D(bestHit[7], bestHit[8], bestHit[9]),
                           vector3D(bestHit[4], bestHit[5], bestHit[6]), bestHit[0]);
      }
    }
    condition = tmpTracks[tracknum]->getNumberOfCalibHits(-1) >=
                (typ == 0 ? minElements[stopI] : minClusterElements[stopI]);
    for (int i = 0; i < tak; i++) {
      condition = (condition && tmpTracks[tracknum]->getNumberOfCalibHits(tak2[i]) > 0);
    }
    if (!condition) {
#ifdef DEBUGINFO
      cout << out.ascii() << " too less elements: " << tmpTracks[tracknum]->getNumberOfCalibHits(-1)
           << "(" << (typ == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
#endif
#ifdef MICROPIXELOUT
      if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid) {
        cout << "Micropixel too less elements(ac): " << path.Direction().toString(_spheric_).ascii()
             << tmpTracks[tracknum]->getNumberOfCalibHits(-1) << "("
             << (typ == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")(";
        for (int i = 0; i < tmpTracks[tracknum]->getNumberOfCalibHits(-2); i++)
          if (tmpTracks[tracknum]->getNumberOfCalibHits(i) > 0)
            cout << i << "," << flush;
        cout << ")" << endl;
      }
#endif
      return false;
    }
    /******* add target ****************/
    myLineFit.addPoint(vector3D(0, 0, 0), vector3D(0, 0, 0), 0.0000001);
    chi = myLineFit.fit();
    if (chi < 0 || chi > (typ == 0 ? maxChi[stopI] : maxClusterChi[stopI])) {
#ifdef DEBUGINFO
      cout << out.ascii() << " chi-squared too high: " << chi << "("
           << (typ == 0 ? maxChi[stopI] : maxClusterChi[stopI]) << ")" << endl;
#endif
#ifdef MICROPIXELOUT
      if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid) {
        cout << "Micropixel chi-squared too high: " << path.Direction().toString(_spheric_).ascii()
             << chi << "(" << (typ == 0 ? maxChi[stopI] : maxClusterChi[stopI]) << ")" << endl;
      }
#endif
      return false;
    }
    point3D f = myLineFit.Foot();       //(res[0],res[1],0);
    vector3D d = myLineFit.Direction(); //(res[2],res[3],modFactor);
    tmpTracks[tracknum]->setPath(sLine3D(f, d));
    tmpTracks[tracknum]->setChiSquared(chi);
  } else {
    tmpTracks[tracknum]->setPath(path);
    tmpTracks[tracknum]->setChiSquared(0);
  }
#ifdef MICROPIXELOUT
  if (tmpTracks[tracknum]->getPixel(0, 0).getID() == pixid) {
    cout << "micropixel:" << chi << path.Direction().toString(_spheric_).ascii()
         << tmpTracks[tracknum]->getNumberOfCalibHits(-1) << " good" << endl;
  }
#endif
  return true;
}

void ALineTrackSearch::process()
{
  /***********************************************
   *for any pixel/any two pixels
   *search a track: if fit is good enough
   *  and enough elements
   *sort by chi-squared
   *eliminate tracks with too many equal elements
   *sort by theta.
   ************************************************/
  numTracks = 0;
  numberOfTracks = 0;
  point3D c;
  vector3D dir;
  dir.setRep(_spheric_);
#ifdef MICROPIXELOUT
  cout << pixid << " : " << (*(numberOfPixels[pixid])) << " pixels found" << endl;
#endif
  for (int i = 0; i < numStop; i++) {
    for (int j = 0; j < (*(numberOfPixels[stopIDs[i]])); j++) {
      if (numTracks >= maxTracks - 1)
        break;
      c = pixels[stopIDs[i]][j]->getShape()->getCenter();
      dir = vector3D(c);
      if (useVertexAsStart) {
        if (dir.Theta() < restrictionT[i * 2] || dir.Theta() > restrictionT[i * 2 + 1] ||
            dir.Phi() < restrictionP[i * 2] || dir.Phi() > restrictionP[i * 2 + 1])
          continue;
        tmpTracks[numTracks]->reset();
        tmpTracks[numTracks]->addPixel(pixels[stopIDs[i]][j]);
        if (searchATrack(numTracks, i, pixels[stopIDs[i]][j], NULL))
          numTracks++;
      } else // useVertexAsStart
      {
        for (int k = 0; k < numStart; k++) {
          for (int l = 0; l < (*(numberOfPixels[startIDs[k]])); l++) {
            tmpTracks[numTracks]->reset();
            tmpTracks[numTracks]->addPixel(pixels[stopIDs[i]][j]);
            tmpTracks[numTracks]->addPixel(pixels[startIDs[k]][l]);
            if (searchATrack(numTracks, i, pixels[stopIDs[i]][j], pixels[startIDs[k]][l]))
              numTracks++;
          }
        }
      }
    }
  }
  for (int i = 0; i < numStopCluster; i++) {
    for (int j = 0; j < (*(numberOfClusters[stopClusterIDs[i]])); j++) {
      if (numTracks >= maxTracks - 1)
        break;
      c = clusters[stopClusterIDs[i]][j]->getCenter();
      dir = vector3D(c);
      if (useVertexAsStart) {
        if (dir.Theta() < restrictionT[numStop * 2 + i * 2] ||
            dir.Theta() > restrictionT[numStop * 2 + i * 2 + 1] ||
            dir.Phi() < restrictionP[numStop * 2 + i * 2] ||
            dir.Phi() > restrictionP[numStop * 2 + i * 2 + 1])
          continue;
        tmpTracks[numTracks]->reset();
        for (int k = 0; k < clusters[stopClusterIDs[i]][j]->getNumberOfPixels(); k++)
          tmpTracks[numTracks]->addPixel(&clusters[stopClusterIDs[i]][j]->getPixelr(k));
        if (searchATrack(numTracks, i, clusters[stopClusterIDs[i]][j], NULL))
          numTracks++;
      } else // useVertexAsStart
      {
        for (int k = 0; k < numStart; k++) {
          for (int l = 0; l < (*(numberOfPixels[startIDs[k]])); l++) {
            tmpTracks[numTracks]->reset();
            for (int m = 0; m < clusters[stopClusterIDs[i]][j]->getNumberOfPixels(); m++)
              tmpTracks[numTracks]->addPixel(&clusters[stopClusterIDs[i]][j]->getPixelr(m));
            tmpTracks[numTracks]->addPixel(pixels[startIDs[k]][l]);
            if (searchATrack(numTracks, i, clusters[stopClusterIDs[i]][j], pixels[startIDs[k]][l]))
              numTracks++;
          }
        }
      }
    }
  }
  // cout<<"sorting"<<endl;
  int num, elnum, n;
  TTrack* ttmp;
  for (int i = 0; i < numTracks; i++) // sort by chi squared
  {
    num = i;
    for (int j = i + 1; j < numTracks; j++) // look for lowest chi
      if (tmpTracks[num]->getChiSquared() > tmpTracks[j]->getChiSquared())
        num = j;
    if (num == i)
      continue;
    ttmp = tmpTracks[num];
    for (int j = num - 1; j >= i; j--)
      tmpTracks[j + 1] = tmpTracks[j];
    tmpTracks[i] = ttmp;
  }
  for (int i = 0; i < numTracks; i++) // sort by number Of elements
  {
    num = i;
    for (int j = i + 1; j < numTracks; j++) // look for highest number of elements
      if (tmpTracks[num]->getNumberOfCalibHits(-1) < tmpTracks[j]->getNumberOfCalibHits(-1))
        num = j;
    if (num == i)
      continue;
    ttmp = tmpTracks[num];
    for (int j = num - 1; j >= i; j--)
      tmpTracks[j + 1] = tmpTracks[j];
    tmpTracks[i] = ttmp;
  }
  for (int i = 0; i < numTracks; i++) // eliminate tracks with too many els in common
  {
    for (int j = i + 1; j < numTracks; j++) {
      elnum = 0;
      for (int k = 0; k < tmpTracks[i]->getNumberOfCalibHits(-2); k++) {
        if (tmpTracks[i]->getNumberOfCalibHits(k) > 0 &&
            tmpTracks[j]->getNumberOfCalibHits(k) > 0 && k != 15 && k != 16 && k != 21) {
          n = 0;
          for (int l = 0; l < tmpTracks[i]->getNumberOfCalibHits(k); l++)
            for (int m = 0; m < tmpTracks[j]->getNumberOfCalibHits(k); m++)
              if (tmpTracks[i]->getCalibSrcr(k, l) == tmpTracks[j]->getCalibSrcr(k, m)) {
                n++;
              }
          elnum += n;
        }
      }
      if (elnum > maxEqualElements) {
        ttmp = tmpTracks[j];
        for (int k = j; k < numTracks - 1; k++)
          tmpTracks[k] = tmpTracks[k + 1];
        tmpTracks[numTracks - 1] = ttmp;
        numTracks--;
        j--;
      }
    }
  }
  for (int i = 0; i < numTracks; i++) // sort by theta
  {
    num = i;
    for (int j = i + 1; j < numTracks; j++)
      if (tmpTracks[num]->getPath().Direction().Theta() >
          tmpTracks[j]->getPath().Direction().Theta())
        num = j;
    if (num == i)
      continue;
    ttmp = tmpTracks[num];
    for (int j = num - 1; j >= i; j--)
      tmpTracks[j + 1] = tmpTracks[j];
    tmpTracks[i] = ttmp;
  }
#ifdef DEBUGINFO
  cout << "prompt tracking:" << numTracks << endl;
#endif
  for (int i = 0; i < numTracks; i++) // copy tracks
  {
    (*tracks[i]) = (*tmpTracks[i]);
    tracks[i]->setOrder(false);
    tracks[i]->setCharge(1);
    setTrackErrors(tracks[i]);
#ifdef DEBUGINFO
    cout << tracks[i]->getPath().toString(_spheric_ + 4 * _spheric_).ascii() << endl;
#endif
  }
  numberOfTracks = numTracks;
#ifdef MICROPIXELCHECK
  for (int i = 0; i < numTracks; i++) {
    for (int j = 0; j < (*numberOfPixels[pixid]); j++) {
      phi_tracking[0] = tracks[i]->getPath().Direction().Phi();
      theta_tracking[0] = tracks[i]->getPath().Direction().Theta();
      phi_tracking[1] = pixels[pixid][j]->getShape()->getCenter().Phi();
      theta_tracking[1] = pixels[pixid][j]->getShape()->getCenter().Theta();
      el_tracking[0] = pixels[pixid][j]->getElementr(0).getElement();
      el_tracking[1] = pixels[pixid][j]->getElementr(1).getElement();
      outTreeTracking->Fill();
    }
  }
#endif
}
algorithm_parameter ALineTrackSearch::getDescription()
{
  algorithm_parameter ret("Line Track search", algorithm_parameter::Category::TRACKING, 0);
  string des = "This is a track search algorithm in a detector "
               "without magnetic field, with thin detectors. "
               "The tracks can be searched in two modes: \n"
               "-with the target as vertex and a pixel center "
               " as source for prompt tracks \n"
               "-taking two pixel center as bias for possibly "
               " secondary tracks.\n"
               "As for the assignment of the hits in the detector "
               "to the track, two modes are possible: \n"
               "-If the enveloping shape of the detector-elements "
               " in the detector is regular, hits around a suspect "
               " element (taken from volumeShape::suspect()) are "
               " taken.\n"
               "-Every hit element of the detector is checked. "
               " If the assumed line is close enough this element "
               " is used. This method is slower than the other one!";
  ret.setDescription(des);
  ret.addValue("use vertex as start", true);
  ret.addValue("use angular distance", true);
  ret.addValue("max # equal elements in two tracks", static_cast<int>(2));
  vector<int> tmp;
  vector<float> tmp2;
  ret.addValue("stop-pixel IDs", tmp);
  ret.addValue("min # elements on track", tmp);
  ret.addValue("essential detector IDs for Pixel", tmp);
  ret.addValue("start-pixel IDs", tmp);
  ret.addValue("detectors", tmp);
  ret.addValue("element search mode", tmp);
  ret.addValue("max element distance", tmp);
  ret.addValue("check pixel IDs", tmp);
  ret.addValue("stop-cluster IDs", tmp);
  ret.addValue("min # elements on track cluster", tmp);
  ret.addValue("essential detector IDs for Cluster", tmp);
  ret.addValue("max chi squared for pixel track", tmp2);
  ret.addValue("theta restriction for pixel/cluster", tmp2);
  ret.addValue("phi restriction for pixel/cluster", tmp2);
  ret.addValue("max distance", tmp2);
  ret.addValue("max chi squared for cluster track", tmp2);
  return ret;
}
ALineTrackSearch::ALineTrackSearch(TSetup& setupIn, TTrack** tracksIn, TCluster*** clustersIn,
                                   TPixel*** pixelsIn, TCalibHit*** hitsIn, int maxTracksIn,
                                   int& numberOfTracksIn, int** numberOfClustersIn,
                                   int** numberOfPixelsIn, int** numberOfHitsIn,
                                   const algorithm_parameter& descr)
    : AAlgorithm("Line Track search"), setup(setupIn), maxTracks(maxTracksIn),
      numberOfTracks(numberOfTracksIn), myLineFit(100)
{
  hits = hitsIn;
  pixels = pixelsIn;
  clusters = clustersIn;
  tracks = tracksIn;
  numberOfHits = numberOfHitsIn;
  numberOfPixels = numberOfPixelsIn;
  numberOfClusters = numberOfClustersIn;
  useVertexAsStart = descr.value(0).value<bool>();
  angularDistance = descr.value(1).value<bool>();
  numStop = descr.value(3).value<vector<int>>().size();
  numStopCluster = descr.value(11).value<vector<int>>().size();
  stopIDs = new int[numStop];
  stopClusterIDs = new int[numStopCluster];
  numEssentials = new int[numStop];
  numClusterEssentials = new int[numStopCluster];
  essentials = new int*[numStop];
  clusterEssentials = new int*[numStopCluster];
  minElements = new int[numStop];
  minClusterElements = new int[numStopCluster];
  maxChi = new float[numStop];
  maxClusterChi = new float[numStopCluster];
  restrictionT = new float[(numStop + numStopCluster) * 2];
  restrictionP = new float[(numStop + numStopCluster) * 2];
  maxDistance = new float[numStop + numStopCluster];
  int j = 0;
  maxEqualElements = descr.value(2).value<int>();
  for (int i = 0; i < numStop; i++) {
    stopIDs[i] = descr.value(3).value<vector<int>>().at(i);
    minElements[i] = descr.value(4).value<vector<int>>().at(i);
    maxChi[i] = descr.value(14).value<vector<float>>().at(i);
    restrictionT[i * 2] = descr.value(15).value<vector<float>>().at(i * 2);
    restrictionT[i * 2 + 1] = descr.value(15).value<vector<float>>().at(i * 2 + 1);
    restrictionP[i * 2] = descr.value(16).value<vector<float>>().at(i * 2);
    restrictionP[i * 2 + 1] = descr.value(16).value<vector<float>>().at(i * 2 + 1);
    maxDistance[i] = descr.value(17).value<vector<float>>().at(i);
    numEssentials[i] = descr.value(5).value<vector<int>>().at(j);
    j++;
    essentials[i] = new int[numEssentials[i]];
    for (int k = 0; k < numEssentials[i]; k++) {
      essentials[i][k] = descr.value(5).value<vector<int>>().at(j);
      j++;
    }
  }
  j = 0;
  for (int i = 0; i < numStopCluster; i++) {
    stopClusterIDs[i] = descr.value(11).value<vector<int>>().at(i);
    minClusterElements[i] = descr.value(12).value<vector<int>>().at(i);
    maxClusterChi[i] = descr.value(18).value<vector<float>>().at(i);
    restrictionT[numStop * 2 + i * 2] =
        descr.value(15).value<vector<float>>().at(numStop * 2 + i * 2);
    restrictionT[numStop * 2 + i * 2 + 1] =
        descr.value(15).value<vector<float>>().at(numStop * 2 + i * 2 + 1);
    restrictionP[numStop * 2 + i * 2] =
        descr.value(16).value<vector<float>>().at(numStop * 2 + i * 2);
    restrictionP[numStop * 2 + i * 2 + 1] =
        descr.value(16).value<vector<float>>().at(numStop * 2 + i * 2 + 1);
    maxDistance[numStop + i] = descr.value(17).value<vector<float>>().at(numStop + i);
    numClusterEssentials[i] = descr.value(13).value<vector<int>>().at(j);
    j++;
    clusterEssentials[i] = new int[numClusterEssentials[i]];
    for (int k = 0; k < numClusterEssentials[i]; k++) {
      clusterEssentials[i][k] = descr.value(13).value<vector<int>>().at(j);
      j++;
    }
  }
  numMid = descr.value(10).value<vector<int>>().size();
  midIDs = new int[numMid];
  for (int i = 0; i < numMid; i++) {
    midIDs[i] = descr.value(10).value<vector<int>>().at(i);
  }
  numStart = descr.value(6).value<vector<int>>().size();
  startIDs = new int[numStart];
  for (int i = 0; i < numStart; i++)
    startIDs[i] = descr.value(6).value<vector<int>>().at(i);
  numDetectors = descr.value(7).value<vector<int>>().size();
  detectorIDs = new int[numDetectors];
  conventional = new int[numDetectors];
  for (int i = 0; i < numDetectors; i++) {
    detectorIDs[i] = descr.value(7).value<vector<int>>().at(i);
    conventional[i] = descr.value(8).value<vector<int>>().at(i);
  }

  away = new int[numDetectors * (numStop + numStopCluster)];
  for (int i = 0; i < numDetectors * (numStop + numStopCluster); i++)
    away[i] = descr.value(9).value<vector<int>>().at(i);
  tmpTracks = new TTrack*[maxTracks];
  for (int i = 0; i < maxTracks; i++)
    tmpTracks[i] = new TTrack(20, tracks[0]->getNumberOfCalibHits(-2));
#ifdef SHOW_PARAMETERS
  cout << "Search in detectors:" << endl;
  for (int i = 0; i < numDetectors; i++)
    cout << detectorIDs[i] << ", " << flush;
  cout << endl;
  for (int i = 0; i < numDetectors; i++)
    cout << conventional[i] << ", " << flush;
  cout << endl;
  cout << "Check pixels in line" << endl;
  for (int i = 0; i < numMid; i++)
    cout << midIDs[i] << "\t";
  cout << endl;
  cout << "pixels to start" << endl;
  for (int i = 0; i < numStart; i++)
    cout << startIDs[i] << "\t";
  cout << endl;
  cout << "use vertex as start (prompt search):" << (useVertexAsStart ? "true" : "false") << endl;
  cout << "use angular distance:" << (angularDistance ? "true" : "false") << endl;
  cout << "Search for Pixels:" << endl << "\t";
  for (int i = 0; i < numStop; i++)
    cout << stopIDs[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStop; i++)
    cout << minElements[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStop; i++)
    cout << maxChi[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStop; i++)
    cout << maxDistance[i] << "\t" << flush;
  cout << endl;
  for (int i = 0; i < numStop; i++) {
    cout << "pixel: " << stopIDs[i] << endl;
    cout << "\t" << restrictionT[2 * i] << "< theta <" << restrictionT[2 * i + 1] << endl;
    cout << "\t" << restrictionP[2 * i] << "< phi   <" << restrictionP[2 * i + 1] << endl;
    cout << "\t"
         << "essential detectors:" << endl
         << "\t";
    for (int j = 0; j < numEssentials[i]; j++)
      cout << essentials[i][j] << ", " << flush;
    cout << endl;
    cout << "\t"
         << "distance to suspect:" << endl
         << "\t";
    for (int j = 0; j < numDetectors; j++)
      cout << away[(i)*numDetectors + j] << ", " << flush;
    cout << endl;
  }
  cout << "Search for Clusters:" << endl << "\t";
  for (int i = 0; i < numStopCluster; i++)
    cout << stopClusterIDs[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStopCluster; i++)
    cout << minClusterElements[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStopCluster; i++)
    cout << maxClusterChi[i] << "\t" << flush;
  cout << endl << "\t";
  for (int i = 0; i < numStopCluster; i++)
    cout << maxDistance[numStop + i] << "\t" << flush;
  cout << endl;
  for (int i = 0; i < numStopCluster; i++) {
    cout << "cluster: " << stopClusterIDs[i] << endl;
    cout << "\t" << restrictionT[2 * numStop + 2 * i] << "< theta <"
         << restrictionT[2 * numStop + 2 * i + 1] << endl;
    cout << "\t" << restrictionP[2 * numStop + 2 * i] << "< phi   <"
         << restrictionP[2 * numStop + 2 * i + 1] << endl;
    cout << "\t"
         << "essential detectors:" << endl
         << "\t";
    for (int j = 0; j < numClusterEssentials[i]; j++)
      cout << clusterEssentials[i][j] << ", " << flush;
    cout << endl;
    cout << "\t"
         << "distance to suspect:" << endl
         << "\t";
    for (int j = 0; j < numDetectors; j++)
      cout << away[(i + numStop) * numDetectors + j] << ", " << flush;
    cout << endl;
  }
#endif
#ifdef MICROPIXELCHECK
  TFile* f = new TFile("micropixelcheck.root", "recreate");
  f->cd();
  outTreeTracking = new TTree("micropixelcheck", "micropixelcheck");
  outTreeTracking->Branch("Theta_track", &theta_tracking[0], "Theta_track/F");
  outTreeTracking->Branch("Theta_pixel", &theta_tracking[1], "Theta_pixel/F");
  outTreeTracking->Branch("Phi_track", &phi_tracking[0], "Phi_track/F");
  outTreeTracking->Branch("Phi_pixel", &phi_tracking[1], "Phi_pixel/F");
  outTreeTracking->Branch("el1", &el_tracking[0], "el1/I");
  outTreeTracking->Branch("el2", &el_tracking[1], "el2/I");
#endif
}
ALineTrackSearch::~ALineTrackSearch()
{
#ifdef MICROPIXELCHECK
  TFile* f = outTreeTracking->GetCurrentFile();
  f->cd();
  outTreeTracking->Write();
  f->Close();
  delete f;
  cout << "pixel check written" << endl;
#endif
  for (int i = 0; i < numStop; i++)
    delete[] essentials[i];
  delete[] essentials;
  delete[] numEssentials;
  delete[] stopIDs;
  delete[] minElements;
  for (int i = 0; i < numStopCluster; i++)
    delete[] clusterEssentials[i];
  delete[] clusterEssentials;
  delete[] numClusterEssentials;
  delete[] stopClusterIDs;
  delete[] minClusterElements;
  delete[] startIDs;
  delete[] conventional;
  delete[] detectorIDs;
  delete[] maxChi;
  delete[] maxClusterChi;
  delete[] restrictionT;
  delete[] restrictionP;
  delete[] maxDistance;
  delete[] away;
  for (int i = 0; i < maxTracks; i++)
    delete tmpTracks[i];
  delete[] tmpTracks;
  delete[] midIDs;
}
