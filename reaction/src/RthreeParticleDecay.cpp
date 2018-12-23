#include "RthreeParticleDecay.h"


RthreeParticleDecay::RthreeParticleDecay():TBase("RthreeParticleDecay")
{
  fname="RthreeParticleDecay";
}
RthreeParticleDecay::RthreeParticleDecay(const RthreeParticleDecay& dec):TBase("RthreeParticleDecay")
{
  fname="RthreeParticleDecay";
  setMasses(dec.mass(-1),dec.mass(0),dec.mass(1),dec.mass(2));
}
RthreeParticleDecay::RthreeParticleDecay(double massI, double massO1, double massO2, double massO3):TBase("RthreeParticleDecay")
{
  fname="RthreeParticleDecay";
  setMasses(massI,massO1,massO2,massO3);
}
bool RthreeParticleDecay::isPossible()const
{
  return possible;
}
void RthreeParticleDecay::setMasses(double *massIn)//has to have 4 components!!!
{
  setMasses(massIn[0],massIn[1],massIn[2],massIn[3]);
}
void RthreeParticleDecay::setMasses(double massI, double massO1, double massO2, double massO3)
{
  masses[0]=massI;
  masses[1]=massO1;
  masses[2]=massO2;
  masses[3]=massO3;
}
double RthreeParticleDecay::mass(int particle)const//particle =-1 for InitParticle, 0, 1, 2, for decay particles
{
  if(particle<-1||particle>2)return -1;
  return masses[particle+1];
}
double RthreeParticleDecay::pInitLab()const
{
  return mom_lab[-1];
}
double RthreeParticleDecay::betaInitLab()const
{
  return beta_lab[-1];
}
double RthreeParticleDecay::eInitLab()const
{
  return E_lab[-1];
}
double RthreeParticleDecay::pLab(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return mom_lab[particle+1];
}
double RthreeParticleDecay::betaLab(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return beta_lab[particle+1];
}
double RthreeParticleDecay::eLab(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return E_lab[particle+1];
}
double RthreeParticleDecay::pCM(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return mom_cm[particle+1];
}
double RthreeParticleDecay::betaCM(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return beta_cm[particle+1];
}
double RthreeParticleDecay::eCM(int particle)const
{
  if(particle<-1||particle>2)return -1;
  return E_cm[particle+1];
}
momentum4D RthreeParticleDecay::PLab(int particle)const
{
  if(particle<-1||particle>2)return momentum4D();
  return P_lab[particle+1];
}
momentum4D RthreeParticleDecay::PCM(int particle)const
{
  if(particle<-1||particle>2)return momentum4D();
  return P_cm[particle+1];
}
void RthreeParticleDecay::check(vector3D dirInit, vector3D dir1, vector3D dir2, vector3D dir3, double value, int type)//calculate properties of decay. Three particle decay needs the three direction vectors and an additional value, such as Momentum (t==1), Energy (t==2), KineticE (t==3), beta (t==4), with type==t*nParticles+(ParticleIdentifier+1)
{
  if(dirInit==dir1)return;
  if(dir3==dir2)return;
  if(value==0)return;
  if(type==0)return;
}
