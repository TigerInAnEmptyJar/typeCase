#include "ACompareToGIN.h"
#include "algorithmparameter.h"
//#include <qfile.h>
#include "Eparticles.h"
#include <TH2.h>
#include <fstream>
//#define DEBUGINFO
extern bool existing(string strFilename);
int getCorrespondingNumber(TTrack* compare, int nTo, TTrack** array, float maxDist, int mode);
ACompareToGIN::ACompareToGIN(TTrack **tracksIn, int& numberOfTracksIn, int &eventNumberIn, int maxTracksIn, const algorithm_parameter &descr):
  AAlgorithm("Compare tracks to GIN"),
  eventNumber(eventNumberIn),
  maxTracks(maxTracksIn),
  numberOfTracks(numberOfTracksIn),
  histoNTracks("Number Of tracks found","Number Of tracks found",21,0,20,21,0,20),
  histoDphi("\\Delta\\Phi","\\Delta\\Phi",100,-0.6,0.6),
  histoDtheta("\\Delta\\theta","\\Delta\\theta",100,-0.15,0.15),
  histoDbeta("\\Delta\\beta","\\Delta\\beta",100,-1,1),
  histoNotFound("Tracks not found","Tracks not found",31,0,30),
  histoDphiPr("Prompt: \\Delta\\Phi","Prompt: \\Delta\\Phi",100,-0.2,0.2),
  histoDthetaPr("Prompt: \\Delta\\theta","Prompt: \\Delta\\theta",100,-0.1,0.1),
  histoDbetaPr("Prompt: \\Delta\\beta","Prompt: \\Delta\\beta",100,-1,1),
  histoNotFoundPr("Prompt: Tracks not found","Prompt: Tracks not found",11,0,10),
  histoDphiN("Neutral: \\Delta\\Phi","Neutral: \\Delta\\Phi",100,-0.6,0.6),
  histoDthetaN("Neutral: \\Delta\\theta","Neutral: \\Delta\\theta",100,-0.15,0.15),
  histoDbetaN("Neutral: \\Delta\\beta","Neutral: \\Delta\\beta",100,-1,1),
  histoNotFoundN("Neutral: Tracks not found","Neutral: Tracks not found",5,0,4),
  histoDphiD("Decay: \\Delta\\Phi","Decay: \\Delta\\Phi",100,-0.5,0.5),
  histoDthetaD("Decay: \\Delta\\theta","Decay: \\Delta\\theta",100,-0.1,0.1),
  histoDbetaD("Decay: \\Delta\\beta","Decay: \\Delta\\beta",100,-1,1),
  histoNotFoundD("Decay: Tracks not found","Decay: Tracks not found",21,0,20)
{
  if(!Eparticles::IsInit())Eparticles::init();
  valid=true;
  RecTracks=tracksIn;
  InTracks=new TTrack*[maxTracks];
  for(int i=0;i<maxTracks;i++)
    InTracks[i]=new TTrack(1,5);//no need for hit or pixel structures
  maxAngularDist=descr.getParam<float>(0).getData();
  numSepDet=descr.getParam<vector<int> >(0).getData().size();
  sepDets=new int[numSepDet];
  for(int i=0;i<numSepDet;i++)
    sepDets[i]=descr.getParam<vector<int> >(0).getData().at(i);
  if(existing(descr.getParam<string>(0).getData()))
    {
      input=new ifstream();
      input->open(descr.getParam<string>(0).getData().data());
    }
  else 
    {
      cout<<"Error!!! GIN-file not found"<<endl;
      valid=false;
    }
  toCout=false;
  if(descr.getNumberOfParam<bool>()>0)
    toCout=descr.getParam<bool>(0).getData();
  outFileName=descr.getParam<string>(1).getData();
  nHistos=5+numSepDet*3;
//   histos=new TH1*[nHistos];
//   for(int i=0;i<nHistos;i++)histos[i]=NULL;
//   histos[0]=new TH2F("Number Of tracks found","Number Of tracks found",100,0,99,100,0,99);
//   histos[1]=new TH1F("\\Delta\\Phi","\\Delta\\Phi",100,-M_PI,M_PI);
//   histos[2]=new TH1F("\\Delta\\theta","\\Delta\\theta",100,-M_PI/2.,M_PI/2.);
//   histos[3]=new TH1F("\\Delta\\beta","\\Delta\\beta",100,-1,1);
//   for(int i=0;i<numSepDet;i++)
//     {
//       cout<<4+i*3<<" "<<4+i*3+1<<" "<<4+i*3+2<<endl;
//       histos[4+i*3+0]=new TH1F((string("\\Delta\\Phi tracks det ")+string_number(sepDets[i])).data(),(string("\\Delta\\Phi tracks det ")+string_number(sepDets[i])).data(),100,-M_PI,M_PI);
//       histos[4+i*3+1]=new TH1F((string("\\Delta\\theta tracks det ")+string_number(sepDets[i])).data(),(string("\\Delta\\theta tracks det ")+string_number(sepDets[i])).data(),100,-M_PI,M_PI);
//       histos[4+i*3+2]=new TH1F((string("\\Delta\\beta tracks det ")+string_number(sepDets[i])).data(),(string("\\Delta\\beta tracks det ")+string_number(sepDets[i])).data(),100,-M_PI,M_PI);
//     }
//   histos[4+numSepDet*3]=new TH1F("Tracks not found","Tracks not found",100,0,99);
//   cout<<4+numSepDet*3<<" "<<nHistos<<" "<<histos[4+numSepDet*3]<<" "<<histos[4+numSepDet*3]->GetName()<<endl;
}
#include <TCanvas.h>
#include <TF1.h>
#include <TStyle.h>
ACompareToGIN::~ACompareToGIN()
{
  delete input;
  for(int i=0;i<maxTracks;i++)
    delete InTracks[i];
  delete []InTracks;
  TCanvas *canvas=new TCanvas();
  TF1 func("function","[0]*exp(-pow((x-[1])/[2],2)/(2))");
  func.SetParameter(0,200);
  func.SetParameter(1,0);
  func.SetParameter(2,0.1);
  histoDphi.Fit(&func,"Q");
  histoDtheta.Fit(&func,"Q");
  histoDbeta.Fit(&func,"Q");
  histoDphiPr.Fit(&func,"Q");
  histoDthetaPr.Fit(&func,"Q");
  histoDbetaPr.Fit(&func,"Q");
  histoDphiN.Fit(&func,"Q");
  histoDthetaN.Fit(&func,"Q");
  histoDbetaN.Fit(&func,"Q");
  histoDphiD.Fit(&func,"Q");
  histoDthetaD.Fit(&func,"Q");
  histoDbetaD.Fit(&func,"Q");
  gStyle->SetOptFit(111);

  histoNTracks.Draw();
  canvas->Print((outFileName+"(").data());

  canvas->Divide(3,1);
  canvas->cd(1);
  histoDphi.Draw();
  canvas->cd(2);
  histoDtheta.Draw();
  canvas->cd(3);
  histoDbeta.Draw();
  canvas->Print(outFileName.data());
  canvas->cd(1);
  histoDphiPr.Draw();
  canvas->cd(2);
  histoDthetaPr.Draw();
  canvas->cd(3);
  histoDbetaPr.Draw();
  canvas->Print(outFileName.data());
  canvas->cd(1);
  histoDphiN.Draw();
  canvas->cd(2);
  histoDthetaN.Draw();
  canvas->cd(3);
  histoDbetaN.Draw();
  canvas->Print(outFileName.data());
  canvas->cd(1);
  histoDphiD.Draw();
  canvas->cd(2);
  histoDthetaD.Draw();
  canvas->cd(3);
  histoDbetaD.Draw();
  canvas->Print(outFileName.data());
  canvas->Clear();
  canvas->Divide(2,2);
  canvas->cd(1);
  histoNotFound.Draw();
  canvas->cd(2);
  histoNotFoundPr.Draw();
  canvas->cd(3);
  histoNotFoundN.Draw();
  canvas->cd(4);
  histoNotFoundD.Draw();
  canvas->Print(outFileName.data());

  histoDphi.SetLineColor(1);
  histoDtheta.SetLineColor(1);
  histoDbeta.SetLineColor(1);
  histoNotFound.SetLineColor(1);
  histoDphiPr.SetLineColor(2);
  histoDphiPr.GetFunction("function")->SetLineColor(2);
  histoDthetaPr.SetLineColor(2);
  histoDthetaPr.GetFunction("function")->SetLineColor(2);
  histoDbetaPr.SetLineColor(2);
  histoDbetaPr.GetFunction("function")->SetLineColor(2);
  histoNotFoundPr.SetLineColor(2);
  histoDphiN.SetLineColor(3);
  histoDphiN.GetFunction("function")->SetLineColor(3);
  histoDthetaN.SetLineColor(3);
  histoDthetaN.GetFunction("function")->SetLineColor(3);
  histoDbetaN.SetLineColor(3);
  histoDbetaN.GetFunction("function")->SetLineColor(3);
  histoNotFoundN.SetLineColor(3);
  histoDphiD.SetLineColor(4);
  histoDphiD.GetFunction("function")->SetLineColor(4);
  histoDthetaD.SetLineColor(4);
  histoDthetaD.GetFunction("function")->SetLineColor(4);
  histoDbetaD.SetLineColor(4);
  histoDbetaD.GetFunction("function")->SetLineColor(4);
  histoNotFoundD.SetLineColor(4);

  canvas->cd(1);
  histoDphi.Draw();
  histoDphiPr.Draw("same");
  histoDphiN.Draw("same");
  histoDphiD.Draw("same");
  canvas->cd(2);
  histoDtheta.Draw();
  histoDthetaPr.Draw("same");
  histoDthetaN.Draw("same");
  histoDthetaD.Draw("same");
  canvas->cd(3);
  histoDbeta.Draw();
  histoDbetaPr.Draw("same");
  histoDbetaN.Draw("same");
  histoDbetaD.Draw("same");
  canvas->cd(4);
  histoNotFound.Draw();
  histoNotFoundPr.Draw("same");
  histoNotFoundN.Draw("same");
  histoNotFoundD.Draw("same");
  canvas->Print((outFileName+")").data());

  /*  canvas->Divide(2,1);
  canvas->cd(1);
  canvas->cd(2);
  canvas->Print((outFileName+"(").data());
  canvas->Clear();
  canvas->Divide(3,1);
  for(int i=0;i<numSepDet+1;i++)
    {
      canvas->cd(1);
      histos[1+i*3+0]->Draw();
      canvas->cd(2);
      histos[1+i*3+1]->Draw();
      canvas->cd(3);
      histos[1+i*3+2]->Draw();
      canvas->Print(outFileName.data());
   }
   canvas->Print((outFileName+"]").data());*/
  delete canvas;
//   for(int i=0;i<nHistos;i++)
//     delete histos[i];
//   delete []histos;
}
algorithm_parameter ACompareToGIN::getDescription()
{
  algorithm_parameter ret("Compare tracks to GIN",0,0);
  vector<string> des;
  des.push_back("This algorithm reads track information, like direction");
  des.push_back("momenta, vertex from a GIN-format-file and compares this");
  des.push_back("to the tracks reconstructed in the analysis. The assignment");
  des.push_back("of the tracks to the GIN-tracks is done via minimum angle");
  des.push_back("between the tracks and the GIN-tracks.");
  ret.setDescription(des);
  vector<int> tmp;
  ret.addParam<string>(single_parameter<string>("GIN-file",""));
  ret.addParam<string>(single_parameter<string>("comparison output file","comparison.ps"));
  ret.addParam<float>(single_parameter<float>("max angle between tracks",0.05));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("separate by detectors",tmp));
  return ret;
}
vector<string>ACompareToGIN::histogramNames()
{
  vector<string> ret;
//   for(int i=0;i<nHistos;i++)
//     ret.push_back(histos[i]->GetName());
  ret.push_back(histoNTracks.GetName());
  ret.push_back(histoDphi.GetName());
  ret.push_back(histoDtheta.GetName());
  ret.push_back(histoDbeta.GetName());
  ret.push_back(histoNotFound.GetName());
  ret.push_back(histoDphiPr.GetName());
  ret.push_back(histoDthetaPr.GetName());
  ret.push_back(histoDbetaPr.GetName());
  ret.push_back(histoNotFoundPr.GetName());
  ret.push_back(histoDphiN.GetName());
  ret.push_back(histoDthetaN.GetName());
  ret.push_back(histoDbetaN.GetName());
  ret.push_back(histoNotFoundN.GetName());
  ret.push_back(histoDphiD.GetName());
  ret.push_back(histoDthetaD.GetName());
  ret.push_back(histoDbetaD.GetName());
  ret.push_back(histoNotFoundD.GetName());
  return ret;
}

TH1* ACompareToGIN::histogram(string histoName)
{
//   for(int i=0;i<nHistos;i++)
//     if(histoName==histos[i]->GetName())
//       return histos[i];
  if(histoName==histoNTracks.GetName())
    return &histoNTracks;
  else if(histoName==histoDphi.GetName())
    return &histoDphi;
  else if(histoName==histoDtheta.GetName())
    return &histoDtheta;
  else if(histoName==histoDbeta.GetName())
    return &histoDbeta;
  else if(histoName==histoNotFound.GetName())
    return &histoNotFound;
  else if(histoName==histoDphiPr.GetName())
    return &histoDphiPr;
  else if(histoName==histoDthetaPr.GetName())
    return &histoDthetaPr;
  else if(histoName==histoDbetaPr.GetName())
    return &histoDbetaPr;
  else if(histoName==histoNotFoundPr.GetName())
    return &histoNotFoundPr;
  else if(histoName==histoDphiN.GetName())
    return &histoDphiN;
  else if(histoName==histoDthetaN.GetName())
    return &histoDthetaN;
  else if(histoName==histoDbetaN.GetName())
    return &histoDbetaN;
  else if(histoName==histoNotFoundN.GetName())
    return &histoNotFoundN;
  else if(histoName==histoDphiD.GetName())
    return &histoDphiD;
  else if(histoName==histoDthetaD.GetName())
    return &histoDthetaD;
  else if(histoName==histoDbetaD.GetName())
    return &histoDbetaD;
  else if(histoName==histoNotFoundD.GetName())
    return &histoNotFoundD;
  return NULL;
}
void* ACompareToGIN::process(void *ptr)
{
  if(!valid)return ptr;
  int evtNr;
  int nt,n,m;
  float f[12];
  char li[1000];
  point3D p;
  vector3D v;
  momentum4D mom;
  (*input)>>evtNr;
  while(eventNumber>evtNr)
    {
      (*input)>>nt;
      (*input).getline(li,1000);
      for(int i=0;i<nt;i++)
	{
	  (*input).getline(li,1000);
	}
      (*input)>>evtNr;
    }
  if(evtNr!=eventNumber)
    {
      //      (*input).unget(evtNr);
      return ptr;
    }
  ///////////sort in Prompt, Vees, Kinks//////////////
  int nPrompt=0;
  int nVees=0;
  int nKinks=0;
  for(int i=0;i<numberOfTracks;i++)
    {
      if(RecTracks[i]->isSecondary())continue;
      if(RecTracks[i]->getNumberOfSecondaryTracks()<=0)nPrompt++;
      else
	{
	  if(RecTracks[i]->getNumberOfSecondaryTracks()>1)nVees++;
	  else
	    if((RecTracks[i]->getNumberOfSecondaryTracks()%2)==1)nKinks++;
	}
    }
  TTrack *Prompt[nPrompt];
  TTrack *Vees[nVees];
  TTrack *Kinks[nKinks];
  nPrompt=0;
  nVees=0;
  nKinks=0;
  for(int i=0;i<numberOfTracks;i++)
    {
      if(RecTracks[i]->isSecondary())continue;
      if(RecTracks[i]->getNumberOfSecondaryTracks()<=0)Prompt[nPrompt++]=RecTracks[i];
      else
	{
	  if(RecTracks[i]->getNumberOfSecondaryTracks()>1)Vees[nVees++]=RecTracks[i];
	  else
	    if((RecTracks[i]->getNumberOfSecondaryTracks()%2)==1)Kinks[nKinks++]=RecTracks[i];
	}
    }
  /////////////////////////
  (*input)>>nt;
  (*input).getline(li,1000);
  numberOfInTracks=nt;
  int IDs[nt];
  for(int i=0;i<nt;i++)
    InTracks[i]->reset();
  int nInPrompt=0,nInVees=0,nInKinks=0;
  for(int i=0;i<nt;i++)
    {
      (*input)>>n>>IDs[i];
      for(int j=0;j<12;j++)(*input)>>f[j];
      (*input)>>m;
      //      if(m==-1)nPrompt++;
      p.setValues(f[0]*10,f[1]*10,f[2]*10);
      v.setValues(f[3],f[4],f[5]);
      mom.setPM(v,Eparticles::getMassG(IDs[i]));
      InTracks[i]->reset();
      InTracks[i]->setPath(sLine3D(p,v));
      InTracks[i]->setSpeed(mom.Beta());
      InTracks[i]->setCharge((int)Eparticles::getChargeG(IDs[i]));
      InTracks[i]->setParticle(mom);
      if(m>=0&&m<maxTracks)
	{
	  InTracks[i]->setOrder();
	  InTracks[i]->setPrimary(InTracks[m]);
	  InTracks[m]->addSecondary(InTracks[i]);
	}
    }
  //////////////////////////
  for(int i=0;i<numberOfInTracks;i++)
    {
      if(InTracks[i]->isSecondary())continue;
      if(InTracks[i]->getNumberOfSecondaryTracks()<=0)nInPrompt++;
      else
	{
	  cout<<IDs[i]<<" "<<Eparticles::getLifeTimeG(IDs[i])<<" "<<InTracks[i]->getSecondary(0)->getPath().Foot()<<InTracks[i]->getNumberOfSecondaryTracks()<<endl;
	  if(InTracks[i]->getSecondary(0)->getPath().Foot().Z()>400||Eparticles::getLifeTimeG(IDs[i])>1||InTracks[i]->getCharge())nInPrompt++;
	  else if(InTracks[i]->getCharge()==0&&InTracks[i]->getNumberOfSecondaryTracks()>1)nInVees++;
	  else
	    if(InTracks[i]->getCharge()!=0&&(InTracks[i]->getNumberOfSecondaryTracks()%2)==1)nInKinks++;
	}
    }
  TTrack *InPrompt[nInPrompt];
  TTrack *InVees[nInVees];
  TTrack *InKinks[nInKinks];
  nInPrompt=0;
  nInVees=0;
  nInKinks=0;
  for(int i=0;i<numberOfInTracks;i++)
    {
      if(InTracks[i]->isSecondary())continue;
      if(InTracks[i]->getNumberOfSecondaryTracks()<=0)InPrompt[nInPrompt++]=InTracks[i];
      else
	{
	  if(InTracks[i]->getSecondary(0)->getPath().Foot().Z()>400||Eparticles::getLifeTimeG(IDs[i])>1||InTracks[i]->getCharge())
	    InPrompt[nInPrompt++]=InTracks[i];
	  else if(InTracks[i]->getCharge()==0&&InTracks[i]->getNumberOfSecondaryTracks()>1)InVees[nInVees++]=InTracks[i];
	  else
	    if(InTracks[i]->getCharge()!=0&&(InTracks[i]->getNumberOfSecondaryTracks()%2)==1)InKinks[nInKinks++]=InTracks[i];
	}
    }
  /////////////////////////////////////
  int InFound[nInPrompt+nInVees+nInKinks];
  int num=-1;
  for(int i=0;i<nInPrompt;i++)
    {
      InFound[i]=getCorrespondingNumber(InPrompt[i], nPrompt, Prompt, maxAngularDist, 0);
    }
  for(int i=0;i<nInVees;i++)
    {
      InFound[i+nInPrompt]=getCorrespondingNumber(InVees[i], nVees, Vees, maxAngularDist, -1);
    }
  for(int i=0;i<nInKinks;i++)
    {
      InFound[i+nInPrompt+nInVees]=getCorrespondingNumber(InKinks[i], nKinks, Kinks, maxAngularDist, 0);
    }
  ///////misstaken///////////
  int misstaken[(nInPrompt+nInVees+nInKinks)*3];
  for(int i=0;i<(nInPrompt+nInVees+nInKinks)*3;i++)misstaken[i]=-1;
  for(int i=0;i<nInPrompt;i++)
    {
      if(InFound[i]>=0)continue;
      misstaken[i]=getCorrespondingNumber(InPrompt[i], nVees, Vees, maxAngularDist, 0);
      if(misstaken[i]>=0){misstaken[i+(nInPrompt+nInVees+nInKinks)*2]=-1;misstaken[i+(nInPrompt+nInVees+nInKinks)]=-1;continue;}
      misstaken[i+(nInPrompt+nInVees+nInKinks)]=getCorrespondingNumber(InPrompt[i], nKinks, Kinks, maxAngularDist, 0);
      if(misstaken[i+(nInPrompt+nInVees+nInKinks)]>=0){misstaken[i+(nInPrompt+nInVees+nInKinks)*2]=-1;continue;}
      misstaken[i+(nInPrompt+nInVees+nInKinks)*2]=getCorrespondingNumber(InPrompt[i], nVees, Vees, maxAngularDist, 2);
      if(misstaken[i+(nInPrompt+nInVees+nInKinks)*2]==-1)
	misstaken[i+(nInPrompt+nInVees+nInKinks)*2]=getCorrespondingNumber(InPrompt[i], nKinks, Kinks, maxAngularDist, 1);
    }
  for(int i=0;i<nInVees;i++)
    {
      if(InFound[i+nInPrompt]>=0)continue;
      misstaken[i+nInPrompt]=getCorrespondingNumber(InVees[i]->getSecondary(0), nPrompt, Prompt, maxAngularDist, 0);
      if(misstaken[i+nInPrompt]<0)
	misstaken[i+nInPrompt]=getCorrespondingNumber(InVees[i]->getSecondary(1), nPrompt, Prompt, maxAngularDist, 0);
      if(misstaken[i+nInPrompt]>=0){misstaken[i+nInPrompt+(nInPrompt+nInVees+nInKinks)*2]=-1;misstaken[i+nInPrompt+(nInPrompt+nInVees+nInKinks)]=-1;continue;}
    }
  for(int i=0;i<nInKinks;i++)
    {
      if(InFound[i+nInPrompt+nInVees]>=0)continue;
      misstaken[i+nInPrompt+nInVees]=getCorrespondingNumber(InKinks[i], nPrompt, Prompt, maxAngularDist, 0);
      if(misstaken[i+nInPrompt+nInVees]>=0){misstaken[i+(nInPrompt+nInVees+nInKinks)*2+nInPrompt+nInVees]=-1;misstaken[i+(nInPrompt+nInVees+nInKinks)+nInPrompt+nInVees]=-1;continue;}
      misstaken[i+nInPrompt+nInVees+(nInPrompt+nInVees+nInKinks)]=getCorrespondingNumber(InKinks[i]->getSecondary(0), nPrompt, Prompt, maxAngularDist, 0);
      if(misstaken[i+nInPrompt+nInVees+(nInPrompt+nInVees+nInKinks)]>=0){misstaken[i+nInPrompt+nInVees+(nInPrompt+nInVees+nInKinks)*2]=-1;continue;}
    }
  //////////////////
  /////////other way round////////////////////////////
  int Found[nPrompt+nVees+nKinks];
  for(int i=0;i<nPrompt;i++)
    {
      num=-1;
      for(int j=0;j<nInPrompt;j++)
	if(InFound[j]==i)
	  num=j;
      Found[i]=num;//getCorrespondingNumber(Prompt[i], nInPrompt, InPrompt, maxAngularDist, 0);
    }
  for(int i=0;i<nVees;i++)
    {
      num=-1;
      for(int j=0;j<nInVees;j++)
	if(InFound[j+nPrompt]==i)
	  num=j;
      Found[i+nPrompt]=num;//getCorrespondingNumber(Vees[i], nInVees, InVees, maxAngularDist, -1);
    }
  for(int i=0;i<nKinks;i++)
    {
      num=-1;
      for(int j=0;j<nInKinks;j++)
	if(InFound[j+nPrompt+nVees]==i)
	  num=j;
      Found[i+nPrompt+nVees]=num;//getCorrespondingNumber(Kinks[i], nInKinks, InKinks, maxAngularDist, 0);
    }
  ///////misstaken///////////
  int Rec_misstaken[(nPrompt+nVees+nKinks)*3];
  for(int i=0;i<(nPrompt+nVees+nKinks)*3;i++)Rec_misstaken[i]=-1;
  for(int i=0;i<nPrompt;i++)
    {
      if(Found[i]>=0)continue;
      Rec_misstaken[i]=getCorrespondingNumber(Prompt[i], nInVees, InVees, maxAngularDist, 0);
      if(Rec_misstaken[i]>=0){Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]=-1;Rec_misstaken[i+(nPrompt+nVees+nKinks)]=-1;continue;}
      Rec_misstaken[i+(nPrompt+nVees+nKinks)]=getCorrespondingNumber(Prompt[i], nInKinks, InKinks, maxAngularDist, 0);
      if(Rec_misstaken[i+(nPrompt+nVees+nKinks)]>=0){Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]=-1;continue;}
      Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]=getCorrespondingNumber(Prompt[i], nInVees, InVees, maxAngularDist*1.2, 2);
      if(Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]==-1)
	Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]=getCorrespondingNumber(Prompt[i], nInKinks, InKinks, maxAngularDist, 1);
    }
  for(int i=0;i<nVees;i++)
    {
      if(Found[i+nPrompt]>=0)continue;
      Rec_misstaken[i+nPrompt]=getCorrespondingNumber(Vees[i]->getSecondary(0), nInPrompt, InPrompt, maxAngularDist, 0);
      if(Rec_misstaken[i+nPrompt]<0)
	Rec_misstaken[i+nPrompt]=getCorrespondingNumber(Vees[i]->getSecondary(1), nInPrompt, InPrompt, maxAngularDist, 0);
      if(Rec_misstaken[i+nPrompt]>=0){Rec_misstaken[i+nPrompt+(nPrompt+nVees+nKinks)*2]=-1;Rec_misstaken[i+nPrompt+(nPrompt+nVees+nKinks)]=-1;continue;}
    }
  for(int i=0;i<nKinks;i++)
    {
      if(Found[i+nPrompt+nVees]>=0)continue;
      Rec_misstaken[i+nPrompt+nVees]=getCorrespondingNumber(Kinks[i], nInPrompt, InPrompt, maxAngularDist, 0);
      if(Rec_misstaken[i+nPrompt+nVees]>=0){Rec_misstaken[i+(nPrompt+nVees+nKinks)*2+nPrompt+nVees]=-1;Rec_misstaken[i+(nPrompt+nVees+nKinks)+nPrompt+nVees]=-1;continue;}
      Rec_misstaken[i+nPrompt+nVees+(nPrompt+nVees+nKinks)]=getCorrespondingNumber(Kinks[i]->getSecondary(0), nInPrompt, InPrompt, maxAngularDist, 0);
      if(Rec_misstaken[i+nPrompt+nVees+(nPrompt+nVees+nKinks)]>=0){Rec_misstaken[i+nPrompt+nVees+(nPrompt+nVees+nKinks)*2]=-1;continue;}
    }
  //////////////////
  /*
  int corresponds[numberOfTracks],backC[numberOfInTracks];
  for(int i=0;i<numberOfTracks;i++)corresponds[i]=-1;
  for(int i=0;i<numberOfInTracks;i++)backC[i]=-1;
  for(int i=0;i<numberOfTracks;i++)
    {
      num=-1;
      for(int j=0;j<numberOfInTracks;j++)
	{
	  if(backC[j]>=0)continue;
	  if(num==-1)
	    {
	      num=j;
	      continue;
	    }
	  if(acos(InTracks[j]->getPath().Direction()*RecTracks[i]->getPath().Direction())<acos(InTracks[num]->getPath().Direction()*RecTracks[i]->getPath().Direction()))
	    {
	      num=j;
	    }
	}
      if(num==-1)continue;
      if(acos(InTracks[num]->getPath().Direction()*RecTracks[i]->getPath().Direction())>maxAngularDist)continue;
      backC[num]=i;
      corresponds[i]=num;
    }
  int nTracksNotFound=0;
  histoNTracks.Fill(numberOfTracks,numberOfInTracks);
  // histos[0]->Fill(numberOfTracks,numberOfInTracks);
#ifdef DEBUGINFO
  cout<<"In tracks:"<<endl;
  for(int i=0;i<numberOfInTracks;i++)
    cout<<IDs[i]<<InTracks[i]->getPath().toString(_spheric_*4).data()<<endl;
  cout<<"reconstructed:"<<endl;
  for(int i=0;i<numberOfTracks;i++)
    cout<<corresponds[i]<<RecTracks[i]->getPath().toString(_spheric_*4).data()<<endl;
#endif
  */
  if(toCout)
    {
      cout<<"In:"<<endl;
      for(int i=0;i<nInPrompt;i++)
	{
	  num=0;
	  for(int j=0;j<numberOfInTracks;j++)if(InPrompt[i]==InTracks[j])num=j;
	  if(InPrompt[i]->getNumberOfSecondaryTracks()>0)
	    cout<<InFound[i]<<" "<<IDs[num]<<" "<<InPrompt[i]->getPath().Direction().toString(_spheric_).data()<<InPrompt[i]->getSecondary(0)->getPath().Foot()<<endl;
	  else
	    cout<<InFound[i]<<" "<<IDs[num]<<" "<<InPrompt[i]->getPath().Direction().toString(_spheric_).data()<<endl;
	}
      for(int i=0;i<nInVees;i++)
	{
	  for(int j=0;j<numberOfInTracks;j++)if(InVees[i]==InTracks[j])num=j;
	  cout<<InFound[i+nInPrompt]<<" "<<IDs[num]<<" "<<InVees[i]->getPath().Direction().toString(_spheric_).data()<<flush;
	  if(InVees[i]->getNumberOfSecondaryTracks()>0)
	    {
	      if(InVees[i]->getSecondary(0)->getPath().Foot().Z()>3200||(IDs[num]!=18))
		cout<<endl;
	      else
		{
		  cout<<InVees[i]->getSecondary(0)->getPath().Foot()<<endl;
		  for(int j=0;j<InVees[i]->getNumberOfSecondaryTracks();j++)
		    cout<<"   "<<" d="<<InVees[i]->getSecondary(j)->getPath().Direction().toString(_spheric_).data()<<endl;
		}
	    }
	  else
	    cout<<endl;
	}
      for(int i=0;i<nInKinks;i++)
	{
	  for(int j=0;j<numberOfInTracks;j++)if(InKinks[i]==InTracks[j])num=j;
	  cout<<InFound[i+nInPrompt+nInVees]<<" "<<IDs[num]<<" "<<InKinks[i]->getPath().Direction().toString(_spheric_).data()<<flush;
	  if(InKinks[i]->getNumberOfSecondaryTracks()>0)
	    {
	      if(InKinks[i]->getSecondary(0)->getPath().Foot().Z()>3200||(IDs[num]!=18))
		cout<<endl;
	      else
		{
		  cout<<InKinks[i]->getSecondary(0)->getPath().Foot()<<endl;
		  for(int j=0;j<InKinks[i]->getNumberOfSecondaryTracks();j++)
		    cout<<"   "<<" d="<<InKinks[i]->getSecondary(j)->getPath().Direction().toString(_spheric_).data()<<endl;
		}
	    }
	  else
	    cout<<endl;
	}
      cout<<"Reconstructed: "<<endl;
      for(int i=0;i<nPrompt;i++)
	{
	  cout<<Found[i]<<" "<<Prompt[i]->getPath().Direction().toString(_spheric_).data();
	  if(Found[i]>=0)
	    cout<<endl;
	  else 
	    {
	      if(Rec_misstaken[i]>0)
		cout<<"misstaken as neutral: "<<Rec_misstaken[i]<<endl;
	      else if(Rec_misstaken[i+(nPrompt+nVees+nKinks)]>0)
		cout<<"misstaken as kink: "<<Rec_misstaken[i+(nPrompt+nVees+nKinks)]<<endl;
	      else if(Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]>0)
		cout<<"misstaken as secondary: "<<Rec_misstaken[i+(nPrompt+nVees+nKinks)*2]<<endl;
	      else
		cout<<"not found"<<endl;
	    }
	}
      for(int i=0;i<nVees;i++)
	{
	  cout<<Found[i+nPrompt]<<" "<<Vees[i]->getPath().Direction().toString(_spheric_).data()<<flush;
	  if(Vees[i]->getNumberOfSecondaryTracks()>0)
	    {
	      cout<<Vees[i]->getSecondary(0)->getPath().Foot()<<endl;
	      for(int j=0;j<Vees[i]->getNumberOfSecondaryTracks();j++)
		cout<<"   "<<" d="<<Vees[i]->getSecondary(j)->getPath().Direction().toString(_spheric_).data()<<endl;
	    }
	  else
	    cout<<endl;
	}
      for(int i=0;i<nKinks;i++)
	{
	  cout<<Found[i+nPrompt+nVees]<<" "<<Kinks[i]->getPath().Direction().toString(_spheric_).data()<<flush;
	  if(Kinks[i]->getNumberOfSecondaryTracks()>0)
	    {
	      cout<<Kinks[i]->getSecondary(0)->getPath().Foot()<<endl;
	      for(int j=0;j<Kinks[i]->getNumberOfSecondaryTracks();j++)
		cout<<"   "<<" d="<<Kinks[i]->getSecondary(j)->getPath().Direction().toString(_spheric_).data()<<endl;
	    }
	  else
	    cout<<endl;
	}
    }
  for(int i=0;i<nPrompt;i++)
    {
      if(Found[i]<0){continue;}
      histoDphi./*histos[1]->*/Fill(Prompt[i]->getPath().Direction().Phi()
		      -InPrompt[Found[i]]->getPath().Direction().Phi());
      histoDtheta./*histos[2]->*/Fill(Prompt[i]->getPath().Direction().Theta()
		      -InPrompt[Found[i]]->getPath().Direction().Theta());
      histoDbeta./*histos[3]->*/Fill(Prompt[i]->getSpeed()
		      -InPrompt[Found[i]]->getSpeed());
      histoDphiPr./*histos[1]->*/Fill(Prompt[i]->getPath().Direction().Phi()
		      -InPrompt[Found[i]]->getPath().Direction().Phi());
      histoDthetaPr./*histos[2]->*/Fill(Prompt[i]->getPath().Direction().Theta()
		      -InPrompt[Found[i]]->getPath().Direction().Theta());
      histoDbetaPr./*histos[3]->*/Fill(Prompt[i]->getSpeed()
		      -InPrompt[Found[i]]->getSpeed());
    }
  for(int i=0;i<nVees;i++)
    {
      if(Found[i+nPrompt]<0){continue;}
      histoDphi./*histos[1]->*/Fill(Vees[i]->getPath().Direction().Phi()
		      -InVees[Found[i+nPrompt]]->getPath().Direction().Phi());
      histoDtheta./*histos[2]->*/Fill(Vees[i]->getPath().Direction().Theta()
		      -InVees[Found[i+nPrompt]]->getPath().Direction().Theta());
      histoDbeta./*histos[3]->*/Fill(Vees[i]->getSpeed()
		      -InVees[Found[i+nPrompt]]->getSpeed());
      histoDphiN./*histos[1]->*/Fill(Vees[i]->getPath().Direction().Phi()
		      -InVees[Found[i+nPrompt]]->getPath().Direction().Phi());
      histoDthetaN./*histos[2]->*/Fill(Vees[i]->getPath().Direction().Theta()
		      -InVees[Found[i+nPrompt]]->getPath().Direction().Theta());
      histoDbetaN./*histos[3]->*/Fill(Vees[i]->getSpeed()
		      -InVees[Found[i+nPrompt]]->getSpeed());
      for(int j=0;j<Vees[i]->getNumberOfSecondaryTracks();j++)
	{
	  num=0;
	  float a=Vees[i]->getSecondary(j)->getPath().Direction()*InVees[Found[i+nPrompt]]->getSecondary(0)->getPath().Direction(),b;
	  for(int k=1;k<InVees[Found[i+nPrompt]]->getNumberOfSecondaryTracks();k++)
	    {
	      b=Vees[i]->getSecondary(j)->getPath().Direction()*InVees[Found[i+nPrompt]]->getSecondary(k)->getPath().Direction();
	      if(a>b)
		num=k;
	    }
	  histoDphi./*histos[1]->*/Fill(Vees[i]->getSecondary(j)->getPath().Direction().Phi()
					-InVees[Found[i+nPrompt]]->getSecondary(num)->getPath().Direction().Phi());
	  histoDtheta./*histos[2]->*/Fill(Vees[i]->getSecondary(j)->getPath().Direction().Theta()
					  -InVees[Found[i+nPrompt]]->getSecondary(num)->getPath().Direction().Theta());
	  histoDbeta./*histos[3]->*/Fill(Vees[i]->getSecondary(j)->getSpeed()
					 -InVees[Found[i+nPrompt]]->getSecondary(num)->getSpeed());
	  histoDphiD./*histos[1]->*/Fill(Vees[i]->getSecondary(j)->getPath().Direction().Phi()
					-InVees[Found[i+nPrompt]]->getSecondary(num)->getPath().Direction().Phi());
	  histoDthetaD./*histos[2]->*/Fill(Vees[i]->getSecondary(j)->getPath().Direction().Theta()
					  -InVees[Found[i+nPrompt]]->getSecondary(num)->getPath().Direction().Theta());
	  histoDbetaD./*histos[3]->*/Fill(Vees[i]->getSecondary(j)->getSpeed()
					 -InVees[Found[i+nPrompt]]->getSecondary(num)->getSpeed());
	}
    }
  for(int i=0;i<nKinks;i++)
    {
      if(Found[i+nPrompt+nVees]<0){continue;}
      histoDphi./*histos[1]->*/Fill(Kinks[i]->getPath().Direction().Phi()
		      -InKinks[Found[i+nPrompt+nVees]]->getPath().Direction().Phi());
      histoDtheta./*histos[2]->*/Fill(Kinks[i]->getPath().Direction().Theta()
		      -InKinks[Found[i+nPrompt+nVees]]->getPath().Direction().Theta());
      histoDbeta./*histos[3]->*/Fill(Kinks[i]->getSpeed()
		      -InKinks[Found[i+nPrompt+nVees]]->getSpeed());
      for(int j=0;j<Kinks[i]->getNumberOfSecondaryTracks();j++)
	{
	  num=0;
	  float a=Kinks[i]->getSecondary(j)->getPath().Direction()*InKinks[Found[i+nPrompt+nVees]]->getSecondary(0)->getPath().Direction(),b;
	  for(int k=1;k<InKinks[Found[i+nPrompt+nVees]]->getNumberOfSecondaryTracks();k++)
	    {
	      b=Kinks[i]->getSecondary(j)->getPath().Direction()*InKinks[Found[i+nPrompt+nVees]]->getSecondary(k)->getPath().Direction();
	      if(a>b)
		num=k;
	    }
	  histoDphi./*histos[1]->*/Fill(Kinks[i]->getSecondary(j)->getPath().Direction().Phi()
					-InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getPath().Direction().Phi());
	  histoDtheta./*histos[2]->*/Fill(Kinks[i]->getSecondary(j)->getPath().Direction().Theta()
					  -InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getPath().Direction().Theta());
	  histoDbeta./*histos[3]->*/Fill(Kinks[i]->getSecondary(j)->getSpeed()
					 -InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getSpeed());
	  histoDphiD./*histos[1]->*/Fill(Kinks[i]->getSecondary(j)->getPath().Direction().Phi()
					-InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getPath().Direction().Phi());
	  histoDthetaD./*histos[2]->*/Fill(Kinks[i]->getSecondary(j)->getPath().Direction().Theta()
					  -InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getPath().Direction().Theta());
	  histoDbetaD./*histos[3]->*/Fill(Kinks[i]->getSecondary(j)->getSpeed()
					 -InKinks[Found[i+nPrompt+nVees]]->getSecondary(num)->getSpeed());
	}
    }
 //  /*      if(RecPrompt[i]->isSecondary())
// 	{
// 	  histoDphiD./*histos[1]->*/Fill(RecTracks[i]->getPath().Direction().Phi()
// 		      -InTracks[Found[i]]->getPath().Direction().Phi());
// 	  histoDthetaD./*histos[2]->*/Fill(RecTracks[i]->getPath().Direction().Theta()
// 		      -InTracks[Found[i]]->getPath().Direction().Theta());
// 	  histoDbetaD./*histos[3]->*/Fill(RecTracks[i]->getSpeed()
// 		      -InTracks[Found[i]]->getSpeed());
// 	}
//       else
// 	{
// 	  if(RecTracks[i]->getNumberOfSecondaryTracks()<=0)
// 	    {
//       histoDphiPr./*histos[1]->*/Fill(RecTracks[i]->getPath().Direction().Phi()
// 		      -InTracks[Found[i]]->getPath().Direction().Phi());
//       histoDthetaPr./*histos[2]->*/Fill(RecTracks[i]->getPath().Direction().Theta()
// 		      -InTracks[Found[i]]->getPath().Direction().Theta());
//       histoDbetaPr./*histos[3]->*/Fill(RecTracks[i]->getSpeed()
// 		      -InTracks[Found[i]]->getSpeed());

// 	    }
// 	  else
// 	    {
//       histoDphiN./*histos[1]->*/Fill(RecTracks[i]->getPath().Direction().Phi()
// 		      -InTracks[Found[i]]->getPath().Direction().Phi());
//       histoDthetaN./*histos[2]->*/Fill(RecTracks[i]->getPath().Direction().Theta()
// 		      -InTracks[Found[i]]->getPath().Direction().Theta());
//       histoDbetaN./*histos[3]->*/Fill(RecTracks[i]->getSpeed()
// 		      -InTracks[Found[i]]->getSpeed());
// 	    }
// 	}*/
//       for(int j=0;j<numSepDet;j++)
// 	{
// 	  if(RecTracks[i]->getNumberOfCalibHits(sepDets[j])<=0)continue;
// 	  histos[4+j*3+0]->Fill(RecTracks[i]->getPath().Direction().Phi()
// 				-InTracks[corresponds[i]]->getPath().Direction().Phi());
// 	  histos[4+j*3+1]->Fill(RecTracks[i]->getPath().Direction().Theta()
// 				-InTracks[corresponds[i]]->getPath().Direction().Theta());
// 	  histos[4+j*3+2]->Fill(RecTracks[i]->getSpeed()
// 				-InTracks[corresponds[i]]->getSpeed());

// 	}
//}
//x  int nTrNFPr=0,nTrNFN=0,nTrNFD=0;
  for(int i=0;i<numberOfInTracks;i++)
    {
      if(IDs[i]==18)
	{
	if(InTracks[i]->getNumberOfSecondaryTracks()>0)
	  {
	    point3D pp;
	    pp=InTracks[i]->getSecondary(0)->getPath().Foot();
#ifdef DEBUGINFO
	    cout<<pp<<(pp.Z()<26||pp.Z()>200?"no way":"reconstructable")<<endl;
#endif
	    //can only reconstruct lambda decay between Microstrip and second Hodo
	    if(pp.Z()<26||pp.Z()>200)
	      return ptr;
	  }
	}
    }
//   for(int i=0;i<numberOfInTracks;i++)
//     {
//       //      cout<<i<<" "<<backC[i]<<endl;
//       if(backC[i]<0)
// 	{
// 	  nTracksNotFound++;
// 	  if(InTracks[i]->isSecondary())
// 	    nTrNFD++;
// 	  else
// 	    {
// 	      if(InTracks[i]->getCharge()==0)
// 		nTrNFN++;
// 	      else
// 		nTrNFPr++;
// 	    }
// 	}
//     }
//   //  cout<<4+numSepDet*3<<" "<<nHistos<<" "<<histos[4+numSepDet*3]<<" "<<nTracksNotFound<<endl;
//   histoNotFound.Fill(nTracksNotFound);
//   histoNotFoundPr.Fill(nTrNFPr);
//   histoNotFoundD.Fill(nTrNFD);
//   histoNotFoundN.Fill(nTrNFN);
// //   cout<<histos[4+numSepDet*3]->GetName()<<endl;
// //   histos[4+numSepDet*3]->Fill(nTracksNotFound);
  return ptr;
}
int getCorrespondingNumber(TTrack* compare, int nTo, TTrack** array, float maxDist, int mode)
{
  int num=0;
  int ret=-1;
  //cout<<"getCorrespondingNumber(compare="<<compare<<", nTo="<<nTo<<", array="<<array<<", mode="<<mode<<") "<<endl;
  if(mode==0)
    {
      if(nTo==0)return -1;
      for(int j=1;j<nTo;j++)
	if(acos(compare->getPath().Direction()*array[j]->getPath().Direction())<acos(compare->getPath().Direction()*array[num]->getPath().Direction()))
	  {
	    num=j;
	  }
      if(num==0)
	{
	  if(acos(compare->getPath().Direction()*array[num]->getPath().Direction())>maxDist)
	    ret=-1;
	  else ret=num;
	}
      else
	ret=num;
    }
  else if(mode==-1)
    {
      if(compare->getNumberOfSecondaryTracks()<2)return -1;
      if(nTo==0)return -1;
      float a1,a2,a3,a4;
      float b1,b2,b3,b4;
      b1=acos(compare->getSecondary(0)->getPath().Direction()*array[0]->getSecondary(0)->getPath().Direction());
      b2=acos(compare->getSecondary(1)->getPath().Direction()*array[0]->getSecondary(1)->getPath().Direction());
      b3=acos(compare->getSecondary(1)->getPath().Direction()*array[0]->getSecondary(0)->getPath().Direction());
      b4=acos(compare->getSecondary(0)->getPath().Direction()*array[0]->getSecondary(1)->getPath().Direction());
      for(int j=1;j<nTo;j++)
	{
	  if(array[j]->getNumberOfSecondaryTracks()<2)continue;
	  a1=acos(compare->getSecondary(0)->getPath().Direction()*array[j]->getSecondary(0)->getPath().Direction());
	  a2=acos(compare->getSecondary(1)->getPath().Direction()*array[j]->getSecondary(1)->getPath().Direction());
	  a3=acos(compare->getSecondary(1)->getPath().Direction()*array[j]->getSecondary(0)->getPath().Direction());
	  a4=acos(compare->getSecondary(0)->getPath().Direction()*array[j]->getSecondary(1)->getPath().Direction());
	  if((a1<b1&&a2<b2)||(a3<b3&&a4<b4))
	    {
	      num=j;
	      b1=a1;b2=a2;b3=a3;b4=a4;
	    }
	}
      if(num==0)
	{
	  if((b1>maxDist||b2>maxDist)&&(b3>maxDist||b4>maxDist))
	    ret=-1;
	  else ret=num;
	}
      else
	ret=num;
    }
  else
    {
      if(nTo==0)return -1;
      for(int j=1;j<nTo;j++)
	{
	  for(int k=0;k<mode;k++)
	    {
	      if(k>=array[j]->getNumberOfSecondaryTracks())continue;
	      if(cos(compare->getPath().Direction()*array[j]->getSecondary(k)->getPath().Direction())<acos(compare->getPath().Direction()*array[num/mode]->getSecondary(num%mode)->getPath().Direction()))
		{
		  num=j*mode+k;
		}
	    }
	}
      if(num==0)
	{
	  if(acos(compare->getPath().Direction()*array[num/mode]->getSecondary(num%mode)->getPath().Direction())>maxDist)
	    ret=-1;
	  else ret=num;
	}
      else
	ret=num;
    }
  return ret;
}
