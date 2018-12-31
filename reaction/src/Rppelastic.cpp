#include "Rppelastic.h"
//#include <TH2.h>
//#include <TTree.h>
// TH2F histo("speedHisto","speedHisto",200,0.6,1,400,0,0.4);
// TH2F referenceThB("\\theta vs. \\beta_{ref}","\\theta vs.
// \\beta_{ref}",60,0,60,200,0.6,1);
// TH2F ThB("\\theta vs. \\beta","\\theta vs. \\beta",60,0,60,200,0,0.4);
// TH2F ringTof("R:tdcfac vs \\theta","R:tdcfac vs
// \\theta",100,10,26,200,-100,0);
// TH2F barrelTof("B:tdcfac vs \\theta","B:tdcfac vs
// \\theta",100,20,60,200,-100,0);
// TH2F ringTofref("R:tdcfac_{ref} vs \\theta","R:tdcfac_{ref} vs
// \\theta",100,10,26,200,-100,0);
// TH2F barrelTofref("B:tdcfac_{ref} vs \\theta","B:tdcfac_{ref} vs
// \\theta",100,20,60,200,-100,0);
// TTree outTree("Speed","Speed");
// Double_t beta_m;
// Double_t beta_r;
// Double_t theta;
// Int_t el[3];
// Int_t det[3];
// Double_t pathlength_C;

// void fillRingHisto(TTrack *t1, double b1)
// {
//   if((t1->getNumberOfCalibHits(3)+t1->getNumberOfCalibHits(4)+t1->getNumberOfCalibHits(5))<=0||(t1->getNumberOfCalibHits(15)+t1->getNumberOfCalibHits(16))<=0)
//     return;
//   sLine3D path=t1->getPath();
//   TCalibHit *r1=NULL, *r2=NULL, *r3=NULL;
//   TCalibHit *s1=NULL,*s2=NULL;
//   point3D sto1,sto2,sto3,sta1,sta2;
//   r1=t1->getCalibSrcr(3,0);
//   r2=t1->getCalibSrcr(4,0);
//   r3=t1->getCalibSrcr(5,0);
//   s1=t1->getCalibSrcr(15,0);
//   s2=t1->getCalibSrcr(16,0);
//   if(r1!=NULL)
//     sto1=r1->getHitShape()->entrance(path);
//   if(r2!=NULL)//->isValid())
//     sto2=r2->getHitShape()->entrance(path);
//   if(r3!=NULL)//->isValid())
//     sto3=r3->getHitShape()->entrance(path);
//   if(s1!=NULL)//->isValid())
//     sta1=s1->getHitShape()->entrance(path);
//   if(s2!=NULL)//->isValid())
//     sta2=s2->getHitShape()->entrance(path);
//   double meanStart=0;
//   double meanStop=0;
//   point3D startHit, stopHit;
//   if(s1!=NULL/*->isValid()*/&&s2!=NULL)//->isValid())
//     {
//       meanStart=(s1->getTDC()+s2->getTDC())*0.5;
//       startHit=sta1+(sta2-sta1)*0.5;
//     }
//   else if(s1!=NULL)//->isValid())
//     {
//       meanStart=s1->getTDC();
//       startHit=sta1;
//     }
//   else if(s2!=NULL)//->isValid())
//     {
//       meanStart=s2->getTDC();
//       startHit=sta2;
//     }
//   int pixpat=0;
//   if(r1!=NULL)//->isValid())
//     pixpat=pixpat+1;
//   if(r2!=NULL)//->isValid())
//     pixpat=pixpat+2;
//   if(r3!=NULL)//->isValid())
//     pixpat=pixpat+4;
//   for(int i=0;i<3;i++)
//     {
//       el[i]=-1;
//       det[i]=-1;
//     }
//   switch(pixpat)
//     {
//     case 1:
//       meanStop=r1->getTDC();
//       stopHit=sto1;
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//       break;
//     case 2:
//       meanStop=r2->getTDC();
//       stopHit=sto2;
//       el[0]=r2->getElement();
//       det[0]=r2->getDetector();
//       break;
//     case 3:
//       meanStop=(r1->getTDC()+r2->getTDC())*0.5;
//       stopHit=sto1+(sto2-sto1)*0.5;
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//       el[1]=r2->getElement();
//       det[1]=r2->getDetector();
//       break;
//     case 4:
//       meanStop=r3->getTDC();
//       stopHit=sto3;
//       el[0]=r3->getElement();
//       det[0]=r3->getDetector();
//       break;
//     case 5:
//       meanStop=(r1->getTDC()+r3->getTDC())*0.5;
//       stopHit=sto1+(sto3-sto1)*0.5;
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//       el[1]=r3->getElement();
//       det[1]=r3->getDetector();
//       break;
//     case 6:
//       meanStop=(r2->getTDC()+r3->getTDC())*0.5;
//       stopHit=sto3+(sto2-sto3)*0.5;
//       el[0]=r2->getElement();
//       det[0]=r2->getDetector();
//       el[1]=r3->getElement();
//       det[1]=r3->getDetector();
//       break;
//     case 7:
//       meanStop=(r1->getTDC()+r2->getTDC()+r3->getTDC())/3.0;
//       stopHit=point3D(0,0,0)+(vector3D(sto1)+vector3D(sto2)+vector3D(sto1))*(1/3.0);
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//       el[1]=r2->getElement();
//       det[1]=r2->getDetector();
//       el[2]=r3->getElement();
//       det[2]=r3->getDetector();
//       break;
//     }
//   double pathlength=(stopHit-startHit).length();
//   double convertedTDC=(1.0/(b1*300.0/pathlength))+meanStart;
//   ringTof.Fill(path.Direction().Theta()*180/M_PI,meanStop);
//   ringTofref.Fill(path.Direction().Theta()*180/M_PI,convertedTDC);
//   beta_m=(pathlength/abs(meanStart-meanStop)/300.0);//t1->getSpeed();
//   beta_r=b1;
//   theta=t1->getPath().Direction().Theta();
//   pathlength_C=pathlength;
//   outTree.Fill();
// }
// void fillBarrelHisto(TTrack *t1, double b1)
// {
//   if((t1->getNumberOfCalibHits(6)+t1->getNumberOfCalibHits(7))<=0||(t1->getNumberOfCalibHits(15)+t1->getNumberOfCalibHits(16))<=0)
//     return;
//   sLine3D path=t1->getPath();
//   TCalibHit *r1=NULL, *r2=NULL;
//   TCalibHit *s1=NULL,*s2=NULL;
//   point3D sto1,sto2,sto3,sta1,sta2;
//   r1=t1->getCalibSrcr(6,0);
//   r2=t1->getCalibSrcr(7,0);
//   s1=t1->getCalibSrcr(15,0);
//   s2=t1->getCalibSrcr(16,0);
//   if(r1!=NULL)//->isValid())
//     sto1=r1->getHitShape()->entrance(path);
//   if(r2!=NULL)//->isValid())
//     sto2=r2->getHitShape()->entrance(path);
//   if(s1!=NULL)//->isValid())
//     sta1=s1->getHitShape()->entrance(path);
//   if(s2!=NULL)//->isValid())
//     sta2=s2->getHitShape()->entrance(path);
//   double meanStart=0;
//   double meanStop=0;
//   point3D startHit, stopHit;
//   if(s1!=NULL/*->isValid()*/&&s2!=NULL)//->isValid())
//     {
//       meanStart=(s1->getTDC()+s2->getTDC())*0.5;
//       startHit=sta1+(sta2-sta1)*0.5;
//     }
//   else if(s1!=NULL)//->isValid())
//     {
//       meanStart=s1->getTDC();
//       startHit=sta1;
//     }
//   else if(s2!=NULL)//->isValid())
//     {
//       meanStart=s2->getTDC();
//       startHit=sta2;
//     }
//   for(int i=0;i<3;i++)
//     {
//       el[i]=-1;
//       det[i]=-1;
//     }
//   if(r1!=NULL/*->isValid()*/&&r2!=NULL)//->isValid())
//     {
//       meanStop=(r1->getTDC()+r2->getTDC())*0.5;
//       stopHit=sto1+(sto2-sto1)*0.5;
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//       el[1]=r2->getElement();
//       det[1]=r2->getDetector();
//     }
//   else if(r1!=NULL)//->isValid())
//     {
//       meanStop=r1->getTDC();
//       stopHit=sto1;
//       el[0]=r1->getElement();
//       det[0]=r1->getDetector();
//     }
//   else if(r2!=NULL)//->isValid())
//     {
//       meanStop=r2->getTDC();
//       stopHit=sto2;
//       el[0]=r2->getElement();
//       det[0]=r2->getDetector();
//     }
//   double pathlength=(stopHit-startHit).length();
//   double convertedTDC=(1.0/(b1*300.0/pathlength))+meanStart;
//   barrelTof.Fill(path.Direction().Theta()*180/M_PI,meanStop);
//   barrelTofref.Fill(path.Direction().Theta()*180/M_PI,convertedTDC);
//   beta_m=(pathlength/abs(meanStart-meanStop)/300.0);//t1->getSpeed();
//   beta_r=b1;
//   theta=t1->getPath().Direction().Theta();
//   pathlength_C=pathlength;
//   outTree.Fill();
// }
Rppelastic::Rppelastic() : RbaseReaction(NULL, 2, 2)
{
  fname = "Rppelastic";
  if (!Eparticles::IsInit())
    Eparticles::init();
  numberOfParticles = 2;
  particles = new momentum4D[getNumberOfSecondaries()];
  cmparticles = new momentum4D[getNumberOfSecondaries()];
  particleIDs = new int[2];
  particleIDs[0] = 14;
  particleIDs[1] = 14;
}
Rppelastic::Rppelastic(AFitAlgorithm* kinFitIn, momentum4D p1, momentum4D p2)
    : RbaseReaction(kinFitIn, 2, 2)
{
  fname = "Rppelastic";
  if (!Eparticles::IsInit())
    Eparticles::init();
  numberOfParticles = 2;
  particles = new momentum4D[getNumberOfSecondaries()];
  cmparticles = new momentum4D[getNumberOfSecondaries()];
  momentum4D p[2];
  p[0] = p1;
  p[1] = p2;
  particles[0].setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
  particles[1].setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
  setInitParticles(2, p);
  particleIDs = new int[2];
  particleIDs[0] = 14;
  particleIDs[1] = 14;
}

Rppelastic::Rppelastic(const Rppelastic& rIn) : RbaseReaction(rIn)
{
  fname = "Rppelastic";
  if (!Eparticles::IsInit())
    Eparticles::init();
  particles = new momentum4D[getNumberOfSecondaries()];
  cmparticles = new momentum4D[getNumberOfSecondaries()];
  momentum4D p[2];
  p[0] = rIn.getInitParticle(0);
  p[1] = rIn.getInitParticle(1);
  setInitParticles(2, p);
  particleIDs = new int[2];
  particleIDs[0] = 14;
  particleIDs[1] = 14;
  GammaDifference = rIn.getGammaDifference();
  CoplanarityDifference = rIn.getCoplanarityDifference();
}

Rppelastic::~Rppelastic() { delete[] cmparticles; }

int Rppelastic::getNumberOfPrimaries() const { return 2; }
int Rppelastic::getNumberOfSecondaries() const { return 2; }
// float Rppelastic::check(TTrack **tracksIn, int num)
// {
//   if(num<2) return -1;
//   int index[2];
//   float ret=justCheck(tracksIn,num,index,NULL);
//   if(index[0]==index[1]||index[0]>=num||index[1]>=num||index[0]<0||index[1]<0)
//     return -1;
//   if(ret<=chiLimit)
//     {
//       float mass=Eparticles::getMassG(14);
//       vector3D d1(tracksIn[index[0]]->getPath().Direction()),
// 	d2(tracksIn[index[1]]->getPath().Direction());
//       if(!useTiming)
// 	{
// // 	  momentum4D pI(0,0,0,0);
// // 	  for(int i=0;i<numberOfInitParticles;i++)
// // 	    pI=pI+initParticles[i];
// 	  float pt=
// initParticles[0].Momentum().R()*tan(d1.Theta())*tan(d2.Theta())/(tan(d1.Theta())+tan(d2.Theta()));
// 	  float
// pl1=initParticles[0].Momentum().R()*tan(d2.Theta())/(tan(d1.Theta())+tan(d2.Theta()));
// 	  float
// pl2=initParticles[0].Momentum().R()*tan(d1.Theta())/(tan(d1.Theta())+tan(d2.Theta()));
// 	  float p1=sqrt(pt*pt+pl1*pl1);
// 	  float p2=sqrt(pt*pt+pl2*pl2);

// 	  particles[0].setPM(d1*p1,mass);
// 	  particles[1].setPM(d2*p2,mass);
// 	}
//       else
// 	{
// 	  particles[0].setVM(d1*tracksIn[index[0]]->getSpeed(),mass);
// 	  particles[1].setVM(d2*tracksIn[index[1]]->getSpeed(),mass);
// 	}
//       chi2=ret;
//       setTrack(0,tracksIn[index[0]]);
//       setTrack(1,tracksIn[index[1]]);
// //
// histo.Fill(particles[0].Velocity().length(),tracksIn[com1[best]]->getSpeed());
// //
// histo.Fill(particles[1].Velocity().length(),tracksIn[com2[best]]->getSpeed());
// //
// referenceThB.Fill(particles[0].Momentum().Theta()*108/M_PI,particles[0].Velocity().length());
// //
// ThB.Fill(particles[0].Momentum().Theta()*108/M_PI,tracksIn[com1[best]]->getSpeed());
// //
// referenceThB.Fill(particles[1].Momentum().Theta()*108/M_PI,particles[1].Velocity().length());
// //
// ThB.Fill(particles[1].Momentum().Theta()*108/M_PI,tracksIn[com2[best]]->getSpeed());
// //       if(tracksIn[com1[best]]->getNumberOfCalibHits(6)==0)
// // 	fillRingHisto(tracksIn[com1[best]], particles[0].Velocity().length());
// //       else
// // 	fillBarrelHisto(tracksIn[com1[best]], particles[0].Velocity().length());
// //       if(tracksIn[com2[best]]->getNumberOfCalibHits(6)==0)
// // 	fillRingHisto(tracksIn[com2[best]], particles[1].Velocity().length());
// //       else
// // 	fillBarrelHisto(tracksIn[com2[best]], particles[1].Velocity().length());

//     }
//   return ret;
// }
// float Rppelastic::justCheck(TTrack **tracksIn, int num, int *indexes,
// momentum4D *results)
// {
//   if(num<2) return 10000;
//   double gamma2=(1-cm.Beta()*cm.Beta());
//   int nPri=0;
//   float speeds[num];
//   vector3D dirs[num];
//   for(int i=0;i<num;i++)
//     {
//       if(tracksIn[i]==NULL)continue;
//       if(tracksIn[i]->isSecondary()||tracksIn[i]->getCharge()==0||tracksIn[i]->getNumberOfSecondaryTracks()>0)continue;
//       nPri++;
//       speeds[i]=tracksIn[i]->getSpeed();
//       dirs[i]=tracksIn[i]->getPath().Direction();
//       dirs[i].normalize();
//       dirs[i].setRep(_spheric_);
//       if(speeds[i]>=0.98)speeds[i]=0.98;
//     }
//   if(nPri<2)return 10000;
//   int pris[nPri];
//   float chis[nPri*nPri];
//   for(int i=0;i<nPri*nPri;i++)chis[i]=10000;
//   nPri=0;
//   for(int i=0;i<num;i++)
//     {
//       if(tracksIn[i]==NULL)continue;
//       if(tracksIn[i]->isSecondary()||tracksIn[i]->getCharge()==0||tracksIn[i]->getNumberOfSecondaryTracks()>0)continue;
//       pris[nPri]=i;      nPri++;
//     }
//   if(nPri<2)return -1;
//   momentum4D p1,p2,p12,pI,pR;
//   pI.setValues(0,0,0,0);
//   //  cout<<pI<<endl;
//   for(int i=0;i<numberOfInitParticles;i++)
//     pI=pI+initParticles[i];
//   pI=initParticles[0]+initParticles[1];
//   vector3D dir;
//   float mass=Eparticles::getMass("proton");
//   double theta1,theta2;
//   vector3D z=pI.Momentum(),dp1,dp2;
//   dp1.setRep(_cylindric_);
//   dp2.setRep(_cylindric_);
//   //cout<<pI<<z<<endl;
//   //  cout<<initParticles[0]<<initParticles[1]<<pI<<z<<endl;
//   z.normalize();
//   float pt;
//   float pl1;
//   float pl2;
//   float px1;
//   float px2;
//   //cout<<CoplanarityDifference<<" "<<GammaDifference<<" "<<z<<endl;
//   for(int i=0;i<nPri;i++)
//     {
//       for(int j=i+1;j<nPri;j++)
// 	{
// 	  chis[i*nPri+j]=10000;
// 	  if(i==j)continue;
// 	  dp1=dirs[pris[i]]-z*(dirs[pris[i]]*z);
// 	  dp2=dirs[pris[j]]-z*(dirs[pris[j]]*z);
// 	  //	  	  cout<<dirs[pris[i]]<<dirs[pris[j]]<<z<<dp1<<dp2<<endl;
// 	  double phid=acos((dp1*dp2)/dp1.R()/dp2.R());
// 	  //  cout<<phid<<dp1<<dp2<<endl;
// 	  phid=fabs(phid-M_PI);
// 	  theta1=acos(dirs[pris[i]]*z/z.R()/dirs[pris[i]].R());
// 	  theta2=acos(dirs[pris[j]]*z/z.R()/dirs[pris[j]].R());
// 	  //	  cout<<phid<<" "<<fabs(tan(theta1)*tan(theta2)-gamma2)<<"
// "<<endl;
// 	  //	  cout<<"\t"<<gamma2<<" "<<tan(theta1)*tan(theta2)<<"
// "<<fabs(tan(theta1)*tan(theta2)-gamma2)<<" "<<phid<<endl;
// 	  if(
// 	 //coplanarity
// 	     (phid>CoplanarityDifference)
//          //within detector ranges
// 	     ||theta1>M_PI/3.0
// 	     ||theta2>M_PI/3.0
// 	     ||theta1<0.17
// 	     ||theta2<0.17
// 	 //elastics condition
// 	     ||fabs(tan(theta1)*tan(theta2)-gamma2)>GammaDifference
// 	     )continue;
// 	  if(!useTiming)
// 	    {
// 	      pt=
// initParticles[0].Momentum().R()*tan(dirs[pris[i]].Theta())*tan(dirs[pris[j]].Theta())/(tan(dirs[pris[i]].Theta())+tan(dirs[pris[j]].Theta()));
// 	      pl2=initParticles[0].Momentum().R()*tan(dirs[pris[i]].Theta())/(tan(dirs[pris[i]].Theta())+tan(dirs[pris[j]].Theta()));
// 	      pl1=initParticles[0].Momentum().R()*tan(dirs[pris[j]].Theta())/(tan(dirs[pris[i]].Theta())+tan(dirs[pris[j]].Theta()));
// 	      px1=sqrt(pt*pt+pl1*pl1);
// 	      px2=sqrt(pt*pt+pl2*pl2);

// 	      p1.setPM(dirs[pris[i]]*px1,mass);
// 	      p2.setPM(dirs[pris[j]]*px2,mass);
// 	    }
// 	  else
// 	    {
// 	      p1.setVM(dirs[pris[i]]*speeds[pris[i]],mass);
// 	      p2.setVM(dirs[pris[j]]*speeds[pris[j]],mass);
// 	    }
// 	  p12=p1+p2;
// 	  pR=pI-p12;
// 	  chis[i*nPri+j]=fabs(tan(theta1)*tan(theta2)-gamma2);//(fabs(pR*pR));
// 	  //	  cout<<"chi"<<chis[i*nPri+j]<<endl;
// 	  //	  cout<<p1<<" "<<p2<<" "<<chis[i*nPri+j]<<endl;
// 	      //	      if(chis[j]<0.0)chis[j]=-chis[j];
// 	      //	      cout<<p12<<pI<<pR<<chis[j]<<endl;
// 	}
//     }
//   //  cout<<"n"<<endl;
//   int best=0;
//   for(int i=1;i<nPri*nPri;i++)
//     if(chis[i]<chis[best])
//       best=i;
//   float ret=chis[best];
//   if(indexes!=NULL)
//     {
//       indexes[0]=pris[best/nPri];
//       indexes[1]=pris[best%nPri];
//     }
//   if(results!=NULL)
//     {
//     }
//   return ret;
// }
//#include<TFile.h>
// RbaseReaction* Rppelastic::checkNew(TTrack **tracks,int num)
// {
//   if(num<2)return NULL;
//   if(tracks==NULL)return NULL;
// //   if(outTree.GetNbranches()==0)
// //     {
// //       outTree.Branch("beta_m",&beta_m,"beta_m/D");
// //       outTree.Branch("beta_r",&beta_r,"beta_r/D");
// //       outTree.Branch("theta",&theta,"theta/D");
// //       outTree.Branch("el",el,"el[3]/I");
// //       outTree.Branch("det",det,"det[3]/I");
// //       outTree.Branch("path",&pathlength_C,"path/D");
// //     }
//   int index[2];
//   float chi1=justCheck(tracks, num, index,NULL);
//   if(chi1>getChiLimit()||chi1<0)
//     {
//       return NULL;
//     }
//   RbaseReaction *ret=new
//   Rppelastic(fkinFit,initParticles[0],initParticles[1]);
//   ret->setChiLimit(chiLimit);
//   ((Rppelastic*)ret)->setGammaDifference(GammaDifference);
//   ((Rppelastic*)ret)->setCoplanarityDifference(CoplanarityDifference);
//   ret->check(tracks,num);
//   //  cout<<"elastic found"<<endl;
// //   if((((int)histo.GetEntries())%100)==0)
// //     {
// //       TFile f("speedy81.root","recreate");
// //       histo.Write();
// //       histo.SetDirectory(0);
// //       ThB.Write();
// //       referenceThB.Write();
// //       ThB.SetDirectory(0);
// //       referenceThB.SetDirectory(0);
// //       ringTof.Write();
// //       barrelTof.Write();
// //       ringTof.SetDirectory(0);
// //       barrelTof.SetDirectory(0);
// //       ringTofref.Write();
// //       barrelTofref.Write();
// //       ringTofref.SetDirectory(0);
// //       barrelTofref.SetDirectory(0);
// //       outTree.Write();
// //       outTree.SetDirectory(0);
// //       f.Close();
// //     }
//   return ret;
// }

void Rppelastic::setInitParticles(int num, momentum4D* p)
{
  RbaseReaction::setInitParticles(num, p);
  excessEnergy = cm.Energy() - 2 * Eparticles::getMass("proton");
  useTiming = false;
}
RbaseReaction* Rppelastic::getClone()
{
  RbaseReaction* ret = new Rppelastic(fkinFit, initParticles[0], initParticles[1]);
  ret->setChiLimit(chiLimit);
  ((Rppelastic*)ret)->setGammaDifference(GammaDifference);
  ((Rppelastic*)ret)->setCoplanarityDifference(CoplanarityDifference);
  return ret; // new Rppelastic(*this);
}
void Rppelastic::setCoplanarityDifference(float val) { CoplanarityDifference = val; }
void Rppelastic::setGammaDifference(float val) { GammaDifference = val; }
float Rppelastic::getCoplanarityDifference() const { return CoplanarityDifference; }
float Rppelastic::getGammaDifference() const { return GammaDifference; }
void Rppelastic::calculate(int justGeometry)
{
  if (tracks[0] == NULL || tracks[1] == NULL)
    return;
  switch (justGeometry) {
  case 0: {
    momentum4D p1, p2;
    TTrack* tr = ((TTrack*)tracks[0]);
    p1.setVM(tr->getPath().Direction() * tr->getSpeed(), Eparticles::getMassG(particleIDs[0]));
    tr = ((TTrack*)tracks[1]);
    p2.setVM(tr->getPath().Direction() * tr->getSpeed(), Eparticles::getMassG(particleIDs[1]));
    particles[0] = p1;
    particles[1] = p2;
    calculateCM();
    break;
  }
  case 1: {
    vector3D dir1 = ((TTrack *)tracks[0])->getPath().Direction(),
             dir2 = ((TTrack *)tracks[1])->getPath().Direction();
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
float Rppelastic::check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                        TTrack** tracks_2s, int num_2s)
{
  if (num_nd == 0)
    return chi2;
  if (tracks_nd == NULL)
    return chi2;
  int index[2];
  float chi = justCheck(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s, index, NULL);
  for (int i = 0; i < 2; i++) {
    if (index[i] < 0)
      return chi2;
    if (index[i] < num_nd)
      setTrack(i, tracks_nd[index[i]]);
    else if (index[i] < num_nd + num_1s)
      setTrack(i, tracks_1s[index[i] - num_nd]);
    else if (index[i] < num_nd + num_1s + num_2s)
      setTrack(i, tracks_2s[index[i] - num_nd - num_1s]);
    else
      return chi2;
  }
  chi2 = chi;
  if (chi > chiLimit)
    return chi2;
  calculate(true);
  return chi2;
}
float Rppelastic::justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                            TTrack** tracks_2s, int num_2s, int* indexes, momentum4D* results)
{
  if (num_nd < 2)
    return 1e+5;
  if (tracks_nd == NULL)
    return 1e+5;
  chi2 = 1e+5;
  if (num_1s == num_2s)
    chi2 = 1e+5;
  if (tracks_1s == NULL || tracks_2s == NULL)
    chi2 = 1e+5;
  double gamma2 = (1 - cm.Beta() * cm.Beta());
  int nPri = 0;
  float speeds[num_nd];
  vector3D dirs[num_nd];
  int pris[num_nd];
  float chis[num_nd * num_nd];
  for (int i = 0; i < num_nd * num_nd; i++)
    chis[i] = 1e+5;
  for (int i = 0; i < num_nd; i++) {
    if (tracks_nd[i] == NULL)
      continue;
    if (tracks_nd[i]->isSecondary() || tracks_nd[i]->getCharge() == 0 ||
        tracks_nd[i]->getNumberOfSecondaryTracks() > 0)
      continue;
    pris[nPri++] = i;
    speeds[i] = tracks_nd[i]->getSpeed();
    dirs[i] = tracks_nd[i]->getPath().Direction();
    dirs[i].normalize();
    dirs[i].setRep(_spheric_);
    if (speeds[i] >= 0.98)
      speeds[i] = 0.98;
  }
  if (nPri < 2)
    return 1e+5;
  momentum4D p1, p2, p12, pI, pR;
  pI = initParticles[0] + initParticles[1];
  vector3D dir;
  float mass = Eparticles::getMass("proton");
  double theta1, theta2;
  vector3D z = pI.Momentum(), dp1, dp2;
  dp1.setRep(_cylindric_);
  dp2.setRep(_cylindric_);
  z.normalize();
  float pt;
  float pl1;
  float pl2;
  float px1;
  float px2;
  for (int i = 0; i < nPri; i++) {
    for (int j = i + 1; j < nPri; j++) {
      chis[i * nPri + j] = 10000;
      if (i == j)
        continue;
      dp1 = dirs[pris[i]] - z * (dirs[pris[i]] * z);
      dp2 = dirs[pris[j]] - z * (dirs[pris[j]] * z);
      double phid = acos((dp1 * dp2) / dp1.R() / dp2.R());
      phid = fabs(phid - M_PI);
      theta1 = acos(dirs[pris[i]] * z / z.R() / dirs[pris[i]].R());
      theta2 = acos(dirs[pris[j]] * z / z.R() / dirs[pris[j]].R());
      if (
          // coplanarity
          (phid > CoplanarityDifference)
          // within detector ranges
          || theta1 > M_PI / 3.0 || theta2 > M_PI / 3.0 || theta1 < 0.17 || theta2 < 0.17
          // elastics condition
          || fabs(tan(theta1) * tan(theta2) - gamma2) > GammaDifference)
        continue;
      if (!useTiming) {
        pt = initParticles[0].Momentum().R() * tan(theta1) * tan(theta2) /
             (tan(theta1) + tan(theta2));
        pl2 = initParticles[0].Momentum().R() * tan(theta1) / (tan(theta2) + tan(theta1));
        pl1 = initParticles[0].Momentum().R() * tan(theta2) / (tan(theta1) + tan(theta2));
        px1 = sqrt(pt * pt + pl1 * pl1);
        px2 = sqrt(pt * pt + pl2 * pl2);

        p1.setPM(dirs[pris[i]] * px1, mass);
        p2.setPM(dirs[pris[j]] * px2, mass);
      } else {
        p1.setVM(dirs[pris[i]] * speeds[pris[i]], mass);
        p2.setVM(dirs[pris[j]] * speeds[pris[j]], mass);
      }
      p12 = p1 + p2;
      pR = pI - p12;
      chis[i * nPri + j] = fabs(tan(theta1) * tan(theta2) - gamma2); //(fabs(pR*pR));
    }
  }
  int best = 0;
  for (int i = 1; i < nPri * nPri; i++)
    if (chis[i] < chis[best])
      best = i;
  float ret = chis[best];
  if (indexes != NULL) {
    indexes[0] = pris[best / nPri];
    indexes[1] = pris[best % nPri];
  }
  if (results != NULL) {
    //     results[0]=
  }
  return ret;
}
RbaseReaction* Rppelastic::checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                                    TTrack** tracks_2s, int num_2s)
{
  if (num_nd + num_1s + num_2s < 2)
    return NULL;
  if (tracks_nd == NULL && tracks_1s == NULL && tracks_2s == NULL)
    return NULL;
  int index[2];
  float chi = justCheck(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s, index, NULL);
  if (chi > getChiLimit() || chi < 0)
    return NULL;
  RbaseReaction* ret = new Rppelastic(fkinFit, initParticles[0], initParticles[1]);
  ret->setChiLimit(chiLimit);
  ((Rppelastic*)ret)->setGammaDifference(GammaDifference);
  ((Rppelastic*)ret)->setCoplanarityDifference(CoplanarityDifference);
  ret->check(tracks_nd, num_nd, tracks_1s, num_1s, tracks_2s, num_2s);
  return ret;
}
