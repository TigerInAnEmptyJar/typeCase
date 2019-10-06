#include "algorithm.h"
#include "container.h"
#include <TF1.h>

class APulsHeightCorrectionWedge : public AAlgorithm
{
private:
  TF1*** correctionFunction; //!
  TTrack** tracks;           //!
  int& numberOfTracks;       //!
  int numberOfRanges;
  int** ranges; //!
  int actualRange;
  int DetID;
  int numberOfElements;
  int& runNr;   //!
  int& eventNr; //!
public:
  APulsHeightCorrectionWedge(TTrack** tracksIn, int& numberOfTracksIn, TDetector& det,
                             int& runNumber, int& eventNumber, vector<string>& calibrationFiles);
  ~APulsHeightCorrectionWedge() override;
  void process() override;
  void getNewCalibration(vector<string>& calibrationFiles);
};

class APulsHeightCorrectionSpiral : public AAlgorithm
{
private:
  TF1*** correctionFunction; //!
  TTrack** tracks;           //!
  int& numberOfTracks;       //!
  int numberOfRanges;
  int** ranges; //!
  int actualRange;
  int DetID;
  int numberOfElements;
  int& runNr;   //!
  int& eventNr; //!
public:
  APulsHeightCorrectionSpiral(TTrack** tracksIn, int& numberOfTracksIn, TDetector& det,
                              int& runNumber, int& eventNumber, vector<string>& calibrationFiles);
  ~APulsHeightCorrectionSpiral() override;
  void process() override;
  void getNewCalibration(vector<string>& calibrationFiles);
};

class APulsHeightCorrectionTube : public AAlgorithm
{
private:
  TF1*** correctionFunction; //!
  TTrack** tracks;           //!
  int& numberOfTracks;       //!
  int numberOfRanges;
  int** ranges; //!
  int actualRange;
  int DetID;
  int numberOfElements;
  int& runNr;   //!
  int& eventNr; //!
public:
  APulsHeightCorrectionTube(TTrack** tracksIn, int& numberOfTracksIn, TDetector& det,
                            int& runNumber, int& eventNumber, vector<string>& calibrationFiles);
  ~APulsHeightCorrectionTube() override;
  void process() override;
  void getNewCalibration(vector<string>& calibrationFiles);
};
