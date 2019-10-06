#include "algorithm.h"
#include "algorithmparameter.h"
#include "detector.h"
#include "track.h"

class ACalculateTrackProperties : public AAlgorithm
{
private:
  const int maxTracks;
  TTrack** tracks;
  int& numberOfTracks;
  float* timing1;
  float* timing2;
  int* startIDs;
  const int numStart;
  const int numStop;
  int* stopIDs;
  point3D* hit1;
  plane3D start;
  float startTiming(TTrack* track, point3D& h);
  float stopTiming(TTrack* track, point3D& h);
  bool chargedPrompt;
  bool neutralPrompt;
  bool secondary[4];

public:
  ACalculateTrackProperties(TTrack** t, int& numberOfTracksIn, int max, const TDetector& st,
                            const algorithm_parameter& descr);
  ACalculateTrackProperties(TTrack** t, int& numberOfTracksIn, int max, const TDetector& st,
                            vector<int> stopDetectors, vector<int> startDetectors);
  ~ACalculateTrackProperties() override;
  virtual void process() override;
  static algorithm_parameter getDescription();
};
