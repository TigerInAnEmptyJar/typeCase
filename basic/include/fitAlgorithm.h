#pragma once
#include "algorithm.h"
#include "geometry.h"
/*!
 * \brief The AFitAlgorithm class
 * Base class to all fitting algorithms.
 */
class AFitAlgorithm : public AAlgorithm
{
public:
  /*!
   * \brief AFitAlgorithm
   * Constructor. Sets the name of the algorithm.
   * \param name the name of the fit algorithm.
   */
  AFitAlgorithm(const std::string& name = std::string());

  /*!
   * \brief Destructor
   */
  virtual ~AFitAlgorithm();

  /*!
   * \brief add
   * Adds a new data-point to the fit.
   * \param data the data to be added.
   */
  virtual void add(Vector& data);

  /*!
   * \brief clear
   * Clears the fit. To make a new fit make use of the add() method.
   */
  virtual void clear();

  /*!
   * \brief getResult
   * Returns a n-dimensional vector containing the result, where n is the number of parameters
   * of the fit.
   * \return the result of the fit.
   */
  virtual Vector getResult();

  /*!
   * \brief getClone
   * Returns a copy of the fit-algorithm. Note, that a new object is allocated. Take care, that
   * you dispose it correctly after use (potential memory leak).
   * \return a clone of this fit algorithm.
   */
  virtual AFitAlgorithm* getClone() = 0;

  /*!
   * \brief getAddPoint
   * A slot defined to add data to the fit so you can make use of the signal-and-slot-mechanism
   * of Qt. With this slot, the calling routine for the fit doesn't have to know the fitting
   * class.
   * \see add
   * \param data the data to add to the fitting algorithm.
   */
  virtual void getAddPoint(Vector& data);

  /*!
   * \brief fit
   * Main fitting procedure. Performs the fit and returns the chi-squared value of the fit.
   * \return the chi-squared of the fit.
   */
  virtual float fit();
};
