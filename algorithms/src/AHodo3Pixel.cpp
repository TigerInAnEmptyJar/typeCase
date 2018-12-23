#include "AHodo3Pixel.h"
#include "fiber.h"
#include "circle.h"
#include "quadrangle.h"
#include "logger.h"
#include "beamtimeparameter.h"
#include "algorithmparameter.h"
//#define AHODO3HISTO
#ifdef AHODO3HISTO
#include <TH2.h>
#include <TFile.h>
TH1F histo_phi_H3("hodo3 phi","3-layered hodo: \\phi",100,-M_PI*1.05,M_PI*1.05);
TH1F histo_theta_H3("hodo3 theta","3-layered hodo: \\theta",100,0,1);
TH2F histo_xy_H3("hodo3 xy","3-layered hodo: x-y",100,-1,1,100,-1,1);
#endif

AHodo3Pixel::AHodo3Pixel(TPixel **pixelIn, int& numberOfPixelsIn, int maxOfPixelsIn, int** numberOfHitsIn, int** numberOfHitClustersIn, TCalibHit ***hitsIn, THitCluster ***hitClusters, TSetup &setup, const algorithm_parameter &ap):AAlgorithm("Pixel calculation for 3-layered hodoscope"), numberOfPixels(numberOfPixelsIn), maxOfPixels(maxOfPixelsIn)
{
  int idx=17,idy=18,idd=23,pid;
  idd=ap.getParam<int>(0).getData();
  idx=ap.getParam<int>(1).getData();
  idy=ap.getParam<int>(2).getData();
  pid=ap.getParam<int>(3).getData();
  if(ap.getParam<bool>(0).getData())
    {
      hitsX=(TCalibHit**)hitClusters[idx];
      hitsY=(TCalibHit**)hitClusters[idy];
      hitsD=(TCalibHit**)hitClusters[idd];
      numberOfHitsX=numberOfHitClustersIn[idx];
      numberOfHitsY=numberOfHitClustersIn[idy];
      numberOfHitsD=numberOfHitClustersIn[idd];
    }
  else
    {
      hitsX=hitsIn[idx];
      hitsY=hitsIn[idy];
      hitsD=hitsIn[idd];
      numberOfHitsX=numberOfHitsIn[idx];
      numberOfHitsY=numberOfHitsIn[idy];
      numberOfHitsD=numberOfHitsIn[idd];
    }
  pixel=pixelIn;
  for(int i=0;i<maxOfPixels;i++)pixel[i]->setID(pid);
  point3D c1,c2;
  vector3D v11,v21,v12,v22;
  fiber *shx0=(fiber*)setup.getDetectorr(idx).getShape(0);
  fiber *shxn=(fiber*)setup.getDetectorr(idx).getShape(setup.getDetectorr(idx).getNumberOfElements()-1);
  fiber *shy0=(fiber*)setup.getDetectorr(idy).getShape(0);
  fiber *shyn=(fiber*)setup.getDetectorr(idy).getShape(setup.getDetectorr(idy).getNumberOfElements()-1);
  sLine3D l1(shx0->getCorner(),shx0->getDirection(0));
  sLine3D l2(shxn->getCorner()+shxn->getDirection(1),shxn->getDirection(0));
  sLine3D l3(shy0->getCorner(),shy0->getDirection(0));
  sLine3D l4(shyn->getCorner()+shyn->getDirection(1),shyn->getDirection(0));
  point3D p1 = (l1-l3).Mid(),p2=(l1-l4).Mid(),p3=(l2-l3).Mid();
  dir1=p2-p1;
  dir2=p3-p1;
  corner=p1;

  normal=shx0->getDirection(2);//dir1^dir2;
  normal.normalize();
}
AHodo3Pixel::AHodo3Pixel(TPixel **pixelIn, int& numberOfPixelsIn, int maxOfPixelsIn, int& numberOfHitsXIn, int &numberOfHitsYIn, int& numberOfHitsDIn, TCalibHit **hitsXIn, TCalibHit **hitsYIn, TCalibHit **hitsDIn, TDetector &dx, TDetector &dy, TDetector &dz):AAlgorithm("Pixel calculation for 3-layered hodoscope"), numberOfPixels(numberOfPixelsIn), maxOfPixels(maxOfPixelsIn), numberOfHitsX(&numberOfHitsXIn), numberOfHitsY(&numberOfHitsYIn), numberOfHitsD(&numberOfHitsDIn)
{
  hitsX=hitsXIn;
  hitsY=hitsYIn;
  hitsD=hitsDIn;
  pixel=pixelIn;
  point3D c1,c2=dz.getShape(0)->getCenter();
  vector3D v11,v21,v12,v22;
  fiber *shx0=(fiber*)dx.getShape(0);
  fiber *shxn=(fiber*)dx.getShape(dx.getNumberOfElements()-1);
  fiber *shy0=(fiber*)dy.getShape(0);
  fiber *shyn=(fiber*)dy.getShape(dy.getNumberOfElements()-1);
  sLine3D l1(shx0->getCorner(),shx0->getDirection(0));
  sLine3D l2(shxn->getCorner()+shxn->getDirection(1),shxn->getDirection(0));
  sLine3D l3(shy0->getCorner(),shy0->getDirection(0));
  sLine3D l4(shyn->getCorner()+shyn->getDirection(1),shyn->getDirection(0));
  point3D p1 = (l1-l3).Mid(),p2=(l1-l4).Mid(),p3=(l2-l3).Mid();
  dir1=p2-p1;
  dir2=p3-p1;
  corner=p1;

  normal=shx0->getDirection(2);//dir1^dir2;
  normal.normalize();
}
AHodo3Pixel::~AHodo3Pixel()
{
#ifdef AHODO3HISTO
  TFile f("hodo3.root","recreate");
  f.cd();
  histo_phi_H3.Write();
  histo_theta_H3.Write();
  histo_xy_H3.Write();
  histo_phi_H3.SetDirectory(0);
  histo_theta_H3.SetDirectory(0);
  histo_xy_H3.SetDirectory(0);
  f.Close();
  cout<<"file \"data/12/analyser/hodo3.root\" written"<<endl;
#endif
}
ofstream logging("hodo3D.log");
bool definePixel(TPixel *pix, TCalibHit *h1, TCalibHit *h2, TCalibHit *h3, planeShape* sh)
{
  if(pix==NULL||sh==NULL)return false;
  if(h1==NULL&&h2==NULL&&h3==NULL)return false;
  planeShape *tmp=pix->getShape();
  pix->reset();
  if(tmp!=NULL)delete tmp;
  pix->setShape(sh);
  if(h1!=NULL)pix->addElement(*h1);
  if(h2!=NULL)pix->addElement(*h2);
  if(h3!=NULL)pix->addElement(*h3);
#ifdef AHODO3HISTO
  histo_phi_H3.Fill(sh->getCenter().Phi());
  histo_theta_H3.Fill(sh->getCenter().Theta());
  histo_xy_H3.Fill(sin(sh->getCenter().Theta())*cos(sh->getCenter().Phi()),
		sin(sh->getCenter().Theta())*sin(sh->getCenter().Phi()));
#endif
  return true;
}
void* AHodo3Pixel::process(void* ptr)
{
  numberOfPixels=0;
  lLine3D poca,p1,p2,p3,p4;
  sLine3D dline[*numberOfHitsD],xline[*numberOfHitsX],yline[*numberOfHitsY],d1,d2,xy1,xy2;
  fiber *dsh,*xsh,*ysh;
  for(int i=0;i<*numberOfHitsD;i++)
    {
      dsh=(fiber*)((hitsD[i])->getHitShape());
      if(dsh!=NULL)
	dline[i]=sLine3D(dsh->getCorner()+dsh->getDirection(1)*0.5+dsh->getDirection(2)*0.5,dsh->getDirection(0));
    }
  for(int i=0;i<*numberOfHitsX;i++)
    {
      dsh=(fiber*)((hitsX[i])->getHitShape());
      if(dsh!=NULL)
	xline[i]=sLine3D(dsh->getCorner()+dsh->getDirection(1)*0.5+dsh->getDirection(2)*0.5,dsh->getDirection(0));
    }
  for(int i=0;i<*numberOfHitsY;i++)
    {
      dsh=(fiber*)((hitsY[i])->getHitShape());
      if(dsh!=NULL)
	yline[i]=sLine3D(dsh->getCorner()+dsh->getDirection(1)*0.5+dsh->getDirection(2)*0.5,dsh->getDirection(0));
    }
  float dx,dy;
  point3D hit,h2,h3;
  vector3D hitInPlane;
  //  logging<<*numberOfHitsD<<" "<<*numberOfHitsX<<" "<<*numberOfHitsY<<endl;
  for(int i=0;i<*numberOfHitsD;i++)
    {
      if(!((hitsD[i])->isValid()))continue;
      dsh=(fiber*)((hitsD[i])->getHitShape());
      for(int j=0;j<*numberOfHitsX;j++)
	{
	  if(!hitsX[j]->isValid())continue;
	  xsh=(fiber*)hitsX[j]->getHitShape();
	  poca=dline[i]-xline[j];
	  hit=poca.Mid();
	  hitInPlane=hit-corner;
	  hitInPlane=hitInPlane-normal*(normal*hitInPlane);
	  dx=hitInPlane*dir1/(dir1.R()*dir1.R());
	  dy=hitInPlane*dir2/(dir2.R()*dir2.R());
	  if(dx>0&&dy>0&&dx<1&&dy<1)//check for 3 layered
	    for(int k=0;k<*numberOfHitsY;k++)
	      {
		if(!hitsY[k]->isValid())continue;
		ysh=(fiber*)hitsY[k]->getHitShape();
		p1=dline[i]-yline[k];
		h2=p1.Mid();
		if((hit-h2).Rho()<5)//poca.Direction().Rho()*2)
		  {
		    p2=xline[j]-yline[k];
		    h3=p2.Mid();
		    h2=h3-((dline[i]-h3)*0.5);
		    planeShape *psh=NULL;
		    psh=new circle(h3,normal,dsh->getDirection(2).R()*1.142);
		    if(definePixel(pixel[numberOfPixels], hitsD[i], hitsX[j], hitsY[k], psh))
		      numberOfPixels++;
		    if(numberOfPixels>=maxOfPixels)
		      {
			anaLog<<"Too many Pixel in three layered hodoscope"<<endli;
			return ptr;;
		      }
		  }
	      }
	  else//check for 2 layered
	    {
	      xy1=sLine3D(xsh->getCorner(),xsh->getDirection(0));
	      xy2=sLine3D(xsh->getCorner()+xsh->getDirection(1),xsh->getDirection(0));
	      d1=sLine3D(dsh->getCorner(),dsh->getDirection(0));
	      d2=sLine3D(dsh->getCorner()+dsh->getDirection(1),dsh->getDirection(0));
	      p1=xy1-d1;
	      p2=xy2-d1;
	      p3=xy2-d2;
	      p4=xy1-d2;
	      planeShape *psh=NULL;
	      psh=new quadrangle(hit,p1.Mid(),p2.Mid(),p3.Mid(),p4.Mid());
	      if(definePixel(pixel[numberOfPixels], hitsD[i], hitsX[j], NULL, psh))
		numberOfPixels++;
	      if(numberOfPixels>=maxOfPixels)
		{
		  anaLog<<"Too many Pixel in three layered hodoscope"<<endli;
		  return ptr;
		}
	    }
	}
      for(int k=0;k<*numberOfHitsY;k++)
	{
	  if(!hitsY[k]->isValid())continue;
	  ysh=(fiber*)hitsY[k]->getHitShape();
	  poca=dline[i]-yline[k];
	  hit=poca.Mid();
	  hitInPlane=hit-corner;
	  hitInPlane=hitInPlane-normal*(normal*hitInPlane);
	  dx=hitInPlane*dir1/(dir1.R()*dir1.R());
	  dy=hitInPlane*dir2/(dir2.R()*dir2.R());
	  if(!(dx>0&&dy>0&&dx<1&&dy<1))//check for 2 layered
	    {
	      xy1=sLine3D(ysh->getCorner(),ysh->getDirection(0));
	      xy2=sLine3D(ysh->getCorner()+ysh->getDirection(1),ysh->getDirection(0));
	      d1=sLine3D(dsh->getCorner(),dsh->getDirection(0));
	      d2=sLine3D(dsh->getCorner()+dsh->getDirection(1),dsh->getDirection(0));
	      p1=xy1-d1;
	      p2=xy2-d1;
	      p3=xy2-d2;
	      p4=xy1-d2;
	      planeShape *psh=NULL;
	      psh=new quadrangle(hit,p1.Mid(),p2.Mid(),p3.Mid(),p4.Mid());
	      if(definePixel(pixel[numberOfPixels], hitsD[i], hitsY[k], NULL, psh))
		numberOfPixels++;
	      if(numberOfPixels>=maxOfPixels)
		{
		  anaLog<<"Too many Pixel in three layered hodoscope"<<endli;
		  return ptr;
		}
	    }
	}
    }
  return ptr;
}

