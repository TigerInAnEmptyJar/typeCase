#include "ATDCcalibration.h"
#include <iostream>
#include <fstream>
#include <TF1.h>
#include "Eparticles.h"
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <stdlib.h>
#include "ring.h"
#include "spiral.h"
#include "wedge.h"
#include "fiber.h"
#include "logger.h"
float BARREL_MIN=0.45;
float BARREL_MAX=0.5;
bool printLine=false;
TH1* getMaxHisto(TH2* hist);
//void fitTheHisto(TH2* h2d, TH1* h1d,TF1 *fitfunc,CommonCalibrationParser *output,int element,bool fixAtMax,int ptt,string opt);
void nextPad(int nPads,TPostScript *ps,TCanvas *canv, int &nc,TObject **pointers);
ATDCcalibration::ATDCcalibration(int &evtnr, int &rnr,TTrack **trIn, TPixel ***pixIn, TCalibHit*** hitIn, int &numTrack, int **numPix, int **numHits, TSetup &setin, const algorithm_parameter &descr):AAlgorithm("Calibration algorithm for beam, geometry and TDC"), eventNumber(evtnr), runNumber(rnr), numberOfTracks(numTrack), setup(setin)
{
  gStyle->SetPalette(1);
  if(!Eparticles::IsInit())Eparticles::init();
  tracks=trIn;
  pixels=pixIn;
  hits=hitIn;
  numberOfPixels=numPix;
  numberOfHits=numHits;
  doGeometry=descr.getParam<bool>(0).getData();
  doBeam=descr.getParam<bool>(1).getData();
  doWalk=descr.getParam<bool>(2).getData();
  doOffset=descr.getParam<bool>(3).getData();
  doTwoSided=descr.getParam<bool>(4).getData();
  eventBased=descr.getParam<bool>(5).getData();
  killFileOnEnd=descr.getParam<bool>(6).getData();
  readFile=descr.getParam<bool>(7).getData();
  filename=descr.getParam<string>(2).getData();
  sumEvents=descr.getParam<int>(0).getData();
  printPattern=descr.getParam<int>(2).getData();
  offsets=NULL;
  slopes=NULL;
//   cout<<"print pattern: "<<printPattern<<" : "
//       <<((printPattern&1))<<" "
//       <<((printPattern&2))<<" "
//       <<((printPattern&4))<<" "
//       <<((printPattern&8))<<" "
//       <<((printPattern&16))<<" "
//       <<((printPattern&32))<<" "
//       <<((printPattern&64))<<" "
//       <<((printPattern&128))<<" "<<endl;

  runCounter=0;
  eventCounter=0;
  if(doGeometry&&(doBeam||doWalk||doOffset))
    {
      cout<<"Error in ATDCcalibration: walk, offset or beam calibration selected with geometry calibration!"<<endl;
      cout<<"  first do geometry then the other stuff!"<<endl;
      cout<<"  walk, offset and beam calibration need tracking! switch them off"<<endl;
      doBeam=false;
      doWalk=false;
      doOffset=false;
    }
  numberOfIterations=descr.getParam<int>(1).getData();
  nCalibDets=descr.getParam<vector<int> >(0).getData().size();
  numElements=new int[nCalibDets];
  detectors=new int[nCalibDets];
  pixelBased=new int[nCalibDets];
  twoSided=new int[nCalibDets];
  isStop=new bool[nCalibDets];
  doLRT=new bool[nCalibDets];
  measuresAgainst=new int[nCalibDets];
  meanOff=new float[nCalibDets*2];
  min_maxLRP=new float[nCalibDets*2];
  mimaqdc=new float[nCalibDets*2];
  pixelDets=new int[nCalibDets];
  single=new int[nCalibDets];
  referencesTo=new int[nCalibDets];
  hasTrack=false;
  hasPix=false;
  postscript=NULL;
  for(int i=0;i<nCalibDets;i++)
    {
      detectors[i]=descr.getParam<vector<int> >(0).getData().at(i);
      numElements[i]=setup.getDetectorr(detectors[i]).getNumberOfElements();
      pixelBased[i]=descr.getParam<vector<int> >(1).getData().at(i);
      measuresAgainst[i]=descr.getParam<vector<int> >(2).getData().at(i);
      isStop[i]=(descr.getParam<vector<int> >(3).getData().at(i)==1);
      twoSided[i]=descr.getParam<vector<int> >(4).getData().at(i);
      pixelDets[i]=descr.getParam<vector<int> >(5).getData().at(i);
      referencesTo[i]=descr.getParam<vector<int> >(6).getData().at(i);
      single[i]=descr.getParam<vector<int> >(7).getData().at(i);
      doLRT[i]=(descr.getParam<vector<int> >(8).getData().at(i)==1);
      if(pixelBased[i]>=0)
	hasPix=true;
      else
	hasTrack=true;
      meanOff[2*i]=descr.getParam<vector<float> >(0).getData().at(2*i);
      meanOff[2*i+1]=descr.getParam<vector<float> >(0).getData().at(2*i+1);
      mimaqdc[2*i]=descr.getParam<vector<float> >(1).getData().at(2*i);
      mimaqdc[2*i+1]=descr.getParam<vector<float> >(1).getData().at(2*i+1);
      min_maxLRP[2*i]=descr.getParam<vector<float> >(2).getData().at(2*i);
      min_maxLRP[2*i+1]=descr.getParam<vector<float> >(2).getData().at(2*i+1);
    }
  ntrackElstructs=0;
  for(int i=0;i<nCalibDets;i++)
    {
      if(pixelBased[i]>=0)continue;
      if(measuresAgainst[i]<0||measuresAgainst[i]>=nCalibDets)continue;
      if(measuresAgainst[measuresAgainst[i]]==i&&i<measuresAgainst[i])ntrackElstructs++;
    }
  trackEls[0]=new int[ntrackElstructs];
  trackEls[1]=new int[ntrackElstructs];
  ntrackElstructs=0;
  for(int i=0;i<nCalibDets;i++)
    {
      if(pixelBased[i]>=0)continue;
      if(measuresAgainst[i]<0||measuresAgainst[i]>=nCalibDets)continue;
      if(measuresAgainst[measuresAgainst[i]]==i&&i<measuresAgainst[i])
	{
	  trackEls[0][ntrackElstructs]=i;
	  trackEls[1][ntrackElstructs]=measuresAgainst[i];
	  ntrackElstructs++;
	}
    }

  nRefPix=descr.getParam<vector<int> >(9).getData().size();
  refPixels=new int[nRefPix];
  for(int i=0;i<nRefPix;i++)
    refPixels[i]=descr.getParam<vector<int> >(9).getData().at(i);
  calibrationOutputPath=descr.getParam<string>(0).getData();
  author=descr.getParam<string>(1).getData();
  params=new CommonCalibrationParser***[nCalibDets];
  numRanges=new int*[nCalibDets];
  numCalibrations=new int[nCalibDets];
  for(int i=0;i<4;i++)
    calibPos[i]=new int[nCalibDets];
  actualRanges=new int*[nCalibDets];
  for(int i=0;i<nCalibDets;i++)
    {
      numCalibrations[i]=0;
      params[i]=new CommonCalibrationParser**[4];
      numRanges[i]=new int[4];
      actualRanges[i]=new int[4];
      for(int j=0;j<4;j++)params[i][j]=NULL;
    }
  for(int i=0;i<nCalibDets;i++)
    {
      calibPos[0][i]=(doWalk?numCalibrations[i]:-1);
      if(doWalk)numCalibrations[i]++;
      calibPos[1][i]=(doOffset?numCalibrations[i]:-1);
      if(doOffset)numCalibrations[i]++;
      calibPos[2][i]=(/*doLRT[i]*/isStop[i]?numCalibrations[i]:-1);
      if(/*doLRT[i]*/isStop[i])numCalibrations[i]++;
      calibPos[3][i]=(doTwoSided&&twoSided[i]>=0?numCalibrations[i]:-1);
      if(doTwoSided&&twoSided[i]>=0)numCalibrations[i]++;
    }
  geoHistos=NULL;
  beamHistos=NULL;
  pixTree=NULL;
  trackTree=NULL;
  elTree=NULL;
  outFile=NULL;
  offsetHistos=NULL;
  resolutionHisto=NULL;
  resolutionHisto1=NULL;
  outParser=NULL;
  offsetHistos2=NULL;
  offsetParser=NULL;
  walkHistos=NULL;
  walkFitFunctions=NULL;
  walkFunctions=NULL;
  if(doGeometry)
    {
      geoHistos=new TH2F*[nCalibDets];
      for(int i=0;i<nCalibDets;i++)
	{
	  geoHistos[i]=new TH2F((string("distance of element to track: det")+string_number(detectors[i])).data(),(string("distance of element to track: det")+string_number(detectors[i])).data(),100,-5,5,numElements[i],0,numElements[i]-1);
	  geoHistos[i]->SetDirectory(NULL);
	}
    }
  if(doBeam)
    {
      beamHistos=new TH1F**[40];
      for(int i=0;i<40;i++)
	{
	  beamHistos[i]=new TH1F*[40];
	  for(int j=0;j<40;j++)
	    {
	      vector3D tmp((i-19.5)*0.001,(j-19.5)*0.001,1);tmp.normalize();
	      beamHistos[i][j]=new TH1F((string("beam ")+string_number(i)+" "+string_number(j)).data(),
					(string("beam at \\theta=")+string_number(tmp.Theta()*180./M_PI)+"°, \\phi="+string_number(tmp.Phi()*180./M_PI)+"°").data(),100,-1,1);
	      beamHistos[i][j]->SetDirectory(NULL);
	    }
	}
    }
  if(doWalk||doOffset||doTwoSided)
    {
      outParser=new CommonCalibrationParser*[nCalibDets];
      for(int i=0;i<nCalibDets;i++)
	outParser[i]=new CommonCalibrationParser[numCalibrations[i]];
      int paramId[4]={2,5,8,6};
      int nparams[4]={5,2,5,4};
      for(int i=0;i<nCalibDets;i++)
	{
	  for(int j=0;j<4;j++)
	    {
	      if(calibPos[j][i]<0)continue;
	      outParser[i][calibPos[j][i]].setDetID(detectors[i]);
	      outParser[i][calibPos[j][i]].setNumParamsElements(numElements[i],nparams[j]);
	      outParser[i][calibPos[j][i]].setParamID(paramId[j]);
	      outParser[i][calibPos[j][i]].setRanges(-1,-1);
	      outParser[i][calibPos[j][i]].setValid(true);
	      for(int k=0;k<numElements[i];k++)
		for(int l=0;l<nparams[j];l++)
		  outParser[i][calibPos[j][i]].setParameter(k,l,(j==3&&l==0?1:0));
	    }
	}
      runNumber=1;
      defineOffsetStuff(false,true,true,true,1);
    }
  if(doTwoSided)
    {
      nTwoSided=0;
      for(int i=0;i<nCalibDets;i++)
	if(twoSided[i]>=0&&twoSided[i]<i)
	  nTwoSided++;
      twoDets=new int[nTwoSided];
      nTwoSided=0;
      for(int i=0;i<nCalibDets;i++)
	if(twoSided[i]>=0&&twoSided[i]>i)
	  {
	    twoDets[nTwoSided]=i;
	    nTwoSided++;
	  }
      twoSidedHisto=new TH2F**[nTwoSided];
      for(int i=0;i<nTwoSided;i++)
	{
	  twoSidedHisto[i]=new TH2F*[numElements[twoDets[i]]];
	  float thmin=0,thmax=thMax(twoDets[i]);
	  for(int j=0;j<numElements[twoDets[i]];j++)
	    {
	      twoSidedHisto[i][j]=new TH2F((string("pos vs \\DeltaTDC:")+string_number(detectors[twoDets[i]])+","+string_number(j)).data(),(string("pos vs \\DeltaTDC:det ")+string_number(detectors[twoDets[i]])+", el"+string_number(j)).data(),100,-10+6,10+6,100,thmin/10.+70,thmax/10.+10);
	      twoSidedHisto[i][j]->SetDirectory(0);
	    }
	}
    }
  if(doWalk||doOffset)
    {
      offsetHistos=new TH1**[nCalibDets];
      resolutionHisto=new TH2F*[nCalibDets*2];
      resolutionHisto1=new TH1F*[nCalibDets];
      for(int i=0;i<nCalibDets;i++)
	{
	  float thmin=0,thmax=thMax(i);
	  float mm=0;
	  resolutionHisto[i*2]=new TH2F((string("resolution vs el, det ")+string_number(detectors[i])).data(),
 					(string("tdc resolution: det ")+string_number(detectors[i])+" versus close det").data(),numElements[i],0,numElements[i],100,mm+(detectors[i]>10?-2:-3),mm+(detectors[i]>10?2:3));
	  resolutionHisto[i*2]->SetDirectory(NULL);

  	    resolutionHisto[i*2+1]=new TH2F((string("resolution vs light run path, det ")+string_number(detectors[i])).data(),
  					(string("tdc resolution: det ")+string_number(detectors[i])+" light run path").data(),(i<2?100:i<5?25:30),thmin*0.9,thmax*1.02,100,mm+(detectors[i]>10?-2:-3),mm+(detectors[i]>10?2:3));
	  resolutionHisto[i*2+1]->SetDirectory(NULL);
	  resolutionHisto1[i]=new TH1F((string("resolution det ")+string_number(detectors[i])).data(),
				       (string("tdc resolution: det ")+string_number(detectors[i])+" versus close det").data(),
				       100,mm+(detectors[i]>10?-2:-3),mm+(detectors[i]>10?2:3));//,100,thmin*0.9,thmax*1.1);
	    
	  resolutionHisto1[i]->SetDirectory(NULL);
	  offsetHistos[i]=new TH1*[numElements[i]];
	  for(int j=0;j<numElements[i];j++)
	    {
	      offsetHistos[i][j]=new TH1F((string("det ")+string_number(detectors[i])+",el "+string_number(j)).data(),
					  (string("det ")+string_number(detectors[i])+",el "+string_number(j)).data(),
					  100,-meanOff[i*2+1],+meanOff[i*2+1]);
	      offsetHistos[i][j]->SetDirectory(0);
	    }
	}
      if(doWalk)
	  defineWalkStuff(false,true,true,15,true,1);
      if(!readFile)
	{
	  outFile=new TFile((calibrationOutputPath+string_number(getpid())+".root").data(),"recreate");
	  cout<<"write calibration trees to \""<<outFile->GetName()<<"\""<<endl;
	  outFile->cd();}
      else
	outFile=new TFile(filename.data(),"read");

      if(doWalk)
	{
	  if(hasPix)
	    {
	      if(!readFile)
		{
		  outFile->cd();
		  pixTree=new TTree("pixelTree","pixelTree");
		  pixTree->Branch("eventNumber",&evtNr,"eventNumber/I");
		  pixTree->Branch("pix",&outpix,"tdc[3]/F:qdc[3]/F:pointx/F:pointy/F:pointz/F:el[3]/I:ID/I");
		}
	      else
		{
		  pixTree=(TTree*)outFile->Get("pixelTree");
		  if(pixTree==NULL)
		    {
		      cout<<"file doesn\'t contain pix tree \""<<outFile->GetName()<<"\""<<endl;
		      outFile->ls();
		      exit(0);
		    }
		  pixTree->SetBranchStatus("*",1);
		  pixTree->SetBranchAddress("eventNumber",&evtNr);
		  pixTree->SetBranchAddress("pix",&outpix);
		}
	    }
	  if(hasTrack)
	    {
	      if(!readFile)
		{
		  outFile->cd();
		  trackTree=new TTree("trackTree","trackTree");
		  trackTree->Branch("eventNumber",&evtNr,"eventNumber/I");
		  trackTree->Branch("tr",&outtrack_el,"pointz[2]/F:tdc[2]/F:qdc[2]/F:theta/F:phi/F:el[2]/I:det/I");
		}
	      else
		{
		  trackTree=(TTree*)outFile->Get("trackTree");
		  if(trackTree==NULL)
		    {
		      cout<<"file doesn\'t contain track tree \""<<outFile->GetName()<<"\""<<endl;
		      outFile->ls();
		      exit(0);
		    }
		  trackTree->SetBranchStatus("*",1);
		  trackTree->SetBranchAddress("eventNumber",&evtNr);
		  trackTree->SetBranchAddress("tr",&outtrack_el);
		}
	    }
	}
      if(doOffset)
	{
	  if(!readFile)
	    {
	      elTree=new TTree("elastic","elastic");
	      elTree->Branch("eventNumber",&evtNr,"eventNumber/I");
	      elTree->Branch("t0",&outtrack[0],"pointz[10]/F:tdc[10]/F:qdc[10]/F:beta/F:theta/F:phi/F:el[10]/I:vt[3]/F");
	      elTree->Branch("t1",&outtrack[1],"pointz[10]/F:tdc[10]/F:qdc[10]/F:beta/F:theta/F:phi/F:el[10]/I:vt[3]/F");
	    }
	  else
	    {
	      elTree=(TTree*)outFile->Get("elastic");
	      elTree->SetBranchStatus("*",1);
	      elTree->SetBranchAddress("eventNumber",&evtNr);
	      elTree->SetBranchAddress("t0",&outtrack[0]);
	      elTree->SetBranchAddress("t1",&outtrack[1]);
	    }
	}
    }
}
int findOffsetRanges(string runname, vector<int>& rages)
{
  vector<string> data;
  string s;
  char c;
  ifstream input;
  input.open((string(getenv("KTOFPACK"))+"/data/run-splittings.data").data());
  while(!input.eof())
    {
      input.get(c);
      s="";while(c!='\n'&&!input.eof()){s=s+c;input.get(c);}
      data.push_back(s);
    }
  input.close();
  bool start=false;
  for(unsigned int i=0;i<data.size();i++)
    {
      if(data[i]==runname)start=true;
      else if(start)
	{
	  if(toInt(data[i])>0)
	    rages.push_back(toInt(data[i]));
	  else
	    start=false;
	}
    }
  return rages.size();
}
float ATDCcalibration::thMax(int id)
{
  if(setup.getDetectorr(detectors[id]).getShape(0)->getName()=="wedge")
    {
      wedge* sh=((wedge*)setup.getDetectorr(detectors[id]).getShape(0));
      return sh->getOuterRadius()-sh->getInnerRadius();
    }
  else if(setup.getDetectorr(detectors[id]).getShape(0)->getName()=="spiral")
    {
      spiral* sh=((spiral*)setup.getDetectorr(detectors[id]).getShape(0));
      return sh->getOuterRadius()-sh->getInnerRadius();
    }
  return 1.1;
}
float ATDCcalibration::thMin(int id)
{
  return 0;
}
void ATDCcalibration::defineWalkStuff(bool justClean,bool redefine, bool mkehistos,int histoptt, bool mkefunctions, float rngmod)
{
  if(redefine||mkehistos||justClean)
    {
      if(walkHistos!=NULL)
	for(int i=0;i<nCalibDets;i++)
	  for(int j=0;j<numElements[i];j++)
	    {
	      if(redefine||(histoptt&1)>0)delete walkHistos[i][j];
	      if(redefine||(histoptt&2)>0)delete walkHistos[i][j+numElements[i]];
	      if(redefine||(histoptt&4)>0)delete walkHistos[i][j+2*numElements[i]];
	      if(doLRT[i])
		if(redefine||(histoptt&8)>0)delete walkHistos[i][j+3*numElements[i]];
	    }
    }
  if(redefine||mkefunctions||justClean)
    {
      if(walkFunctions!=NULL&&walkFitFunctions!=NULL)
	for(int i=0;i<nCalibDets;i++)
	  {
	    delete walkFitFunctions[i];
	    for(int j=0;j<numElements[i];j++)
	      delete walkFunctions[i][j];
	  }
    }
  if(redefine||justClean)
    {
      for(int i=0;i<nCalibDets;i++)
	{
	  if(walkHistos!=NULL) delete []walkHistos[i];
	  if(walkFunctions!=NULL)delete []walkFunctions[i];
	}
      if(walkHistos!=NULL)delete []walkHistos;
      if(walkFitFunctions!=NULL)delete []walkFitFunctions;
      if(walkFunctions!=NULL)delete []walkFunctions;
    }
  if(justClean)
    {
      walkHistos=NULL;
      walkFitFunctions=NULL;
      walkFunctions=NULL;
      return;
    }
  if(redefine)
    {
      walkHistos=new TH1**[nCalibDets];
      walkFitFunctions=new TF1*[nCalibDets];
      walkFunctions=new TF1**[nCalibDets];
      for(int i=0;i<nCalibDets;i++)
	{
	  walkHistos[i]=new TH1*[numElements[i]*(3+(doLRT[i]?1:0))];
	  walkFitFunctions[i]=new TF1("func","[0]/(x-[1])+[2]*log(abs(x-[3]))+[4]",0,10000);
	  walkFunctions[i]=new TF1*[numElements[i]];
	  for(int j=0;j<numElements[i];j++)
	    walkFunctions[i][j]=new TF1("walkfunction","[0]/(x-[1])+[2]*log(abs(x-[3]))+[4]",0,4000);
	}
    }
  if(redefine||mkehistos)
    {
      float mm=0,thmin=0,thmax;
      string s[4]={string("tdc vs qdc:"),string("qdc:"),string("tdc:"),string("")};
      for(int i=0;i<nCalibDets;i++)
	{
	  thmax=thMax(i);
	  for(int j=0;j<numElements[i];j++)
	    {
	      s[3]=string(" det:")+string_number(detectors[i])+" el"+string_number(j);
	      if(redefine||(histoptt&1)>0)
		walkHistos[i][j]=
		  new TH2F((s[0]+s[3]).data(), (s[0]+s[3]).data(), 
			   50,mimaqdc[2*i],mimaqdc[2*i+1],
			   50,mm-meanOff[i*2+1]*(0.3+(twoSided[i]==measuresAgainst[i]?1:0)*rngmod),
			   mm+meanOff[i*2+1]*(0.3+(twoSided[i]==measuresAgainst[i]?1:0)*rngmod));
	      if(redefine||(histoptt&2)>0)
		walkHistos[i][j+numElements[i]]=
		  new TH1F((s[1]+s[3]).data(), (s[1]+s[3]).data(), 
			   100,mimaqdc[2*i],mimaqdc[2*i+1]);
	      if(redefine||(histoptt&4)>0)
		walkHistos[i][j+2*numElements[i]]=
		  new TH1F((s[2]+s[3]).data(), (s[2]+s[3]).data(), 
			   100,//meanOff[2*i]*((histoptt&64)>0?1:0)
			   -meanOff[i*2+1]*(1+(twoSided[i]==measuresAgainst[i]?9:0)*rngmod),
			   //meanOff[2*i]*((histoptt&64)>0?1:0)
			   +meanOff[i*2+1]*(1+(twoSided[i]==measuresAgainst[i]?9:0)*rngmod));
	      walkHistos[i][j]->SetDirectory(0);
	      walkHistos[i][j+numElements[i]]->SetDirectory(0);
	      walkHistos[i][j+2*numElements[i]]->SetDirectory(0);
	      if(redefine||(histoptt&8)>0)
		if(doLRT[i])
		  {
		    walkHistos[i][j+3*numElements[i]]=
		      new TH2F((string("tdc vs \\theta:")+s[3]).data(), 
			       (string("tdc vs \\theta:")+s[3]).data(), 
			       30,thmin*0.9,thmax*1.02, 
			       50,mm-meanOff[i*2+1]/5.,mm+meanOff[i*2+1]/5.);
		    walkHistos[i][j+3*numElements[i]]->SetDirectory(0);
		  }
	    }
	}	
    }
  if(redefine||mkefunctions)
    {
      for(int i=0;i<nCalibDets;i++)
	{
	  for(int j=0;j<5;j++)walkFitFunctions[i]->SetParameter(j,0);
	  walkFitFunctions[i]->SetParLimits(1,0,1e4);
	  walkFitFunctions[i]->SetParLimits(3,-1e6,1e6);
	  if(twoSided[i]>=0)
	    {
	      walkFitFunctions[i]->SetParameters(0,0,0,0,0);
	      walkFitFunctions[i]->SetParLimits(0,-1e7,1e-7);//1e3);
	      walkFitFunctions[i]->SetParLimits(2,-1e-6,0);
	      walkFitFunctions[i]->SetParLimits(3,-1e-4,200);//1e-4);
	      walkFitFunctions[i]->SetParLimits(1,-1e6,200);
	    }
	  else if(isStop[i]&&((detectors[i]%3)>0))//pixelBased[i]<0)
	    {
	      walkFitFunctions[i]->SetParLimits(0,-1e6,0);//1e3);
	      walkFitFunctions[i]->SetParLimits(1,-1e6,0);
	      walkFitFunctions[i]->SetParLimits(2,-1e-4,0);
	      walkFitFunctions[i]->SetParLimits(3,-1e-4,200);//1e-4);
	    }
	  else if(isStop[i])
	    {
	      walkFitFunctions[i]->SetParLimits(0,-1e6,0);//1e3);
	      walkFitFunctions[i]->SetParLimits(1,-1e6,0);
	      walkFitFunctions[i]->SetParLimits(2,-1e-4,0);
	      walkFitFunctions[i]->SetParLimits(3,-1e-4,1e-4);
	    }
	}
      for(int i=0;i<nCalibDets;i++)
	{
	  for(int j=0;j<numElements[i];j++)
	    {
	      walkFunctions[i][j]->SetLineColor(1);
	      walkFunctions[i][j]->SetLineWidth(2);
	    }
	}
    }
}
TH1F* getSlice(TH2F *histo,int bin, int axis)
{
  if(histo==NULL)return NULL;
  if(bin<0||(axis==0&&bin>histo->GetNbinsX()+1)||(axis==1&&bin>histo->GetNbinsY()+1))return NULL;
  TH1F *ret=new TH1F((string(histo->GetName())+"_sl").data(),histo->GetName(),
		     (axis==0?histo->GetNbinsY():histo->GetNbinsX()),
		     (axis==0?histo->GetYaxis()->GetXmin():histo->GetXaxis()->GetXmin()),
		     (axis==0?histo->GetYaxis()->GetXmax():histo->GetXaxis()->GetXmax()));
  for(int i=1;i<=ret->GetNbinsX();i++)
    ret->Fill(ret->GetBinCenter(i),histo->GetBinContent((axis==0?bin:i),(axis==0?i:bin)));
  return ret;
}
void ATDCcalibration::defineOffsetStuff(bool justClean,bool redefine, bool mkehistos, bool mkeparser, float rngmod)
{
  if(mkehistos||redefine||justClean)
    {
      if(offsetHistos2!=NULL)
	for(int i=0;i<noffsetRanges;i++)
	  for(int j=0;j<nCalibDets;j++)
	    for(int k=0;k<numElements[j];k++)
	      delete offsetHistos2[i][j][k];
    }
  if(redefine||mkeparser||justClean)
    {
      if(offsetParser!=NULL)
	for(int i=0;i<nCalibDets;i++)
	  if(offsetParser[i]!=NULL)
	    delete []offsetParser[i];
    }
  if(redefine||justClean)
    {
      if(offsetHistos2!=NULL)
	for(int i=0;i<noffsetRanges;i++)
	  {
	    for(int j=0;j<nCalibDets;j++)
	      delete []offsetHistos2[i][j];
	    delete []offsetHistos2[i];
	  }
      if(offsets!=NULL)
	{
	  for(int i=0;i<noffsetRanges;i++)
	    {
	      for(int j=0;j<nCalibDets;j++)
		{
		  if(offsets[i][j]!=NULL)delete[] offsets[i][j];
		  if(slopes[i][j]!=NULL)delete[] slopes[i][j];
		}
	      if(offsets[i]!=NULL)delete []offsets[i];
	      if(slopes[i]!=NULL)delete []slopes[i];
	    }
	  if(offsets!=NULL)delete []offsets;
	  if(slopes!=NULL)delete []slopes;
	}
     }
  if(justClean)
    {
      offsetHistos2=NULL;
      offsetParser=NULL;
      return;
    }
  if(redefine)
    {
      vector<int> raa;
      findOffsetRanges(string("run ")+string_number(runNumber)+" even",raa);
      noffsetRanges=raa.size();
      if(noffsetRanges==0)noffsetRanges=1;
      offsetHistos2=new TH2F***[noffsetRanges];
      offsetRanges=new int[noffsetRanges+1];
      offsetRanges[0]=0;for(unsigned int i=0;i<raa.size();i++)offsetRanges[i+1]=raa[i];
      evtNr=(int)2.4e06;
      if(trackTree!=NULL)if(trackTree->GetEntries()>10)
	trackTree->GetEntry(trackTree->GetEntries()-1);
      if(pixTree!=NULL)if(pixTree->GetEntries()>10)
	pixTree->GetEntry(pixTree->GetEntries()-1);
      if(raa.size()==0)offsetRanges[1]=evtNr;
      offsetParser=new CommonCalibrationParser*[nCalibDets];
      for(int i=0;i<nCalibDets;i++)
	offsetParser[i]=new CommonCalibrationParser[noffsetRanges];
      offsets=new float**[noffsetRanges];
      slopes=new float**[noffsetRanges];
      for(int i=0;i<noffsetRanges;i++)
	{
	  offsets[i]=new float*[nCalibDets];
	  slopes[i]=new float*[nCalibDets];
	  offsetHistos2[i]=new TH2F**[nCalibDets];
	  for(int j=0;j<nCalibDets;j++)
	    {
	      offsetHistos2[i][j]=new TH2F*[numElements[j]];
	      offsets[i][j]=new float[numElements[j]];
	      slopes[i][j]=new float[numElements[j]];
	      for(int k=0;k<numElements[j];k++)
		{offsets[i][j][k]=0;slopes[i][j][k]=0;}
	    }
	}
    }
  if(redefine||mkeparser)
    {
      for(int i=0;i<noffsetRanges;i++)
	for(int j=0;j<nCalibDets;j++)
	  {
	    offsetParser[j][i].setDetID(detectors[j]);
	    offsetParser[j][i].setNumParamsElements(numElements[j],2);
	    offsetParser[j][i].setParamID(5);
	    offsetParser[j][i].setRanges(-1,-1);
	    offsetParser[j][i].setValid(true);
	    for(int k=0;k<numElements[j];k++)
	      for(int l=0;l<2;l++)
		offsetParser[j][i].setParameter(k,l,0);
	  }
    }
  if(redefine||mkehistos)
    {
      float *meanPosition[nCalibDets];
      TCanvas *canv=new TCanvas();
      canv->Divide(1,1);
      canv->cd(1);
      for(int j=0;j<nCalibDets;j++)
	{
	  TH2F tmphisto("tmpHisto","tmpHisto",numElements[j],-0.5,numElements[j]-0.5,2000,0,200);
	  meanPosition[j]=new float[numElements[j]];
	  int tn=-1, td=-1;
	  string dr="",cu="";
	  if(pixelBased[j]<0)
	    {
	      for(int k=0;k<ntrackElstructs;k++)
		for(int kk=0;kk<2;kk++)
		  if(trackEls[kk][k]==j){tn=kk;td=k;}
	      dr=string("tr.tdc[")+string_number(tn)+"]:tr.el["+string_number(tn)+"]>>tmpHisto";
	      cu=string("tr.det==")+string_number(td);
	    }
	  else
	    {
	      dr=string("pix.tdc[")+string_number(pixelDets[j])+"]:pix.el["
		+string_number(pixelDets[j])+"]>>tmpHisto";
	      cu=string("pix.ID")+"=="+string_number(pixelBased[j]);
	    }
	  if(pixelBased[j]>=0)
	    {
	      if(pixTree!=NULL)
		if(pixTree->GetEntries()>100)
		  pixTree->Draw(dr.data(),(cu).data());
	    }
	  else if(tn>=0&&td>=0)
	    {
	      if(trackTree!=NULL)
		if(trackTree->GetEntries()>100)
		  trackTree->Draw(dr.data(),(cu).data());
	    }
	  for(int k=0;k<numElements[j];k++)
	    {
	      meanPosition[j][k]=meanOff[2*j];
	      TH1F *his=getSlice(&tmphisto,k+1,0);
	      if(tmphisto.GetEntries()>100)
		meanPosition[j][k]=-his->GetMean()-
	          (twoSided[j]==measuresAgainst[j]?4*(twoSided[j]<j?1:1):0);//*30:0);
	      delete his;
	    }
	}
      delete canv;
      string s1="offset:evtNr ";
      for(int i=0;i<noffsetRanges;i++)
	for(int j=0;j<nCalibDets;j++)
	  {
	    int nx=100,ny=100,nev=offsetRanges[i+1]-offsetRanges[i];
	    float perc=1;
	    switch(detectors[j])
	      {
	      case 0: case 1: case 2: perc=0.3;break;
	      case 3: case 4: case 5: perc=0.5;break;
	      case 6: case 7: perc=0.4;break;
	      }
	    nx=(int)abs(nev/numElements[j]/200*perc);
	    if(nx>100) nx=100;
	    if(nx<5) nx=5;
	    ny=(int)abs(nev/numElements[j]/nx/5*perc);
	    if(ny>100) ny=100;
	    if(ny<5) ny=5;
	    for(int k=0;k<numElements[j];k++)
	      {
		offsetHistos2[i][j][k]=
		  new TH2F((s1+string_number(detectors[j])+"el"+string_number(k)+" "+string_number(i)).data(),
			   (s1+string_number(detectors[j])+" el "+string_number(k)).data(),
			   nx,offsetRanges[i],offsetRanges[i+1],ny,
			   meanPosition[j][k]-meanOff[j*2+1]*rngmod*
			   (twoSided[j]==measuresAgainst[j]?3:1),
			   meanPosition[j][k]+meanOff[j*2+1]*rngmod*
			   (twoSided[j]==measuresAgainst[j]?3:1));
	      }
	  }
      for(int j=0;j<nCalibDets;j++)
	delete []meanPosition[j];
    }
}
void ATDCcalibration::finalAndReset()
{
  ofstream output;
  output.open((calibrationOutputPath+"run"+string_number(runNumber)+"_"+string_number(runCounter)+".cal").data());
  cout<<"write data to \""<<(calibrationOutputPath+"run"+string_number(runNumber)+"_"+string_number(runCounter)+".cal").data()<<"\""<<endl;
  if(printPattern>0)
    {
      postscript=new TPostScript((calibrationOutputPath+"run"+string_number(runNumber)+"_"+string_number(runCounter)+"_control.ps").data(),112);
      postscript->Range(24,16);
    }
  runCounter++;
  eventCounter=0;
  if(doGeometry)doCalibrationGeo();
  if(doBeam)doCalibrationBeam();
  defineOffsetStuff(false,/*redefine*/true,/*mkehistos*/true,/*mkeparser*/true,/*rngmod*/1);
  if(doTwoSided)
    doCalibrationTwoSided();
  calculateOffsets(4);
  if(doWalk)
    doCalibrationWALK();
  if(doOffset)
    doCalibrationOffset();
  if(doTwoSided)
    doCalibrationTwoSided();
  //cout<<"calibration done"<<endl;
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<numCalibrations[i];j++)
	{
	  if(outParser[i][j].getParamID()==5)continue;
	  outParser[i][j].setRange(1,runNumber);
	  if(eventBased)
	    outParser[i][j].setRange(3,eventNumber);
	  /******************/
	  outParser[i][j].setRange(0,4000);
	  outParser[i][j].setRange(1,5200);
	  /*****************/
	  writeCalibration(&(outParser[i][j]),output);
	}
    }
  if(doWalk||doOffset)
    {
      for(int i=0;i<nCalibDets;i++)
	for(int j=0;j<noffsetRanges;j++)
	  {
	    offsetParser[i][j].setRange(0,runNumber);
	    offsetParser[i][j].setRange(1,runNumber);
	    offsetParser[i][j].setRange(2,offsetRanges[j]);
	    offsetParser[i][j].setRange(3,offsetRanges[j+1]);
	    writeCalibration(&(offsetParser[i][j]),output);
	  }
//       TCanvas *canvas=new TCanvas();
//       canvas->Divide(4,3);
//       int nc=0;
      resetWalkHistos();
//       string s1="offset:evtNr ";
//       for(int i=0;i<nCalibDets;i++)
// 	{
// 	  if(isStop[i])continue;
// 	  for(int k=0;k<numElements[i];k++)
// 	    {
// 	      delete offsetHistos2[0][i][k];
// 	      offsetHistos2[0][i][k]=new TH2F((s1+string_number(detectors[i])+"el"+string_number(k)+" 0").data(),
// 					      (s1+string_number(detectors[i])+" el "+string_number(k)).data(),
// 					      100,0,offsetRanges[1],100,
// 					      -meanOff[i*2+1],+meanOff[i*2+1]);	    
// 	    }
// 	}
      fillWalkHistos(7);
      for(int i=0;i<nCalibDets;i++)
	{

	  if(isStop[i])continue;
	  if(numRanges[i][3]==0)continue;
// 	  resolutionHisto1[i]->Fit("gaus","Q0+");
// 	  TF1 *f1=(TF1*)resolutionHisto1[i]->GetFunction("gaus");
// 	  float x=f1->GetParameter(2)*2;//resolutionHisto1[i]->GetFunction("gaus")->GetParameter(2)*3.;
// 	  for(int k=0;k<numElements[i];k++)
// 	    {
// 	      canvas->cd(nc+1);
// 	      offsetHistos2[0][i][k]->Draw("col");
// 	      nextPad(12,postscript,canvas,nc,((TObject**)NULL));
// 	    }
	  for(int k=0;k<numElements[i];k++)
	    {
// 	      canvas->cd(nc+1);
	      walkHistos[i][k+numElements[i]*2]->Fit("gaus","Q0+");
// 	      nextPad(12,postscript,canvas,nc,((TObject**)NULL));
	      TF1 *ff=walkHistos[i][k+numElements[i]*2]->GetFunction("gaus");
	      float x1=ff->GetX(0.01*ff->GetParameter(0),walkHistos[i][k+numElements[i]*2]->GetXaxis()->GetXmin(),ff->GetParameter(1));
	      float x2=ff->GetX(0.01*ff->GetParameter(0),ff->GetParameter(1),walkHistos[i][k+numElements[i]*2]->GetXaxis()->GetXmax());
	      for(int j=0;j<numRanges[i][3];j++)
		{
		  params[i][3][j]->setParameter(k,2,500+x1);
		  params[i][3][j]->setParameter(k,3,500+x2);
		}
	    }
	  for(int j=0;j<numRanges[i][3];j++)
	    writeCalibration(params[i][3][j],output);
	}
    }
  output.close();
  //cout<<"writing done"<<endl;
  if(printPattern>0)
    {
      postscript->Close();
      delete postscript;
    }
  //cout<<"postscript closed"<<endl;
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<numCalibrations[i];j++)
	{
	  outParser[i][j].setRange(1,runNumber);
	  if(eventBased)
	    outParser[i][j].setRange(3,eventNumber);
	  for(int l=0;l<numElements[i];l++)
	    for(int k=0;k<outParser[i][j].getNumParameters();k++)
	      outParser[i][j].setParameter(l,k,0);
	}
    }
  //cout<<"parser structure resetted"<<endl;
  if(doWalk&&eventBased)
    {
      if(hasTrack)trackTree->Reset();
      if(hasPix)pixTree->Reset();
    }
  //cout<<"trees reseted"<<endl;
  if(doOffset&&eventBased)
    elTree->Reset();

  //cout<<"reset done"<<endl;
}
#include <qprocess.h>
ATDCcalibration::~ATDCcalibration()
{
  //cout<<"final and reset"<<endl;
  if(!readFile)
    {
      outFile->cd();
      if(doWalk)
	{
	  outFile->cd();
	  if(hasPix)
	    pixTree->Write();
	  outFile->cd();
	  if(hasTrack)
	    trackTree->Write();
	}
      outFile->cd();
      if(doOffset)
	elTree->Write();
    }
  finalAndReset();
  //cout<<"cleanup"<<endl;
  if(doGeometry)
    {
      for(int i=0;i<nCalibDets;i++)
	delete geoHistos[i];
      delete[] geoHistos;
    }
  //cout<<"done geo"<<endl;
  if(doBeam)
    {
      for(int i=0;i<40;i++)
	{
	  for(int j=0;j<40;j++)
	    delete beamHistos[i][j];
	  delete [] beamHistos[i];
	}
      delete []beamHistos;
    }
  //cout<<"done beam"<<endl;
  if(doTwoSided)
    {
      for(int i=0;i<nTwoSided;i++)
	{
	  for(int j=0;j<numElements[twoDets[i]];j++)
	    delete twoSidedHisto[i][j];
	  delete []twoSidedHisto[i];
	}
      delete[] twoDets;
      delete []twoSidedHisto;
    }
  //cout<<"done twoSided"<<endl;
  if(doWalk||doOffset)
    {
      for(int i=0;i<nCalibDets;i++)
	{
	  delete resolutionHisto[i*2];
	  delete resolutionHisto[i*2+1];
	  delete resolutionHisto1[i];
	  delete []outParser[i];
	}
      delete []resolutionHisto;
      delete []resolutionHisto1;
      delete[]outParser;

      
     if(doWalk)
	{
	  defineWalkStuff(true,false,true,15,true,1);
	  delete []trackEls[0];
	  delete []trackEls[1];
	}
      for(int i=0;i<nCalibDets;i++)
	{
	  for(int j=0;j<numElements[i];j++)
	    delete offsetHistos[i][j];
	  delete []offsetHistos[i];
	}
      delete []offsetHistos;
      defineOffsetStuff(true,true,true,true,1);
      //cout<<"done walk and offset histos"<<endl;
      
      string s=outFile->GetName();
      outFile->Close();
      delete outFile;
      //cout<<"done file"<<endl;
      if(killFileOnEnd)
	{
#if QT_VERSION < 0x040000
	  QProcess killer(string("rm"));
	  killer.addArgument("-f");
	  killer.addArgument(s);
	  killer.start();
	  anaLog<<"deleting file "<<s;
	  while(killer.isRunning()){}
	  anaLog<<" done"<<endli;
#else
	  QProcess killer;
	  killer.start((string("rm -f ")+s).data());
	  anaLog<<"deleting file "<<s;
	  while(killer.waitForFinished()){}
	  anaLog<<" done"<<endli;
#endif
	}
    }
  //cout<<"done histograms"<<endl;
  delete []numCalibrations;
  for(int i=0;i<4;i++)
    delete []calibPos[i];
  delete []numElements;
  delete []detectors;
  delete []pixelBased;
  delete []isStop;
  delete []measuresAgainst;
  delete []meanOff;
  delete []mimaqdc;
  delete []min_maxLRP;
  delete []refPixels;
  //cout<<"working structures done"<<endl;
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<3;j++)
	if(params[i][j]!=NULL)
	  {
	    for(int k=0;k<numRanges[i][j];k++)
	      if(params[i][j][k]!=NULL)
		delete params[i][j][k];
	    delete []params[i][j];
	  }
      delete []params[i];
      delete []numRanges[i];
      delete []actualRanges[i];
    }
  delete []params;
  delete []numRanges;
  delete []actualRanges;
  //cout<<"parameter done"<<endl;
}
void* ATDCcalibration::process(void* ptr)
{
  //  cout<<"process()"<<endl;
  if(readFile)return ptr;
  if(doGeometry)fillGeoHisto();
  if(doBeam)fillBeamHisto();
  if(doWalk||doOffset)
    {
      findRanges();
      if(doWalk)
	{
	  fillPixTree();
	  fillTrackTree();
	}
      if(doOffset)fillElTree();
      if(doOffset&&!doWalk)fillOnlineOffsetHisto();
    }
  if(doTwoSided&&!(doWalk||doOffset))
    fillOnlineTwoSidedHisto();
  if(eventBased)
    {
      eventCounter++;
      if(eventCounter>=sumEvents)
	finalAndReset();
    }
  return ptr;
}
vector<string>ATDCcalibration::treeNames()
{
  vector<string>ret;
  if(doOffset)ret.push_back(elTree->GetName());
  if(doWalk)
    {
      if(hasPix)ret.push_back(pixTree->GetName());
      if(hasTrack)ret.push_back(trackTree->GetName());
    }
  return ret;
}
TTree* ATDCcalibration::tree(string treename)
{
  if(doOffset)if(treename==elTree->GetName())return elTree;
  if(doWalk)
    {
      if(hasPix)if(treename==pixTree->GetName())return pixTree;
      if(hasTrack)if(treename==trackTree->GetName())return trackTree;
    }
  return NULL;
}
algorithm_parameter ATDCcalibration::getDescription()
{
  algorithm_parameter ret("Calibration algorithm",-1,0,0);
  vector<string> des;
  des.push_back("This algorithm does the calibration of detector- and beam geometry and ");
  des.push_back("calibration of TDC walk and offsets. ");
  des.push_back("For geometry calibration some pixels are taken for reference producing a ");
  des.push_back("line when connected to the target. The distance of this line to the hit ");
  des.push_back("element is plotted versus the element number. This gives information about ");
  des.push_back("the in plane shift of the detector center and the z-position. ");
  des.push_back("Beam geometry takes all 2-track events, plotting the angle of the assumed ");
  des.push_back("beam with the plane the two tracks define. ");
  des.push_back("Walk plotts the tdc-difference of defined pixel elements versus the qdc ");
  des.push_back("of the element. This is done iteratively. Here also some offsets are ");
  des.push_back("determined to nivellize before plotting.");
  des.push_back("Offset determination is done as last, moving all tdcs to the same mean ");
  des.push_back("value for one detector (starts at 500). The offset between detectors is");
  des.push_back("calculated using elastics, where the TOF can be calculated.");
  des.push_back("Note: beam-, Walk- and offset calibration need tracking, while geometry");
  des.push_back("calibration is essential for tracking. If you select geometry all other");
  des.push_back("types are switched off!");
  ret.setDescription(des);
  ret.addParam<bool>(single_parameter<bool>("do geometry calibration",false));
  ret.addParam<bool>(single_parameter<bool>("do beam calibration",false));
  ret.addParam<bool>(single_parameter<bool>("do WALK calibration",true));
  ret.addParam<bool>(single_parameter<bool>("do TDC-Offset calibration",true));
  ret.addParam<bool>(single_parameter<bool>("do on event basis",false));
  ret.addParam<int>(single_parameter<int>("n Events for cal",100000));
  ret.addParam<int>(single_parameter<int>("number of iterations",4));
  ret.addParam<int>(single_parameter<int>("print pattern",0));
  vector<int> tmp;
  vector<float> tmpf;
  ret.addParam<vector<int> >(single_parameter<vector<int> >("detectors to calibrate",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("pixel based",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("measures against",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("is Stop pixel",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("has 2-sided readout",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("pixels dets",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("pixel det references to det",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("walk do only single iteration",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("do light run correction",tmp));
  ret.addParam<vector<int> >(single_parameter<vector<int> >("reference pixels",tmp));
  ret.addParam<vector<float> >(single_parameter<vector<float> >("mean, width for offset",tmpf));
  ret.addParam<vector<float> >(single_parameter<vector<float> >("min, max for qdc",tmpf));
  ret.addParam<string>(single_parameter<string>("calibration Output Path",""));
  ret.addParam<string>(single_parameter<string>("author",""));
  return ret;
}
void ATDCcalibration::onNewRun(run_parameter &rp)
{
  searchParser(rp);
  beamtime="default";
  if(rp.getParent()!=NULL)beamtime=rp.getParent()->getName();
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<numCalibrations[i];j++)
	if(outParser[i][j].getRange(0)<=0)
	  outParser[i][j].setRange(0,rp.getRunNumber());
	else if(outParser[i][j].getRange(1)<rp.getRunNumber())
	  outParser[i][j].setRange(1,rp.getRunNumber());
    }
  runCounter=0;
}
void ATDCcalibration::searchParser(const run_parameter &rp)
{
  int nP=4;
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<nP;j++)
	{
	  if(params[i][j]!=NULL)
	    {
	      for(int k=0;k<numRanges[i][j];k++)
		if(params[i][j][k]!=NULL)
		  delete params[i][j][k];
	      delete []params[i][j];
	    }
	    params[i][j]=NULL;
	    numRanges[i][j]=0;
	    actualRanges[i][j]=0;
	}
    }
  vector<CommonCalibrationParser*> tmp;
  vector<string> files;
  if(rp.hasOwnCalibration()||rp.getParent()==NULL||rp.hasAdditionalCalibration())
    for(int i=0;i<rp.getNumberOfCalibrationFiles();i++)
      files.push_back(rp.getCalibrationFile(i));
  if(rp.getParent()!=NULL&&(!rp.hasOwnCalibration()||rp.hasAdditionalCalibration()))
    for(int i=0;i<rp.getParent()->getNumberOfCalibrationFiles();i++)
      files.push_back(rp.getParent()->getCalibrationFile(i));
  for(unsigned int i=0;i<files.size();i++)
    CommonCalibrationParser::getFileContent(files[i], tmp);
  vector<CommonCalibrationParser*> ttmp[nP];
  int used;
  while(!tmp.empty())
    {
      used=-1;
      for(int j=0;j<nCalibDets;j++)
	if(tmp.back()->getDetector()==detectors[j]&&
	   (tmp.back()->getParamID()==0||tmp.back()->getParamID()==1||tmp.back()->getParamID()==2||tmp.back()->getParamID()==3))
	  used=j;
      if(used>=0)
	{
	  ttmp[tmp.back()->getParamID()].push_back(tmp.back());
	  numRanges[used][tmp.back()->getParamID()]++;
	  //	  if(!isStop[used]&&tmp.back()->getParamID()==3)cout<<detectors[used]<<endl;
	}
      else
	{
	  CommonCalibrationParser *tpp=tmp.back();
	  delete tpp;
	}
      tmp.pop_back();
    }
  for(int i=0;i<nCalibDets;i++)
    for(int j=0;j<nP;j++)
      {
	params[i][j]=new CommonCalibrationParser*[numRanges[i][j]];
	numRanges[i][j]=0;
      }
  for(int j=0;j<nP;j++)
    {
      while(!ttmp[j].empty())
	{
	  if(ttmp[j].back()==NULL)
	    {
	      ttmp[j].pop_back();
	      continue;
	    }
	  for(int i=0;i<nCalibDets;i++)
	    if(ttmp[j].back()->getDetector()==detectors[i])
	      {
		params[i][j][numRanges[i][j]]=ttmp[j].back();
		ttmp[j].pop_back();
		numRanges[i][j]++;
		break;
	      }
	}
    }  
}
void ATDCcalibration::findRanges()
{
  bool found;
  int closest;
  int diff1,diff2,diff3,diff4;
  for(int i=0;i<nCalibDets;i++)
    {
      for(int j=0;j<3;j++)
	{
	  if(numRanges[i][j]<=1)continue;
	  found=(params[i][j][actualRanges[i][j]]->getRange(0)>=runNumber&&
		 params[i][j][actualRanges[i][j]]->getRange(1)<=runNumber&&
		 params[i][j][actualRanges[i][j]]->getRange(2)>=eventNumber&&
		 params[i][j][actualRanges[i][j]]->getRange(3)<=eventNumber);
	  if(found)continue;
	  closest=actualRanges[i][j];
	  diff1=(int)abs(params[i][j][closest]->getRange(0)-runNumber);
	  if(abs(params[i][j][closest]->getRange(1)-runNumber)<diff1)
	    diff1=(int)abs(params[i][j][closest]->getRange(1)-runNumber);
	  diff2=(int)abs(params[i][j][closest]->getRange(2)-eventNumber);
	  if(abs(params[i][j][closest]->getRange(3)-eventNumber)<diff2)
	    diff2=(int)abs(params[i][j][closest]->getRange(3)-eventNumber);

	  for(int k=0;k<numRanges[i][j];k++)
	    {
	      diff3=(int)abs(params[i][j][k]->getRange(0)-runNumber);
	      if(abs(params[i][j][k]->getRange(1)-runNumber)<diff3)
		diff3=(int)abs(params[i][j][k]->getRange(1)-runNumber);
	      diff4=(int)abs(params[i][j][k]->getRange(2)-eventNumber);
	      if(abs(params[i][j][k]->getRange(3)-eventNumber)<diff4)
		diff4=(int)abs(params[i][j][k]->getRange(3)-eventNumber);
	      if(diff1>diff3||(diff1==diff3&&diff2>diff4))
		{
		  closest=k;
		  diff1=(int)abs(params[i][j][closest]->getRange(0)-runNumber);
		  if(abs(params[i][j][closest]->getRange(1)-runNumber)<diff1)
		    diff1=(int)abs(params[i][j][closest]->getRange(1)-runNumber);
		  diff2=(int)abs(params[i][j][closest]->getRange(2)-eventNumber);
		  if(abs(params[i][j][closest]->getRange(3)-eventNumber)<diff2)
		    diff2=(int)abs(params[i][j][closest]->getRange(3)-eventNumber);
		}
	    }
	  actualRanges[i][j]=closest;
	}
    }
}
void ATDCcalibration::fillGeoHisto()
{
  bool has=false;
  for(int i=0;i<nCalibDets;i++)
    if(*(numberOfHits[detectors[i]])>0)has=true;
  if(!has)return;

  TCalibHit *currHit;
  volumeShape *hitSh;
  vector3D tmpv;
  for(int j=0;j<nRefPix;j++)
    for(int k=0;k<*(numberOfPixels[refPixels[j]]);k++)
      {
	sLine3D line(point3D(0,0,0),pixels[refPixels[j]][k]->getShape()->getCenter()-point3D(0,0,0));
	for(int i=0;i<nCalibDets;i++)
	  for(int l=0;l<*(numberOfHits[detectors[i]]);l++)
	    {
	      currHit=hits[detectors[i]][l];
	      if(!currHit->isValid())continue;
	      if(currHit->getHitShape()==NULL)continue;
	      hitSh=currHit->getHitShape();
	      if(hitSh->getName()=="fiber")
		{
		  tmpv=hitSh->distance(line);//line -> shape
		  if(tmpv.R()>0)//left of shape+ , right of shape -
		    geoHistos[l]->Fill(currHit->getElement(),tmpv.R()*(tmpv*((fiber*)hitSh)->getDirection(1)<0?1:-1));
		}
	      else if(hitSh->getName()=="ring")
		{
		  plane3D pl(hitSh->getCenter(),((ring*)hitSh)->getNormal());
		  point3D p=pl-line;
		  tmpv=p-hitSh->getCenter();
		  if(tmpv.R()<((ring*)hitSh)->getInnerRadius())
		    geoHistos[l]->Fill(currHit->getElement(),tmpv.R()-((ring*)hitSh)->getInnerRadius());
		  
		  else if(tmpv.R()>((ring*)hitSh)->getOuterRadius())
		    geoHistos[l]->Fill(currHit->getElement(),tmpv.R()-((ring*)hitSh)->getOuterRadius());
		}
	      else if(hitSh->getName()=="wedge")
		{
		  tmpv=hitSh->distance(line);
		  if(tmpv.R()>0)
		    geoHistos[l]->Fill(currHit->getElement(),
				       tmpv.R()*(((wedge*)hitSh)->getNormal().angle(tmpv,((wedge*)hitSh)->getLowerPhiEdge())<0?1:-1));
		}
	    }
      }
}
void ATDCcalibration::fillBeamHisto()
{
  if(numberOfTracks!=2)return;
  vector3D beam;
  plane3D pl(point3D(0,0,0),tracks[0]->getPath().Direction(),tracks[1]->getPath().Direction());
  for(int i=0;i<40;i++)
    for(int j=0;j<40;j++)
      {
	beam.setValues((i-19.5)*0.001,(j-19.5)*0.001,1);beam.normalize();
	beamHistos[i][j]->Fill(asin((beam*pl.Normal())/beam.R())*180./M_PI);
      }
}
extern float getLightWay(volumeShape *sh, point3D onPoint, bool from);
float ATDCcalibration::getCalibratedTDC(const TRawHit &h, int dn, const point3D &p)
{
  float tdc,qdc;
  int el=h.getElement();
  if(el<0||el>numElements[dn])return -1;
  qdc=h.getADC();
  tdc=h.getTDC();
  if(numRanges[dn][0]>0)qdc=qdc-params[dn][0][actualRanges[dn][0]]->getParameter(el,0);
  float wal=0;
  if(numRanges[dn][2]>0)
    {
      wal=params[dn][2][actualRanges[dn][2]]->getParameter(el,0)/
	(qdc-params[dn][2][actualRanges[dn][2]]->getParameter(el,1))
	+params[dn][2][actualRanges[dn][2]]->getParameter(el,2)*
	log(abs(qdc-params[dn][2][actualRanges[dn][2]]->getParameter(el,3)))
	+params[dn][2][actualRanges[dn][2]]->getParameter(el,4);
    }
  if(numRanges[dn][1]>0)
    tdc=tdc*params[dn][1][actualRanges[dn][1]]->getParameter(el,0);
  if(!doWalk)tdc=tdc+wal;
  volumeShape *sh=setup.getDetectorr(detectors[dn]).getShape(el);
  if(p.getState()!=_undefined_&&sh!=NULL&&h.getDetector()!=6&&h.getDetector()!=7)
    tdc=tdc+getLightWay(sh,p, false)/setup.getDetectorr(detectors[dn]).getMaterial()->getSpeedOfLight()/299.792;
  return tdc;
}
void ATDCcalibration::fillPixTree()
{
  bool use;
  int dets[3];
  point3D p;
  evtNr=eventNumber;
  for(int i=0;i<10;i++)
    {
      if(*(numberOfPixels[i])==0)continue;
      use=false;
      for(int j=0;j<nCalibDets;j++)
	if(pixelBased[j]==i)use=true;
      if(use)
	{
	  if(*(numberOfPixels[i])>0)
	    for(int k=0;k<pixels[i][0]->getNumberOfActualElements();k++)
	      for(int j=0;j<nCalibDets;j++)
		if(pixels[i][0]->getElementr(k).getDetector()==detectors[j]) dets[k]=j;
	  outpix.ID=i;
	  for(int j=0;j<*(numberOfPixels[i]);j++)
	    {
	      p=pixels[i][j]->getShape()->getCenter();
	      for(int l=0;l<pixels[i][j]->getNumberOfActualElements();l++)
		{
		  outpix.tdcs[l]=getCalibratedTDC(pixels[i][j]->getElementr(l).getRaw(), dets[l], p);
		  outpix.el[l]=pixels[i][j]->getElementr(l).getElement();
		  outpix.qdcs[l]=pixels[i][j]->getElementr(l).getRawADC()
		    -params[dets[l]][0][actualRanges[dets[l]][0]]->getParameter(outpix.el[l],0);
		}
	      outpix.pointx=p.X();
	      outpix.pointy=p.Y();
	      outpix.pointz=p.Z();
	      pixTree->Fill();
	    }
	}
    }
}
void ATDCcalibration::fillTrackTree()
{
  if(numberOfTracks==0)return;
  evtNr=eventNumber;
  TCalibHit *currentHit;
  point3D p;
  TTrack *currentTrack;
  vector3D nor;
  int kk=0;
  for(int i=0;i<numberOfTracks;i++)
    {
      currentTrack=tracks[i];
      if(currentTrack==NULL)continue;
      outtrack_el.theta=currentTrack->getPath().Direction().Theta();
      outtrack_el.phi=currentTrack->getPath().Direction().Phi();
      for(int k=0;k<ntrackElstructs;k++)
	{
	  if(currentTrack->getNumberOfCalibHits(detectors[trackEls[0][k]])>0&&
	     currentTrack->getNumberOfCalibHits(detectors[trackEls[1][k]])>0)
 	    {
	      outtrack_el.det=k;
	      for(int j=0;j<2;j++)
		{
		  currentHit=currentTrack->getCalibSrcr(detectors[trackEls[j][k]],kk);
		  if(currentHit==NULL)continue;
		  if(currentHit->getHitShape()==NULL)continue;
		  if(currentHit->getElement()>=numElements[trackEls[j][k]]||currentHit->getElement()<0)continue;
		  p=getPoint(currentHit->getHitShape(),currentTrack->getPath());
		  outtrack_el.pointz[j]=p.Z();
		  outtrack_el.el[j]=currentHit->getElement();
		  outtrack_el.tdcs[j]=getCalibratedTDC(currentHit->getRaw(), trackEls[j][k],p);//currentHit->getRawTDC();
		  outtrack_el.qdcs[j]=currentHit->getRawADC()
		    -params[trackEls[j][k]][0][actualRanges[trackEls[j][k]][0]]->getParameter(outtrack_el.el[j],0);
		}
	      trackTree->Fill();
 	    }
	}
    }
}
void ATDCcalibration::fillElTree()
{
  if(numberOfTracks!=2)return;
  evtNr=eventNumber;
  float mass=Eparticles::getMassG(14);
  vector3D beam=setup.getBeamr().getParticle();
  float gammaCM=(setup.getBeamr().getParticle()+setup.getTargetr().getParticle()).Gamma();
  float momentum=beam.R();
  beam.normalize();
  vector3D d[2]={tracks[0]->getPath().Direction(),tracks[1]->getPath().Direction()};
  vector3D n[2]={d[0]^beam,beam^d[1]};
  for(int i=0;i<2;i++)n[i].normalize();
  float ang=(n[0]*n[1])/(n[0].R()*n[1].R());
  if(acos(ang)>0.05)return;//not coplanar!!!
  
  float th1=acos((d[0]*beam)/(d[0].R()*beam.R())),
    th2=acos((d[1]*beam)/(d[1].R()*beam.R()));
  float tth1=tan(th1),tth2=tan(th2);
  float gamma=1/sqrt(tth1*tth2);
  float pt=tth1*tth2/(tth1+tth2);
  float pl1=tth2/(tth1+tth2);
  float pl2=tth1/(tth1+tth2);
  float p1=momentum*sqrt(pt*pt+pl1*pl1);
  float p2=momentum*sqrt(pt*pt+pl2*pl2);
  momentum4D moms[2];
  if(abs(gamma-gammaCM)>0.06)//dpi+
    return;
    
  TCalibHit *currentHit;
  point3D p;
  TTrack *currentTrack;
  vector3D nor;
  for(int i=0;i<2;i++)
    {
      currentTrack=tracks[i];
      if(currentTrack==NULL)return;
      outtrack[i].theta=currentTrack->getPath().Direction().Theta();
      outtrack[i].phi=currentTrack->getPath().Direction().Phi();
      outtrack[i].vt[0]=currentTrack->getPath().Foot().X();
      outtrack[i].vt[1]=currentTrack->getPath().Foot().Y();
      outtrack[i].vt[2]=currentTrack->getPath().Foot().Z();
      for(int k=0;k<nCalibDets;k++)
	{
	  if(k>=10)continue;
	  outtrack[i].tdcs[k]=-1;
	  outtrack[i].qdcs[k]=-1;
	  outtrack[i].el[k]=-1;
	  if(currentTrack->getNumberOfCalibHits(detectors[k])>0)
	    {
	      int kk=0;
	      currentHit=currentTrack->getCalibSrcr(detectors[k],kk);
	      if(currentHit==NULL)continue;
	      if(currentHit->getHitShape()==NULL)continue;
	      if(currentHit->getElement()>=numElements[k]||currentHit->getElement()<0)continue;
	      p=getPoint(currentHit->getHitShape(),currentTrack->getPath());
	      outtrack[i].el[k]=currentHit->getElement();
	      outtrack[i].tdcs[k]=getCalibratedTDC(currentHit->getRaw(), k,p);//currentHit->getRawTDC();
	      outtrack[i].qdcs[k]=currentHit->getRawADC()
		-params[k][0][actualRanges[k][0]]->getParameter(outtrack[i].el[k],0);
	      outtrack[i].pointz[k]=p.Z();
	    }
	}
    }
  moms[0].setPM(p1*d[0],mass);
  moms[1].setPM(p2*d[1],mass);

  for(int j=0;j<2;j++)
    outtrack[j].bt=moms[j].Beta();
  elTree->Fill();

}
void ATDCcalibration::fillWalkHistos(int pattern)//, float ***offsets,float*** slopes)
{
  cout<<"."<<flush;
  //cout<<"fillWalkHistos(int pattern="<<pattern<<", float **offsets)"<<flush;
  bool c1=((pattern&1)>0);
  bool c2=((pattern&2)>0);
  int n, nSum;
  int el[nCalibDets];
  int det[2];
//   float ttdc[2];
  float tdcs[2][nCalibDets];
  float qdcs[nCalibDets];
  float z[2];
  float tdcSum;
  float twoadd;
  float lrp[nCalibDets];
  if(resolutionHisto!=NULL)
    for(int i=0;i<nCalibDets*2;i++)
      if(resolutionHisto[i]!=NULL)resolutionHisto[i]->Reset();
  if(resolutionHisto1!=NULL)
    for(int i=0;i<nCalibDets;i++)
      if(resolutionHisto1[i]!=NULL)resolutionHisto1[i]->Reset();
  float zfront[nCalibDets];
  float zmid[nCalibDets];
  for(int i=0;i<nCalibDets;i++)
    {
      zfront[i]=0;
      zmid[i]=0;
      if(calibPos[3][i]<0)continue;
      wedge* sh=((wedge*)setup.getDetectorr(detectors[i]).getShape(0));
      zfront[i]=sh->getCenter().Z()-sqrt(sh->getOuterRadius()*sh->getOuterRadius()-sh->getDistToBarrelCenter()*sh->getDistToBarrelCenter());
      float z1=sqrt(sh->getOuterRadius()*sh->getOuterRadius()-sh->getDistToBarrelCenter()*sh->getDistToBarrelCenter());
      float z2=z1*(sh->getInnerRadius()/sh->getOuterRadius());
      zmid[i]=(z1-z2)/2.;
    }
  int cor=0;
  if(hasTrack)
    {
      for(int j=0;j<trackTree->GetEntries()/*500000*/;j++)
	{
	  trackTree->GetEntry(j);
	  while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
	  el[0]=-1;el[1]=-1;
	  for(int i=0;i<2;i++)
	    {
	      det[i]=trackEls[i][outtrack_el.det];
	      qdcs[i]=outtrack_el.qdcs[i];
	      el[i]=outtrack_el.el[i];
	      z[i]=outtrack_el.pointz[i];
	      if(el[i]<numElements[det[i]]&&el[i]>=0)
		{
		  lrp[i]=getLightWay(setup.getDetectorr(detectors[det[i]]).getShape(el[i]), 
				     point3D(z[i]*tan(outtrack_el.theta)*cos(outtrack_el.phi),
					     z[i]*tan(outtrack_el.theta)*sin(outtrack_el.phi),
					     z[i]), false);
		  tdcs[0][i]=getTDC(evtNr,det[i], el[i], outtrack_el.tdcs[i], outtrack_el.qdcs[i], lrp[i], cor/*offsets[cor],slopes[cor]*/, c1, true, (!twoSided[det[i]]));
		  tdcs[1][i]=getTDC(evtNr,det[i], el[i], outtrack_el.tdcs[i], outtrack_el.qdcs[i], lrp[i], cor/*offsets[cor],slopes[cor]*/, c2, true, (!twoSided[det[i]]));
		}
	    }
	  if(pixelBased[trackEls[0][outtrack_el.det]]>=0||el[0]<0)continue;
	  if(pixelBased[trackEls[1][outtrack_el.det]]>=0||el[1]<0)continue;
	  twoadd=0;
 	  if(calibPos[3][det[0]]>=0)
	    {
	      twoadd=-(((outtrack_el.pointz[0]/*+zmid[det[0]]*0.5*/-zfront[det[0]])/10.+
			-outParser[det[0]][calibPos[3][det[0]]].getParameter(el[0],1))
		       /outParser[det[0]][calibPos[3][det[0]]].getParameter(el[0],0))*2.;
// 	      cout<<outtrack_el.pointz[0]<<" "<<det[0]<<" "<<zfront[det[0]]<<" "<<outParser[det[0]][calibPos[3][det[0]]].getParameter(el[0],1)<<" "<<outParser[det[0]][calibPos[3][det[0]]].getParameter(el[0],0)<<" "<<flush;
// 	      cout<<twoadd<<endl;
	    }
	  if((pattern&4)>0)
	    for(int i=0;i<2;i++)
	      {
		if(qdcs[i]>1e8||el[i]<0)continue;
		walkHistos[det[i]][numElements[det[i]]+el[i]]->Fill(qdcs[i]);
		if(lrp[i]>=min_maxLRP[det[i]*2]&&lrp[i]<=min_maxLRP[det[i]*2+1])
		  {
		    walkHistos[det[i]][numElements[det[i]]*2+el[i]]->Fill(-tdcs[0][i]-0.5*twoadd*(2*i-1));
		    offsetHistos2[cor][det[i]][el[i]]->Fill(evtNr,-tdcs[0][i]-0.5*twoadd*(2*i-1));
		  }
	      }
	  if(twoSided[det[0]]==det[1])
	    {
	      for(int i=0;i<2;i++)
		{
		  if(lrp[i]>=min_maxLRP[det[i]*2]&&lrp[i]<=min_maxLRP[det[i]*2+1])
		    {
		      walkHistos[det[i]][el[i]]->Fill(qdcs[i], tdcs[1][i]-tdcs[0][1-i]+twoadd*(2*i-1));
		    }
		  resolutionHisto[det[i]*2]->Fill(el[i],   tdcs[1][i]-tdcs[0][1-i]+twoadd*(2*i-1));
		  resolutionHisto[det[i]*2+1]->Fill(lrp[i],tdcs[1][i]-tdcs[0][1-i]+twoadd*(2*i-1));
		  resolutionHisto1[det[i]]->Fill(          tdcs[1][i]-tdcs[0][1-i]+twoadd*(2*i-1));
		  if(doLRT[det[i]])
		    walkHistos[det[i]][el[i]+3*numElements[det[i]]]->
		      Fill(lrp[i],tdcs[1][i]-tdcs[0][1-i]+twoadd*(2*i-1));
		}
	    }
	  else 
	    {
	      for(int i=0;i<2;i++)
		{
		  if(lrp[i]>=min_maxLRP[det[i]*2]&&lrp[i]<=min_maxLRP[det[i]*2+1])
		    walkHistos[det[i]][el[i]]->Fill(qdcs[i], tdcs[1][i]-tdcs[0][1-i]);
		  resolutionHisto[det[i]*2]->Fill(el[i],   tdcs[1][i]-tdcs[0][1-i]);
		  resolutionHisto[det[i]*2+1]->Fill(lrp[i],tdcs[1][i]-tdcs[0][1-i]);
		  resolutionHisto1[det[i]]->Fill(          tdcs[1][i]-tdcs[0][1-i]);
		  if(doLRT[det[i]])
		    walkHistos[det[i]][el[i]+3*numElements[det[i]]]->Fill(lrp[i],tdcs[1][i]-tdcs[0][1-i]);
		}
	    }
	}
    }
  cor=0;
  if(hasPix)
    {
      float theta;
      for(int j=0;j<pixTree->GetEntries()/*50000*/;j++)
	{
	  pixTree->GetEntry(j);
	  while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
	  theta=atan(sqrt(outpix.pointx*outpix.pointx+outpix.pointy*outpix.pointy)/outpix.pointz);
	  for(int i=0;i<nCalibDets;i++)
	    {
	      tdcs[0][i]=1e9;
	      qdcs[i]=1e9;
	      el[i]=-1;
	      if(outpix.ID!=pixelBased[i])continue;
	      if(outpix.el[pixelDets[i]]<numElements[i]&&outpix.el[pixelDets[i]]>=0)
		{
		  qdcs[i]=outpix.qdcs[pixelDets[i]];
		  el[i]=outpix.el[pixelDets[i]];
		  if(el[i]<0)continue;
		  lrp[i]=getLightWay(setup.getDetectorr(detectors[i]).getShape(el[i]), 
				     point3D(outpix.pointx,outpix.pointy,outpix.pointz), false);
		  tdcs[0][i]=getTDC(evtNr,i, el[i], outpix.tdcs[pixelDets[i]], outpix.qdcs[pixelDets[i]], lrp[i], cor/*offsets[cor],slopes[cor]*/, c1, true, true);
		  tdcs[1][i]=getTDC(evtNr,i, el[i], outpix.tdcs[pixelDets[i]], outpix.qdcs[pixelDets[i]], lrp[i], cor/*offsets[cor],slopes[cor]*/, c2, true, true);
		}
	    }
	  if((pattern&4)>0)
	    for(int i=0;i<nCalibDets;i++)
	      {
		if(qdcs[i]>1e8||el[i]<0)continue;
		walkHistos[i][numElements[i]+el[i]]->Fill(qdcs[i]);
		walkHistos[i][numElements[i]*2+el[i]]->Fill(-tdcs[0][i]);
		offsetHistos2[cor][i][el[i]]->Fill(evtNr,-tdcs[0][i]);
	      }
	  for(int i=0;i<nCalibDets;i++)
	    {
	      if(pixelBased[i]<0||el[i]<0)continue;
	      if(measuresAgainst[i]<nCalibDets)
		{
		  tdcSum=tdcs[0][measuresAgainst[i]];
		}
	      else
		{
		  tdcSum=0;
		  nSum=0;
		  n=measuresAgainst[i];
		  while(n>0)
		    {
		      tdcSum+=tdcs[0][n%nCalibDets];
		      if(n<nCalibDets)n=-1;
		      else
			n=n/nCalibDets;
		      nSum++;
		    }
		  tdcSum=tdcSum/((double)nSum);
		}
	      if(!((detectors[i]%3)>0&&lrp[i]>=min_maxLRP[i*2]&&lrp[i]<=min_maxLRP[i*2+1]))//lrp[i]>100))
		walkHistos[i][el[i]]->Fill(qdcs[i],tdcs[1][i]-tdcSum);
	      resolutionHisto[i*2]->Fill(el[i],    tdcs[1][i]-tdcSum);
	      resolutionHisto1[i]->Fill(           tdcs[1][i]-tdcSum);
	      if(doLRT[i])
		walkHistos[i][el[i]+3*numElements[i]]->Fill(lrp[i],tdcs[1][i]-tdcs[0][detectors[i]<3?8:9]);
	      if((detectors[i]%3)==0)
		resolutionHisto[i*2+1]->Fill(lrp[i],tdcs[1][i]-tdcSum);
	      else
		resolutionHisto[i*2+1]->Fill(lrp[i],tdcs[1][i]-tdcs[0][detectors[i]<3?8:9]);
	    }
	}
    }
}
#include "shape_utilities.h"
void ATDCcalibration::fillOffsetHistos(/*float ***offsets, float ***slopes,*/ TH1***absOffsetHistos, int pattern)
{
  cout<<"x"<<flush;
  for(int i=0;i<nCalibDets;i++)
    {
      (*absOffsetHistos)[i]=NULL;
      if(!isStop[i])
	{
	(*absOffsetHistos)[i]=new TH1F((string("difference starts")+string_number(detectors[i])).data(),
				    (string("\\Delta TOF start1-start2")+
				     string_number(detectors[i])).data(),100,-2,2);
	}
      else 
	{
	  float thmin=0,thmax=thMax(i);
	  float tofmean=-12.5, tofrange=1.5;
	  if(doLRT[i])
	    tofrange=2;
	  else if(twoSided[i]>=0)
	    {
	      tofmean=-10;
	      tofrange=2;
	      if(pattern<5)tofrange=2+0.1*(5-pattern);
	    }
	  if(calibPos[2][i]>=0&&(pattern<=1||(pattern<5&&twoSided[i]>=0)))
	    {
	      (*absOffsetHistos)[i]=new TH2F((string("difference ")+string_number(detectors[i])).data(),
					     (string("\\Delta TOF calculated-measured: front-det")+
					      string_number(detectors[i])).data(),(detectors[i]<6?20:100),
					     thmin*0.,thmax*1.,100,tofmean-tofrange,tofmean+tofrange);
	    }
	  else
	    {
	      (*absOffsetHistos)[i]=new TH1F((string("difference ")+string_number(detectors[i])).data(),
					     (string("\\Delta TOF calculated-measured: front-det")+
					      string_number(detectors[i])).data(),100,tofmean-tofrange,tofmean+tofrange);
	    }
	}
      if((*absOffsetHistos)[i]!=NULL)(*absOffsetHistos)[i]->SetDirectory(0);
    }
  float tdcF[2],tdcB[2], /*tofT[2],*/ tdcSum;
  int nsum;
  vector3D pF[2],pB[2];
  float pb=setup.getBeamr().getParticle().Momentum().R(), mass=Eparticles::getMassG(14);//setup.getBeamr().getParticle().Mass();
  float c=299.792;

  float th1,th2,p1,p2;
  float bta[2];
  momentum4D pp;
  vector3D v1,v2;
  float lrp;

  float tof,tofCalc,ppath;
  sLine3D path[2];
  int cor=0;
  for(int i=0;i<elTree->GetEntries();i++)
    {
      elTree->GetEntry(i);
      while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
      for(int j=0;j<2;j++)
	{
	  v2.setValues(outtrack[j].vt[0],outtrack[j].vt[1],outtrack[j].vt[2]);
	  v1.setValues(cos(outtrack[j].phi)*sin(outtrack[j].theta),
		       sin(outtrack[j].phi)*sin(outtrack[j].theta),
		       cos(outtrack[j].theta));
	  path[j]=sLine3D(point3D(v2),v1);
	}
      v1=path[0].Direction();
      v2=path[1].Direction();

      th1=tan(v1.Theta());
      th2=tan(v2.Theta());
      p1=pb/(th1+th2)*th2*sqrt(th1*th1+1);
      p2=pb/(th1+th2)*th1*sqrt(th2*th2+1);
      bta[0]=p1/sqrt(p1*p1+mass*mass);
      bta[1]=p2/sqrt(p2*p2+mass*mass);

      for(int j=0;j<2;j++)
	{
	  nsum=0;tdcSum=0;pF[j].setValues(0,0,0);
	  for(int k=0;k<nCalibDets;k++)
	    {
	      if(isStop[k]){continue;}
	      if((outtrack[j].el[k]<0||outtrack[j].el[k]>=numElements[k]))continue;
	      nsum++;
	      tdcSum=tdcSum
		+getTDC(evtNr,k, outtrack[j].el[k], outtrack[j].tdcs[k], outtrack[j].qdcs[k], 0, cor/*offsets[cor],slopes[cor]*/, true, true, false);

	      pF[j]=pF[j]+getEntrance(setup.getDetectorr(detectors[k]).getShape(outtrack[j].el[k]),path[j]);
	    }
	  tdcF[j]=tdcSum/((double)nsum);
	  pF[j]=point3D( (1./((double)nsum)) * vector3D(pF[j]));

	  for(int k=0;k<nCalibDets;k++)
	    {
	      if(!isStop[k]){continue;}
	      if((outtrack[j].el[k]<0||outtrack[j].el[k]>=numElements[k]))continue;

	      pB[j]=getEntrance(setup.getDetectorr(detectors[k]).getShape(outtrack[j].el[k]),path[j]);
	      lrp=abs(getLightWay(setup.getDetectorr(detectors[k]).getShape(outtrack[j].el[k]),pB[j],false));

	      tdcB[j]=getTDC(evtNr,k, outtrack[j].el[k], 
			     outtrack[j].tdcs[k], outtrack[j].qdcs[k], lrp, 
			     cor/*offsets[cor], slopes[cor]*/,true, true, calibPos[2][k]>=0);

	      if(twoSided[k]>=0)
		{
		  if(outtrack[j].el[k]!=outtrack[j].el[twoSided[k]])continue;
		  tdcB[j]=(tdcB[j]+
			   getTDC(evtNr,twoSided[k], outtrack[j].el[twoSided[k]], 
				  outtrack[j].tdcs[twoSided[k]], outtrack[j].qdcs[twoSided[k]], 
				  lrp, cor/*offsets[cor],slopes[cor]*/, true, true, 
				  calibPos[2][k]>=0))*0.5;
		}
	      tof=tdcF[j]-tdcB[j];
	      ppath=(pF[j]-pB[j]).R();
	      tofCalc=ppath/c/bta[j];
	      if(calibPos[2][i]>=0&&(pattern<=1||(pattern<5&&twoSided[i]>=0)))
		(*absOffsetHistos)[k]->Fill(lrp,tof-tofCalc);
	      else 
		(*absOffsetHistos)[k]->Fill(tof-tofCalc);

	    }
	}
      for(int k=0;k<nCalibDets;k++)
	{
	  if(pixelBased[k]>=0)continue;
	  if(!isStop[k]/* &&abs(v1.Theta()-v2.Theta())<0.1*/)
	    (*absOffsetHistos)[k]->Fill(tdcF[0]-tdcF[1]);
	}
		   }
  // cout<<"done"<<endl;
}
void ATDCcalibration::fillOnlineOffsetHisto()
{
  TCalibHit *currentHit;
  int cor=0;
  while(cor<noffsetRanges&&offsetRanges[cor]<eventNumber)cor++;
  for(int i=0;i<numberOfTracks;i++)
    {
      for(int j=0;nCalibDets;j++)
	{
	  for(int k=0;k<tracks[i]->getNumberOfCalibHits(detectors[j]);k++)
	    {
	      currentHit=tracks[i]->getCalibSrcr(detectors[j],k);
	      offsetHistos2[cor][j][currentHit->getElement()]->Fill(currentHit->getTDC());
	    }
	}
    }
}
void ATDCcalibration::doCalibrationGeo()
{
  //cout<<"doCalibrationGeo()"<<endl;
  TCanvas *canvas=NULL;
  bool printMe=(printPattern &64);
  if(printMe)
    {
      canvas=new TCanvas();
      canvas->Divide(2,2);
      canvas->SetPad(0,0,1.1,1.2);
      canvas->cd(1);
    }
  string functionString[nCalibDets];
  float from[nCalibDets],to[nCalibDets];
  for(int j=0;j<nCalibDets;j++)
    {
      TProfile *prf=geoHistos[j]->ProfileX();
      point3D p;
      vector3D v;
      TF1 *f;
      if(setup.getDetectorr(detectors[j]).getShape(0)->getName()=="fiber")
	{
	  fiber *sh=((fiber*)setup.getDetectorr(detectors[j]).getShape(0));
	  functionString[j]=string("[0]+[1]*(x-")+string_number(sh->getHalvedAt())+")";
	  v=sh->getDirection(1);
	  TF1 func("func",functionString[j].data(),from[j],to[j]);
	  prf->Fit(&func,"QR0");
	  f=prf->GetFunction("func");
	  cout<<detectors[j]<<":"<<-f->GetParameter(0)*v<<endl;
	}
      else if(setup.getDetectorr(detectors[j]).getShape(0)->getName()=="wedge")
	{
	  wedge *sh=((wedge*)setup.getDetectorr(detectors[j]).getShape(0));
	  functionString[j]="[0]*sin([1]*(x-[2]))+[3]";
	  p=sh->getCenter();
	  TF1 func("func",functionString[j].data(),0,setup.getDetectorr(detectors[j]).getNumberOfElements());
	  prf->Fit(&func,"QR0");
	  f=prf->GetFunction("func");
	  cout<<detectors[j]<<":"<<f->GetParameter(0)<<"*sin("<<f->GetParameter(1)<<"*(x-"<<f->GetParameter(2)<<"))+"<<f->GetParameter(3)<<endl;
	}
      else //if(setup.getDetectorr(detectors[j]).getShape(0)->getName()=="ring")
	{
	  functionString[j]="[0]+[1]*x";
	  p=setup.getDetectorr(detectors[j]).getShape(0)->getCenter();
	  TF1 func("func",functionString[j].data(),from[j],to[j]);
	  prf->Fit(&func,"QR0");
	  f=prf->GetFunction("func");
	  cout<<detectors[j]<<":"<<f->GetParameter(0)<<"+x*"<<f->GetParameter(1)<<endl;
	}
      if(printMe)
	{
	  canvas->cd(1)->SetLogz(0);
	  geoHistos[j]->Draw();
	  canvas->cd(2)->SetLogz(0);
	  geoHistos[j]->Draw("col");
	  prf->Draw("same");
	  canvas->cd(3)->SetLogz(1);
	  geoHistos[j]->Draw();
	  canvas->cd(4)->SetLogz(1);
	  geoHistos[j]->Draw("col");
	  prf->Draw("same");
	  canvas->Update();
	  postscript->NewPage();
	}
      delete prf;
    }
  if(printMe)
    {
      delete canvas;
    }
}
void ATDCcalibration::doCalibrationBeam()
{
  //cout<<"doCalibrationBeam()"<<endl;
  int mini=0,minj=0;
  float minv=1000;
  TF1 fun("myGaus","gaus(0)+[3]+[4]*x",1.4,1.56);
  TCanvas *canvas=NULL;
  bool printMe=(printPattern &32);
  int nc=0;
  if(printMe)
    {
      canvas=new TCanvas();
      canvas->Divide(5,4);
      canvas->SetPad(0,0,1.1,1.2);
      canvas->cd(1);
    }
  for(int i=0;i<40;i++)
    for(int j=0;j<40;j++)
      {
	if(printMe)
	  canvas->cd(nc+1);
	beamHistos[i][j]->Fit(&fun,"QR0");
	if(beamHistos[i][j]->GetFunction(fun.GetName())->GetParameter(2)<minv)
	  {mini=i;minj=j;minv=beamHistos[i][j]->GetFunction(fun.GetName())->GetParameter(2);}
	if(printMe)
	  {
	    beamHistos[i][j]->Draw();
	    if(nc==19){nc=-1;canvas->Update();postscript->NewPage();}
	    nc++;
	  }
      }
  if(printMe)
    {
      if(nc>0){canvas->Update();postscript->NewPage();}
      delete canvas;
    }
  vector3D tmp((mini-19.5)*0.001,(minj-19.5)*0.001,1);tmp.normalize();
  cout<<"minimum found at beam direction "<<tmp.toString(_spheric_).data()<<endl;
  //cout<<"doCalibrationBeam() done"<<endl;
}
void ATDCcalibration::resetWalkHistos()
{
  for(int i=0;i<nCalibDets;i++)
    for(int j=0;j<numElements[i];j++)
      {
	walkHistos[i][j]->Reset();
	walkHistos[i][j+numElements[i]]->Reset();
	walkHistos[i][j+2*numElements[i]]->Reset();
	if(doLRT[i])walkHistos[i][j+3*numElements[i]]->Reset();
      }
}
void ATDCcalibration::doCalibrationWALK()
{
//   couta<<"doCalibrationWALK()"<<endl;
  if(trackTree!=NULL)cout<<"Track-Entries:"<<trackTree->GetEntries()<<flush;
  if(pixTree!=NULL)cout<<" Pix-Entries:"<<pixTree->GetEntries()<<flush;
  TCanvas *canvas=NULL;
  int nc=0;
  bool printIteration=(printPattern &4);
  bool printBefore=(printPattern &8);
  bool printAfter=(printPattern &16);
  bool printLastIteration=(printPattern &128);
  bool printMe=(printIteration||printBefore||printAfter||printLastIteration||((printPattern&1)>0));
  if(printMe)
    {
      canvas=new TCanvas();
      canvas->Divide(4,3);
      canvas->SetPad(0,0,1.1,1.2);
      canvas->cd(1);
      postscript->NewPage();
      gStyle->SetOptStat(11111);
    }
  resetWalkHistos();
//   printOffsetHistos(canvas,12, nc);
  if(printBefore)
    {printLine=false;
    resetWalkHistos();
    fillWalkHistos(0);
    printWalkHistos(canvas,12,nc,0,nCalibDets-1);}
  //redo histograms!
  //cout<<"redeclare walk histograms"<<endl;
  defineWalkStuff(false,true, true, 1, false, 0.667);
  //cout<<"calculate iteration modes"<<endl;
  bool hasSingle=false;
  for(int i=0;i<nCalibDets;i++)
    hasSingle=(single[i]||hasSingle);
  int nRR=1;
  int *beg;int *ennd;
  if(hasSingle)
    {
      for(int i=1;i<nCalibDets;i++)
	if(!single[i]&&single[i-1])nRR++;
      if(single[nCalibDets-1]&&!single[nCalibDets-2])nRR--;
      beg=new int[nRR];
      ennd=new int[nRR];
      nRR=0;
      int n=0;
      beg[0]=0;ennd[0]=0;
      while(n<nCalibDets)
	{
	  if(single[n])
	    {
	      if(beg[nRR]==n)
		{
		  beg[nRR]++;ennd[nRR]++;
		}
	      else if(ennd[nRR]==n)
		{
		  ennd[nRR]=ennd[nRR];
		  nRR++;
		  beg[nRR]=n+1;ennd[nRR]=n+1;
		}
	    }
	  else
	    ennd[nRR]++;
	  n++;
	}
      nRR++;
    }
  else 
    {
      beg=new int[1];
      beg[0]=0;
      ennd=new int[1];
      ennd[0]=nCalibDets;
    }
  //cout<<"begin iterations "<<numberOfIterations<<" "<<nRR<<endl;
  for(int rr=0;rr<nRR;rr++)
    {
      for(int r=0;r<numberOfIterations;r++)
	{
	  resetWalkHistos();
	  for(int i=beg[rr];i<ennd[rr];i++)
	    {
// 	      if(calibPos[0][i]>=0)
		for(int j=0;j<numElements[i];j++)
		  for(int k=0;k<walkFunctions[i][j]->GetNumberFreeParameters();k++)
		    walkFunctions[i][j]->SetParameter(k,outParser[i][calibPos[0][i]].getParameter(j,k));
	    }
	      
	  fillWalkHistos(1);
	  for(int j=beg[rr];j<ennd[rr];j++)
	    {
// 	      if(calibPos[0][j]>=0)
		for(int k=0;k<numElements[j];k++)
		  {
		    if(twoSided[j]>=0)
		      walkFitFunctions[j]->SetParameters(0,0,0,0,10);
		    fitTheHisto(((TH2*)walkHistos[j][k]), walkHistos[j][k+numElements[j]],
				walkFitFunctions[j],&outParser[j][calibPos[0][j]],k,/*fixAtMax=*/false,
				(twoSided[j]==measuresAgainst[j]?0:-1),"0");
		    for(int l=0;l<walkFunctions[j][k]->GetNumberFreeParameters();l++)
		      walkFunctions[j][k]->SetParameter(l,outParser[j][calibPos[0][j]].getParameter(k,l));
		  }
	    }
	  if(printIteration)
	    {printLine=false;
	    printWalkHistos(canvas,12,nc,beg[rr],ennd[rr]);}
	    
	}
    }
  //cout<<"do LRTs"<<endl;
  TF1 func("func","[0]+[1]*x+[2]*x*x+[3]*x*x*x");
  func.SetParameters(0,0,0,0);
  resetWalkHistos();
  fillWalkHistos(3);//, offsets,slopes);
  for(int i=0;i<nCalibDets;i++)
    {
      if(!doLRT[i])continue;
	for(int j=0;j<numElements[i];j++)
	  fitTheHisto(((TH2*)walkHistos[i][j+3*numElements[i]]), NULL,
		      &func,&outParser[i][calibPos[2][i]],j,/*fixAtMax=*/false,-1,"0");
    }
        
  //cout<<"do non-iterative walks"<<endl;
  resetWalkHistos();
  fillWalkHistos(1);//, offsets,slopes);
  for(int rr=0;rr<nRR+1;rr++)
    {
      for(int j=(rr==0?0:ennd[rr-1]);j<(rr==nRR?nCalibDets:beg[rr]);j++)
	{
// 	  if(calibPos[0][j]>=0)
	    for(int k=0;k<numElements[j];k++)
	      {
		fitTheHisto(((TH2*)walkHistos[j][k]), walkHistos[j][k+numElements[j]],
			    walkFitFunctions[j],&outParser[j][calibPos[0][j]],k,/*fixAtMax=*/false,-1,"0");
		for(int l=0;l<walkFunctions[j][k]->GetNumberFreeParameters();l++)
		  walkFunctions[j][k]->SetParameter(l,outParser[j][calibPos[0][j]].getParameter(k,l));
	      }
	}
      if(printIteration)
	{printLine=false;
	printWalkHistos(canvas,12,nc,(rr==0?0:ennd[rr-1]),(rr==nRR?nCalibDets:beg[rr]));
	}
    }
  //cout<<"improved lrt"<<endl;
  defineWalkStuff(false,false, true,8+64,false, 1/9./6.);
  resetWalkHistos();
  for(int i=0;i<nCalibDets;i++)
    if(calibPos[2][i]>=0)
      if(doLRT[i])
	for(int j=0;j<numElements[i];j++)
	  for(int k=0;k<4;k++)
	    outParser[i][calibPos[2][i]].setParameter(j,k,0);
  TF1*tmpf1;
  for(int r=0;r<numberOfIterations;r++)
    {
      resetWalkHistos();
      fillWalkHistos(3);//, offsets,slopes);
      for(int i=0;i<nCalibDets;i++)
	{
	  if(!doLRT[i])continue;
	  if(calibPos[2][i]>=0)
	    for(int j=0;j<numElements[i];j++)
	      {
		TH1* tmp=((TH2*)walkHistos[i][j+3*numElements[i]])->ProfileX();
		// 	      fitTheHisto(((TH2*)walkHistos[i][j+3*numElements[i]]), NULL,
		// 			  &func,&outParser[i][doOffset?2:1],j,/*fixAtMax=*/false,-1,"");
		tmp->Fit(&func,"Q");
		tmpf1=tmp->GetFunction(func.GetName());
		if(tmpf1!=NULL)
		  {
		    for(int k=0;k<4;k++)
		      outParser[i][calibPos[2][i]].setParameter(j,k,outParser[i][calibPos[2][i]].getParameter(j,k)+tmpf1->GetParameter(k));
		  }
		delete tmp;
	      }
	}
    }
  if(printLastIteration)
    {
//   cout<<"final printing"<<endl;
      printLine=false;
      resetWalkHistos();
      fillWalkHistos(1);//, offsets,slopes);
      printWalkHistos(canvas,12,nc,0,nCalibDets);
      TH1* tmpH[12];
      for(int j=0;j<nCalibDets;j++)
	{
	  if(doLRT[j]&&calibPos[2][j]>=0)
	    for(int k=0;k<numElements[j];k++)
	      {
		canvas->cd(nc+1);
		tmpH[nc]=((TH2*)walkHistos[j][k+3*numElements[j]])->ProfileX();
		for(int l=0;l<4;l++)
		  func.SetParameter(l,outParser[j][calibPos[2][j]].getParameter(k,l));
		walkHistos[j][k+3*numElements[j]]->Draw("col");
		func.Draw("same");
		tmpH[nc]->Draw("same");
		nextPad(12,postscript,canvas,nc,((TObject**)tmpH));
	      }
	}
    }
  if(printAfter)
    {
      resetWalkHistos();
      fillWalkHistos(7);
      printLine=true;
      printWalkHistos(canvas,12,nc,0,nCalibDets);
      TH1* tmpH[12];
      TLine line;line.SetLineWidth(2);
      for(int j=0;j<nCalibDets;j++)
	{
	  if(doLRT[j])
	    for(int k=0;k<numElements[j];k++)
	    {
	      canvas->cd(nc+1);
	      tmpH[nc]=((TH2*)walkHistos[j][k+3*numElements[j]])->ProfileX();
	      walkHistos[j][k+3*numElements[j]]->Draw("col");
	      tmpH[nc]->Draw("same");
	      line.DrawLine(walkHistos[j][k+3*numElements[j]]->GetXaxis()->GetXmin(),0,walkHistos[j][k+3*numElements[j]]->GetXaxis()->GetXmax(),0);
	      nextPad(12,postscript,canvas,nc,((TObject**)tmpH));
	    }
	}
     }
//   cout<<"resolution printing"<<endl;
  if(printMe)
    {
      if(!printAfter){resetWalkHistos();fillWalkHistos(7);}
      if(nc>0){canvas->Update();postscript->NewPage();}
      for(int j=0;j<nCalibDets;j++)
	{
	  for(int k=0;k<2;k++)
	    {
	      canvas->cd(k*4+(j%4)+1);
	      if(resolutionHisto[j*2+k]->IsA()==TH2F::Class())
		resolutionHisto[j*2+k]->Draw("col");
	    }
	  canvas->cd(2*4+(j%4)+1);
	  resolutionHisto1[j]->Fit("gaus","Q");
	  gStyle->SetOptFit(111);
	  if((j%4)==3){canvas->Update();postscript->NewPage();}//	  nextPad(12,postscript,canvas,nc,NULL);
	}
      if((nCalibDets%4)>0){canvas->Update();postscript->NewPage();}
      delete canvas;
    }
}
void ATDCcalibration::doCalibrationOffset()
{
//   cout<<"doCalibrationOffset()"<<endl;
  if(!doOffset)return;
  TCanvas *canvas=NULL;
  int nc=0;
  bool printAbsolute=(printPattern &1);
  bool printIndividual=(printPattern &2);
  bool printMe=printAbsolute||printIndividual;
  if(printMe)
    {
      canvas=new TCanvas();
      canvas->SetPad(0,0,1.1,1.2);
      if(printIndividual)
	canvas->Divide(4,3);
      else
	canvas->Divide(2,2);
      canvas->cd(1);
      if(printPattern<=3)
	postscript->NewPage();
    }
  TH1 **absOffsetHisto=new TH1*[nCalibDets];

  if(!doWalk)
    {
      TF1 line("line","-([0]+[1]*x*1e-6)");
      for(int i=0;i<noffsetRanges;i++)
	for(int j=0;j<nCalibDets;j++)
	  for(int k=0;k<numElements[j];k++)
	    {
	      getProfileFit(offsetHistos2[i][j][k],(isStop[j]?"landau":"gaus"), 1, 
			    line, offsets[i][j][k], slopes[i][j][k],0,1);
	    }    
      fillOffsetHistos(/*offsets,slopes,*/&absOffsetHisto,1);
      if(printIndividual)
	printOffsetHistos(canvas,12, nc);
      if(printAbsolute)
	{
	  if(printIndividual)
	    {
	      if(nc>0){canvas->Update();postscript->NewPage();}
	      canvas->Clear();
	      canvas->Divide(1,1);
	    }
	  canvas->cd(1);
	  for(int j=0;j<nCalibDets;j++)
	    {absOffsetHisto[j]->Draw();canvas->Update();;postscript->NewPage();}
	}
      for(int i=0;i<noffsetRanges;i++)
	for(int j=0;j<nCalibDets;j++)
	  for(int k=0;k<numElements[j];k++)
	    {
	      offsetParser[j][i].setParameter(j,0,offsets[i][j][k]-500);
	      offsetParser[j][i].setParameter(j,1,slopes[i][j][k]);
	    }
      if(printMe)
	delete canvas;
      for(int i=0;i<nCalibDets;i++)if(absOffsetHisto[i]!=NULL)delete absOffsetHisto[i];
      delete []absOffsetHisto;
      return;  
    }
  //with walk
  resetWalkHistos();
  for(int j=0;j<nCalibDets;j++)
    for(int k=0;k<numElements[j];k++)
      for(int i=0;i<noffsetRanges;i++)
	{
	  offsetHistos2[i][j][k]->Reset();
	  offsets[i][j][k]=0;
	  slopes[i][j][k]=0;
	}
  calculateOffsets(7);
  if(printIndividual)
    printOffsetHistos(canvas,12, nc);
  fillOffsetHistos(&absOffsetHisto,0);
  if(printAbsolute)
    {
      if(printIndividual)
	if(nc>0){canvas->Update();postscript->NewPage();}
      canvas->Clear();
      canvas->Divide(2,2);	
      canvas->cd(1);
    }
  nc=0;
  TH1*tmph[4]={NULL,NULL,NULL,NULL};
  TF1 func("func","[0]+[1]*x+[2]*x*x+[3]*x*x*x");
  bool redo=true;
  float mean[nCalibDets];
  for(int ii=0;ii<nCalibDets;ii++)
    if(detectors[ii]<6)mean[ii]=12.5;
    else mean[ii]=10;
  string option="Q0+";
  if(printAbsolute)option="Q";
  for(int ii=0;ii<6;ii++)
    {
      if(redo)
	{
	  redo=false;
	  fillOffsetHistos(&absOffsetHisto,ii);
	  for(int j=0;j<nCalibDets;j++)
	    {
	      if(printAbsolute)
		canvas->cd(nc+1);
	      if(absOffsetHisto[j]->IsA()==TH2F::Class())
		{
		  if(absOffsetHisto[j]->GetEntries()>0)
		    {
		      if(printAbsolute)
			absOffsetHisto[j]->Draw("col");
		      tmph[nc]=((TH2*)absOffsetHisto[j])->ProfileX();
		      tmph[nc]->Fit(&func,option.data(),"same");
		      if(calibPos[2][j]>=0)
			{
// 			  for(int k=0;k<numElements[j];k++)
// 			    for(int l=0;l<4;l++)
// 			      outParser[j][calibPos[2][j]].setParameter(k,l,outParser[j][calibPos[2][j]].getParameter(k,l)
// 									-(tmph[nc]->GetFunction("func")->GetParameter(l)+(l==0?mean[j]:0)));
 			  redo=true;
			}
		      if(printAbsolute)
			{
			  //  nextPad(4,postscript,canvas,nc,((TObject**)NULL));
			}
		      else delete tmph[nc];tmph[nc]=NULL;
		    }
		}
	      else if(ii==5&&printAbsolute)
		{
		  canvas->cd(nc+1);
		  if(absOffsetHisto[j]->IsA()==TH1F::Class()&&absOffsetHisto[j]->GetEntries()>10)
		    absOffsetHisto[j]->Fit("gaus","q");
		  nextPad(4,postscript,canvas,nc,((TObject**)NULL));
		}
	    }
	}
    }
  if(printAbsolute)
    {
      if(nc>0){canvas->Update();postscript->NewPage();}
      nc=0;
    }
  for(int i=0;i<noffsetRanges;i++)
    for(int j=0;j<nCalibDets;j++)
      for(int k=0;k<numElements[j];k++)
	{
	  offsetParser[j][i].setParameter(k,0,offsets[i][j][k]-500);
	  offsetParser[j][i].setParameter(k,1,slopes[i][j][k]);
	}
  float add[nCalibDets];
  for(int j=0;j<nCalibDets;j++)
    {
      add[j]=0;
    }
  for(int j=0;j<nCalibDets;j++)
    {
      if(calibPos[1][j]>=0)
	if(absOffsetHisto[j]->IsA()==TH1F::Class()&&absOffsetHisto[j]->GetEntries()>10)
	  {
	    absOffsetHisto[j]->Fit("gaus","q");
	    TF1 *fun=absOffsetHisto[j]->GetFunction("gaus");
	    if(fun!=NULL)
	      {
		for(int k=0;k<numElements[j];k++)
		  for(int i=0;i<noffsetRanges;i++)
		    offsetParser[j][i].setParameter(k,0,offsetParser[j][i].getParameter(k,0)-(fun->GetParameter(1)+add[j]));
	      }
	  }
    }
  //////////
  for(int j=0;j<nCalibDets;j++)
    {
      if(referencesTo[j]<0||calibPos[1][j]<0)continue;
      if(absOffsetHisto[j]->IsA()==TH1F::Class()&&absOffsetHisto[j]->GetEntries()<10)
	{
	  absOffsetHisto[referencesTo[j]]->Fit("gaus","q");
	  TF1 *fun=absOffsetHisto[referencesTo[j]]->GetFunction("gaus");
	  if(fun!=NULL)
	    {
	      for(int k=0;k<numElements[j];k++)
		for(int i=0;i<noffsetRanges;i++)
		  offsetParser[j][i].setParameter(k,0,offsetParser[j][i].getParameter(k,0)-(fun->GetParameter(1)+add[j]));
	    }
	}
    }
  //////////
  if(printMe)
    delete canvas;
  for(int i=0;i<nCalibDets;i++)if(absOffsetHisto[i]!=NULL)delete absOffsetHisto[i];
  delete []absOffsetHisto;
  //cout<<"doCalibrationOffset() done"<<endl;
}
void ATDCcalibration::fillOnlineTwoSidedHisto()
{
  if((doWalk||doOffset)||!doTwoSided)return;
  TCalibHit *h1,*h2;
  float zfront[nTwoSided];
  for(int i=0;i<nTwoSided;i++)
    {
      wedge* sh=((wedge*)setup.getDetectorr(detectors[twoDets[i]]).getShape(0));
      zfront[i]=sh->getCenter().Z()-sqrt(sh->getOuterRadius()*sh->getOuterRadius()-sh->getDistToBarrelCenter()*sh->getDistToBarrelCenter());
    }

  for(int i=0;i<numberOfTracks;i++)
    {
      for(int j=0;j<nTwoSided;j++)
	{
	  for(int k=0;k<tracks[i]->getNumberOfCalibHits(detectors[twoDets[j]]);k++)
	    {
	      h1=tracks[i]->getCalibSrcr(detectors[twoDets[j]],k);
	      if(h1==NULL)continue;
	      if(!h1->isValid())continue;
	      if(h1->getElement()>0||h1->getElement()>=numElements[twoDets[j]])continue;
	      h2=NULL;
	      for(int l=0;l<tracks[i]->getNumberOfCalibHits(detectors[twoSided[twoDets[j]]]);l++)
		if(tracks[i]->getCalibSrcr(detectors[twoSided[twoDets[j]]],l)!=NULL)
		  if(tracks[i]->getCalibSrcr(detectors[twoSided[twoDets[j]]],l)->getElement()==h1->getElement())
		    h2=tracks[i]->getCalibSrcr(detectors[twoSided[twoDets[j]]],l);
	      if(h2==NULL)continue;
	      if(!h2->isValid())continue;

	      twoSidedHisto[j][h1->getElement()]->Fill((h2->getTDC()-h1->getTDC())/2,(getPoint(h1->getHitShape(),tracks[i]->getPath()).Z()-zfront[j])/10);
	    }
	}
    }
}
void ATDCcalibration::fillTwoSidedHisto(int ptt)//, float ***offs,float ***slop)
{
  if(!(doWalk||doOffset))return;
  int el;
  float tdc1, tdc2;
  float zfront[nTwoSided];
  for(int i=0;i<nTwoSided;i++)
    {
      wedge* sh=((wedge*)setup.getDetectorr(detectors[twoDets[i]]).getShape(0));
      zfront[i]=sh->getCenter().Z()-sqrt(sh->getOuterRadius()*sh->getOuterRadius()-sh->getDistToBarrelCenter()*sh->getDistToBarrelCenter());
    }
  for(int i=0;i<nTwoSided;i++)
    for(int j=0;j<numElements[twoDets[i]];j++)
      twoSidedHisto[i][j]->Reset();
  switch(ptt)
    {
    case 2:
      {
	int cor=0;
	for(int k=0;k<elTree->GetEntries();k++)
	  {
	    elTree->GetEntry(k);
	    while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
	    for(int j=0;j<2;j++)
	      for(int i=0;i<nTwoSided;i++)
		{
		  el=outtrack[j].el[twoDets[i]];
		  if(el<0||el!=outtrack[j].el[twoSided[twoDets[i]]]||el>=numElements[twoDets[i]])continue;
		  tdc1=getTDC(evtNr,twoDets[i], el, outtrack[j].tdcs[twoDets[i]], 
			      outtrack[j].qdcs[twoDets[i]], 0, cor/*offsets[cor],slopes[cor]*/, true, true, false);
		  tdc2=getTDC(evtNr,twoSided[twoDets[i]], el, outtrack[j].tdcs[twoSided[twoDets[i]]], 
			      outtrack[j].qdcs[twoSided[twoDets[i]]], 0, cor/*offsets[cor],slopes[cor]*/, true, true, false);
		  twoSidedHisto[i][el]->Fill((tdc2-tdc1)/2,(outtrack[j].pointz[twoDets[i]]-zfront[i])/10);
		}
	  }
	break;
      }
     case 0:
      {
	int num;
	int cor=0;
	for(int k=0;k<trackTree->GetEntries();k++)
	  {
	    trackTree->GetEntry(k);
	    while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
	    for(int i=0;i<nTwoSided;i++)
	      {
		if(!(trackEls[0][outtrack_el.det]==twoDets[i]||trackEls[1][outtrack_el.det]==twoDets[i]))continue;
		if(trackEls[0][outtrack_el.det]==twoDets[i])num=0;else num=1;
		el=outtrack_el.el[num];
		if(el<0||el!=outtrack_el.el[1-num]||el>=numElements[trackEls[num][outtrack_el.det]])continue;
		tdc1=getTDC(evtNr,twoDets[i], el, outtrack_el.tdcs[num], outtrack_el.qdcs[num], 0, cor/*offsets[cor],slopes[cor]*/, !true, true, false);
		tdc2=getTDC(evtNr,twoSided[twoDets[i]], el, outtrack_el.tdcs[1-num], outtrack_el.qdcs[1-num], 
			    0, cor/*offsets[cor],slopes[cor]*/, !true, true, false);
		twoSidedHisto[i][el]->Fill((tdc2-tdc1)/2,(outtrack_el.pointz[num]-zfront[i])/10);
	      }
	  }
	break;
      }
    case 1:
      {
	float theta;
	int cor=0;
	for(int k=0;k<pixTree->GetEntries();k++)
	  {
	    pixTree->GetEntry(k);
	    while(evtNr>offsetRanges[cor+1]&&cor<noffsetRanges)cor++;
	    theta=vector3D(outpix.pointx,outpix.pointy,outpix.pointz).theta();
	    for(int i=0;i<nTwoSided;i++)
	      {
		el=outpix.el[pixelDets[twoDets[i]]];
		if(el<0||el!=outpix.el[pixelDets[twoSided[twoDets[i]]]]||el>=numElements[twoDets[i]])continue;
		tdc1=getTDC(evtNr,twoDets[i], el, outpix.tdcs[pixelDets[twoDets[i]]], outpix.qdcs[pixelDets[twoDets[i]]],
			    0,cor/* offsets[cor],slopes[cor]*/, true, true, false);
		tdc2=getTDC(evtNr,twoSided[twoDets[i]], el, outpix.tdcs[pixelDets[twoSided[twoDets[i]]]], 
			    outpix.qdcs[pixelDets[twoSided[twoDets[i]]]], 0, cor/*offsets[cor],slopes[cor]*/, true, true, false);
		twoSidedHisto[i][el]->Fill((tdc2-tdc1)/2,(outpix.pointz-zfront[i])/10);
	      }
	  }
	break;
      }
   }
}
void ATDCcalibration::doCalibrationTwoSided()
{
  //get histos filled
  if(doWalk)
    {
      bool haspixbase=false;
      bool hastrackbase=false;
      for(int i=0;i<nCalibDets;i++)
	{
	  if(twoSided[i]<0)continue;
	  if(pixelBased[i]>=0)haspixbase=true;
	  else hastrackbase=true;
	}
      if(hastrackbase)
	fillTwoSidedHisto(0);
      if(haspixbase)
	fillTwoSidedHisto(1);
    }
  else
    {
    }
  //evaluate histos
  bool printMe=((printPattern&256)>0);
  TCanvas *canvas=NULL;int nc=0;
  canvas=new TCanvas();
  canvas->Divide(1,1);
  if(printMe)
    {
      canvas->Clear();
      canvas->Divide(4,3);
      canvas->SetPad(0,0,1.1,1.2);
      gStyle->SetOptStat(11111);
      //postscript->NewPage();
    }
  canvas->cd(1);
  TH1* tmp[12];
  TF1 line("line","([1]+[0]*x)");
  float mmin,mmax;
  TF1 *ff;
  for(int i=0;i<nTwoSided;i++)
    {
      if(calibPos[3][twoDets[i]]>=0)
	for(int j=0;j<numElements[twoDets[i]];j++)
	  {
	    if(printMe)canvas->cd(nc+1);
	    if(printMe)
	      twoSidedHisto[i][j]->Draw("col");
	    for(int ll=0;ll<3;ll++)
	      {
		if(ll>0)delete tmp[nc];
		switch(ll)
		  {
		  case 2:tmp[nc]=getMaxHisto((TH2*)twoSidedHisto[i][j]);break;
		  case 1:tmp[nc]=((TH2*)twoSidedHisto[i][j])->ProfileX();break;
		  case 0:tmp[nc]=getProfile((TH2*) twoSidedHisto[i][j],"gaus",1);break;
		  }
		mmin=twoSidedHisto[i][j]->GetMean()-twoSidedHisto[i][j]->GetRMS()*2;
		mmax=twoSidedHisto[i][j]->GetMean()+twoSidedHisto[i][j]->GetRMS()*2;
		line.SetParameters(14,140);
		tmp[nc]->Fit(&line,(printMe?"Q":"Q0"),(printMe?"same":""),mmin,mmax);
		ff=tmp[nc]->GetFunction("line");
		if(ff->GetChisquare()/ff->GetNDF()>15)
		  {
		    for(int l=0;l<10;l++)
		      {
			mmin=twoSidedHisto[i][j]->GetMean()-twoSidedHisto[i][j]->GetRMS()*(2.-0.11*l);
			mmax=twoSidedHisto[i][j]->GetMean()+twoSidedHisto[i][j]->GetRMS()*(2.-0.11*l);
			line.SetParameters(14,140);
			tmp[nc]->Fit(&line,(printMe?"Q":"Q0"),(printMe?"same":""),mmin,mmax);
			ff=tmp[nc]->GetFunction("line");
			if(ff->GetChisquare()/ff->GetNDF()<15)break;
		      }
		    if(ff->GetChisquare()/ff->GetNDF()>15)
		      {
			for(int l=0;l<10;l++)
			  {
			    mmin=twoSidedHisto[i][j]->GetMean()-twoSidedHisto[i][j]->GetRMS()*(2.-0.11*l);
			    mmax=twoSidedHisto[i][j]->GetMean()+twoSidedHisto[i][j]->GetRMS()*(2.-0.11*l);
			    line.SetParameters(14,140);
			    tmp[nc]->Fit(&line,(printMe?"lQ":"lQ0"),(printMe?"same":""),mmin,mmax);
			    ff=tmp[nc]->GetFunction("line");
			    if(ff->GetChisquare()/ff->GetNDF()<15)break;
			  }
		      }
		  }
		if(ff->GetChisquare()/ff->GetNDF()<15)break;
	      }
	    outParser[twoDets[i]][calibPos[3][twoDets[i]]].setParameter(j,0,ff->GetParameter(0));
	    outParser[twoDets[i]][calibPos[3][twoDets[i]]].setParameter(j,1,ff->GetParameter(1));
	    if(printMe)nextPad(12,postscript,canvas,nc,((TObject**)tmp));
	    else delete tmp[nc];
	  }
    }
  for(int i=0;i<nTwoSided;i++)
    for(int j=0;j<numElements[twoDets[i]];j++)
      twoSidedHisto[i][j]->Reset();
  delete canvas;
}

void ATDCcalibration::calculateOffsets(int ptt)
{
  if(!doWalk)return;
  fillWalkHistos(ptt);
  TF1 line("line","-([0]+[1]*x*1e-6)");
  for(int i=0;i<noffsetRanges;i++)
    for(int j=0;j<nCalibDets;j++)
      {
	for(int k=0;k<numElements[j];k++)
	  {
	    line.SetParameters(offsetHistos2[i][j][k]->GetBinCenter(offsetHistos2[i][j][k]->GetNbinsX())-offsetHistos2[i][j][k]->GetBinCenter(1),0);
	    getProfileFit(offsetHistos2[i][j][k],(isStop[j]?"landau":"gaus"),1, 
			  line, offsets[i][j][k], slopes[i][j][k],0,1);
	  }
      }
}
void ATDCcalibration::calculateLines()//float ***offs,float ***slop)
{
  if(!doWalk)return;
  for(int j=0;j<nCalibDets;j++)
    for(int k=0;k<numElements[j];k++)
      walkHistos[j][k]->Reset();
  fillWalkHistos(0);//, offs,slop);
  for(int j=0;j<nCalibDets;j++)
    {
      if(twoSided[j]>=0)
	{
	  for(int k=0;k<numElements[j];k++)
	    fitTheHisto(((TH2*)walkHistos[j][k]),NULL,walkLines[j][k],NULL,k,true,-1,"W");
	}
    }
}
TH1* getMaxHisto(TH2* hist)
{
  if(hist==NULL)return NULL;
  TH1* histo1=new TH1F((string(hist->GetName())+"_Max").data(),hist->GetTitle(),hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
  int maxBin;
  float sum,summe;
  float err;
  int nEntries;
  float v,all=hist->GetEntries();
  bool is2s=hasA(string(hist->GetName()),"pos vs \\DeltaTDC");
  TH1* tmpH=hist->ProfileX();
  for(int i=1;i<hist->GetNbinsX()+1;i++)
    {
      summe=hist->GetBinContent(i,1);
      maxBin=1;
      nEntries=(int)hist->GetBinContent(i,maxBin);
      sum=hist->GetBinContent(i,maxBin)/hist->GetBinError(i,maxBin)/hist->GetBinError(i,maxBin);
      err=1./hist->GetBinError(i,maxBin)/hist->GetBinError(i,maxBin);
      for(int j=2;j<hist->GetNbinsY()+1;j++)
	{
	  summe+=hist->GetBinContent(i,j);
	  nEntries+=(int)hist->GetBinContent(i,j);
	  sum+=hist->GetBinContent(i,j)/hist->GetBinError(i,j)/hist->GetBinError(i,j);
	  err+=1/hist->GetBinError(i,j)/hist->GetBinError(i,j);
	  if(hist->GetBinContent(i,maxBin)<hist->GetBinContent(i,j))
	    maxBin=j;
	}
      v=hist->GetBinContent(i,maxBin);
      histo1->SetBinContent(i,hist->GetYaxis()->GetBinCenter(maxBin));
      histo1->SetBinError(i,(nEntries>20?abs(v)/sqrt(v*nEntries*(nEntries/all)):50));
      if(summe<20&&is2s)
	{
	  histo1->SetBinContent(i,tmpH->GetBinContent(i));
	  histo1->SetBinError(i,tmpH->GetBinError(i));
	}
    }
  delete tmpH;
  return histo1;
}
void ATDCcalibration::getProfileFit(TH2*histo,const string &functionName, int parnum, 
				    TF1& function, float& value, float &value2,
				    int parnum1, int parnum2)
{
  TH1* tmpH;
//   if(histo->GetEntries()/histo->GetNbinsX()<histo->GetNbinsY()*1.5)
//     {
//       tmpH=histo->ProjectionY();
// //       tmpH->Fit(functionName.data(),"Q0+");
// //       value=tmpH->GetFunction(functionName.data())->GetParameter(parnum);
//       value=tmpH->GetMean();//histo->GetBinCenter(histo->GetNbinsX()/2);
//       value2=0;
//     }
//   else
    {
      tmpH=getProfile(histo,functionName,parnum);
      tmpH->Fit(&function,"Q0+");
      TF1 *ff=tmpH->GetFunction(function.GetName());
//       cout<<histo->GetName()<<" "<<ff->GetChisquare()/ff->GetNDF()<<endl;
      if(ff->GetChisquare()/ff->GetNDF()>10)
	tmpH->Fit(&function,"Q0+","",
		  tmpH->GetBinCenter((int)(tmpH->GetNbinsX()*0.1)+1),
		  tmpH->GetBinCenter((int)(tmpH->GetNbinsX()*0.9)+1));
      ff=tmpH->GetFunction(function.GetName());
      value=ff->GetParameter(parnum1);
      value2=ff->GetParameter(parnum2);
    }
  delete tmpH;
}
TH1* ATDCcalibration::getProfile(TH2* histo,string functionName,int parnum)
{
  if(histo==NULL)return NULL;
  if(functionName=="")return histo->ProfileX();
  TF1 function("f1",functionName.data());
  if(function.GetNumberFreeParameters()<=parnum)parnum=function.GetNumberFreeParameters()-1;
//   if(histo->GetEntries()/histo->GetNbinsX()<histo->GetNbinsY()*1.5)
//     {
//       return histo->ProjectionY();
//     }
  TH1* tmph=NULL;
  bool is2s=hasA(string(histo->GetName()),"pos vs \\DeltaTDC");
  if(is2s)
    {
      if(histo->GetEntries()<6*histo->GetNbinsX())
	return getMaxHisto(histo);
      tmph=getMaxHisto(histo);
    }
  TH1F *ret=new TH1F((string(histo->GetName())+"_PFx").data(),histo->GetTitle(),histo->GetNbinsX(),histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax());
  TH1F tmp("tmpHisto","tmpHisto",histo->GetNbinsY(),histo->GetYaxis()->GetXmin(),histo->GetYaxis()->GetXmax());
  float ne;
  for(int i=1;i<histo->GetNbinsX()+1;i++)
    {
      tmp.Reset();ne=0;
      for(int j=1;j<histo->GetNbinsY()+1;j++)
	{
	  tmp.SetBinContent(j,histo->GetBinContent(i,j));
	  ne+=histo->GetBinContent(i,j);
	}
      if(ne>20)
	{
	  tmp.Fit(&function,"Q0");
	  ret->SetBinContent(i,tmp.GetFunction(function.GetName())->GetParameter(parnum));
	  ret->SetBinError(i,tmp.GetFunction(function.GetName())->GetParError(parnum));
	}
      else
	{
	  ret->SetBinContent(i,tmp.GetBinCenter(tmp.GetNbinsX()/2));//tmp.GetFunction(function.GetName())->GetParameter(parnum));
	  ret->SetBinError(i,(tmp.GetBinCenter(1)+tmp.GetBinCenter(tmp.GetNbinsX()))/2.);//tmp.GetFunction(function.GetName())->GetParError(parnum));
	  if(tmph!=NULL)
	    {
	      ret->SetBinContent(i,tmph->GetBinContent(i));
	      ret->SetBinError(i,tmph->GetBinError(i));
	    }
	}
    }
  if(tmph!=NULL)delete tmph;
  return ret;
}
void ATDCcalibration::fitTheHisto(TH2* h2d, TH1* h1d,TF1 *fitfunc,CommonCalibrationParser *output,int element,bool fixAtMax,int ptt,string opt)
{
  TH1*tmp=(ptt==-1?((TH2*)h2d)->ProfileX():getProfile(h2d,"gaus",1));//(ptt>0?project2DHistogram((TH2*) h2d,ptt):((TH2*)h2d)->ProfileX());
  float ffmin=h2d->GetXaxis()->GetXmin();// *1.1;
  float ffmax=h2d->GetXaxis()->GetXmax();// *0.9;
  if(ptt==0)ffmin=ffmin;
  if(h1d!=NULL)
    {
      float rmsfac=1;
      if(output!=NULL)
	{
	  if(output->getParamID()==6)rmsfac=3.;
	  else if((output->getDetector()==6||output->getDetector()==7))rmsfac=0.1;
	}
      ffmin=h1d->GetBinCenter(h1d->GetMaximumBin())-h1d->GetRMS()*rmsfac;//(/*(j<2||j>3)?*/160/*:h1d->GetRMS()*/);
      ffmax=h1d->GetBinCenter(h1d->GetMaximumBin())+h1d->GetRMS()*3;//(j<2?3:(j>3?3.:1.5));
      if(output!=NULL)
	{
	  if(output->getDetector()==6||output->getDetector()==7)
	    {
	      int mb=h1d->FindBin(ffmin);
	      int xb=h1d->FindBin(ffmax);
	      while(h1d->GetBinContent(mb)<100&&mb<xb)mb++;
	      ffmin=h1d->GetBinCenter(mb);
	    }
	}
    }
  if(fixAtMax)
    {
      int bx,by,bz;
      ((TH2*)h2d)->GetMaximumBin( bx, by, bz);
      float ax=h2d->GetXaxis()->GetBinCenter(bx);
      float ay=h2d->GetYaxis()->GetBinCenter(by);
      ffmin=ax-1;
      fitfunc->SetParLimits(0,ax-1,ax+1);
      fitfunc->SetParLimits(2,ay-.1,ay+.1);
    }
  TF1*func=NULL;
  if(tmp->IsA()==TGraphErrors::Class())
    {
//       ((TGraph*)tmp)->SetMarkerSize(MARKERSIZE);
//       ((TGraph*)tmp)->SetMarkerStyle(MARKERSTYLE);
//       ((TGraph*)tmp)->Draw("sameP");
      ((TGraph*)tmp)->Fit(fitfunc,(opt+"Q0").data(),"same",ffmin,ffmax);
      func=((TGraph*)tmp)->GetFunction(fitfunc->GetName());
      if(func->GetChisquare()/func->GetNDF()>100)
	{
	  ((TGraph*)tmp)->Fit(fitfunc,(opt+"Qw0").data(),"same",ffmin,ffmax);
	  func=((TGraph*)tmp)->GetFunction(fitfunc->GetName());
	}
      if(func->GetChisquare()/func->GetNDF()>100)
	{
	  ((TGraph*)tmp)->Fit(fitfunc,(opt+"Ql0").data(),"same",ffmin,ffmax);
	  func=((TGraph*)tmp)->GetFunction(fitfunc->GetName());
	}
    }
  else
    {
      ((TH1*)tmp)->Fit(fitfunc,(opt+"Q").data(),"same",ffmin,ffmax);
      func=((TH1*)tmp)->GetFunction(fitfunc->GetName());
//       if(func->GetChisquare()/func->GetNDF()>100)
// 	{
// 	  ((TH1*)tmp)->Fit(fitfunc,(opt+"Qw").data(),"same",ffmin,ffmax);
// 	  func=((TH1*)tmp)->GetFunction(fitfunc->GetName());
// 	}
      if(func->GetChisquare()/func->GetNDF()>100)
	{
	  ((TH1*)tmp)->Fit(fitfunc,(opt+"Ql").data(),"same",ffmin,ffmax);
	  func=((TH1*)tmp)->GetFunction(fitfunc->GetName());
	}
      if(func->GetChisquare()/func->GetNDF()>100)
	{
	  delete tmp;
	  return;
	}
 //      ((TH1*)tmp)->SetMarkerStyle(MARKERSTYLE);
//       ((TH1*)tmp)->SetMarkerSize(MARKERSIZE);
    }
//   cout<<func->GetNumberFreeParameters()<<endl;
  if(output!=NULL)
    for(int l=0;l<func->GetNumberFreeParameters();l++)
      {
	output->setParameter(element,l,func->GetParameter(l));
	fitfunc->SetParameter(l,func->GetParameter(l));
      }
  else
    {
      for(int l=0;l<func->GetNumberFreeParameters();l++)
	fitfunc->SetParameter(l,func->GetParameter(l));
    }
  delete tmp;
}
void nextPad(int nPads,TPostScript *ps,TCanvas *canv, int &nc, TObject **pointers)
{
  nc++;
  if(nc==nPads)
    {
      nc=0;
      canv->Update();
      ps->NewPage();
      if(pointers!=NULL)
	{
	  for(int ii=0;ii<nPads;ii++)
	    {
	      if(pointers[ii]!=NULL)delete pointers[ii];pointers[ii]=NULL;
	    }
	}
    }
}
void ATDCcalibration::printWalkHistos(TCanvas* canv,int npads, int &ncanv, int begin, int end)
{
//   cout<<"print walk Histos "<<begin<<"->"<<end<<flush;
  gStyle->SetOptFit(111);
  if(begin<0)begin=0;
  if(end>nCalibDets)end=nCalibDets;
  TH1*obj[npads];
  TLine lin;
  lin.SetLineColor(1);lin.SetLineWidth(2);
  for(int j=begin;j<end;j++)
    {//if(detectors[j]!=6)continue;
      for(int k=0;k<numElements[j];k++)
	{
	  canv->cd(ncanv+1);
	  walkHistos[j][k]->Draw("col");
	  obj[ncanv]=((TH2*)walkHistos[j][k])->ProfileX();
	  obj[ncanv]->Draw("same");
	  if(printLine)
	    lin.DrawLine(walkHistos[j][k]->GetXaxis()->GetXmin(),0,walkHistos[j][k]->GetXaxis()->GetXmax(),0);
	  else
	    {
	      walkFunctions[j][k]->Draw("same");
	    }
	  nextPad(npads,postscript,canv,ncanv,((TObject**)obj));
	}
    }
//   cout<<" done"<<endl;
}
void ATDCcalibration::printOffsetHistos(TCanvas* canv,int npads, int &ncanv)
{
  gStyle->SetOptFit(111);
  TH1*obj[npads];
  TF1 *line[npads];
  for(int i=0;i<npads;i++)
    line[i]=new TF1((string("line")+string_number(i)).data(),"-([0]+[1]*x*1e-6)",0.,2.4e06);
  for(int j=0;j<nCalibDets;j++)
    {
      for(int k=0;k<numElements[j];k++)
	{
	  if(doWalk)
	    {
	      for(int i=0;i<noffsetRanges;i++)
		{
		  canv->cd(ncanv+1);
		  obj[ncanv]=getProfile(offsetHistos2[i][j][k],(isStop[j]?"landau":"gaus"),1);
		  line[ncanv]->SetParameters(offsets[i][j][k],slopes[i][j][k]);

		  offsetHistos2[i][j][k]->Draw("col");
		  obj[ncanv]->Draw("same");
		  line[ncanv]->Draw("same");
		  nextPad(npads,postscript,canv,ncanv,((TObject**)obj));
		}
	    }
	  else
	    {
	      offsetHistos[j][k]->Draw();
	      if(offsetHistos[j][k]->GetListOfFunctions()->GetEntries()>0)
		((TF1*)offsetHistos[j][k]->GetListOfFunctions()->Last())->Draw("same");
	      nextPad(npads,postscript,canv,ncanv,((TObject**)obj));
	    }
	}
    }
  for(int i=0;i<npads;i++)
    delete line[i];
}

float ATDCcalibration::calculateWALK(int detectorNumber,int ElementNumber, float qdc)
{
  if(calibPos[0][detectorNumber]<0)return 0;
  return (outParser[detectorNumber][calibPos[0][detectorNumber]].getParameter(ElementNumber,0)/
    (qdc-outParser[detectorNumber][calibPos[0][detectorNumber]].getParameter(ElementNumber,1))
    +outParser[detectorNumber][calibPos[0][detectorNumber]].getParameter(ElementNumber,2)
    *log(abs(qdc-outParser[detectorNumber][calibPos[0][detectorNumber]].getParameter(ElementNumber,3)))
    +outParser[detectorNumber][calibPos[0][detectorNumber]].getParameter(ElementNumber,4));
  // [0]/(x-[1])+[2]*log(abs(x-[3]))+[4]  [0]/(x-[1])+[2]*log(abs(x-[3]))+[4] [0]/(x-[1])+[2]*log(abs(x-[3]))+[4]
}
float ATDCcalibration::calculateLRT(int detectorNumber,int ElementNumber, float lrp)
{
  if(calibPos[2][detectorNumber]<0/*||!(doLRT[detectorNumber]||twoSided[detectorNumber]>=0)*/)
    {cout<<"wrong detector for LRT:"<<detectors[detectorNumber]<<endl;return 0;}
  return outParser[detectorNumber][calibPos[2][detectorNumber]].getParameter(ElementNumber,0)
    +outParser[detectorNumber][calibPos[2][detectorNumber]].getParameter(ElementNumber,1)*lrp
    +outParser[detectorNumber][calibPos[2][detectorNumber]].getParameter(ElementNumber,2)*lrp*lrp
    +outParser[detectorNumber][calibPos[2][detectorNumber]].getParameter(ElementNumber,3)*lrp*lrp*lrp;
  //    +outParser[detectorNumber][pos].getParameter(ElementNumber,4)*lrp*lrp*lrp*lrp;
}
float ATDCcalibration::getTDC(int evtnr, int detectorNumber, int elementNumber, float tdc, float qdc, float path,int corr, bool hwalk, bool hoff, bool hlrt)
{
  float ret=tdc;
  if(hoff)ret=ret-
	    (offsets[corr][detectorNumber][elementNumber]
	     +slopes[corr][detectorNumber][elementNumber]*evtNr*1e-6);
  if(hlrt&&calibPos[2][detectorNumber]>=0&&path>0)ret=ret-calculateLRT(detectorNumber,elementNumber, path);
  if(hwalk)ret=ret-calculateWALK(detectorNumber,elementNumber,qdc);
  return ret;
}
point3D ATDCcalibration::getPoint(volumeShape *sh, const sLine3D &path)
{
  return getEntrance(sh,path);
  if(sh==NULL)return point3D();
  point3D p;
  point3D cc=sh->getCenter();
  vector3D nor(0,0,-1);
  if(sh->getName()=="wedge")
    nor=((wedge*)sh)->getNormal();
  else if(sh->getName()=="spiral")
    nor=((spiral*)sh)->getNormal();
  else if(sh->getName()=="ring")
    nor=((ring*)sh)->getNormal();
  else if(sh->getName()=="fiber")
    nor=((fiber*)sh)->getDirection(2);
  plane3D pl(cc,nor);//((wedge*)sh)->getNormal());
  p=pl-path;
  if(sh->getName()=="wedge")
    {
      if(((wedge*)sh)->getDistToBarrelCenter()>0)
	{
	  wedge* w=(wedge*)sh;
	  float alpha=asin(w->getDistToBarrelCenter()/w->getOuterRadius());
	  float s=(w->getCenter().Z()*sin(path.Direction().Theta())/
		   sin(M_PI-alpha-path.Direction().Theta()));
	  p=w->getCenter()-vector3D(0,0,1)*
	    sqrt(s*s-w->getDistToBarrelCenter()*w->getDistToBarrelCenter());

	  //	  cout<<p<<"\t"<<w->getCenter()<<"\t"<<alpha<<"\t"<<path.Direction().Theta()<<"\t"<<cos(M_PI-alpha-path.Direction().Theta())<<" "<<cos(path.Direction().Theta())<<" "<<w->getOuterRadius()<<" "<<w->getCenter().Z()-sqrt(w->getOuterRadius()*w->getOuterRadius()-w->getDistToBarrelCenter()*w->getDistToBarrelCenter())<<endl;
// 	  p=path.Foot()+path.Direction()*
// 	    (sh->getCenter().Z()*cos(alpha)/
// 	     cos(M_PI-alpha-path.Direction().Theta()));// (entrance(path);
	}
  }
  return p;
}
void ATDCcalibration::writeCalibration(CommonCalibrationParser *pa, ostream &out)
{
  if(pa==NULL)return;
  if(pa->getDetector()<0||pa->getParamID()<0)return;
//   cout<<"writeCalibration() "<<pa->getDetector()<<" "<<pa->getParamID()<<endl;
  string detname="", paramname=string_number(pa->getParamID());
  switch(pa->getDetector())
    {
    case 0:detname="QUIRL_GERADE";break;
    case 1:detname="QUIRL_LINKS";break;
    case 2:detname="QUIRL_RECHTS";break;
    case 3:detname="RING_GERADE";break;
    case 4:detname="RING_LINKS";break;
    case 5:detname="RING_RECHTS";break;
    case 6:detname="BARREL_VORNE";break;
    case 7:detname="BARREL_HINTEN";break;
    case 15:detname="TORTE_A";break;
    case 16:detname="TORTE_B";break;
    case 17:detname="HODO_X";break;
    case 18:detname="HODO_Y";break;
    case 19:detname="HODO_U";break;
    case 20:detname="HODO_V";break;
    case 21:detname="MICRO_R";break;
    case 22:detname="MICRO_PHI";break;
    case 23:detname="HODO_D";break;
    case 25:detname="CALORIMETER";break;
    }
  switch(pa->getParamID())
    {
    case 0:paramname="PEDESTAL";break;
    case 1:paramname="TdcFactor";break;
    case 2:paramname="WALK";break;
    case 3:paramname="CUTS";break;
    case 4:paramname="QdcFactor";break;
    case 5:paramname="TdcOffset";break;
    case 6:paramname="zBarrel";break;
    case 7:paramname="tdcRadialPix";break;
    case 8:paramname="tdcRadialPol";break;
    case 9:paramname="tdcRadialSpPix";break;
    case 10:paramname="tdcRadialSpPol";break;
    case 11:paramname="qdcRadialPix";break;
    case 12:paramname="qdcRadialPol";break;
    case 13:paramname="tdcBarrelAdd";break;
    }
  out<<"#======================new calibration type=========================="<<endl;
  out<<""<<endl;
  out<<"==>               "<<detname.data()<<" "<<pa->getDetector()<<" "<<pa->getNumElements()<<"  "<<paramname.data()<<"  "<<pa->getParamID()<<"  "<<pa->getNumParameters()<<endl;
  out<<""<<endl;
  out<<"#---------------------------------------------------"<<endl;
  out<<"author            "<<author.data()<<endl;
  out<<"date              after 27th of January 2005 "<<endl;
  out<<"fromRun           "<<pa->getRange(0)<<endl;
  out<<"uptoRun           "<<pa->getRange(1)<<endl;
  if(eventBased||(((pa->getDetector()>14&&pa->getParamID()==3)||pa->getParamID()==5)&&noffsetRanges>1))
    {
      out<<"fromEvent         "<<pa->getRange(2)<<endl;
      out<<"uptoEvent         "<<pa->getRange(3)<<endl;
    }
  out<<"beamTime          "<<beamtime.data()<<endl;
  out<<"gotFromFile       newCalDat.data"<<endl;
  out<<"#----------------------------------------------------"<<endl;
  for(int i=0;i<pa->getNumElements();i++)
    {
      out.width(3);
      out<<pa->getDetector();
      out.width(4);
      out<<i<<"\t";
      for(int j=0;j<pa->getNumParameters();j++)
	{
	  out.width(10);
	  out<<pa->getParameter(i,j)<<" ";
	}
      out<<endl;
    }
  out<<endl;
}

