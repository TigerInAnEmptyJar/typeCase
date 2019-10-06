#pragma once

#include <string>

class algorithm_parameter;

/*!
 * \brief The AAlgorithm class
 * Basic algorithm class. Any derived classes of AAlgorithm can be inserted into the
 * typeCase-analyser.
 * It is movable but non-copyable.
 */
class AAlgorithm
{
public:
  virtual ~AAlgorithm() = default;
  AAlgorithm(AAlgorithm const&) = delete;
  AAlgorithm(AAlgorithm&&) = default;
  AAlgorithm& operator=(AAlgorithm const&) = delete;
  AAlgorithm& operator=(AAlgorithm&&) = default;

  /*!
   * \brief AAlgorithm
   * Constructor. Sets the name of the algorithm.
   * \param n Name of the algorithm.
   */
  AAlgorithm(std::string n);

  /*!
   * \brief getName
   * Returns the name of the algorithm.
   * \return
   */
  std::string getName() const;

  /*!
   * \brief process
   * Base functionality is defined here. In the analysis class, this method will be called
   * once for every event, so plug in the functionality you want to provide with your
   * algorithm here.
   * \param ptr ignored.
   * \return nullptr
   */
  virtual void process() = 0;

  /*!
   * \brief getDescription
   * Returns an algorithm_parameter containing a description and the necessary parameters
   * for the algorithm to run. The content of these parameters is not necessarily
   * sensible.
   * \return
   */
  static algorithm_parameter getDescription();

private:
  std::string name; //!
};
