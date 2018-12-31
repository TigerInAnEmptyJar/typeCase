#include "algorithm.h"
#include "event.h"
#include "setup.h"
#include <TFile.h>
#include <TTree.h>

class AReadFromRoot : public AAlgorithm
{
  Q_OBJECT
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

public:
  AReadFromRoot(const string& filename, const string& directory, const string& treeName,
                TEvent& eventIn, TSetup& setupIn, int& readInIDIN, int numEvents,
                void* input_mutexIn);
  AReadFromRoot(TFile* rootFileIn, const string& directory, const string& treeName, TEvent& eventIn,
                TSetup& setupIn, int& readInIDIN, int numEvents, void* input_mutexIn);
  virtual ~AReadFromRoot();
  virtual void* process(void* ptr);
signals:
  void eventRead();
public slots:
  void newEvent();
  void getNewFile(const string& filename, const string& directory, const string& treeName);
};
