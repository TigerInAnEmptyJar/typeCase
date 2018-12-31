#ifndef _ANALYSER_
#define _ANALYSER_
#include "TFile.h"
#include "TH1.h"
#include "algorithm.h"
#include "container.h"
#include "fitAlgorithm.h"
#include "parameterManager.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include <TTree.h>
#include <fstream>
#include <pthread.h>
#include <vector>
using namespace std;
class tofAnalysis : public QObject
{
  Q_OBJECT
private:
  static vector<bool> ids;
  static int currID;
  const int maxAlgorithms;
  const int nThreads;
  pthread_mutex_t count_mutex;  // number of Events that have been calculated
  pthread_mutex_t tcount_mutex; // the actual thread number -first to be locked
                                // in thread_run
  pthread_mutex_t input_mutex;  // given to the input algorithms to block the io-channel
  pthread_mutex_t histo_mutex;  // given to the histogramming algorithms not to
                                // write the same histo at once
  /*     pthread_mutex_t *term_mutex; // is the thread still running -second to
   * be locked */
  /*     bool *running;               // see term_mutex */
  /*     int count_mutex; number of Events that have been calculated */
  /*     int tcount_mutex;the actual thread number -first to be locked in
   * thread_run */
  /*     int input_mutex; given to the input algorithms to block the io-channel
   */
  /*     int histo_mutex; given to the histogramming algorithms not to write the
   * same histo at once
   */
  /*     int *term_mutex; is the thread still running -second to be locked */
  int count;  // see count_mutex
  int tCount; // see tcount_mutex
  int* threadID;
  int numAlgorithms;
  int numberOfEventsToAnalyse;
  TEvent* Event;
  TSetup Setup;
  int*** NumberOfHits;
  int** NumberOfTracks;
  int*** NumberOfPixels;
  int*** NumberOfClusters;
  int*** NumberOfHitClusters;
  TRawHit**** Raws;
  TCalibHit**** CalibratedHits;
  THitCluster**** HitClusters;
  TTrack*** Tracks;
  TPixel**** Pixels;
  TCluster**** Clusters;
  momentum4D*** Particles;
  int maxOfCalibratedHits;
  int maxOfPixels;
  int maxOfClusters;
  int maxOfTracks;
  int NumberOfMaterials;
  int NumberOfDetectors;
  TMaterial** Materials;
  TDetector** Detectors;
  AAlgorithm*** f;
  AFitAlgorithm*** fits;
  bool* waitFor;
  bool isInitD;
  bool isInitA;
  bool isInitS;
  ifstream* dataInput;
  TFile** rootFile;
  int numOfHitsInEvent;
  int readInID;
  vector<string> inputParameter;
  vector<run_parameter> runs;
  vector<beamTime_parameter> bts;
  int showFrequency;
  bool single;
  bool readInValid;
  bool run;
  bool callEventSearch;
  int getAlgorithm(const algorithm_parameter& param, const run_parameter& firstRun, int position,
                   int positionFit, int& nFits);

public:
  tofAnalysis(int max = 100, int maxDets = 100, int NThreads = 1);
  ~tofAnalysis();
  bool process();
  void initStep(const vector<algorithm_parameter>& p, vector<beamTime_parameter>& beamParam,
                vector<run_parameter>& runParam, vector<detector_parameter>& dets,
                vector<material_parameter>& mats, reaction_parameter col);
  bool step(int num = 1);
  void endStep();
  void initAlgorithms(const vector<algorithm_parameter>& p, const run_parameter& runParam);
  void killAlgorithms();
  void initData();
  void killData();
  static volumeShape* getVShape(shape_parameter& sh);
  void defineMaterials(const string& fileName, const string& fileName2);
  void defineMaterials(vector<material_parameter>& mats, vector<detector_parameter>& dets);
  void defineDetectors(const string& fileName);
  void defineDetectors(vector<detector_parameter>& dets);
  void defineReaction(reaction_parameter col);
  void createSetup(vector<detector_parameter>& dets, vector<material_parameter>& mats,
                   reaction_parameter col);
  void createSetup(const string& setupFileName, const string& materialFileName);
  void killSetup();
  void* thread_run(void* ptr);
  vector<vector<string>> getHistogramNames();
  TH1* getHisto(const string& name);
  vector<vector<string>> getTreeNames();
  TTree* getTree(const string& name);
  static vector<string> getVariables();
  static vector<string> getTheTypes();
  static vector<int> getTheVID();
  static int getID();
  static void newID();
  static void init();
  static algorithm_parameter getAlgorithmParameter(int ID);
  static int getAlgorithm(AAlgorithm*** out, int& executeUpTo, const algorithm_parameter& param,
                          const run_parameter& firstRun, TEvent& event, TSetup& setup,
                          int** numberOfHits, int* numberOfTracks, int** numberOfPixels,
                          int** numberOfClusters, int** numberOfHitClusters, TRawHit*** raws,
                          TCalibHit*** calibratedHits, THitCluster*** hitClusters, TTrack** tracks,
                          TPixel*** pixels, TCluster*** clusters, TMaterial** materials,
                          TDetector** detectors, bool& readValid, int& readInID, QObject* stearIt,
                          bool& eventRequesting);
public slots:
  int stopAnalysis();
  void addRun(run_parameter& rp);
  void removeRun(run_parameter& rp);
  void doRequestEvent(int eventNumber, int runNumber);
  void doRequestNextEvent();
  void showNewRun(run_parameter& run);
signals:
  void newRun(run_parameter& r, beamTime_parameter& b);
  void newEvent(int nrEvent);
signals:
  void changeEvent(TEvent* ev);
  void algorithmInit(int algoID);
  void algorithmInited(int algoID);
  void analysisFinished(int numEvents);
  void initStateChanged(const char* msg);
  void finishStateChanged(const char* msg);
  void requestEvent(int eventnumber, int runnumber);
  void requestNextEvent();
};
#endif
