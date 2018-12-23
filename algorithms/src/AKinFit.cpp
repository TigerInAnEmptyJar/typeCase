#include "AKinFit.h"
#include <stdlib.h>
AKinFit::AKinFit(int maxIterationIn,int maxParticlesIn,float epsilonIn):AFitAlgorithm("WASA kin Fit"),epsilon(epsilonIn),maxParticles(maxParticlesIn),maxIteration(maxIterationIn),useCovariant(false)
{
  in =new int*[maxParticles];
  for(int i=0;i< maxParticles;i++)
    in[i]=new int[maxParticles+5];
  particles=new vector3D[maxParticles];
  dpin=new vector3D[maxParticles];
  am=new float[maxParticles];
  ain=new float[maxParticles];
  p=new vector3D[maxParticles];
  nP=0;
  nC=0;
}

AKinFit::~AKinFit()
{
  for(int i=0;i<maxParticles;i++)
    delete []in[i];
  delete []in;
  delete []particles;
  delete []dpin;
  delete []am;
  delete []ain;
  delete []p;
}
void *AKinFit::process(void*ptr)
{
  fit();
  return ptr;
}

void AKinFit::add(Vector &data)
{
  if(data.getDimension()<7)return;
  if(nP>=maxParticles)return;
  particles[nP].setValues(data.getValue(0),data.getValue(1),data.getValue(2));
  dpin[nP].setValues(data.getValue(3),data.getValue(4),data.getValue(5));
  am[nP]=data.getValue(6);
  for(int i=0;i<nC+5;i++)
    if(i+8>data.getDimension())
      in[nP][i]=0;
    else
      in[nP][i]=(int)data.getValue(i+7);
  nP++;
}

void AKinFit::clear()
{
  nP=0;
}

Vector AKinFit::getResult()
{
  Vector ret(nP*3);
  for(int i=0;i<nP;i++)
    {
      ret.setValue(i*3+0,p[i].getValue(0));
      ret.setValue(i*3+1,p[i].getValue(1));
      ret.setValue(i*3+2,p[i].getValue(2));
    }
  return ret;
}

void AKinFit::getAddPoint(Vector &data)
{
  add(data);
}

void AKinFit::setConstraints(int nCin,Vector masses)
{
  if(nCin>=maxParticles)return;
  if(masses.getDimension()<nCin)return;
  nC=nCin;
  for(int i=0;i<nC;i++)
    ain[i]=masses.getValue(i);
}

float AKinFit::fit()
{
  int mode=0;
  bool printEventInfo=((mode%10)==1||(mode%10)==2||(mode%10)==3);
  bool printIterationInfo=((mode%10)==2||(mode%10)==3);
  bool printMatrices=((mode%10)==3);
  float chi2=10000,d=10000;//
  //  int ifl;//nc-nVariablesUnmeasured=4-nc-nu ifl<0 error
  momentum4D ppx[nP];
  //  p=new vector3D[nP];
  //  int outcode=-1;

  //copy particles
  for(int i=0;i<nP;i++)
    {
      p[i]=particles[i];
    }
  int nMeasured=0,nUnmeasured=0;

  for(int i=0;i<nP;i++)
    for(int j=0;j<3;j++)
      if(in[i][j]==1)
	nMeasured++;
      else if(in[i][j]==3)
	nUnmeasured++;
  if(printEventInfo)
    {
      cout<<"Input:\nparticles:"<<nP<<endl;
      for(int i=0;i<nP;i++)
	cout<<i<<particles[i]<<" "<<am[i]<<" "<<in[i][3]<<endl;
      cout<<"errors"<<endl;
      for(int i=0;i<nP;i++)
	cout<<i<<dpin[i]<<endl;
    }
  //define working structures
  matrixNxM BM(4+nC,nMeasured),BU(4+nC,nUnmeasured);//matrices containing derivatives 
  //  matrixNxM BM_P(4+nC,nMeasured),BU_P(4+nC,nUnmeasured);//matrices containing derivatives 
  matrixNxM GB(4+nC,4+nC);
  matrixNxM GB_1(4+nC,4+nC);
  matrixNxM GM(nMeasured,nMeasured),GM_1(nMeasured,nMeasured),GU(nUnmeasured,nUnmeasured);//error matrix and weighting matrix !diagonal!
  matrixNxM GMIN(nMeasured,nMeasured);//if useCovariant own covariant matrix
  //  matrixNxM GM_P(nMeasured);
  matrixNxM G1(nUnmeasured,nUnmeasured);
  matrixNxM *matPoint;
  Vector epsM(nMeasured),epsU(nUnmeasured);//corrections to the variables to be introduced by the fit
  Vector FM(4+nC),FI(4+nC),FM0(4+nC);//
  Vector al(4+nC);//lagrange multipliers
  GU.Unity();
  al.transpose();
  FM.transpose();
  FI.transpose();
  FM0.transpose();
  epsM.transpose();
  for(int i=0;i<nMeasured;i++)  epsM.setValue(i,0);
  for(int i=0;i<nUnmeasured;i++)  epsU.setValue(i,0);
  epsU.transpose();
  Vector sf[nC];for(int i=0;i<nC;i++)sf[i].reDimension(5);
//   float **sf=new float*[5];
//   for(int i=0;i<5;i++)sf[i]=new float[nC];
  //index saving
  int *Meas=new int[nMeasured],*Unmeas=new int[nUnmeasured];
  bool condition=true;
  //  int info=nC+4;
  int iteration=0;
  float chi2_prev=0,d_prev,d0;
  float sum;
  int num,pos,number,*mmtmp;
  vector4D tmp4d(1,1,1,1);
  momentum4D tmpm4d;
  //init measured-/unmeasured variable holders
  nMeasured=0;
  nUnmeasured=0;
  for(int i=0;i<nP;i++)
    for(int j=0;j<3;j++)
      if(abs(in[i][j])==1)
	{
	  Meas[nMeasured++]=i*10+j;
	}
      else if(abs(in[i][j])==3)
	{
	  Unmeas[nUnmeasured++]=i*10+j;
	}
  epsM.Zero();
  epsU.Zero();
  if(printEventInfo)
    cout<<"unmeasured variables: "<<nUnmeasured<<" measured variables: "<<nMeasured<<endl;
  //init g-matrices
  GM.Unity();
  for(int i=0;i<nMeasured;i++)
    for(int j=0;j<nMeasured;j++)
      GM.setValue(i,j,0);
  if(useCovariant)
    {
      for(int i=0;i<nMeasured;i++)
	for(int j=0;j<nMeasured;j++)
	  GM.setValue(i,j,GMIN.getValue(((Meas[i]/10))*3+(int)(Meas[i]%10),
					((Meas[j]/10))*3+(int)(Meas[j]%10)));
    }
  else
    {
      for(int i=0;i<nMeasured;i++)
	{
	  GM.setValue(i,i,
		      dpin[Meas[i]/10].getValue((int)(Meas[i]%10))
		      *dpin[Meas[i]/10].getValue((int)(Meas[i]%10)));
	}
    }
  if(printMatrices)
    cout<<"GM-matrix\n"<<GM;
  GM_1=-GM;
  //  GM_P=GM;
  //calculate FM kinematic
  for(int i=0;i<nP;i++)
    {
      ppx[i]=getMomentum(i,p[i]);
      //cout<<p[i]<<ppx[i]<<in[i][4]<<" "<<am[i]<<endl;
    }
  tmpm4d.setValues(0,0,0,0);//=ppx[0]*(in[0][3]>0?1:-1);//.setValues(0,0,0,0);
  int sig;
  for(int i=0;i<nP;i++)
    {
      if(in[i][4]==0)continue;
      if(in[i][4]<0)sig=-1;
      else sig=1;
      tmpm4d=tmpm4d+(ppx[i]*sig);
    }
  for(int i=0;i<4;i++)
    FM.setValue(i,tmpm4d[i]);
  //calculate FM additional constraints
  for(int i=0;i<nC;i++)
    {
      sf[i].setValues(0,0,0,0,0);
//       for(int j=0;j<4;j++)
// 	sf[j][i]=0;
      for(int j=0;j<nP;j++)
	{
	  if(in[j][i+5]==0)sig=0;
	  else if(in[j][i+5]<1)sig=-1;
	  else sig=1;
	  for(int k=0;k<4;k++)
	    sf[i].setValue(k,sf[i].getValue(k)+sig*ppx[j].getValue(k)); //[k][i]+=in[j][i+5]*ppx[j].getValue(k);
	}
      sum=0;
      for(int j=0;j<3;j++)
	sum+=sf[i][j]*sf[i][j];
	//	sum+=sf[j][i]*sf[j][i];
      sum=sf[i][3]*sf[i][3]-sum;
      //       sum=sf[3][i]*sf[3][i]-sum;
      if(sum<=0)sum=0.001;
//       sf[4][i]=sqrt(sum);
      sf[i].setValue(4,sqrt(sum));
//       FM.setValue(i+4,sf[4][i]-ain[i]);
      FM.setValue(i+4,sf[i][4]-ain[i]);
    }
  d=sqrt(FM*FM);
  FM0=FM;
  d0=d;
  if(printEventInfo)
    cout<<"FM: "<<FM<<" d: "<<d<<endl;
  //iteration
  float value;
  while(condition)
    {
      if(printIterationInfo)
	cout<<"iteration: "<<iteration<<endl;
      //derivative matrices for measured and unmeasured variables. looks principally alike:
      for(int l=0;l<2;l++)
	{
	  if(l==1)      //calculate Matrix with derivatives BM (info x nMeasured) for measured Variables
	    {
	      matPoint=&BM;
	      number=nMeasured;
	      mmtmp=Meas;
	    }
	  else      //calculate Matrix with derivatives BU (info x nUnmeasured) for unmeasured variables
	    {
	      matPoint=&BU;
	      number=nUnmeasured;
	      mmtmp=Unmeas;
	    }
	  for(int i=0;i<number;i++)
	    {
	      num=mmtmp[i]/10;//particle number
	      pos=mmtmp[i]%10;//property number
	      tmp4d=getDerivative(in[num][3],pos,p[num],am[num]);
	      if(in[num][4]==0)sig=0;
	      else if(in[num][4]<1)sig=-1;
	      else sig=1;
	      for(int j=0;j<4;j++)
		matPoint->setValue(j,i,sig*tmp4d.getValue(j));
	      for(int j=0;j<nC;j++)
		{
		  if(in[num][j+5]!=0)
		    {
		      value=0;
		      for(int k=0;k<3;k++)
			value+=sf[j][k]*tmp4d[k];
			//value+=sf[k][j]*tmp4d[k];
		      matPoint->setValue(j+4,i,getDerivativeConstraint(in[num][3], pos, p[num], ppx[num], am[num], sf[j]));//-value/sf[4][j]*(in[num][j+5]==0?0:(in[num][j+5]<0?-1:1)));
		    }
		  else		  
		    matPoint->setValue(j+4,i,0);
		}
	    }
	}
      //      BM_P=BM;
      //      BU_P=BU;
      if(printMatrices)
	{
	  cout<<"BM: \n"<<BM;
	  cout<<"\nBM*epsM: "<<BM*epsM;
	  if(nUnmeasured>0)
	    {
	      cout<<"\nBU: \n"<<BU;
	      cout<<"\nBU*epsU: "<<BU*epsU;
	    }
	}
      FI=FM-BM*epsM-BU*epsU;
      if(FI.isLine())FI.transpose();
      if(printIterationInfo)
	cout<<"FI: "<<FI<<endl;
      //calculate GB matrix
      if(useCovariant)
	{
	  //	  GB=BM*((GM_1)*!BM);//GM^-1???
	  GB=BM*((GM)*!BM);//GM^-1???
	}
      else
	{
// 	  for(int i=0;i<nC+4;i++)
// 	    for(int j=0;j<nC+4;j++)
// 	      {
// 		sum=0;
// 		for(int k=0;k<nMeasured;k++)
// 		  sum+=BM.getValue(j,k)*GM.getValue(k,k)*BM.getValue(i,k);//GM^-1???
// 		GB.setValue(i,j,sum);
// 	      }
	  GB=BM*((GM)*!BM);//GM^-1???
	}
      if(printMatrices)
	cout<<"BM^T\n"<<!BM<<"GB:\n"<<GB;

      GB_1=-GB;
      if(printMatrices)
	cout<<"GB^1:  GB*GB-¹:"<<(GB_1*GB).Determinant()<<"\n"<<GB_1<<endl;;
      //      GB_P=GB;
      if(nUnmeasured>0)
	{
	  //G1 matrix
	  //  	  G1=(-((!BU)*GB_1*BU))*-1;//-(GU+BU^T*GB*BU)^-1 ???
	  //GBU=INV(BM*GB_1*BM^T)
	  //epsU=-1*INV(GU+BU^T*GBU*BU)*BU^T*GBU*FI
	  G1=-(BM*GB_1*!BM);
	  G1=(!BU)*G1;
	  epsU=(((-(GU+(G1*BU)))*G1)*FI);
	  epsU=epsU*-1;
	  //calculate epsilon for unmeasured
	  //	  epsU=G1*(!BU*(GB_1*FI));
	  //calculate lagrange multipliers
	  //	  al=(GB_1*((BU*epsU+FI)));
	  al=(GB_1*((BU*epsU+FI)));
	  if(printIterationInfo)
	    cout<<"epsilon U: "<<epsU<<endl;
	}
      else
	{	
	  //	  al=GB_1*FI;
	  al=GB_1*FI;}
      //calculate epsilon for measured
      epsM=-1*(((GM)*(!BM))*al);
      //      epsM=(((GM_1)*(!BM))*al);
      if(printIterationInfo)
	cout<<"epsilon M: "<<epsM<<"\nLambda: "<<al<<endl;
      //finalize: calculate fitted variables, and chi^2
      int n,m;
      for(int i=0;i<nUnmeasured;i++)
	{
	  n=Unmeas[i]/10;
	  m=Unmeas[i]%10;
	  if((in[n][3]==2||in[n][3]==3||in[n][3]==4)&&m!=0)
	    {
	      float prop=particles[n][m]+epsU[i];
	      if(m==1)
		{
		  if(prop<0)
		    {
		      int o=(int)(prop/M_PI+1);
		      epsU.setValue(i,epsU[i]-o*M_PI);
		    }
		  else if(prop>M_PI)
		    epsU.setValue(i,epsU[i]-((int)prop/M_PI)*M_PI);
		    
		}
	      else
		if(prop<-M_PI)
		  epsU.setValue(i,epsU[i]-((int)((prop+M_PI)/2./M_PI))*M_PI*2);
		else if(prop>M_PI)
		  epsU.setValue(i,epsU[i]-((int)((prop-M_PI)/2./M_PI))*2*M_PI);
	    }
	  p[n].setValue(m,particles[n].getValue(m)+
			epsU.getValue(i));
	}
      for(int i=0;i<nMeasured;i++)
	{
	  n=Meas[i]/10;
	  m=Meas[i]%10;
	  //	  cout<<"Particle "<<n<<" property "<<m<<" old "<<p[n][m]<<"\t new "<<particles[n][m]+epsM[i]<<endl;
	  if((in[n][3]==2||in[n][3]==3||in[n][3]==4)&&m!=0)
	    {
	      float prop=particles[n][m]+epsM[i];
	      if(m==1)
		{
		  if(prop<0)
		    {
		      int o=(int)(prop/M_PI+1);
		      epsM.setValue(i,epsM[i]-o*M_PI);
		    }
		  else if(prop>M_PI)
		    epsM.setValue(i,epsM[i]-((int)prop/M_PI)*M_PI);
		    
		}
	      else
		if(prop<-M_PI)
		  epsM.setValue(i,epsM[i]-((int)((prop+M_PI)/2./M_PI))*M_PI*2);
		else if(prop>M_PI)
		  epsM.setValue(i,epsM[i]-((int)((prop-M_PI)/2./M_PI))*2*M_PI);
	    }
	  p[n].setValue(m,particles[n].getValue(m)+
			epsM.getValue(i));
	}
      chi2_prev=chi2;
      chi2=epsM*((GM)*epsM)+epsU*(GU*epsU)+2*al*(FI+BM*epsM+BU*epsU);//epsM*(GM*epsm)+epsU*(GU*epsU)+2*al*(FI+BM*epsM+BU*epsU) ???
      //calculate FM kinematic
      for(int i=0;i<nP;i++)
	ppx[i]=getMomentum(i,p[i]);
      if(printIterationInfo)
	{
	  cout<<"new variables: chi²="<<chi2<<endl;
	  for(int i=0;i<nP;i++)cout<<p[i]<<ppx[i]<<endl;
	}
      tmpm4d.setValues(0,0,0,0);
      for(int i=0;i<nP;i++)
	{
	  if(in[i][4]==0)continue;
	  if(in[i][4]<0)sig=-1;
	  else sig=1;
	  tmpm4d=tmpm4d+(ppx[i]*sig);
	  //	  cout<<i<<" "<<ppx[i]<<sig<<tmpm4d<<endl;
	}
      for(int i=0;i<4;i++)
	FM.setValue(i,tmpm4d[i]);
      //calculate FM additional constraints
      for(int i=0;i<nC;i++)
	{
	  sf[i].setValues(0,0,0,0,0);
	  //       for(int j=0;j<4;j++)
	  // 	sf[j][i]=0;
	  for(int j=0;j<nP;j++)
	    {
	      if(in[j][i+5]==0)sig=0;
	      else if(in[j][i+5]<1)sig=-1;
	      else sig=1;
	      for(int k=0;k<4;k++)
		sf[i].setValue(k,sf[i].getValue(k)+sig*ppx[j].getValue(k)); //[k][i]+=in[j][i+5]*ppx[j].getValue(k);
	    }
	  sum=0;
	  for(int j=0;j<3;j++)
	    sum+=sf[i][j]*sf[i][j];
	  //	sum+=sf[j][i]*sf[j][i];
	  sum=sf[i][3]*sf[i][3]-sum;
	  //       sum=sf[3][i]*sf[3][i]-sum;
	  if(sum<=0)sum=0.001;
	  //       sf[4][i]=sqrt(sum);
	  sf[i].setValue(4,sqrt(sum));
	  //       FM.setValue(i+4,sf[4][i]-ain[i]);
	  FM.setValue(i+4,sf[i][4]-ain[i]);
	}
      d_prev=d;
      d=sqrt(FM*FM);
      if(printIterationInfo)
	{
	cout<<"FM: "<<FM<<" d: "<<d<<endl;;
	cout<<"FM0: "<<FM0<<endl;}
      iteration++;
     //calculate iteration condition
      if((d<epsilon)||(abs(chi2-chi2_prev)<0.000001))//||chi2<1)
	{
	  if(printEventInfo)
	    cout<<"regular break: d="<<d<<" epsilon="<<epsilon<<" chi²="<<chi2<<" chi²_prev="<<chi2_prev<<endl;
	  condition=false;
	  outCode=iteration;
	}
      if(d>d_prev)
	{
	  //convergence wrong
	}
      if(iteration>maxIteration)
	{
	  if(printEventInfo)
	    cout<<"break too much iterations"<<endl;
	  condition=false;
	  outCode=-iteration;
	}
    }
  if(printEventInfo)
    {
      cout<<"loop gone with: "<<outCode<<endl;
      cout<<"resulting variables: "<<chi2<<endl;
      for(int i=0;i<nP;i++)cout<<p[i]<<endl;
    }
//   for(int i=0;i<nP;i++)
//     pout[i]=p[i];
  return (chi2);//*(outCode<0?-1:1));
}

momentum4D AKinFit::getMomentum(int num,vector3D pm)
{
  momentum4D ret;
  switch(abs(in[num][3]))
    {
    case 1:ret.setPM(pm,am[num]);break;
    case 2://E,th,ph
      {
	if(p[num].X()<0)p[num].setValue(0,0.001);
	float P=sqrt(p[num].X()*(p[num].X()+2*am[num]));
	ret.setPM(vector3D(P*sin(p[num].Y())*cos(p[num].Z()),
			   P*sin(p[num].Y())*sin(p[num].Z()),
			   P*cos(p[num].Y())),
		  am[num]);
	break;
      }
    case 3://v,th,ph
      if(p[num].X()>1)p[num].setValue(0,0.9995);
      if(p[num].X()<0)p[num].setValue(0,0.001);
      ret.setVM(vector3D(p[num].X()*cos(p[num].Z())*sin(p[num].Y()),
			p[num].X()*sin(p[num].Z())*sin(p[num].Y()),
			p[num].X()*cos(p[num].Y())),am[num]);
      break;
    case 4:ret.setPM(p[num],am[num]);break;
    }
  return ret;
}

vector4D AKinFit::getDerivative(int isIn,int pos,vector3D pm,float mass)
{
  vector4D ret(0,0,0,0);
  float tmpf;
  switch(abs(isIn))
    {
    case 1://px,py,pz
      for(int k=0;k<3;k++)
	ret.setValue(k,(pos==k?1:0));
      ret.setValue(3,pm.getValue(pos)/sqrt(pm.R()*pm.R()+mass*mass));
      break;
    case 2://E,th,ph
      if(pm.X()<0)pm.setValue(0,0.001);
      tmpf=sqrt((pm.X()+2*mass)*pm.X());
      switch(pos)
	{
	case 0:		
	  ret.setValue(0,(pm.X()+mass)/tmpf*(sin(pm.Y())*cos(pm.Z())));
	  ret.setValue(1,(pm.X()+mass)/tmpf*(sin(pm.Y())*sin(pm.Z())));
	  ret.setValue(2,(pm.X()+mass)/tmpf*(cos(pm.Y())));
	  break;
	case 1:		  
	  ret.setValue(0,tmpf*(cos(pm.Y())*cos(pm.Z())));
	  ret.setValue(1,tmpf*(cos(pm.Y())*sin(pm.Z())));
	  ret.setValue(2,-tmpf*(sin(pm.Y())));
	  break;
	case 2:	  
	  ret.setValue(0,-tmpf*(sin(pm.Y())*sin(pm.Z())));
	  ret.setValue(1,tmpf*(sin(pm.Y())*cos(pm.Z())));
	  ret.setValue(2,0);
	  break;
	}
      //ret.setValue(0,ret.getValue(0)*ppx.Energy()/tmpf);
      ret.setValue(3,(pos==0?1:0));
      break;
    case 3://v,th,phi
      {
	float g=1/sqrt(1-pm.X()*pm.X());
	float p=mass*pm.X()*g;//sqrt(1-pm.X()*pm.X());
	tmpf=mass*g*(1+pm.X()*pm.X()*g*g);
	//      tmpf=mass*pm.X()/sqrt(1-pm.X()*pm.X());//sqrt(p.X()*(p.X()+2*mass));
	switch(pos)
	  {
	  case 0:		
	    ret.setValues(tmpf*(sin(pm.Y())*cos(pm.Z())),
			  tmpf*(sin(pm.Y())*sin(pm.Z())),
			  tmpf*(cos(pm.Y())),0);
	    break;
	  case 1:		  
	    ret.setValues(p*(cos(pm.Y())*cos(pm.Z())),
			  p*(cos(pm.Y())*sin(pm.Z())),
			  -p*(sin(pm.Y())),0);
	    break;
	  case 2:	  
	    ret.setValues(-p*(sin(pm.Y())*sin(pm.Z())),
			  p*(sin(pm.Y())*cos(pm.Z())),0,0);
	    break;
	  }
	//      ret.setValue(0,ret.getValue(0)*ppx.W()/(sqrt(p.X()*(p.X()+2*mass))));
	ret.setValue(3,(pos==0?mass*pm.X()*g*g*g:0));//(1-pm.X()*pm.X())/sqrt(1-pm.X()*pm.X()):0));
	break;
      }
    case 4:break;
    }
  return ret;
}
float AKinFit::getDerivativeConstraint(int representation, int pos, vector3D pm,momentum4D px, float mass, Vector sf)
{
  //Derivative of invariant mass with respect to the particle in a specific representation and its component pos:
  //d(Minv)/d(prop) = 1/Minv*(E_sum* d(E_part)/d(prop) - SUM_i(P_sum_i * d(P_i)/d(prop)))

  switch(abs(representation))
    {
    case 1://px,py,pz
      return (sf[3]*px[pos]/px.Energy()-sf[pos])/sf[4];
    case 2://E,th,ph
      {
	float sum=0;
	momentum4D der=getDerivative(representation, pos, pm, mass);
	for(int i=0;i<3;i++)
	  sum+=sf[i]*der[i];
	if(pos==0)
	  return (sf[3]-sum)/sf[4];
	else 
	  return (-sum)/sf[4];
	break;
      }
    case 3://v,th,phi
      {
	float sum=0;
	momentum4D det=getDerivative(representation,pos,pm,mass);
	for(int i=0;i<3;i++)
	  sum+=sf[i]*det[i];
	return (sf[3]*det[4]-sum)/sf[4];
	break;
      }
    case 4://p,th,phi
      {
	float sum=0;
	momentum4D der=getDerivative(representation, pos, pm, mass);
	for(int i=0;i<3;i++)
	  sum+=sf[i]*der[i];
	if(pos==0)
	  return (sf[3]*pm[pos]/px.Energy()-sum)/sf[4];
	else
	  return (-sum)/sf[4];
	break;
      }
    }
  return 0;
}

AFitAlgorithm* AKinFit::getClone()
{
  AFitAlgorithm *ret= new AKinFit(maxIteration,maxParticles,epsilon);
  Vector c(nC);
  for(int i=0;i<nC;i++)c.setValue(i,ain[i]);
  ((AKinFit*)ret)->setConstraints(nC,c);
  return ret;
}


int AKinFit::getOutInfo()const
{
  return outCode;
}
