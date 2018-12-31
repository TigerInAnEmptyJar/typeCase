#include "AReadFromRoot.h"
AReadFromRoot::AReadFromRoot(const string& filename, const string& directory,
                             const string& treeName, TEvent& eventIn, TSetup& setupIn,
                             int& readInIDIN, int numEvents, void* input_mutexIn)
    : AAlgorithm("Read data from rootFile"), setup(setupIn), whichID(readInIDIN)
{
  input_mutex = input_mutexIn;
  ownFile = true;
  event = &eventIn;
  evtNum = numEvents;
  accNum = 0;
  rootFile = new TFile(filename.data(), "READ");
  if (directory == "")
    currentDir = rootFile;
  else
    currentDir = (TDirectory*)rootFile->Get(directory.data());
  eventTree = (TTree*)currentDir->Get(treeName.data());
  eventTree->SetBranchAddress("Event", &event);
  if (eventTree->GetEntries() < evtNum)
    evtNum = eventTree->GetEntries();
  currentDir->cd();
  //  setup.Read("setup");
}

AReadFromRoot::AReadFromRoot(TFile* rootFileIn, const string& directory, const string& treeName,
                             TEvent& eventIn, TSetup& setupIn, int& readInIDIN, int numEvents,
                             void* input_mutexIn)
    : AAlgorithm("Read data from rootFile"), setup(setupIn), whichID(readInIDIN)
{
  input_mutex = input_mutexIn;
  ownFile = false;
  event = &eventIn;
  evtNum = numEvents;
  accNum = 0;
  rootFile = rootFileIn;
  if (directory == "")
    currentDir = rootFile;
  else
    currentDir = (TDirectory*)rootFile->Get(directory.data());
  eventTree = (TTree*)currentDir->Get(treeName.data());
  eventTree->SetBranchAddress("Event", &event);
  //  if(eventTree->GetEntries()<evtNum) evtNum=eventTree->GetEntries();
  currentDir->cd();
  //  setup.Read("setup");
}

AReadFromRoot::~AReadFromRoot()
{
  if (ownFile) {
    rootFile->Close();
    delete rootFile;
  }
}

void* AReadFromRoot::process(void* ptr)
{
  if (whichID == 20) {
    pthread_mutex_lock((pthread_mutex_t*)input_mutex);
    if (accNum < evtNum || ((accNum < eventTree->GetEntries()) && (evtNum == -1))) {
      eventTree->GetEntry(accNum);
      accNum++;
      emit eventRead();
    }
    pthread_mutex_unlock((pthread_mutex_t*)input_mutex);
  }
  return ptr;
}

void AReadFromRoot::getNewFile(const string& filename, const string& directory,
                               const string& treeName)
{
  if (ownFile) {
    rootFile->Close();
    delete rootFile;
  }
  ownFile = true;
  evtNum = evtNum - accNum;
  accNum = 0;
  rootFile = new TFile(filename.data(), "READ");
  if (directory == "")
    currentDir = rootFile;
  else
    currentDir = (TDirectory*)rootFile->Get(directory.data());
  eventTree = (TTree*)currentDir->Get(treeName.data());
  eventTree->SetBranchAddress("Event", &event);
  if (eventTree->GetEntries() < evtNum)
    evtNum = eventTree->GetEntries();
  currentDir->cd();
  //  setup.Read("setup");
}
void AReadFromRoot::newEvent() { accNum++; }
