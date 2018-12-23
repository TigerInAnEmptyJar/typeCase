#include "Calibrations.h"

AWalkCalibration::AWalkCalibration(TEvent &eventIn, TSetup &setup, algorithm_parameter & descr):AAlgorithm("Generate walk calibration"), event(eventIn), minEntriesPerHisto(descr.getParam<int>(0).getData())
{
  vector<int> tmp(descr.getParam<vector<int> >(0).getData());
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  histograms=new TH1**[numberOfDetectors];
  string nn;
  int xmin=0,xmax=10000;
  if(descr.getNumberOfParam<int>()>1)xmin=descr.getParam<int>(1).getData();
  if(descr.getNumberOfParam<int>()>2)xmax=descr.getParam<int>(2).getData();
  int ymin=0,ymax=10000;
  if(descr.getNumberOfParam<int>()>3)xmin=descr.getParam<int>(3).getData();
  if(descr.getNumberOfParam<int>()>4)xmax=descr.getParam<int>(4).getData();
  eventStart=0;
  for(int i=0;i<numberOfDetectors;i++)
    {
      detectorIDs[i]=tmp[i];
      if(setup.getNumberOfDetectors()>detectorIDs[i])
	numberOfElements[i]=setup.getDetectorr(i).getNumberOfElements();
      else
	numberOfElements[i]=0;
      histograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("tdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  histograms[i][j]=new TH2F(nn.data(),nn.data(),100,xmin,xmax,100,ymin,ymax);
	}
    }
}
AWalkCalibration::AWalkCalibration(TEvent &eventIn, TSetup &setup, vector<int> &detectors, int minEntriesPerHistoIn):AAlgorithm("Generate tdcFactor calibration"), event(eventIn), minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  histograms=new TH1**[numberOfDetectors];
  string nn;
  int xmin=0,xmax=10000;
  int ymin=0,ymax=10000;
  eventStart=0;
  for(int i=0;i<numberOfDetectors;i++)
    {
      detectorIDs[i]=tmp[i];
      if(setup.getNumberOfDetectors()>detectorIDs[i])
	numberOfElements[i]=setup.getDetectorr(i).getNumberOfElements();
      else
	numberOfElements[i]=0;
      histograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("tdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  histograms[i][j]=new TH2F(nn.data(),nn.data(),100,xmin,xmax,100,ymin,ymax);
	}
    }
}

AWalkCalibration::~AWalkCalibration()
{
  for(int i=0;i<numberOfDetectors;i++)
    {
      for(int j=0;j<numberOfElements[i];j++)
	delete histograms[i][j];
      delete []histograms[i];
    }
  delete []histograms;
  delete []detectorIDs;
  delete []numberOfElements;
}
void* AWalkCalibration::process(void *ptr)
{
  int n;
  TCalibHit *th;
  for(int i=0;i<numberOfDetectors;i++)
    {
      for(int j=0;j<event.getNumberOfHits(detectorIDs[i]);j++)
	{
	  th=&event.getHit(detectorIDs[i],j);
	  n=th->getElement();
	  histograms[i][n]->Fill(th->getRawADC(),th->getRawTDC());
	}
    }
  return ptr;
}
void AWalkCalibration::toEvaluate()
{
  for(int i=0;i<numberOfDetectors;i++)
    emit evaluated(evaluate(i));
  eventStart=event.getEventNumber();
}
CommonCalibrationParser* AWalkCalibration::evaluate(int num)
{
  CommonCalibrationParser *ret=new CommonCalibrationParser(numberOfElements[num],detectorIDs[num]);
  ret->setNumParamsElements(6,numberOfElements[num]);
  ret->setParamID(2);
  ret->setValid();
  ret->setRange(0,event.getRunNumber());
  ret->setRange(1,event.getRunNumber());
  ret->setRange(2,eventStart);
  ret->setRange(3,event.getEventNumber());
  float v[6]={0,0,0,0,0,0};
  for(int i=0;i<numberOfElements[num];i++)
    {
      //insert calibration routine
      /*********************************

      **********************************/
      for(int j=0;j<6;j++)
	ret->setParameter(i,j,v[j]);
    }
  return ret;
}

