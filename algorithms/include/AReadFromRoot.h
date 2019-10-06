#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TTree.h>

#include <boost/signals2.hpp>

class AReadFromRoot : public AAlgorithm
{
private:
  TEvent* event; //!
  TSetup& setup; //!
  int evtNum;
  int accNum;             //!
  TFile* rootFile;        //!
  TDirectory* currentDir; //!
  TTree* eventTree;       //!
  bool ownFile;           //!
  int& whichID;
  void* input_mutex;
  boost::signals2::signal<void()> _eventReadSignal;

public:
  AReadFromRoot(const string& filename, const string& directory, const string& treeName,
                TEvent& eventIn, TSetup& setupIn, int& readInIDIN, int numEvents,
                void* input_mutexIn);
  AReadFromRoot(TFile* rootFileIn, const string& directory, const string& treeName, TEvent& eventIn,
                TSetup& setupIn, int& readInIDIN, int numEvents, void* input_mutexIn);
  ~AReadFromRoot() override;
  void process() override;
  boost::signals2::connection connectEventRead(std::function<void()> subscriber);
  void newEvent();
  void getNewFile(const string& filename, const string& directory, const string& treeName);
};
