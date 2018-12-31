#include "ACalculateTrackProperties.h"
#include "logger.h"
#include "shape_utilities.h"
#include "wedge.h"
#include <TFile.h>
#include <TH2.h>
#include <TTree.h>
// TH2F sHisto("speed1-speed2","v1-v2",200,-0.1,1.1,200,-0.1,1.1);
//#define BETATESTOUT
//#define TWOTWO_out
#ifdef TWOTWO_out
typedef struct
{
  Double_t vertex[3];
  Double_t fitted[3];
  Int_t el[30];
  Int_t det[30];
  Double_t phi;
  Double_t theta;
  Double_t beta;
  Double_t energy;
  Double_t momentum;
  Double_t front;
  Double_t back;
  Double_t path;
  Double_t lpath;
  Int_t ID;
  Int_t nEl;
} trackT;
void finalTwotwoTracks();
void setTwotwoTracks(TTrack* p1, TTrack* p2, TTrack* s1, TTrack* s2, point3D* pf, point3D* pb,
                     float* tf, float* tb);
void defineTwotwoTree();
TTree* twotwoTree = NULL;
TFile* twotwoFile = NULL;
trackT twotwoTrack[4];
Int_t twotwoEvent;
Int_t twotwoRun;
Float_t minv_dec;
Float_t minv[10];
Float_t mmiss[10];
Float_t chi_twotwo;
Float_t angle_twotwo[20];
#endif
#ifdef BETATESTOUT
TH1F weglaenge("length", "length", 200, 0, 5000);
TH1F startTiminghisto("start Timing", "start Timing", 200, -100, 100);
TH1F stopTiminghisto("stop Timing", "stop Timing", 200, -100, 100);
TH1F timeDifference("\\Delta t", "\\Delta t", 200, -100, 100);
TH2F timeway("length vs \\Delta t", "length vs \\Delta t", 200, -100, 100, 200, 0, 5000);
TTree betaOutTree("betaProperties", "betaProperties");
Float_t dist;
Float_t dtdc;
Float_t stdc;
Float_t otdc;
Int_t detID;
Int_t trNr;
Float_t th, ph;
Float_t zpos;
#endif
#define TDC_OUTPUT
#ifdef TDC_OUTPUT
#include "Eparticles.h"
#include "RtwoParticleDecay.h"
#include <TFile.h>
#include <TTree.h>
typedef struct
{
  Float_t tdcB1;
  Float_t tdcB2;
  Float_t tdcB3;
  Float_t tdcF1;
  Float_t tdcF2;
  Float_t theta;
  Float_t beta;
  Float_t phi;
  Float_t pt;
  Float_t pl;
  Float_t ll;
  Float_t sollTOF;
  Float_t dTOF;
  Float_t TOF;
  Int_t det1;
  Int_t det2;
  Int_t det3;
  Int_t elB1;
  Int_t elB2;
  Int_t elB3;
  Int_t elF1;
  Int_t elF2;
} betaInfo;
TTree* treeinput_out = NULL;
betaInfo* tra_info_out;
#endif

ACalculateTrackProperties::ACalculateTrackProperties(TTrack** t, int& numberOfTracksIn, int max,
                                                     const TDetector& st,
                                                     const algorithm_parameter& descr)
    : AAlgorithm("Calculate track properties"), maxTracks(max), numberOfTracks(numberOfTracksIn),
      numStart(descr.getParam<vector<int>>(0).getData().size()),
      numStop(descr.getParam<vector<int>>(1).getData().size())
{
  startIDs = new int[numStart];
  for (int i = 0; i < numStart; i++)
    startIDs[i] = descr.getParam<vector<int>>(0).getData().at(i);
  stopIDs = new int[numStop];
  for (int i = 0; i < numStop; i++)
    stopIDs[i] = descr.getParam<vector<int>>(1).getData().at(i);
#ifdef SHOWPARAMS
  cout << "start detectors:" << flush;
  for (int i = 0; i < numStart; i++)
    cout << startIDs[i] << "\t" << flush;
  cout << "\nstop detectors:" << flush;
  for (int i = 0; i < numStop; i++)
    cout << stopIDs[i] << "\t" << flush;
  cout << endl;
#endif
  tracks = t;
  timing1 = new float[max];
  timing2 = new float[max];
  hit1 = new point3D[max];
  volumeShape* sh;
  sh = st.getShape(0);
  start = plane3D(((wedge*)sh)->getCenter(), ((wedge*)sh)->getNormal());
  if (descr.getNumberOfParam<bool>() > 0)
    chargedPrompt = descr.getParam<bool>(0).getData();
  else
    chargedPrompt = true;
  if (descr.getNumberOfParam<bool>() > 1)
    neutralPrompt = descr.getParam<bool>(1).getData();
  else
    neutralPrompt = false;
  for (int i = 0; i < 4; i++)
    if (descr.getNumberOfParam<bool>() > 2 + i)
      secondary[i] = descr.getParam<bool>(2 + i).getData();
    else
      secondary[i] = false;
#ifdef TWOTWO_out
  defineTwotwoTree();
#endif
}

ACalculateTrackProperties::ACalculateTrackProperties(TTrack** t, int& numberOfTracksIn, int max,
                                                     const TDetector& st, vector<int> stopDetectors,
                                                     vector<int> startDetectors)
    : AAlgorithm("Calculate track properties"), maxTracks(max), numberOfTracks(numberOfTracksIn),
      numStart(startDetectors.size()), numStop(stopDetectors.size())
{
  startIDs = new int[numStart];
  for (int i = 0; i < numStart; i++)
    startIDs[i] = startDetectors[i];
  stopIDs = new int[numStop];
  for (int i = 0; i < numStop; i++)
    stopIDs[i] = stopDetectors[i];
  tracks = t;
  timing1 = new float[max];
  timing2 = new float[max];
  hit1 = new point3D[max];
  volumeShape* sh;
  sh = st.getShape(0);
  start = plane3D(((wedge*)sh)->getCenter(), ((wedge*)sh)->getNormal());
  chargedPrompt = true;
  neutralPrompt = false;
  for (int i = 0; i < 4; i++)
    secondary[i] = false;
#ifdef TWOTWO_out
  defineTwotwoTree();
#endif
}

ACalculateTrackProperties::~ACalculateTrackProperties()
{
  delete[] timing1;
  delete[] timing2;
  delete[] hit1;
  delete[] startIDs;
  delete[] stopIDs;
#ifdef TWOTWO_out
  finalTwotwoTracks();
#endif
}
#include "container_utilities.h"
void* ACalculateTrackProperties::process(void* ptr)
{
  point3D hit2, hit3, hittmp;
  if (numberOfTracks >= 100 || numberOfTracks == 0)
    return 0;
  float startTimings[numberOfTracks];
  float stopTimings[numberOfTracks];
  point3D startPoints[numberOfTracks];
  point3D stopPoints[numberOfTracks];
  float globalTiming = 0;
  int nGlobals = 0;
  //   cout<<"new event"<<endl;
  for (int i = 0; i < numberOfTracks; i++) {
    startTimings[i] = -1;
    if (tracks[i] == NULL)
      continue;
    startTimings[i] = startTiming(tracks[i], startPoints[i]);
    stopTimings[i] = stopTiming(tracks[i], stopPoints[i]);
    if (startTimings[i] != -1) {
      globalTiming += startTimings[i];
      nGlobals++;
    }
  }
  globalTiming = globalTiming * (nGlobals > 0 ? 1 / ((double)nGlobals) : 1.);
  float timeStart, distance, flightTime;
  TCalibHit* stopHits[numStop];
  TCalibHit* startHits[numStart];
  vector3D myDistance;
  int n1, n2;
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i] == NULL)
      continue;
    //       cout<<"track "<<i<<" "<<tracks[i]->getNumberOfCalibHits()<<"
    //       hits"<<endl;
    if (tracks[i]->getNumberOfCalibHits() == 0)
      continue;
    n1 = 0;
    n2 = 0;
    for (int j = 0; j < numStop; j++) {
      stopHits[n1] = NULL;
      if (tracks[i]->getNumberOfCalibHits(stopIDs[j]) > 0)
        stopHits[n1++] = tracks[i]->getCalibSrcr(stopIDs[j], 0);
    }
    for (int j = 0; j < numStart; j++) {
      startHits[n2] = NULL;
      if (tracks[i]->getNumberOfCalibHits(startIDs[j]) > 0)
        startHits[n2++] = tracks[i]->getCalibSrcr(startIDs[j], 0);
    }
    if (n1 == 0 || n2 == 0) {
      distance = (stopPoints[i] - startPoints[i]).R();
    } else {
      myDistance = getPath(n2, startHits, n1, stopHits, tracks[i]->getPath());
      // 	  cout<<myDistance.toString(_spheric_).data()<<endl;
      if (myDistance.getState() == _undefined_)
        distance = (stopPoints[i] - startPoints[i]).R();
      else
        distance = myDistance.R();
    }
    timeStart = startTimings[i];
    if (timeStart == -1)
      timeStart = globalTiming;
    flightTime = abs(timeStart - stopTimings[i]);
    if (tracks[i]->isSecondary()) {
      // 	  point3D hh1;
      // 	  hh1=tracks[i]->getPath().Foot();
      // 	  if((stopPoints[i]-hh1)*(hh1-startPoints[i])>0)
      // 	    distance=(stopPoints[i]-hh1).R()+(startPoints[i]-hh1).R();
      // flightTime+=adjustSec;
    }
    tracks[i]->setSpeed((distance / flightTime) / 299.792);
    tracks[i]->setErrorSpeed(tracks[i]->getSpeed() / (flightTime));
    //       cout<<"track "<<i<<" "<<"d="<<distance<<" tof="<<flightTime<<"
    //       beta="<<tracks[i]->getSpeed()<<endl;
  }
#ifdef TWOTWO_out
  if (numberOfTracks == 5 && tracks[2]->getNumberOfSecondaryTracks() == 2) {
    point3D pf[4] = {hit1[0], hit1[1], point3D(0, 0, 16.1), point3D(0, 0, 16.1)},
            pb[4] = {stopPoints[0], stopPoints[1], stopPoints[3], stopPoints[4]};
    float tf[4] = {startTimings[0], startTimings[1], globalTiming, globalTiming},
          tb[4] = {stopTimings[0], stopTimings[1], stopTimings[3], stopTimings[4]};
    vector3D v1 = tracks[2]->getSecondary(0)->getPath().Direction();
    vector3D v2 = tracks[2]->getSecondary(1)->getPath().Direction();
    vector3D d = tracks[2]->getPath().Direction();
    if (acos(v1 * d) < acos(v2 * d))
      setTwotwoTracks(tracks[0], tracks[1], tracks[2]->getSecondary(0), tracks[2]->getSecondary(1),
                      pf, pb, tf, tb);
    else {
      float tmpf = tf[3];
      tf[3] = tf[2];
      tf[2] = tmpf;
      tmpf = tb[3];
      tb[3] = tb[2];
      tb[2] = tmpf;
      point3D tmpp = pb[3];
      pb[3] = pb[2];
      pb[2] = tmpp;
      setTwotwoTracks(tracks[0], tracks[1], tracks[2]->getSecondary(1), tracks[2]->getSecondary(0),
                      pf, pb, tf, tb);
    }
  }
#endif
  return ptr;
}

// #include "spiral.h"
// #include "ring.h"
float ACalculateTrackProperties::startTiming(TTrack* track, point3D& htt)
{
  float t1 = 0, tim;
  point3D hiti2[numStart], hittmp(0, 0, 0);
  int starts = 0;
  volumeShape* sh = NULL;
  for (int i = 0; i < numStart; i++) {
    if (track->getNumberOfCalibHits(startIDs[i]) > 0)
      if (track->getCalibSrc(startIDs[i], 0).getHitShape() != NULL) {
        sh = track->getCalibSrcr(startIDs[i], 0)->getHitShape();
        hiti2[starts] = getEntrance(sh, track->getPath()); // ps-track->getPath();
        if (hiti2[starts].getState() != _undefined_) {
          hittmp = hittmp + vector3D(hiti2[starts]);
          t1 += track->getCalibSrcr(startIDs[i], 0)->getTDC();
          starts++;
        }
      }
  }
  if (starts == 0) {
    htt = plane3D(point3D(0, 0, 16.2), vector3D(0, 0, -1)) - track->getPath();
    tim = -1;
  } else {
    tim = t1 / ((double)starts);
    htt = point3D(vector3D(hittmp) * (1 / ((double)starts)));
  }
  return tim;
}
float ACalculateTrackProperties::stopTiming(TTrack* track, point3D& h)
{
  float t1 = 0, tim;
  point3D hiti2[numStop], hittmp(0, 0, 0);
  int stops = 0;
  volumeShape* sh = NULL;
  for (int i = 0; i < numStop; i++) {
    if (track->getNumberOfCalibHits(stopIDs[i]) > 0) {
      if (track->getCalibSrcr(stopIDs[i], 0) != NULL) {
        sh = track->getCalibSrcr(stopIDs[i], 0)->getHitShape();
        if (sh != NULL) {
          hiti2[stops] = getEntrance(sh, track->getPath());
          if (hiti2[stops].getState() != _undefined_) {
            t1 += track->getCalibSrcr(stopIDs[i], 0)->getTDC();
            hittmp = hittmp + vector3D(hiti2[stops]);
            stops++;
          }
        }
      }
    }
  }
  h = point3D(vector3D(hittmp) * (1 / ((double)stops)));
  if (stops == 0) {
    h.setValues(0, 0, 0);
    tim = -1;
  } else
    tim = t1 / ((double)stops);
  return tim;
}

#ifdef TWOTWO_out
void defineTwotwoTree()
{
  twotwoFile = new TFile("twotwo.root", "recreate");
  twotwoFile->cd();
  twotwoTree = new TTree("p2s2", "p2s2");
  twotwoTree->Branch("EventNumber", &twotwoEvent, "EventNumber/I");
  twotwoTree->Branch("RunNumber", &twotwoRun, "RunNumber/I");
  twotwoTree->Branch("p1", &twotwoTrack[0], "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                            "D:theta:beta:energy:momentum:front:back:path:lp:ID/"
                                            "I:nEl");
  twotwoTree->Branch("p2", &twotwoTrack[1], "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                            "D:theta:beta:energy:momentum:front:back:path:lp:ID/"
                                            "I:nEl");
  twotwoTree->Branch("s1", &twotwoTrack[2], "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                            "D:theta:beta:energy:momentum:front:back:path:lp:ID/"
                                            "I:nEl");
  twotwoTree->Branch("s2", &twotwoTrack[3], "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                            "D:theta:beta:energy:momentum:front:back:path:lp:ID/"
                                            "I:nEl");
  twotwoTree->Branch("invariantMass_decay", &minv_dec, "invariantMass_decay/F");
  twotwoTree->Branch("invariantMass", &minv, "invariantMass[10]/F");
  twotwoTree->Branch("missingMass", &mmiss, "missingMass[10]/F");
  twotwoTree->Branch("chi", &chi_twotwo, "chi/F");
  twotwoTree->Branch("angle", &angle_twotwo, "angle[20]/F");
  for (int i = 0; i < 4; i++) {
    twotwoTrack[i].energy = 0;
    twotwoTrack[i].momentum = 0;
    for (int j = 0; j < 30; j++) {
      twotwoTrack[i].det[j] = -1;
      twotwoTrack[i].el[j] = -1;
    }
  }
  twotwoTrack[0].ID = -1;
  twotwoTrack[1].ID = -1;
  twotwoTrack[2].ID = 14;
  twotwoTrack[3].ID = 9;
}
//#include "RtwoParticleDecay.h"
#include "fittingRoutines.h"
void doKinfitTwoTwo(kinFit* kfit, vector3D v1, vector3D v2, vector3D v3, vector3D v4, float m1,
                    float m2, float m3, float m4, float b1, float b2, float b3, float b4);
extern float getLightWay(volumeShape* sh, point3D onPoint, bool from);
extern vector3D solve(const vector3D& v1, const vector3D& v2, const vector3D& v3,
                      const vector3D& init);
void doVfit_twotwo(TTrack* p1, TTrack* p2, vector3D& v0, vector3D& v1, point3D& vert);
void setTwotwoTracks(TTrack* p1, TTrack* p2, TTrack* s1, TTrack* s2, point3D* pf, point3D* pb,
                     float* tf, float* tb)
{
  if (twotwoTree == NULL)
    return;
  int n;
  momentum4D tmp1, tmp2, tmp11, tmp22;
  momentum4D initA(0, 0, 0, Eparticles::getMassG(14));
  momentum4D initB(0, 0, 0, 1);
  initB.setPM(vector3D(0, 0, 3.059), Eparticles::getMassG(14));
  TTrack* currentTrack;
  vector3D v[4][2];
  vector3D ld[6];
  ld[0] = s1->getPath().Foot() - point3D(0, 0, 0);
  lLine3D poca = p1->getPath() - p2->getPath();
  point3D vert1 = poca.Mid();
  for (int i = 0; i < 4; i++) {
    currentTrack = NULL;
    switch (i) {
    case 0:
      currentTrack = p1;
      break;
    case 1:
      currentTrack = p2;
      break;
    case 2:
      currentTrack = s1;
      break;
    case 3:
      currentTrack = s2;
      break;
    }
    if (currentTrack == NULL)
      continue;
    v[i][0] = currentTrack->getPath().Direction();
    if (i >= 2)
      v[i][1] = currentTrack->getPath().Direction();
    //       if(i>=2)
    {
      twotwoTrack[i].vertex[0] = currentTrack->getPath().Foot().X();
      twotwoTrack[i].vertex[1] = currentTrack->getPath().Foot().Y();
      twotwoTrack[i].vertex[2] = currentTrack->getPath().Foot().Z();
    }
    //       else
    // 	{
    // 	  twotwoTrack[i].vertex[0]=vert1.X();
    // 	  twotwoTrack[i].vertex[1]=vert1.Y();
    // 	  twotwoTrack[i].vertex[2]=vert1.Z();
    // 	}
    twotwoTrack[i].phi = v[i][0].Phi();
    twotwoTrack[i].theta = v[i][0].Theta();
    twotwoTrack[i].beta = currentTrack->getSpeed();
    twotwoTrack[i].front = tf[i];
    twotwoTrack[i].back = tb[i];
    if (i >= 2)
      twotwoTrack[i].path = (pf[i] - currentTrack->getPath().Foot()).R() +
                            (currentTrack->getPath().Foot() - pb[i]).R();
    else
      twotwoTrack[i].path = (pf[i] - pb[i]).R();
    n = 0;
    for (int k = 0; k < 30; k++)
      for (int j = 0; j < currentTrack->getNumberOfCalibHits(k); j++) {
        if (n >= 30)
          continue;
        twotwoTrack[i].el[n] = currentTrack->getCalibSrcr(k, j)->getElement();
        twotwoTrack[i].det[n] = currentTrack->getCalibSrcr(k, j)->getDetector();
        n++;
      }
    twotwoTrack[i].nEl = n;
    twotwoTrack[i].lpath = -1;
    switch (twotwoTrack[i].det[0]) {
    case 0:
      twotwoTrack[i].lpath =
          getLightWay(currentTrack->getCalibSrcr(0, 0)->getHitShape(), pb[i], false);
      break;
    case 3:
      twotwoTrack[i].lpath =
          getLightWay(currentTrack->getCalibSrcr(3, 0)->getHitShape(), pb[i], false);
      break;
    case 6:
      twotwoTrack[i].lpath =
          getLightWay(currentTrack->getCalibSrcr(6, 0)->getHitShape(), pb[i], false);
      break;
    }
  }
  //
  point3D vert;
  doVfit_twotwo(p1, p2, v[0][1], v[1][1], vert);
  ld[1] = s1->getPath().Foot() - vert;
  //
  tmp1.setVM(twotwoTrack[0].beta * v[0][0], Eparticles::getMassG(14));
  tmp11.setVM(twotwoTrack[0].beta * v[0][0], Eparticles::getMassG(11));
  tmp2.setVM(twotwoTrack[1].beta * v[1][0], Eparticles::getMassG(11));
  tmp22.setVM(twotwoTrack[1].beta * v[1][0], Eparticles::getMassG(14));
  ld[2] = (initA + initB - tmp1 - tmp2).Momentum();
  ld[2] = ld[2] / ld[2].R();
  ld[3] = (initA + initB - tmp11 - tmp22).Momentum();
  ld[3] = ld[3] / ld[3].R();
  minv[0] = sqrt((tmp1 + tmp2) * (tmp1 + tmp2));
  minv[1] = sqrt((tmp11 + tmp22) * (tmp11 + tmp22));
  minv[2] = RtwoParticleDecay::getPrimary(Eparticles::getMassG(twotwoTrack[2].ID),
                                          Eparticles::getMassG(twotwoTrack[3].ID), ld[1], v[2][0],
                                          v[3][0], s1->getSpeed())
                .Mass();
  mmiss[0] = ((initA + initB - tmp1 - tmp2) * (initA + initB - tmp1 - tmp2));
  mmiss[1] = ((initA + initB - tmp11 - tmp22) * (initA + initB - tmp11 - tmp22));
  vector3D d;
  if (abs(mmiss[0] - 1.11568 * 1.11568) < abs(mmiss[1] - 1.11568 * 1.11568)) {
    d = (initA + initB - tmp1 - tmp2).Momentum();
    d.normalize();
  } else {
    d = (initA + initB - tmp11 - tmp22).Momentum();
    d.normalize();
  }
  plane3D pl(s1->getPath().Foot(), v[2][0], v[3][0]);
  vector3D w = ld[0]; // s2->getPath().Foot()-point3D(0,0,0);w.normalize();
  angle_twotwo[2] = acos((d * w) / d.R() / w.R());
  angle_twotwo[5] = asin((d * pl.Normal()) / d.R());
  angle_twotwo[1] = acos((ld[3] * w) / ld[3].R() / w.R());
  angle_twotwo[4] = asin((ld[3] * pl.Normal()) / ld[3].R());
  angle_twotwo[0] = acos((ld[2] * w) / ld[2].R() / w.R());
  angle_twotwo[3] = asin((ld[2] * pl.Normal()) / ld[2].R());
  angle_twotwo[6] = asin((w * pl.Normal()) / w.R());

  tmp1.setVM(twotwoTrack[0].beta * v[0][1], Eparticles::getMassG(14));
  tmp11.setVM(twotwoTrack[0].beta * v[0][1], Eparticles::getMassG(11));
  tmp2.setVM(twotwoTrack[1].beta * v[1][1], Eparticles::getMassG(11));
  tmp22.setVM(twotwoTrack[1].beta * v[1][1], Eparticles::getMassG(14));
  ld[4] = (initA + initB - tmp1 - tmp2).Momentum();
  ld[4] = ld[4] / ld[4].R();
  ld[5] = (initA + initB - tmp11 - tmp22).Momentum();
  ld[5] = ld[5] / ld[5].R();
  angle_twotwo[10] = acos((ld[1] * ld[4]) / ld[1].R() / ld[4].R());
  angle_twotwo[11] = acos((ld[1] * ld[5]) / ld[1].R() / ld[5].R());
  angle_twotwo[13] = asin((ld[1] * pl.Normal()) / ld[1].R() / pl.Normal().R());
  angle_twotwo[14] = asin((ld[4] * pl.Normal()) / ld[4].R() / pl.Normal().R());
  angle_twotwo[16] = asin((ld[5] * pl.Normal()) / ld[5].R() / pl.Normal().R());

  tmp1.setVM(twotwoTrack[2].beta * v[2][0], Eparticles::getMassG(twotwoTrack[2].ID));
  twotwoTrack[2].energy = tmp1.Energy() - tmp1.Mass();
  twotwoTrack[2].momentum = tmp1.Momentum().R();
  tmp2.setVM((twotwoTrack[2].beta < 1 ? twotwoTrack[2].beta : 0.95) * v[3][0],
             Eparticles::getMassG(twotwoTrack[3].ID));
  twotwoTrack[3].energy = tmp2.Energy() - tmp2.Mass();
  twotwoTrack[3].momentum = tmp2.Momentum().R();
  minv_dec = sqrt((tmp1 + tmp2) * (tmp1 + tmp2));
  vector3D ret = solve(v[0][1], v[1][1], ld[1], vector3D(0, 0, 3.059));

  twotwoTrack[0].momentum = ret[0];
  twotwoTrack[1].momentum = ret[1];
  tmp1.setPM(v[0][1] * ret[0], Eparticles::getMassG(14));
  tmp11.setPM(v[0][1] * ret[0], Eparticles::getMassG(11));
  tmp2.setPM(v[1][1] * ret[1], Eparticles::getMassG(11));
  tmp22.setPM(v[1][1] * ret[1], Eparticles::getMassG(14));
  mmiss[2] = ((initA + initB - tmp1 - tmp2) * (initA + initB - tmp1 - tmp2));
  mmiss[3] = ((initA + initB - tmp11 - tmp22) * (initA + initB - tmp11 - tmp22));
  if (abs(mmiss[2] - 1.2447418624) < abs(mmiss[3] - 1.2447418624))
    mmiss[5] = mmiss[2];
  else
    mmiss[5] = mmiss[3];
  d = (initA + initB - tmp1 - tmp2).Momentum();
  d.normalize();
  //     cout<<acos(d*v)<<endl;
  float mp = Eparticles::getMassG(14), mk = Eparticles::getMassG(11), mpi = Eparticles::getMassG(9);
  kinFit* myKinFit[4];
  for (int i = 0; i < 4; i++)
    myKinFit[i] = new kinFit(50, 7, 1e-5);
  doKinfitTwoTwo(myKinFit[0], v[0][0], v[1][0], v[2][0], v[3][0], mp, mk, mp, mpi, p1->getSpeed(),
                 p2->getSpeed(), s1->getSpeed(), s2->getSpeed());
  doKinfitTwoTwo(myKinFit[1], v[0][0], v[1][0], v[2][0], v[3][0], mp, mk, mpi, mp, p1->getSpeed(),
                 p2->getSpeed(), s1->getSpeed(), s2->getSpeed());
  doKinfitTwoTwo(myKinFit[2], v[0][0], v[1][0], v[2][0], v[3][0], mk, mp, mp, mpi, p1->getSpeed(),
                 p2->getSpeed(), s1->getSpeed(), s2->getSpeed());
  doKinfitTwoTwo(myKinFit[3], v[0][0], v[1][0], v[2][0], v[3][0], mk, mp, mpi, mp, p1->getSpeed(),
                 p2->getSpeed(), s1->getSpeed(), s2->getSpeed());
  int best = 0;
  if (myKinFit[1]->Chi2() >= 0 &&
      ((!(myKinFit[best]->Chi2() < 1e20)) || myKinFit[1]->Chi2() < myKinFit[best]->Chi2()))
    best = 1;
  if (myKinFit[2]->Chi2() >= 0 &&
      ((!(myKinFit[best]->Chi2() < 1e20)) || myKinFit[2]->Chi2() < myKinFit[best]->Chi2()))
    best = 2;
  if (myKinFit[3]->Chi2() >= 0 &&
      ((!(myKinFit[best]->Chi2() < 1e20)) || myKinFit[3]->Chi2() < myKinFit[best]->Chi2()))
    best = 3;
  chi_twotwo = myKinFit[best]->Chi2();
  //   cout<<chi_twotwo<<endl;
  for (int i = 0; i < 4; i++) {
    momentum4D myp = myKinFit[best]->getMomentum(i + 2, myKinFit[best]->FittedParticle(i + 2));
    //       cout<<"\t"<<myp<<endl;
    twotwoTrack[i].fitted[0] = myp.Momentum().Theta();
    twotwoTrack[i].fitted[1] = myp.Momentum().Phi();
    twotwoTrack[i].fitted[2] = myp.Beta();
  }
  tmp1 = myKinFit[best]->getMomentum(2, myKinFit[best]->FittedParticle(2));
  tmp2 = myKinFit[best]->getMomentum(3, myKinFit[best]->FittedParticle(3));
  mmiss[4] = ((initA + initB - tmp1 - tmp2) * (initA + initB - tmp1 - tmp2));
  d = (initA + initB - tmp1 - tmp2).Momentum();
  d.normalize();
  angle_twotwo[7] = asin((d * pl.Normal()) / d.R());
  for (int i = 0; i < 4; i++)
    delete myKinFit[i];
  twotwoTree->Fill();
}

void doVfit_twotwo(TTrack* p1, TTrack* p2, vector3D& v0, vector3D& v1, point3D& vert)
{
  vertexFit myFit(2, 40);
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < p1->getNumberOfCalibHits(i); j++) {
      if (p1->getCalibSrcr(i, j)->getHitShape() == NULL)
        continue;
      Vector hp(p1->getCalibSrcr(i, j)->getHitShape()->HitParams(p1->getPath()));
      myFit.addPoint(vector3D(hp[7], hp[8], hp[9]), vector3D(hp[4], hp[5], hp[6]), hp[0], 0);
    }
    for (int j = 0; j < p2->getNumberOfCalibHits(i); j++) {
      if (p2->getCalibSrcr(i, j)->getHitShape() == NULL)
        continue;
      Vector hp(p2->getCalibSrcr(i, j)->getHitShape()->HitParams(p2->getPath()));
      myFit.addPoint(vector3D(hp[7], hp[8], hp[9]), vector3D(hp[4], hp[5], hp[6]), hp[0], 1);
    }
  }
  myFit.fit();
  vert = myFit.Vertex();
  v0 = myFit.Line(0);
  v1 = myFit.Line(1);
}
void doKinfitTwoTwo(kinFit* kfit, vector3D v1, vector3D v2, vector3D v3, vector3D v4, float m1,
                    float m2, float m3, float m4, float b1, float b2, float b3, float b4)
{
  int mode = 0;
  kfit->add(vector3D(0, 0, 0), vector3D(0, 0, 0), vector3D(2, 2, 2), Eparticles::getMassG(14), 1,
            -1);
  kfit->add(vector3D(0, 0, 3.059), vector3D(0, 0, 0), vector3D(2, 2, 2), Eparticles::getMassG(14),
            1, -1);
  momentum4D tmp;
  vector3D mea(1, 1, 1);
  float e[4];
  float p[4];
  vector3D v[4] = {v1 / v1.R(), v2 / v2.R(), v3 / v3.R(), v4 / v4.R()};
  float b[4] = {b1, b2, b3, b4};
  float m[4] = {m1, m2, m3, m4};
  vector3D err[4][6];
  for (int i = 0; i < 4; i++) {
    if (b[i] > 1)
      b[i] = 0.99;
    tmp.setVM(v[i] * b[i], m[i]);
    e[i] = tmp.Energy() - m[i];
    p[i] = tmp.Momentum().R();
    err[i][1] = v[i] * (p[i] * 0.05);
  }
  for (int j = 0; j < 2; j++) {
    err[j][0].setValues(.02, .02, .05);
    err[j][2].setValues(.02, .05, .05);
    err[j][3].setValues(b[j] * 0.3 / (twotwoTrack[j].front - twotwoTrack[j].back), .02, .05);
    err[j][4].setValues(b[j] * 0.3 / (twotwoTrack[j].front - twotwoTrack[j].back), .02, .05);
    err[j][5].setValues(p[j] * 0.05, .05, .05);
  }
  for (int j = 2; j < 4; j++) {
    err[j][0].setValues(.05, .04, .1);
    err[j][1] = err[j][1] * 2.;
    err[j][2].setValues(.05, .06, .06);
    err[j][3].setValues(b[j] * 0.3 / (twotwoTrack[j].front - twotwoTrack[j].back), .06, .06);
    err[j][4].setValues(b[j] * 0.3 / (twotwoTrack[j].front - twotwoTrack[j].back), .06, .06);
    err[j][5].setValues(p[j] * 0.05, .06, .06);
  }
  err[3][3].setValues(.5, .02, .05);
  err[3][4].setValues(.5, .06, .06);
  for (int i = 0; i < 4; i++) {
    switch (mode) {
    case 0:
      kfit->add(vector3D(e[i], v[i].Theta(), v[i].Phi()), err[i][mode], mea, m[i], 2);
      break;
    case 1:
      kfit->add(p[i] * v[i], err[i][mode], mea, m[i], 1);
      break;
    case 2:
      kfit->add(vector3D(e[i], v[i].X() / v[i].Z(), v[i].Y() / v[i].Z()), err[i][mode], mea, m[i],
                5);
      break;
    case 3:
      kfit->add(vector3D(b[i], v[i].Theta(), v[i].Phi()), err[i][mode], mea, m[i], 3);
      break;
    case 4:
      kfit->add(vector3D(b[i], v[i].X() / v[i].Z(), v[i].Y() / v[i].Z()), err[i][mode], mea, m[i],
                7);
      break;
    case 5:
      kfit->add(vector3D(p[i], v[i].X() / v[i].Z(), v[i].Y() / v[i].Z()), err[i][mode], mea, m[i],
                6);
      break;
    }
  }
  kfit->fit();
}
void finalTwotwoTracks()
{
  if (twotwoFile == NULL)
    return;
  if (twotwoTree == NULL)
    return;
  twotwoFile->cd();
  twotwoTree->Write();
  cout << "twoTwo-file written with " << twotwoTree->GetEntries() << " Entries" << endl;
  twotwoFile->Close();
  delete twotwoFile;
  twotwoFile = NULL;
  twotwoTree = NULL;
}
#endif
