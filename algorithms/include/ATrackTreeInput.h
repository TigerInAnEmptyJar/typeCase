#include "algorithm.h"
#include "calibHit.h"
#include "event.h"
#include "setup.h"
#include "track.h"
class TTree;
class TLeaf;
class algorithm_parameter;
class run_parameter;

using namespace std;

class ATrackTreeInput : public AAlgorithm
{
  Q_OBJECT
private:
  int numPattern;
  int* pattern;
  bool doPattern;
  TTree* trackTree;
  TLeaf* numbers[6];
  TLeaf* header[3];
  TLeaf* prompt[14];
  TLeaf* kink[25];
  TLeaf* vee[22];
  int currentEntry;
  TTrack** tracks;
  int& numberOfTracks;
  int maxtracks;
  int numDets;
  int** numberOfCalibHits;
  TCalibHit*** hits;
  int** numberOfPixels;
  TPixel*** pixels;
  bool localDirectory; //!
  string directory;    //!
  string copyFile;     //!
  bool searchForEvent;
  bool& valid;
  int& eventNumber;
  int& runNumber;
  int& trigger;
  void readPrompt();
  void readVee();
  void readKink();
  void clearTree();
  void setTrackErrors(TTrack* tr);
  void assignHitsToTrack(TLeaf* det, TLeaf* el, TTrack* tr, int nel, int count);
  void assignPixelsToTrack(TTrack* tr);

public:
  ATrackTreeInput(int& evtNrIn, int& runNrIn, int& triIn, TTrack** trackIn, int& numTrackIn,
                  int maxTrackIn, int numDetsIn, int** numHitsIn, TCalibHit*** hitIn,
                  int** numPixIn, TPixel*** PixIn, bool& readInValid,
                  const algorithm_parameter& param);
  virtual ~ATrackTreeInput();
  virtual void* process(void* ptr);
  static algorithm_parameter getDescription();
public slots:
  virtual void readEvent(int EvtNr);
  virtual void onNewRun(run_parameter& run);
  virtual void readEntry(int entry);
  virtual void prepareNextEntry();
};
