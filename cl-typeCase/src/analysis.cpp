#include "tofAnalysis_t.h"
#include <iostream>
#include <fstream>
#include<qdatetime.h>
#include "loggingClass.h"
#include "parameterManager.h"
#include <iostream>
#include <signal.h>
#include <qfile.h>
#include <stdlib.h>
#include <time.h>
#include <boost/program_options.hpp>
using namespace std;
namespace po = boost::program_options;
tofAnalysis *pointer=NULL;
extern bool existing(string filename);
void  INThandler(int sig);
void STOPhandler(int sig);
struct parameters_t
{
  int numberOfEvents;
  string setupFile;
  bool rootS;
  string algoFile;
  bool rootA;
  string dataFile;
  string runsFile;
  bool rootD;
  string matFile;
  bool rootM;
  int nThreads;
  bool completeDataInput;
  bool shiftEvent;
  int EventShift;
  string outpath;
  string runName;
  bool showInitState;
  bool local;
  string localDirectory;
  vector<string> runNames;
  bool paintIt;
  bool recalibration;
  string recalibrationFile;
  bool showParams;
};

int modAlgos(vector<algorithm_parameter> &ap, vector<run_parameter>&rp, parameters_t &param);
void writeAnalysisParameter(vector<algorithm_parameter> ap,vector<run_parameter> rp,vector<detector_parameter> dp,vector<material_parameter> mp,reaction_parameter react);
void setAllowedOptions(po::options_description &desc, parameters_t& param);
bool checkOptions(po::options_description &desc,po::variables_map &vam, parameters_t& param);

int main(int argc, char **argv)
{
  parameterManager::init();
  QDateTime now=QDateTime::currentDateTime();
  srandom(now.date().year()+now.date().day()+now.date().month()+now.time().hour()+now.time().minute()+now.time().second());
  //get file names
  parameters_t parameter;
  po::options_description description("Allowed options");
  po::variables_map vm;
  setAllowedOptions(description,parameter);
  po::store(po::parse_command_line(argc, argv, description), vm,true);
  if(existing("analysisInit.init")){
    ifstream configfile;configfile.open("analysisInit.init");
    po::store(po::parse_config_file(configfile, description), vm,true);
    configfile.close();
  }
  po::notify(vm);    
  if(!checkOptions(description, vm, parameter))return 1;
  vector<algorithm_parameter> aparams;
  vector<beamTime_parameter> bparams;
  vector<run_parameter> rparams;
  vector<detector_parameter> dparams;
  reaction_parameter react;
  vector<material_parameter> mparams;

  if(!parameter.rootD)
    {
      if(parameter.completeDataInput)
	{
	  vector<beamTime_parameter>bret;
	  vector<run_parameter>rret;
	  if(!parameter.showParams)cout<<"read data from file \""<<parameter.dataFile.data()<<"\""<<endl;
	  if(!parameterManager::readDataBasis_ascii(parameter.dataFile, bret, rret))
	    {
	      cerr<<"could not read data file:\""<<parameter.dataFile.data()<<"\""<<endl;
	      return 0;
	    }
	  bool hasT, hasH;
	  for(unsigned int j=0;j<parameter.runNames.size();j++)
	    {
	      for(unsigned int i=0;i<rret.size();i++)
		{
		  if(rret[i].getName()==parameter.runNames[j]||rret[i].getParent()->getName()==parameter.runNames[j])
		    {
		      if(rret[i].getParent()->getName()==parameter.runNames[j])
			{
			  if(rret[i].getNumberOfCalibrationFiles()<1)continue;
			  hasH=false;hasT=false;
			  for(int k=0;k<rret[i].getNumberOfFiles();k++)
			    {
			      if(rret[i].getFileType(k)==257)hasH=true;
			      if(rret[i].getFileType(k)==513)hasT=true;
			    }
			  if(!(hasH&&hasT))continue;
			}
		      rparams.push_back(rret[i]);
		      int num=-1;
		      for(unsigned int k=0;k<bparams.size();k++)
			if(bparams[k].getName()==rret[i].getParent()->getName())
			  num=k;
		      if(num<0)
			{
			  num=bparams.size();
			  beamTime_parameter tmp(*rret[i].getParent());
			  bparams.push_back(tmp);
			}
		      rparams.back().setParent(&bparams[num]);
		    }
		}
	    }
	  if(rparams.size()>0)
	    {
	      if(!parameter.showParams)
		for(unsigned int i=0;i<rparams.size();i++)
		  cout<<"use run "<<rparams[i].getName().data()<<" from beamtime "<<rparams[i].getParent()->getName().data()<<endl;
	      parameter.runName=rparams[0].getName();
// 	      for(int i=0;i<rparams[0].getNumberOfFiles();i++)
// 		{
// 		  if(rparams[0].getFileType(i)==0)
// 		    runName=rparams[0].getFile(i);
// 		}
	    }
	  else 
	    {
	      cout<<"Warning: no run with this name found: \""<<parameter.runName.data()<<"\""<<endl;
	      return 0;
	    }
	  if(rparams.size()>20)
	    cout<<rparams.size()<<" runs in total"<<endl;
	}
      else //if(parameter.completeDataInput)
	{
	  vector<beamTime_parameter>bret;
	  vector<run_parameter>rret;
	  if(!parameterManager::readDataBasis_ascii(parameter.dataFile, bret, rret,2))
	    {
	      cerr<<"could not read data file:\""<<parameter.dataFile.data()<<"\""<<endl;
	      return 0;
	    }
 	}
    }
  //Read Setup from File
  if(rparams[0].hasOwnSetup())
    parameter.setupFile=rparams[0].getSetupFile();
  else
    parameter.setupFile=rparams[0].getParent()->getSetupFile();
  cout<<"read setup file \""<<parameter.setupFile.data()<<"\""<<existing(parameter.setupFile)<<endl;
  if(!parameter.rootS)
    if(!parameterManager::readDetectorParameter_ascii(parameter.setupFile, dparams, react))
      {
	cerr<<"could not read setup file:\""<<parameter.setupFile.data()<<"\""<<endl;
	return 0;
      }
  cout<<"read setup file \""<<parameter.matFile.data()<<"\""<<existing(parameter.matFile)<<endl;
  if(!parameter.rootM)
    if(!parameterManager::readMaterialParameter_ascii(parameter.matFile, mparams))
      {
	cerr<<"could not read material file:\""<<parameter.matFile.data()<<"\""<<endl;
	return 0;
      }
  if(!parameter.showParams)cout<<dparams.size()<<" detectors read from file "<<parameter.setupFile.data()<<endl;
  if(!parameter.showParams)cout<<mparams.size()<<" materials read"<<endl;
  if(!parameter.showParams)cout<<"read algorithms from file: \""<<parameter.algoFile.data()<<"\""<<endl;
  //Read algorithms from file
  if(!parameter.rootA)
    if(!parameterManager::readUsedAlgorithmParameter_ascii(parameter.algoFile, aparams))
      {
	cerr<<"could not read algorithm file:\""<<parameter.algoFile.data()<<"\""<<endl;
	return 0;
      }
  if(!parameter.showParams)cout<<aparams.size()<<" algorithms read "<<endl;
  //modify analysis-parameter according to command-line-options
  int nE=modAlgos(aparams, rparams, parameter);//completeDataInput, shiftEvent, EventShift, runName, outpath,local,bPar,sPar);
  if(parameter.showParams)
    {
      for(unsigned int i=0;i<aparams.size();i++)
	{
	  if(!aparams[i].IsUsed())continue;
	  cout<<aparams[i].getName().data();
	  cout<<" "<<aparams[i].getNumberOfParam<algorithm_parameter>()<<endl;
	  for(int j=0;j<aparams[i].getNumberOfParam<algorithm_parameter>();j++)
	    cout<<"  "<<aparams[i].getParam<algorithm_parameter>(j).getName().data()<<endl;
	  if(aparams[i].getID()==34)
	    cout<<"  to file:"<<aparams[i].getParam<string>(1).getData().data()<<endl;
	  cout<<"------------"<<endl;
	}
    }
  cout<<QTime::currentTime().toString().data()<<" "<<getenv("HOSTNAME")<<endl;
  //start analysis;
  loggingClass logg(string("/dev/shm/")+getenv("USER")+"/");
  logg.setData(aparams[aparams.size()-1].getParam<string>(1).getData());
  if(parameter.shiftEvent)
    logg.setStartingEvent(parameter.EventShift);
  if(nE==-1||rparams.size()>1)logg.setEventsToAnalyse(parameter.numberOfEvents);
  else logg.setEventsToAnalyse(nE);
  tofAnalysis analyser(200,30,1);//nThreads);
  pointer=&analyser;
  QObject::connect(&analyser,SIGNAL(newRun(run_parameter&, beamTime_parameter&)),&logg,SLOT(newRun(run_parameter&)));
  QObject::connect(&analyser,SIGNAL(analysisFinished(int)),&logg,SLOT(finished(int)));
  QObject::connect(&analyser,SIGNAL(newEvent(int)),&logg,SLOT(eventsDone(int)));
  //  cout<<(paintIt?"paint":"no paint")<<endl;
  if(parameter.paintIt)
    QObject::connect(&analyser,SIGNAL(changeEvent(TEvent*)),&logg,SLOT(eventsDone(TEvent*)));
  if(parameter.showInitState)
    {
      QObject::connect(&analyser,SIGNAL(initStateChanged(const char *)),&logg,SLOT(onInitState(const char *)));
      QObject::connect(&analyser,SIGNAL(finishStateChanged(const char *)),&logg,SLOT(onCleanUpState(const char *)));
    }
  QTime timer;
  timer.start();
  writeAnalysisParameter(aparams,rparams,dparams,mparams,react);
  analyser.initStep(aparams,bparams,rparams,dparams,mparams,react);
  signal(SIGINT, INThandler);
  if(parameter.numberOfEvents==-1)
    {
      analyser.process();
      cout<<"Time needed: "<<timer.elapsed()/1000.0<<" s"<<endl;
    }
  else
    {
      char c=' ';
      while (c!='q')
	{
	  if(parameter.paintIt)
	    {
	      for(int i=0;i<parameter.numberOfEvents;i++)
		analyser.step(1);
	      if(parameter.numberOfEvents>1)
		c='q';
	      else
		cin.get(c);
	    }
	  else
	    {
	      analyser.step(parameter.numberOfEvents);
	      if(parameter.numberOfEvents>1)
		c='q';
	      else
		cin.get(c);
	    }
	  cout<<"Time needed: "<<timer.elapsed()/1000.0<<" s"<<endl;
	}
      cout<<QTime::currentTime().toString().data()<<endl;
      signal(SIGINT,SIG_DFL);
      analyser.endStep();
    }
}

int modAlgos(vector<algorithm_parameter> &ap, vector<run_parameter>&rp, parameters_t &param)//bool cdi, bool sE, int eS, string rN, string oP,bool localDir, bool *bp, string* sp)
{
  int ret=-1;
  if(rp.size()==0)return -1;
  cout<<"filePattern:"<<param.outpath.data()<<endl;
  int trackoutCount=0;
  int count=0;
  for(unsigned int i=0;i<ap.size();i++)if(ap[i].getID()==63)count++;
  for(unsigned int i=0;i<ap.size();i++)
    {
      if (ap[i].getID()==9)
	{
	  if(param.completeDataInput)
	    {
	      for(int j=0;j<rp[0].getNumberOfFiles();j++)
		if(rp[0].getFileType(j)==0)
		  ap[i].changeParam<string>(string("input file"),rp[0].getFile(j));
	    }
	}
      else if(ap[i].getID()==40)//read from simple Tree
	{
	  ret=0;
	  int bas=0;
	  if(ap[i].getParam<bool>(3).getData())bas=6;
	  else if(ap[i].getParam<bool>(0).getData())bas=34;
	  for(unsigned int k=0;k<rp.size();k++)
	    {
	      for(int j=0;j<rp[k].getNumberOfFiles();j++)
		if(rp[k].getFileType(j)==bas)
		  ret+=rp[k].getFileEvents(j);
	    }
	  string fn;
	  for(int j=0;j<rp[0].getNumberOfFiles();j++)
	    if(rp[0].getFileType(j)==6)
	      fn=rp[0].getFile(j);
	  ap[i].changeParam<string>(ap[i].getParam<string>(5).getName(),fn);
	  for(int j=0;j<rp[0].getNumberOfFiles();j++)
	    if(rp[0].getFileType(j)==34)
	      fn=rp[0].getFile(j);
	  ap[i].changeParam<string>(ap[i].getParam<string>(2).getName(),fn);
	  ap[i].changeParam<bool>(ap[i].getParam<bool>(6).getName(),param.local);
	  if(param.shiftEvent)
	    {
	      ap[i].changeParam<bool>(ap[i].getParam<bool>(5).getName(),true);
	      ap[i].changeParam<int>(ap[i].getParam<int>(0).getName(),param.EventShift);
	    }
	}
      else if(ap[i].getID()==34)
	{
	  if(param.outpath!="")
	    ap[i].changeParam<string>(string("Output file"),param.outpath+remove(rp[0].getName(),' '));
	  else
	    {
	      int n=-1;
	      for(int j=0;j<ap[i].getNumberOfParam<string>();j++)
		if(ap[i].getParam<string>(j).getName()=="Output file")
		  n=j;
	      if(n==-1)continue;
	      ap[i].changeParam<string>(string("Output file"),remove(ap[i].getParam<string>(n).getData()+rp[0].getName(),' '));
	    }
	  ap[i].changeParam<bool>(ap[i].getParam<bool>(8).getName(),param.local);
	}
      else if(ap[i].getID()==54)
	{
	  if(param.outpath!="")
	    {
	      if(ap[i].getParam<bool>(0).getData())
		ap[i].changeParam<string>(string("File-pattern"),param.outpath);
	      else
		ap[i].changeParam<string>(string("File-pattern"),param.outpath+remove(rp[0].getName(),' '));
	    }
	  else
	    {
	      int n=-1;
	      for(int j=0;j<ap[i].getNumberOfParam<string>();j++)
		if(ap[i].getParam<string>(j).getName()=="File-pattern")
		  n=j;
	      if(n==-1)continue;
	      if(ap[i].getParam<bool>(0).getData())
		ap[i].changeParam<string>(string("File-pattern"),ap[i].getParam<string>(n).getData());
	      else
		ap[i].changeParam<string>(string("File-pattern"),ap[i].getParam<string>(n).getData()+remove(rp[0].getName(),' '));
	    }
	  ap[i].changeParam<bool>(ap[i].getParam<bool>(1).getName(),param.local);
	}
      else if(ap[i].getID()==57)
	{
	  ap[i].changeParam<bool>(string("read from file"),false);
	  if(param.recalibration)
	    if(existing(param.recalibrationFile))
	      {
		ap[i].changeParam<bool>(string("read from file"),param.recalibration);
		ap[i].changeParam<string>(string("filename"),param.recalibrationFile);
	      }
	}
      else if(ap[i].getID()==61)//hit tree output
	{
 	  ap[i].changeParam<bool>("Use local directory",param.local);
	  if(param.localDirectory!="")
	    ap[i].changeParam<string>("local directory",param.localDirectory);
 	  if(param.outpath!="")
 	    {
//  	      if(rp.size()==1&&!hasA(param.runName,"fifo")&&!hasA(param.runName,"evt"))
//  		ap[i].changeParam<string>(string("file path"),param.outpath+remove(param.runName,' '));
//  	      else
		ap[i].changeParam<string>(string("file path"),param.outpath+remove(rp[0].getName(),' '));
 	    }
	}
      else if(ap[i].getID()==63)//track tree output
	{
	  ap[i].changeParam<bool>("Use local directory",param.local);
	  if(param.localDirectory!="")
	    ap[i].changeParam<string>("local directory",param.localDirectory);
 	  if(param.outpath!="")
 	    {
//  	      if(rp.size()==1&&!hasA(sp[4],"fifo"))
//  		ap[i].changeParam<string>(string("file path"),param.outpath+remove(param.runName,' '));
//  	      else
	      string pattern=param.outpath;
	      if(count>1)
		switch(trackoutCount)
		  {
		  case 0:pattern=pattern+"prompt_";break;
		  case 1:pattern=pattern+"vee_";break;
		  case 2:pattern=pattern+string_number(trackoutCount);break;
		  case 3:pattern=pattern+string_number(trackoutCount);break;
		  case 4:pattern=pattern+string_number(trackoutCount);break;
		  }
	      ap[i].changeParam<string>(string("file path"),pattern+remove(rp[0].getName(),' '));
	      trackoutCount++;
 	    }
	}
      else if(ap[i].getID()==60)//hit tree input
	{
	  ap[i].changeParam<bool>("Use local directory",param.local);
	  if(param.localDirectory!="")
	    ap[i].changeParam<string>("local directory",param.localDirectory);
	}
      else if(ap[i].getID()==62||ap[i].getID()==60)//track tree input
	{
	  ap[i].changeParam<bool>("Use local directory",param.local);
	  if(param.localDirectory!="")
	    ap[i].changeParam<string>("local directory",param.localDirectory);
	}
    }
  return ret;
}
#include <TCanvas.h>
#include <eventpainter.h>
loggingClass::loggingClass(string fn):painter()
{
  computerID=string(getenv("HOSTNAME"));
  if(fn!="")filename=fn;
  filename=filename+computerID+string_number(getpid())+".log";
  startingDate=QDate::currentDate();
  timer.start();
  ofstream out;
  dataAnalysed="";
  startingEvent=0;
  eventsTo=100000;
  out.open(filename.data());
  out<<"logging startet on computer: "<<computerID.data()<<endl;
  out<<"time:"<<timer.toString().data()<<endl;
  out<<"data to analyse: "<<dataAnalysed.data()<<endl;
  out.close();
  canvas=new TCanvas();
  canvas->Print("eventPattern.ps[");
  painter.setTrackLike(false);
  painter.setCanvas(canvas);
  painter.addDetector(21,5);
  painter.addDetector(22,5);
  for(unsigned int i=0;i<8;i++)
    painter.addDetector(i,(i%6)+1);
  painter.addDetector(19,4);
  painter.addDetector(20,4);
  painter.addDetector(17,3);
  painter.addDetector(18,3);
  painter.addDetector(23,3);
  painter.addDetector(15,4);
  painter.addDetector(16,4);
  painter.addPixel(0,2);
  painter.addPixel(1,2);
  painter.addPixel(2,2);
  //  painter.addPixel(3,3);
  //  painter.addPixel(4,4);
  //  painter.addPixel(5,2);
  for(int i=0;i<5;i++)
    painter.addTrackColor(1);
}
loggingClass::~loggingClass()
{
  ofstream out;
  out.open(filename.data());
  double taken=((double)timer.elapsed())/1000;
  if(timer.secsTo(QTime::currentTime())>=0)
    taken+=(double)abs(startingDate.daysTo(QDate::currentDate()))*86400;
  else
    taken+=(double)(abs(startingDate.daysTo(QDate::currentDate()))-1)*86400;
  out<<"logging on computer: "<<computerID.data()<<endl;
  out<<"start time:"<<timer.toString().data()<<endl;
  out<<"analysis taken: "<<taken<<endl;
  out<<"events analysed: "<<actuallyDone<<endl;
  out<<"events per second: "<<((double)actuallyDone)/taken<<endl;
  out<<"data to analyse: "<<dataAnalysed.data()<<endl;
  out<<"starting event: "<<startingEvent<<endl;
  out<<"events to analyse: "<<actuallyDone<<endl;
  out.close();
  canvas->Print("eventPattern.ps]");
  delete canvas;
}
void loggingClass::eventsDone(TEvent *e)
{
  //  cout<<e<<endl;
  if(e==NULL)return;
  if(e->getNumberOfTracks()>0)
    {
      painter.rePaint(e);
      canvas->Print("eventPattern.ps");
    }
}
void loggingClass::eventsDone(int num)
{
  ofstream out;
  out.open(filename.data());
  double taken=((double)timer.elapsed())/1000;
  if(timer.secsTo(QTime::currentTime())>=0)
    taken+=(double)abs(startingDate.daysTo(QDate::currentDate()))*86400;
  else
    taken+=(double)(abs(startingDate.daysTo(QDate::currentDate()))-1)*86400;
  out<<"logging on computer: "<<computerID.data()<<endl;
  out<<"start time:"<<timer.toString().data()<<endl;
  out<<"analysis taken: "<<taken<<endl;
  out<<"events analysed: "<<num<<endl;
  out<<"events per second: "<<((double)num)/taken<<endl;
  out<<"data to analyse: "<<dataAnalysed.data()<<endl;
  out<<"starting event: "<<startingEvent<<endl;
  out<<"events to analyse: "<<eventsTo<<endl;
  out.close();
  actuallyDone=num;
}
void loggingClass::finished(int num)
{
  ofstream out;
  out.open(filename.data());
  double taken=((double)timer.elapsed())/1000;
  if(timer.secsTo(QTime::currentTime())>=0)
    taken+=(double)abs(startingDate.daysTo(QDate::currentDate()))*86400;
  else
    taken+=(double)(abs(startingDate.daysTo(QDate::currentDate()))-1)*86400;
  out<<"logging on computer: "<<computerID.data()<<endl;
  out<<"start time:"<<timer.toString().data()<<endl;
  out<<"analysis taken: "<<taken<<endl;
  out<<"events analysed: "<<num<<endl;
  out<<"events per second: "<<((double)num)/taken<<endl;
  out<<"data to analyse: "<<dataAnalysed.data()<<endl;
  out<<"starting event: "<<startingEvent<<endl;
  out<<"events to analyse: "<<num<<endl;
  out.close();
  actuallyDone=num;
}
void loggingClass::setData(string d)
{
  dataAnalysed=d;
}
void loggingClass::newRun(run_parameter &r)
{
  dataAnalysed=r.getName();
  cout<<"new run: \""<<r.getName().data()<<"\""<<endl;
}
void loggingClass::setStartingEvent(int value)
{
  startingEvent=value;
}
void loggingClass::setEventsToAnalyse(int value)
{
  eventsTo=value;
}  
void loggingClass::onInitState(const char *state)
{
  cout<<state<<endl;
}
void loggingClass::onCleanUpState(const char *state)
{
  cout<<state<<endl;
}

void  INThandler(int sig) {
  char  c;
  c=0;
  signal(sig, SIG_IGN);
  cerr << "OUCH, did you hit Ctrl-C?\n"
       << "Do you really want to quit? [y/n]" << endl;
  c = getchar();
  if (c == 'y' || c == 'Y') 
    {
      signal(SIGINT,SIG_DFL);
      if(pointer!=NULL)
	pointer->endStep();
      exit(0);
    }
  else
    signal(SIGINT, INThandler);
}  

void STOPhandler(int sig)
{
  signal(sig,SIG_IGN);
  cerr << "Stoptest";
  signal(SIGSTOP,STOPhandler);
}
void writeAnalysisParameter(vector<algorithm_parameter> ap,vector<run_parameter> rp,vector<detector_parameter> dp,vector<material_parameter> mp,reaction_parameter react)
{
  ofstream out;
  string name="analysis_";
  name+=string_number(getpid())+".log";
  out.open(name.data());
  time_t tme=time(NULL);
  tm loc=*localtime(&tme);
  out<<"############# ANALYSIS: "<<loc.tm_mday<<"."<<loc.tm_mon+1<<"."<<1900+loc.tm_year<<" ##################"<<endl;
  out<<"############# DATA-BASIS ###########################"<<endl;
  for(unsigned int i=0;i<rp.size();i++)out<<rp[i]<<endl;
  out<<"############# SETUP ###########################"<<endl;
  out<<react<<endl;
  for(unsigned int i=0;i<dp.size();i++)out<<dp[i]<<endl;
  out<<"############# MATERIALS ###########################"<<endl;
  for(unsigned int i=0;i<mp.size();i++)out<<mp[i]<<endl;
  out<<"############# ALGORITHMS ###########################"<<endl;
  for(unsigned int i=0;i<ap.size();i++)out<<ap[i]<<endl;
  out.close();
}
/**** Command line parameters ***********************************************/
void setAllowedOptions(po::options_description &desc, parameters_t& param)
{
  desc.add_options()
    ("help","produce help message")
    ("n",po::value<string>(),"set the number of events to analyse")
    ("a",po::value<string>(),"specify a file which contains the algorithms to use and their parameters, ascii version")
    //    ("s",po::value<string>(),"specify a setup file, ascii version")
    ("m",po::value<string>(),"specify a file containing the used materials, ascii version")
    ("d",po::value<string>(),"specify a file which Contains the description of the data and their calibration, ascii version")
    //    ("A",po::value<string>(),"specify a file which contains the algorithms to use and their parameters, ROOT version")
    //    ("S",po::value<string>(),"specify a setup file, ROOT version")
    //    ("M",po::value<string>(),"specify a file containing the used materials, ROOT version")
    //    ("D",po::value<string>(),"specify a file which Contains the description of the data and their calibration, ROOT version")
    ("r",po::value<vector<string> >(),"select single run(s) out of data basis, data basis will them be read with typeCase format. If it is the name of a beam-time, all runs having both filetypes (257, 513) will be used")
    ("runs",po::value<string>(),"specify a file with names of runs to analyse")
    ("o",po::value<string>(),"output path. This parameter is passed to output-algorithms that allow this feature.")
    ("j",po::value<int>(),"the number of threads to use (default 1) Note: do not use more threads, than your machine has processors, there my be problems else")
    ("l",po::value<int>(),"begin with event number other than 0. This is only possible if input-algorithms with this feature are used.")
    ("local","analysis on local directory")
    ("local-directory",po::value<string>(),"directory to use for local copy of in- and output-files")
    ("nonLocal","no local directory")
    ("paint","do printout of event pattern")
    ("H","show more information about analysis")
    ("I","show the current init state of the analysis")
    ("recalibrate",po::value<string>(),"do recalibration with calibration root file ARG")
    ;
  param.numberOfEvents=1;
  param.completeDataInput=false;
  param.shiftEvent=false;
  param.EventShift=0;
  param.outpath="";
  param.runName="";
  param.nThreads=1;
  param.showInitState=false;
  param.local=true;
  param.localDirectory="";
  param.paintIt=false;
  param.showParams=false;
}
bool checkOptions(po::options_description &desc, po::variables_map &vam, parameters_t& param)
{
  if (vam.count("help")) {
    cout<<"command-line version of analysis program:"<<endl;
    cout<<"             typeCase"<<endl;
    cout<<"___________________________________________"<<endl;
    cout<<"use:\n"<<endl;
    cout << desc << "\n";
    return false;
  }
  bool show=false,showA=false;
  if(vam.count("s")){param.setupFile=vam["s"].as<string>();param.rootS=false;}
  if(vam.count("S")){param.setupFile=vam["S"].as<string>();param.rootS=true;}
  if(vam.count("a")){param.algoFile=vam["a"].as<string>();param.rootA=false;}
  if(vam.count("A")){param.algoFile=vam["A"].as<string>();param.rootA=true;}
  if(vam.count("m")){param.matFile=vam["m"].as<string>();param.rootM=false;}
  if(vam.count("M")){param.matFile=vam["M"].as<string>();param.rootM=true;}
  if(vam.count("d")){param.dataFile=vam["d"].as<string>();param.rootD=false;}
  if(vam.count("D")){param.dataFile=vam["D"].as<string>();param.rootD=true;}
  if(vam.count("n")){
    string s=vam["n"].as<string>();
    if(s.find("k")<s.npos||s.find("M")<s.npos)
      {
	if(s.find("k")<s.npos)
	  param.numberOfEvents=toInt(s.substr(0,s.find("k")))*1000;
	else
	  param.numberOfEvents=toInt(s.substr(0,s.find("M")))*1000000;
      }
    else
      param.numberOfEvents=toInt(s);
  }
  if(vam.count("j"))param.nThreads=vam["j"].as<int>();
  if(vam.count("l")){param.shiftEvent=true;param.EventShift=vam["l"].as<int>();}
  if(vam.count("o")){param.outpath=vam["o"].as<string>();}
  if(vam.count("H")){show=true;showA=true;}
  if(vam.count("I"))param.showInitState=true;
  if(vam.count("local"))param.local=true;
  if(vam.count("nonLocal"))param.local=false;
  if(vam.count("local-directory")){param.local=true;param.localDirectory=vam["local-directory"].as<string>();}
  if(vam.count("paint"))param.paintIt=true;
  if(vam.count("runs"))param.runsFile=vam["runs"].as<string>();
  if(vam.count("recalibrate")){
    param.recalibration=true;
    param.recalibrationFile=vam["recalibrate"].as<string>();
    cout<<"recalibrate: "<<param.recalibrationFile.data()<<endl;
  }
  if(vam.count("r"))
    {
      param.completeDataInput=true;
      for(unsigned int i=0;i<vam["r"].as<vector<string> >().size();i++)
	param.runNames.push_back(vam["r"].as<vector<string> >().at(i));
    }
  param.rootS=((int)param.setupFile.find(".root")==(int)param.setupFile.length()-5);
  param.rootA=((int)param.algoFile.find(".root")==(int)param.algoFile.length()-5);
  param.rootD=((int)param.dataFile.find(".root")==(int)param.dataFile.length()-5);
  param.rootM=((int)param.matFile.find(".root")==(int)param.matFile.length()-5);
  if(!existing(param.algoFile))
    if(existing(string(getenv("KTOFPACK"))+"/data/algorithms/"+param.algoFile))
      param.algoFile=string(getenv("KTOFPACK"))+"/data/algorithms/"+param.algoFile;
  if(!existing(param.dataFile))
    if(existing(string(getenv("KTOFPACK"))+"/data/data/"+param.dataFile))
      param.dataFile=string(getenv("KTOFPACK"))+"/data/data/"+param.dataFile;
  if(!existing(param.setupFile))
    if(existing(string(getenv("KTOFPACK"))+"/data/detectors/"+param.setupFile))
      param.setupFile=string(getenv("KTOFPACK"))+"/data/detectors/"+param.setupFile;
  if(!existing(param.matFile))
    if(existing(string(getenv("KTOFPACK"))+"/data/detectors/"+param.matFile))
      param.matFile=string(getenv("KTOFPACK"))+"/data/detectors/"+param.matFile;
  if(!existing(param.algoFile)){cout<<"algorithms file: \""<<param.algoFile.data()<<"\" doesn\'t exist."<<endl;return false;}
  if(!existing(param.setupFile)){cout<<"setup file: \""<<param.setupFile.data()<<"\" doesn\'t exist."<<endl;return false;}
  if(!existing(param.dataFile)){cout<<"data-basis file: \""<<param.dataFile.data()<<"\" doesn\'t exist."<<endl;return false;}
  if(!existing(param.matFile)){cout<<"material file: \""<<param.matFile.data()<<"\" doesn\'t exist."<<endl;return false;}
  if(param.runsFile!=""&&existing(param.runsFile))
    {
      ifstream runin;runin.open(param.runsFile.data());
      string s;char c;
      while(!runin.eof())
	{
	  s="";runin.get(c);while(c!='\n'&&!runin.eof()){s=s+c;runin.get(c);}
	  if(s!="")param.runNames.push_back(s);
	}
      runin.close();
    }
  if(show)
    {
      cout<<"setup file: "<<param.setupFile.data()<<endl;
      cout<<"algorithm file: "<<param.algoFile.data()<<endl;
      cout<<"databasis file: "<<param.dataFile.data()<<endl;
      cout<<"material file: "<<param.matFile.data()<<endl;
      if(param.runNames.size()>0)
	cout<<"run name: "<<param.runNames[0].data()<<endl;
      cout<<"output-path: "<<param.outpath.data()<<endl;
      cout<<"number of Events: "<<param.numberOfEvents<<endl;
      cout<<"start at event "<<param.EventShift<<" "<<(param.shiftEvent?"on":"off")<<endl;
      cout<<"use "<<param.nThreads<<" Threads"<<endl;
    }
  if(!param.showParams)cout<<"Do analysis with "<<param.nThreads<<" thread(s). For "<<param.numberOfEvents<<" events"<<endl;
  return true;
}
