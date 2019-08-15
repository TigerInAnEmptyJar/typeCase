#include "APixelTracking.h"
#include "fittingRoutines.h"
#include "hitCluster.h"
//#define DEBUGGING
//#define DEBUG_FINAL
//#define DEBUG_FINAL_OUT
//#define DEBUG_FIT
//#define DEBUG_SEARCH
//#define DEBUG_VEE
sLine3D ausgleichsgerade(int nP, point3D* p)
{
  //   point3D minz=p[0],maxz=p[0];
  //   for(int i=1;i<nP;i++)
  //     {
  //       if(p[i].Z()<minz.Z())minz=p[i];
  //       if(p[i].Z()>maxz.Z())maxz=p[i];
  //     }
  //   vector3D vv=maxz-minz;vv.normalize();
  //   point3D pp(minz-vv*(minz.Z()/vv.Z()));
  //   return sLine3D(pp,vv);
  float sumz = 0, sumz2 = 0, sumx = 0, sumy = 0, sumzy = 0, sumzx = 0;
  for (int i = 0; i < nP; i++) {
    sumz = sumz + p[i].Z();
    sumx = sumx + p[i].X();
    sumy = sumy + p[i].Y();
    sumz2 = sumz2 + p[i].Z() * p[i].Z();
    sumzx = sumzx + p[i].X() * p[i].Z();
    sumzy = sumzy + p[i].Y() * p[i].Z();
  }
  float sum = nP * sumz2 - sumz * sumz;
  float xx0 = (sumx * sumz2 - sumz * sumzx) / sum;
  float yy0 = (sumy * sumz2 - sumz * sumzy) / sum;
  float xx1 = (nP * sumzx - sumz * sumx) / sum;
  float yy1 = (nP * sumzy - sumz * sumy) / sum;
  return sLine3D(point3D(xx0, yy0, 0.), vector3D(xx1, yy1, 1.));
}
void APixelTracking::eliminateTooCloseToPrompt(int& n, TTrack** array, int max)
{
  if (n == 0 || numberOfTracks == 0)
    return;
  int elnum = 0;
  TTrack* ttmp;
  for (int j = 0; j < numberOfTracks; j++) {
    for (int i = 0; i < n; i++) {
      elnum = nElementsInCommon(array[i], tracks[j]);
      if (elnum > max || (acos(array[i]->getPath().Direction() * tracks[j]->getPath().Direction()) <
                              2 * M_PI / 180. &&
                          (array[i]->getPath().Foot() - tracks[j]->getPath().Foot()).R() < 10)) {
        ttmp = array[i];
        for (int k = i; k < n - 1; k++)
          array[k] = array[k + 1];
        array[n - 1] = ttmp;
        n--;
        i--;
      }
    }
  }
#ifdef DEBUGGING
  cout << "APixelTracking::eliminateDoubleTracks(n=" << n << ",max=" << max << ") done" << endl;
#endif
}
int APixelTracking::nElementsInCommon(TTrack* t1, TTrack* t2)
{
  int n = 0;
  for (int k = 0; k < setup.getNumberOfDetectors(); k++) {
    if (t1->getNumberOfCalibHits(k) > 0 && t2->getNumberOfCalibHits(k) > 0 && k != 15 && k != 16 &&
        k != 21) {
      for (int l = 0; l < t1->getNumberOfCalibHits(k); l++)
        for (int m = 0; m < t2->getNumberOfCalibHits(k); m++)
          if (t1->getCalibSrcr(k, l) == t2->getCalibSrcr(k, m))
            n++;
    }
  }
  return n;
}
void APixelTracking::eliminateDoubleTracks(int& n, TTrack** array, int max)
{
  if (n < 2)
    return;
  // #ifdef DEBUGGING
  //   cout<<"APixelTracking::eliminateDoubleTracks(n="<<n<<",max="<<max<<")"<<endl;
  // #endif
  int elnum = 0;
  TTrack* ttmp;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      elnum = nElementsInCommon(array[i], array[j]);
      if (elnum > max || (acos(array[i]->getPath().Direction() * array[j]->getPath().Direction()) <
                              2 * M_PI / 180. &&
                          (array[i]->getPath().Foot() - array[j]->getPath().Foot()).R() < 10)) {
        ttmp = array[j];
        for (int k = j; k < n - 1; k++)
          array[k] = array[k + 1];
        array[n - 1] = ttmp;
        n--;
        j--;
      }
    }
  }
#ifdef DEBUGGING
  cout << "APixelTracking::eliminateDoubleTracks(n=" << n << ",max=" << max << ") done" << endl;
#endif
}
void APixelTracking::eliminateDoubleTracks(int& n1, TTrack** array1, int& n2, TTrack** array2,
                                           int max)
{
  if (n2 + n1 < 2)
    return;
  // #ifdef DEBUGGING
  //   cout<<"APixelTracking::eliminateDoubleTracks(n="<<n<<",max="<<max<<")"<<endl;
  // #endif
  int elnum = 0;
  TTrack* ttmp;
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
      elnum = nElementsInCommon(array1[i], array2[j]);
      if (elnum > max || acos(array1[i]->getPath().Direction() * array2[j]->getPath().Direction()) <
                             2 * M_PI / 180.) {
        if (array1[i]->getNumberOfCalibHits() /*ChiSquared()<*/ >
            array2[j]->getNumberOfCalibHits() /*ChiSquared()*/) {
          ttmp = array2[j];
          for (int k = j; k < n2 - 1; k++)
            array2[k] = array2[k + 1];
          array2[n2 - 1] = ttmp;
          if (n2 > 0)
            n2--;
          j--;
        } else {
          ttmp = array1[i];
          for (int k = i; k < n1 - 1; k++)
            array1[k] = array1[k + 1];
          array1[n1 - 1] = ttmp;
          n1--;
          if (i > 0)
            i--;
        }
      }
    }
  }
#ifdef DEBUGGING
  cout << "APixelTracking::eliminateDoubleTracks(n=" << n << ",max=" << max << ") done" << endl;
#endif
}
void sortTracks(int& n, TTrack** array, float* values)
{
  if (n < 2)
    return;
  int num;
  TTrack* ttmp;
  float tmpf;
  for (int i = 0; i < n; i++) {
    num = i;
    for (int j = i + 1; j < n; j++)
      if (values[num] > values[j])
        num = j;
    if (num == i)
      continue;
    ttmp = array[num];
    tmpf = values[num];
    for (int j = num - 1; j >= i; j--) {
      array[j + 1] = array[j];
      values[j + 1] = values[j];
    }
    array[i] = ttmp;
    values[i] = tmpf;
  }
}
void APixelTracking::sortAndCopyPrompt(int& n, TTrack** array)
{
#ifdef DEBUG_FINAL
  cout << "APixelTracking::sortAndCopyPrompt(n=" << n << ")" << endl;
#endif
  if (n <= 0)
    return;
  // sort by chi-squared
  float values[n];
  for (int i = 0; i < n; i++)
    values[i] = array[i]->getChiSquared();
  sortTracks(n, array, values);
  eliminateDoubleTracks(n, array, maxPromptEqual);
  // sort by theta
  for (int i = 0; i < n; i++)
    values[i] = array[i]->getPath().Direction().Theta();
  sortTracks(n, array, values);
  if (n <= 0)
    return;
// copy
#ifdef DEBUG_FINAL_OUT
  cout << "tracks " << n << endl;
  for (int i = 0; i < n; i++) {
    cout << "\t" << array[i]->getPath().toString(_spheric_).data() << array[i]->getChiSquared()
         << " " << array[i]->getNumberOfCalibHits() << endl
         << "\t\t(";
    for (int j = 0; j < setup.getNumberOfDetectors(); j++)
      if (array[i]->getNumberOfCalibHits(j) > 0)
        cout << j << ", ";
    cout << ")" << endl;
  }
#endif
  for (int i = 0; i < n; i++) {
    (*tracks[i]) = (*array[i]);
    tracks[i]->setOrder(false);
    tracks[i]->setCharge(1);
    setTrackErrors(tracks[i]);
  }
  numberOfTracks = n;
#ifdef DEBUG_FINAL
  cout << "APixelTracking::sortAndCopyPrompt(n=" << n << ") done" << endl;
#endif
}
void APixelTracking::sortAndCopyVee(int& n, TTrack** array)
{
#ifdef DEBUG_FINAL
  cout << "APixelTracking::sortAndCopyVee(n=" << n << ")" << endl;
#endif
  if (n == 0)
    return;
  point3D tc = target->getCenter();
  vector3D neutralDirection;
  vector3D dir1, dir2;
  int add1, add2;
  if (numberOfTracks + n * 3 > maxTracks)
    n = (maxTracks - numberOfTracks) / 3;
#ifdef DEBUG_FINAL_OUT
  for (int i = 0; i < n; i++) {
    cout << "\t" << array[2 * i]->getPath().Foot().toString(_spheric_).data() << endl;
    cout << "\t  " << array[2 * i]->getPath().Direction().toString(_spheric_).data() << endl;
    cout << "\t  " << array[2 * i + 1]->getPath().Direction().toString(_spheric_).data() << endl;
  }
#endif
  for (int i = 0; i < n; i++) {
    dir1 = array[i * 2]->getPath().Direction();
    dir1.normalize();
    dir2 = array[i * 2 + 1]->getPath().Direction();
    dir2.normalize();
    neutralDirection = array[i * 2]->getPath().Foot() - tc;
    neutralDirection.normalize();
    add1 = 0;
    add2 = 1;
    if (acos(dir1 * neutralDirection) < acos(dir2 * neutralDirection)) {
      add1 = 1;
      add2 = 0;
    }
    (*tracks[numberOfTracks + n + 2 * i]) = (*array[i * 2 + add1]);
    (*tracks[numberOfTracks + n + 2 * i + 1]) = (*array[i * 2 + add2]);
    tracks[numberOfTracks + n + 2 * i]->setOrder(true);
    tracks[numberOfTracks + n + 2 * i]->setCharge(1);
    tracks[numberOfTracks + n + 2 * i + 1]->setOrder(true);
    tracks[numberOfTracks + n + 2 * i + 1]->setCharge(1);
    setTrackErrors(tracks[numberOfTracks + n + 2 * i]);
    setTrackErrors(tracks[numberOfTracks + n + 2 * i + 1]);

    tracks[numberOfTracks + i]->reset();
    tracks[numberOfTracks + i]->setOrder(false);
    tracks[numberOfTracks + i]->setCharge(0);
    tracks[numberOfTracks + i]->setPath(sLine3D(tc, neutralDirection));
    tracks[numberOfTracks + i]->addSecondary(tracks[numberOfTracks + n + 2 * i]);
    tracks[numberOfTracks + i]->addSecondary(tracks[numberOfTracks + n + 2 * i + 1]);
    tracks[numberOfTracks + n + 2 * i]->setPrimary(tracks[numberOfTracks + i]);
    tracks[numberOfTracks + n + 2 * i + 1]->setPrimary(tracks[numberOfTracks + i]);
  }
  numberOfTracks += n * 3;
#ifdef DEBUG_FINAL
  cout << "APixelTracking::sortAndCopyVee(n=" << n << ") done" << endl;
#endif
}
#include <stdlib.h>
TCalibHit* APixelTracking::searchElement(const sLine3D& line, int ID, int max_value)
{
  //  cout<<"APixelTracking::searchElement(line="<<line.toString(_spheric_).data()<<",
  //  ID="<<ID<<")"<<endl;
  if (ID < 0 || ID >= nndetpos)
    return NULL;
  if (checkdets[ID] >= setup.getNumberOfDetectors() || checkdets[ID] < 0)
    return NULL;
  int nEl = setup.getDetectorr(checkdets[ID]).getNumberOfElements();
  if (nEl == 0)
    return NULL;
  //  if((*(numberOfHits[ID]))==0)return NULL;
  if (ndetpos[ID] == 0)
    return NULL;
  int suspect = setup.getDetectorr(checkdets[ID])
                    .getShape(0)
                    ->suspect(line, setup.getDetectorr(checkdets[ID]).getStackType());
  if (suspect < 0 || suspect >= nEl)
    return NULL;
  int best = 0;
  int diff, bestDiff = nEl;
  //  cout<<"suspect "<<suspect<<endl;
  for (int i = 0; i < ndetpos[ID]; i++) {
    if (!hits[checkdets[ID]][detpos[ID][i]]->isValid())
      continue;
    if (hits[checkdets[ID]][detpos[ID][i]]->getHitShape() == NULL)
      continue;
    diff = abs(hits[checkdets[ID]][detpos[ID][i]]->getElement() - suspect);
    if (diff == 0) {
      best = i;
      bestDiff = 0;
      break;
    }
    if (setup.getDetectorr(checkdets[ID]).isCircular()) {
      if (abs(diff - nEl) < diff)
        diff = abs(diff - nEl);
    }
    if (diff < bestDiff) {
      best = i;
      bestDiff = diff;
    }
  }
#ifdef DEBUG_SEARCH
  cout << "APixelTracking::searchElement(line=" << line.toString(_spheric_).data()
       << ", ID=" << checkdets[ID] << ") done \n\t(suspect=" << suspect << ", diff=" << bestDiff
       << ", max=" << max_value << " ->";
  if (bestDiff <= max_value)
    cout << hits[checkdets[ID]][detpos[ID][best]]->getElement() << ")" << endl;
  else
    cout << ")" << endl;
#endif
  if (bestDiff <= max_value)
    return hits[checkdets[ID]][detpos[ID][best]];
  return NULL;
}
float APixelTracking::trackFit(TTrack* track, int min_points, float max_dist)
{
#ifdef DEBUG_FIT
  cout << "APixelTracking::trackFit() " << track->getNumberOfCalibHits() << " " << min_points
       << endl;
#endif
  if (track->getNumberOfCalibHits() < min_points) {
#ifdef DEBUG_FIT
    cout << "(";
    for (int i = 0; i < setup.getNumberOfDetectors(); i++)
      if (track->getNumberOfCalibHits(i) > 0)
        cout << i << ", ";
    cout << ")" << endl;
#endif
    return -1;
  }
  lineFit myFit(track->getNumberOfCalibHits() + 2);
  myFit.clear();
  sLine3D line = track->getPath();
  float sigma;
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    for (int j = 0; j < track->getNumberOfCalibHits(i); j++) {
      Vector hitParam = track->getCalibSrcr(i, j)->getHitShape()->HitParams(line);
      vector3D dist(hitParam[10], hitParam[11], hitParam[12]);
#ifdef DEBUG_FIT
      cout << "(" << i << ": " << track->getCalibSrcr(i, j)->getElement() << ")"
           << dist.toString(_spheric_).data() << max_dist << endl;
#endif
      // 	  if(i<=2)
      // 	    cout<<"("<<i<<":
      // "<<track->getCalibSrcr(i,j)->getElement()<<")"<<dist.toString(_spheric_).data()<<endl;
      sigma = 1; //(i<6?10:1);
      if (!(dist.R() <= max_dist * sigma)) {
        track->removeCalibSrc(track->getCalibSrcr(i, j));
        j--;
        continue;
      }
      vector3D p(hitParam[7], hitParam[8], hitParam[9]);
      vector3D res(hitParam[4], hitParam[5], hitParam[6]);
      sigma = hitParam[0];
      myFit.addPoint(p, res, sigma);
    }
  }
#ifdef DEBUG_FIT
  cout << myFit.Points() << endl;
#endif
  if (myFit.Points() < min_points)
    return -1;
  myFit.fit();
#ifdef DEBUG_FIT
  cout << myFit.Chi() << myFit.Foot() << myFit.Direction().toString(_spheric_).data() << endl;
#endif
  track->setPath(sLine3D(myFit.Foot(), myFit.Direction()));
  track->setChiSquared(myFit.Chi());
#ifdef DEBUG_FIT
  cout << "APixelTracking::trackFit() done " << myFit.Chi() << endl;
#endif
  return myFit.Chi();
}

float APixelTracking::veeFit(TTrack* track1, TTrack* track2)
{
#ifdef DEBUG_FIT
  cout << "APixelTracking::veeFit()" << endl;
#endif
  if (track1->getNumberOfCalibHits() + track2->getNumberOfCalibHits() < minPointsVee)
    return -1;
  vertexFit myFit(3, track1->getNumberOfCalibHits() + track2->getNumberOfCalibHits() + 2);
  myFit.clear();
  float sigma;
  sLine3D line[2] = {track1->getPath(), track2->getPath()};
  TTrack* track[2] = {track1, track2};
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    for (int k = 0; k < 2; k++) {
      for (int j = 0; j < track[k]->getNumberOfCalibHits(i); j++) {
        Vector hitParam = track[k]->getCalibSrcr(i, j)->getHitShape()->HitParams(line[k]);
        vector3D dist(hitParam[10], hitParam[11], hitParam[12]);
        // 	      if(!(dist.R()<=maxDistance))
        // 		{
        // 		  track[k]->removeCalibSrc(track[k]->getCalibSrcr(i,j));
        // 		  j--;
        // 		  continue;
        // 		}
        vector3D p(hitParam[7], hitParam[8], hitParam[9]);
        vector3D res(hitParam[4], hitParam[5], hitParam[6]);
        sigma = hitParam[0];
        myFit.addPoint(p, res, sigma, k);
      }
    }
  }
  if (myFit.Points(0) + myFit.Points(1) < minPointsVee)
    return -1;
  myFit.fit();
  track[0]->setPath(sLine3D(myFit.Vertex(), myFit.Line(0)));
  track[1]->setPath(sLine3D(myFit.Vertex(), myFit.Line(1)));
  track[0]->setChiSquared(myFit.Chi());
  track[1]->setChiSquared(myFit.Chi());
#ifdef DEBUG_FIT
  cout << "APixelTracking::veeFit() done " << myFit.Chi() << endl;
#endif
  return myFit.Chi();
}
bool APixelTracking::searchPromptTrack(TTrack* track, int ID)
{
#ifdef DEBUG_SEARCH
  cout << "APixelTracking::searchPromptTrack()" << endl;
#endif
  int nStart = 2;
  int start[2] = {15, 16};
  int nTracking = 7;
  int tracking[7] = {21, 22, 17, 18, 23, 19, 20};
  sLine3D line(track->getPath());
#ifdef DEBUG_SEARCH
  cout << "path=" << line << endl;
#endif
  int nst = 0;
  for (int i = 0; i < nStart; i++) {
    TCalibHit* tmpHit = searchElement(line, start[i], maxDiff[ID]);
    if (tmpHit == NULL) // both start needed
      return false;
    track->addCalibSrc(tmpHit);
    nst++;
  }
  for (int i = 0; i < nTracking; i++) {
    if (track->getNumberOfCalibHits(tracking[i]) > 0)
      continue;
    TCalibHit* tmpHit = searchElement(line, tracking[i], maxDiff[ID]);
    if (tmpHit != NULL)
      track->addCalibSrc(tmpHit);
  }
#ifdef DEBUG_SEARCH
  cout << "APixelTracking::searchPromptTrack() done " << track->getNumberOfCalibHits() << "(";
  for (int i = 0; i < setup.getNumberOfDetectors(); i++)
    if (track->getNumberOfCalibHits(i) > 0)
      cout << i << ", ";
  cout << ")" << endl;
#endif
  return true;
}
bool APixelTracking::searchDecayTrack(TTrack* track, int ID)
{
#ifdef DEBUG_SEARCH
  cout << "APixelTracking::searchDecayTrack()" << endl;
#endif
  int nStart = 2;
  int start[2] = {21, 22};
  int nTracking = 5;
  int tracking[5] = {17, 18, 23, 19, 20};
  sLine3D line = track->getPath();
  int nst = 0;
  for (int i = 0; i < nStart; i++) {
    // to be decayed after microstrip
    TCalibHit* tmpHit = searchElement(line, start[i], maxDiff[ID]);
    if (tmpHit != NULL)
      nst++;
  }
  if (nst > maxDecayStart)
    return false;
  for (int i = 0; i < nTracking; i++) {
    if (track->getNumberOfCalibHits(tracking[i]) > 0)
      continue;
    TCalibHit* tmpHit = searchElement(line, tracking[i], maxDiff[ID]);
    if (tmpHit != NULL)
      track->addCalibSrc(tmpHit);
  }
#ifdef DEBUG_SEARCH
  cout << "APixelTracking::searchDecayTrack() done" << endl;
#endif
  return true;
}
int contains(THitCluster* cl, THitCluster* hit)
{
  if (cl == NULL || hit == NULL)
    return 0;
  int n = 0;
  for (int j = 0; j < hit->getNumberOfHits(); j++)
    for (int i = 0; i < cl->getNumberOfHits(); i++)
      if (cl->getHit(i) == hit->getHit(j) ||
          (cl->getHit(i)->getElement() == hit->getHit(j)->getElement() &&
           cl->getHit(i)->getDetector() == hit->getHit(j)->getDetector()))
        n++;
  return n;
}
int contains(THitCluster* cl, TCalibHit* hit)
{
  if (cl == NULL || hit == NULL)
    return 0;
  if (hit->name() == "THitCluster")
    return contains(cl, (THitCluster*)hit);
  int n = 0;
  for (int i = 0; i < cl->getNumberOfHits(); i++)
    if (cl->getHit(i) == hit || (cl->getHit(i)->getElement() == hit->getElement() &&
                                 cl->getHit(i)->getDetector() == hit->getDetector()))
      n++;
  return n;
}
int contains(TCalibHit* hit1, TCalibHit* hit2)
{
  if (hit1 == NULL || hit2 == NULL)
    return 0;
  if (hit1->name() == "THitCluster" || hit2->name() == "THitCluster") {
    if (hit1->name() == "THitCluster" && hit2->name() == "THitCluster")
      return contains((THitCluster*)hit1, (THitCluster*)hit2);
    if (hit1->name() == "THitCluster")
      return contains((THitCluster*)hit1, hit2);
    return contains((THitCluster*)hit2, hit1);
  } else {
    if (hit1 == hit2 ||
        (hit1->getElement() == hit2->getElement() && hit1->getDetector() == hit2->getDetector()))
      return 1;
    return 0;
  }
}
int contains(TTrack* tr, THitCluster* hit)
{
  if (tr == NULL || hit == NULL)
    return 0;
  int n = 0;
  for (int j = 0; j < tr->getNumberOfCalibHits(hit->getDetector()); j++) {
    for (int k = 0; k < hit->getNumberOfHits(); k++) {
      n += contains(hit->getHit(k), tr->getCalibSrcr(hit->getDetector(), j));
    }
  }
  return n;
}
int contains(TTrack* tr, TCalibHit* hit)
{
  if (tr == NULL || hit == NULL)
    return 0;
  if (hit->name() == "THitCluster")
    return contains(tr, (THitCluster*)hit);
  int n = 0;
  for (int j = 0; j < tr->getNumberOfCalibHits(hit->getDetector()); j++) {
    n += contains(hit, tr->getCalibSrcr(hit->getDetector(), j));
  }
  return n;
}
int contains(TPixel* pi, THitCluster* hit)
{
  if (pi == NULL || hit == NULL)
    return 0;
  int n = 0;
  for (int j = 0; j < pi->getNumberOfActualElements(); j++)
    for (int i = 0; i < hit->getNumberOfHits(); i++)
      n += contains(&pi->getElementr(j), hit->getHit(i));
  return n;
}
int contains(TPixel* pi, TCalibHit* hit)
{
  if (pi == NULL || hit == NULL)
    return 0;
  if (hit->name() == "THitCluster")
    return contains(pi, (THitCluster*)hit);
  int n = 0;
  for (int j = 0; j < pi->getNumberOfActualElements(); j++)
    n += contains(&pi->getElementr(j), hit);
  return n;
}
bool APixelTracking::checkWithPrompt(TCalibHit* hit)
{
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->getNumberOfCalibHits(hit->getDetector()) <= 0)
      continue;
    if (contains(tracks[i], hit) > 0)
      return false;
  }
  return true;
}
bool APixelTracking::checkWithPrompt(TPixel* pix)
{
  TCalibHit* hit;
  int n = 0, c, m = 0;
  for (int k = 0; k < pix->getNumberOfActualElements(); k++) {
    hit = &pix->getElementr(k);
    for (int i = 0; i < numberOfTracks; i++) {
      if (tracks[i]->getNumberOfCalibHits(hit->getDetector()) <= 0)
        continue;
      c = contains(tracks[i], hit);
      if (c > 1)
        return false;
      if (c > 0)
        n++;
    }
    if (n > 0)
      m++;
  }
  if (m > 1)
    return false;
  return true;
}
void* APixelTracking::process(void* ptr)
{
#ifdef DEBUGGING
  cout << "APixelTracking::process()" << endl;
#endif
  int tmpNum = 0;
#ifdef USEPP
  int pixelID1 = 3;
  int pixelID2 = 4;
  int suspect;
  int nstopdets = 3;
  int stopdets[3] = {0, 3, 6};
  int nstopFound;
  bool enoughStop;
#endif
  int trackID;
  point3D p1, p2, pp;
  vector3D v;
  TCalibHit *tmpHit, *addHit;
  vector3D distance, bestDistance;
  //  TTrack *promptTracks[maxTracks];
  //  int nprompttracks=0;
  TTrack* decayTracks[maxTracks];
  int ndecaytracks = 0;
  TTrack* veeTracks[maxTracks];
  int nveetracks = 0;
  float chi;
  int foundPixel;
  //  int nCheckPixel=5;
  //  int checkPixel[5]={3,4,5,0,1};
  int nPoints;
  point3D points[10];
  int barrel = 6;
  float maxPixelDistance[5] = {4, 4, 0, 100, 100};
  /********************************************************
   * in order to avoid conflicts with prompt dets, create
   * pixel and element lists that contain only elements
   * that are not used in prompt tracks
   ********************************************************/
  for (int i = 0; i < nndetpos; i++) {
    ndetpos[i] = 0;
    for (int j = 0; j < (*(numberOfHits[checkdets[i]])); j++) {
      if (ndetpos[i] >= 1000)
        continue;
      if (checkWithPrompt(hits[checkdets[i]][j])) {
        detpos[i][ndetpos[i]] = j;
        ndetpos[i] = ndetpos[i] + 1;
      }
    }
  }
  for (int i = 0; i < nnpixpos; i++) {
    npixpos[i] = 0;
    for (int j = 0; j < (*(numberOfPixels[checkpix[i]])); j++) {
      if (npixpos[i] >= 1000)
        continue;
      if (checkWithPrompt(pixels[checkpix[i]][j])) {
        pixpos[i][npixpos[i]] = j;
        npixpos[i] = npixpos[i] + 1;
      }
    }
  }
  TPixel* onPixel[10];
  int nOnPixel;
  for (int i = 0; i < nnpixpos - 2; i++) {
    if (npixpos[i] == 0)
      continue;
    for (int j = i + 1; j < nnpixpos; j++) {
      if (i == j)
        continue;
      if (npixpos[j] == 0)
        continue;
      for (int ii = 0; ii < npixpos[i]; ii++) {
        if (tmpNum >= maxTracks)
          continue;
        if (pixels[checkpix[i]][pixpos[i][ii]]->getShape() == NULL)
          continue;
        p1 = pixels[checkpix[i]][pixpos[i][ii]]->getShape()->getCenter();
        for (int jj = 0; jj < npixpos[j]; jj++) {
          if (tmpNum >= maxTracks)
            continue;
          if (pixels[checkpix[j]][pixpos[j][jj]]->getShape() == NULL)
            continue;
          p2 = pixels[checkpix[j]][pixpos[j][jj]]->getShape()->getCenter();
          v = p2 - p1;
          if (v.Z() < 0)
            v = v * -1;
          pp = p1 - v * (p1.Z() / v.Z());
          sLine3D line(pp, v);
          trackID = -1;
          nOnPixel = 2;
          onPixel[0] = pixels[checkpix[i]][pixpos[i][ii]];
          onPixel[1] = pixels[checkpix[j]][pixpos[j][jj]];
          for (int k = 0; k < nnpixpos; k++) {
            if (k == i || k == j)
              continue;
            foundPixel = -1;
            for (int kk = 0; kk < npixpos[k]; kk++) {
              if (pixels[checkpix[k]][pixpos[k][kk]]->getShape() == NULL)
                continue;
              distance = pixels[checkpix[k]][pixpos[k][kk]]->getShape()->distance(line);
              if (distance.R() > 0) {
                if (pixels[checkpix[k]][pixpos[k][kk]]->getShape()->circumscribedRadius() >
                    distance.R())
                  distance.setValues(0, 0, 0);
                else
                  distance =
                      distance *
                      (distance.R() -
                       pixels[checkpix[k]][pixpos[k][kk]]->getShape()->circumscribedRadius()) /
                      distance.R();
              }
              if (foundPixel < 0 || distance.R() < bestDistance.R()) {
                bestDistance = distance;
                foundPixel = kk;
              }
            }
            if (foundPixel >= 0 && bestDistance.R() < maxPixelDistance[k]) {
              onPixel[nOnPixel] = pixels[checkpix[k]][pixpos[k][foundPixel]];
              nOnPixel++;
            }
          }
          for (int k = 0; k < nOnPixel; k++) {
            if (onPixel[k]->getID() == 0) {
              trackID = 0;
              break;
            }
            if (onPixel[k]->getID() == 1) {
              trackID = 1;
              break;
            }
          }
          if (nOnPixel == 2 && trackID == 1)
            continue;
          if (nOnPixel == 2 && trackID == 0)
            if (line.Direction().Theta() > 0.15)
              continue;
          if (nOnPixel > 2) {
            for (int k = 0; k < nOnPixel; k++)
              points[k] = onPixel[k]->getShape()->getCenter();
            sLine3D line2 = ausgleichsgerade(nOnPixel, points);
            line = line2;
          }
          // 		  if(i==2||j==2)
          // 		    {
          // 		      distance=target->distance(line);
          // 		      if(distance.R()>maxTargetDist)continue;
          // 		    }
          tmptracks[tmpNum]->reset();
          if (trackID < 0) {
            addHit = 0;
            tmpHit = searchElement(line, barrel, 1);
            if (tmpHit != NULL) {
              for (int k = 0; k < ndetpos[barrel + 1]; k++) {
                if (hits[barrel + 1][detpos[barrel + 1][k]]->isValid() &&
                    hits[barrel + 1][detpos[barrel + 1][k]]->getHitShape() != NULL &&
                    hits[barrel + 1][detpos[barrel + 1][k]]->getElement() == tmpHit->getElement())
                  addHit = hits[barrel + 1][detpos[barrel + 1][k]];
              }
              if (!(addHit == NULL || tmpHit == NULL)) {
                tmptracks[tmpNum]->addCalibSrc(tmpHit);
                tmptracks[tmpNum]->addCalibSrc(addHit);
                trackID = 2;
              }
            }
          }
          if (trackID < 0)
            continue;
          for (int k = 0; k < nOnPixel; k++) {
            tmptracks[tmpNum]->addPixel(onPixel[k]);
            for (int kk = 0; kk < onPixel[k]->getNumberOfActualElements(); kk++)
              tmptracks[tmpNum]->addCalibSrc(&onPixel[k]->getElementr(kk));
          }
          tmptracks[tmpNum]->setPath(line);
          for (int k = 0; k < nndetpos; k++)
            if (tmptracks[tmpNum]->getNumberOfCalibHits(checkdets[k]) <= 0) {
              tmpHit = searchElement(line, k, 1);
              if (tmpHit != NULL)
                tmptracks[tmpNum]->addCalibSrc(tmpHit);
            }
          chi = trackFit(tmptracks[tmpNum], 4, maxDistance[trackID]);
          distance = target->distance(tmptracks[tmpNum]->getPath());
          if (1 == 0 && distance.R() <= maxTargetDist &&
              (tmptracks[tmpNum]->getNumberOfCalibHits(21) > 0 ||
               tmptracks[tmpNum]->getNumberOfCalibHits(22) > 0)) {
            /***************************************
             *prompt tracking done elsewhere
             ***************************************/
            continue;
            if (!(chi >= 0 && chi < maxChiPrompt[trackID]))
              continue;
            nPoints = 0;
            for (int k = 0; k < setup.getNumberOfDetectors(); k++)
              if (tmptracks[tmpNum]->getNumberOfCalibHits(k) > 0)
                nPoints++;
            if (nPoints < minPoints[trackID])
              continue;
            if (tmptracks[tmpNum]->getNumberOfCalibHits(15) <= 0 ||
                tmptracks[tmpNum]->getNumberOfCalibHits(16) <= 0)
              continue;
            //		      promptTracks[nprompttracks++]=tmptracks[tmpNum];
            tmpNum++;
          } else if (1 == 1 || distance.R() > maxTargetDist) {
            // cout<<"D"<<trackID<<line.toString(_spheric_).data()<<nOnPixel<<endl;
            nPoints = 0;
            for (int k = 0; k < setup.getNumberOfDetectors(); k++)
              if (tmptracks[tmpNum]->getNumberOfCalibHits(k) > 0)
                nPoints++;
            // 		      cout<<"\t"<<nPoints<<"(";
            // 		      for(int k=0;k<setup.getNumberOfDetectors();k++)
            // 			if(tmptracks[tmpNum]->getNumberOfCalibHits(k)>0)cout<<k<<",
            // ";
            // 		      cout<<")"<<endl;
            if (!(chi >= 0 && chi < maxChiDecay[trackID])) {
              // cout<<"\tbad chi-squared:"<<chi<<" "<<distance.R()<<endl;
              continue;
            }
            if ((trackID < 2 && nPoints < 8) || (trackID >= 2 && nPoints < 7)) {
              // cout<<"\tnot enough points on track :"<<distance.R()<<endl;
              continue;
            }
            if (tmptracks[tmpNum]->getNumberOfCalibHits(21) > 0 &&
                tmptracks[tmpNum]->getNumberOfCalibHits(22) > 0) {
              // cout<<"\tboth mu-strip found "<<distance.R()<<endl;
              continue;
            }
            decayTracks[ndecaytracks++] = tmptracks[tmpNum];
            tmpNum++;
          }
          // 		  cout<<line.toString(_spheric_).data()<<chi<<"(";
          // 		  for(int k=0;k<setup.getNumberOfDetectors();k++)
          // 		    if(tmptracks[tmpNum]->getNumberOfCalibHits(k)>0)cout<<k<<",
          // ";
          // 		  cout<<")"<<endl;
        }
      }
    }
  }
  /*
  for(int i=0;i<numberOfTracks;i++)
    {
      cout<<tracks[i]->getPath().toString(_spheric_).data()<<tracks[i]->getChiSquared()<<"(";
      for(int k=0;k<setup.getNumberOfDetectors();k++)
        if(tracks[i]->getNumberOfCalibHits(k)>0)cout<<k<<", ";
      cout<<")"<<endl;
      }*/
  float values[ndecaytracks];
  for (int i = 0; i < ndecaytracks; i++) {
    values[i] = 0;
    for (int j = 0; j < setup.getNumberOfDetectors(); j++)
      if (decayTracks[i]->getNumberOfCalibHits(j) > 0)
        values[i]++;
    values[i] = 1. / values[i];
  }
  sortTracks(ndecaytracks, decayTracks, values);
  eliminateDoubleTracks(ndecaytracks, decayTracks, maxPromptEqual);
  /*for(int i=0;i<ndecaytracks;i++)
    {
      cout<<"\t"<<decayTracks[i]->getPath().toString(_spheric_).data()<<decayTracks[i]->getChiSquared()<<"(";
      for(int k=0;k<setup.getNumberOfDetectors();k++)
        if(decayTracks[i]->getNumberOfCalibHits(k)>0)cout<<k<<", ";
      cout<<")"<<endl;
    }
    cout<<"left"<<endl;*/

  eliminateTooCloseToPrompt(ndecaytracks, decayTracks, maxPromptEqual);
/*for(int i=0;i<ndecaytracks;i++)
  {
    cout<<"\t"<<decayTracks[i]->getPath().toString(_spheric_).data()<<decayTracks[i]->getChiSquared()<<"(";
    for(int k=0;k<setup.getNumberOfDetectors();k++)
      if(decayTracks[i]->getNumberOfCalibHits(k)>0)cout<<k<<", ";
    cout<<")"<<endl;
  }
  cout<<endl;*/

/*  for(int i=0;i<nprompttracks;i++)
  {
    cout<<promptTracks[i]->getPath().toString(_spheric_).data()<<promptTracks[i]->getChiSquared()<<"(";
    for(int k=0;k<setup.getNumberOfDetectors();k++)
      if(promptTracks[i]->getNumberOfCalibHits(k)>0)cout<<k<<", ";
    cout<<")"<<endl;
  }
for(int i=0;i<ndecaytracks;i++)
  {
    cout<<"\t"<<decayTracks[i]->getPath().toString(_spheric_).data()<<decayTracks[i]->getChiSquared()<<"(";
    for(int k=0;k<setup.getNumberOfDetectors();k++)
      if(decayTracks[i]->getNumberOfCalibHits(k)>0)cout<<k<<", ";
    cout<<")"<<endl;
  }
cout<<endl;
*/
// sortAndCopyPrompt(nprompttracks,promptTracks);
#ifdef DEBUGGING
  cout << "prompt: " << nprompttracks << ", decay: " << ndecaytracks << endl;
#endif
  int nincommon;
  //   float chis[ndecaytracks];
  //   for(int i=0;i<ndecaytracks;i++)
  //     chis[i]=decayTracks[i]->getChiSquared();
  //   sortTracks(ndecaytracks,decayTracks,chis);
  //   eliminateDoubleTracks(ndecaytracks,decayTracks,maxPromptEqual);
  //   for(int i=0;i<ndecaytracks;i++)
  //     cout<<decayTracks[i]->getPath().toString(_spheric_).data()<<decayTracks[i]->getChiSquared()<<"
  //     "<<decayTracks[i]->getNumberOfCalibHits()<<endl;
  // search vees
  for (int i = 0; i < ndecaytracks; i++) {
    sLine3D l1(decayTracks[i]->getPath());
    for (int j = i + 1; j < ndecaytracks; j++) {
      sLine3D l2(decayTracks[j]->getPath());
      lLine3D poca(l1 - l2);
#ifdef DEBUG_VEE
      cout << i << " " << j << " " << poca << endl;
#endif
      if (!(poca.length() < maxDistVee))
        continue;
      nincommon = nElementsInCommon(decayTracks[i], decayTracks[j]);
      //	  cout<<nincommon<<endl;
      if (nincommon > maxVeeCommon)
        continue;
      if (poca.Mid().Z() > 100)
        continue;
      plane3D plane(poca.Mid(), l1.Direction(), l2.Direction());
      distance = plane & target->getCenter();
      //	  cout<<distance.r()<<endl;
      if (distance.R() < maxVeeTargetDist) {
        chi = veeFit(decayTracks[i], decayTracks[j]);
        // cout<<chi<<endl;
        if (!(chi >= 0 && chi <= maxChiVee))
          continue;
        veeTracks[nveetracks * 2] = decayTracks[i];
        veeTracks[nveetracks * 2 + 1] = decayTracks[j];
        nveetracks++;
      }
    }
  }
#ifdef DEBUGGING
  cout << "vees:" << nveetracks << endl;
#endif
  sortAndCopyVee(nveetracks, veeTracks);
#ifdef DEBUGGING
  cout << "APixelTracking::process() done" << endl;
#endif
  return ptr;
}
#include "algorithmparameter.h"
#include "cylinder.h"
APixelTracking::APixelTracking(TSetup& setupIn, TTrack** tracksIn, TPixel*** pixelsIn,
                               TCluster*** clusterIn, TCalibHit*** hitsIn, int& nTracksIn,
                               int** nPixelsIn, int** nClusterIn, int** nHitsIn, int maxTracksIn,
                               const algorithm_parameter& param)
    : AAlgorithm("pixel track search"), numberOfTracks(nTracksIn), setup(setupIn),
      maxTracks(maxTracksIn)
{
  tracks = tracksIn;
  pixels = pixelsIn;
  clusters = clusterIn;
  hits = hitsIn;
  numberOfPixels = nPixelsIn;
  numberOfClusters = nClusterIn;
  numberOfHits = nHitsIn;
  tmptracks = new TTrack*[maxTracks];
  for (int i = 0; i < maxTracks; i++)
    tmptracks[i] = new TTrack(10, setup.getNumberOfDetectors() + 2);
  target = new cylinder(point3D(0, 0, 0), vector3D(0, 0, 3), 2); //&setup.getTargetr().getShaper();
  maxPromptEqual = param.value(0).value<int>();
  maxVeeCommon = param.value(1).value<int>();
  minPointsVee = param.value(2).value<int>();
  maxDecayStart = param.value(3).value<int>();
  maxTargetDist = param.value(4).value<float>();
  maxVeeTargetDist = param.value(5).value<float>();
  maxDistVee = param.value(6).value<float>();
  maxChiVee = param.value(7).value<float>();
  maxDiff = new int[3];
  minPoints = new int[3];
  maxDistance = new float[3];
  maxChiPrompt = new float[3];
  maxChiDecay = new float[3];
  for (int i = 0; i < 3; i++) {
    maxDiff[i] = param.value(8).value<vector<int>>().at(i);
    minPoints[i] = param.value(9).value<vector<int>>().at(i);
    maxDistance[i] = param.value(10).value<vector<float>>().at(i);
    maxChiPrompt[i] = param.value(11).value<vector<float>>().at(i);
    maxChiDecay[i] = param.value(12).value<vector<float>>().at(i);
  }
  nndetpos = 15;
  ndetpos = new int[nndetpos];
  detpos = new int*[nndetpos];
  checkdets = new int[nndetpos];
  for (int i = 0; i < 8; i++)
    checkdets[i] = i;
  for (int i = 8; i < nndetpos; i++)
    checkdets[i] = i + 9;
  for (int i = 0; i < nndetpos; i++)
    detpos[i] = new int[1000];
  nnpixpos = 5;
  npixpos = new int[nnpixpos];
  pixpos = new int*[nnpixpos];
  checkpix = new int[nnpixpos];
  checkpix[0] = 3;
  checkpix[1] = 4;
  checkpix[2] = 5;
  checkpix[3] = 0;
  checkpix[4] = 1;
  for (int i = 0; i < nnpixpos; i++)
    pixpos[i] = new int[1000];
  /*
  cout<<"maxPromptEqual "<<maxPromptEqual<<endl;
  cout<<"maxVeeCommon "<<maxVeeCommon<<endl;
  cout<<"minPointsVee "<<minPointsVee<<endl;
  cout<<"maxDecayStart "<<maxDecayStart<<endl;
  cout<<"maxTargetDist "<<maxTargetDist<<endl;
  cout<<"maxVeeTargetDist "<<maxVeeTargetDist<<endl;
  cout<<"maxDistVee "<<maxDistVee<<endl;
  cout<<"maxChiVee "<<maxChiVee<<endl;
  cout<<"max diff suspect: "<<maxDiff[0]<<" "<<maxDiff[1]<<"
  "<<maxDiff[2]<<endl;
  cout<<"min Points: "<<minPoints[0]<<" "<<minPoints[1]<<"
  "<<minPoints[2]<<endl;
  cout<<"max distance: "<<maxDistance[0]<<" "<<maxDistance[1]<<"
  "<<maxDistance[2]<<endl;
  cout<<"max chi P: "<<maxChiPrompt[0]<<" "<<maxChiPrompt[1]<<"
  "<<maxChiPrompt[2]<<endl;
  cout<<"max chi D: "<<maxChiDecay[0]<<" "<<maxChiDecay[1]<<"
  "<<maxChiDecay[2]<<endl;
  */
}
APixelTracking::~APixelTracking()
{
  delete[] maxDiff;
  delete[] maxDistance;
  delete[] minPoints;
  delete[] maxChiPrompt;
  delete[] maxChiDecay;
  for (int i = 0; i < maxTracks; i++)
    delete tmptracks[i];
  delete[] tmptracks;
  for (int i = 0; i < nndetpos; i++)
    delete[] detpos[i];
  delete[] detpos;
  delete[] ndetpos;
  delete[] checkdets;
  for (int i = 0; i < nnpixpos; i++)
    delete[] pixpos[i];
  delete[] pixpos;
  delete[] npixpos;
  delete[] checkpix;
}
algorithm_parameter APixelTracking::getDescription()
{
  algorithm_parameter ret("Pixel Tracking", 0);
  string des = "This Algorithm is a very simple tracking algorithm to"
               "produce out of any 2 pixels in the hodoscopes a"
               "track. Stop pixel is requested."
               "Searches prompt tracks and decay tracks. decay tracks are "
               "assembled to vees. A check for coplanarity with target is"
               "performed.";
  ret.setDescription(des);
  ret.addValue("max Prompt Equal", static_cast<int>(0));
  ret.addValue("max Vee Equal", static_cast<int>(0));
  ret.addValue("min Points Vee", static_cast<int>(0));
  ret.addValue("max Decay Start", static_cast<int>(0));
  ret.addValue("max Target Dist", 1.f);
  ret.addValue("max Vee Target Dist", 1.f);
  ret.addValue("max Dist Vee", 1.f);
  ret.addValue("max Chi Vee", 1.f);
  vector<int> tmpi;
  vector<float> tmpf;
  ret.addValue("max Diff suspect", tmpi);
  ret.addValue("min # Points", tmpi);
  ret.addValue("max Distance", tmpf);
  ret.addValue("max Chi Prompt", tmpf);
  ret.addValue("max Chi Decay", tmpf);
  return ret;
}
#include "ring.h"
#include "spiral.h"
#include "wedge.h"
void APixelTracking::setTrackErrors(TTrack* tr)
{
  //  cout<<""<<endl;
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
