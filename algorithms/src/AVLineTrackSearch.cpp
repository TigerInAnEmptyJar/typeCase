#include "AVLineTrackSearch.h"
#include <stdlib.h>
class trackVertexTrack;
//#define SHOWPARAMETER
//#define DEBUGINFO
//#define DEBUGCHECK
//#define DEBUGTRACKING
//#define DEBUGPLANING
//#define DEBUGTRACKSEARCH

algorithm_parameter AVLineTrackSearch::getDescription()
{
  algorithm_parameter ret("V-Line Track search", 0, 0);
  string des = "This is a track search algorithm in a detector"
               "without magnetic field, with thin detectors"
               "for decays of neutral particles."
               "The algorithm assumes a prompt neutral track"
               "that decays and causes hits in two stop detectors"
               "=>two stop pixels/clusters."
               "Out of momentum-conservation reasons, the tracks"
               "all lie in the plane defined by the target and the"
               "two pixel centers."
               "Here there are pixels/clusters searched that are"
               "close to the plane. Out of these pixels tracks are"
               "searched with a secondary vertex point close to the "
               "plane. "
               "The angles of the tracks have to be able to add "
               "up to the primary track."
               "As for the assignment of the hits in the detector"
               "to the track, two modes are possible:"
               "-If the enveloping shape of the detector-elements"
               " in the detector is regular, hits around a suspect"
               " element (taken from volumeShape::suspect()) are"
               " taken."
               "-Every hit element of the detector is checked."
               " If the assumed line is close enough this element"
               " is used. This method is slower than the other one!";
  ret.setDescription(des);
  ret.addValue("", false);
  ret.addValue("", false);
  ret.addValue("max number of elements in 2 tracks", static_cast<int>(2));
  ret.addValue("min # elements on track-vertex-track", static_cast<int>(7));
  ret.addValue("max Distance Vertex-Plane", 10.f);
  ret.addValue("maximum chi-squared for track-vertex-track", 10.f);
  ret.addValue("max angle diff pri to secondary", 0.05f);
  ret.addValue("max Distance pixel-plane", 10.f);
  vector<int> tmp;
  vector<float> tmp2;
  ret.addValue("pixel stop-IDs", tmp);
  ret.addValue("pixel min # elements on track", tmp);
  ret.addValue("pixel: essential detector IDs", tmp);
  ret.addValue("cluster stop-IDs", tmp);
  ret.addValue("cluster min # elements on track", tmp);
  ret.addValue("cluster: essential detector IDs", tmp);
  ret.addValue("pixel start-IDs", tmp);
  ret.addValue("cluster start-IDs", tmp);
  ret.addValue("min start objects per plane", tmp);
  ret.addValue("detectors", tmp);
  ret.addValue("element search mode", tmp);
  ret.addValue("conventional search detector", tmp);
  ret.addValue("must not detectors", tmp);
  ret.addValue("max element distance", tmp);
  ret.addValue("max element suspect-distance", tmp);
  ret.addValue("pixel: max chi squared for track", tmp2);
  ret.addValue("cluster: max chi squared for track", tmp2);
  ret.addValue("max distance", tmp2);
  ret.addValue("vertex z-position from target", tmp2);
  return ret;
}
void coutHitsInTrack(TTrack* tr, const char* separator)
{
  for (int i = 0; i < tr->getNumberOfCalibHits(-2); i++)
    for (int j = 0; j < tr->getNumberOfCalibHits(i); j++)
      cout << tr->getCalibSrcr(i, j)->getDetector() << "-" << tr->getCalibSrcr(i, j)->getElement()
           << separator;
}
#include "fittingRoutines.h"
class trackVertexTrack
{
private:
  point3D vertex;
  vector3D dir1, dir2;
  plane3D pl;
  vertexFit fit;
  TTrack *t1, *t2;
  float chi;
  int num;

public:
  trackVertexTrack(int maxHit) : fit(2, maxHit) {}
  ~trackVertexTrack() {}
  void setTracks(TTrack* tr1, TTrack* tr2);
  vector3D getDirection(int i) const;
  point3D getVertex() const;
  float getChiSquared() const;
  plane3D getPlane() const;
  int getNumberOfElements() const;
  TTrack* getTrack(int i);
};
vector3D trackVertexTrack::getDirection(int i) const
{
  if (i == 0)
    return dir1;
  if (i == 1)
    return dir2;
  return vector3D();
}
point3D trackVertexTrack::getVertex() const { return vertex; }
float trackVertexTrack::getChiSquared() const { return chi; }
plane3D trackVertexTrack::getPlane() const { return pl; }
int trackVertexTrack::getNumberOfElements() const { return num; }
TTrack* trackVertexTrack::getTrack(int i)
{
  if (i == 0)
    return t1;
  if (i == 1)
    return t2;
  return NULL;
}
void trackVertexTrack::setTracks(TTrack* tr1, TTrack* tr2)
{
  t1 = tr1;
  t2 = tr2;
  fit.clear();
  int maxDet = tr1->getNumberOfCalibHits(-2);
  int nInCommon = 0;
  for (int i = 10; i < 26; i++)
    for (int j = 0; j < tr1->getNumberOfCalibHits(i); j++)
      for (int k = 0; k < tr2->getNumberOfCalibHits(i); k++)
        if (tr1->getCalibSrcr(i, j)->getElement() == tr2->getCalibSrcr(i, k)->getElement())
          nInCommon++;
  //    fit(2,tr1->getNumberOfCalibHits()+tr2->getNumberOfCalibHits()+10);
  volumeShape* sh;
  for (int j = 0; j < maxDet; j++)
    for (int i = 0; i < tr1->getNumberOfCalibHits(j); i++) {
      if (!(tr1->getCalibSrcr(j, i)->isValid() && tr1->getCalibSrcr(j, i)->getHitShape() != NULL))
        continue;
      sh = tr1->getCalibSrcr(j, i)->getHitShape();
      Vector hp(sh->HitParams(tr1->getPath()));
      fit.addPoint(vector3D(hp[7], hp[8], hp[9]), vector3D(hp[4], hp[5], hp[6]), hp[0], 0);
    }
  for (int j = 0; j < maxDet; j++)
    for (int i = 0; i < tr2->getNumberOfCalibHits(j); i++) {
      if (!(tr2->getCalibSrcr(j, i)->isValid() && tr2->getCalibSrcr(j, i)->getHitShape() != NULL))
        continue;
      sh = tr2->getCalibSrcr(j, i)->getHitShape();
      Vector hp(sh->HitParams(tr2->getPath()));
      fit.addPoint(vector3D(hp[7], hp[8], hp[9]), vector3D(hp[4], hp[5], hp[6]), hp[0], 1);
    }
  chi = fit.fit();
  vertex = fit.Vertex(); //.setValues(v[0],v[1],v[6]);
  dir1 = fit.Line(0);    //.setValues(v[2],v[3],1);
  dir2 = fit.Line(1);    //.setValues(v[4],v[5],1);
  dir1.normalize();
  dir2.normalize();
  pl = plane3D(vertex, dir1, dir2);
  int sum = 0;
  num = tr1->getNumberOfCalibHits() + tr1->getNumberOfCalibHits() - sum;
  if (nInCommon > 2)
    chi = 1000;
}
int nEventsWithVees;
AVLineTrackSearch::AVLineTrackSearch(TSetup& setupIn, TTrack** tracksIn, TCluster*** clustersIn,
                                     TPixel*** pixelsIn, TCalibHit*** hitsIn, int maxTracksIn,
                                     int& numberOfTracksIn, int** numberOfClustersIn,
                                     int** numberOfPixelsIn, int** numberOfHitsIn,
                                     const algorithm_parameter& descr)
    : AAlgorithm("V-Line Track search"), setup(setupIn), maxTracks(maxTracksIn),
      numberOfTracks(numberOfTracksIn)
{
  nEventsWithVees = 0;
  hits = hitsIn;
  pixels = pixelsIn;
  clusters = clustersIn;
  tracks = tracksIn;
  numberOfHits = numberOfHitsIn;
  numberOfPixels = numberOfPixelsIn;
  numberOfClusters = numberOfClustersIn;

  numStop = descr.value(8).value<vector<int>>().size();
  stopIDs = new int[numStop];
  numEssentials = new int[numStop];
  essentials = new int*[numStop];
  minElements = new int[numStop];
  maxChi = new float[numStop];
  int j = 0;
  for (int i = 0; i < numStop; i++) {
    stopIDs[i] = descr.value(8).value<vector<int>>().at(i);
    minElements[i] = descr.value(9).value<vector<int>>().at(i);
    maxChi[i] = descr.value(23).value<vector<float>>().at(i);
    numEssentials[i] = descr.value(10).value<vector<int>>().at(j++);
    essentials[i] = new int[numEssentials[i]];
    for (int k = 0; k < numEssentials[i]; k++)
      essentials[i][k] = descr.value(10).value<vector<int>>().at(j++);
  }

  numStopCluster = descr.value(11).value<vector<int>>().size();
  stopClusterIDs = new int[numStopCluster];
  numClusterEssentials = new int[numStopCluster];
  clusterEssentials = new int*[numStopCluster];
  minClusterElements = new int[numStopCluster];
  maxClusterChi = new float[numStopCluster];
  j = 0;
  for (int i = 0; i < numStopCluster; i++) {
    stopClusterIDs[i] = descr.value(11).value<vector<int>>().at(i);
    minClusterElements[i] = descr.value(12).value<vector<int>>().at(i);
    maxClusterChi[i] = descr.value(24).value<vector<float>>().at(i);
    numClusterEssentials[i] = descr.value(13).value<vector<int>>().at(j++);
    clusterEssentials[i] = new int[numClusterEssentials[i]];
    for (int k = 0; k < numClusterEssentials[i]; k++)
      clusterEssentials[i][k] = descr.value(13).value<vector<int>>().at(j++);
  }
  maxDistance = new float[numStop + numStopCluster];
  for (int i = 0; i < numStop; i++)
    maxDistance[i] = descr.value(25).value<vector<float>>().at(i);
  for (int i = 0; i < numStopCluster; i++)
    maxDistance[i + numStop] = descr.value(25).value<vector<float>>().at(i + numStop);

  numStart = descr.value(14).value<vector<int>>().size();
  startIDs = new int[numStart];
  for (int i = 0; i < numStart; i++)
    startIDs[i] = descr.value(14).value<vector<int>>().at(i);
  numStartCluster = descr.value(15).value<vector<int>>().size();
  startClusterIDs = new int[numStartCluster];
  for (int i = 0; i < numStartCluster; i++)
    startClusterIDs[i] = descr.value(15).value<vector<int>>().at(i);
  minStarts = new int[numStart + numStartCluster];
  for (int i = 0; i < numStart + numStartCluster; i++)
    minStarts[i] = descr.value(16).value<vector<int>>().at(i);
  numDetectors = descr.value(17).value<vector<int>>().size();
  detectorIDs = new int[numDetectors];
  conventional = new int[numDetectors];
  for (int i = 0; i < numDetectors; i++) {
    detectorIDs[i] = descr.value(17).value<vector<int>>().at(i);
    conventional[i] = descr.value(18).value<vector<int>>().at(i);
  }
  numNoGo = descr.value(19).value<vector<int>>().size();
  noGoDetectors = new int[numNoGo];
  for (int i = 0; i < numNoGo; i++)
    noGoDetectors[i] = descr.value(19).value<vector<int>>().at(i);
  away = new int[(numDetectors + numNoGo) * (numStop + numStopCluster)];
  for (int i = 0; i < (numDetectors + numNoGo) * (numStop + numStopCluster); i++)
    away[i] = descr.value(20).value<vector<int>>().at(i);
  maxDistanceVertexPlane = descr.value(4).value<float>();
  maxVertexChi = descr.value(5).value<float>();
  maxAngle = descr.value(6).value<float>();
  minElementsPerVee = descr.value(3).value<int>();
  maxEqualElements = descr.value(2).value<int>();
  maxDistanceToPlane = descr.value(7).value<float>();
  numZDists = descr.value(26).value<vector<float>>().size() / 2;
  Zdists = new float[numZDists * 2];
  for (int i = 0; i < numZDists; i++) {
    Zdists[i * 2] = descr.value(26).value<vector<float>>().at(i * 2);
    Zdists[i * 2 + 1] = descr.value(26).value<vector<float>>().at(i * 2 + 1);
  }

  tmpTracks = new TTrack*[maxTracks];
  for (int i = 0; i < maxTracks; i++)
    tmpTracks[i] = new TTrack(20, tracks[0]->getNumberOfCalibHits(-2));

  maxPlanes = maxTracks / 2;
  planeTrack = new TTrack**[2];
  pointTracks = new TTrack**[2];
  plane = new plane3D[maxPlanes];
  numPlanePoints = new int[maxPlanes];
  planePoints = new TBase**[maxPlanes];
  hitParameter = NULL; // new Vector**[2];
  for (int i = 0; i < 2; i++) {
    planeTrack[i] = new TTrack*[maxPlanes];
    pointTracks[i] = new TTrack*[maxTracks];
    //      hitParameter[i]=new Vector*[maxTracks];
    for (int j = 0; j < maxPlanes; j++)
      planeTrack[i][j] = new TTrack(20, tracks[0]->getNumberOfCalibHits(-2));
    for (int j = 0; j < maxTracks; j++) {
      pointTracks[i][j] = new TTrack(20, tracks[0]->getNumberOfCalibHits(-2));
      //	  hitParameter[i][j]=new Vector[setup.getNumberOfDetectors()*5];
      //	  for(int k=0;k<setup.getNumberOfDetectors();k++)
      //	    hitParameter[i][j][k].reDimension(13);
    }
  }
  for (int j = 0; j < maxPlanes; j++)
    planePoints[j] = new TBase*[maxTracks];
  vee = new trackVertexTrack*[maxPlanes];
  for (int i = 0; i < maxPlanes; i++)
    vee[i] = new trackVertexTrack(100);
  doAlternateSearch = descr.value(1).value<bool>();
  alternateDist = 0.8;
  maxEl = 200;
  nDet = setup.getNumberOfDetectors();
  nEl = new int[nDet];
  chels = new TCalibHit**[nDet];
  for (int i = 0; i < nDet; i++)
    chels[i] = new TCalibHit*[maxEl];
  checkForPromptElements = descr.value(0).value<bool>();
//  maxPixelPrompt = descr.value(?).value<int>();
#ifdef SHOWPARAMETER
  cout << "Plane defining points:" << endl;
  cout << "  pixels: " << flush;
  for (int i = 0; i < numStop; i++)
    cout << stopIDs[i] << "\t" << flush;
  cout << endl << "  min # : " << flush;
  for (int i = 0; i < numStop; i++)
    cout << minElements[i] << "\t" << flush;
  cout << endl << "  max x-squared: " << flush;
  for (int i = 0; i < numStop; i++)
    cout << maxChi[i] << "\t" << flush;
  cout << endl << "  mx dist:" << flush;
  for (int i = 0; i < numStop; i++)
    cout << maxDistance[i] << "\t" << flush;

  cout << endl << "  clusters: " << flush;
  for (int i = 0; i < numStopCluster; i++)
    cout << stopClusterIDs[i] << "\t" << flush;
  cout << endl << "  min # : " << flush;
  for (int i = 0; i < numStopCluster; i++)
    cout << minClusterElements[i] << "\t" << flush;
  cout << endl << "  max x-squared: " << flush;
  for (int i = 0; i < numStopCluster; i++)
    cout << maxClusterChi[i] << "\t" << flush;
  cout << endl << "  mx dist:" << flush;
  for (int i = 0; i < numStopCluster; i++)
    cout << maxDistance[i + numStop] << "\t" << flush;

  cout << "\n" << endl;
  cout << "in Plane points: " << endl;
  cout << "pixels   :" << flush;
  for (int i = 0; i < numStart; i++)
    cout << startIDs[i] << "\t" << flush;
  cout << "\nclusters :" << flush;
  for (int i = 0; i < numStartCluster; i++)
    cout << startClusterIDs[i] << "\t" << flush;
  cout << "\n\nDetectors : " << flush;
  for (int i = 0; i < numDetectors; i++)
    cout << detectorIDs[i] << "\t" << flush;
  cout << endl << "          : " << flush;
  for (int i = 0; i < numDetectors; i++)
    cout << conventional[i] << "\t" << flush;
  cout << endl << "  suspectDist:\n";
  for (int i = 0; i < numStop; i++) {
    cout << "  pix " << stopIDs[i] << "   : " << flush;
    for (int j = 0; j < numDetectors; j++)
      cout << away[i * (numDetectors + numNoGo) + j] << "\t" << flush;
    cout << endl;
  }
  for (int i = 0; i < numStopCluster; i++) {
    cout << "  Clu " << stopClusterIDs[i] << "   : " << flush;
    for (int j = 0; j < numDetectors; j++)
      cout << away[(numStop + i) * (numDetectors + numNoGo) + j] << "\t" << flush;
    cout << endl;
  }
  cout << endl << "No go detectors:" << flush;
  for (int i = 0; i < numNoGo; i++)
    cout << noGoDetectors[i] << "\t" << flush;
  cout << endl << "  suspectDist:\n";
  for (int i = 0; i < numStop; i++) {
    cout << "  pix " << stopIDs[i] << "       : " << flush;
    for (int j = 0; j < numNoGo; j++)
      cout << away[i * (numDetectors + numNoGo) + j + numDetectors] << "\t" << flush;
    cout << endl;
  }
  for (int i = 0; i < numStopCluster; i++) {
    cout << "  Clu " << stopClusterIDs[i] << "       : " << flush;
    for (int j = 0; j < numNoGo; j++)
      cout << away[(numStop + i) * (numDetectors + numNoGo) + j + numDetectors] << "\t" << flush;
    cout << endl;
  }
  cout << "distances for vertex to target in Z:" << endl;
  for (int i = 0; i < numZDists; i++)
    cout << Zdists[i * 2] << " - " << Zdists[i * 2 + 1] << "\t" << flush;
  cout << endl;
#endif
}
AVLineTrackSearch::~AVLineTrackSearch()
{
  delete[] stopIDs;
  delete[] numEssentials;
  for (int i = 0; i < numStop; i++)
    delete[] essentials[i];
  delete[] essentials;
  delete[] minElements;
  delete[] maxChi;
  delete[] stopClusterIDs;
  delete[] numClusterEssentials;
  for (int i = 0; i < numStopCluster; i++)
    delete[] clusterEssentials[i];
  delete[] clusterEssentials;
  delete[] minClusterElements;
  delete[] maxClusterChi;
  delete[] maxDistance;
  delete[] away;

  delete[] startIDs;
  delete[] startClusterIDs;
  delete[] minStarts;
  delete[] detectorIDs;
  delete[] conventional;
  delete[] noGoDetectors;
  delete[] Zdists;
  for (int i = 0; i < maxTracks; i++)
    delete tmpTracks[i];
  delete[] tmpTracks;
  for (int j = 0; j < maxPlanes; j++)
    delete vee[j];
  delete[] vee;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < maxPlanes; j++)
      delete planeTrack[i][j];
    for (int j = 0; j < maxTracks; j++) {
      delete pointTracks[i][j];
      //	  delete []hitParameter[i][j];
    }
    delete[] planeTrack[i];
    delete[] pointTracks[i];
    //      delete []hitParameter[i];
  }
  delete[] nEl;
  for (int i = 0; i < nDet; i++)
    delete[] chels[i];
  delete[] chels;

  return;
  for (int j = 0; j < maxPlanes; j++)
    delete[] planePoints[j];
  delete[] planeTrack;
  delete[] pointTracks;
  delete[] plane;
  delete[] numPlanePoints;
  delete[] planePoints;
  //  delete []hitParameter;
}
bool AVLineTrackSearch::searchATrack(int tracknum, int basis, int stopI, TBase* pix, TBase* spix)
{
  if (tracknum >= maxTracks)
    return false;
  if (pix == NULL)
    return false;
  if (spix == NULL)
    return false;
  if (!(pix->name() == "TPixel" || pix->name() == "TCluster"))
    return false;
  if (!(spix->name() == "TPixel" || spix->name() == "TCluster"))
    return false;
  point3D vertex(0, 0, 0), p1, p2;
  int typ1, typ2;
  if (pix->name() == "TPixel")
    typ1 = 0;
  else
    typ1 = 1;
  if (spix->name() == "TPixel")
    typ2 = 0;
  else
    typ2 = 1;
  if (typ1 == 0)
    p1 = ((TPixel*)pix)->getShape()->getCenter();
  else
    p1 = ((TCluster*)pix)->getCenter();
  if (typ2 == 0)
    p2 = ((TPixel*)spix)->getShape()->getCenter();
  else
    p2 = ((TCluster*)spix)->getCenter();
  sLine3D path(p2, p1 - p2);
#ifdef DEBUGTRACKSEARCH
  cout << "searchATrack()" << path << endl;
#endif
  bool found;
  TDetector* det;
  volumeShape* shape;
  int suspect;
  Vector hitParam(13), bestHit(13);
  float bestD = 1000000;
  int bestEl;
  vector3D dis;
  TPixel* ppix;
  int closest;
  vector3D dist, v1;
  int maxHitPerTrack = 100;
  Vector parameter[maxHitPerTrack];
  for (int i = 0; i < maxHitPerTrack; i++)
    parameter[i].reDimension(13);
  int constant = (typ1 * numStop + stopI) * (numDetectors + numNoGo) + numDetectors;
  TTrack* tr = pointTracks[basis][tracknum];
  for (int i = 0; i < numNoGo; i++) {
    for (int k = 0; k < tr->getNumberOfPixels(); k++) {
      ppix = &tr->getPixel(0, k);
      for (int j = 0; j < ppix->getNumberOfActualElements(); j++)
        if (noGoDetectors[i] == ppix->getElementr(j).getDetector()) {
#ifdef DEBUGTRACKSEARCH
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
#ifdef DEBUGTRACKING
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
          return false; // has no go detector
        }
    }
    det = &setup.getDetectorr(noGoDetectors[i]);
    shape = det->getShape(0);
    suspect = shape->suspect(path, det->getStackType());
    closest = 0;
    if (suspect >= 0 && suspect < static_cast<int>(det->getNumberOfElements())) {
      for (int j = 0; j < /*(*(numberOfHits[noGoDetectors[i]]))*/ nEl[noGoDetectors[i]]; j++) {
        if (!/*hits*/ chels[noGoDetectors[i]][j]->isValid())
          continue;
        if (/*hits*/ chels[noGoDetectors[i]][j]->getHitShape() == NULL)
          continue;
        if (abs(/*hits*/ chels[noGoDetectors[i]][closest]->getElement() - suspect) >
            abs(/*hits*/ chels[noGoDetectors[i]][j]->getElement() - suspect))
          closest = j;
        if (/*hits*/ chels[noGoDetectors[i]][j]->getElement() == suspect) {
#ifdef DEBUGTRACKSEARCH
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
#ifdef DEBUGTRACKING
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
          return false; // has no go detector
        } else if (abs(/*hits*/ chels[noGoDetectors[i]][j]->getElement() - suspect) <=
                   away[constant + i]) {
#ifdef DEBUGTRACKSEARCH
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
#ifdef DEBUGTRACKING
          cout << "no go detector found " << noGoDetectors[i] << endl;
#endif
          return false; // has no go detector
        }
      }
    }
  }
  int num = 0, n1;
  for (int i = 0; i < tr->getNumberOfCalibHits(-2); i++)
    for (int j = 0; j < tr->getNumberOfCalibHits(i); j++) {
      bestHit = tr->getCalibSrcr(i, j)->getHitShape()->HitParams(path);
      if (vector3D(bestHit[10], bestHit[11], bestHit[12]).R() >
          maxDistance[typ1 * numStop + stopI]) {
        tr->removeCalibSrc(tr->getCalibSrcr(i, j));
        continue;
      }
      parameter[num] = bestHit;
      if (num < maxHitPerTrack - 1)
        num++;
    }
  constant = (typ1 * numStop + stopI) * (numDetectors + numNoGo);
  for (int i = 0; i < numDetectors; i++) {
    found = false;
    for (int k = 0; k < tr->getNumberOfPixels(); k++) {
      ppix = &tr->getPixel(0, k);
      for (int j = 0; j < ppix->getNumberOfActualElements(); j++)
        if (detectorIDs[i] == ppix->getElementr(j).getDetector()) {
          found = true;
        }
    }
    if (!found) {
      if (conventional[i] == 1) {
        bestEl = -1;
        bestD = 10000000;
        for (int j = 0; j < /*(*(numberOfHits[detectorIDs[i]]))*/ nEl[detectorIDs[i]]; j++) {
          if (!/*hits*/ chels[detectorIDs[i]][j]->isValid())
            continue;
          shape = /*hits*/ chels[detectorIDs[i]][j]->getHitShape();
          if (shape == NULL)
            continue;
          hitParam = shape->HitParams(path);
          dist.setValues(hitParam[10], hitParam[11], hitParam[12]);
          if (dist.R() < __prec) {
            bestEl = j;
            bestHit = hitParam;
            break;
          }
          if (dist.R() > maxDistance[typ1 * numStop + stopI])
            continue;
          if (bestD > dist.R()) {
            bestEl = j;
            bestD = dist.R();
            bestHit = hitParam;
          }
        }
        if (bestEl < 0)
          continue;
        n1 = tr->getNumberOfCalibHits(detectorIDs[i]);
        tr->addCalibSrc(/*hits*/ chels[detectorIDs[i]][bestEl]);
        if (n1 == tr->getNumberOfCalibHits(detectorIDs[i]))
          continue;
        parameter[num] = bestHit;
        if (num < maxHitPerTrack - 1)
          num++;
      } else {
        det = &setup.getDetectorr(detectorIDs[i]);
        shape = det->getShape(0);
        suspect = shape->suspect(path, det->getStackType());
        closest = 0;
        if (suspect >= 0 && suspect < static_cast<int>(det->getNumberOfElements())) {
          for (int j = 0; j < nEl[detectorIDs[i]] /*(*(numberOfHits[detectorIDs[i]]))*/; j++) {
            if (!/*hits*/ chels[detectorIDs[i]][j]->isValid())
              continue;
            if (/*hits*/ chels[detectorIDs[i]][j]->getHitShape() == NULL)
              continue;
            if (abs(/*hits*/ chels[detectorIDs[i]][closest]->getElement() - suspect) >
                abs(/*hits*/ chels[detectorIDs[i]][j]->getElement() - suspect))
              closest = j;
            if (/*hits*/ chels[detectorIDs[i]][j]->getElement() == suspect)
              tr->addCalibSrc(/*hits*/ chels[detectorIDs[i]][j]);
            else if (abs(/*hits*/ chels[detectorIDs[i]][j]->getElement() - suspect) <=
                     away[constant + i])
              tr->addCalibSrc(/*hits*/ chels[detectorIDs[i]][j]);
          }
        }
      }
    }
  }

#ifdef DEBUGTRACKSEARCH
  cout << "# hits:" << tr->getNumberOfCalibHits(-1)
       << " min: " << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << endl;
  for (int i = 0; i < tr->getNumberOfCalibHits(-2); i++) {
    for (int j = 0; j < tr->getNumberOfCalibHits(i); j++)
      cout << "det:" << i << " el:" << tr->getCalibSrcr(i, j)->getElement() << endl;
  }
#endif
  bool condition =
      tr->getNumberOfCalibHits(-1) >= (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]);
  if (!condition) {
#ifdef DEBUGTRACKING
    cout << "searchATrack() condition not met " << tr->getNumberOfCalibHits(-1) << "("
         << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
#endif
    return false;
  }
  int tak = (typ1 == 0 ? numEssentials[stopI] : numClusterEssentials[stopI]),
      *tak2 = (typ1 == 0 ? essentials[stopI] : clusterEssentials[stopI]);
  for (int i = 0; i < tak; i++) {
    condition = (condition && (tr->getNumberOfCalibHits(tak2[i]) > 0));
#ifdef DEBUGTRACKSEARCH
    cout << "essential " << tak2[i] << ":" << tr->getNumberOfCalibHits(tak2[i]) << endl;
#endif
  }
  if (!condition) {
#ifdef DEBUGTRACKING
    cout << "searchATrack() condition not met " << tr->getNumberOfCalibHits(-1) << "("
         << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
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
  for (int i = 0; i < numDetectors; i++) {
    if (conventional[i])
      continue;
    for (int j = 0; j < tr->getNumberOfCalibHits(detectorIDs[i]); j++) {
      hit = tr->getCalibSrcr(detectorIDs[i], j);
      bestHit = hit->getHitShape()->HitParams(path);
      enter.setValues(bestHit[1], bestHit[2], bestHit[3]);
      htt.setValues(bestHit[7], bestHit[8], bestHit[9]);
      tmp = enter - vertex;
      tmp2 = htt - vertex;
      vector3D dist(bestHit[10], bestHit[11], bestHit[12]);
      if (dist.R() > maxDistance[numStop * typ1 + stopI]) {
        tr->removeCalibSrc(hit);
        continue;
      }
      parameter[num] = bestHit;
      if (num < maxHitPerTrack - 1)
        num++;
    }
  }
  condition =
      tr->getNumberOfCalibHits(-1) >= (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]);
  condition = (condition && (tr->getNumberOfCalibHits(6) + tr->getNumberOfCalibHits(0) +
                                 tr->getNumberOfCalibHits(3) >
                             0));
#ifdef DEBUGTRACKSEARCH
  cout << "# hits:" << tr->getNumberOfCalibHits(-1)
       << " min: " << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << endl;
#endif
  if (!condition) {
#ifdef DEBUGTRACKING
    cout << "searchATrack() condition not met " << tr->getNumberOfCalibHits(-1) << "("
         << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
#endif
    return false;
  }
  for (int i = 0; i < tak; i++) {
    condition = (condition && tr->getNumberOfCalibHits(tak2[i]) > 0);
#ifdef DEBUGTRACKSEARCH
    cout << "essential " << tak2[i] << ":" << tr->getNumberOfCalibHits(tak2[i]) << endl;
#endif
  }
  if (!condition) {
#ifdef DEBUGTRACKING
    cout << "searchATrack() condition not met " << tr->getNumberOfCalibHits(-1) << "("
         << (typ1 == 0 ? minElements[stopI] : minClusterElements[stopI]) << ")" << endl;
#endif
    return false;
  }
  if (tr->getNumberOfCalibHits(-1) > 4) {
    lineFit line1(tr->getNumberOfCalibHits(-1) + 2);
#ifdef DEBUGTRACKSEARCH
    cout << num << " " << tr->getNumberOfCalibHits() << endl;
#endif
    for (int i = 0; i < num; i++) {
#ifdef DEBUGTRACKSEARCH
      cout << "hitparam: " << parameter[i] << endl;
#endif
      line1.addPoint(vector3D(parameter[i][7], parameter[i][8], parameter[i][9]),
                     vector3D(parameter[i][4], parameter[i][5], parameter[i][6]), parameter[i][0]);
    }
    chi = line1.fit();
#ifdef DEBUGTRACKSEARCH
    cout << "chi-squared " << chi << " max " << (typ1 == 0 ? maxChi[stopI] : maxClusterChi[stopI])
         << endl;
#endif

    if (chi < 0 || chi > (typ1 == 0 ? maxChi[stopI] : maxClusterChi[stopI])) {
#ifdef DEBUGTRACKING
      cout << "chi-squared " << chi << " max " << (typ1 == 0 ? maxChi[stopI] : maxClusterChi[stopI])
           << endl;
#endif
      return false;
    }
    point3D f(line1.Foot());
    vector3D d(line1.Direction()); // modFactor);
    tr->setPath(sLine3D(f, d));
    tr->setChiSquared(chi);
  } else {
    tr->setPath(path);
    tr->setChiSquared(0);
  }
  return true;
}
void addPixAndElTotrack(TBase* obj, TTrack* tr)
{
  if (obj == NULL)
    return;
  if (obj->name() == "TPixel") {
    TPixel* p = (TPixel*)obj;
    tr->addPixel(p);
    for (int k = 0; k < p->getNumberOfActualElements(); k++)
      tr->addCalibSrc(&p->getElementr(k));
  } else if (obj->name() == "TCluster") {
    TCluster* c = (TCluster*)obj;
    for (int j = 0; j < c->getNumberOfPixels(); j++) {
      tr->addPixel(&c->getPixelr(j));
      for (int k = 0; k < c->getPixelr(j).getNumberOfActualElements(); k++)
        tr->addCalibSrc(&c->getPixelr(j).getElementr(k));
    }
  }
}
bool AVLineTrackSearch::trackAPlane(int planenum, int id[2])
{

#ifdef DEBUGTRACKING
  cout << "trackAPlane(plane number " << planenum << ", ids={" << id[0] << "," << id[1] << "})"
       << endl;
  cout << "points" << numPlanePoints[planenum] << endl;
#endif
  if (numPlanePoints[planenum] < 4) {
#ifdef DEBUGINFO
    cout << "trackAPlane(" << planenum << ") not enough points " << numPlanePoints[planenum]
         << endl;
#endif
    return false;
  }
  if (planePoints[planenum][0] == NULL || planePoints[planenum][1] == NULL)
    return false;
  for (int i = 0; i < numPlanePoints[planenum]; i++)
    if (!(planePoints[planenum][i]->name() == "TPixel" ||
          planePoints[planenum][i]->name() == "TCluster"))
      return false;
  point3D points[numPlanePoints[planenum]];
  for (int i = 0; i < numPlanePoints[planenum]; i++)
    if (planePoints[planenum][i]->name() == "TPixel")
      points[i] = ((TPixel*)planePoints[planenum][i])->getShape()->getCenter();
    else
      points[i] = ((TCluster*)planePoints[planenum][i])->getCenter();

  int numPlaneTracks[2] = {0, 0};
  int typ[2] = {(planePoints[planenum][0]->name() == "TPixel" ? 0 : 1),
                (planePoints[planenum][1]->name() == "TPixel" ? 0 : 1)};
#ifdef DEBUGTRACKING
  cout << "typ {" << typ[0] << "," << typ[1] << "}" << endl;
#endif
  int nPixels[2] = {1, 1};
  TPixel** pixs[2];
  for (int j = 0; j < 2; j++) {
    if (typ[j] == 1)
      nPixels[j] = ((TCluster*)planePoints[planenum][j])->getNumberOfPixels();
    pixs[j] = new TPixel*[nPixels[j]];
    if (typ[j] == 1)
      for (int i = 0; i < nPixels[j]; i++)
        pixs[j][i] = &((TCluster*)planePoints[planenum][j])->getPixelr(i);
    else
      pixs[j][0] = (TPixel*)planePoints[planenum][j];
  }
#ifdef DEBUGINFO
  cout << "possible Tracks" << endl;
  for (int l = 0; l < 2; l++)
    for (int i = 2; i < numPlanePoints[planenum]; i++) {
      cout << points[i] << points[l] << (points[l] - points[i]).toString(_spheric_).data() << endl;
      cout << sLine3D(points[i], points[l] - points[i]) /*.toString(_spheric_*4).data()*/
           << endl;
    }
#endif
  // Idea: fix stop-point and try tracks with any combination of one stop and
  // one intermediate point
  // works well for quirl and ring <== fixed geometry, but not so well for
  // barrel
  // solution try also any combination of intermediate pix, and check, that
  // pixel-shape is hit
  //  int ID=0;
  for (int l = 0; l < 2; l++) {
//      if(typ[l]==1)
//	ID=((TCluster*)planePoints[planenum][l])->getID();
//      else
//	ID=((TPixel*)planePoints[planenum][l])->getID();
#ifdef DEBUGINFO
    cout << "bias " << l << endl;
#endif
    numPlaneTracks[l] = 0;
    for (int i = 2; i < numPlanePoints[planenum]; i++) {
      if (numPlaneTracks[l] >= maxPlanes)
        continue;
      pointTracks[l][numPlaneTracks[l]]->reset();
#ifdef DEBUGINFO
      cout << "Path: " << points[i] << (points[l] - points[i]).toString(_spheric_).data() << endl;
      cout << "add pixels " << nPixels[l] << endl;
      for (int j = 0; j < nPixels[l]; j++) {
        cout << pixs[l][j] << endl;
        cout << numPlaneTracks[l] << endl;
        cout << pointTracks[l][numPlaneTracks[l]] << endl;
        cout << "add hits of pixel" << endl;
      }
      cout << "add pixels of " << i << endl;
#endif
      addPixAndElTotrack(planePoints[planenum][l], pointTracks[l][numPlaneTracks[l]]);
      addPixAndElTotrack(planePoints[planenum][i], pointTracks[l][numPlaneTracks[l]]);
      pointTracks[l][numPlaneTracks[l]]->setPath(sLine3D(points[i], points[l] - points[i]));
#ifdef DEBUGTRACKING
      cout << "search track for: "
           << pointTracks[l][numPlaneTracks[l]]->getPath().toString(_spheric_ * 4).data() << endl;
#endif
      if (searchATrack(numPlaneTracks[l], l, id[l], planePoints[planenum][l],
                       planePoints[planenum][i])) {
#ifdef DEBUGTRACKING
        cout << "taken: "
             << pointTracks[l][numPlaneTracks[l]]->getPath().toString(_spheric_ * 4).data() << endl;
#endif
        numPlaneTracks[l]++;
      }
      for (int j = i + 1; j < numPlanePoints[planenum]; j++) {
        if (numPlaneTracks[l] >= maxPlanes)
          continue;
        pointTracks[l][numPlaneTracks[l]]->reset();
#ifdef DEBUGINFO
        cout << "Path: " << points[i] << (points[l] - points[i]).toString(_spheric_).data() << endl;
        cout << "add pixels " << nPixels[l] << endl;
        for (int jj = 0; jj < nPixels[l]; jj++) {
          cout << pixs[l][jj] << endl;
          cout << numPlaneTracks[l] << endl;
          cout << pointTracks[l][numPlaneTracks[l]] << endl;
          cout << "add hits of pixel" << endl;
        }
        cout << "add pixels of " << i << endl;
#endif
        pointTracks[l][numPlaneTracks[l]]->setPath(sLine3D(points[i], points[j] - points[i]));
        vector3D dist;
        if (typ[l] == 0)
          dist = ((TPixel*)planePoints[planenum][l])
                     ->getElementr(0)
                     .getHitShape()
                     ->distance(pointTracks[l][numPlaneTracks[l]]->getPath());
        else
          dist = pointTracks[l][numPlaneTracks[l]]->getPath() -
                 ((TCluster*)planePoints[planenum][l])->getCenter();
        if (dist.R() > 10)
          continue;
        addPixAndElTotrack(planePoints[planenum][l], pointTracks[l][numPlaneTracks[l]]);
        addPixAndElTotrack(planePoints[planenum][i], pointTracks[l][numPlaneTracks[l]]);
        addPixAndElTotrack(planePoints[planenum][j], pointTracks[l][numPlaneTracks[l]]);
#ifdef DEBUGTRACKING
        cout << "search track for: "
             << pointTracks[l][numPlaneTracks[l]]->getPath().toString(_spheric_ * 4).data() << endl;
#endif
        if (searchATrack(numPlaneTracks[l], l, id[l], planePoints[planenum][l],
                         planePoints[planenum][i])) {
#ifdef DEBUGTRACKING
          cout << "taken: "
               << pointTracks[l][numPlaneTracks[l]]->getPath().toString(_spheric_ * 4).data()
               << endl;
#endif
          numPlaneTracks[l]++;
        }
      }
    }

#ifdef DEBUGTRACKING
    cout << "number of tracks on plane to point " << l << ":" << numPlaneTracks[l] << endl;
#endif
    if (numPlaneTracks[l] < 1) {
#ifdef DEBUGINFO
      cout << "trackAPlane(" << planenum << ") not enough tracks found for point " << l << "("
           << numPlanePoints[planenum] - 2 << ")" << endl;
#endif
      delete[] pixs[0];
      delete[] pixs[1];
      return false;
    }
  }
  point3D vertex(0, 0, 0);
  vector3D d1, d2, d3, dist;
#ifdef DEBUGTRACKING
  cout << "number of tracks on plane to point " << 0 << ":" << numPlaneTracks[0] << endl;
  cout << "number of tracks on plane to point " << 1 << ":" << numPlaneTracks[1] << endl;
#endif
  numVees = 0;
  float zDist;
  bool inDists;
#ifdef DEBUGINFO
  int nD = numPlaneTracks[0] * numPlaneTracks[1];
  float chivees[nD];
#endif
  for (int i = 0; i < numPlaneTracks[0]; i++)
    for (int j = 0; j < numPlaneTracks[1]; j++) {
#ifdef DEBUGTRACKING
      cout << "combine tracks " << i << " and " << j << endl;
      cout << "# hits " << pointTracks[0][i]->getNumberOfCalibHits() << " "
           << pointTracks[1][j]->getNumberOfCalibHits() << " minimum " << minElementsPerVee << endl;
#endif
#ifdef DEBUGINFO
      chivees[j + i * numPlaneTracks[1]] = -1;
#endif
      if (numVees >= maxPlanes - 1)
        continue;
      if (pointTracks[0][i]->getNumberOfCalibHits() + pointTracks[1][j]->getNumberOfCalibHits() <
          minElementsPerVee)
        continue;
      int nInCommon = 0;
      for (int iii = 10; iii < 26; iii++)
        for (int jjj = 0; jjj < pointTracks[0][i]->getNumberOfCalibHits(iii); jjj++)
          for (int kkk = 0; kkk < pointTracks[1][j]->getNumberOfCalibHits(iii); kkk++) {
            if (pointTracks[0][i]->getCalibSrcr(iii, jjj)->getElement() ==
                pointTracks[1][j]->getCalibSrcr(iii, kkk)->getElement())
              nInCommon++;
          }
      if (nInCommon > 2)
        continue;
      if (&pointTracks[0][i]->getPixel(0, 0) == &pointTracks[1][j]->getPixel(0, 0) ||
          &pointTracks[0][i]->getPixel(1, 0) == &pointTracks[1][j]->getPixel(1, 0))
        continue;
      vee[numVees]->setTracks(pointTracks[0][i], pointTracks[1][j]);
      zDist = (vee[numVees]->getVertex() - vertex).Z();
#ifdef DEBUGINFO
      chivees[j + i * numPlaneTracks[1]] = vee[numVees]->getChiSquared();
#endif
#ifdef DEBUGTRACKING
      cout << "chi-squared " << vee[numVees]->getChiSquared() << " max " << maxVertexChi << endl;
      cout << "vertex " << vee[numVees]->getVertex() << endl;
      cout << "dir1   " << vee[numVees]->getDirection(0).toString(_spheric_).data() << endl;
      cout << "dir2   " << vee[numVees]->getDirection(1).toString(_spheric_).data() << endl;
#endif
      if (vee[numVees]->getChiSquared() > maxVertexChi)
        continue;
      inDists = false;
      for (int i = 0; i < numZDists; i++)
        if (Zdists[i * 2] <= zDist && Zdists[i * 2 + 1] >= zDist)
          inDists = true;
      if (!inDists)
        continue;
      d1 = vee[numVees]->getDirection(0);
      d2 = vee[numVees]->getDirection(1);
      d3 = vee[numVees]->getVertex() - vertex;
      dist = plane[planenum] & vee[numVees]->getVertex();
      d3.normalize();
      d1.normalize();
      d2.normalize();
#ifdef DEBUGTRACKING
      cout << "distance vertex-plane " << dist.R() << " max " << maxDistanceVertexPlane << endl;
      cout << "angle " << abs(acos(d1 * d3) + acos(d2 * d3) - acos(d1 * d2)) << " max " << maxAngle
           << endl;
#endif
      if (dist.R() > maxDistanceVertexPlane)
        continue;
      if ((abs(acos(d1 * d3) + acos(d2 * d3) - acos(d1 * d2)) > maxAngle))
        continue;
      numVees++;
    }
#ifdef DEBUGTRACKING
  cout << "number of vees found :" << numVees << endl;
#endif
  if (numVees <= 0) {
#ifdef DEBUGINFO
    cout << "trackAPlane(" << planenum << ") no vees found" << endl;
    cout << "  out of " << numPlaneTracks[0] << "*" << numPlaneTracks[1] << " combinations" << endl;
    cout << "  chi-squareds(" << flush;
    for (int i = 0; i < nD - 1; i++)
      cout << chivees[i] << ", ";
    cout << chivees[nD - 1] << ")" << endl;
#endif
    delete[] pixs[0];
    delete[] pixs[1];
    return false;
  }
  int best = 0;
  sLine3D path1, path2;
  for (int i = 1; i < numVees; i++) // sort by chi squared| no take just the best
  {
    if (vee[best]->getChiSquared() > vee[i]->getChiSquared())
      best = i;
  }
#ifdef DEBUGINFO
  cout << "out of " << nD << " combinations:" << endl;
  cout << "chi-squared " << vee[best]->getChiSquared() << " max " << maxVertexChi << endl;
  cout << "vertex " << vee[best]->getVertex() << endl;
  cout << "dir1   " << vee[best]->getDirection(0).toString(_spheric_).data() << endl;
  cout << "dir2   " << vee[best]->getDirection(1).toString(_spheric_).data() << endl;
#endif
  point3D pp = vee[best]->getVertex();
  vector3D vv1 = vee[best]->getDirection(0);
  vector3D vv2 = vee[best]->getDirection(1);
  path1 = sLine3D(pp, vv1);
  path2 = sLine3D(pp, vv2);
  *(planeTrack[0][planenum]) = *(vee[best]->getTrack(0));
  *(planeTrack[1][planenum]) = *(vee[best]->getTrack(1));
  planeTrack[0][planenum]->setPath(path1);
  planeTrack[1][planenum]->setPath(path2);
  planeTrack[0][planenum]->setChiSquared(vee[best]->getChiSquared());
  planeTrack[1][planenum]->setChiSquared(vee[best]->getChiSquared());
  delete[] pixs[0];
  delete[] pixs[1];
  return true;
}
bool AVLineTrackSearch::assignPointsToPlane(int planenum)
{
#ifdef DEBUGPLANING
  cout << "assignPointsToPlane(plane number:" << planenum << ")" << endl;
#endif
#ifdef DEBUGINFO
  int npix = 0, nclu = 0;
#endif
  vector3D dist;
  vector3D radius;
  float factor;
  planeShape* shape;
  int n;
  for (int i = 0; i < numStart; i++) // search all start pixel types
  {
    n = 0;
#ifdef DEBUGPLANING
    cout << "pixID " << startIDs[i] << " " << *(numberOfPixels[startIDs[i]]) << endl;
#endif
    for (int j = 0; j < *(numberOfPixels[startIDs[i]]); j++) // search all pixels of that type
    {
      shape = pixels[startIDs[i]][j]->getShape();
      if (shape == NULL)
        continue;
      if (checkWithPrompt(pixels[startIDs[i]][j]) > maxPixelPrompt)
        continue;
      // dist=shape->distancePlane(plane[planenum]);
      dist = plane[planenum] & shape->getCenter();
      radius = shape->getNormal();
      factor =
          (radius * dist) / dist.R() / radius.R(); // cos(angle_between_normalOfShape_and_distance)
      factor = sqrt(1 - factor * factor) * shape->circumscribedRadius();
#ifdef DEBUGPLANING
      cout << "distance " << dist.R() /*-factor*/
           << " maximum:" << maxDistanceToPlane << endl;
#endif
      if (dist.R() > maxDistanceToPlane /*+factor*/)
        continue;
      planePoints[planenum][numPlanePoints[planenum]] = pixels[startIDs[i]][j];
      numPlanePoints[planenum]++;
      n++;
#ifdef DEBUGPLANING
      cout << "pixel added " << endl;
#endif
    }
    if (n < minStarts[i])
      return false;
  }
#ifdef DEBUGINFO
  npix = numPlanePoints[planenum];
#endif
  for (int i = 0; i < numStartCluster; i++) // search all start cluster types
  {
    n = 0;
#ifdef DEBUGPLANING
    cout << "clusterID " << startClusterIDs[i] << " " << *(numberOfClusters[startClusterIDs[i]])
         << endl;
#endif
    for (int j = 0; j < *(numberOfClusters[startClusterIDs[i]]);
         j++) // search all clusters of that type
    {
      if (checkWithPrompt(clusters[startClusterIDs[i]][j]) > maxPixelPrompt)
        continue;
      dist = plane[planenum] & clusters[startClusterIDs[i]][j]->getCenter();
      radius = clusters[startClusterIDs[i]][j]->getClusterRadius();
      factor = (radius * dist) / dist.R() / radius.R();
      factor = sqrt(1 - factor * factor) * radius.R();
#ifdef DEBUGPLANING
      cout << "distance " << dist.R() - factor << " maximum:" << maxDistanceToPlane << endl;
#endif
      if (dist.R() > maxDistanceToPlane /*+factor*/)
        continue;
      planePoints[planenum][numPlanePoints[planenum]] = clusters[startClusterIDs[i]][j];
      numPlanePoints[planenum]++;
      n++;
#ifdef DEBUGPLANING
      cout << "cluster added" << endl;
#endif
    }
    if (n < minStarts[i + numStart])
      return false;
  }
#ifdef DEBUGPLANING
  cout << "points found: " << numPlanePoints[planenum] << endl;
#endif
  if (numPlanePoints[planenum] >= 4)
    return true;
#ifdef DEBUGINFO
  nclu = numPlanePoints[planenum] - npix;
  cout << "assignPointsToPlane() " << numPlanePoints[planenum] << " points found (" << npix << ","
       << nclu << ")" << endl;
#endif
  return false;
}
bool AVLineTrackSearch::checkPlane(int planenum, int l, int k)
{
  if (numPlanePoints[planenum] < 4)
    return false;
  int counter1 = 0, counter2 = 0;
// count double elements in end_points
#ifdef DEBUGCHECK
  cout << "checkPlane() count double elements in end points" << endl;
#endif
  switch (l * 2 + k) {
  case 0: {
    TPixel *pix1 = (TPixel*)planePoints[planenum][0], *pix2 = (TPixel*)planePoints[planenum][1];
    for (int i = 0; i < pix1->getNumberOfActualElements(); i++)
      for (int j = 0; j < pix2->getNumberOfActualElements(); j++)
        if (&pix1->getElementr(i) == &pix2->getElementr(j))
          counter1++;
    break;
  }
  case 1: {
    TPixel* pix1 = (TPixel*)planePoints[planenum][0];
    TCluster* pix2 = (TCluster*)planePoints[planenum][1];
    for (int i = 0; i < pix1->getNumberOfActualElements(); i++)
      for (int jj = 0; jj < pix2->getNumberOfPixels(); jj++)
        for (int j = 0; j < pix2->getPixelr(jj).getNumberOfActualElements(); j++)
          if (&pix1->getElementr(i) == &pix2->getPixelr(jj).getElementr(j))
            counter1++;
    break;
  }
  case 2: {
    TPixel* pix1 = (TPixel*)planePoints[planenum][1];
    TCluster* pix2 = (TCluster*)planePoints[planenum][0];
    for (int i = 0; i < pix1->getNumberOfActualElements(); i++)
      for (int jj = 0; jj < pix2->getNumberOfPixels(); jj++)
        for (int j = 0; j < pix2->getPixelr(jj).getNumberOfActualElements(); j++)
          if (&pix1->getElementr(i) == &pix2->getPixelr(jj).getElementr(j))
            counter1++;
    break;
  }
  case 3: {
    TCluster* pix1 = (TCluster*)planePoints[planenum][0];
    TCluster* pix2 = (TCluster*)planePoints[planenum][1];
    for (int ii = 0; ii < pix1->getNumberOfPixels(); ii++)
      for (int i = 0; i < pix1->getPixelr(ii).getNumberOfActualElements(); i++)
        for (int jj = 0; jj < pix2->getNumberOfPixels(); jj++)
          for (int j = 0; j < pix2->getPixelr(jj).getNumberOfActualElements(); j++)
            if (&pix1->getPixelr(ii).getElementr(i) == &pix2->getPixelr(jj).getElementr(j))
              counter1++;
    break;
  }
  }
#ifdef DEBUGCHECK
  cout << "counter1 " << counter1 << endl;
  cout << "count double elements with prompt tracks" << endl;
#endif
  // count double elements with prompt tracks
  int det;
  bool pixfound;
  if (l == 0) {
    TPixel* pix = (TPixel*)planePoints[planenum][0];
    for (int i = 0; i < numberOfTracks; i++) {
      pixfound = false;
      for (int j = 0; j < tracks[i]->getNumberOfPixels(); j++)
        if (&tracks[i]->getPixel(0, j) == pix)
          pixfound = true;
      if (pixfound)
        continue;
      for (int j = 0; j < pix->getNumberOfActualElements(); j++) {
        det = pix->getElementr(j).getDetector();
        for (int ii = 0; ii < tracks[i]->getNumberOfCalibHits(det); ii++) {
          if (tracks[i]->getCalibSrcr(det, ii) == &pix->getElementr(j)) {
#ifdef DEBUGINFO
            cout << "double element found: Track " << i << " det:" << det
                 << " el: " << pix->getElementr(j).getElement() << " - "
                 << tracks[i]->getCalibSrcr(det, ii)->getElement() << " " << &pix->getElementr(j)
                 << " " << tracks[i]->getCalibSrcr(det, ii) << endl;
#endif
            counter2++;
          }
        }
      }
    }
  } else {
    TCluster* pix = (TCluster*)planePoints[planenum][0];
    for (int i = 0; i < numberOfTracks; i++) {
      pixfound = false;
      for (int j = 0; j < pix->getNumberOfPixels(); j++) {
        for (int jj = 0; jj < tracks[i]->getNumberOfPixels(); jj++)
          if (&tracks[i]->getPixel(0, jj) == &pix->getPixelr(j))
            pixfound = true;
      }
      if (pixfound)
        continue;
      for (int j = 0; j < pix->getNumberOfPixels(); j++)
        for (int jj = 0; jj < pix->getPixelr(j).getNumberOfActualElements(); jj++)
          for (int ii = 0; ii < tracks[i]->getNumberOfCalibHits(
                                    pix->getPixelr(j).getElementr(jj).getDetector());
               ii++) {
            if (tracks[i]->getCalibSrcr(pix->getPixelr(j).getElementr(jj).getDetector(), ii) ==
                &pix->getPixelr(j).getElementr(jj))
              counter2++;
          }
    }
  }
  if (k == 0) {
    TPixel* pix = (TPixel*)planePoints[planenum][1];
    for (int i = 0; i < numberOfTracks; i++) {
      pixfound = false;
      for (int j = 0; j < tracks[i]->getNumberOfPixels(); j++)
        if (&tracks[i]->getPixel(0, j) == pix)
          pixfound = true;
      if (pixfound)
        continue;
      for (int j = 0; j < pix->getNumberOfActualElements(); j++) {
        det = pix->getElementr(j).getDetector();
        for (int ii = 0; ii < tracks[i]->getNumberOfCalibHits(pix->getElementr(j).getDetector());
             ii++)
          if (tracks[i]->getCalibSrcr(pix->getElementr(j).getDetector(), ii) ==
              &pix->getElementr(j)) {
#ifdef DEBUGINFO
            cout << "double element found: Track " << i << " det:" << det
                 << " el: " << pix->getElementr(j).getElement() << " - "
                 << tracks[i]->getCalibSrcr(det, ii)->getElement() << " " << &pix->getElementr(j)
                 << " " << tracks[i]->getCalibSrcr(det, ii) << endl;
#endif
            counter2++;
          }
      }
    }
  } else {
    TCluster* pix = (TCluster*)planePoints[planenum][1];
    for (int i = 0; i < numberOfTracks; i++) {
      pixfound = false;
      for (int j = 0; j < pix->getNumberOfPixels(); j++) {
        for (int jj = 0; jj < tracks[i]->getNumberOfPixels(); jj++)
          if (&tracks[i]->getPixel(0, jj) == &pix->getPixelr(j))
            pixfound = true;
      }
      if (pixfound)
        continue;
      for (int j = 0; j < pix->getNumberOfPixels(); j++)
        for (int jj = 0; jj < pix->getPixelr(j).getNumberOfActualElements(); jj++)
          for (int ii = 0; ii < tracks[i]->getNumberOfCalibHits(
                                    pix->getPixelr(j).getElementr(jj).getDetector());
               ii++) {
            if (tracks[i]->getCalibSrcr(pix->getPixelr(j).getElementr(jj).getDetector(), ii) ==
                &pix->getPixelr(j).getElementr(jj))
              counter2++;
          }
    }
  }
#ifdef DEBUGCHECK
  cout << "counter2 " << counter2 << endl;
  cout << "maximum: " << maxEqualElements << endl;
#endif
  if (counter1 <= maxEqualElements && counter2 <= maxEqualElements)
    return true;
#ifdef DEBUGINFO
  cout << "checkPlane() counter1: " << counter1 << "(" << maxEqualElements
       << "), counter2: " << counter2 << "(" << maxEqualElements << ")" << endl;
#endif
  return false;
}
bool AVLineTrackSearch::checkWithPrompt(TBase* p1, TBase* p2, int i, int j)
{
#ifdef DEBUGCHECK
  cout << "checkWithPrompt(): numberOfPrompt: " << numberOfTracks << endl;
  cout << "point1:" << p1 << " point2:" << p2 << endl;
  cout << "t1    :" << i << " t2    :" << j << endl;
  cout << "t1_c  :" << (p1->name() == "TPixel" ? 0 : (p1->name() == "TCluster" ? 1 : 2))
       << " t2_c  :" << (p2->name() == "TPixel" ? 0 : (p2->name() == "TCluster" ? 1 : 2)) << endl;
#endif
  if (p1 == NULL || p2 == NULL)
    return false;
  int used1 = 0, used2 = 0;
  if (i == 0) {
    for (int ll = 0; ll < numberOfTracks; ll++) {
      for (int lll = 0; lll < tracks[ll]->getNumberOfPixels(); lll++)
        if ((TPixel*)p1 == &tracks[ll]->getPixel(0, lll))
          used1++;
    }
  } else {
    for (int ll = 0; ll < numberOfTracks; ll++) {
      for (int lll = 0; lll < tracks[ll]->getNumberOfPixels(); lll++) {
        for (int llll = 0; llll < ((TCluster*)p1)->getNumberOfPixels(); llll++)
          if (&((TCluster*)p1)->getPixelr(llll) == &tracks[ll]->getPixel(0, lll))
            used1++;
      }
    }
  }
  if (j == 0) {
    for (int ll = 0; ll < numberOfTracks; ll++) {
      for (int lll = 0; lll < tracks[ll]->getNumberOfPixels(); lll++)
        if ((TPixel*)p2 == &tracks[ll]->getPixel(0, lll))
          used2++;
    }
  } else {
    for (int ll = 0; ll < numberOfTracks; ll++) {
      for (int lll = 0; lll < tracks[ll]->getNumberOfPixels(); lll++) {
        for (int llll = 0; llll < ((TCluster*)p2)->getNumberOfPixels(); llll++)
          if (&((TCluster*)p2)->getPixelr(llll) == &tracks[ll]->getPixel(0, lll))
            used2++;
      }
    }
  }
#ifdef DEBUGCHECK
  cout << "used1:" << used1 << " used2:" << used2 << endl;
#endif
  if (used1 > 0 || used2 > 0) {
#ifdef DEBUGINFO
    cout << "checkWithPrompt() used1:" << used1 << " used2:" << used2 << endl;
#endif
    return false;
  }
  return true;
}
bool AVLineTrackSearch::checkWithPrompt(TCalibHit* hit)
{
  if (hit == NULL)
    return false;
  for (int i = 0; i < numberOfTracks; i++)
    for (int k = 0; k < tracks[i]->getNumberOfCalibHits(hit->getDetector()); k++)
      if (hit->getElement() == tracks[i]->getCalibSrcr(hit->getDetector(), k)->getElement())
        return false;
  return true;
}
int AVLineTrackSearch::checkWithPrompt(TPixel* pix)
{
  if (pix == NULL)
    return 0;
  int nEls = 0;
  bool found;
  for (int i = 0; i < pix->getNumberOfActualElements(); i++) {
    found = false;
    for (int j = 0; j < numberOfTracks; j++)
      for (int k = 0; k < tracks[j]->getNumberOfCalibHits(pix->getElementr(i).getDetector()); k++)
        if (tracks[j]->getCalibSrcr(pix->getElementr(i).getDetector(), k)->getElement() ==
            pix->getElementr(i).getElement())
          found = true;
    if (found)
      nEls++;
  }
  return nEls;
}
int AVLineTrackSearch::checkWithPrompt(TCluster* clu)
{
  if (clu == NULL)
    return 0;
  int nEls = 0;
  bool found;
  for (int l = 0; l < clu->getNumberOfPixels(); l++)

    for (int i = 0; i < clu->getPixelr(l).getNumberOfActualElements(); i++) {
      found = false;
      for (int j = 0; j < numberOfTracks; j++)
        for (int k = 0;
             k < tracks[j]->getNumberOfCalibHits(clu->getPixelr(l).getElementr(i).getDetector());
             k++)
          if (tracks[j]
                  ->getCalibSrcr(clu->getPixelr(l).getElementr(i).getDetector(), k)
                  ->getElement() == clu->getPixelr(l).getElementr(i).getElement())
            found = true;
      if (found)
        nEls++;
    }
  return nEls;
}
// const unsigned int MAX=1000;
// int id[MAX];
// int pID[MAX];
// point3D pts[MAX];
// TBase *pto[MAX];
void* AVLineTrackSearch::process(void* ptr)
{
  /**********************************************************
   *
   * for any two clusters or pixels
   *  define a decay plane: Point1-Target-Point2
   *  search pixels/clusters close to that plane
   *  for any combination of end_pixel-mid_pixel
   *   do tracking
   *
   * don't use a end_point combination if
   *  both pixels are used by prompt tracks
   *
   * Plane is accepted if more than two mid_pixel are found,
   *  the number of elements equal in both end_points
   *   and equal with prompt tracks is lower than maxEqualElements,
   *  for each end_pixel at least one track is found,
   *  the number of elements on a two track combination is enough,
   *  not both end_point are used in prompt tracks,
   *  the end_points are from target more distant than 0.05rad
   *  and the result of the two-track-fit is:
   *   the decay-vertex is close enough to the plane
   *   chi-squared is lower than a maximum
   *   the direction target-vertex is close to:
   *    d_vert = a * d_1 + b * d_2 with a,b>0
   *
   **********************************************************/
  for (int i = 0; i < nDet; i++) {
    nEl[i] = 0;
    for (int j = 0; j < (*(numberOfHits[i])); j++) {
      if (!checkForPromptElements) {
        chels[i][nEl[i]] = hits[i][j];
        nEl[i]++;
      } else if (checkWithPrompt(hits[i][j])) {
        chels[i][nEl[i]] = hits[i][j];
        nEl[i]++;
      }
    }
  }
#ifdef DEBUGINFO
  cout << "AVLineTrackSearch::process()" << endl;
#endif
  int outcode = 0;
  int MAX = 1;
  for (int i = 0; i < numStop; i++)
    MAX = MAX + (*(numberOfPixels[stopIDs[i]]));
  for (int i = 0; i < numStopCluster; i++)
    MAX = MAX + (*(numberOfClusters[stopClusterIDs[i]]));
  int id[MAX];
  int pID[MAX];
  //   point3D pts[MAX];
  //   TBase *pto[MAX];

  int nPoints = 0;
  int tmpNum = 0;
  // put all points of stop into common array
  for (int i = 0; i < numStop; i++) {
#ifdef DEBUGINFO
    cout << stopIDs[i] << ":" << (*(numberOfPixels[stopIDs[i]])) << endl;
#endif
    for (int j = 0; j < (*(numberOfPixels[stopIDs[i]])); j++) {
      if (pixels[stopIDs[i]][j]->getShape() == NULL)
        continue;
      if (checkWithPrompt(pixels[stopIDs[i]][j]) > maxPixelPrompt)
        continue;
      pto[nPoints] = pixels[stopIDs[i]][j];
      pID[nPoints] = stopIDs[i];
      id[nPoints] = 0;
      pts[nPoints] = pixels[stopIDs[i]][j]->getShape()->getCenter();
      if (nPoints >= (int)MAX - 1)
        continue;
      nPoints++;
    }
  }
  for (int i = 0; i < numStopCluster; i++) {
#ifdef DEBUGINFO
    cout << stopClusterIDs[i] << ":" << (*(numberOfClusters[stopClusterIDs[i]])) << endl;
#endif
    for (int j = 0; j < (*(numberOfClusters[stopClusterIDs[i]])); j++) {
      if (checkWithPrompt(clusters[stopClusterIDs[i]][j]) > maxPixelPrompt)
        continue;
      pto[nPoints] = clusters[stopClusterIDs[i]][j];
      id[nPoints] = 1;
      pID[nPoints] = stopClusterIDs[i];
      pts[nPoints] = clusters[stopClusterIDs[i]][j]->getCenter();
      if (nPoints >= (int)MAX - 1)
        continue;
      nPoints++;
    }
  }
  int IDs[2];
  point3D vertex(0, 0, 0), p1, p2;
  vector3D v1, v2;
  if (doAlternateSearch) {
    nStopP = nPoints;
    tmpNum = alternatePlaneSearch();
  } else {
#ifdef DEBUGINFO
    cout << "start " << nPoints << endl;
#endif
    for (int i = 0; i < nPoints - 1; i++) {
#ifdef DEBUGINFO
      cout << " " << i << " " << nPoints << endl;
#endif
      for (int j = i + 1; j < nPoints; j++) {
#ifdef DEBUGINFO
        cout << "  " << i << " " << j << " " << nPoints << endl;
#endif
        if (tmpNum >= maxPlanes - 1)
          continue;
        if (!checkWithPrompt(pto[i], pto[j], id[i], id[j]))
          continue;
        if (outcode < 1)
          outcode = 1;
        numPlanePoints[tmpNum] = 2;
        planePoints[tmpNum][0] = pto[i];
        planePoints[tmpNum][1] = pto[j];
        v1 = pts[i] - vertex;
        v2 = pts[j] - vertex;
#ifdef DEBUGINFO
// cout<<"point2:"<<p2<<endl;
// cout<<"angle of plane:"<<acos((v1*v2)/v1.R()/v2.R())<<" "<<acos(.9987)<<endl;
#endif
        if ((v1 * v2) / v1.R() / v2.R() > .9987)
          continue;
        plane[tmpNum] = plane3D(vertex, v1, v2);
        if (!assignPointsToPlane(tmpNum)) {
#ifdef DEBUGINFO
          cout << "assignPointsToPlane() not successful " << numPlanePoints[tmpNum] << endl;
          for (int lll = 2; lll < numPlanePoints[tmpNum]; lll++)
            if (planePoints[tmpNum][lll] != NULL) {
              if (planePoints[tmpNum][lll]->name() == "TPixel")
                cout << ((TPixel*)planePoints[tmpNum][lll])->getID() << "\t" << flush;
              else if (planePoints[tmpNum][lll]->name() == "TCluster")
                cout << ((TCluster*)planePoints[tmpNum][lll])->getID() << "\t" << flush;
            }
          cout << endl;
#endif
          continue;
        }
        if (outcode < 2)
          outcode = 2;
        if (!checkPlane(tmpNum, id[i], id[j])) {
#ifdef DEBUGINFO
          cout << "checkPlane() not successful " << endl;
#endif
          continue;
        }
        if (outcode < 3)
          outcode = 3;
        IDs[0] = pID[i];
        IDs[1] = pID[j];
        if (!trackAPlane(tmpNum, IDs))
          continue;
        if (outcode < 4)
          outcode = 4;
        tmpNum++;
      } // for(int jj=0;jj<*(nPoints2[pointIDs2[j]]);jj++)
    }
  }
#ifdef DEBUGINFO
  cout << "num planes " << tmpNum << endl;
  cout << "prompt tracks:" << numberOfTracks << endl;
  for (int i = 0; i < numberOfTracks; i++)
    cout << tracks[i]->getPath().toString(_spheric_ * 4).data() << endl;
  if (tmpNum <= 0)
    cout << "done" << endl;
  if (outcode > 2)
    cout << outcode << endl;
#endif
  if (tmpNum <= 0)
    return ptr;
  int position[tmpNum];
  int pos;
  int tmp;
  vector3D neutral, neut;
  for (int i = 0; i < tmpNum; i++)
    position[i] = i;
  // Sort by chi-squared
  for (int i = 0; i < tmpNum; i++) {
    pos = position[i];
    for (int j = i + 1; j < tmpNum; j++)
      if (planeTrack[0][position[pos]]->getChiSquared() >
          planeTrack[0][position[j]]->getChiSquared())
        pos = j;
    tmp = position[i];
    position[i] = position[pos];
    position[pos] = tmp;
  }
  // Delete planes to close
  for (int i = 0; i < tmpNum; i++)
    for (int j = i + 1; j < tmpNum; j++) {
#ifdef DEBUGINFO
// 	cout<<"vertex distance
// "<<(planeTrack[0][position[i]]->getPath().Foot()-planeTrack[0][position[j]]->getPath().Foot()).toString(_spheric_).data()<<endl;
// 	cout<<"Normal :
// "<<plane[position[i]].Normal()*plane[position[j]].Normal()<<"
// "<<plane[position[i]].Normal()<<plane[position[j]].Normal()<<endl;
// 	cout<<"0-0
// :"<<planeTrack[0][position[i]]->getPath().Direction()*planeTrack[0][position[j]]->getPath().Direction()<<endl;
// 	cout<<"1-1  :"<<planeTrack[1][position[i]]->getPath().Direction()*
// 	  planeTrack[1][position[j]]->getPath().Direction()<<endl;
// 	cout<<"0-1  :"<<planeTrack[0][position[i]]->getPath().Direction()*
// 	  planeTrack[1][position[j]]->getPath().Direction()<<endl;
// 	cout<<"1-0  :"<<planeTrack[1][position[i]]->getPath().Direction()*
// 	  planeTrack[0][position[j]]->getPath().Direction()<<endl;
#endif
      if (abs(plane[position[i]].Normal() * plane[position[j]].Normal()) > 0.9987 &&
          (((planeTrack[0][position[i]]->getPath().Direction() *
                 planeTrack[0][position[j]]->getPath().Direction() >
             0.9987) &&
            (planeTrack[1][position[i]]->getPath().Direction() *
                 planeTrack[1][position[j]]->getPath().Direction() >
             0.9987)) ||
           ((planeTrack[0][position[i]]->getPath().Direction() *
                 planeTrack[1][position[j]]->getPath().Direction() >
             0.9987) &&
            (planeTrack[1][position[i]]->getPath().Direction() *
                 planeTrack[0][position[j]]->getPath().Direction() >
             0.9987))))
      // 	   &&((planeTrack[0][position[i]]->getPath().Foot()-
      // 	       planeTrack[0][position[j]]->getPath().Foot()).R()<5))
      {
#ifdef DEBUGINFO
// cout<<"eliminate "<<j<<endl;
#endif
        for (int k = j; k < tmpNum - 1; k++)
          position[k] = position[k + 1];
        tmpNum--;
        j--;
      }
    }
#ifdef DEBUGINFO
  cout << "left " << tmpNum << endl;
  cout << numberOfTracks << " " << maxTracks << " " << maxTracks - numberOfTracks << " "
       << (maxTracks - numberOfTracks) / 3 << endl;
#endif
  if (tmpNum > (maxTracks - numberOfTracks) / 3)
    tmpNum = (maxTracks - numberOfTracks) / 3;
  // Sort by theta_neutral
  //   for(int i=0;i<tmpNum;i++)
  //     {
  //       pos=position[i];
  //       for(int j=i+1;j<tmpNum;j++)
  // 	if((vertex-planeTrack[0][position[pos]]->getPath().Foot()).Theta()>(vertex-planeTrack[0][position[j]]->getPath().Foot()).Theta())
  // 	  pos=j;
  //       tmp=position[i];
  //       position[i]=position[pos];
  //       position[pos]=tmp;
  //     }
  // copy tracks
  int nums = tmpNum;
  if (nums * 3 + numberOfTracks > maxTracks) {
    nums = maxTracks - numberOfTracks;
    nums = nums / 3;
  }
  vector3D direction;
  direction.setRep(_spheric_);
  // first all primary then the secondary tracks
  for (int i = 0; i < nums; i++) {
#ifdef DEBUGINFO
    cout << "plane " << i << ":" << position[i] << endl;
#endif
    planeTrack[0][position[i]]->setOrder(true);
    planeTrack[1][position[i]]->setOrder(true);
    planeTrack[0][position[i]]->setCharge(1);
    planeTrack[1][position[i]]->setCharge(1);
    direction = planeTrack[0][position[i]]->getPath().Foot() - vertex;
    direction.normalize();
#ifdef DEBUGINFO
    cout << "plane " << i << ":" << position[i] << " " << numberOfTracks + nums + i * 2 + 1 << " "
         << maxTracks << endl;
    cout << (tracks[numberOfTracks + nums + i * 2 + 0]) << " " << (planeTrack[0][position[i]])
         << " " << (tracks[numberOfTracks + nums + i * 2 + 1]) << " "
         << (planeTrack[1][position[i]]) << " " << flush
         << *(tracks[numberOfTracks + nums + i * 2 + 0]) << "\n " << *(planeTrack[0][position[i]])
         << "\n " << *(tracks[numberOfTracks + nums + i * 2 + 1]) << "\n "
         << *(planeTrack[1][position[i]]) << "\n " << endl;

#endif
    (*(tracks[numberOfTracks + nums + i * 2 + 0])) = (*(planeTrack[0][position[i]]));
    (*(tracks[numberOfTracks + nums + i * 2 + 1])) = (*(planeTrack[1][position[i]]));
    tracks[numberOfTracks + nums + i * 2 + 0]->setPrimary(tracks[numberOfTracks + i]);
    tracks[numberOfTracks + nums + i * 2 + 1]->setPrimary(tracks[numberOfTracks + i]);
    tracks[numberOfTracks + nums + i * 2 + 0]->setOrder(true);
    tracks[numberOfTracks + nums + i * 2 + 1]->setOrder(true);
    tracks[numberOfTracks + i]->reset();
    tracks[numberOfTracks + i]->setOrder(false);
    tracks[numberOfTracks + i]->setCharge(0);
    tracks[numberOfTracks + i]->setPath(sLine3D(vertex, direction));
    tracks[numberOfTracks + i]->addSecondary(tracks[numberOfTracks + nums + i * 2 + 0]);
    tracks[numberOfTracks + i]->addSecondary(tracks[numberOfTracks + nums + i * 2 + 1]);
    tracks[numberOfTracks + nums + i * 2 + 0]->setErrorTheta(2 * M_PI / 180.);
    tracks[numberOfTracks + nums + i * 2 + 1]->setErrorTheta(2 * M_PI / 180.);
    tracks[numberOfTracks + nums + i * 2 + 0]->setErrorPhi(4 * M_PI / 180.);
    tracks[numberOfTracks + nums + i * 2 + 1]->setErrorPhi(4 * M_PI / 180.);
#ifdef DEBUGINFO
    cout << "plane " << i << ":" << position[i] << endl;
    cout << " chi-squared   :" << planeTrack[0][position[i]]->getChiSquared() << "\t"
         << " # Elements: " << planeTrack[0][position[i]]->getNumberOfCalibHits(-1) << " "
         << planeTrack[1][position[i]]->getNumberOfCalibHits(-1) << endl;
    cout << " neutral:" << direction << endl;
    cout << " vertex :" << planeTrack[0][position[i]]->getPath().Foot() << endl;
    cout << " first  :"
         << planeTrack[0][position[i]]->getPath().Direction().toString(_spheric_).data() << endl;
    cout << " second :"
         << planeTrack[1][position[i]]->getPath().Direction().toString(_spheric_).data() << endl;
#endif
  }
#ifdef DEBUGINFO
  cout << "done " << numberOfTracks << endl;
#endif
  numberOfTracks = numberOfTracks + nums * 3;
  return ptr;
}
int AVLineTrackSearch::alternatePlaneSearch()
{
  /***********************************************
Alternative plane search:
when point-projected from target, a plane containing
the target will be a straight line with stop-pixels at
eighter end:
define line between any 2 stopPixels
search for start pixels close to the line, if too less, skip line
sort the start points on the line, and check if the points
  are right in z-position.
search a track for both halves of line
do Vee-fit
   ***********************************************/
  //  cout<<"AVLineTrackSearch::alternatePlaneSearch()"<<endl;
  projector::setProjector(plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)), point3D(0, 0, 0),
                          projector::central);
  for (int i = 0; i < nStopP; i++)
    stopPoints[i] = projector::project(pts[i]);
  nStartP = 0;
  point3D s_pts[MAX];
  for (int i = 0; i < numStart; i++)
    for (int j = 0; j < *(numberOfPixels[startIDs[i]]); j++) {
      if (pixels[startIDs[i]][j]->getShape() == NULL)
        continue;
      if (checkWithPrompt(pixels[startIDs[i]][j]) > maxPixelPrompt)
        continue;
      s_pto[nStartP] = pixels[startIDs[i]][j];
      s_pts[nStartP] = pixels[startIDs[i]][j]->getShape()->getCenter();
      startPoints[nStartP] = projector::project(pixels[startIDs[i]][j]->getShape()->getCenter());
      s_id[nStartP] = i;
      zBuffer[nStartP] =
          pixels[startIDs[i]][j]->getShape()->getCenter().R(); /// startPoints[nStartP].R();
      nStartP++;
    }
  for (int i = 0; i < numStartCluster; i++)
    for (int j = 0; j < *(numberOfClusters[startClusterIDs[i]]); j++) {
      if (checkWithPrompt(clusters[startClusterIDs[i]][j]) > maxPixelPrompt)
        continue;
      s_pto[nStartP] = clusters[startClusterIDs[i]][j];
      s_pts[nStartP] = clusters[startClusterIDs[i]][j]->getCenter();
      startPoints[nStartP] = projector::project(clusters[startClusterIDs[i]][j]->getCenter());
      s_id[nStartP] = i + numStart;
      zBuffer[nStartP] =
          clusters[startClusterIDs[i]][j]->getCenter().R(); /// startPoints[nStartP].R();
      nStartP++;
    }
  int pop[100];
  int npop = 0;
  int nOn[numStart + numStartCluster];
  int planeNum = 0;
  int id1, id2;
  float size;
  float size1, size2;
  vector2D dist;
  int nvee = 0;
  int best = 0, ncommon = 0;
  point3D pp;
  bool hasPrimary;
  for (int i = 0; i < nStopP; i++) {
    if (planeNum >= maxPlanes)
      continue;
    if (pto[i]->name() == "TPixel") {
      id1 = ((TPixel*)pto[i])->getID();
      size1 = ((TPixel*)pto[i])->getShape()->circumscribedRadius() /
              ((TPixel*)pto[i])->getShape()->getCenter().Z();
    } else {
      id1 = ((TCluster*)pto[i])->getID();
      size1 = ((TCluster*)pto[i])->getClusterRadius().R() / ((TCluster*)pto[i])->getCenter().Z();
    }
    hasPrimary = false;
    for (int ii = 0; ii < numberOfTracks; ii++)
      if (acos(tracks[ii]->getPath().Direction() * (vector3D(pts[i]) / pts[i].R())) < 0.01)
        hasPrimary = true;
    if (hasPrimary)
      continue;
    for (int j = i + 1; j < nStopP; j++) {
      if (planeNum >= maxPlanes)
        continue;
      hasPrimary = false;
      for (int ii = 0; ii < numberOfTracks; ii++)
        if (acos(tracks[ii]->getPath().Direction() * (vector3D(pts[j]) / pts[j].R())) < 0.01)
          hasPrimary = true;
      if (hasPrimary)
        continue;
      if (pto[j]->name() == "TPixel") {
        id2 = ((TPixel*)pto[j])->getID();
        size2 = ((TPixel*)pto[j])->getShape()->circumscribedRadius() /
                ((TPixel*)pto[j])->getShape()->getCenter().Z();
      } else {
        id2 = ((TCluster*)pto[j])->getID();
        size2 = ((TCluster*)pto[j])->getClusterRadius().R() / ((TCluster*)pto[j])->getCenter().Z();
      }
      npop = 0;
      sLine2D line(stopPoints[i], stopPoints[j]);
      for (int k = 0; k < numStart + numStartCluster; k++)
        nOn[k] = 0;
      for (int k = 0; k < nStartP; k++) {
        if (s_pto[k]->name() == "TPixel")
          size = ((TPixel*)s_pto[k])->getShape()->circumscribedRadius() * 2 / zBuffer[k];
        else
          size = ((TCluster*)s_pto[k])->getClusterRadius().R() * 2 / zBuffer[k];
        dist = line - startPoints[k];

        if (dist.R() < alternateDist * (size /*+size1+size2*/) && npop < 100 &&
            (startPoints[k] - stopPoints[i]).R() > size1 &&
            (startPoints[k] - stopPoints[j]).R() > size2) {
          pop[npop] = k;
          nOn[s_id[k]]++;
          npop++;
        }
      }
      if (npop < (numStart + numStartCluster) * 2)
        continue;
      bool enough = true;
      for (int k = 0; k < numStart + numStartCluster; k++)
        if (nOn[k] < 2)
          enough = false;
      if (!enough)
        continue;
      int smallest, smallestz = 0;
      int tmp;
      for (int k = 1; k < npop; k++)
        if (zBuffer[pop[k]] < zBuffer[smallestz])
          smallestz = pop[k];
      for (int k = 0; k < npop; k++) {
        smallest = k;
        for (int l = k + 1; l < npop; l++) {
          if ((stopPoints[i] - startPoints[pop[smallest]]).R() >
              (stopPoints[i] - startPoints[pop[l]]).R())
            smallest = l;
        }
        tmp = pop[k];
        pop[k] = pop[smallest];
        pop[smallest] = tmp;
      }
      tmp = 1;
      while (zBuffer[pop[tmp - 1]] >= zBuffer[pop[tmp]] && tmp < npop - 1)
        tmp++;
      smallest = tmp;
      while (zBuffer[pop[tmp - 1]] <= zBuffer[pop[tmp]] && tmp < npop - 1)
        tmp++;
      if (tmp < npop - 2)
        continue;
      enough = true;
      int ndt[2] = {0, 0};
      for (int k = 0; k < 2; k++) {
        for (int l = k * (smallest - 1); l < (k == 0 ? smallest + 1 : npop); l++) {
          pointTracks[k][ndt[k]]->reset();
          if ((k == 0 ? id1 : id2) < 2)
            addPixAndElTotrack(pto[(k == 0 ? i : j)], pointTracks[k][ndt[k]]);
          addPixAndElTotrack(s_pto[pop[l]], pointTracks[k][ndt[k]]);
          pointTracks[k][ndt[k]]->setPath(
              sLine3D(pts[(k == 0 ? i : j)], s_pts[k] - pts[(k == 0 ? i : j)]));
          if (searchATrack(ndt[k], k, (k == 0 ? id1 : id2), pto[(k == 0 ? i : j)],
                           s_pto[pop[l]])) // smallest-1*k+1]]));
            ndt[k]++;
        }
      }
      if (ndt[0] == 0 || ndt[1] == 0)
        continue;
      nvee = 0;
      best = 0;
      for (int k = 0; k < ndt[0]; k++) {
        for (int l = 0; l < ndt[1]; l++) {
          if (nvee >= maxPlanes)
            continue;
          if (pointTracks[0][k]->getNumberOfCalibHits() +
                  pointTracks[1][l]->getNumberOfCalibHits() <
              minElementsPerVee)
            continue;
          ncommon = 0;
          for (int ii = 0; ii < pointTracks[0][k]->getNumberOfCalibHits(-2); ii++)
            for (int iii = 0; iii < pointTracks[0][k]->getNumberOfCalibHits(ii); iii++)
              for (int jj = 0; jj < pointTracks[1][l]->getNumberOfCalibHits(ii); jj++)
                if (pointTracks[0][k]->getCalibSrcr(ii, iii) ==
                    pointTracks[1][l]->getCalibSrcr(ii, jj))
                  ncommon++;
          if (ncommon >= 2)
            continue;
          vee[nvee]->setTracks(pointTracks[0][k], pointTracks[1][l]);
          if (!(vee[nvee]->getChiSquared() <= 1000))
            continue;
          if (!(vee[nvee]->getVertex().Z() >= 5 && vee[nvee]->getVertex().Z() <= 120))
            continue;
          // if(vee[nvee]->getVertex().Z()>zBuffer[smallest]*cos(vee[nvee]->getVertex().theta())*0.9)continue;
          if (vee[nvee]->getChiSquared() < vee[best]->getChiSquared())
            best = nvee;
          nvee++;
        }
      }
      if (nvee <= 0)
        continue;
      // if(pointTracks[0][planeNum]->getNumberOfCalibHits()+pointTracks[1][planeNum]->getNumberOfCalibHits()<minElementsPerVee)continue;
      // vee[planeNum]->setTracks(pointTracks[0][planeNum],pointTracks[1][planeNum]);
      cout << vee[best]->getChiSquared() << endl;
      if (vee[best]->getChiSquared() > maxVertexChi)
        continue;
      planeTrack[0][planeNum] = vee[best]->getTrack(0); // pointTracks[0][planeNum];
      planeTrack[1][planeNum] = vee[best]->getTrack(1); // pointTracks[1][planeNum];
      planeTrack[0][planeNum]->setPath(sLine3D(vee[best]->getVertex(), vee[best]->getDirection(0)));
      planeTrack[1][planeNum]->setPath(sLine3D(vee[best]->getVertex(), vee[best]->getDirection(1)));
      planeTrack[0][planeNum]->setChiSquared(vee[best]->getChiSquared());
      planeTrack[1][planeNum]->setChiSquared(vee[best]->getChiSquared());
      planeNum++;
      cout << "addPlane" << endl;
    }
  }
  if (planeNum > 0) {
    nEventsWithVees++;
    if ((nEventsWithVees % 100) == 0)
      cout << "Vee-Events:" << nEventsWithVees << endl;
  }
  return planeNum;
}
