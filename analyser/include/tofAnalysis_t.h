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

/*!
 * \brief The tofAnalysis class
 */
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

  /*!
   * \brief getAlgorithm
   * \param param
   * \param firstRun
   * \param position
   * \param positionFit
   * \param nFits
   * \return
   */
  int getAlgorithm(const algorithm_parameter& param, const run_parameter& firstRun, int position,
                   int positionFit, int& nFits);

public:
  /*!
   * \brief tofAnalysis
   * Constructor. Sets the maximum number of hits, pixels, clusters and tracks (per detector or ID)
   * to max (default = 100) and the maximum number of IDs or detectors to maxDet (default = 100).
   * The number of parallel processes is set to NThreads. Don't set it to a number larger than the
   * number of processors in your machine. It won't break, but there will only be a speed decrease.
   * \param max
   * \param maxDets
   * \param NThreads
   */
  tofAnalysis(int max = 100, int maxDets = 100, int NThreads = 1);

  /*!
   * \brief Destructor
   */
  ~tofAnalysis();

  /*!
   * \brief process
   * Does analysis until all input is read.
   * \return
   */
  bool process();

  /*!
   * \brief initStep
   * Initializes the complete analysis. The setup is defined with col describing the reaction
   * properties, dets the detectors, mats the materials. The algorithms are initialized (see
   * initAlgorithms(). and the beamParam and runParam are initializing the data and calibration for
   * read in.
   * \param p
   * \param beamParam
   * \param runParam
   * \param dets
   * \param mats
   * \param col
   */
  void initStep(const vector<algorithm_parameter>& p, vector<beamTime_parameter>& beamParam,
                vector<run_parameter>& runParam, vector<detector_parameter>& dets,
                vector<material_parameter>& mats, reaction_parameter col);

  /*!
   * \brief step
   * Do num events in analysis. Stops if no data is left to read.
   * \param num
   * \return
   */
  bool step(int num = 1);

  /*!
   * \brief endStep
   * Kills all data, algorithms and such. Done before finally leaving the analysis.
   */
  void endStep();

  /*!
   * \brief initAlgorithms
   * Initializes the algorithms used in the analysis. In principle it is a giant switch-command
   * taking the ID of the parameter p for all of the algorithms. Here the algorithms-constructors
   * are called, the data structures are passed. At the end, the newRun signal is emitted.
   * \param p
   * \param runParam
   */
  void initAlgorithms(const vector<algorithm_parameter>& p, const run_parameter& runParam);

  /*!
   * \brief killAlgorithms
   * Deletes the algorithms used in the analysis. Here the destructor is called. In many an
   * algorithm the write-to-file is done here!
   */
  void killAlgorithms();

  /*!
   * \brief initData
   * Initializes the Data structures for analysis.
   */
  void initData();

  /*!
   * \brief killData
   * Deletes the Data-structures for the analysis.
   */
  void killData();

  /*!
   * \brief getVShape
   * Static method. Takes a shape_parameter and converts it into a volumeShape of the appropriate
   * type. Has to be modified if new shapes are defined.
   * \param sh
   * \return
   */
  static volumeShape* getVShape(shape_parameter& sh);

  /*!
   * \brief defineMaterials
   * Reads the material parameters from file fileName2 and creates the materials as described. It
   * also reads the setupdescription from fileName to assign the correct materials to the detectors.
   * \param fileName
   * \param fileName2
   */
  void defineMaterials(const string& fileName, const string& fileName2);

  /*!
   * \brief defineMaterials
   * Defines the materials according to the material describtions mats and assigns them to the
   * detectors.
   * \param mats
   * \param dets
   */
  void defineMaterials(vector<material_parameter>& mats, vector<detector_parameter>& dets);

  /*!
   * \brief defineDetectors
   * Reads the detector parameters from file fileName and creates the detectors according to the
   * description.
   * \param fileName
   */
  void defineDetectors(const string& fileName);

  /*!
   * \brief defineDetectors
   * Defines the detectors as described by dets.
   * \param dets
   */
  void defineDetectors(vector<detector_parameter>& dets);

  /*!
   * \brief defineReaction
   * Defines the reaction properties according to the reaction parameter col.
   * \param col
   */
  void defineReaction(reaction_parameter col);

  /*!
   * \brief createSetup
   * Creates a setup out of the detectors parameterized by dets, the materials by mats and the
   * reaction properties col.
   * \param dets
   * \param mats
   * \param col
   */
  void createSetup(vector<detector_parameter>& dets, vector<material_parameter>& mats,
                   reaction_parameter col);

  /*!
   * \brief createSetup
   * Creates a setup by reading a setup-file and a material-file.
   * \param setupFileName
   * \param materialFileName
   */
  void createSetup(const string& setupFileName, const string& materialFileName);

  /*!
   * \brief killSetup
   * Destroys the detector setup. All shapes, materials, and detectors are deleted.
   */
  void killSetup();

  /*!
   * \brief thread_run
   * Runs a thread. Do not call. Will be called from inside the class. Would have been private, but
   * that was not possible out of technical reasons
   * \param ptr
   * \return
   */
  void* thread_run(void* ptr);

  /*!
   * \brief getHistogramNames
   * Returns the histogram names of the histograms that have been defined by the algorithms in the
   * analysis. Note that the first element in the vector-elements are the algorithm name.
   * \return
   */
  vector<vector<string>> getHistogramNames();

  /*!
   * \brief getHisto
   * Returns a pointer to the histogram of name name defined in one of the algorithms. Returns NULL
   * if no such histogram has been defined.
   * \param name
   * \return
   */
  TH1* getHisto(const string& name);

  /*!
   * \brief getTreeNames
   * Returns the names of the TTree-variables that are defined in the algorithms of the analysis.
   * The first element of each vector-element is the algorithm name.
   * \return
   */
  vector<vector<string>> getTreeNames();

  /*!
   * \brief getTree
   * Returns the TTree-variable of name name, that was defined by one of the algorithms. Returns
   * NULL if no such tree has been declared.
   * \param name
   * \return
   */
  TTree* getTree(const string& name);

  /*!
   * \brief getVariables
   * Static method. Returns a vector of strings. Each three following elements define a variable.
   * First is the variable name, second its type and the third one defines the binding.
   * \return
   */
  static vector<string> getVariables();

  /*!
   * \brief getTheTypes
   * obsolete. Static method for retrieving the types of the variables the class defines for the
   * analysis.
   * \return
   */
  static vector<string> getTheTypes();

  /*!
   * \brief getTheVID
   * obsolete. Static method to retrieve the binding of the variables defined in the class for
   * analysis purpurses.
   * \return
   */
  static vector<int> getTheVID();

  /*!
   * \brief getID
   * obsolete. Static method. Returns the currently new ID for a new algorithm.
   * \return
   */
  static int getID();

  /*!
   * \brief newID
   * obsolete. Static method. Generates a possible new ID for a new algorithm.
   */
  static void newID();

  /*!
   * \brief init
   * Static method to init the IDs. obsolete!
   */
  static void init();

  /*!
   * \brief getAlgorithmParameter
   * Depending on the ID, the result of the static getDescription() of the corresponding installed
   * algorithm is returned.
   * \param ID
   * \return
   */
  static algorithm_parameter getAlgorithmParameter(int ID);

  /*!
   * \brief getAlgorithm
   * Static method. This is the main Algorithm defining method. It contains the huge switch-call to
   * decide on the ID of the parameter which algorithm to initialize. It returns the total number of
   * algorithms allocated. In out these algorithms are saved. Delete them after use. executeUpTo
   * gives the number of algorithms that should be called in the event-loop, the rest can be such
   * things as fitting algorithms. Apart from the data-structures, there are readValid that is given
   * to reading-algorithms that will switch it to false if the number of remaining events for the
   * current file is zero, readInID which is obsolete but kept for backward comptability, stearIt
   * the class, that provides the essential signals for e.g. new runs, eventRequesting that provides
   * a switch, that of several different reading algorithms one (not necessarily the first one) can
   * act as an event-input-list.
   * \param out
   * \param executeUpTo
   * \param param
   * \param firstRun
   * \param event
   * \param setup
   * \param numberOfHits
   * \param numberOfTracks
   * \param numberOfPixels
   * \param numberOfClusters
   * \param numberOfHitClusters
   * \param raws
   * \param calibratedHits
   * \param hitClusters
   * \param tracks
   * \param pixels
   * \param clusters
   * \param materials
   * \param detectors
   * \param readValid
   * \param readInID
   * \param stearIt
   * \param eventRequesting
   * \return
   */
  static int getAlgorithm(AAlgorithm*** out, int& executeUpTo, const algorithm_parameter& param,
                          const run_parameter& firstRun, TEvent& event, TSetup& setup,
                          int** numberOfHits, int* numberOfTracks, int** numberOfPixels,
                          int** numberOfClusters, int** numberOfHitClusters, TRawHit*** raws,
                          TCalibHit*** calibratedHits, THitCluster*** hitClusters, TTrack** tracks,
                          TPixel*** pixels, TCluster*** clusters, TMaterial** materials,
                          TDetector** detectors, bool& readValid, int& readInID, QObject* stearIt,
                          bool& eventRequesting);
public slots:
  /*!
   * \brief stopAnalysis
   * Stops the analysis. The number of processed events is returned. Actually, the analysis will
   * stop, when the last algorithm for this event has finished.
   * \return
   */
  int stopAnalysis();

  /*!
   * \brief addRun
   * Adds a run to the list of runs to process.
   * \param rp
   */
  void addRun(run_parameter& rp);

  /*!
   * \brief removeRun
   * Remove run rp from the list of runs to process. Does nothing if rp is not in the list.
   * \param rp
   */
  void removeRun(run_parameter& rp);

  /*!
   * \brief doRequestEvent
   * Emit the requestEvent-Signal, that can be caught by an input algorithm.
   * \param eventNumber
   * \param runNumber
   */
  void doRequestEvent(int eventNumber, int runNumber);

  /*!
   * \brief doRequestNextEvent
   * Emit the requestNextEvent-Signal, that can be caught by an input algorithm.
   */
  void doRequestNextEvent();

  /*!
   * \brief showNewRun
   * Debugging output. Writes Run information to analysisLog.
   * \param run
   */
  void showNewRun(run_parameter& run);
signals:
  /*!
   * \brief newRun
   * This signal is emitted, whenever a run has been read and a new one is due. It passes the new
   * run r and its beam-time b.This signal is emitted, whenever a run has been read and a new one is
   * due. It passes the new run r and its beam-time b.
   * \param r
   * \param b
   */
  void newRun(run_parameter& r, beamTime_parameter& b);

  /*!
   * \brief newEvent
   * This signal is emitted, when an event has been processed. If you chose to analyse a single
event it will be emitted after this, but if the number of events is large, it is only emitted all
"showFrequency" events.
This is 10000 if no more than 4 algorithms have been defined, 1000 else. Some algorithms also modify
this value. For single event mode, the event-number is passed, the number of processed events else.
   * \param nrEvent
   */
  void newEvent(int nrEvent);
signals:
  /*!
   * \brief changeEvent
   * This signal is emitted, when an event has been processed. If you chose to analyse a single
event it will be emitted after this, but if the number of events is large, it is only emitted all
"showFrequency" events.
This is 10000 if no more than 4 algorithms have been defined, 1000 else. Some algorithms also modify
this value.
The complete event-structure is passed when called. Do not modify it. Others do perhaps also want to
use it.
   * \param ev
   */
  void changeEvent(TEvent* ev);

  /*!
   * \brief algorithmInit
   * Signal that is emitted, when an algorithm is going to be initialized. algoID is the position of
   * the algorithm in the array.
   * \param algoID
   */
  void algorithmInit(int algoID);

  /*!
   * \brief algorithmInited
   * Signal that is emitted, when an algorithm has been initialized. algoID is the position of the
   * algorithm in the array.
   * \param algoID
   */
  void algorithmInited(int algoID);

  /*!
   * \brief analysisFinished
   * Signal that is emitted, when the analysis finished (because of reached maximum of events to
   * process or read in is no longer valid). The parameter numEvents will hold the number of events
   * that have been processed.
   * \param numEvents
   */
  void analysisFinished(int numEvents);

  /*!
   * \brief initStateChanged
   * This signal is emitted, whenever a new algorithm is going to be initialized, the
   * data-structures, setup etc are being created.
   * \param msg
   */
  void initStateChanged(const char* msg);

  /*!
   * \brief finishStateChanged
   * Signal emitted when a part of the finalisation is finished. Which one is given in msg.
   * \param msg
   */
  void finishStateChanged(const char* msg);

  /*!
   * \brief requestEvent
   * Requests a specific event. Should be cought by an input algorithm.
   * \param eventnumber
   * \param runnumber
   */
  void requestEvent(int eventnumber, int runnumber);

  /*!
   * \brief requestNextEvent
   * Requests the next Event in queue. Should be cought by an input algorithm.
   */
  void requestNextEvent();
};
#endif
