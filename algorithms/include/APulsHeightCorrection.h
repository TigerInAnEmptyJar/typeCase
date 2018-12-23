#include "algorithm.h"
#include <TF1.h>
#include "container.h"

class APulsHeightCorrectionWedge:public AAlgorithm
{
Q_OBJECT
  private:
 TF1 ***correctionFunction;//!
 TTrack **tracks;//!
 int &numberOfTracks;//!
    int numberOfRanges;
    int **ranges;//!
    int actualRange;
    int DetID;
    int numberOfElements;
    int &runNr;//!
    int &eventNr;//!
 public:
    APulsHeightCorrectionWedge(TTrack **tracksIn,int &numberOfTracksIn, TDetector &det, int& runNumber, int &eventNumber, vector<string> &calibrationFiles);
    virtual ~APulsHeightCorrectionWedge();
    virtual void *process(void*ptr);
    public slots:
	void getNewCalibration(vector<string> &calibrationFiles);
};

class APulsHeightCorrectionSpiral:public AAlgorithm
{
Q_OBJECT
  private:
 TF1 ***correctionFunction;//!
 TTrack **tracks;//!
 int &numberOfTracks;//!
    int numberOfRanges;
    int **ranges;//!
    int actualRange;
    int DetID;
    int numberOfElements;
    int &runNr;//!
    int &eventNr;//!
 public:
    APulsHeightCorrectionSpiral(TTrack **tracksIn,int &numberOfTracksIn, TDetector &det, int& runNumber, int &eventNumber, vector<string> &calibrationFiles);
    virtual ~APulsHeightCorrectionSpiral();
    virtual void *process(void*ptr);
    public slots:
	void getNewCalibration(vector<string> &calibrationFiles);
};
class APulsHeightCorrectionTube:public AAlgorithm
{
Q_OBJECT
 private:
 TF1 ***correctionFunction;//!
 TTrack **tracks;//!
 int &numberOfTracks;//!
    int numberOfRanges;
    int **ranges;//!
    int actualRange;
    int DetID;
    int numberOfElements;
    int &runNr;//!
    int &eventNr;//!
 public:
    APulsHeightCorrectionTube(TTrack **tracksIn,int &numberOfTracksIn, TDetector &det, int& runNumber, int &eventNumber, vector<string> &calibrationFiles);
    virtual ~APulsHeightCorrectionTube();
    virtual void *process(void*ptr);
    public slots:
	void getNewCalibration(vector<string> &calibrationFiles);
};
