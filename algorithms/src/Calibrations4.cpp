#include "Calibrations.h"
#include "reactions.h"
AqdcFactorCalibration::AqdcFactorCalibration(TEvent &eventIn, TSetup &setup, algorithm_parameter & descr):AAlgorithm("Generate qdcFactor calibration"), event(eventIn), minEntriesPerHisto(descr.getParam<int>(0).getData())
{
  vector<int> tmp(descr.getParam<vector<int> >(0).getData());
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  qdcHistograms=new TH1**[numberOfDetectors];
  string nn;
  int xmin=0,xmax=10000;
  if(descr.getNumberOfParam<int>()>1)xmin=descr.getParam<int>(1).getData();
  if(descr.getNumberOfParam<int>()>2)xmax=descr.getParam<int>(2).getData();
  eventStart=0;
  for(int i=0;i<numberOfDetectors;i++)
    {
      detectorIDs[i]=tmp[i];
      if(setup.getNumberOfDetectors()>detectorIDs[i])
	numberOfElements[i]=setup.getDetectorr(i).getNumberOfElements();
      else
	numberOfElements[i]=0;
      qdcHistograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("qdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  qdcHistograms[i][j]=new TH1F(nn.data(),nn.data(),1000,xmin,xmax);
	}
    }
}
AqdcFactorCalibration::AqdcFactorCalibration(TEvent &eventIn, TSetup &setup, vector<int> &detectors, int minEntriesPerHistoIn):AAlgorithm("Generate qdcFactor calibration"), event(eventIn), minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  qdcHistograms=new TH1**[numberOfDetectors];
  string nn;
  int xmin=0,xmax=10000;
  eventStart=0;
  for(int i=0;i<numberOfDetectors;i++)
    {
      detectorIDs[i]=tmp[i];
      if(setup.getNumberOfDetectors()>detectorIDs[i])
	numberOfElements[i]=setup.getDetectorr(i).getNumberOfElements();
      else
	numberOfElements[i]=0;
      qdcHistograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("qdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  qdcHistograms[i][j]=new TH1F(nn.data(),nn.data(),1000,xmin,xmax);
	}
    }
}
AqdcFactorCalibration::~AqdcFactorCalibration()
{
  for(int i=0;i<numberOfDetectors;i++)
    {
      for(int j=0;j<numberOfElements[i];j++)
	delete qdcHistograms[i][j];
      delete []qdcHistograms[i];
    }
  delete []qdcHistograms;
  delete []detectorIDs;
  delete []numberOfElements;
}
void* AqdcFactorCalibration::process(void *ptr)
{
  if(event.getNumberOfReactions()==0)return ptr;
  //check for reactions:
  int found=-1;
//   for(int i=0;i<numberOfReactions;i++)
//     if(reactions[i]->IsA()==event.getReaction(0)->IsA())
//       found=i;
  if(found<0)return ptr;
  RbaseReaction *react=event.getReaction(0);
  int pos,num;
  TCalibHit* tmp;
  for(int i=0;i<react->getNumberOfTracks();i++)
    {
      for(int j=0;j<react->getTrack(i)->getNumberOfCalibHits(-2);j++)
	for(int k=0;k<react->getTrack(i)->getNumberOfCalibHits(j);k++)
	  {
	    pos=-1;
	    tmp=react->getTrack(i)->getCalibSrcr(j,k);
	    for(int l=0;l<numberOfDetectors;l++)
	      if(tmp->getDetector()==detectorIDs[l])
		pos=l;
	    if(pos<0)continue;
	    num=tmp->getElement();
	    qdcHistograms[pos][num]->Fill(tmp->getRawADC());//perhaps minus value of found reaction??
	  }
    }
  return ptr;
}
void AqdcFactorCalibration::toEvaluate()
{
  for(int i=0;i<numberOfDetectors;i++)
    emit evaluated(evaluate(i));
  eventStart=event.getEventNumber();
}
CommonCalibrationParser* AqdcFactorCalibration::evaluate(int num)
{
  CommonCalibrationParser *ret=new CommonCalibrationParser(numberOfElements[num],detectorIDs[num]);
  ret->setNumParamsElements(1,numberOfElements[num]);
  ret->setParamID(4);
  ret->setValid();
  ret->setRange(0,event.getRunNumber());
  ret->setRange(1,event.getRunNumber());
  ret->setRange(2,eventStart);
  ret->setRange(3,event.getEventNumber());
  float v=0;
  for(int i=0;i<numberOfElements[num];i++)
    {
      //insert calibration routine
      /*********************************

      **********************************/
      ret->setParameter(i,0,v);
    }
  return ret;
}

