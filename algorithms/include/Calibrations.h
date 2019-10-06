#include "CommonCalibrationParser.h"
#include "RbaseReaction.h"
#include "TH2.h"
#include "TProfile.h"
#include "TTree.h"
#include "algorithm.h"
#include "algorithmparameter.h"
#include "event.h"
#include "setup.h"

#include <memory>

class CalibrationAlgorithm : public AAlgorithm
{
public:
  using AAlgorithm::AAlgorithm;
  virtual void toEvaluate() = 0;
  std::shared_ptr<CommonCalibrationParser> getCalibration();

protected:
  std::shared_ptr<CommonCalibrationParser> _parser;
};

class APedestalCalibration : public CalibrationAlgorithm
{
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
  ~APedestalCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AtdcFactorCalibration : public CalibrationAlgorithm
{
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
  ~AtdcFactorCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AWalkCalibration : public CalibrationAlgorithm
{
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
  ~AWalkCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class ACutsCalibration : public CalibrationAlgorithm
{
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
  ~ACutsCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AqdcFactorCalibration : public CalibrationAlgorithm
{
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
  ~AqdcFactorCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AtdcOffsetCalibration : public CalibrationAlgorithm
{
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
  ~AtdcOffsetCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AzBarrelCalibration : public CalibrationAlgorithm
{
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
  ~AzBarrelCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AtdcRadialPixCalibration : public CalibrationAlgorithm
{
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
  ~AtdcRadialPixCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AtdcRadialPolCalibration : public CalibrationAlgorithm
{
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
  ~AtdcRadialPolCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AqdcRadialPixCalibration : public CalibrationAlgorithm
{
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
  ~AqdcRadialPixCalibration() override;
  void process() override;
  void toEvaluate() override;
};

class AqdcRadialPolCalibration : public CalibrationAlgorithm
{
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
  ~AqdcRadialPolCalibration() override;
  void process() override;
  void toEvaluate() override;
};
