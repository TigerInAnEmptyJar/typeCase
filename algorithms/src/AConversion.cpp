#include "AConversion.h"

AConversion::AConversion(const string& filename, TEvent& ev, void* input_mutexIn,
                         bool& validInputIn)
    : AAlgorithm("Convert data from one format to KTOF format"), actualEvent(0), event(ev),
      validInput(validInputIn)
{
  input_mutex = input_mutexIn;
  //  convertedData=new TTree();
  getNewRun(filename);
  //  convertedData->SetBranchAddress("Event",&event);
}

AConversion::~AConversion() { delete convertedData; }

void* AConversion::Process(void* ptr)
{
  if (actualEvent + 1 >= convertedData->GetEntries())
    validInput = false;
  //  convertedData->GetEntry(actualEvent);
  actualEvent++;
  emit eventRead();
  return ptr;
}
void AConversion::newEvent() { actualEvent++; }
//#include "your_header_files.h"
void AConversion::getNewRun(const string& filename)
{
  pthread_mutex_lock((pthread_mutex_t*)input_mutex); // for multiThreading: let
                                                     // only read one thread at
                                                     // a time, lock input
  int maxHit = 0, maxPixel = 0, maxCluster = 0, maxTracks = 0, maxDetectors = 0;
  TEvent ev;
  ev.reDimension(maxHit, maxPixel, maxCluster, maxTracks, maxDetectors);
  // open file
  if (filename == "")
    return;
  // read your own data

  // fill event constants
  // ev.setRunNumber(readRunNumber);
  // ev.setEventNumber(readEventNumber);

  // fill hits
  // ev.getNumberOfHits(detectorID)=numHitsFound;
  // ev.getHit(detectorID,count).getRaw().setTDC();//...

  // fill pixels
  // ev.getNumberOfPixels(PixelID)=numPixelsFound;
  // ev.getPixel(PixelID,count).addElement(ev.getHit(detectorID,num));

  // fill clusters
  // ev.getNumberOfClusters(ClusterID)=numClustersFound;
  // ev.getCluster(ClusterID,count).setCenter(point3D(0,0,0));

  // fill tracks
  // ev.getNumberOfTracks()=numTracksFound;
  // ev.getTrack(count).setPath(sLine3D(point3D(0,0,0),vector3D(0,0,1)));

  //  convertedData->Fill();
  pthread_mutex_unlock((pthread_mutex_t*)input_mutex); // for multiThreading:
                                                       // let only read one
                                                       // thread at a time,
                                                       // unlock input
}
