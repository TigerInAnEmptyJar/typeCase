#include "RbaseReaction.h"
#include "algorithm.h"
#include "event.h"
#include "setup.h"

/*!
 * \brief The AReactionRecognition class
 */
class AReactionRecognition : public AAlgorithm
{
private:
  int numberOfReactions;
  RbaseReaction** reactions; //!
  TEvent& event;             //!
  TTrack** tracks;           //!
  int getReactions(TSetup& setup, string option);
  AFitAlgorithm* kinfit; //!
public:
  /*!
   * \brief AReactionRecognition
   * \param setup
   * \param eventIn
   * \param kinfitIn
   * \param options
   */
  AReactionRecognition(TSetup& setup, TEvent& eventIn, AFitAlgorithm* kinfitIn, string options);

  /*!
   * \brief AReactionRecognition
   * \param setup
   * \param eventIn
   * \param kinfitIn
   * \param processes
   */
  AReactionRecognition(TSetup& setup, TEvent& eventIn, AFitAlgorithm* kinfitIn,
                       vector<algorithm_parameter> processes);

  /*!
   * \brief ~AReactionRecognition
   */
  ~AReactionRecognition() override;

  /*!
   * \brief process
   * \param ptr
   * \return
   */
  void process() override;
};
