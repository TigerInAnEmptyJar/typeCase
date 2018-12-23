#include "Calibrations.h"
AtdcOffsetCalibration::AtdcOffsetCalibration(TEvent &eventIn, TSetup &setup, algorithm_parameter & descr):AAlgorithm("Generate tdcOffset calibration"), event(eventIn), minEntriesPerHisto(descr.getParam<int>(0).getData())
{
  vector<int> tmp(descr.getParam<vector<int> >(0).getData());
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  tdcHistograms=new TH1**[numberOfDetectors];
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
      tdcHistograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("tdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  tdcHistograms[i][j]=new TH1F(nn.data(),nn.data(),1000,xmin,xmax);
	}
    }
}

AtdcOffsetCalibration::AtdcOffsetCalibration(TEvent &eventIn, TSetup &setup, vector<int> &detectors, int minEntriesPerHistoIn):AAlgorithm("Generate tdcOffset calibration"), event(eventIn), minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  numberOfDetectors=tmp.size();
  detectorIDs=new int[numberOfDetectors];
  numberOfElements=new int[numberOfDetectors];
  tdcHistograms=new TH1**[numberOfDetectors];
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
      tdcHistograms[i]=new TH1*[numberOfElements[i]];
      for(int j=0;j<numberOfElements[i];j++)
	{
	  nn=(string("tdc: detID-")+string_number(detectorIDs[i])+" el-"+string_number(j)).data();
	  tdcHistograms[i][j]=new TH1F(nn.data(),nn.data(),1000,xmin,xmax);
	}
    }
}
AtdcOffsetCalibration::~AtdcOffsetCalibration()
{
  for(int i=0;i<numberOfDetectors;i++)
    {
      for(int j=0;j<numberOfElements[i];j++)
	delete tdcHistograms[i][j];
      delete []tdcHistograms[i];
    }
  delete []tdcHistograms;
  delete []detectorIDs;
  delete []numberOfElements;
}
void* AtdcOffsetCalibration::process(void *ptr)
{
  TPixel *pix;
  int pos,num;
  TCalibHit* tmp;
  float pixelMeanTime;
  for(int i=0;i<event.getMaxNumber<TDetector>();i++)
    {
      for(int j=0;j<event.getNumberOfPixels(i);j++)
	{
	  pix=&event.getPixel(i,j);
	  pixelMeanTime=0;
	  for(int k=0;k<pix->getNumberOfActualElements();k++)
	    pixelMeanTime+=pix->getElementr(k).getRawTDC();
	  pixelMeanTime=pixelMeanTime/pix->getNumberOfActualElements();
	  for(int k=0;k<pix->getNumberOfActualElements();k++)
	    {
	      pos=-1;
	      tmp=&pix->getElementr(k);
	      for(int l=0;l<numberOfDetectors;l++)
		if(tmp->getDetector()==detectorIDs[l])
		  pos=l;
	      if(pos<0)continue;
	      num=tmp->getElement();
	      tdcHistograms[pos][num]->Fill(tmp->getRawTDC()-pixelMeanTime);
	    }
	  }
    }
  return ptr;
}

void AtdcOffsetCalibration::toEvaluate()
{
  for(int i=0;i<numberOfDetectors;i++)
    emit evaluated(evaluate(i));
  eventStart=event.getEventNumber();
}
CommonCalibrationParser* AtdcOffsetCalibration::evaluate(int num)
{
  CommonCalibrationParser *ret=new CommonCalibrationParser(numberOfElements[num],detectorIDs[num]);
  ret->setNumParamsElements(1,numberOfElements[num]);
  ret->setParamID(5);
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
