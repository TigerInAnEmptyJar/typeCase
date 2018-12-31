#include "RbaseReaction.h"
#include "fitAlgorithm.h"
RbaseReaction::RbaseReaction(AFitAlgorithm* kinFitIn, int nTracks, int nInits)
    : TBase("RbaseReaction"), numberOfInitParticles(nInits), numberOfTracks(nTracks)
{
  fkinFit = kinFitIn;
  tracks = new TTrack*[nTracks];
  initParticles = new momentum4D[nInits];
}

RbaseReaction::RbaseReaction(const RbaseReaction& rIn)
    : TBase("RbaseReaction"), numberOfInitParticles(rIn.getNumberOfPrimaries()),
      numberOfTracks(rIn.getNumberOfTracks())
{
  fkinFit = rIn.getKinFit();
  tracks = new TTrack*[numberOfTracks];
  initParticles = new momentum4D[numberOfInitParticles];
  setChiLimit(rIn.getChiLimit());
  excessEnergy = rIn.getExcessEnergy();
}

RbaseReaction::~RbaseReaction()
{
  delete[] tracks;
  delete[] initParticles;
  delete[] particles;
  delete[] particleIDs;
}

int RbaseReaction::getNumberOfTracks() const { return numberOfTracks; }

int RbaseReaction::getNumberOfPrimaries() const { return -1; }

int RbaseReaction::getNumberOfSecondaries() const { return -1; }

momentum4D RbaseReaction::getParticle(int num) const
{
  if (num < 0 || num >= numberOfParticles)
    return momentum4D();
  else
    return particles[num];
}

void RbaseReaction::setTrack(int num, TTrack* track)
{
  if (num < 0 || num > numberOfTracks)
    return;
  tracks[num] = track;
}
TTrack* RbaseReaction::getTrack(int num)
{
  if (num < 0 || num > numberOfTracks)
    return NULL;
  TTrack* ret = (tracks[num]);
  return ret;
}

// float RbaseReaction::check(TTrack **tracksIn, int num)
// {
//   return -1;
// }

// float RbaseReaction::justCheck(TTrack **tracksIn, int num, int *indexes,
// momentum4D *results)
// {
//   return -1;
// }

// RbaseReaction* RbaseReaction::checkNew(TTrack **tracks,int num)
// {
//   RbaseReaction *ret=new RbaseReaction(*this);
//   if(ret->check(tracks,num)>getChiLimit()||ret->getChiSquared()==-1)
//     {
//       delete ret;
//       return NULL;
//     }
//   return ret;
// }

void RbaseReaction::setChiLimit(float chi)
{
  if (chi <= 0)
    return;
  chiLimit = chi;
}

float RbaseReaction::getChiLimit() const { return chiLimit; }

void RbaseReaction::setInitParticles(int num, momentum4D* p)
{
  for (int i = 0; i < (num < numberOfInitParticles ? num : numberOfInitParticles); i++)
    initParticles[i] = p[i];
  switch (numberOfInitParticles) {
  case 1:
    cm = initParticles[0];
    break;
  case 2:
    cm = momentum4D::CM(initParticles[0], initParticles[1]);
    break;
  case 3:
    cm = momentum4D::CM(initParticles[0], initParticles[1], initParticles[2]);
    break;
  case 4:
    cm = momentum4D::CM(initParticles[0], initParticles[1], initParticles[2], initParticles[3]);
    break;
  }
  //  calculateCM();
}

momentum4D RbaseReaction::getInitParticle(int num) const
{
  if (num < 0 || num > numberOfInitParticles)
    return momentum4D();
  return initParticles[num];
}

float RbaseReaction::getExcessEnergy() const { return excessEnergy; }

momentum4D RbaseReaction::getCenterOfMass() const { return cm; }

momentum4D RbaseReaction::getCMParticle(int num) const
{
  if (num < 0 || num > numberOfParticles)
    return momentum4D();
  return cmparticles[num];
}
float RbaseReaction::getChiSquared() const { return chi2; }
AFitAlgorithm* RbaseReaction::getKinFit() const { return fkinFit; }
void RbaseReaction::calculateCM()
{
  vector3D part, beta = cm.Velocity() * -1;
  for (int i = 0; i < numberOfParticles; i++) {
    cmparticles[i] = particles[i].boost(beta);
  }
}
RbaseReaction* RbaseReaction::getClone() { return new RbaseReaction(*this); }
// bool RbaseReaction::operator<(const RbaseReaction &r)
// {
//   return r.getChiSquared()<getChiSquared();
// }
// bool RbaseReaction::operator<=(const RbaseReaction &r)
// {
//   return r.getChiSquared()<=getChiSquared();
// }
// bool RbaseReaction::operator>(const RbaseReaction &r)
// {
//   return r.getChiSquared()>getChiSquared();
// }
// bool RbaseReaction::operator>=(const RbaseReaction &r)
// {
//   return r.getChiSquared()>=getChiSquared();
// }
// Bool_t RbaseReaction::IsSortable()
// {
//   return kTRUE;
// }
int RbaseReaction::getParticleID(int num) const
{
  if (num >= numberOfParticles || num < 0)
    return -1;
  return particleIDs[num];
}
void RbaseReaction::calculate(int justGeometry)
{
  if (justGeometry < 1000)
    calculateCM();
}
float RbaseReaction::check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                           TTrack** tracks_2s, int num_2s)
{
  if (tracks_nd == NULL || tracks_1s == NULL || tracks_2s == NULL)
    return -1;
  if (num_nd == 0 && num_1s == 0 && num_2s == 0)
    return -1;
  return -1;
}
float RbaseReaction::justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                               TTrack** tracks_2s, int num_2s, int* indexes, momentum4D* results)
{
  if (tracks_nd == NULL || tracks_1s == NULL || tracks_2s == NULL)
    return -1;
  if (num_nd == 0 && num_1s == 0 && num_2s == 0)
    return -1;
  if (indexes == NULL || results == NULL)
    return -1;
  return -1;
}
RbaseReaction* RbaseReaction::checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s,
                                       int num_1s, TTrack** tracks_2s, int num_2s)
{
  if (tracks_nd == NULL || tracks_1s == NULL || tracks_2s == NULL)
    return NULL;
  if (num_nd == 0 && num_1s == 0 && num_2s == 0)
    return NULL;
  return NULL;
}
vector3D solve(const vector3D& d1, const vector3D& d2, const vector3D& d3, const vector3D& s)
{
  Vector l[3];
  for (int i = 0; i < 3; i++)
    l[i].reDimension(4);
  for (int i = 0; i < 3; i++) {
    l[i].setValue(0, d1[i] / d1.R());
    l[i].setValue(1, d2[i] / d2.R());
    l[i].setValue(2, d3[i] / d3.R());
    l[i].setValue(3, s[i]);
    //       cout<<l[i]<<endl;
  }
  //   cout<<endl;
  for (int i = 0; i < 3; i++)
    if (l[0][i] == 0 && l[1][i] == 0 && l[2][i] == 0)
      return vector3D();
  Vector* ll[3] = {NULL, NULL, NULL};
  for (int i = 0; i < 3; i++)
    ll[i] = &l[i];
  if (ll[0]->getValue(0) == 0 || ll[1]->getValue(0) == 0) {
    if (ll[0]->getValue(0) == 0) {
      if (ll[2]->getValue(0) == 0) {
        Vector* t = ll[0];
        ll[0] = ll[1];
        ll[1] = t;
      } else {
        Vector* t = ll[0];
        ll[0] = ll[1];
        ll[1] = ll[2];
        ll[2] = t;
      }
    } else {
      if (ll[2]->getValue(0) != 0) {
        Vector* t = ll[1];
        ll[1] = ll[2];
        ll[2] = t;
      }
    }
  }
  if (ll[1]->getValue(0) == 0 && ll[2]->getValue(0) == 0) {
    if (ll[1]->getValue(1) == 0) {
      Vector* t = ll[1];
      ll[1] = ll[2];
      ll[2] = t;
    }
  }
  if (ll[1]->getValue(0) != 0)
    (*ll[1]) = (*ll[1]) - (*ll[0]) * (ll[1]->getValue(0) / ll[0]->getValue(0));
  if (ll[2]->getValue(0) != 0)
    (*ll[2]) = (*ll[2]) - (*ll[0]) * (ll[2]->getValue(0) / ll[0]->getValue(0));
  if (ll[2]->getValue(1) != 0)
    (*ll[2]) = (*ll[2]) - (*ll[1]) * (ll[2]->getValue(1) / ll[1]->getValue(1));
  vector3D ret(0, 0, 0);
  ret.setValue(2, ll[2]->getValue(3) / ll[2]->getValue(2));
  ret.setValue(1, (ll[1]->getValue(3) - ret[2] * ll[1]->getValue(2)) / ll[1]->getValue(1));
  ret.setValue(0, (ll[0]->getValue(3) - ret[2] * ll[0]->getValue(2) - ret[1] * ll[0]->getValue(1)) /
                      ll[0]->getValue(0));
  return ret;
}
