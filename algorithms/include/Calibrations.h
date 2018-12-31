#include "CommonCalibrationParser.h"
#include "RbaseReaction.h"
#include "TH2.h"
#include "TProfile.h"
#include "algorithm.h"
#include "algorithmparameter.h"
#include "event.h"
#include "setup.h"
using namespace std;

class APedestalCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** qdcHistograms;  //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int num);
  int eventStart;

public:
  APedestalCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  APedestalCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                       int minEntriesPerHisto);
  virtual ~APedestalCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AtdcFactorCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** tdcHistograms;  //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  AtdcFactorCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AtdcFactorCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                        int minEntriesPerHisto);
  virtual ~AtdcFactorCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AWalkCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** histograms;     //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  AWalkCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AWalkCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors, int minEntriesPerHisto);
  virtual ~AWalkCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class ACutsCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** qdcHistograms;  //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  ACutsCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  ACutsCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors, int minEntriesPerHisto);
  virtual ~ACutsCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AqdcFactorCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** qdcHistograms;  //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  AqdcFactorCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AqdcFactorCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                        int minEntriesPerHisto);
  virtual ~AqdcFactorCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AtdcOffsetCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** tdcHistograms;  //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  AtdcOffsetCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AtdcOffsetCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                        int minEntriesPerHisto);
  virtual ~AtdcOffsetCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AzBarrelCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** histograms;     //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  int eventStart;

public:
  AzBarrelCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AzBarrelCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                      int minEntriesPerHisto);
  virtual ~AzBarrelCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AtdcRadialPixCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TH1**** qdcHistograms;  //!
  TH1**** qdcHistograms1; //!
  TEvent& event;          //!
  int numberOfReactions;
  RbaseReaction** reactions; //!
  int numberOfDetectors;
  int* detectorIDs;           //[numberOfDetectors]
  bool* innerReadOut;         //[numberOfDetectors]
  float* CinSzinti;           //[numberOfDetectors]
  int* numberOfElements;      //[numberOfDetectors]
  int numberOfStartDetectors; //!
  int* startDetectors;        //!
  int numberOfPixels;         //!
  int* pixelIDs;              //!
  int* pixelpoints;           //!
  const int minEntriesPerHisto;
  bool postScriptHistos;
  int eventStart;
  CommonCalibrationParser* evaluate(int);
  void getReactions(const vector<string>& names);

public:
  AtdcRadialPixCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AtdcRadialPixCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                           vector<int>& Stdetectors, vector<int> pixIDs, vector<int> readSide,
                           vector<int> pointsPerPixel, vector<string> reactionNames,
                           int minEntriesPerHisto);
  virtual ~AtdcRadialPixCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AtdcRadialPolCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TH1*** qdcHistograms;  //!
  TH1*** qdcHistograms1; //!
  TTree* tree;           //!
  Int_t ints[20];        //!
  Float_t floats[20];
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  bool* innerReadOut;    //[numberOfDetectors]
  float* CinSzinti;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  int numberOfReactions;
  RbaseReaction** reactions;  //!
  int numberOfStartDetectors; //!
  int* startDetectors;        //!
  const int minEntriesPerHisto;
  bool postScriptHistos;
  CommonCalibrationParser* evaluate(int);
  void getReactions(const vector<string>& names);
  int eventStart;

public:
  AtdcRadialPolCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AtdcRadialPolCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                           vector<int>& Stdetectors, vector<int> readSide,
                           vector<string> reactionNames, int minEntriesPerHisto);
  virtual ~AtdcRadialPolCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AqdcRadialPixCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  bool* innerReadOut;    //[numberOfDetectors]
  float* CinSzinti;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1**** qdcHistograms; //!
  int numberOfReactions;
  RbaseReaction** reactions;  //!
  int numberOfStartDetectors; //!
  int* startDetectors;        //!
  int numberOfPixels;         //!
  int* pixelIDs;              //!
  int* pixelpoints;           //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  void getReactions(const vector<string>& names);
  int eventStart;

public:
  AqdcRadialPixCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AqdcRadialPixCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                           vector<int>& Stdetectors, vector<int> pixIDs, vector<int> readSide,
                           vector<int> pointsPerPixel, vector<string> reactionNamesw,
                           int minEntriesPerHisto);
  virtual ~AqdcRadialPixCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
class AqdcRadialPolCalibration : public AAlgorithm
{
  Q_OBJECT
private:
  TEvent& event; //!
  int numberOfDetectors;
  int* detectorIDs;      //[numberOfDetectors]
  bool* innerReadOut;    //[numberOfDetectors]
  float* CinSzinti;      //[numberOfDetectors]
  int* numberOfElements; //[numberOfDetectors]
  TH1*** qdcHistograms;  //!
  int numberOfReactions;
  RbaseReaction** reactions;  //!
  int numberOfStartDetectors; //!
  int* startDetectors;        //!
  const int minEntriesPerHisto;
  CommonCalibrationParser* evaluate(int);
  void getReactions(const vector<string>& names);
  int eventStart;

public:
  AqdcRadialPolCalibration(TEvent& eventIn, TSetup& setup, algorithm_parameter& descr);
  AqdcRadialPolCalibration(TEvent& eventIn, TSetup& setup, vector<int>& detectors,
                           vector<int>& Stdetectors, vector<int> readSide,
                           vector<string> reactionNames, int minEntriesPerHisto);
  virtual ~AqdcRadialPolCalibration();
  virtual void* process(void* ptr);
signals:
  void evaluated(CommonCalibrationParser*);
public slots:
  virtual void toEvaluate();
};
