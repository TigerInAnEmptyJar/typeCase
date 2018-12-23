#include "ACalibration.h"
//#include <fstream>
#include <vector>
#include "logger.h"
#include "beamtimeparameter.h"
ACalibration::ACalibration(TCalibHit **hitsIn, int& numberOfHitsIn, const TDetector &det,const vector<string> &filenames, int &runNrIn, int &evtNrIn, bool cA, bool cT):AAlgorithm("Calibration"), numberOfHits(numberOfHitsIn),DetID(det.getID()),correctADC(cA),correctTDC(cT),runNumber(runNrIn),eventNumber(evtNrIn)
{
  numberOfElements=det.getNumberOfElements();
  hits=hitsIn;
  numberOfRanges=new int[6];
  actualRange=new int[6];
  for(int i=0;i<6;i++)numberOfRanges[i]=0;
  parameter=NULL;
  vector<string>tmp;
  for(unsigned int i=0;i<filenames.size();i++)
    tmp.push_back(filenames[i]);
  //  getFromFiles(tmp);
}

ACalibration::~ACalibration()
{
  for(int i=0;i<6;i++)
    {
      for(int j=0;j<numberOfRanges[i];j++)
	delete parameter[i][j];
      delete []parameter[i];
    }
  delete []parameter;
  delete []actualRange;
  delete []numberOfRanges;
}
#include<stdlib.h>
void *ACalibration::process(void*ptr)
{
  if(!checkRange())return ptr;
  int numElem;
  int numDet;
  int rawADC,rawTDC;
  int nv=0;
  float walk,cADC,cTDC;
  for(int i=0;i<numberOfHits;i++)
    {
      numElem=hits[i]->getElement();
      numDet=hits[i]->getDetector();
      if(numElem>=numberOfElements||numElem<0)
	{
	  hits[i]->setValid(false);
	  hits[i]->setADC(-1);
	  hits[i]->setTDC(-1);
	  continue;
	}
      rawADC=hits[i]->getRawADC();
      rawTDC=hits[i]->getRawTDC(); 
      //appy TDC and QDC -cuts
      if(numberOfRanges[3]>0)
	{
	  if((((rawADC>=parameter[3][actualRange[3]]->getParameter(numElem,0))&&
	       (rawADC<=parameter[3][actualRange[3]]->getParameter(numElem,1)))||(!correctADC)))
	    //&&
// 	     (((rawTDC>=parameter[3][actualRange[3]]->getParameter(numElem,2)))))//&&
	    //	       (rawTDC<=parameter[3][actualRange[3]]->getParameter(numElem,3)))||(!correctTDC)))
	    {
	      hits[i]->setValid();
	    }
	  else
	    {
	      hits[i]->setValid(false);
	      hits[i]->setADC(-1);
	      hits[i]->setTDC(-1);
	      //	      continue;
	    }
	}
      else
	hits[i]->setValid();
      if(!(hits[i]->isValid())) continue;
      /**********************************************
       *   found wrong formulas in description of
       *   calibration format. now taking formulas 
       *   derived from the Juelich analysis software 
       *
       *   since calibration didn't work properly
       *   I use my own...
       *
       ***********************************************/
      //calculate WALK correction
      if((numberOfRanges[2]>0)&&(numberOfRanges[0]>0)&&(rawTDC!=-1))
	{//"[0]/(x-[1])+[2]*log(|x-[3]|)+[4]" x=rADC-adcOffs
	  walk=parameter[2][actualRange[2]]->getParameter(numElem,0)/
	    (rawADC-parameter[0][actualRange[0]]->getParameter(numElem,0)
	     -parameter[2][actualRange[2]]->getParameter(numElem,1))
	    +parameter[2][actualRange[2]]->getParameter(numElem,2)*
	    log(abs(rawADC-parameter[0][actualRange[0]]->getParameter(numElem,0)
		    -parameter[2][actualRange[2]]->getParameter(numElem,3)))
	    +parameter[2][actualRange[2]]->getParameter(numElem,4);
	}
      else
	walk=0;

      cTDC=rawTDC;//+rand()/RAND_MAX;
      if(correctTDC)
	{
	  if(numberOfRanges[1]>0)
	    cTDC=cTDC*parameter[1][actualRange[1]]->getParameter(numElem,0);
	  if(numberOfRanges[2]>0)
	    cTDC=cTDC-walk;
	  if(numberOfRanges[5]>0)
	    cTDC=cTDC-(parameter[5][actualRange[5]]->getParameter(numElem,0)
		       +parameter[5][actualRange[5]]->getParameter(numElem,1)*eventNumber/1e6);
	  if(!((numberOfRanges[1]>0)&&(rawTDC!=-1)))
	    cTDC=-1;
	  if(numberOfRanges[3]>0)
	    if((cTDC<parameter[3][actualRange[3]]->getParameter(numElem,2))||
	       (cTDC>parameter[3][actualRange[3]]->getParameter(numElem,3)))
	      {
		hits[i]->setADC(-1);
		hits[i]->setTDC(-1);
		hits[i]->setValid(false);
		continue;
	      }
	}
      //apply QDC correction
      cADC=rawADC;
      if(correctADC)
	{
	  if(numberOfRanges[0]>0)
	    cADC=cADC-parameter[0][actualRange[0]]->getParameter(numElem,0);
	  if(numberOfRanges[4]>0)
	    cADC=cADC/parameter[4][actualRange[4]]->getParameter(numElem,0);
	  if(!(numberOfRanges[0]>0))
	    cADC=-1;
	}
      hits[i]->setValid();
      if(correctADC)hits[i]->setADC(cADC);
      if(correctTDC)hits[i]->setTDC(cTDC);
      nv++;
    }
  return ptr;
}
bool ACalibration::checkRange()
{
  bool rangeFound=false;
  for(int i=0;i<6;i++)
    {
      if(actualRange[i]<0||numberOfRanges[i]==0)continue;
      if((!correctADC)&&(i==0))continue;
      if((!correctTDC)&&(i==1||i==2||i==5))continue;
      if(!((runNumber>=parameter[i][actualRange[i]]->getRange(0))&&
	   (runNumber<=parameter[i][actualRange[i]]->getRange(1))&&
	   (eventNumber<=parameter[i][actualRange[i]]->getRange(3))&&
	   (eventNumber>=parameter[i][actualRange[i]]->getRange(2))))
	{
	  rangeFound=false;
	  for(int j=0;j<numberOfRanges[i];j++)
	    {
	      if((runNumber>=parameter[i][j]->getRange(0))&&
		 (runNumber<=parameter[i][j]->getRange(1))&&
		 (eventNumber<=parameter[i][j]->getRange(3))&&
		 (eventNumber>=parameter[i][j]->getRange(2)))
		{
		  actualRange[i]=j;
		  rangeFound=true;
		}
	    }
	  if((!rangeFound)&&(numberOfRanges[i]>0))
	    {
	      for(int k=0;k<numberOfHits;k++)
		{
		  hits[k]->setValid(false);
		  hits[k]->setADC(-1);
		  hits[k]->setTDC(-1);
		}
	      return false;
	    }
	}
    }
  return true;
}

void ACalibration::getNewRun(run_parameter &r)//, beamTime_parameter &b)
{
  anaLog<<"ACalibration::getNewRun()"<<endli;
  vector<string> filenames;
  if(r.hasOwnCalibration()&&!r.hasAdditionalCalibration())
    {
      for(int iii=0;iii<r.getNumberOfCalibrationFiles();iii++)
	filenames.push_back(r.getCalibrationFile(iii));
    }
  else if(r.hasAdditionalCalibration())
    {
      for(int iii=0;iii<r.getParent()->getNumberOfCalibrationFiles();iii++)
	filenames.push_back(r.getParent()->getCalibrationFile(iii));
      for(int iii=0;iii<r.getNumberOfCalibrationFiles();iii++)
	filenames.push_back(r.getCalibrationFile(iii));
    }
  else
    {
      for(int iii=0;iii<r.getParent()->getNumberOfCalibrationFiles();iii++)
	filenames.push_back(r.getParent()->getCalibrationFile(iii));
    }
  getFromFiles(filenames);
}
void ACalibration::getFromFiles(const vector<string> &calibrationFiles)
{
  if(parameter!=NULL)
    {
      for(int i=0;i<6;i++)
	{
	  for(int j=0;j<numberOfRanges[i];j++)
	    delete parameter[i][j];
	  delete []parameter[i];
	}
    }
  else
    parameter=new CommonCalibrationParser**[6];
  CommonCalibrationParser *tmpParser;
  vector<CommonCalibrationParser*>tmp,tmp1;
  for(int i=0;i<6;i++)numberOfRanges[i]=0;
  for(unsigned int i=0;i<calibrationFiles.size();i++)
    {
      CommonCalibrationParser::getFileContent(calibrationFiles[i],tmp);//<<" "<<calibrationFiles[i].data()<<endl;
    }
  while(!tmp.empty())
    {
      tmpParser=tmp.back();
      tmp.pop_back();
      if(tmpParser->getDetector()==DetID&&tmpParser->getParamID()<6&&tmpParser->getParamID()>=0)
	{
	  numberOfRanges[tmpParser->getParamID()]++;
	  tmp1.push_back(tmpParser);
	}
      else
	delete tmpParser;
	
    }
  for(int i=0;i<6;i++)
    {
      parameter[i]=new CommonCalibrationParser*[numberOfRanges[i]];
      actualRange[i]=(numberOfRanges[i]==0?-1:0);
      numberOfRanges[i]=0;
    }
  while(!tmp1.empty())
    {
      tmpParser=tmp1.back();
      parameter[tmpParser->getParamID()][numberOfRanges[tmpParser->getParamID()]]=tmpParser;
      numberOfRanges[tmpParser->getParamID()]++;
      tmp1.pop_back();
    }
  for(int i=0;i<6;i++)
    {
      if(!(correctADC&&correctTDC))
      {
	if(!correctADC&&(i==0||i==4))continue;
	if(!correctTDC&&(i==2||i==5||i==1))continue;
      }
      anaLog<<"detID-"<<DetID<<" PID-"<<i<<" #-ranges:"<<numberOfRanges[i];
      for(int k=0;k<numberOfRanges[i];k++)
	anaLog<<" from run "<<parameter[i][k]->getRange(0)<<" to "<<parameter[i][k]->getRange(1)<<endli;
      if(numberOfRanges[i]==0)anaLog<<endli;
    }
  anaLog<<endli;
}
