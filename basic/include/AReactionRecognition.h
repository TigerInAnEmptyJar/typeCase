#include "algorithm.h"
#include "RbaseReaction.h"
#include "event.h"
#include "setup.h"

class AReactionRecognition:public AAlgorithm
{
 private:
  int numberOfReactions;
  RbaseReaction **reactions;//!
  TEvent &event;//!
  TTrack **tracks;//!
  int getReactions(TSetup& setup,string option);
  AFitAlgorithm *kinfit;//!
 public:
  AReactionRecognition(TSetup &setup,TEvent &eventIn,AFitAlgorithm *kinfitIn,string options);
  AReactionRecognition(TSetup &setup,TEvent &eventIn,AFitAlgorithm *kinfitIn,vector<algorithm_parameter> processes);
  virtual ~AReactionRecognition();
  virtual void *process(void *ptr);
};
