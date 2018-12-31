#include "Rpp_dpi+.h"
Rpp_dpiPlus::Rpp_dpiPlus() : RbaseReaction(NULL, 2, 2)
{
  fname = "Rpp_dpiPlus";
  if (!Eparticles::IsInit())
    Eparticles::init();
  numberOfParticles = 2;
  particles = new momentum4D[getNumberOfSecondaries()];
  cmparticles = new momentum4D[getNumberOfSecondaries()];
  particleIDs = new int[2];
  particleIDs[0] = 45;
  particleIDs[1] = 8;
  coplanarity = 0.05;
  angleLimit = 1.25;
  crossover = 0.6;
}
Rpp_dpiPlus::Rpp_dpiPlus(AFitAlgorithm* kinFitIn, momentum4D p1, momentum4D p2)
    : RbaseReaction(kinFitIn, 2, 2)
{
  fname = "Rpp_dpiPlus";
  if (!Eparticles::IsInit())
    Eparticles::init();
  numberOfParticles = 2;
  particles = new momentum4D[getNumberOfSecondaries()];
  cmparticles = new momentum4D[getNumberOfSecondaries()];
  momentum4D p[2];
  p[0] = p1;
  p[1] = p2;
  particles[0].setPM(vector3D(0, 0, 0), Eparticles::getMassG(45));
  particles[1].setPM(vector3D(0, 0, 0), Eparticles::getMassG(7));
  particleIDs = new int[2];
  particleIDs[0] = 45;
  particleIDs[1] = 8;
  setInitParticles(2, p);
  coplanarity = 0.05;
  angleLimit = 1.25;
  crossover = 0.6;
}
Rpp_dpiPlus::Rpp_dpiPlus(const Rpp_dpiPlus& rIn) : RbaseReaction(rIn)
{
  fname = "Rpp_dpiPlus";
  if (!Eparticles::IsInit())
    Eparticles::init();
  particles = new momentum4D[rIn.getNumberOfSecondaries()];
  cmparticles = new momentum4D[rIn.getNumberOfSecondaries()];
  momentum4D p[2];
  p[0] = rIn.getInitParticle(0);
  p[1] = rIn.getInitParticle(1);
  particleIDs = new int[2];
  particleIDs[0] = 45;
  particleIDs[1] = 8;
  setInitParticles(2, p);
  coplanarity = 0.05;
  angleLimit = 1.25;
  crossover = 0.6;
}

Rpp_dpiPlus::~Rpp_dpiPlus() { delete[] cmparticles; }
int Rpp_dpiPlus::getNumberOfPrimaries() const { return 2; }
int Rpp_dpiPlus::getNumberOfSecondaries() const { return 2; }
void Rpp_dpiPlus::setCoplanarityDifference(double value) { coplanarity = value; }
void Rpp_dpiPlus::setAngleLimit(double value) { angleLimit = value; }
void Rpp_dpiPlus::setCrossoverMomentum(double value) { crossover = value; }
void Rpp_dpiPlus::setInitParticles(int num, momentum4D* p)
{
  RbaseReaction::setInitParticles(num, p);
  double s = 0, m3 = Eparticles::getMassG(particleIDs[0]),
         m4 = Eparticles::getMassG(particleIDs[1]);
  s = (p[0] + p[1]) * (p[0] + p[1]);
  excessEnergy = sqrt(s) - m3 - m4;
  double p34 = (s - m3 * m3 - m4 * m4);
  p34 = sqrt(p34 * p34 - 4.0 * m3 * m4) / (2.0 * sqrt(s));
  double b3 = p34 / sqrt(p34 * p34 + m3 * m3);
  double b4 = p34 / sqrt(p34 * p34 + m4 * m4);
  double g3 = cm.Beta() / b3, g4 = cm.Beta() / b4;
  //   cout<<"s="<<s<<" ,";
  //   cout<<"p34="<<p34<<" ,";
  //   cout<<"beta_deuteron_cm="<<b3<<" ,";
  //   cout<<"beta_pion_cm=    "<<b4<<" ,"<<endl;
  //   cout<<"beta_cm=         "<<cm.Beta()<<" "<<cm.Velocity().R()<<" ,";
  //   cout<<"gs: "<<g3<<" "<<g4<<endl;
  double gamma = 1.0 / sqrt(1.0 - cm.Beta() * cm.Beta());
  thetaDmax =
      (g3 < 1.0 ? M_PI : (g3 == 1.0 ? M_PI / 2.0 : atan(1.0 / sqrt(g3 * g3 - 1.0) / gamma)));
  thetaPimax =
      (g4 < 1.0 ? M_PI : (g4 == 1.0 ? M_PI / 2.0 : atan(1.0 / sqrt(g4 * g4 - 1.0) / gamma)));
  //  cout<<"theta_deuteron_max: "<<thetaDmax<<" , theta_pion_max:
  //  "<<thetaPimax<<endl;
}

// RbaseReaction* Rpp_dpiPlus::checkNew(TTrack **tracks,int num)
// {
//   if(num<2)return NULL;
//   int index[2];
//   float chi1=justCheck(tracks,num,index);
//   if(chi1>=getChiLimit()||chi1<0)
//     {
//       return NULL;
//     }
//   RbaseReaction *ret=new
//   Rpp_dpiPlus(fkinFit,initParticles[0],initParticles[1]);
//   ret->setChiLimit(chiLimit);
//   ((Rpp_dpiPlus*)ret)->setCoplanarityDifference(coplanarity);
//   ((Rpp_dpiPlus*)ret)->setAngleLimit(angleLimit);
//   ((Rpp_dpiPlus*)ret)->setCrossoverMomentum(crossover);
//   TTrack **tracks2=new TTrack*[2];
//   tracks2[0]=tracks[index[0]];
//   tracks2[1]=tracks[index[1]];
//   ret->check(tracks2,2);
//   delete []tracks2;
//   return ret;
// }

RbaseReaction* Rpp_dpiPlus::getClone()
{
  RbaseReaction* ret = new Rpp_dpiPlus(fkinFit, initParticles[0], initParticles[1]);
  ret->setChiLimit(chiLimit);
  ((Rpp_dpiPlus*)ret)->setCoplanarityDifference(coplanarity);
  ((Rpp_dpiPlus*)ret)->setAngleLimit(angleLimit);
  ((Rpp_dpiPlus*)ret)->setCrossoverMomentum(crossover);
  return ret; // new Rpp_dpiPlus(*this);
}
// float Rpp_dpiPlus::check(TTrack **tracksIn, int num)
// {
//   bool useTime=false;
//   int indexes[2];
//   float bestChi=justCheck(tracksIn, num, indexes);
//   if(bestChi<chiLimit)
//     {
//       int i,j;
//       i=indexes[0];
//       j=indexes[1];
//       momentum4D d,pi;
//       vector3D dir1,dir2;
//       float sp1,sp2;
//       dir1=tracksIn[i]->getPath().Direction();
//       dir2=tracksIn[j]->getPath().Direction();
//       sp1=tracksIn[i]->getSpeed();
//       sp2=tracksIn[j]->getSpeed();
//       if(!useTime)
// 	{
// 	  float pt,pl1,pl2,p1,p2;
// 	  float x1,x2,y1,y2,r1,r2;
// 	  y1=tan(dir1.Theta())*cos(dir1.Phi());
// 	  x1=tan(dir1.Theta())*sin(dir1.Phi());
// 	  y2=tan(dir2.Theta())*cos(dir2.Phi());
// 	  x2=tan(dir2.Theta())*sin(dir2.Phi());

// 	  r1=sqrt(x1*x1+y1*y1);
// 	  r2=sqrt(x2*x2+y2*y2);

// 	  pt= initParticles[0].Momentum().R()*r1*r2/(r1+r2);
// 	  pl1=initParticles[0].Momentum().R()*r2/(r1+r2);
// 	  pl2=initParticles[0].Momentum().R()*r1/(r1+r2);
// 	  p1=sqrt(pt*pt+pl1*pl1);
// 	  p2=sqrt(pt*pt+pl2*pl2);
// 	  d.setPM(dir1*p1,Eparticles::getMassG(particleIDs[0]));
// 	  pi.setPM(dir2*p2,Eparticles::getMassG(particleIDs[1]));
// 	}
//       else
// 	{
// 	  d.setVM(dir1*sp1,Eparticles::getMassG(particleIDs[0]));
// 	  pi.setVM(dir2*sp2,Eparticles::getMassG(particleIDs[1]));
// 	}
//       particles[0]=d;
//       particles[1]=pi;
//       chi2=bestChi;
//       calculateCM();
//       setTrack(0,tracksIn[i]);
//       setTrack(1,tracksIn[j]);
//     }
//   return bestChi;
// }
// float Rpp_dpiPlus::justCheck(TTrack **tracksIn, int num, int *indexes,
// momentum4D *results)
// {
//   bool useTime=false;
//   int Npri=0;
//   for(int i=0;i<num;i++)
//     if(!tracksIn[i]->isSecondary()&&tracksIn[i]->getCharge()!=0)
//       Npri++;
//   if(Npri<2)return 1000;
//   //  if(Npri>2)return 1000;
//   int prims[Npri];
//   Npri=0;
//   for(int i=0;i<num;i++)
//     if(!tracksIn[i]->isSecondary()&&tracksIn[i]->getCharge()!=0)
//       prims[Npri++]=i;
//   float bestChi=1000*chiLimit;
//   int best=-1;
//   momentum4D d1,d2,pi1,pi2;
//   vector3D dir1,dir2;
//   float sp1,sp2;
//   matrixNxM A(3,2);
//   Vector pi(3);
//   Vector px(2);
//   pi.transpose();
//   px.transpose();
//   momentum4D pI=initParticles[0]+initParticles[1];
//   pi.setValues(pI.X(),pI.Y(),pI.Z());
//   //  float p_l_min_d=1.0,p_l_max_pi=2.0;
// //   float thetaPi11,thetaPi12,thetaPi21,thetaPi22;
//   /*assuming one track as deuteron, there are two different possible angles
//     for the corresponding pion:
//     S=pI*pI
//     znen=gamma*(cm.Beta()/beta_pi_cm-cos(theta_d_cm))
//     p_d_cm=sqrt((S-m_d*m_d-m_pi*m_pi)*(S-m_d*m_d-m_pi*m_pi)-4*m_d*m_d*m_pi*m_pi)/(2*sqrt(s))
//     beta_pi_cm=p_d_cm/sqrt(p_d_cm*p_d_cm+m_pi*m_pi)

//     p_d_lab_1=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)+sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)))
//     p_d_lab_2=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)-sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)))
//     theta_d_cm=abs(asin(p_d_lab*sin(theta_d)/p_d_cm))
//     thetaPi1=atan(sin(theta_d_cm)/znen)
//    */
// //   float s=pI*pI;
//   float m_d=Eparticles::getMassG(particleIDs[0]);
//   float m_pi=Eparticles::getMassG(particleIDs[1]);
// //   float
// p_d_cm=sqrt((s-m_d*m_d-m_pi*m_pi)*(s-m_d*m_d-m_pi*m_pi)-4*m_d*m_d*m_pi*m_pi)/(2*sqrt(s));
// //   float beta_pi_cm=p_d_cm/sqrt(p_d_cm*p_d_cm+m_pi*m_pi);
// //   float beta_d_cm=p_d_cm/sqrt(p_d_cm*p_d_cm+m_d*m_d);
// //   float gamma=1.0/sqrt(1.0-cm.Beta()*cm.Beta());
// //   float znen11,znen12,znen21,znen22,theta_d;
// //   float thetaDcm11,thetaDcm12,thetaDcm21,thetaDcm22;
// //   float p_d_lab_11,p_d_lab_12,p_d_lab_21,p_d_lab_22;
//   float pt,pl1,pl2,p1,p2;
//   float x1,x2,y1,y2,r1,r2;
//   float ebeam=initParticles[0].Energy()+initParticles[1].Energy();
//   float etrans;
//   float chiq=1000;
//   for(int i=0;i<Npri;i++)
//     for(int j=0;j<Npri;j++)
//       {
// 	if(i==j)continue;
// 	dir1=tracksIn[i]->getPath().Direction();
// 	dir2=tracksIn[j]->getPath().Direction();
// 	if(fabs(fabs(dir1.Phi()-dir2.Phi())-M_PI)>coplanarity)continue;
// 	if(dir1.Theta()+dir2.Theta()>angleLimit)continue;
// 	if(dir1.Theta()>thetaDmax*1.1)continue;
// 	if(dir2.Theta()>thetaPimax*1.1)continue;
// // 	theta_d=dir1.Theta();
// //
// p_d_lab_11=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)+sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)));
// //
// p_d_lab_12=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)-sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)));
// // 	thetaDcm11=abs(asin(p_d_lab_11*sin(theta_d)/p_d_cm));
// // 	thetaDcm12=abs(asin(p_d_lab_12*sin(theta_d)/p_d_cm));
// // 	znen11=gamma*(cm.Beta()/beta_pi_cm-cos(thetaDcm11));
// // 	znen12=gamma*(cm.Beta()/beta_pi_cm-cos(thetaDcm12));
// // 	theta_d=dir2.Theta();
// //
// p_d_lab_21=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)+sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)));
// //
// p_d_lab_22=m_d*(cm.Beta()*cm.Beta()*cos(theta_d)-sqrt(beta_d_cm*beta_d_cm*gamma*gamma-cm.Beta()*cm.Beta()*gamma*gamma*sin(theta_d)))/(gamma*(1-cm.Beta()*cm.Beta()*cos(theta_d)*cos(theta_d)));
// // 	thetaDcm21=abs(asin(p_d_lab_21*sin(theta_d)/p_d_cm));
// // 	thetaDcm22=abs(asin(p_d_lab_22*sin(theta_d)/p_d_cm));
// // 	znen21=gamma*(cm.Beta()/beta_pi_cm-cos(thetaDcm21));
// // 	znen22=gamma*(cm.Beta()/beta_pi_cm-cos(thetaDcm22));

// // 	thetaPi11=atan(sin(thetaDcm11)/znen11);
// // 	thetaPi12=atan(sin(thetaDcm12)/znen12);
// // 	thetaPi21=atan(sin(thetaDcm21)/znen21);
// // 	thetaPi22=atan(sin(thetaDcm22)/znen22);

// 	sp1=tracksIn[i]->getSpeed();
// 	sp2=tracksIn[j]->getSpeed();
// 	if(!useTime)
// 	  {
// 	    y1=tan(dir1.Theta())*cos(dir1.Phi());
// 	    x1=tan(dir1.Theta())*sin(dir1.Phi());
// 	    y2=tan(dir2.Theta())*cos(dir2.Phi());
// 	    x2=tan(dir2.Theta())*sin(dir2.Phi());

// 	    r1=sqrt(x1*x1+y1*y1);
// 	    r2=sqrt(x2*x2+y2*y2);

// 	    pt= initParticles[0].Momentum().R()*r1*r2/(r1+r2);
// 	    pl1=initParticles[0].Momentum().R()*r2/(r1+r2);
// 	    pl2=initParticles[0].Momentum().R()*r1/(r1+r2);
// 	    p1=sqrt(pt*pt+pl1*pl1);
// 	    p2=sqrt(pt*pt+pl2*pl2);
// 	    etrans=sqrt(p1*p1+m_d*m_d)+sqrt(p2*p2+m_pi*m_pi);
// 	    if(pt>crossover&&dir2.Theta()<dir1.Theta())continue;
// 	    if(pt<crossover&&dir2.Theta()>dir1.Theta())continue;
// 	    //	    if(abs(etrans-ebeam)>0.2)continue;
// 	    d1.setPM(dir1*p1,m_d);
// 	    pi1.setPM(dir2*p2,m_pi);
// 	    //
// if(abs(sqrt(abs((d1-pI)*(d1-pI)))-Eparticles::getMassG(particleIDs[1]))>0.15)continue;
// 	    //
// if(abs(sqrt(abs((pi1-pI)*(pi1-pI)))-Eparticles::getMassG(particleIDs[0]))>0.2)continue;
// 	  }
// 	else
// 	  {
// 	    d1.setVM(dir1*sp1,m_d);
// 	    pi1.setVM(dir2*sp2,m_pi);
// 	    etrans=d1.Energy()+pi1.Energy();
// 	  }
// 	chiq=abs(ebeam-etrans);
// 	//
// if(abs(dir2.Theta()-thetaPi11)>0.1&&abs(dir2.Theta()-thetaPi12)>0.1)continue;
// 	if(bestChi>chiq)
// 	  {
// 	    bestChi=chiq;
// 	    best=i*Npri+j;
// 	  }
//       }
//   if(chiLimit>bestChi&&indexes!=NULL)
//     {
//       indexes[0]=best/Npri;
//       indexes[1]=best%Npri;
//     }
//   if(results!=NULL)
//     {
//     }
//   return bestChi;
// }
void Rpp_dpiPlus::calculate(int justGeometry)
{
  if (tracks[0] == NULL || tracks[1] == NULL)
    return;
  switch (justGeometry) {
  case 0: {
    momentum4D p1, p2;
    TTrack* tr = (tracks[0]);
    p1.setVM(tr->getPath().Direction() * tr->getSpeed(), Eparticles::getMassG(particleIDs[0]));
    tr = (tracks[1]);
    p2.setVM(tr->getPath().Direction() * tr->getSpeed(), Eparticles::getMassG(particleIDs[1]));
    particles[0] = p1;
    particles[1] = p2;
    calculateCM();
    break;
  }
  case 1: {
    vector3D dir1 = (tracks[0])->getPath().Direction(), dir2 = (tracks[1])->getPath().Direction();
    float y1 = tan(dir1.Theta()) * cos(dir1.Phi());
    float x1 = tan(dir1.Theta()) * sin(dir1.Phi());
    float y2 = tan(dir2.Theta()) * cos(dir2.Phi());
    float x2 = tan(dir2.Theta()) * sin(dir2.Phi());

    float r1 = sqrt(x1 * x1 + y1 * y1);
    float r2 = sqrt(x2 * x2 + y2 * y2);

    float pt = initParticles[0].Momentum().R() * r1 * r2 / (r1 + r2);
    float pl1 = initParticles[0].Momentum().R() * r2 / (r1 + r2);
    float pl2 = initParticles[0].Momentum().R() * r1 / (r1 + r2);
    float p1 = sqrt(pt * pt + pl1 * pl1);
    float p2 = sqrt(pt * pt + pl2 * pl2);
    particles[0].setPM(dir1 * p1, Eparticles::getMassG(particleIDs[0]));
    particles[1].setPM(dir2 * p2, Eparticles::getMassG(particleIDs[1]));
    calculateCM();
    break;
  }
  }
}
float Rpp_dpiPlus::check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                         TTrack** tracks_2s, int num_2s)
{
  chi2 = 1e+5;
  if ((tracks_nd == NULL && tracks_1s == NULL && tracks_2s == NULL) || num_nd + num_1s + num_2s < 2)
    return chi2;
  int inde[2] = {-1, -1};
  float chi = justCheck(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s, inde, NULL);
  for (int i = 0; i < 2; i++)
    if (inde[i] < 0 || inde[i] > num_nd + num_1s + num_2s)
      return chi2;
    else if (inde[i] < num_nd)
      setTrack(i, tracks_nd[inde[i]]);
    else if (inde[i] < num_nd + num_1s)
      setTrack(i, tracks_1s[inde[i] - num_1s]);
    else if (inde[i] < num_nd + num_1s + num_2s)
      setTrack(i, tracks_2s[inde[i] - num_1s - num_2s]);
  if (!(chi < 0 || chi > chiLimit))
    calculate(true);
  chi2 = chi;
  return chi2;
}
float Rpp_dpiPlus::justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                             TTrack** tracks_2s, int num_2s, int* indexes, momentum4D* results)
{
  if (tracks_nd == NULL && tracks_1s == NULL && num_nd + num_1s < 2)
    return 1e+5;
  if (num_2s == 0 || tracks_2s == NULL)
    chi2 = 1e+5;
  int pris[num_nd + num_1s];
  int nPr = 0;
  float theta[num_nd + num_1s];
  vector3D vIPl[num_nd + num_1s];
  momentum4D pI(initParticles[0] + initParticles[1]);
  vector3D z = pI.Momentum();
  z.normalize();
  for (int i = 0; i < num_nd; i++) {
    if (tracks_nd[i] == NULL)
      continue;
    if (tracks_nd[i]->isSecondary())
      continue;
    if (tracks_nd[i]->getCharge() == 0)
      continue;
    if (tracks_nd[i]->getNumberOfSecondaryTracks() > 0)
      continue;
    vIPl[nPr] = tracks_nd[i]->getPath().Direction() - z * (tracks_nd[i]->getPath().Direction() * z);
    theta[nPr] = acos(tracks_nd[i]->getPath().Direction() * z);
    pris[nPr++] = i;
  }
  for (int i = 0; i < num_1s; i++) {
    if (tracks_1s[i] == NULL)
      continue;
    if (tracks_1s[i]->isSecondary())
      continue;
    if (tracks_1s[i]->getCharge() == 0)
      continue;
    vIPl[nPr] = tracks_1s[i]->getPath().Direction() - z * (tracks_1s[i]->getPath().Direction() * z);
    theta[nPr] = acos(tracks_1s[i]->getPath().Direction() * z);
    pris[nPr++] = num_nd + i;
  }
  if (nPr < 2)
    return 1e+5;
  float dphi;
  int best = 0;
  float bchi = 1e+6;
  float pl1, pl2, pt, e11, e21, e12, e22, p1, p2;
  float m1 = Eparticles::getMassG(particleIDs[0]), m2 = Eparticles::getMassG(particleIDs[1]);
  for (int i = 0; i < nPr; i++) {
    for (int j = i + 1; j < nPr; j++) {
      dphi = fabs(acos(vIPl[i] * vIPl[j]) - M_PI);
      if (dphi > coplanarity)
        continue;
      pt = tan(theta[i]) * tan(theta[j]) / (tan(theta[i]) + tan(theta[j]));
      pl1 = tan(theta[j]) / (tan(theta[i]) + tan(theta[j]));
      pl2 = tan(theta[i]) / (tan(theta[i]) + tan(theta[j]));
      p1 = sqrt(pt * pt + pl1 * pl1);
      p2 = sqrt(pt * pt + pl2 * pl2);
      e11 = sqrt(p1 * p1 + m1 * m1);
      e22 = sqrt(p2 * p2 + m2 * m2);
      e12 = sqrt(p2 * p2 + m1 * m1);
      e21 = sqrt(p1 * p1 + m2 * m2);
      if (abs(e11 + e22 - pI.Energy()) < abs(e12 + e21 - pI.Energy())) {
        if (bchi > abs(e11 + e22 - pI.Energy())) {
          bchi = abs(e11 + e22 - pI.Energy());
          best = i * nPr + j;
        }
      } else {
        if (bchi > abs(e21 + e12 - pI.Energy())) {
          bchi = abs(e21 + e12 - pI.Energy());
          best = j * nPr + i;
        }
      }
    }
  }
  if (bchi < chiLimit) {
    if (indexes != NULL) {
      indexes[0] = best / nPr;
      indexes[1] = best % nPr;
    }
    if (results != NULL) {
      int i = best / nPr, j = best % nPr;
      pt = tan(theta[i]) * tan(theta[j]) / (tan(theta[i]) + tan(theta[j]));
      pl1 = tan(theta[j]) / (tan(theta[i]) + tan(theta[j]));
      pl2 = tan(theta[i]) / (tan(theta[i]) + tan(theta[j]));
      p1 = sqrt(pt * pt + pl1 * pl1);
      p2 = sqrt(pt * pt + pl2 * pl2);
      e11 = sqrt(p1 * p1 + m1 * m1);
      e22 = sqrt(p2 * p2 + m2 * m2);
      if (pris[i] < num_nd)
        results[0].setPE(p1 * tracks_nd[pris[i]]->getPath().Direction(), e11);
      else
        results[0].setPE(p1 * tracks_1s[pris[i] - num_nd]->getPath().Direction(), e11);
      if (pris[j] < num_nd)
        results[1].setPE(p2 * tracks_nd[pris[j]]->getPath().Direction(), e22);
      else
        results[1].setPE(p2 * tracks_1s[pris[j] - num_nd]->getPath().Direction(), e22);
    }
    return bchi;
  }
  return 1e+5;
}
RbaseReaction* Rpp_dpiPlus::checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                                     TTrack** tracks_2s, int num_2s)
{
  if ((tracks_nd == NULL && tracks_1s == NULL && tracks_2s == NULL) || num_nd + num_1s + num_2s < 2)
    return NULL;
  float chi = justCheck(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s, NULL, NULL);
  if (chi < 0 || chi > chiLimit)
    return NULL;
  RbaseReaction* ret = this->getClone();
  ret->check(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s);
  return ret;
}
