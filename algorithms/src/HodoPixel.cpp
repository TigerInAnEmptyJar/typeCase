#include "HodoPixel.h"
#include "logger.h"
#include "planeShape.h"
#include "circle.h"
//#define AHODO2HISTO
#ifdef AHODO2HISTO
#include <TH2.h>
#include <TFile.h>
TH1F histo_phi_H2("hodo2 phi","2-layered hodo: \\phi",100,-M_PI*1.05,M_PI*1.05);
TH1F histo_theta_H2("hodo2 theta","2-layered hodo: \\theta",100,0,1);
TH2F histo_xy_H2("hodo2 xy","2-layered hodo: x-y",100,-1,1,100,-1,1);
#endif

AHodoPixel::AHodoPixel(int maxPix, int& numPix, TPixel **pixIn, int **numberOfHits, TCalibHit ***hits,int **numberOfHitClusters, THitCluster ***hitClusters,const algorithm_parameter& ap):AAlgorithm("HodoPixel"),numPixel(numPix),maxPixel(maxPix),maxHits(2)
{
  ID=ap.getParam<int>(0).getData();
  Pixels=pixIn;
  if(ap.getParam<bool>(0).getData())
    {
      hit1=(TCalibHit**)hitClusters[ap.getParam<int>(1).getData()];
      hit2=(TCalibHit**)hitClusters[ap.getParam<int>(2).getData()];
      numHits1=numberOfHitClusters[ap.getParam<int>(1).getData()];
      numHits2=numberOfHitClusters[ap.getParam<int>(2).getData()];
    }
  else
    {
      hit1=hits[ap.getParam<int>(1).getData()];
      hit2=hits[ap.getParam<int>(2).getData()];
      numHits1=numberOfHits[ap.getParam<int>(1).getData()];
      numHits2=numberOfHits[ap.getParam<int>(2).getData()];
    }
  maxChi=ap.getParam<float>(0).getData();
  for(int i=0;i<maxPix;i++)
    Pixels[i]->setID(ID);
  planeNormal.setValues(0,0,1);
  if(ap.getNumberOfParam<bool>()>1)
    {
      useMiddlePlane=ap.getParam<bool>(1).getData();
      if(useMiddlePlane)
	{
	  projectionPoint=ap.getParam<point3D>(0).getData();
	  planePoint=ap.getParam<point3D>(1).getData();
	  planeNormal=ap.getParam<vector3D>(0).getData();
	}
    }
  else
    useMiddlePlane=false;
  modulationFunction=NULL;
  if(ap.getNumberOfParam<bool>()>2&&ap.getNumberOfParam<string>()>0)
    {
      useModulation=ap.getParam<bool>(2).getData();
      if(useModulation)
	modulationFunction=new TF1("mudulationFunction",ap.getParam<string>(0).getData().data());
    }
}
AHodoPixel::AHodoPixel(int IDIn, float maxChiIn, int maxPix, int& numPix, TPixel **pixIn, int maxHit, int &numHit1, int &numHit2, TCalibHit **hits1, TCalibHit **hits2, bool middlePlane, point3D planeP, point3D ProjectionPoint, vector3D normal):AAlgorithm("HodoPixel"),numPixel(numPix), maxPixel(maxPix), maxHits(maxHit),maxChi(maxChiIn), numHits1(&numHit1), numHits2(&numHit2),ID(IDIn)
{
  Pixels=pixIn;
  hit1=hits1;
  hit2=hits2;
  useMiddlePlane=middlePlane;
  projectionPoint=ProjectionPoint;
  planePoint=planeP;
  planeNormal=normal;
  for(int i=0;i<maxPix;i++)
    Pixels[i]->setID(ID);
  useModulation=false;
  modulationFunction=NULL;
}

AHodoPixel::~AHodoPixel()
{
  if(modulationFunction!=NULL)delete modulationFunction;
#ifdef AHODO2HISTO
  TFile f("hodo2.root","recreate");
  f.cd();
  histo_phi_H2.Write();
  histo_theta_H2.Write();
  histo_xy_H2.Write();
  histo_phi_H2.SetDirectory(0);
  histo_theta_H2.SetDirectory(0);
  histo_xy_H2.SetDirectory(0);
  cout<<"file \"data/12/analyser/hodo2.root\" written"<<endl;
  f.Close();
#endif
}

void *AHodoPixel::process(void*ptr)
{
  // cout<<"HodoPixel "<<*numHits1<<" "<<*numHits2<<endl;
  numPixel = 0;
  fiber *sh1, *sh2;
  planeShape *plSh, *tmpSh;
  sLine3D line1[*numHits1],line2[*numHits2];
  for (int i = 0; i < *numHits1; i++)
    {
      if((!(hit1[i]->isValid()))||(hit1[i]->getHitShape()==0))continue;
      sh1=(fiber*)hit1[i]->getHitShape();
      line1[i]=sLine3D(sh1->getCorner()+((sh1->getDirection(1)+sh1->getDirection(2))*0.5),sh1->getDirection(0));
    }
  for (int i = 0; i < *numHits2; i++)
    {
      if((!(hit2[i]->isValid()))||(hit2[i]->getHitShape()==0))continue;
      sh1=(fiber*)hit2[i]->getHitShape();
      line2[i]=sLine3D(sh1->getCorner()+((sh1->getDirection(1)+sh1->getDirection(2))*0.5),sh1->getDirection(0));
    }
//   point3D p[4];
//   float fac=0.55;
  for (int i = 0; i < *numHits1; i++)
    {
      if((!(hit1[i]->isValid()))||(hit1[i]->getHitShape()==0))continue;
      sh1=(fiber*)hit1[i]->getHitShape();
      for (int j = 0; j < *numHits2; j++)
	{
	  if((!(hit2[j]->isValid()))||(hit2[j]->getHitShape()==0))continue;
	  sh2=(fiber*)hit2[j]->getHitShape();
	  if (numPixel < maxPixel)
	    {
	      Pixels[numPixel]->resetElements();
	      Pixels[numPixel]->addElement(*(hit1[i]));
	      Pixels[numPixel]->addElement(*(hit2[j]));
	      Pixels[numPixel]->setMaxChiSquared(maxChi);
	      lLine3D pca=line1[i]-line2[j];
	      point3D cent=pca.Mid();
// 	      p[0]=cent+sh1->getDirection(1)*fac+sh2->getDirection(1)*fac;
// 	      p[1]=cent+sh1->getDirection(1)*fac-sh2->getDirection(1)*fac;
// 	      p[2]=cent-sh1->getDirection(1)*fac-sh2->getDirection(1)*fac;
// 	      p[3]=cent-sh1->getDirection(1)*fac+sh2->getDirection(1)*fac;
// 	      if(useModulation)
// 		{
// 		  float mod=modulationFunction->Eval(cent.Phi());
// 		  cent.setValues(sin(cent.Theta())*cos(cent.Phi()+mod)*cent.R(),
// 				 sin(cent.Theta())*sin(cent.Phi()+mod)*cent.R(),
// 				 cent.Z());
// 		}
	      //cout<<"("<<cent.Theta()*180./M_PI<<", "<<cent.Phi()*180./M_PI<<") "<<hit1[i]->getElement()<<" "<<hit2[j]->getElement()<<" "<<sh2->getDirection(1).R()<<endl;
	      plSh=new circle(cent,planeNormal,sh2->getDirection(1).R()*3);//p[0],p[1],p[2],p[3]);//sh2->getDirection(1).R()*3);
	      //plSh=new rectangle(cent,sh2->getDirection(2),p[0],p[1],p[2],p[3]);//sh2->getDirection(1).R()*3);
	      //	      plSh->setCenter(cent);
	      //	      plSh->setNormal(planeNormal);
	      tmpSh=Pixels[numPixel]->getShape();
	      Pixels[numPixel]->setShape(plSh);
	      if(tmpSh!=NULL)delete tmpSh;
	      tmpSh=NULL;
#ifdef AHODO2HISTO
	      histo_phi_H2.Fill(plSh->getCenter().Phi());
	      histo_theta_H2.Fill(plSh->getCenter().Theta());
	      histo_xy_H2.Fill(sin(plSh->getCenter().Theta())*cos(plSh->getCenter().Phi()),
			    sin(plSh->getCenter().Theta())*sin(plSh->getCenter().Phi()));
#endif
	      numPixel++;
	    }
	  else
	    {
	      anaLog << "Too many Pixels in Hodo" << endli;
	      return ptr;
	    }
	}
    }
  return ptr;
}
