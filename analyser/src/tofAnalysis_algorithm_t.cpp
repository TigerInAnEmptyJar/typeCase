#include "algorithms.h"
#include "AReactionRecognition.h"
#include "logger.h"
#include <QtCore/QDateTime>
#include "tofAnalysis_t.h"
#include "shapes.h" 

pthread_mutex_t initer=PTHREAD_MUTEX_INITIALIZER; 

int tofAnalysis::getAlgorithm(const algorithm_parameter &param,const run_parameter &firstRun, int position, int positionfit, int &nFits)
{
  AAlgorithm **tmp;
  int n;
  int u;
  int sum=0;
  //  int tmpInt;
  for(int ii=0;ii<nThreads;ii++)
    {
      n=tofAnalysis::getAlgorithm(&(tmp),u,param,firstRun, Event[ii],Setup, NumberOfHits[ii], &Event[ii].getNumberOfTracks(), NumberOfPixels[ii], NumberOfClusters[ii], NumberOfHitClusters[ii], Raws[ii], CalibratedHits[ii], HitClusters[ii], Tracks[ii],Pixels[ii],Clusters[ii],NULL/* TMaterial **materials*/,Detectors, readInValid,readInID, this,callEventSearch);
      sum=u;
      nFits=(n-u);
      for(int i=0;i<sum;i++)
	f[ii][position+i]=tmp[i];
      for(int i=u;i<nFits;i++)
	fits[ii][positionfit+i-u]=(AFitAlgorithm*)tmp[i];
      delete []tmp;
    }
  return sum;
}
void tofAnalysis::initAlgorithms(const vector<algorithm_parameter> &p, const run_parameter & runParam)
{
  QTime timer;
  timer.start();
  if(!isInitD)return;
  anaLog<<"init algorithms:"<<incD<<endli;
  vector<algorithm_parameter> ap;
   for(unsigned int i=0;i<p.size();i++)
     {
       algorithm_parameter aptemp(p[i]);
       ap.push_back(aptemp);
     }
  callEventSearch=false;
  vector<string> tmp;
  vector<int> tmi;
  int numf=0;
  int numFit=0;
  //  int numMatF=0;
  anaLog<<"count algorithms: ";
  emit initStateChanged("count algorithms");
  for(unsigned int i=0;i<ap.size();i++) 
    {
      if(!ap[i].IsUsed())continue;
      switch(ap[i].getID())
	{
	case 16: numf++; break;
	case 0: numf=numf+Event[0].getNumberOfHits();break;//pedestal
	case 1: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//tdc off1
	case 2: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//tdcoff2
	case 3: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//tdcfac1
	case 4: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//tdcfac2
	case 5: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//adcoff1
	case 6: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//adcoff2
	case 7: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//adcfac1
	case 8: numf=numf+Event[0].getNumberOfHits()
		  +NumberOfDetectors;break;//adcfac2
	case 9: numf=numf+1;break;//readFromTade
	case 10: numf++;break; //fillRawTDCHistograms
	case 11: numf++;break; //fillRawADCHistograms
	case 12: numf++;break; //fillDetectorHistograms
	case 13: numf++;break; //fillTDCHistograms
	case 14: numf++;break; //fillADCHistograms
	case 15: numf++;break; //writeEventToScreen
	case 17: numf++;numFit++;break; //prompt Tracking
	case 18: numf=numf+NumberOfDetectors;break; //get Shape
	case 19: numf=numf+NumberOfDetectors;break; //Pedestal/binning/offset/walk
	case 20: numf++;break; //read data from root file
	case 21: numf++;break; //hodoPixel
	case 22: numf++;break; //barrelPixel
	case 23: numf++;break; //microPixel
	case 24: numf++;break; //write data to root file(s)
	case 25: numf++;break; //write histos to root file
	case 26: numf++;break; //calibration of barrel pixel
	case 27: numf=numf+NumberOfDetectors;break; //Pulse height correction
	case 28: numf=numf+NumberOfDetectors;break; //Light run correction
	case 29: numf++;break;
	case 32: numf++;break;
	case 30:
	case 31: numf++;break; //quirl/ringPixel
	case 33: numf++;break; //geometry to momentum
	case 34: numf++;break;
	case 35: numf=numf+1;break;
	case 36: numf=numf+1;numFit=numFit+3;break;//V-search
	case 37: numf=numf+1;break;
	case 38: numf=numf+1;numFit++;break;//reaction recognition
	case 39: numf=numf+1;break;
	case 40: numf=numf+1;break;
	case 41: numf=numf+1;break;
	case 42: numf=numf+ap[i].getParam<vector<int> >(0).getData().size();break;
	case 43: numf=numf+1;numFit++;break;
	case 44: numf=numf+1;break;
	case 45: numf=numf+1;break;
	case 46: numf=numf+1;break;
	case 47: numf=numf+1;break;
	case 48: numf=numf+1;break;
	case 49: numf=numf+1;break;
	case 50: numf=numf+1;break;
	case 51: numf=numf+1;break;
      case 52: numf=numf+1;break;
      case 53: numf=numf+1;break;
      case 54: numf=numf+1;break;
      case 55: numf=numf+1;break;
      case 56: numf=numf+1;break;
	case 57:numf=numf+1;break;
	case 58:numf=numf+1;break;
      case 60: numf=numf+1;break;
      case 61: numf=numf+1;break;
      case 62: numf=numf+1;break;
      case 63: numf=numf+1;break;
      case 64: numf=numf+1;break;
	}
    }
  anaLog<<numf<<endli;
  waitFor=new bool[numf];
  f=new AAlgorithm**[nThreads];
  fits=new AFitAlgorithm**[nThreads];
  for(int ii=0;ii<nThreads;ii++)
    {
      f[ii]=new AAlgorithm*[numf];
      fits[ii]=new AFitAlgorithm*[numFit];
      for(int i=0;i<numf;i++)
	f[ii][i]=0;
    }
  int j=0;
  int jfit=0;
  int nF;
  anaLog<<"init: "<<endli;
  for(unsigned int i=0;i<ap.size();i++)
    {
      if(!ap[i].IsUsed())continue;
       emit initStateChanged((string("init algorithm ")+ap[i].getName()).data());
      emit algorithmInit(i);

      j+=getAlgorithm(ap[i],runParam, j, jfit, nF);
      jfit+=nF;
       emit algorithmInited(i);	
    }
  if(numf<j)cout<<numf<<" "<<j<<endl;
  numf=j;
    anaLog<<numf<<" algorithms for "<<nThreads<<" threads declared, taking "<<timer.elapsed()<<"ms"<<decD<<endli;
    numAlgorithms=numf;
    isInitA=true;
}
void tofAnalysis::killAlgorithms() 
{
    anaLog<<"delete algorithms"<<endli;   
    if(f==NULL||isInitA==false)
      {
	f=NULL;
	isInitA=false;
	anaLog<<"done"<<endli;
	return;
      }
    for(int ii=nThreads-1;ii>=0;ii--)
      {
	anaLog<<"kill Thread "<<ii<<endli;
	if(f[ii]==NULL)continue;
	emit finishStateChanged((string("finalize thread ")+string_number(ii)).data());
	for (int i=numAlgorithms-1;i>=0;i--)
	  {
	    anaLog<<f[ii][i]->getName();
	    emit finishStateChanged((string("finalize \"")+f[ii][i]->getName()+"\"").data());
	    if(f[ii][i])delete f[ii][i];
	    f[ii][i]=NULL;
	    anaLog<<" done"<<endli;
	  }
	delete []f[ii];
	f[ii]=NULL;
      }
    delete [] f;
    f=NULL;
    isInitA=false;
    anaLog<<"done"<<endli;
}

algorithm_parameter tofAnalysis::getAlgorithmParameter(int ID)
{
  switch(ID)
    {
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 5: break;
    case 6: break;
    case 7: break;
    case 8: 
      return AConversion::getDescription();
    case 9: 
      return AReadFromTade::getDescription();
    case 10: break;
    case 11: break;
    case 12: break;
    case 13: break;
    case 14: break;
    case 15: break;//writeEventToScreen
    case 30:
    case 31:
      return ARingpixel::getDescription();
    //case 17: 
      //return ATuTrackSearch::getDescription();
    //case 36:
      //return ATuVSearch::getDescription();
    case 18: 
      return AGetTheShape::getDescription();
    case 19: 
      return ACalibration::getDescription();
    case 20:
      return AReadFromRoot::getDescription();
    case 21: 
      return AHodoPixel::getDescription();
    case 22:
      return ABarrelPixel::getDescription();
    case 23: 
      return AMicroPixel::getDescription();
    case 24:
      return AWriteDataToRootFile::getDescription();
    case 25: 
      return AWriteHistogramsToRootFile::getDescription();
//     case 26:
//       return ABarrelCalibration::getDescription();
    case 27:
      return APulsHeightCorrectionWedge::getDescription();
//     case 28:
//       return ALightRunCorrectionWedge::getDescription();
    case 29:
      return ACalculateTrackProperties::getDescription();
    case 32:
      return ACMTransformation::getDescription();
//     case 33:
//       return AMomentumFromGeometry::getDescription();
    case 34:
      return AGenerateOutput::getDescription();
    case 35:
      return AHodo3Pixel::getDescription();
    case 37:
      return AMicroCluster::getDescription();
    case 38:
      return AReactionRecognition::getDescription();
    case 39:
      return AGenerateEvents::getDescription();
    case 40:
      return AReadFromSimpleTree::getDescription();
    case 41:
      return AKinkSearch::getDescription();
    case 42:
      return ATeufelCorrection::getDescription();
    case 43:
      return APromptHistoTracker::getDescription();
    case 44:
      return ATofPixCorrection::getDescription();
    case 45:
      return ACalibrationGeneration::getDescription();
    case 46:
      return AReadReactionFromRoot::getDescription();
    case 47:
      return ALineTrackSearch::getDescription();
    case 48:
      return AGenerateEventsWithDecay::getDescription();
    case 49:
      return ACompareToGIN::getDescription();
    case 50:
      return AVLineTrackSearch::getDescription();
    case 51:
      return AVirtualMachine::getDescription();
//     case 52:
//       return ATofStrawTrackFinder::getDescription();
    case 53:
      return AHitClusterSearch::getDescription();
    case 54:
      return AMultipleTreeOutput::getDescription();
    case 55:
      return AMultipleTreeInput::getDescription();
    case 56:
      return APixelTreeWriteOut::getDescription();
    case 57:
      return ATDCcalibration::getDescription();
    case 58:
      return AapplyLRC::getDescription();
    case 60:
      return AHitTreeInput::getDescription();
    case 61:
      return AHitTreeOutput::getDescription();
    case 62:
      return ATrackTreeInput::getDescription();
    case 63:
      return ATrackTreeOutput::getDescription();
    case 64:
      return APixelTracking::getDescription();
    }
  return algorithm_parameter("none",0,0);
}




int tofAnalysis::getAlgorithm(AAlgorithm ***out,int &executeUpTo,const algorithm_parameter &param,const run_parameter &firstRun, TEvent &event,TSetup &setup, int **numberOfHits, int *numberOfTracks, int **numberOfPixels, int **numberOfClusters, int **numberOfHitClusters, TRawHit ***raws, TCalibHit ***calibratedHits, THitCluster ***hitClusters, TTrack **tracks, TPixel ***pixels, TCluster ***clusters, TMaterial **materials, TDetector **detectors, bool &readValid,int &readInID, QObject *stearIt,bool &eventRequesting)
{
  int nAlgos=0;
  executeUpTo=1;
  nAlgos=1;     
  switch(param.getID())
    {
    case 0: 
      {
	executeUpTo=0;
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ALineFitAlgorithm(setup.getNumberOfDetectors());
	break;
      }
    case 1:{
      executeUpTo=0;
      (*out)=new AAlgorithm*[nAlgos];
      (*out)[0]=new AVertexFit(setup.getNumberOfDetectors());
      break;}
    case 2:
      {
	executeUpTo=0;
	(*out)=new AAlgorithm*[nAlgos];
	if(param.getNumberOfParam<int>()>1&&param.getNumberOfParam<float>()>0)
	  (*out)[0]=new AKinFit(param.getParam<int>(0).getData(),
				param.getParam<int>(1).getData(),
				param.getParam<float>(0).getData());
	else
	  (*out)[0]=new AKinFit(400,30,0.00001); 
	break;
      }
    case 3: break;
    case 4: break;
    case 5: break;
    case 6: break;
    case 7: break;
    case 8: 
      {
	anaLog<<"convert data from other format: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AConversion(""/*inputParameter[0]*/,event,NULL/*&input_mutex*/, readValid);
// 	for(int ii=0;ii<nThreads;ii++)
// 	  for(int jj=0;jj<nThreads;jj++)
// 	    if(ii!=jj) 
// 	      connect(f[ii][position],SIGNAL(eventRead()),f[jj][position],SLOT(newEvent()));
	anaLog<<" done"<<endli;
	break;
      }
    case 9: 
      {
	anaLog<<"read data from tade file: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AReadFromTade(raws,
				    event.getEventNumber(),
				    event.getTrigger(),
				    numberOfHits, 
				    setup.getNumberOfDetectors(),
				    event.getMaxNumber<TCalibHit>(),
				    readInID,NULL,
				    readValid);
	connect(stearIt,SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(newInput(run_parameter&)));
// 	for(int ii=0;ii<nThreads;ii++)
// 	  {
// 	    if(ii!=0)
// 	      connect(f[0][position],SIGNAL(newRun(ifstream*)),f[ii][position],SLOT(newInput(ifstream*)));
// 	    for(int jj=0;jj<nThreads;jj++)
// 	      if(ii!=jj) 
// 		connect(f[ii][position],SIGNAL(eventRead(int,int,int)),f[jj][position],SLOT(newEvent(int,int,int)));
// 	  }
	anaLog<<" done"<<endli;
	break;
      }
    case 10: break;
    case 11: break;
    case 12: break;
    case 13: break;
    case 14: break;
    case 15: break;//writeEventToScreen
    case 30:
    case 31:
      { 
	anaLog<<"ring pixel calculation: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ARingpixel(param.getParam<bool>(0).getData(),
				 param.getParam<bool>(1).getData(),
				 param.getParam<bool>(2).getData(),
				 param.getParam<bool>(3).getData(),
				 param.getParam<bool>(4).getData(),
				 param.getParam<bool>(5).getData(),
				 param.getParam<bool>(6).getData(),
				 param.getParam<point3D>(0).getData(),
				 param.getParam<point3D>(1).getData(),
				 param.getParam<vector3D>(0).getData(),
				 *detectors[param.getParam<int>(0).getData()],
				 *detectors[param.getParam<int>(1).getData()],
				 *detectors[param.getParam<int>(2).getData()],
				 param.getParam<int>(3).getData(),
				 param.getParam<int>(4).getData(),
				 param.getParam<int>(5).getData(),
				 param.getParam<int>(6).getData(),
				 param.getParam<int>(7).getData(),
				 param.getParam<float>(0).getData(),
				 param.getParam<float>(1).getData(),
				 numberOfPixels[param.getParam<int>(7).getData()],
				 calibratedHits[param.getParam<int>(0).getData()],
				 calibratedHits[param.getParam<int>(1).getData()],
				 calibratedHits[param.getParam<int>(2).getData()],
				 *numberOfHits[param.getParam<int>(0).getData()],
				 *numberOfHits[param.getParam<int>(1).getData()],
				 *numberOfHits[param.getParam<int>(2).getData()],
				 pixels[param.getParam<int>(7).getData()],
				 event.getMaxNumber<TPixel>());
	anaLog<<"done"<<endli;
	break;//quirl/ringPixel
      }
    //case 17: 
      //{
	//anaLog<<"prompt tracking: ";
	//int n=0;
	//AAlgorithm **tmpA;
	//int ec;
	//if(param.getNumberOfParam<algorithm_parameter>()>0)
	  //n=getAlgorithm(&tmpA,ec,param.getParam<algorithm_parameter>(0).getData(),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	//else
	  //n=getAlgorithm(&tmpA,ec,algorithm_parameter("line fit",0,0,0),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	//nAlgos=n+1;     
	//(*out)=new AAlgorithm*[nAlgos];
	//for(int i=0;i<n;i++)(*out)[i+1]=tmpA[i];
	//delete[]tmpA;
	////showFrequency=showFrequency/10;
	//int num;//param.getNumberOfParam<QBitArray>();
	//num=30;
	//int tmp[num];
	//for(int k=0;k< num;k++)
	  //tmp[num]=0;
	//(*out)[0]=new ATuTrackSearch(event.getMaxNumber<TTrack>(), 
				     //numberOfTracks,tracks, 
				     //numberOfPixels,pixels, 
				     //setup.getNumberOfDetectors(), 
				     //numberOfHits,calibratedHits, 
				     //param.getParam<int>(0).getData(), 
				     //((AFitAlgorithm*)(*out)[1]), tmp,
				     //param.getParam<vector<int> >(0).getData(), 
				     //param.getParam<vector<int> >(1).getData(), 
				     //param.getParam<vector<int> >(2).getData(),
				     //param.getParam<vector<float> >(0).getData(),
				     //param.getParam<vector<float> >(1).getData(),
				     //param.getParam<vector<int> >(3).getData(),
				     //param.getParam<vector<int> >(4).getData(),
				     //param.getParam<vector<int> >(5).getData());
	//anaLog<<"done"<<endli;
	//break;//prompt Tracking
      //}
    //case 36:
      //{
	//anaLog<<"Tuebingen vertex tracking: ";
	//if(param.getNumberOfParam<algorithm_parameter>()>0)
	  //{
	    //AAlgorithm **tmpA[param.getNumberOfParam<algorithm_parameter>()];
	    //int n[param.getNumberOfParam<algorithm_parameter>()];
	    //nAlgos=1;
	    //int ec;
	    //for(int i=0;i<param.getNumberOfParam<algorithm_parameter>();i++)
	      //{
		//n[i]=getAlgorithm(&(tmpA[i]),ec,param.getParam<algorithm_parameter>(0).getData(),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
		//nAlgos+=n[i];
	      //}
	    //(*out)=new AAlgorithm*[nAlgos];
	    //int sum=0;
	    //for(int j=0;j<param.getNumberOfParam<algorithm_parameter>();j++)
	      //{
		//for(int i=0;i<n[j];i++)
		  //(*out)[i+1+sum]=tmpA[j][i];
		//sum+=n[j];
		//delete[] tmpA[j];
	      //}
	  //}
	//else
	  //{
	    //AAlgorithm **tmpA=NULL;
	    //AAlgorithm **tmpB=NULL;
	    //AAlgorithm **tmpC=NULL;
	    //int ec;
	    //int n1=getAlgorithm(&tmpA,ec,algorithm_parameter("line fit",0,0,0),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	    //int n2=getAlgorithm(&tmpA,ec,algorithm_parameter("line fit",0,0,0),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	    //int n3=getAlgorithm(&tmpA,ec,algorithm_parameter("vertex fit",0,0,1),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	    //nAlgos=n1+n2+n3+1;     
	    //(*out)=new AAlgorithm*[nAlgos];
	    //for(int i=0;i<n1;i++)(*out)[i+1]=tmpA[i];
	    //delete[]tmpA;
	    //for(int i=0;i<n2;i++)(*out)[i+1+n2]=tmpB[i];
	    //delete[]tmpB;
	    //for(int i=0;i<n3;i++)(*out)[i+1+n2+n3]=tmpC[i];
	    //delete[]tmpC;
	  //}

	////	showFrequency=showFrequency/10;
	//(*out)[0]=new ATuVSearch(setup.getNumberOfDetectors(),event.getMaxNumber<TTrack>(),
				 //event.getMaxNumber<TPixel>(),event.getMaxNumber<TPixel>(), 
				 //*numberOfTracks, numberOfHits, numberOfPixels,
				 //numberOfClusters, 
				 //calibratedHits, pixels, clusters, tracks, 
				 //((AFitAlgorithm*)(*out)[3]),((AFitAlgorithm*) (*out)[2]),((AFitAlgorithm*)(*out)[1]), 
				 //param.getParam<float>(0).getData(), param.getParam<float>(1).getData(), 
				 //param.getParam<int>(0).getData(), param.getParam<point3D>(0).getData(), 
				 //param.getParam<int>(1).getData(),param.getParam<float>(2).getData(),
				 //param.getParam<int>(2).getData(),param.getParam<float>(3).getData(),
				 //param.getParam<float>(4).getData(),/*param.getParam<float>(5).getData(),
								      //param.getParam<float>(6).getData(),*/param.getParam<bool>(0).getData(),
				 //param.getParam<vector<int> >(0).getData(),param.getParam<vector<float> >(0).getData(),
				 //param.getParam<vector<int> >(2).getData(),param.getParam<vector<int> >(3).getData(),
				 //param.getParam<vector<float> >(1).getData(),param.getParam<vector<int> >(1).getData(),
				 //param.getParam<vector<int> >(4).getData(),param.getParam<vector<int> >(5).getData(),
				 //param.getParam<vector<int> >(6).getData(),param.getParam<vector<float> >(2).getData(),
				 //param.getParam<vector<float> >(3).getData());
	//anaLog<<"done"<<endli;
	//break;//tuebingen vertex search
      //}
    case 18: 
      {
	anaLog<<"asign shapes to the hits: ";
	int n=0;
	for(int ii=0;ii<setup.getNumberOfDetectors();ii++)
	  if(detectors[ii]->getNumberOfElements()>1)
	    n++;
	nAlgos=n;
	executeUpTo=n;
	(*out)=new AAlgorithm*[nAlgos];
	n=0;
	for(int ii=0;ii<setup.getNumberOfDetectors();ii++)
	  {
	    anaLog<<"#"<<ii;
	    if(detectors[ii]->getNumberOfElements()>1)
	      {
		(*out)[n]=new AGetTheShape(calibratedHits[ii],numberOfHits[ii],detectors[ii]);
		n++;
	      }
	  }
	anaLog<<"done"<<endli;
	break; //get Shape
      }
    case 19: 
      {
	anaLog<<"calibration (walk/pedestal/binning/cuts):";
	int n=0;
	bool ut,ua;
	for(int ii=0;ii<setup.getNumberOfDetectors();ii++)
	  {
	    ut=false;ua=false;
	    for(unsigned int iii=0;iii<param.getParam<vector<int> >(0).getData().size();iii++)
	      if(param.getParam<vector<int> >(0).getData().at(iii)==ii)
		ua=true;
	    for(unsigned int iii=0;iii<param.getParam<vector<int> >(1).getData().size();iii++)
	      if(param.getParam<vector<int> >(1).getData().at(iii)==ii)
		ut=true;
	    if((ut||ua))n++;
	  }
	nAlgos=n;
	executeUpTo=n;
	(*out)=new AAlgorithm*[nAlgos];
	vector<string> tmp;
	n=0;
	for(int ii=0;ii<setup.getNumberOfDetectors();ii++)
	  {
	    ut=false;ua=false;
	    for(unsigned int iii=0;iii<param.getParam<vector<int> >(0).getData().size();iii++)
	      if(param.getParam<vector<int> >(0).getData().at(iii)==ii)
		ua=true;
	    for(unsigned int iii=0;iii<param.getParam<vector<int> >(1).getData().size();iii++)
	      if(param.getParam<vector<int> >(1).getData().at(iii)==ii)
		ut=true;
	    if(!(ut||ua))continue;
	    if(detectors[ii]->getNumberOfElements()>0)
	      {
		anaLog<<"#"<<ii;
		(*out)[n]=new ACalibration(calibratedHits[ii], 
					   *numberOfHits[ii],
					   *detectors[ii],
					   tmp,event.getRunNumber(),
					   event.getEventNumber(), ua,ut);
		connect(stearIt,SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[n],SLOT(getNewRun(run_parameter &)));
		n++;
	      }
	  }
	anaLog<<"done"<<endli;
	break;//Pedestal/binning/offset/walk
      }
    case 20:
      {
	anaLog<<"read data from root file: ";
	(*out)=new AAlgorithm*[nAlgos];
	//	for(int ii=0;ii<param.getNumberOfParam<string>();ii++)
	//inputParameter.push_back(param.getParam<string>(ii).getData());
	(*out)[0]=new AReadFromRoot(param.getParam<string>(0).getData(), 
				    param.getParam<string>(1).getData(), 
				    "event", event,setup,readInID,
				    1000000000,NULL);
// 	for(int ii=0;ii<nThreads;ii++)
// 	  for(int jj=0;jj<nThreads;jj++)
// 	    if(ii!=jj) 
// 	      connect(f[ii][position],SIGNAL(eventRead()),f[jj][position],SLOT(newEvent()));
	anaLog<<"done"<<endli;
	break;//read data from root file
      }
    case 21: 
      {
	anaLog<<"hodo pixel calculation: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AHodoPixel(event.getMaxNumber<TPixel>(),
				 *numberOfPixels[param.getParam<int>(0).getData()],
				 pixels[param.getParam<int>(0).getData()],
				 numberOfHits,calibratedHits,
				 numberOfHitClusters,hitClusters,param);
	anaLog<<"done"<<endli;
	break;//hodoPixel
      }
    case 22:
      {
	anaLog<<"barrel pixel calculation: ";
	(*out)=new AAlgorithm*[nAlgos];
	vector<string> tmp;
	(*out)[0]=new ABarrelPixel(
				   event.getMaxNumber<TPixel>(), 
				   param.getParam<int>(0).getData(), 
				   *numberOfPixels[param.getParam<int>(0).getData()], 
				   pixels[param.getParam<int>(0).getData()], 
				   *numberOfHits[param.getParam<int>(1).getData()], 
				   *numberOfHits[param.getParam<int>(2).getData()], 
				   calibratedHits[param.getParam<int>(1).getData()], 
				   calibratedHits[param.getParam<int>(2).getData()], 
				   detectors[param.getParam<int>(1).getData()], 
				   param.getParam<float>(0).getData(),
				   event.getEventNumber(),event.getRunNumber(),tmp);
	connect(stearIt,SIGNAL(newRun(run_parameter&,beamTime_parameter&)),(*out)[0],SLOT(onNewRun(run_parameter&)));
	anaLog<<"done"<<endli;
	break; //barrelPixel
      }
    case 23: 
      {
	anaLog<<"micro strip pixel calculation: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AMicroPixel(event.getMaxNumber<TPixel>(), 
				  *numberOfPixels[param.getParam<int>(0).getData()], 
				  pixels[param.getParam<int>(0).getData()], 
				  numberOfHits, numberOfHitClusters, 
				  calibratedHits, hitClusters,param);
	anaLog<<"done"<<endli;
	break;//microPixel
      }
    case 24:
      {
	anaLog<<"write data to root file: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AWriteDataToRootFile(param.getParam<string>(0).getData(),	
					   param.getParam<string>(1).getData(),	
					   "event",event,setup,
					   event.getMaxNumber<TTrack>(),true);
	anaLog<<"done"<<endli;
	break;//write data to root file(s)
      }
    case 25: 
      {
	anaLog<<"write standard histos to root file: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AWriteHistogramsToRootFile(param.getParam<string>(0).getData(),
						 param.getParam<string>(1).getData(),	
						 event,setup,
						 param.getParam<string>(2).getData(),true, NULL);
	anaLog<<"done"<<endli;
	break;//write histos to root file
      }
//     case 26:
//       {
// 	anaLog<<"barrel pixel calibration: ";
// 	(*out)=new AAlgorithm*[nAlgos];
// 	(*out)[0]=new ABarrelCalibration(setup.getNumberOfDetectors(), 
// 					 pixels[param.getParam<int>(3).getData()], 
// 					 pixels[param.getParam<int>(4).getData()], 
// 					 pixels[param.getParam<int>(0).getData()], 
// 					 *numberOfPixels[param.getParam<int>(3).getData()], 
// 					 *numberOfPixels[param.getParam<int>(4).getData()], 
// 					 *numberOfPixels[param.getParam<int>(0).getData()], 
// 					 calibratedHits[param.getParam<int>(1).getData()],
// 					 calibratedHits[param.getParam<int>(2).getData()], 
// 					 *numberOfHits[param.getParam<int>(1).getData()],
// 					 *numberOfHits[param.getParam<int>(2).getData()], 
// 					 param.getParam<float>(0).getData(), 
// 					 param.getParam<float>(1).getData(),
// 					 detectors[param.getParam<int>(1).getData()],
// 					 NULL);
// 	anaLog<<"done"<<endli;
// 	break;//calibration of Barrel pixel
//       }
    case 27:
      {
	anaLog<<"puls height correction of QDC: ";
	nAlgos=param.getParam<vector<int> >(0).getData().size();     
	executeUpTo=nAlgos;
	(*out)=new AAlgorithm*[nAlgos];
	int detID;
	vector<string> calibrationFiles;
	if(firstRun.hasOwnCalibration()||firstRun.hasAdditionalCalibration())
	  for(int iii=0;iii<firstRun.getNumberOfCalibrationFiles();iii++)
	    calibrationFiles.push_back(firstRun.getCalibrationFile(iii));
	if(!firstRun.hasAdditionalCalibration())
	  for(int iii=0;iii<firstRun.getParent()->getNumberOfCalibrationFiles();iii++)
	    calibrationFiles.push_back(firstRun.getParent()->getCalibrationFile(iii));
	for(unsigned int ii=0;ii<param.getParam<vector<int> >(0).getData().size();ii++)
	  {
	    anaLog<<"#"<<ii;
	    detID=param.getParam<vector<int> >(0).getData().at(ii);
	    volumeShape *tmp=detectors[detID]->getShape(0);
	    if(tmp->getName()=="wedge")
	      {
		(*out)[ii]=new APulsHeightCorrectionWedge(tracks,
							 *numberOfTracks,
							 *detectors[detID],
							 event.getEventNumber(),
							 event.getRunNumber(),
							 calibrationFiles);
	      }
	    else if(tmp->getName()=="spiral")
	      {
		(*out)[ii]=new APulsHeightCorrectionSpiral(tracks,
							   *numberOfTracks,
							   *detectors[detID],
							   event.getEventNumber(),
							   event.getRunNumber(),
							   calibrationFiles);
	      }
	    else if(tmp->getName()=="cylinder")
	      {
		(*out)[ii]=new APulsHeightCorrectionTube(tracks,
							 *numberOfTracks,
							 *detectors[detID],
							 event.getEventNumber(),
							 event.getRunNumber(),
							 calibrationFiles);
	      }
	  }
	anaLog<<"done"<<endli;
	break; //Pulse height correction
      }
//     case 28:
//       {
// 	anaLog<<"correction for light run time in detectors: ";
// 	nAlgos=param.getParam<vector<int> >(0).getData().size();     
// 	executeUpTo=nAlgos;
// 	(*out)=new AAlgorithm*[nAlgos];
// 	int detID;
// 	for(unsigned int ii=0;ii<param.getParam<vector<int> >(0).getData().size();ii++)
// 	  {
// 	    anaLog<<"#"<<ii;
// 	    detID=param.getParam<vector<int> >(0).getData().at(ii);
// 	    volumeShape *tmp=detectors[detID]->getShape(0);
// 	    vector<string> calibrationFiles;
// 	    if(tmp->getName()=="wedge")
// 	      {
// 		(*out)[ii]=new ALightRunCorrectionWedge(tracks,
// 							*numberOfTracks,
// 							*detectors[detID],
// 							event.getRunNumber(),event.getEventNumber(),
// 							calibrationFiles, param);
// 		connect(stearIt,SIGNAL(newRun(run_parameter&,beamTime_parameter&)),(*out)[ii],SLOT(getNewCalibrations(run_parameter&)));
// 	      }
// 	    if(tmp->getName()=="spiral")
// 	      {
// 		(*out)[ii]=new ALightRunCorrectionSpiral(tracks,
// 							 *numberOfTracks,
// 							 *detectors[detID],
// 							 event.getRunNumber(), event.getEventNumber(),
// 							 calibrationFiles, param);
// 		connect(stearIt,SIGNAL(newRun(run_parameter&,beamTime_parameter&)),(*out)[ii],SLOT(getNewCalibrations(run_parameter&)));
// 	      }
// 	    if(tmp->getName()=="fiber")
// 	      {
// 		(*out)[ii]=new ALightRunCorrectionFiber(tracks,
// 							*numberOfTracks,
// 							*detectors[detID],
// 							event.getRunNumber(),event.getEventNumber(),
// 							calibrationFiles, param);
// 		connect(stearIt,SIGNAL(newRun(run_parameter&,beamTime_parameter&)),(*out)[ii],SLOT(getNewCalibrations(run_parameter&)));
// 	      }
// 	  }
// 	anaLog<<"done"<<endli;
// 	break; //Light run correction
//       }
    case 29:
      {
	anaLog<<"calculate particle speed: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ACalculateTrackProperties(tracks, 
						*numberOfTracks, 
						event.getMaxNumber<TTrack>(), 
						*detectors[param.getParam<vector<int> >(0).getData()[0]],
						param);
	anaLog<<"done"<<endli;
	break;//calculate particle speed
      }
    case 32:
      {
	anaLog<<"Transform to CM-system: ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=NULL;//new ACMTransformation(setup, particles, event.getMaxNumber<TTrack>(), *numberOfTracks,
	//	NULL,event.getEventNumber(),event.getRunNumber());
	anaLog<<"done"<<endli;
	break;//transform particle properties to CM-system
      }
//     case 33:
//       {
// 	anaLog<<"Calculate momentum of Track-particles from geometry: ";
// 	(*out)=new AAlgorithm*[nAlgos];
// 	(*out)[0]=new AMomentumFromGeometry(setup, tracks, *numberOfTracks);
// 	anaLog<<"done"<<endli;
// 	break;//Calculate momentum of Track-particles from geometry
//       }
    case 34:
      {
	anaLog<<"Generate output for analysis comparison: ";
	(*out)=new AAlgorithm*[nAlgos];
	pthread_mutex_t *mutexer=NULL;
// 	if(nThreads>1)
// 	  {
// 	    mutexer=new pthread_mutex_t;
// 	    *mutexer=initer;
// 	  }
	(*out)[0]=new AGenerateOutput(event,
				      param.getParam<string>(0).getData(),
				      param.getParam<string>(1).getData(),
				      param.getParam<string>(2).getData(),
				      param.getParam<bool>(8).getData(),
				      100,
				      param.getParam<bool>(0).getData(),
				      param.getParam<bool>(1).getData(),
				      param.getParam<bool>(2).getData(),
				      param.getParam<bool>(3).getData(),
				      param.getParam<bool>(4).getData(),
				      param.getParam<bool>(5).getData(),
				      param.getParam<vector<string> >(0).getData(), 
				      0,
				      param.getParam<bool>(6).getData(),
				      param.getParam<bool>(7).getData(),
				      mutexer);
	  
// 	for(int ii=1;ii<nThreads;ii++)
// 	  {
// 	    if(param.getParam<bool>(1).getData())
// 	      connect(f[ii][position],SIGNAL(finalizeTracks(vector<TTree*>*,TTree*)),f[0][position],SIGNAL(OnFinalizeTracks(vector<TTree*>*,TTree*)));
// 	    if(param.getParam<bool>(2).getData())
// 	      connect(f[ii][position],SIGNAL(finalizeCluster(vector<TTree*>*,TTree*)),f[0][position],SIGNAL(OnFinalizeCluster(vector<TTree*>*,TTree*)));
// 	    if(param.getParam<bool>(3).getData())
// 	      connect(f[ii][position],SIGNAL(finalizePixel(vector<TTree*>*,TTree*)),f[0][position],SIGNAL(OnFinalizePixel(vector<TTree*>*,TTree*)));
// 	    if(param.getParam<bool>(4).getData())
// 	      connect(f[ii][position],SIGNAL(finalizeHits(vector<TTree*>*,TTree*)),f[0][position],SIGNAL(OnFinalizeHits(vector<TTree*>*,TTree*)));
// 	    if(param.getParam<bool>(5).getData())
// 	      connect(f[ii][position],SIGNAL(finalizeReactions(vector<TTree*>*)),
// 		      f[0][position],SIGNAL(OnFinalizeReactions(vector<TTree*>*)));
// 	  }
	connect(stearIt,SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(OnNewRun(run_parameter&)));
	anaLog<<"done"<<endli;
	break;//generate output file with track data
      }
    case 35:
      {
	anaLog<<"calculate pixel in 3 layered hodo:";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AHodo3Pixel(pixels[param.getParam<int>(3).getData()],
				  *numberOfPixels[param.getParam<int>(3).getData()],
				  event.getMaxNumber<TPixel>(),
				  numberOfHits,numberOfHitClusters,
				  calibratedHits,hitClusters,
				  setup,param);
	anaLog<<" done"<<endli;
	break;
      }
    case 37:
      {
	anaLog<<"simple Cluster search:";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AMicroCluster(param.getParam<int>(0).getData(),
				    *numberOfPixels[param.getParam<int>(1).getData()],
				    pixels[param.getParam<int>(1).getData()],
				    event.getMaxNumber<TPixel>(),
				    *numberOfClusters[param.getParam<int>(0).getData()],
				    clusters[param.getParam<int>(0).getData()],
				    param.getParam<int>(2).getData());
	anaLog<<" done"<<endli;
	break;
      }
    case 38:
      {
	anaLog<<"reaction recognition:"; 
	int n=0;
	AAlgorithm **tmpA;
	int ec;
	if(param.getNumberOfParam<algorithm_parameter>()>0)
	  n=getAlgorithm(&tmpA,ec,param.getParam<algorithm_parameter>(0).getData(),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	else
	  n=getAlgorithm(&tmpA,ec,algorithm_parameter("kin fit",0,0,2),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	nAlgos=n+1;   
	executeUpTo=1;  
	(*out)=new AAlgorithm*[nAlgos];
	for(int i=0;i<n;i++)(*out)[i+1]=tmpA[i];
	delete[]tmpA;
	vector<algorithm_parameter> tmp;
	for(int i=0;i<param.getNumberOfParam<algorithm_parameter>();i++)
	  if(param.getParam<algorithm_parameter>(i).getData().getName()!="kinFit")
	    tmp.push_back(param.getParam<algorithm_parameter>(i).getData());
	if(param.getNumberOfParam<algorithm_parameter>()>0)
	  (*out)[0]=new AReactionRecognition(setup, event, ((AFitAlgorithm*)(*out)[1]), tmp);
	else
	  (*out)[0]=new AReactionRecognition(setup, event, ((AFitAlgorithm*)(*out)[1]), param.getParam<string>(0).getData()); 
	anaLog<<" done"<<endli; 
	break; 
      }
    case 39:
      {
	anaLog<<"Event Generator:";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AGenerateEvents(tracks,*numberOfTracks,
				      event.getEventNumber(),
				      setup,
				      param.getParam<vector<int> >(0).getData(),
				      param.getParam<string>(0).getData(),
				      param.getParam<vector<int> >(1).getData(),
				      param.getParam<bool>(0).getData(), 
				      readValid);
	anaLog<<" done"<<endli;
	break;
      }
    case 40:
      {
	anaLog<<"read from simple tree:";
	(*out)=new AAlgorithm*[nAlgos];
	pthread_mutex_t *inmutex=NULL;
// 	if(nThreads>1)
// 	  {
// 	    inmutex=new pthread_mutex_t;
// 	    (*inmutex)=initer;
// 	  }
	(*out)[0]=new AReadFromSimpleTree(param.getParam<string>(0).getData(),
					  param.getParam<string>(1).getData(),
					  param.getParam<string>(6).getData(),
					  param.getParam<bool>(6).getData(),
					  tracks,
					  *numberOfTracks,
					  event.getEventNumber(),
					  event.getMaxNumber<TTrack>(),
					  event,
					  readInID,
					  &setup ,
					  param.getParam<bool>(0).getData(),
					  param.getParam<bool>(1).getData(), 
					  param.getParam<bool>(2).getData(), 
					  param.getParam<bool>(3).getData(),
					  param.getParam<string>(2).getData(),
					  param.getParam<string>(3).getData(),
					  param.getParam<string>(4).getData(), 
					  param.getParam<string>(5).getData(), 
					  param.getParam<bool>(4).getData(),
					  param.getParam<bool>(5).getData(),
					  param.getParam<int>(0).getData(),
					  readValid,inmutex);
// 	for(int ii=0;ii<nThreads;ii++)
// 	  {
	    connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(getNewRun(run_parameter&)));
// 	    for(int iii=ii+1;iii<nThreads;iii++)
// 	      {
// 		connect(f[ii][position],SIGNAL(hitRead(int)),
// 			f[iii][position],SLOT(OnHitRead(int)));
// 		connect(f[ii][position],SIGNAL(pixelRead(int)),
// 			f[iii][position],SLOT(OnPixelRead(int)));
// 		connect(f[ii][position],SIGNAL(clusterRead(int)),
// 			f[iii][position],SLOT(OnClusterRead(int)));
// 		connect(f[ii][position],SIGNAL(trackRead(int)),
// 			f[iii][position],SLOT(OnTrackRead(int)));
// 		connect(f[iii][position],SIGNAL(hitRead(int)),
// 			f[ii][position],SLOT(OnHitRead(int)));
// 		connect(f[iii][position],SIGNAL(pixelRead(int)),
// 			f[ii][position],SLOT(OnPixelRead(int)));
// 		connect(f[iii][position],SIGNAL(clusterRead(int)),
// 			f[ii][position],SLOT(OnClusterRead(int)));
// 		connect(f[iii][position],SIGNAL(trackRead(int)),
// 			f[ii][position],SLOT(OnTrackRead(int)));
// 	      }
// 	  }
	anaLog<<" done"<<endli;
	break;
      }
    case 41:
      {
	anaLog<<"search for carged decay:"; 
// 	int n=0;
// 	AAlgorithm **tmpA;
// 	int ec;
// 	if(param.getNumberOfParam<algorithm_parameter>()>0)
// 	  n=getAlgorithm(&tmpA,ec,param.getParam<algorithm_parameter>(0).getData(),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
// 	else
// 	  n=getAlgorithm(&tmpA,ec,algorithm_parameter("vertex fit",0,0,1),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
// 	nAlgos=n+1;   
// 	executeUpTo=1;  
	(*out)=new AAlgorithm*[nAlgos];
// 	for(int i=0;i<n;i++)(*out)[i+1]=tmpA[i];
// 	delete[]tmpA;
	(*out)[0]=new AKinkSearch(tracks, 
				  event.getMaxNumber<TTrack>(), 
				  *numberOfTracks, 
				  pixels, 
				  numberOfPixels, 
				  clusters, 
				  numberOfClusters, 
				  calibratedHits, 
				  numberOfHits, 
				  param.getParam<vector<int> >(0).getData(), 
				  param.getParam<vector<int> >(1).getData(), 
				  param.getParam<vector<int> >(2).getData(), 
				  param.getParam<vector<int> >(3).getData(), 
				  param.getParam<vector<int> >(4).getData(), 
				  param.getParam<vector<int> >(5).getData(), 
				  param.getParam<vector<int> >(6).getData(), 
				  param.getParam<vector<int> >(7).getData(), 
				  param.getParam<vector<int> >(8).getData(), 
				  param.getParam<int>(0).getData(), 
				  param.getParam<int>(1).getData(), 
				  param.getParam<vector<int> >(9).getData(), 
				  param.getParam<vector<int> >(10).getData(), 
				  param.getParam<float>(0).getData()); 
	anaLog<<" done"<<endli; 
	break; 
      }
    case 42:
      {
	anaLog<<"Teufel correction:";
	nAlgos=param.getParam<vector<int> >(0).getData().size();     
	executeUpTo=nAlgos;
	(*out)=new AAlgorithm*[nAlgos];
	vector<int>tmp=param.getParam<vector<int> >(0).getData();
	vector<string> tmps;
	for(int iii=0;iii<firstRun.getNumberOfCalibrationFiles();iii++)
	  tmps.push_back(firstRun.getCalibrationFile(iii));
	int id;
	for(unsigned int iii=0;iii<param.getParam<vector<int> >(0).getData().size();iii++)
	  {
	    id=param.getParam<vector<int> >(0).getData().at(iii);
	    anaLog<<id;
	    (*out)[iii]=new ATeufelCorrection(calibratedHits[id],
					      *numberOfHits[id],
					      id,
					      detectors[id]->getNumberOfElements(),
					      event.getEventNumber(),
					      event.getRunNumber(),
					      tmps,
					      param.getParam<string>(0).getData());
	    connect(stearIt,SIGNAL(newRun(run_parameter &, beamTime_parameter &)),(*out)[iii],SLOT(onNewRum(run_parameter &)));
	  }
	anaLog<<" done"<<endli;
	break;	 
      }
    case 43:
      {
	anaLog<<"Prompt histo tracker:"; 
	int n=0;
	AAlgorithm **tmpA;
	int ec;
	if(param.getNumberOfParam<algorithm_parameter>()>0)
	  n=getAlgorithm(&tmpA,ec,param.getParam<algorithm_parameter>(0).getData(),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	else
	  n=getAlgorithm(&tmpA,ec,algorithm_parameter("line fit",0,0,0),firstRun, event,setup, numberOfHits, numberOfTracks, numberOfPixels, numberOfClusters, numberOfHitClusters, raws, calibratedHits, hitClusters, tracks, pixels, clusters, materials, detectors, readValid,readInID,stearIt,eventRequesting);
	nAlgos=n+1;     
	(*out)=new AAlgorithm*[nAlgos];
	for(int i=0;i<n;i++)(*out)[i+1]=tmpA[i];
	delete[]tmpA;

	TFormula *form=new TFormula(); 
	(*out)[0]=new APromptHistoTracker(setup, tracks, 
					  *numberOfTracks, 
					  event.getMaxNumber<TTrack>(), 
					  calibratedHits,numberOfHits, 
					  false,param.getParam<int>(0).getData(), 
					  false,0.,false, form, 
					  ((AFitAlgorithm*)(*out)[1]), 
					  param.getParam<int>(1).getData(), 
					  param.getParam<int>(2).getData(), 
					  param.getParam<float>(3).getData(), 
					  param.getParam<float>(4).getData(), 
					  param.getParam<float>(1).getData(), 
					  param.getParam<float>(2).getData(), 
					  param.getParam<bool>(0).getData(), 
					  param.getParam<bool>(1).getData(), 
					  param.getParam<string>(0).getData(), 
					  param.getParam<float>(0).getData()); 
	anaLog<<" done"<<endli; 
	break; 
      }
    case 44:
      {
	anaLog<<"Tof Pix correction:"; 
	(*out)=new AAlgorithm*[nAlgos];
	vector<string> tmp; 
	(*out)[0]=new ATofPixCorrection(event.getEventNumber(), 
					event.getRunNumber(), 
					tracks, *numberOfTracks, 
					setup, param, tmp); 
	connect(stearIt,SIGNAL(newRun(run_parameter &, beamTime_parameter &)),(*out)[0],SLOT(setNewRun(run_parameter &))); 
	anaLog<<" done"<<endli; 
	break; 
      }
    case 45:
      {
	anaLog<<"Calibration Generation:";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ACalibrationGeneration(event,setup,param);
	connect(stearIt,SIGNAL(newRun(run_parameter &, beamTime_parameter &)),(*out)[0],SLOT(getNewRun(run_parameter &))); 
	anaLog<<" done"<<endli;
	break;
      }
    case 46:
      {
	anaLog<<"Read reactions from root file";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AReadReactionFromRoot(setup,event,readValid,param);
	connect(stearIt,
		SIGNAL(newRun(run_parameter &,beamTime_parameter&)),
		(*out)[0],
		SLOT(OnNewRun(run_parameter &,beamTime_parameter&)));
	anaLog<<" done"<<endli;
	break;
      }
    case 47:
      {
	anaLog<<"Track search for straight tracks";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ALineTrackSearch(setup, tracks, clusters, pixels, calibratedHits, event.getMaxNumber<TTrack>(), *(numberOfTracks), numberOfClusters, numberOfPixels, numberOfHits, param);
	anaLog<<" done"<<endli;
	break;
      }
    case 48:
      {
	anaLog<<"Generate Events with decaying Particles";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AGenerateEventsWithDecay(tracks,*(numberOfTracks), event.getEventNumber(), setup,readValid, param);
	anaLog<<" done"<<endli;
	break;
      }
    case 49:
      {
	anaLog<<"Compare reconstructed tracks with GIN tracks";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new ACompareToGIN(tracks,*(numberOfTracks), event.getEventNumber(), event.getMaxNumber<TTrack>(), param);
	anaLog<<" done"<<endli;
	break;
      }
    case 50:
      {
	anaLog<<"V-line-track-search";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AVLineTrackSearch(setup, 
					tracks, clusters, 
					pixels, calibratedHits, 
					event.getMaxNumber<TTrack>(), 
					*(numberOfTracks),(numberOfClusters), 
					(numberOfPixels), (numberOfHits), 
					param);
	anaLog<<" done"<<endli;
	break;
      }
    case 51:
      {
	anaLog<<"Track particle through virtual detector ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AVirtualMachine(tracks, 
				      calibratedHits, 
				      setup, 
				      *numberOfTracks, 
				      numberOfHits, 
				      param);
          anaLog<<" done"<<endli;
          break;
      }
//     case 52:
//       {
// 	anaLog<<"Interface to Tof-straw-track-finder ";
// 	(*out)=new AAlgorithm*[nAlgos];
// 	(*out)[0]=new ATofStrawTrackFinder(tracks, 
// 					   *numberOfTracks, 
// 					   setup, 
// 					   calibratedHits, 
// 					   numberOfHits, 
// 					   param);
//           anaLog<<" done"<<endli;
//           break;
//       }
    case 53:
      {
	anaLog<<"Hit-Cluster search ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AHitClusterSearch(setup, 
					hitClusters, 
					numberOfHitClusters, 
					calibratedHits, 
					numberOfHits, 
					param);
	anaLog<<" done"<<endli;
	break;
      }
    case 54:
      {
	anaLog<<"Multiple tree output ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AMultipleTreeOutput(20,event.getEventNumber(), 
					  event.getRunNumber(), 
					  event.getTrigger(), 
					  *numberOfTracks, 
					  tracks, 
					  param);
	anaLog<<" done"<<endli;
	break;
      }
    case 55:
      {
	anaLog<<"Read Tracks from multiple trees ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new AMultipleTreeInput(event.getEventNumber(), 
					 event.getRunNumber(), 
					 event.getTrigger(), 
					 numberOfHits, 
					 tracks, 
					 *numberOfTracks, 
					 calibratedHits, 
					 readValid, 
					 param);
	connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(OnNewRun(run_parameter&)));
	anaLog<<" done"<<endli;
	break;
      }
    case 56:
      {
	anaLog<<"Geometry calibration ";
	(*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new APixelTreeWriteOut(setup, 
					 event.getEventNumber(), 
					 event.getRunNumber(), 
					 pixels, 
					 numberOfPixels, 
					 param);
	anaLog<<" done"<<endli;
	break;
      }
      case 57:
        {
	  anaLog<<"Calibrate Geometry, beam, walk, offset";
	  (*out)=new AAlgorithm*[nAlgos];
	  (*out)[0]=new ATDCcalibration(
		event.getEventNumber(),
		event.getRunNumber(),
		tracks, 
		pixels, 
		calibratedHits, 
		*numberOfTracks, 
		numberOfPixels, 
		numberOfHits, 
		setup, 
		param);
	  connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(onNewRun(run_parameter&)));
	  anaLog<<" done"<<endli;
          break;
        }
      case 58:
        {
	  anaLog<<"apply calibration for signal run time in detector";
	  (*out)=new AAlgorithm*[nAlgos];
	  (*out)[0]=new AapplyLRC(setup,
				  tracks, 
				  *numberOfTracks, 
				  event.getEventNumber(),
				  event.getRunNumber(),
				  param);
	  connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(onNewRun(run_parameter&)));
	  anaLog<<" done"<<endli;
          break;
        }
    case 60:
      {
        anaLog<<"Read hit tree ";
        nAlgos=1;
        (*out)=new AAlgorithm*[nAlgos];
        (*out)[0]=new AHitTreeInput(
            setup, 
            calibratedHits, 
            numberOfHits, 
            event.getEventNumber(), 
            event.getRunNumber(), 
            event.getTrigger(), 
            event.getMaxNumber<TCalibHit>(), 
            setup.getNumberOfDetectors(), 
            readValid, 
            param);
	  connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(newRun(run_parameter&)));
	  if(param.getNumberOfParam<bool>()>1)
	    if(param.getParam<bool>(1).getData())
	      {
		eventRequesting=true;
		connect(stearIt,SIGNAL(requestNextEvent()),(*out)[0],SLOT(prepareNextEntry()));
	      }
        anaLog<<" done"<<endli;
        break;
      }
    case 61:
      {
        anaLog<<"Write Hits to dynamical tree ";
        nAlgos=1;
        (*out)=new AAlgorithm*[nAlgos];
        (*out)[0]=new AHitTreeOutput(
            calibratedHits, 
            numberOfHits, 
            event.getEventNumber(), 
            event.getRunNumber(), 
            event.getTrigger(), 
            setup.getNumberOfDetectors(), 
            param);
        anaLog<<" done"<<endli;
        break;
      }
    case 62:
      {
        anaLog<<"Read track tree ";
        nAlgos=1;
        (*out)=new AAlgorithm*[nAlgos];
        (*out)[0]=new ATrackTreeInput(
            event.getEventNumber(), 
            event.getRunNumber(), 
            event.getTrigger(), 
            tracks,
	    *numberOfTracks, 
            event.getMaxNumber<TTrack>(), 
	    setup.getNumberOfDetectors(),
            numberOfHits, 
            calibratedHits, 
            numberOfPixels, 
            pixels, 
            readValid, 
            param);
	  connect(stearIt, SIGNAL(newRun(run_parameter&, beamTime_parameter&)),(*out)[0],SLOT(onNewRun(run_parameter&)));
	  //cout<<"Read track tree "<<param.getNumberOfParam<bool>()<<" "<<param.getParam<bool>(1).getData()<<endl;
	  if(param.getNumberOfParam<bool>()>1)
	    if(param.getParam<bool>(1).getData())
	      {
		eventRequesting=true;
		connect(stearIt,SIGNAL(requestNextEvent()),(*out)[0],SLOT(prepareNextEntry()));
	      }
        anaLog<<" done"<<endli;
        break;
      }
    case 63:
      {
        anaLog<<"Write Tracks to dynamical tree ";
        nAlgos=1;
        (*out)=new AAlgorithm*[nAlgos];
        (*out)[0]=new ATrackTreeOutput(
				       event,
				       tracks, 
				       *numberOfTracks, 
				       event.getEventNumber(), 
				       event.getRunNumber(), 
				       event.getTrigger(), 
				       setup.getNumberOfDetectors(), 
				       param);
        anaLog<<" done"<<endli;
        break;
      }
    case 64:
      {
        anaLog<<"Pixel Tracking";
        nAlgos=1;
        (*out)=new AAlgorithm*[nAlgos];
	(*out)[0]=new APixelTracking(setup,tracks,pixels,clusters,
				     calibratedHits,
				     *numberOfTracks,numberOfPixels,
				     numberOfClusters,numberOfHits,
				     event.getMaxNumber<TTrack>(),param);
        anaLog<<" done"<<endli;
        break;
      }
    }
  return nAlgos;
}






























