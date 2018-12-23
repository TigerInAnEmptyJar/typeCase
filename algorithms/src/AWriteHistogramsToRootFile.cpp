#include "AWriteHistogramsToRootFile.h"
#include <TCanvas.h>
//#include <qfile.h>
#include <QtCore/QDateTime>
#include "logger.h"
extern bool existing(string filename);
AWriteHistogramsToRootFile::AWriteHistogramsToRootFile(const string &filename, const string &directory, TEvent &eventIn,TSetup &setup,const string &option,bool serv,void *histo_mutexIn):AAlgorithm("Write histograms to root file"),event(eventIn),server(serv)
{
  histo_mutex=histo_mutexIn;
  numberOfDetectors=setup.getNumberOfDetectors();
  if(server)
    {
      string s=directory;
      rootFile=new TFile*;
      if(existing(filename))
	{
	  if(s=="")
	    {
	      (*rootFile)=new TFile(filename.data(),"RECREATE");
	      currentDirectory=(*rootFile);
	    }
	  else
	    {
	      (*rootFile)=new TFile(filename.data(),"UPDATE");
	      currentDirectory=(TDirectory*)(*rootFile)->Get(s.data());
	      int i=0;
	      while(currentDirectory!=NULL)
		{
		  i++;
		  s=directory+string_number(i);
		  currentDirectory=(TDirectory*)(*rootFile)->Get(s.data());
		}
	      if(i>0)s=directory+string_number(i);
	      if(!currentDirectory)
		currentDirectory=(*rootFile)->mkdir(s.data());
	    }
	}
      else
	{
	  (*rootFile)=new TFile(filename.data(),"RECREATE");
	  if(directory==""){
	    currentDirectory=(*rootFile);
	  }
	  else
	    {
	      currentDirectory=(*rootFile)->mkdir(s.data());
	    }
	}
    }
  defineData(event);
  cout<<option.data()<<endl;
  declareHistos(setup,option);
  own=true;
}

AWriteHistogramsToRootFile::AWriteHistogramsToRootFile(TFile **rootFileIn, const string &directory, TEvent &eventIn,TSetup &setup,const string &option,bool serv,void *histo_mutexIn):AAlgorithm("Write histograms to root file"),event(eventIn),server(serv)
{
  histo_mutex=histo_mutexIn;
  numberOfDetectors=setup.getNumberOfDetectors();
  if(server)
    {
      rootFile=rootFileIn;
      string s=directory;
      if(s=="")
	{
	  int i=0;
	  currentDirectory=(TDirectory*)(*rootFile)->Get((directory+string_number(i)).data());
	  while(currentDirectory)
	    {
	      currentDirectory=(TDirectory*)(*rootFile)->Get((directory+string_number(i)).data());
	      i++;
	    }
	  s=directory+string_number(i);
	  if(!currentDirectory)
	    currentDirectory=(*rootFile)->mkdir(s.data());
	}
      else
	{
	  currentDirectory=(TDirectory*)(*rootFile)->Get(s.data());
	  int i=0;
	  while(currentDirectory)
	    {
	      i++;
	      currentDirectory=(TDirectory*)(*rootFile)->Get((s+string_number(i)).data());
	    }
	  if(i)s=directory+string_number(i);
	  if(!currentDirectory)
	    currentDirectory=(*rootFile)->mkdir(s.data());
	}
    }
  defineData(event);
  declareHistos(setup,option);
}

AWriteHistogramsToRootFile::~AWriteHistogramsToRootFile()
{
  if(server)
    {
      for(int i=0;i<numF;i++)
	{
	  anaLog<<i<<"...";
	  f[i]->write();
	  anaLog<<"done ";
	}
    }
    (*rootFile)->Close();
  for(int i=0;i<numF;i++)
    {
      anaLog<<"kill "<<f[i]->getName();
      delete f[i];
      //      anaLog<<" done"<<endli;
    }
  if(server)
    {
      delete []f;
      delete []histos;
      delete []TDhistos;
    }
  delete [] numberOfHits;
  for(int i=0;i<numberOfDetectors;i++)
    {
      delete [] hits[i];
      delete [] raws[i];
    }
  delete [] hits;
  delete [] raws;
  delete [] numberOfPixels;
  for(int i=0;i<numberOfDetectors;i++)
    delete [] pixels[i];
  delete [] pixels;
  delete [] tracks;
}
void AWriteHistogramsToRootFile::declareHistos(TSetup &setup,const string &option)
{
  cout<<option.data()<<endl;
  vector<string> lis;
  string tmp,s;
  s=option;
  while(s.find("__")>s.npos)
    {
       int len=s.find("__",5);
      if(len==-1)
	{
	  tmp=s;
	  s="";
	}
      else
	{
	  tmp=s.substr(0,len+1); 
	  s=s.substr(len+1,s.length()-len-1);
	}
      lis.push_back(tmp);
    }
  numF=lis.size();
  f=new AHistoAlgorithm*[numF];
  for(int i=0;i<numF;i++)
    {
      cout<<lis[i].data()<<endl;
      if(lis[i].substr(2,2)=="tr")
	{
	  f[i]=new ATrackHistoAlgorithm(tracks, *numberOfTracks,currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	}
      else if(lis[i].substr(2,2)=="cr")
	{
	  f[i]=new ARawHitCutHistoAlgorithm(hits,numberOfHits,setup,currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	}
      else if(lis[i].substr(2,2)=="rh")
	{
	  f[i]=new ARawHitHistoAlgorithm(raws,numberOfHits,setup,currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	}
      else if(lis[i].substr(2,2)=="ch")
	{
	  cout<<"declare calibHistos "<<lis[i].substr(5,lis[i].length()-5).data()<<endl;
	  f[i]=new ACalibHitHistoAlgorithm(hits,numberOfHits,setup,currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	  cout<<"done"<<endl;
	}
      else if(lis[i].substr(2,2)=="pi")
	{
	  f[i]=new APixelHistoAlgorithm(pixels,numberOfPixels,setup.getNumberOfDetectors(),currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	}
      else if(lis[i].substr(2,2)=="re")
	{
	  f[i]=new AReactionHistoAlgorithm(&event,currentDirectory,lis[i].substr(5,lis[i].length()-5),!server);
	}
      //      else 
    }
  numberOfHistos=0;
  numberOf2DHistos=0;
  for(int i=0;i<numF;i++)
    {
      numberOfHistos+=f[i]->getNumberOfHistos(1);
      numberOf2DHistos+=f[i]->getNumberOfHistos(2);
    }
  if(server)
    {
      histos=new TH1*[numberOfHistos];
      for(int i=0;i<numberOfHistos;i++)
	histos[i]=NULL;
      TDhistos=new TH2*[numberOf2DHistos];
      for(int i=0;i<numberOf2DHistos;i++)
	TDhistos[i]=NULL;
      int s1=0,s2=0;
      for(int i=0;i<numF;i++)
	{
	  s1+=f[i]->declareHistos(histos,s1);
	  s2+=f[i]->declare2DHistos(TDhistos,s2);
	}
    }
  return;
}

void *AWriteHistogramsToRootFile::process(void*ptr)
{
  //  pthread_mutex_lock((pthread_mutex_t*)histo_mutex);
  for(int i=0;i<numF;i++)
    {
      f[i]->process(0);
    }
  //  pthread_mutex_unlock((pthread_mutex_t*)histo_mutex);
  return ptr;
}

void AWriteHistogramsToRootFile::defineData(TEvent& ev)
{
  numberOfHits=new int*[numberOfDetectors];
  for(int j=0;j<numberOfDetectors;j++)	    
    numberOfHits[j]=&ev.getNumberOfHits(j);
  int maxHit=ev.getMaxNumber<TCalibHit>();
  int maxPixel=ev.getMaxNumber<TPixel>();
  int maxTrack=100;
  hits=new TCalibHit**[numberOfDetectors];
  raws=new TRawHit**[numberOfDetectors];
  for(int i=0;i<numberOfDetectors;i++)
    {
      hits[i]=new TCalibHit*[maxHit];
      raws[i]=new TRawHit*[maxHit];
    }
  for(int j=0;j<numberOfDetectors;j++)	    
    for(int i=0;i<maxHit;i++)
      {
	hits[j][i]=&event.getHit(j,i);
	raws[j][i]=&hits[j][i]->getRaw();
      }
  pixels=new TPixel**[numberOfDetectors];
  numberOfPixels=new int*[numberOfDetectors];
  for(int j=0;j<numberOfDetectors;j++)	    
    numberOfPixels[j]=&ev.getNumberOfPixels(j);
  for(int i=0;i<numberOfDetectors;i++)
    pixels[i]=new TPixel*[maxPixel];
  for(int j=0;j<numberOfDetectors;j++)	    
    for(int i=0;i<maxPixel;i++)
      pixels[j][i]=&ev.getPixel(j,i);
  tracks=new TTrack*[100];
  numberOfTracks=&ev.getNumberOfTracks();
  for(int i=0;i<maxTrack;i++)
    tracks[i]=&ev.getTrack(i);
}

TFile *AWriteHistogramsToRootFile::getFile()
{
  return *rootFile;
}

TH1* AWriteHistogramsToRootFile::getHisto(const string &name)
{
  for(int i=0;i<numberOfHistos;i++)
    if(histos[i])
      if(name==string(histos[i]->GetName()))
	return histos[i];
  for(int i=0;i<numberOf2DHistos;i++)
    if(TDhistos[i])
      if(name==string(TDhistos[i]->GetName()))
	return TDhistos[i];
  return 0;
}

void AWriteHistogramsToRootFile::getDestroyHistos(TH1** ODH, TH2** TDH)
{
  if(server)
    {
      for(int i=0;i<numberOfHistos;i++)
	if(histos[i])
	  histos[i]->Add(ODH[i]);
      for(int i=0;i<numberOf2DHistos;i++)
	if(TDhistos[i])
	  TDhistos[i]->Add(TDH[i]);
    }
}
TH1**AWriteHistogramsToRootFile::getHistos1()
{
  return histos;
}

TH2**AWriteHistogramsToRootFile::getHistos2()
{
  return TDhistos;
}

void AWriteHistogramsToRootFile::setHistos(TH1** h1,TH2 **h2)
{
  if(!server)
    {
      histos=h1;
      TDhistos=h2;
    }
  if(server)
    {
      histos=new TH1*[numberOfHistos];
      TDhistos=new TH2*[numberOf2DHistos];
      int s1=0,s2=0;
      for(int i=0;i<numF;i++)
	{
	  s1+=f[i]->declareHistos(histos,s1);
	  s2+=f[i]->declare2DHistos(TDhistos,s2);
	}
    }
}
vector<string>AWriteHistogramsToRootFile::histogramNames()
{
  vector<string>tmp;
  tmp.push_back(getName());
  for(int i=0;i<numberOfHistos;i++)
    if(histos[i]!=NULL)tmp.push_back(histos[i]->GetName());
  for(int i=0;i<numberOf2DHistos;i++)
    if(TDhistos[i]!=NULL)tmp.push_back(TDhistos[i]->GetName());
  return tmp;
}
TH1* AWriteHistogramsToRootFile::histogram(string histoName)
{
  for(int i=0;i<numberOfHistos;i++)
    if(histos[i]!=NULL)
      if(histoName==histos[i]->GetName())
	return histos[i];
  for(int i=0;i<numberOf2DHistos;i++)
    if(TDhistos[i]!=NULL)
      if(histoName == TDhistos[i]->GetName())
	return TDhistos[i];
  return NULL;
}
