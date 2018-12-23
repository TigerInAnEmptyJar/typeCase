#include "ATrackTreeOutput.h"
#include "algorithmparameter.h"
#include "Eparticles.h"
#include <TFile.h>
#include <TSystem.h>
#include "logger.h"
extern logger readWriteLog;
#include "RbaseReaction.h"
#include "container_utilities.h"
void ATrackTreeOutput::initialize(const string &filename)
{
  string fn=filename;
  if(localDirectory)
    {
      copyFile=filename;
      if(hasA(copyFile,"/"))
	copyFile=copyFile.substr(copyFile.rfind("/")+1,copyFile.length()-copyFile.rfind("/")-1);
      copyFile=directory+copyFile;
      if(copyFile==filename){copyFile="";}
      else{
	cout<<"temporary write to \""<<copyFile.data()<<"\""<<endl;
	fn=copyFile;
      }
    }
  readWriteLog<<"Open file \""<<fn.data()<<"\" for track tree output"<<endli;
  if(trackTree!=NULL)finalize();
  TFile *f=new TFile(fn.data(),"recreate");
  f->cd();
  trackTree=new TTree("trackTree","trackTree");
  trackTree->Branch("eventNumber",&Event,"eventNumber/I");
  trackTree->Branch("runNumber",&Run,"runNumber/I");
  trackTree->Branch("trigger",&Trig,"trigger/I");
  trackTree->Branch("numberOfPrompt",&nPrompt,"numberOfPrompt/I");
  trackTree->Branch("numberOfVee",&nVee,"numberOfVee/I");
  trackTree->Branch("numberOfKink",&nKink,"numberOfKink/I");
  trackTree->Branch("numberOfPromptHits",&nPromptHit,"numberOfPromptHits/I");
  trackTree->Branch("numberOfVeeHits",&nVeeHit,"numberOfVeeHits/I");
  trackTree->Branch("numberOfKinkHits",&nKinkHit,"numberOfKinkHits/I");
  trackTree->Branch("prompt.theta",&outPrompt.theta,"prompt.theta[numberOfPrompt]/F");
  trackTree->Branch("prompt.phi",&outPrompt.phi,"prompt.phi[numberOfPrompt]/F");
  trackTree->Branch("prompt.beta",&outPrompt.beta,"prompt.beta[numberOfPrompt]/F");
  trackTree->Branch("prompt.theta_kf",&outPrompt.theta_kf,"prompt.theta_kf[numberOfPrompt]/F");
  trackTree->Branch("prompt.phi_kf",&outPrompt.phi_kf,"prompt.phi_kf[numberOfPrompt]/F");
  trackTree->Branch("prompt.beta_kf",&outPrompt.beta_kf,"prompt.beta_kf[numberOfPrompt]/F");
  trackTree->Branch("prompt.vertex_x",&outPrompt.vertex_x,"prompt.vertex_x[numberOfPrompt]/F");
  trackTree->Branch("prompt.vertex_y",&outPrompt.vertex_y,"prompt.vertex_y[numberOfPrompt]/F");
  trackTree->Branch("prompt.vertex_z",&outPrompt.vertex_z,"prompt.vertex_z[numberOfPrompt]/F");
  trackTree->Branch("prompt.chi",&outPrompt.chi,"prompt.chi[numberOfPrompt]/F");
  trackTree->Branch("prompt.nEl",&outPrompt.nel,"prompt.nEl[numberOfPrompt]/S");
  trackTree->Branch("prompt.ID",&outPrompt.id,"prompt.ID[numberOfPrompt]/S");
  trackTree->Branch("prompt.el",&outPrompt.el,"prompt.el[numberOfPromptHits]/S");
  trackTree->Branch("prompt.det",&outPrompt.det,"prompt.det[numberOfPromptHits]/S");
  trackTree->Branch("vee.theta1",&outVee.theta1,"vee.theta1[numberOfVee]/F");
  trackTree->Branch("vee.phi1",&outVee.phi1,"vee.phi1[numberOfVee]/F");
  trackTree->Branch("vee.beta1",&outVee.beta1,"vee.beta1[numberOfVee]/F");
  trackTree->Branch("vee.theta2",&outVee.theta2,"vee.theta2[numberOfVee]/F");
  trackTree->Branch("vee.phi2",&outVee.phi2,"vee.phi2[numberOfVee]/F");
  trackTree->Branch("vee.beta2",&outVee.beta2,"vee.beta2[numberOfVee]/F");
  trackTree->Branch("vee.theta1_kf",&outVee.theta1_kf,"vee.theta1_kf[numberOfVee]/F");
  trackTree->Branch("vee.phi1_kf",&outVee.phi1_kf,"vee.phi1_kf[numberOfVee]/F");
  trackTree->Branch("vee.beta1_kf",&outVee.beta1_kf,"vee.beta1_kf[numberOfVee]/F");
  trackTree->Branch("vee.theta2_kf",&outVee.theta2_kf,"vee.theta2_kf[numberOfVee]/F");
  trackTree->Branch("vee.phi2_kf",&outVee.phi2_kf,"vee.phi2_kf[numberOfVee]/F");
  trackTree->Branch("vee.beta2_kf",&outVee.beta2_kf,"vee.beta2_kf[numberOfVee]/F");
  trackTree->Branch("vee.vertex_x",&outVee.vertex_x,"vee.vertex_x[numberOfVee]/F");
  trackTree->Branch("vee.vertex_y",&outVee.vertex_y,"vee.vertex_y[numberOfVee]/F");
  trackTree->Branch("vee.vertex_z",&outVee.vertex_z,"vee.vertex_z[numberOfVee]/F");
  trackTree->Branch("vee.chi",&outVee.chi,"vee.chi[numberOfVee]/F");
  trackTree->Branch("vee.nEl1",&outVee.nel1,"vee.nEl1[numberOfVee]/S");
  trackTree->Branch("vee.ID1",&outVee.id1,"vee.ID1[numberOfVee]/S");
  trackTree->Branch("vee.el",&outVee.el,"vee.el[numberOfVeeHits]/S");
  trackTree->Branch("vee.det",&outVee.det,"vee.det[numberOfVeeHits]/S");
  trackTree->Branch("vee.nEl2",&outVee.nel2,"vee.nEl2[numberOfVee]/S");
  trackTree->Branch("vee.ID2",&outVee.id2,"vee.ID2[numberOfVee]/S");
  trackTree->Branch("kink.theta_p",&outKink.theta_p,"kink.theta_p[numberOfKink]/F");
  trackTree->Branch("kink.phi_p",&outKink.phi_p,"kink.phi_p[numberOfKink]/F");
  trackTree->Branch("kink.beta_p",&outKink.beta_p,"kink.beta_p[numberOfKink]/F");
  trackTree->Branch("kink.theta_d",&outKink.theta_d,"kink.theta_d[numberOfKink]/F");
  trackTree->Branch("kink.phi_d",&outKink.phi_d,"kink.phi_d[numberOfKink]/F");
  trackTree->Branch("kink.beta_d",&outKink.beta_d,"kink.beta_d[numberOfKink]/F");
  trackTree->Branch("kink.theta_p_kf",&outKink.theta_p_kf,"kink.theta_p_kf[numberOfKink]/F");
  trackTree->Branch("kink.phi_p_kf",&outKink.phi_p_kf,"kink.phi_p_kf[numberOfKink]/F");
  trackTree->Branch("kink.beta_p_kf",&outKink.beta_p_kf,"kink.beta_p_kf[numberOfKink]/F");
  trackTree->Branch("kink.theta_d_kf",&outKink.theta_d_kf,"kink.theta_d_kf[numberOfKink]/F");
  trackTree->Branch("kink.phi_d_kf",&outKink.phi_d_kf,"kink.phi_d_kf[numberOfKink]/F");
  trackTree->Branch("kink.beta_d_kf",&outKink.beta_d_kf,"kink.beta_d_kf[numberOfKink]/F");
  trackTree->Branch("kink.vertex_p_x",&outKink.vertex_p_x,"kink.vertex_p_x[numberOfKink]/F");
  trackTree->Branch("kink.vertex_p_y",&outKink.vertex_p_y,"kink.vertex_p_y[numberOfKink]/F");
  trackTree->Branch("kink.vertex_p_z",&outKink.vertex_p_z,"kink.vertex_p_z[numberOfKink]/F");
  trackTree->Branch("kink.vertex_d_x",&outKink.vertex_d_x,"kink.vertex_d_x[numberOfKink]/F");
  trackTree->Branch("kink.vertex_d_y",&outKink.vertex_d_y,"kink.vertex_d_y[numberOfKink]/F");
  trackTree->Branch("kink.vertex_d_z",&outKink.vertex_d_z,"kink.vertex_d_z[numberOfKink]/F");
  trackTree->Branch("kink.chi",&outKink.chi,"kink.chi[numberOfKink]/F");
  trackTree->Branch("kink.nEl_p",&outKink.nel_p,"kink.nEl_p[numberOfKink]/S");
  trackTree->Branch("kink.ID_p",&outKink.id_p,"kink.ID_p[numberOfKink]/S");
  trackTree->Branch("kink.nEl_d",&outKink.nel_d,"kink.nEl_d[numberOfKink]/S");
  trackTree->Branch("kink.ID_d",&outKink.id_d,"kink.ID_d[numberOfKink]/S");
  trackTree->Branch("kink.el",&outKink.el,"kink.el[numberOfKinkHits]/S");
  trackTree->Branch("kink.det",&outKink.det,"kink.det[numberOfKinkHits]/S");
      trackTree->Branch("prompt.tdc",&trackTdcs,"prompt.tdc[numberOfPromptHits]/F");
      trackTree->Branch("prompt.qdc",&trackQdcs,"prompt.qdc[numberOfPromptHits]/F");
      trackTree->Branch("prompt.path",&trackpaths,string("prompt.path[numberOfPrompt]/F").data());
  int nP=2,nv=1;
  pklTree=new TTree("pklTree","pklTree");
  pklTree->Branch("eventNumber",&Event,"eventNumber/I");
  pklTree->Branch("chi",&Chi,"chi/F");
  pklTree->Branch("runNumber",&Run,"runNumber/I");
  pklTree->Branch("trigger",&Trig,"trigger/I");
  pklTree->Branch("numberOfPromptHits",&nPromptHit,"numberOfPromptHits/I");
  pklTree->Branch("numberOfVeeHits",&nVeeHit,"numberOfVeeHits/I");
  pklTree->Branch("numberOfKinkHits",&nKinkHit,"numberOfKinkHits/I");
  if(nP>0)
    {
      pklTree->Branch("prompt.theta",&outPrompt.theta,(string("prompt.theta[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.phi",&outPrompt.phi,(string("prompt.phi[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.beta",&outPrompt.beta,(string("prompt.beta[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.theta_kf",&outPrompt.theta_kf,(string("prompt.theta_kf[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.phi_kf",&outPrompt.phi_kf,(string("prompt.phi_kf[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.beta_kf",&outPrompt.beta_kf,(string("prompt.beta_kf[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.vertex_x",&outPrompt.vertex_x,(string("prompt.vertex_x[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.vertex_y",&outPrompt.vertex_y,(string("prompt.vertex_y[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.vertex_z",&outPrompt.vertex_z,(string("prompt.vertex_z[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.chi",&outPrompt.chi,(string("prompt.chi[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.nEl",&outPrompt.nel,(string("prompt.nEl[")+string_number(nP)+"]/S").data());
      pklTree->Branch("prompt.ID",&outPrompt.id,(string("prompt.ID[")+string_number(nP)+"]/S").data());
      pklTree->Branch("prompt.el",&outPrompt.el,"prompt.el[numberOfPromptHits]/S");
      pklTree->Branch("prompt.det",&outPrompt.det,"prompt.det[numberOfPromptHits]/S");
      pklTree->Branch("prompt.tdc",&trackTdcs,"prompt.tdc[numberOfPromptHits]/F");
      pklTree->Branch("prompt.qdc",&trackQdcs,"prompt.qdc[numberOfPromptHits]/F");
      pklTree->Branch("prompt.path",&trackpaths,(string("prompt.path[")+string_number(nP)+"]/F").data());
      pklTree->Branch("prompt.path1",&trackpaths1,(string("prompt.path1[")+string_number(nP)+"]/F").data());
    }
  if(nv>0)
    {
      pklTree->Branch("vee.theta1",&outVee.theta1,(string("vee.theta1[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.phi1",&outVee.phi1,(string("vee.phi1[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.beta1",&outVee.beta1,(string("vee.beta1[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.theta2",&outVee.theta2,(string("vee.theta2[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.phi2",&outVee.phi2,(string("vee.phi2[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.beta2",&outVee.beta2,(string("vee.beta2[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.theta1_kf",&outVee.theta1_kf,(string("vee.theta1_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.phi1_kf",&outVee.phi1_kf,(string("vee.phi1_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.beta1_kf",&outVee.beta1_kf,(string("vee.beta1_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.theta2_kf",&outVee.theta2_kf,(string("vee.theta2_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.phi2_kf",&outVee.phi2_kf,(string("vee.phi2_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.beta2_kf",&outVee.beta2_kf,(string("vee.beta2_kf[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.vertex_x",&outVee.vertex_x,(string("vee.vertex_x[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.vertex_y",&outVee.vertex_y,(string("vee.vertex_y[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.vertex_z",&outVee.vertex_z,(string("vee.vertex_z[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.chi",&outVee.chi,(string("vee.chi[")+string_number(nv)+"]/F").data());
      pklTree->Branch("vee.nEl1",&outVee.nel1,(string("vee.nEl1[")+string_number(nv)+"]/S").data());
      pklTree->Branch("vee.ID1",&outVee.id1,(string("vee.ID1[")+string_number(nv)+"]/S").data());
      pklTree->Branch("vee.el",&outVee.el,"vee.el[numberOfVeeHits]/S");
      pklTree->Branch("vee.det",&outVee.det,"vee.det[numberOfVeeHits]/S");
      pklTree->Branch("vee.nEl2",&outVee.nel2,(string("vee.nEl2[")+string_number(nv)+"]/S").data());
      pklTree->Branch("vee.ID2",&outVee.id2,(string("vee.ID2[")+string_number(nv)+"]/S").data());
    }
}
void ATrackTreeOutput::finalize()
{
  if(trackTree==NULL)return;
  TFile *f=trackTree->GetCurrentFile();
  f->cd();
  trackTree->Write();
  readWriteLog<<"Tracks : "<<(int)trackTree->GetEntries();
  pklTree->Write();
  readWriteLog<<", PKLambda : "<<(int)pklTree->GetEntries();
  f->Close();
  readWriteLog<<"Entries written to track-tree-file \""<<f->GetName()<<"\""<<endli;
  delete f;
  trackTree=NULL;
  for(int i=0;i<numReactions;i++)if(referReact[i]!=NULL)delete referReact[i];
  delete [] referReact;
  if(localDirectory)
    {
      if(copyFile!="")
	{
	  string command="mv ";
	  command=command+copyFile+" "+outFile;
	  cout<<"bash # "<<command.data()<<flush;
	  gSystem->Exec(command.data());
	  cout<<"   ==> done"<<endl;
	}
    }
}
//extern RbaseReaction* getReactionFromName(string name);
ATrackTreeOutput::ATrackTreeOutput(TEvent& eventIn,TTrack **trackIn, int &nTrack, int &evtNr, int &rnNr, int &trg, int maxD, const algorithm_parameter &param):AAlgorithm("Track tree output"),EventStructure(eventIn),numberOfTracks(nTrack),EventNumber(evtNr),RunNumber(rnNr),Trigger(trg),maxDet(maxD)
{
  tracks=trackIn;
  trackTree=NULL;
  pklTree=NULL;
  outFile=param.getParam<string>(0).getData()+"T.root";
  int numReactions=1;
  referReact=new RbaseReaction*[numReactions];
  string nme="pp-pkl-pkppi";
  referReact[0]=getReactionFromName(nme);
  localDirectory=false;
  if(param.getNumberOfParam<bool>()>3&&param.getNumberOfParam<string>()>1)
    {
      localDirectory=param.getParam<bool>(3).getData();
      directory=param.getParam<string>(1).getData();
    }
  if(param.getNumberOfParam<bool>()>4&&param.getNumberOfParam<vector<int> >()>0)
    {
      writePattern=param.getParam<bool>(4).getData();
      nPattern=param.getParam<vector<int> >(0).getData().size()/3;
      pattern=new int[nPattern*3];
      for(int i=0;i<3*nPattern;i++)
	pattern[i]=param.getParam<vector<int> >(0).getData().at(i);
    }
  copyFile="";
  initialize(outFile);
}
ATrackTreeOutput::~ATrackTreeOutput()
{
  finalize();
  if(writePattern)delete []pattern;
}
void ATrackTreeOutput::setPrompt(int num, TTrack *tr)
{
  outPrompt.theta[num]=tr->getPath().Direction().Theta();
  outPrompt.beta[num]=tr->getSpeed();
  outPrompt.phi[num]=tr->getPath().Direction().Phi();
  outPrompt.theta_kf[num]=0;
  outPrompt.beta_kf[num]=0;
  outPrompt.phi_kf[num]=0;
  outPrompt.vertex_x[num]=tr->getPath().Foot().X();
  outPrompt.vertex_y[num]=tr->getPath().Foot().Y();
  outPrompt.vertex_z[num]=tr->getPath().Foot().Z();
  outPrompt.chi[num]=tr->getChiSquared();
  outPrompt.id[num]=tr->getParticleID();
  outPrompt.nel[num]=tr->getNumberOfCalibHits();
  vector3D front,back;
  front.setValues(0,0,0);
  back.setValues(0,0,0);
  int nb=0,nf=0,nf1=0,nb1=0;
  TCalibHit *b[100],*f[100];
  for(int j=0;j<maxDet;j++)
    {
      for(int k=0;k<tr->getNumberOfCalibHits(j);k++)
	{
	  if(nPromptHit>=MAX_TRACKS*MAX_H_P_T)continue;
	  outPrompt.det[nPromptHit]=tr->getCalibSrcr(j,k)->getDetector();
	  outPrompt.el[nPromptHit]=tr->getCalibSrcr(j,k)->getElement();
// 	  switch(outPrompt.det[nPromptHit])
// 	    {
// 	    case 0:
// 	      if(outPrompt.theta[num]>0.17)cout<<"!!!"<<*tr<<endl;
// 	      break;
// 	    case 3:
// 	      if(outPrompt.theta[num]>0.45&&outPrompt.theta[num]<0.17)cout<<"!!!"<<*tr<<endl;
// 	      break;
// 	    case 6:
// 	      if(outPrompt.theta[num]<0.45)cout<<"!!!"<<*tr<<endl;
// 	      break;
// 	    }
	  trackTdcs[nPromptHit]=tr->getCalibSrcr(j,k)->getTDC();
	  trackQdcs[nPromptHit]=tr->getCalibSrcr(j,k)->getADC();
	  if(tr->getCalibSrcr(j,k)->getHitShape()!=NULL)
	    switch(outPrompt.det[nPromptHit])
	      {
	      case 0:case 1:case 2:
	      case 3:case 4:case 5:
	      case 6:case 7: b[nb++]=tr->getCalibSrcr(j,k);break;
	      case 15:case 16: f[nf++]=tr->getCalibSrcr(j,k);break;
	      }
	  // 	  if(numberOfTracks==5)
 	    {
 	      if(j<8&&tr->getCalibSrcr(j,k)->getHitShape()!=NULL)
 		{
 		  back=back+((tr->getCalibSrcr(j,k)->getHitShape()->entrance(tr->getPath()))-point3D(0,0,0));
 		  nb1++;
 		}
 	      if((j==15||j==16)&&tr->getCalibSrcr(j,k)->getHitShape()!=NULL)
 		{
 		  front=front+((tr->getCalibSrcr(j,k)->getHitShape()->entrance(tr->getPath()))-point3D(0,0,0));
 		  nf1++;
 		}
 	    }
	  nPromptHit++;
	}
    }
  if(nb>0)back=back*(1./nb1);
  if(nf>0)front=front*(1./nf1);
  trackpaths[num]=getPath(nf,f,nb,b,tr->getPath()).R();//(back-front).R();
  trackpaths1[num]=(back-front).R();
}
void ATrackTreeOutput::setVee(int num, TTrack *tr)
{
  if(tr->getSecondary(0)==NULL||tr->getSecondary(1)==NULL)return;
  outVee.theta2_kf[num]=0;
  outVee.beta2_kf[num]=0;
  outVee.phi2_kf[num]=0;
  outVee.theta1_kf[num]=0;
  outVee.beta1_kf[num]=0;
  outVee.phi1_kf[num]=0;
  outVee.vertex_x[num]=tr->getSecondary(0)->getPath().Foot().X();
  outVee.vertex_y[num]=tr->getSecondary(0)->getPath().Foot().Y();
  outVee.vertex_z[num]=tr->getSecondary(0)->getPath().Foot().Z();
  outVee.chi[num]=tr->getSecondary(0)->getChiSquared();
  outVee.theta1[num]=tr->getSecondary(0)->getPath().Direction().Theta();
  outVee.phi1[num]=tr->getSecondary(0)->getPath().Direction().Phi();
  outVee.beta1[num]=tr->getSecondary(0)->getSpeed();
  outVee.id1[num]=tr->getSecondary(0)->getParticleID();
  outVee.nel1[num]=tr->getSecondary(0)->getNumberOfCalibHits();
  outVee.theta2[num]=tr->getSecondary(1)->getPath().Direction().Theta();
  outVee.phi2[num]=tr->getSecondary(1)->getPath().Direction().Phi();
  outVee.beta2[num]=tr->getSecondary(1)->getSpeed();
  outVee.id2[num]=tr->getSecondary(1)->getParticleID();
  outVee.nel2[num]=tr->getSecondary(1)->getNumberOfCalibHits();
  for(int j=0;j<maxDet;j++)
    {
      for(int k=0;k<tr->getSecondary(0)->getNumberOfCalibHits(j);k++)
	{
	  if(nVeeHit>=MAX_TRACKS*MAX_H_P_T)continue;
	  outVee.det[nVeeHit]=tr->getSecondary(0)->getCalibSrcr(j,k)->getDetector();
	  outVee.el[nVeeHit]=tr->getSecondary(0)->getCalibSrcr(j,k)->getElement();
	  nVeeHit++;
	}
    }
  for(int j=0;j<maxDet;j++)
    {
      for(int k=0;k<tr->getSecondary(1)->getNumberOfCalibHits(j);k++)
	{
	  if(nVeeHit>=MAX_TRACKS*MAX_H_P_T)continue;
	  outVee.det[nVeeHit]=tr->getSecondary(1)->getCalibSrcr(j,k)->getDetector();
	  outVee.el[nVeeHit]=tr->getSecondary(1)->getCalibSrcr(j,k)->getElement();
	  nVeeHit++;
	}
    }
}
void ATrackTreeOutput::setKink(int num, TTrack *tr)
{
  if(tr->getSecondary(0)==NULL)return;
  outKink.theta_p_kf[num]=0;
  outKink.beta_p_kf[num]=0;
  outKink.phi_p_kf[num]=0;
  outKink.theta_d_kf[num]=0;
  outKink.beta_d_kf[num]=0;
  outKink.phi_d_kf[num]=0;
  outKink.theta_p[num]=tr->getPath().Direction().Theta();
  outKink.beta_p[num]=tr->getSpeed();
  outKink.phi_p[num]=tr->getPath().Direction().Phi();
  outKink.vertex_p_x[num]=tr->getPath().Foot().x();
  outKink.vertex_p_y[num]=tr->getPath().Foot().y();
  outKink.vertex_p_z[num]=tr->getPath().Foot().Z();
  outKink.chi[num]=tr->getChiSquared();
  outKink.id_p[num]=tr->getParticleID();
  outKink.nel_p[num]=tr->getNumberOfCalibHits();
  for(int j=0;j<maxDet;j++)
    {
      for(int k=0;k<tr->getNumberOfCalibHits(j);k++)
	{
	  if(nKinkHit>=MAX_TRACKS*MAX_H_P_T)continue;
	  outKink.det[nKinkHit]=tr->getCalibSrcr(j,k)->getDetector();
	  outKink.el[nKinkHit]=tr->getCalibSrcr(j,k)->getElement();
	  nKinkHit++;
	}
    }
  outKink.theta_d[num]=tr->getSecondary(0)->getPath().Direction().Theta();
  outKink.beta_d[num]=tr->getSecondary(0)->getSpeed();
  outKink.phi_d[num]=tr->getSecondary(0)->getPath().Direction().Phi();
  outKink.vertex_d_x[num]=tr->getSecondary(0)->getPath().Foot()[0];
  outKink.vertex_d_y[num]=tr->getSecondary(0)->getPath().Foot()[1];
  outKink.vertex_d_z[num]=tr->getSecondary(0)->getPath().Foot()[2];
  outKink.id_d[num]=tr->getSecondary(0)->getParticleID();
  outKink.nel_d[num]=tr->getSecondary(0)->getNumberOfCalibHits();
  for(int j=0;j<maxDet;j++)
    {
      for(int k=0;k<tr->getSecondary(0)->getNumberOfCalibHits(j);k++)
	{
	  if(nKinkHit>=MAX_TRACKS*MAX_H_P_T)continue;
	  outKink.det[nKinkHit]=tr->getSecondary(0)->getCalibSrcr(j,k)->getDetector();
		    outKink.el[nKinkHit]=tr->getSecondary(0)->getCalibSrcr(j,k)->getElement();
		    nKinkHit++;
	}
    }
}
extern vector3D solve(const vector3D &, const vector3D &, const vector3D &, const vector3D &);
void* ATrackTreeOutput::process(void* ptr)
{
  if(trackTree==NULL)return ptr;
  if(numberOfTracks==0)return ptr;
  //  int maxDet=30;
  Event=EventNumber;
  Run=RunNumber;
  Trig=Trigger;
  nPrompt=0;
  nVee=0;
  nKink=0;
  nPromptHit=0;
  nVeeHit=0;
  nKinkHit=0;
  for(int i=0;i<numberOfTracks;i++)
    {
      if(tracks[i]==NULL)continue;
      if(tracks[i]->isSecondary())continue;
      switch(tracks[i]->getNumberOfSecondaryTracks())
	{
	case 0://prompt
	  {
	    if(nPrompt>=MAX_TRACKS)continue;
 	    setPrompt(nPrompt,tracks[i]);
	    nPrompt++;
	    break;
	  }
	case 1://kink
	  {
	    if(nKink>=MAX_TRACKS)continue;
 	    if(tracks[i]->getSecondary(0)==NULL)continue;
	    setKink(nKink,tracks[i]);
	    nKink++;
	    break;
	  }
	case 2://vee
	  {
	    if(nVee>=MAX_TRACKS)continue;
	    if(tracks[i]->getSecondary(0)==NULL||tracks[i]->getSecondary(1)==NULL)continue;
	    setVee(nVee,tracks[i]);
	    nVee++;
	    break;
	  }
	}
    }
  bool dowrite=true;
  if(writePattern)
    {
      dowrite=false;
      for(int i=0;i<nPattern;i++)
	{
	  if((pattern[0+i*3]==nPrompt||pattern[0+i*3]<0)&&
	     (pattern[1+i*3]==nKink  ||pattern[1+i*3]<0)&&
	     (pattern[2+i*3]==nVee   ||pattern[2+i*3]<0))
	    dowrite=true;
	}
    }
  if(dowrite)
    trackTree->Fill();
  if(nPrompt==2&&nVee==1)
    {
      Chi=-1;
      //      bool found=false;
      vector3D geomom, beam(0,0,1);
      geomom=solve(tracks[0]->getPath().Direction(),tracks[1]->getPath().Direction(),tracks[2]->getPath().Direction(),beam);
	  pklTree->Fill();
      if(!(geomom[0]<0||geomom[1]<0||geomom[2]<0))
	{
	  /*
	  for(int i=0;i<EventStructure.getNumberOfReactions();i++)
	    {
	      RbaseReaction *r=EventStructure.getReaction(i);
	      if(r==NULL)continue;
	      if(r->name()!=referReact[0]->name())continue;
	      found=true;
	      Chi=r->getChiSquared();
	      nVeeHit=0;
	      nPromptHit=0;
	      setPrompt(0,r->getTrack(0));
	      setPrompt(1,r->getTrack(1));
	      setVee(0,r->getTrack(2));
	      momentum4D p;
	      for(int i=0;i<2;i++) {
		p=r->getParticle(i);
		outPrompt.theta_kf[i]=p.Theta();
		outPrompt.beta_kf[i]=p.Beta();
		outPrompt.phi_kf[i]=p.Phi();}
	      p=r->getParticle(3);
	      outVee.theta1_kf[0]=p.Theta();
	      outVee.beta1_kf[0]=p.Beta();
	      outVee.phi1_kf[0]=p.Phi();
	      p=r->getParticle(4);
	      outVee.theta2_kf[0]=p.Theta();
	      outVee.beta2_kf[0]=p.Beta();
	      outVee.phi2_kf[0]=p.Phi();
	    }
	  
	  if(1==0)//!found)
	    {
	      float mp=Eparticles::getMassG(14),mk=Eparticles::getMassG(11),ml=Eparticles::getMassG(18);
	      momentum4D mom[4],p1,p2,t1,t2;
	      p1.setPM(vector3D(0,0,0),mp);p2.setPM(vector3D(0,0,3.059),mp);
	      for(int i=0;i<2;i++)
		{
		  mom[i].setVM(tracks[i]->getPath().Direction()*tracks[i]->getSpeed(),mp);
		  mom[i+2].setVM(tracks[i]->getPath().Direction()*tracks[i]->getSpeed(),mk);
		}
	      t1=mom[0]+mom[3]-(p1+p2);t2=mom[1]+mom[2]-(p1+p2);
	      nPromptHit=0;
	      if(abs(t1*t1-ml*ml)<=abs(t2*t2-ml*ml))
		{
		  setPrompt(0,tracks[0]);
		  setPrompt(1,tracks[1]);
		}
	      else
		{
		  setPrompt(1,tracks[0]);
		  setPrompt(0,tracks[1]);
		}
	    }
	  */
	}
    }
  return ptr;
}
algorithm_parameter ATrackTreeOutput::getDescription()
{
  algorithm_parameter ret("Track Tree output",0,0);
  vector<string> des;
  des.push_back("");
  des.push_back("This Algorithm writes track information to a root tree.");
  des.push_back("This track information isdynamical in size and complete");
  des.push_back("in information.");
  ret.setDescription(des);
  ret.addParam<string>(single_parameter<string>("file name",""));
  ret.addParam<bool>(single_parameter<bool>(string("use local directory"),true));
  ret.addParam<string>(single_parameter<string>(string("local directory"),string("")));
  return ret;
}
