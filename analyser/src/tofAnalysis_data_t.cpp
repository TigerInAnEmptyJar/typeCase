//#include "algorithms.h"
#include "logger.h"
#include <QtCore/QDateTime>
#include "tofAnalysis_t.h"
void tofAnalysis::initData()
{
    QTime timer;
    timer.start();
    if(!isInitS)return;
    anaLog<<"init data structures"<<incD<<endli;
    anaLog<<"init scalars ... ";
    NumberOfDetectors=Setup.getNumberOfDetectors();
    NumberOfMaterials=0;//Setup.getNumberOfMaterials();
    maxOfCalibratedHits=Event[0].getMaxNumber<TRawHit>();
    maxOfPixels=Event[0].getMaxNumber<TPixel>();
    maxOfClusters=Event[0].getMaxNumber<TCluster>();
    maxOfTracks=Event[0].getMaxNumber<TTrack>();
    //    cout<<maxOfCalibratedHits<<" "<<maxOfPixels<<" "<<maxOfClusters<<" "<<maxOfTracks<<endl;
    anaLog <<"init container"<<endli;
    NumberOfHits=new int**[nThreads];
    CalibratedHits=new TCalibHit***[nThreads];
    Raws=new TRawHit***[nThreads];
    NumberOfHitClusters=new int **[nThreads];
    HitClusters=new THitCluster***[nThreads];
    NumberOfPixels=new int**[nThreads];
    Pixels=new TPixel***[nThreads];
    NumberOfClusters=new int**[nThreads];
    Clusters=new TCluster***[nThreads];
    Tracks=new TTrack**[nThreads];
    Particles=new momentum4D**[nThreads];
    NumberOfTracks=new int*[nThreads];
    for(int ii=0;ii<nThreads;ii++)
      {
	anaLog<<"init Hits ... ";
	NumberOfHits[ii]=new int*[NumberOfDetectors];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  NumberOfHits[ii][j]=&Event[ii].getNumberOfHits(j);
	CalibratedHits[ii]=new TCalibHit**[NumberOfDetectors];
	for(int i=0;i<NumberOfDetectors;i++)
	  CalibratedHits[ii][i]=new TCalibHit*[maxOfCalibratedHits];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  for(int i=0;i<maxOfCalibratedHits;i++)
	    (CalibratedHits[ii][j])[i]=&Event[ii].getHit(j,i);
	anaLog<<"init raw Hits ... ";
	Raws[ii]=new TRawHit**[NumberOfDetectors];
	for(int i=0;i<NumberOfDetectors;i++)
	  Raws[ii][i]=new TRawHit*[maxOfCalibratedHits];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  for(int i=0;i<maxOfCalibratedHits;i++)
	    Raws[ii][j][i]=&CalibratedHits[ii][j][i]->getRaw();
	anaLog<<"init Hit-cluster ... ";
	HitClusters[ii]=new THitCluster**[NumberOfDetectors];
	NumberOfHitClusters[ii]=new int*[NumberOfDetectors];
	for(int i=0;i<NumberOfDetectors;i++)
	  HitClusters[ii][i]=new THitCluster*[maxOfCalibratedHits];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  for(int i=0;i<maxOfCalibratedHits;i++)
	    HitClusters[ii][j][i]=&Event[ii].getHitCluster(j,i);
	for(int j=0;j<NumberOfDetectors;j++)	    
	  NumberOfHitClusters[ii][j]=&Event[ii].getNumberOfHitClusters(j);
	anaLog<<"init Pixels ... ";
	NumberOfPixels[ii]=new int*[NumberOfDetectors];
	Pixels[ii]=new TPixel**[NumberOfDetectors];
	for(int i=0;i<NumberOfDetectors;i++)
	  Pixels[ii][i]=new TPixel*[maxOfPixels];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  for(int i=0;i<maxOfPixels;i++)
	    {
	      Pixels[ii][j][i]=&Event[ii].getPixel(j,i);
	      Pixels[ii][j][i]->setID(j);
	    }
	for(int j=0;j<NumberOfDetectors;j++)	    
	  NumberOfPixels[ii][j]=&Event[ii].getNumberOfPixels(j);
	anaLog<<"init Clusters ... ";
	Clusters[ii]=new TCluster**[NumberOfDetectors];
	NumberOfClusters[ii]=new int*[NumberOfDetectors];
	for(int i=0;i<NumberOfDetectors;i++)
	  Clusters[ii][i]=new TCluster*[maxOfClusters];
	for(int j=0;j<NumberOfDetectors;j++)	    
	  for(int i=0;i<maxOfClusters;i++)
	    Clusters[ii][j][i]=&Event[ii].getCluster(j,i);
	for(int j=0;j<NumberOfDetectors;j++)	    
	  NumberOfClusters[ii][j]=&Event[ii].getNumberOfClusters(j);
	anaLog<<"init Tracks and Particles ... ";
	Tracks[ii]=new TTrack*[maxOfTracks];
	Particles[ii]=new momentum4D*[maxOfTracks];
	NumberOfTracks[ii]=&Event[ii].getNumberOfTracks();
	for(int i=0;i<maxOfTracks;i++)
	  {
	    Tracks[ii][i]=&Event[ii].getTrack(i);
	    Particles[ii][i]=&(Tracks[ii][i]->getParticler());
	  }
      }
    isInitD=true;
    anaLog<<"init data structures done, taking "<<timer.elapsed()<<"ms"<<decD<<endli;
}
void tofAnalysis::killData()
{
    anaLog<<"delete data"<<endli;
    for(int ii=0;ii<nThreads;ii++)
      {
	if(Raws!=NULL)
	  {
	    if(Raws[ii]!=NULL)
	      {
		for(int i=0;i<NumberOfDetectors;i++)
		  delete [] Raws[ii][i];
		delete[] Raws[ii];
	      }
	    Raws[ii]=NULL;
	  }
	if(CalibratedHits!=NULL)
	  {
	    if(CalibratedHits[ii]!=NULL)
	      {
		for(int i=0;i<NumberOfDetectors-1;i++)
		  if(CalibratedHits[ii][i]!=NULL)delete [] CalibratedHits[ii][i];
		delete[] CalibratedHits[ii];
		CalibratedHits[ii]=NULL;
	      }
	  }
	if(HitClusters!=NULL)
	  {
	    if(HitClusters[ii]!=NULL)
	      {
		for(int i=0;i<NumberOfDetectors;i++)
		  delete[] HitClusters[ii][i];
		delete[] HitClusters[ii];
	      }
	    HitClusters[ii]=NULL;
	  }
	if(Pixels!=NULL)
	  {
	    if(Pixels[ii]!=NULL)
	      {
		for(int i=0;i<NumberOfDetectors;i++)
		  delete [] Pixels[ii][i];
		delete[] Pixels[ii];
	      }
	    Pixels[ii]=NULL;
	  }
	if(Tracks!=NULL)
	  {
	    if(Tracks[ii]!=NULL)
	      delete[] Tracks[ii];
	    Tracks[ii]=NULL;
	  }
	if(Clusters!=NULL)
	  {
	    if(Clusters[ii]!=NULL)
	      {
		for(int i=0;i<NumberOfDetectors;i++)
		  delete[] Clusters[ii][i];
		delete[] Clusters[ii];
	      }
	    Clusters[ii]=NULL;
	  }
	if(Particles!=NULL)
	  {
	    if(Particles[ii]!=NULL)delete [] Particles[ii];
	    Particles[ii]=NULL;
	  }
	if(NumberOfHits!=NULL)
	  {
	    if(NumberOfHits[ii]!=NULL)delete[]NumberOfHits[ii];
	    NumberOfHits[ii]=NULL;
	  }
	if(NumberOfPixels!=NULL)
	  {
	    if(NumberOfPixels[ii]!=NULL)delete[]NumberOfPixels[ii];
	    NumberOfPixels[ii]=NULL;
	  }
	if(NumberOfClusters!=NULL)
	  {
	    if(NumberOfClusters[ii]!=NULL)delete[]NumberOfClusters[ii];
	    NumberOfClusters[ii]=NULL;
	  }
      }
    if(NumberOfHits!=NULL)delete[]NumberOfHits;
    NumberOfHits=NULL;
    if(Raws!=NULL)delete[]Raws;
    Raws=NULL;
    if(CalibratedHits!=NULL)delete[]CalibratedHits;
    CalibratedHits=NULL;
    if(NumberOfPixels!=NULL)delete[]NumberOfPixels;
    NumberOfPixels=NULL;
    if(Pixels!=NULL)delete[]Pixels;
    Pixels=NULL;
    if(NumberOfClusters!=NULL)delete[]NumberOfClusters;
    NumberOfClusters=NULL;
    if(Clusters!=NULL)delete[]Clusters;
    Clusters=NULL;
    if(NumberOfTracks!=NULL)delete[]NumberOfTracks;
    NumberOfTracks=NULL;
    if(Tracks!=NULL)delete[]Tracks;
    Tracks=NULL;
    if(Particles!=NULL)delete[]Particles;
    Particles=NULL;
    isInitD=false;
    anaLog<<"done"<<endli;
}
