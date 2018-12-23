#include <fstream>
#include <stdlib.h>
#include "tofAnalysis_t.h"
#include "logger.h"
#include <qdatetime.h>
#include "Eparticles.h"
#ifndef INT_MAX
#define INT_MAX 100000000
#endif
void* eventThread(void* ptr)
{
  if(ptr==NULL)return NULL;
  //  if(typeid(ptr)!=typeid(tofAnalysis))return NULL;
  ((tofAnalysis*)ptr)->thread_run(NULL);
  return NULL;
}

void* stepThread(void* ptr)
{
  if(ptr==NULL)return NULL;
  ((AAlgorithm*)ptr)->process(NULL);
  return NULL;
}

pthread_mutex_t initM=PTHREAD_MUTEX_INITIALIZER;
tofAnalysis::tofAnalysis(int max,int maxDets, int NThreads):QObject(),maxAlgorithms(max)/*,Event(0,0,100,1000,1000,100,maxDets)*/,nThreads((NThreads>1?NThreads:1)),Setup(maxDets)
{
    NumberOfHits=NULL;
    Raws=NULL;
    CalibratedHits=NULL;
    NumberOfPixels=NULL;
    Pixels=NULL;
    NumberOfClusters=NULL;
    Clusters=NULL;
    NumberOfTracks=NULL;
    Tracks=NULL;
    Particles=NULL;
    numAlgorithms=0;
    isInitD=false;
    isInitS=false;
    isInitA=false;
    rootFile=NULL;
    Event=new TEvent[nThreads];
    for(int i=0;i<nThreads;i++)Event[i].reDimension(200,300,300,200,maxDets);
    threadID=new int[nThreads];
    pthread_mutex_init(&count_mutex,NULL);//PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&tcount_mutex,NULL);// = initM;
    pthread_mutex_init(&input_mutex,NULL);// = initM;
    pthread_mutex_init(&histo_mutex,NULL);//= initM;
    run=true;
    connect(this,SIGNAL(newRun(run_parameter&, beamTime_parameter&)),this,SLOT(showNewRun(run_parameter &)));
}

tofAnalysis::~tofAnalysis()
{
    if(isInitA) killAlgorithms();
    if(isInitD) killData();
    if(isInitS)
    {
	for(int i=0;i<NumberOfDetectors;i++)
	    delete Detectors[i];
	delete [] Detectors;
	for(int i=0;i<NumberOfMaterials;i++)
	    delete Materials[i];
	delete [] Materials;
    }
    delete []Event;
//     delete []term_mutex;
 //    delete []running;
}
vector<vector<string> > tofAnalysis::getHistogramNames()
{
  vector<vector<string> >tmp;
  if(!isInitA)return vector<vector<string> >();
  for(int i=0;i<numAlgorithms;i++)
    if(f[0][i]!=NULL)tmp.push_back(f[0][i]->histogramNames());
  return tmp;
}
TH1 *tofAnalysis::getHisto(const string &name)
{
  if(!isInitA)return NULL;
  TH1 *ht=NULL;
  for(int i=0;i<numAlgorithms;i++)
    if(f[0][i]!=NULL)
      {
	ht=f[0][i]->histogram(name);
	if(ht!=NULL)return ht;
      }
  return NULL;
}
vector<vector<string> > tofAnalysis::getTreeNames()
{
  vector<vector<string> >tmp;
  if(!isInitA)return vector<vector<string> >();
  for(int i=0;i<numAlgorithms;i++)
    if(f[0][i]!=NULL)tmp.push_back(f[0][i]->treeNames());
  return tmp;
}
TTree *tofAnalysis::getTree(const string &name)
{
  if(!isInitA)return NULL;
  TTree *ht=NULL;
  for(int i=0;i<numAlgorithms;i++)
    if(f[0][i]!=NULL)
      {
	ht=f[0][i]->tree(name);
	if(ht!=NULL)return ht;
      }
  return NULL;
}


bool tofAnalysis::process()
{	
  if(!Eparticles::IsInit())Eparticles::init();
  anaLog<<"copy run and beamtime parameter"<<endli;
  if(!isInitA) return false;
  if(!isInitD) return false;
  if(!isInitS) return false;
//   for(int i=0;i<nThreads;i++)
//     Event[i].setRunNumber(runs.back().getRunNumber());
  int ii=0;
  readInID=9;
  anaLog<<"go..."<<endli;
  numberOfEventsToAnalyse=INT_MAX;
  QTime timer;
  timer.start();
  count=0;
  tCount=0;
  run=true;
    //    pthread_t threads[nThreads];
  single=false;
  pthread_t threads[nThreads];
  readInValid=true;//dataInput->good();
  while(readInValid&&run)
    { 
      while(readInValid&&run)
	{
	  if(nThreads==1)
	    {
	      thread_run(NULL);
	    }
	  else
	    {
 	      for(int i=0;i<nThreads;i++)
 		{
		  // 		  pthread_mutex_unlock(&term_mutex[ii]);
 		  pthread_mutex_lock(&tcount_mutex);
 		  tCount=i;
 		  pthread_mutex_unlock(&tcount_mutex);
 		  pthread_create(&threads[i],NULL,eventThread,this);
		}
 	      for(int i=0;i<nThreads;i++)
	        pthread_join(threads[i],NULL);
 	    }
	}
      if(!(readInValid)) 
	{
	  anaLog<<"file finished at event: "<<count<<endli;
	  anaLog<<((int)(runs.size()))<<" still to read. ";
	  if(runs.size()!=0)
	    {
	      emit newRun(runs.back(),bts[runs.back().getParentNumber()]);
	      for(int i=0;i<nThreads;i++)
		Event[i].setRunNumber(runs.back().getRunNumber());
	      readInValid=true;
	      runs.pop_back();
	    }
	}
    }
//     if(nThreads>1)
//       for(int i=0;i<nThreads;i++)
// 	pthread_join(threads[i],NULL);
    emit analysisFinished(count);
    anaLog<<"analysis finished, taking "<<timer.elapsed()<<"ms: "<<((float)timer.elapsed())/ (ii+1)<<"ms/event"<<endli;
    return true;
}
bool tofAnalysis::step(int num)
{
    anaLog<<"single step: "<<num<<"steps"<<endli;
    if(!isInitA) return false;
    if(!isInitD) return false;
    if(!isInitS) return false;
    QTime timer,tim;
    timer.start();
    long times[numAlgorithms];
    for(int i=0;i<numAlgorithms;i++)
	times[i]=0;
    //    int i=0;
    if(run==true)
      count=0;
    tCount=0;
    pthread_t threads[nThreads];
    numberOfEventsToAnalyse=num;
    single=(num==1);
    //    int n;
    readInValid=true;//dataInput->good();
    run=true;
    while(count<num&&readInValid&&run)
    { 
      if(nThreads==1)
	{
	  thread_run(NULL);
	}
      else
	{
	  for(int i=0;i<nThreads;i++)
	    {
	      //	      pthread_mutex_unlock(&term_mutex[ii]);
	      pthread_mutex_lock(&tcount_mutex);
	      tCount=i;
	      pthread_mutex_unlock(&tcount_mutex);
	      pthread_create(&threads[i],NULL,eventThread,this);
	    }
	  for(int i=0;i<nThreads;i++)
	    pthread_join(threads[i],NULL);
	}
      while((!(readInValid))&&(runs.size()>0)&&run) 
	{
	  anaLog<<"file read ";
	  if(runs.size()!=0)
	    {
	      for(int ii=0;ii<nThreads;ii++)
		Event[ii].setRunNumber(runs.back().getRunNumber());
	      emit newRun(runs.back(),*runs.back().getParent());
	      anaLog<<"continue analysis"<<endli;
	      runs.pop_back();
	    }
	}
    }    
//     if(nThreads>1)
//       for(int i=0;i<nThreads;i++)
// 	pthread_join(threads[i],NULL);
    anaLog<<"analysis finished, taking "<<timer.elapsed()<<"ms: "<<((float)timer.elapsed())/ (num+1)<<"ms/event"<<endli;
    emit analysisFinished(count);
    return true;
}

void tofAnalysis::endStep()
{
    anaLog<<"kill all"<<endli;
    anaLog<<"delete algorithms"<<endli;
    emit finishStateChanged("finalize algorithms");
    killAlgorithms();
    emit finishStateChanged("finalize algorithms done");
    anaLog<<"close files "<<endli;
    if(rootFile!=NULL)
    {
      if((*rootFile)!=NULL){
	(*rootFile)->Close();
	delete (*rootFile);}
      delete rootFile;
      rootFile=NULL;
    }
    anaLog<<"delete data"<<endli;
    killData();
    anaLog<<"done"<<endli;
    killSetup();
}
void tofAnalysis::initStep(const vector<algorithm_parameter> &p, vector<beamTime_parameter> &beamParam, vector<run_parameter> & runParam,vector<detector_parameter> &dets, vector<material_parameter> &mats, reaction_parameter col)
{
  if(p.size()<4)showFrequency=10000;
  else showFrequency=1000;
  if(!Eparticles::IsInit())Eparticles::init();
    anaLog<<"begin complete init"<<incD<<endli;
    anaLog<<"copy run and beamtime parameter"<<endli;
    for(unsigned int i=0;i< runParam.size();i++)
    {
	run_parameter r=runParam[runParam.size()-1-i];
	runs.push_back(r);
    }
    for(unsigned int i=0;i<beamParam.size();i++)
    {
	beamTime_parameter b=beamParam[i];
	bts.push_back(b);
	for(unsigned int j=0;j< runParam.size();j++)
	  {
	    if(runs[j].getParent()==&beamParam[i])
	      {
		runs[j].setParent(&bts.back());
	      }
	  }
    }

//     rootFile=new TFile*;
//     (*rootFile)=0;
    anaLog<<"init"<<endli;
    emit initStateChanged("create setup");
    createSetup(dets,mats,col);
    emit initStateChanged("init data structures");
    initData();
    emit initStateChanged("init algorithms");
    initAlgorithms(p,runs.back());
    for(int i=0;i<nThreads;i++)
      Event[i].setRunNumber(runs.back().getRunNumber());
    emit initStateChanged((string("algorithms done \n emitting newRun-SIGNAL :")+runs.back().getName()).data());
    emit initStateChanged((string(" parent name:")+runs.back().getParent()->getName()).data());
    run_parameter rp(runs.back());
    emit newRun(rp,*runs.back().getParent());
    runs.pop_back();
    readInID=9;
    emit initStateChanged("init complete");
}
void tofAnalysis::addRun(run_parameter &rp)
{
  if(!isInitA) return;
  if(runs.size()>0||readInValid)
    {
      run_parameter r=rp;
      runs.push_back(r);
    }
  else
    {
      for(int ii=0;ii<nThreads;ii++)
	Event[ii].setRunNumber(rp.getRunNumber());
      emit newRun(rp,*rp.getParent());
    }
}
void tofAnalysis::removeRun(run_parameter &rp)
{
  if(!isInitA) return;
  for(unsigned int i=0;i<runs.size();i++)
    {
      if(rp.getName()==runs[i].getName())
	{
	  takeItemFromVector(runs,runs[i]);
	}
    }
}
int tofAnalysis::stopAnalysis()
{
  pthread_mutex_lock(&count_mutex);
  run=false;
  pthread_mutex_unlock(&count_mutex);
  return count;
}
void tofAnalysis::showNewRun(run_parameter &run)
{
  anaLog<<"emit newRun-Signal: "<<run<<endli;
  if(run.hasAdditionalCalibration())
    for(int i=0;i<run.getParent()->getNumberOfCalibrationFiles();i++)
      anaLog<<"C"<<run.getParent()->getCalibrationFile(i)<<endli;
  if(!run.hasOwnSetup())
    anaLog<<"S"<<run.getParent()->getSetupFile()<<endli;
}
void tofAnalysis::doRequestEvent(int eventNumber, int runNumber)
{
  emit requestEvent(eventNumber,runNumber);
}
void tofAnalysis::doRequestNextEvent()
{
  emit requestNextEvent();
}

void *tofAnalysis::thread_run(void *ptr)
{
  pthread_mutex_lock(&tcount_mutex);
  int j=tCount;
  pthread_mutex_lock(&count_mutex);
  int num;
  num=count++;
  pthread_mutex_unlock(&count_mutex);
  pthread_mutex_unlock(&tcount_mutex);
  while(num<numberOfEventsToAnalyse&&run)
    {
      //      if(num%1000==0)anaLog<<"Event "<<num<<endli;//" "<<Event[j].getTotalNumberOfHits()<<endli;
      //     anaLog<<"reset ";
      //     cout<<"Event: "<<num<<endl;
      if(callEventSearch)doRequestNextEvent();
      Event[j].reset();
      for(int i=0;i<numAlgorithms;i++)
	{
	  //anaLog<<f[j][i]->getName().data()<<" "<<num<<" "<<endli;
	  f[j][i]->process(0);
	  //cout<<f[j][i]->getName().data()<<endl;
	}
      //cout<<"Event:"<<Event[j].getEventNumber()<<endl;
      if((num+1)%showFrequency==0||single)
	{
	  emit newEvent((single?Event[j].getEventNumber():num));
	  emit changeEvent(&Event[j]);
// 	  cout<<"."<<flush;
// 	  n_n++;
// 	  if(n_n==50)
// 	    {
// 	      cout<<num+1;
// 	      n_n=0;
// 	    }
	}
      if(!(readInValid))return NULL;
      pthread_mutex_lock(&count_mutex);
      num=count++;
      pthread_mutex_unlock(&count_mutex);
    }
  return ptr;
}


vector<bool> tofAnalysis::ids=vector<bool>();
int tofAnalysis::currID=0;



// pthread_mutex_t tofAnalysis::count_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t tofAnalysis::tcount_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t tofAnalysis::input_mutex = PTHREAD_MUTEX_INITIALIZER;
