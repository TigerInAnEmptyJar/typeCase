#include "AWriteDataToRootFile.h"
#include <QtCore/QFile>
#include <TArrayI.h>
extern bool existing(string filename);
AWriteDataToRootFile::AWriteDataToRootFile(const string& filename, const string& directory,
                                           const string& treeName, TEvent& eventIn, TSetup& setupIn,
                                           int maxTracks, bool serv)
    : AAlgorithm("Write data to root file"), setup(setupIn),
      maxDet(eventIn.getMaxNumber<TDetector>()), maxTrack(maxTracks),
      maxCluster(eventIn.getMaxNumber<TCluster>()), maxPixel(eventIn.getMaxNumber<TPixel>()),
      maxHit(eventIn.getMaxNumber<TCalibHit>()), server(serv)
{
  event = &eventIn;

  saveHits = new TCalibHit**[maxDet];
  savePixel = new TPixel**[maxDet];
  saveCluster = new TCluster**[maxDet];
  saveTracks = new TTrack*[maxTracks];
  numHits = new int*[maxDet];
  numPix = new int*[maxDet];
  numClus = new int*[maxDet];

  numTr = &event->getNumberOfTracks();
  for (int j = 0; j < maxTracks; j++)
    saveTracks[j] = &event->getTrack(j);
  for (int i = 0; i < maxDet; i++) {
    saveHits[i] = new TCalibHit*[maxHit];
    savePixel[i] = new TPixel*[maxPixel];
    saveCluster[i] = new TCluster*[maxCluster];
    for (int j = 0; j < maxHit; j++)
      saveHits[i][j] = &event->getHit(j, i);
    for (int j = 0; j < maxPixel; j++)
      savePixel[i][j] = &event->getPixel(j, i);
    for (int j = 0; j < maxCluster; j++)
      saveCluster[i][j] = &event->getCluster(j, i);
    numHits[i] = &event->getNumberOfHits(i);
    numPix[i] = &event->getNumberOfPixels(i);
    numClus[i] = &event->getNumberOfClusters(i);
  }

  string tn = treeName;
  hits = new TObjArray*;
  (*hits) = new TObjArray[maxDet]; //!
  pixels = new TObjArray*;
  (*pixels) = new TObjArray[maxDet]; //!
  clusters = new TObjArray*;
  (*clusters) = new TObjArray[maxDet];  // 1
  tracks = new TObjArray(maxTracks, 0); //!
  reactions = new TObjArray(maxTrack, 0);
  eventNumber = &event->getEventNumber();
  runNumber = &event->getRunNumber();
  for (int i = 0; i < maxDet; i++) {
    hits[i] = new TObjArray(maxHit, 0);
    pixels[i] = new TObjArray(maxPixel, 0);
    clusters[i] = new TObjArray(maxCluster, 0);
  }

  rootFile = new TFile*;
  if (existing(filename))
    (*rootFile) = new TFile(filename.data(), "UPDATE");
  else
    (*rootFile) = new TFile(filename.data(), "RECREATE");
  if (directory == "")
    currentDir = (*rootFile);
  else {
    currentDir = (TDirectory*)(*rootFile)->Get(directory.data());
    if (!currentDir)
      currentDir = (*rootFile)->mkdir(directory.data());
  }
  if ((currentDir)->Get(treeName.data()) != 0) {
    int i = 0;
    while ((currentDir)->Get((treeName + string_number(i)).data()) != 0) {
      i++;
    }
    tn = (treeName + string_number(i));
  }
  tree = new TTree(tn.data(), "data from event");
  //  tree=new TTree(tn.data(),"data from event");
  tree->Branch("EventNumber", eventNumber, "EventNumber/I");
  tree->Branch("RunNumber", runNumber, "RunNumber/I");
  for (int i = 0; i < maxDet; i++) {
    tree->Branch((string("hits") + string_number(i)).data(), "TObjArray", &hits[i], 32000, 0);
    tree->Branch((string("pixels") + string_number(i)).data(), "TObjArray", &pixels[i], 32000, 0);
    tree->Branch((string("clusters") + string_number(i)).data(), "TObjArray", &clusters[i], 32000,
                 0);
  }
  tree->Branch("Tracks", "TObjArray", &tracks, 32000, 0);
}

AWriteDataToRootFile::AWriteDataToRootFile(TFile** rootFileIn, const string& directory,
                                           const string& treeName, TEvent& eventIn, TSetup& setupIn,
                                           int maxTracks, bool serv)
    : AAlgorithm("Write data to root file"), setup(setupIn),
      maxDet(eventIn.getMaxNumber<TDetector>()), maxTrack(maxTracks),
      maxCluster(eventIn.getMaxNumber<TCluster>()), maxPixel(eventIn.getMaxNumber<TPixel>()),
      maxHit(eventIn.getMaxNumber<TCalibHit>()), server(serv)
{
  // this is used
  //  cout<<treeName.data()<<endl;
  event = &eventIn;

  string tn = treeName;
  hits = new TObjArray*;
  (*hits) = new TObjArray[maxDet]; //!
  pixels = new TObjArray*;
  (*pixels) = new TObjArray[maxDet]; //!
  clusters = new TObjArray*;
  (*clusters) = new TObjArray[maxDet];  // 1
  tracks = new TObjArray(maxTracks, 0); //!
  reactions = new TObjArray(maxTracks, 0);
  eventNumber = &event->getEventNumber();
  runNumber = &event->getRunNumber();
  for (int i = 0; i < maxDet; i++) {
    hits[i] = new TObjArray(maxHit, 0);
    pixels[i] = new TObjArray(maxPixel, 0);
    clusters[i] = new TObjArray(maxCluster, 0);
  }

  rootFile = rootFileIn;
  if (directory == "")
    currentDir = (*rootFile);
  else {
    currentDir = (TDirectory*)(*rootFile)->Get(directory.data());
    if (!currentDir)
      currentDir = (*rootFile)->mkdir(directory.data());
  }
  if ((currentDir)->Get(treeName.data()) != 0) {
    int i = 0;
    while ((currentDir)->Get((treeName + string_number(i)).data()) != 0) {
      i++;
    }
    tn = (treeName + string_number(i));
  }

  tree = new TTree(tn.data(), "data from event");
  tree->Branch("EventNumber", eventNumber, "EventNumber/I");
  tree->Branch("RunNumber", runNumber, "RunNumber/I");
  for (int i = 0; i < maxDet; i++) {
    tree->Branch((string("hits") + string_number(i)).data(), "TObjArray", &hits[i], 32000, 0);
    tree->Branch((string("pixels") + string_number(i)).data(), "TObjArray", &pixels[i], 32000, 0);
    tree->Branch((string("clusters") + string_number(i)).data(), "TObjArray", &clusters[i], 32000,
                 0);
  }
  tree->Branch("Tracks", "TObjArray", &tracks, 32000, 0);
  saveHits = new TCalibHit**[maxDet];
  savePixel = new TPixel**[maxDet];
  saveCluster = new TCluster**[maxDet];
  saveTracks = new TTrack*[maxTracks];
  numHits = new int*[maxDet];
  numPix = new int*[maxDet];
  numClus = new int*[maxDet];

  numTr = &event->getNumberOfTracks();
  for (int j = 0; j < maxTracks; j++)
    saveTracks[j] = &event->getTrack(j);
  for (int i = 0; i < maxDet; i++) {
    saveHits[i] = new TCalibHit*[maxHit];
    savePixel[i] = new TPixel*[maxPixel];
    saveCluster[i] = new TCluster*[maxCluster];
    for (int j = 0; j < maxHit; j++)
      saveHits[i][j] = &event->getHit(i, j);
    for (int j = 0; j < maxPixel; j++)
      savePixel[i][j] = &event->getPixel(i, j);
    for (int j = 0; j < maxCluster; j++)
      saveCluster[i][j] = &event->getCluster(i, j);
    numHits[i] = &event->getNumberOfHits(i);
    numPix[i] = &event->getNumberOfPixels(i);
    numClus[i] = &event->getNumberOfClusters(i);
  }
}

AWriteDataToRootFile::~AWriteDataToRootFile()
{
  if (server) {
    currentDir->cd();
    //   TArrayI tmp(5);
    //   tmp.add(maxDet);
    //   tmp.addAt(1,maxHit);
    //   tmp.addAt(2,maxPixel);
    //   tmp.addAt(3,maxCluster);
    //   tmp.addAt(4,maxTrack);
    //   tmp.Write("Event Constants");
    tree->Write();
    //      setup.Write("setup");
  } else {
    emit destroyer(tree);
  }
  delete tree;
  for (int i = 0; i < maxDet; i++) {
    delete[] saveHits[i];
    delete[] savePixel[i];
    delete[] saveCluster[i];
  }
  delete[] saveHits;
  delete[] savePixel;
  delete[] saveCluster;
  delete[] saveTracks;
  delete[] numHits;
  delete[] numPix;
  delete[] numClus;
  for (int i = 0; i < maxDet; i++) {
    delete hits[i];
    delete pixels[i];
    delete clusters[i];
  }
  //   cout<<13<<flush;
  //   delete hits;
  //   cout<<14<<flush;
  //   delete pixels;
  //   cout<<15<<flush;
  //   delete clusters;
  //   cout<<16<<flush;
  //   delete tracks;
  //   cout<<17<<flush;
}

void* AWriteDataToRootFile::process(void* ptr)
{
  for (int i = 0; i < maxDet; i++) {
    //      cout<<i<<flush;
    hits[i]->Clear();
    //       for(int j=0;j<(*numHits[i]);j++)
    // 	hits[i]->Add(saveHits[i][j]);
    //      cout<<"h"<<flush;
    pixels[i]->Clear();
    //       for(int j=0;j<(*numPix[i]);j++)
    // 	pixels[i]->Add(savePixel[i][j]);
    // cout<<"p"<<flush;
    clusters[i]->Clear();
    //       for(int j=0;j<(*numClus[i]);j++)
    // 	clusters[i]->Add(saveCluster[i][j]);
    // cout<<"c"<<endl;
  }
  reactions->Clear();
  //   for(int j=0;j<event->getNumberOfReactions();j++)
  //     reactions->Add(event->getReaction(j));
  tracks->Clear();
  //   for(int j=0;j<(*numTr);j++)
  //     tracks->Add(saveTracks[j]);
  tree->Fill();
  return ptr;
}

TFile* AWriteDataToRootFile::getFile() { return *rootFile; }
void AWriteDataToRootFile::getDestroyTree(TTree* tr)
{
  if (server) {
    TList li;
    li.Add(tr);
    tree->Merge(&li);
  }
}
// void AWriteDataToRootFile::Streamer(TBuffer &b)
// {
// }
