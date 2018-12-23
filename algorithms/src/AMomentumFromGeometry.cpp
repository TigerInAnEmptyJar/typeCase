#include "AMomentumFromGeometry.h"
#include <stdlib.h>
AMomentumFromGeometry::AMomentumFromGeometry(TSetup &setup, TTrack **tracksIn, int &numberOfTracksIn):AAlgorithm("Calculate momentum from Geometry"),numberOfTracks(numberOfTracksIn)
{
  tracks=tracksIn;
  inParticle1=setup.getBeamr(0).getParticle();
  if(setup.hasTarget())
    inParticle2=setup.getTargetr().getParticle();
  else
    inParticle2=setup.getBeamr(1).getParticle();
  inParticle1.setPM(vector3D(0,0,3.059),0.938271998);
  inParticle2.setPM(vector3D(0,0,0),0.938271998);
  CM=inParticle1+inParticle2;
}

AMomentumFromGeometry::~AMomentumFromGeometry()
{
}

void *AMomentumFromGeometry::process(void*ptr)
{
  vector3D v[numberOfTracks];
  for(int i=0;i< numberOfTracks;i++)
    v[i]=tracks[i]->getPath().Direction();
  switch(numberOfTracks)
    {
    case 0:break;
    case 1:
      {
	break;
      }
    case 2:
       {
 	vector3D v1(tracks[0]->getPath().Direction()),v2(tracks[1]->getPath().Direction());
	 if(abs(abs(v1.Phi()-v2.Phi())-M_PI)>M_PI/18)
	   {
	     tracks[0]->getParticler().setPM(vector3D(0,0,0),1);
	     tracks[1]->getParticler().setPM(vector3D(0,0,0),1);
	     tracks[1]->setSpeed(tracks[1]->getParticle().Velocity().length());
	     tracks[1]->setKineticEnergy(tracks[1]->getParticle().Energy());
	     tracks[0]->setSpeed(tracks[0]->getParticle().Velocity().length());
	     tracks[0]->setKineticEnergy(tracks[0]->getParticle().Energy());
	     return 0;
	   }
 	float th1=v1.Theta(),th2=v2.Theta(),ph1=v1.Phi(),ph2=v2.Phi();
 	float p1,p2;
	float /*sith1=sin(th1),*/csth1=cos(th1),tnth1=tan(th1),sith2=sin(th2),/*csth2=cos(th2),*/tnth2=tan(th2);
	float siph1=sin(ph1),csph1=cos(ph1),siph2=sin(ph2),csph2=cos(ph2);
	float s1=siph1+csph1,s2=siph2+csph2;
	p1=(CM.Z()/csth1-(CM.Y()+CM.X())/(csth1*tnth2*s2))/(1-tnth1*s1/tnth2/s2);
	// 	p1=(CM.Z()/cos(th1)+CM.Y()/(sin(ph2)*cos(th1)*tan(th2)))/(1-(sin(ph1)*tan(th1))/(sin(ph2)*tan(th2)));
	p2=((CM.X()+CM.Y())/s2/sith2-CM.Z()*tnth1*s1/sith2/s2)/(1-tnth1/tnth2*s1/s2);
	// 	p2=CM.Z()/cos(th2)-p1*cos(th1)/cos(th2);
	//	vector3D beta(-CM.X(),-CM.Y(),-CM.Z());
	//	beta=beta*(1.0/CM.Energy());
	//	float g=1.0/sqrt(1.0-beta*beta);
	float m1,m2;
// 	if(abs(tan(v1.Theta())*tan(v2.Theta())-1/g/g)<0.1)
// 	  {
	    m1=0.938271998;
	    m2=m1;
// 	  }
// 	else
// 	  {
// 	    m1=1.87414;
// 	    m2=0.13957012;
// 	  }
	    if((v1*p1+v2*p2-CM.Momentum()).length()/CM.Momentum().length()<0.05)
	      {
 	tracks[0]->getParticler().setPM(v1*p1,m1);
 	tracks[1]->getParticler().setPM(v2*p2,m2);
	tracks[1]->setSpeed(tracks[1]->getParticle().Velocity().length());
	tracks[1]->setKineticEnergy(tracks[1]->getParticle().Energy());
	tracks[0]->setSpeed(tracks[0]->getParticle().Velocity().length());
	tracks[0]->setKineticEnergy(tracks[0]->getParticle().Energy());
	      }
	    else
	      {
		tracks[0]->getParticler()=momentum4D();//.setPM(vector3D(0,0,0),1);
		tracks[1]->getParticler()=momentum4D();//.setPM(vector3D(0,0,0),1);
		tracks[1]->setSpeed(0);//tracks[1]->getParticle().Velocity().length());
		tracks[1]->setKineticEnergy(0);//tracks[1]->getParticle().Energy());
		tracks[0]->setSpeed(0);//tracks[0]->getParticle().Velocity().length());
		tracks[0]->setKineticEnergy(0);//tracks[0]->getParticle().Energy());
	      }
	break;
       }
    case 3:
      {
	matrixNxM A(3,numberOfTracks),At(numberOfTracks,3);
	Vector b(3),ret(numberOfTracks);
	b.transpose();
	ret.transpose();
	for(int i=0;i<numberOfTracks;i++)
	  {
	    A.setValue(i,0,sin(v[i].Theta())*cos(v[i].Phi()));
	    A.setValue(i,1,sin(v[i].Theta())*sin(v[i].Phi()));
	    A.setValue(i,2,cos(v[i].Theta()));
	  }
	b.setValue(0,CM.X());b.setValue(1,CM.Y());b.setValue(2,CM.Z());
	At=!A;
	matrixNxM A_1(At*A);
	A_1=-A_1;
	if(A_1.Lines()==0) break;
	ret=(A_1)*(At*b);
	double chi2 = 0;
	Vector cC(numberOfTracks);
	cC=A*ret;
	cC=cC-b;
	chi2=cC*cC;
	if(3!=numberOfTracks)
	  chi2=chi2/abs(3-numberOfTracks);
	for(int i=0;i<numberOfTracks;i++)
	  {
	    momentum4D mom;
	    mom.setPM(v[i]*ret.getValue(i),0.938271998);
	    tracks[i]->setParticle(mom);
	    tracks[i]->setSpeed(tracks[i]->getParticle().Velocity().length());
	    tracks[i]->setKineticEnergy(tracks[i]->getParticle().Energy());
	  }
	break;
      }
    case 4:break;
    }
  return ptr;
}
