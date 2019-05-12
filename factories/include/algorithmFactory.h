#pragma once

#include "algorithm.h"
#include "algorithmparameter.h"

#include <boost/signals2.hpp>
#include <boost/uuid/uuid.hpp>

#include <map>

class TEvent;
class TSetup;

/*!
 * \brief The type an algorithm can have
 * This enum describes the types an algorithm can have. Especially input algorithms need mutexes to
 * serialize reading.
 */
enum class AlgorithmType
{
  //! For input algorithms
  Input,
  //! For output algorithms
  Output,
  //! For general processing algorithms
  Processing,
  //! Undefined, kind of an invalid value
  Undefined
};

/*!
 * \brief A factory for algorithm classes.
 *
 * This class is the factory for all algorithms. It collects factory functions for the installed
 * algorithms. This way the factory only knows about the basic kind of algorithm, but is unaware of
 * the actual implementation. If you want to publish an algorithm to the factory (and thus to
 * typeCase) use the addAlgorithmToFactory method to add the factory functor.
 *
 * This class is non-copyable. If you want to have your own factory, create one, but keep in mind:
 * there is no factory tree implemented.
 */
class AlgorithmFactory
{
public:
  AlgorithmFactory() = default;
  ~AlgorithmFactory() = default;
  AlgorithmFactory(AlgorithmFactory const& other) = delete;
  AlgorithmFactory& operator=(AlgorithmFactory const& other) = delete;

  //! \name Use management
  //! \{
  /*!
   * \brief retrieve a list of defined algorithms
   * \return a list of all currently defined algorithms.
   */
  std::vector<algorithm_parameter> definedAlgorithms() const;

  /*!
   * \brief get an algorithmParameter of a defined algorithm
   * \param id the id of the algorithm to return the parameter.
   * \return an algorithmParameter for the specified id.
   */
  algorithm_parameter algorithmParameter(boost::uuids::uuid id) const;

  /*!
   * \brief get the type of a defined algorithm
   * \param id the id of the algorithm to query.
   * \return the AlgorithmType of the queried algorithm.
   */
  AlgorithmType algorithmType(boost::uuids::uuid id) const;

  /*!
   * \brief is the algorithm defined in this factory
   * Determines whether the creation of algorithms with the provided id is supported by this
   * factory.
   * \param id the id of the algorithm to query.
   * \return true if the factory provides the creation of an algorithm of this id.
   */
  bool isAlgorithmDefined(boost::uuids::uuid id) const;
  //!\}

  //!\name Generation
  //! \{
  /*!
   * \brief Create an algorithm
   * This method creates an algorithm. If the provided description isn't registered in this
   * factory, a nullptr is returned.
   * \param description the description of the algorithm to generate.
   * \param event the event structure the new algorithm shall work on.
   * \param setup the setup the new algorithm shall reference to.
   * \return a shared-pointer to the newly created algorithm.
   * \todo creation function for algorithms: add mutex-variables
   */
  std::shared_ptr<AAlgorithm> create(algorithm_parameter const& description, TEvent& event,
                                     TSetup const& setup) const;
  //! \}

  //! \name Management
  //! \{
  /*!
   * This type defines the functor type to create an algorithm. The values for the algorithm are
   * taken from the algorithm_parameter. The provided event structure shall be the structure the
   * algorithm is working on and the provided setup is the one the algorithm is working on.
   */
  using AlgorithmCreation_t = std::function<std::shared_ptr<AAlgorithm>(algorithm_parameter const&,
                                                                        TEvent&, TSetup const&)>;

  /*!
   * \brief add an algorithm to the factory
   * This method adds the generation of an algorithm to the factory.
   * \param description a general description of the algorithm containing all needed parameters
   * for the algorithm to work. They don't need to be valid values though at this stage. The only
   * value, that must be correct, is the uuid.
   * \param algorithmType the type of the algorithm.
   * \param creationFunction the functor for creating the algorithm.
   * \return true if registering the algorithm was successful.
   * \note All subscribers to the algorithm added signal will be notified.
   */
  bool addAlgorithmToFactory(algorithm_parameter const& description, AlgorithmType algorithmType,
                             AlgorithmCreation_t creationFunction);

  /*!
   * \brief remove an algorithm from the factory
   * This method removes a generation of an algorithm from the factory.
   * \param id the id of the algoritm to remove.
   * \return true if unregistering was successful.
   * \note All subscribers to the algorithm removed signal will be notified.
   */
  bool removeAlgorithmFromFactory(boost::uuids::uuid id);
  //! \}

  //! \name Signalling
  //! \{
  /*!
   * Functor type for notifications for added algorithm signal. The parameter is the id of the
   * added algorithm
   */
  using AlgorithmAddedSignal_t = std::function<void(boost::uuids::uuid)>;

  /*!
   * \brief subscribe to Algorithm Added signal
   * Use this method to subscibe to the algorithm added signal.
   * \param subscriber the functor to be called in case an algorithm has been added.
   * \return the connection.
   */
  boost::signals2::connection subscribeAlgorithmAdded(AlgorithmAddedSignal_t subscriber);

  /*!
   * Functor type for notifications for removed algorithm signal. The parameter is the id of the
   * removed algorithm
   */
  using AlgorithmRemovedSignal_t = std::function<void(boost::uuids::uuid)>;

  /*!
   * \brief subscribe to Algorithm Removed signal
   * Use this method to subscibe to the algorithm removed signal.
   * \param subscriber the functor to be called in case an algorithm has been removed.
   * \return the connection.
   */
  boost::signals2::connection subscribeAlgorithmRemoved(AlgorithmRemovedSignal_t subscriber);
  //! \}

  /*!
   * \brief get the singleton instance
   * \return the one instance of algorithm factory.
   */
  static AlgorithmFactory& getInstance();

private:
  struct Configuration
  {
    AlgorithmCreation_t creation;
    algorithm_parameter parameter;
    AlgorithmType algorithm_type;
  };

  std::map<boost::uuids::uuid, Configuration> _installed_algorithms;
  boost::signals2::signal<void(boost::uuids::uuid)> _algorithmAdded;
  boost::signals2::signal<void(boost::uuids::uuid)> _algorithmRemoved;
};

class FactoryAlgorithmProvider
{
public:
  virtual ~FactoryAlgorithmProvider() = default;
  virtual void addToFactory(AlgorithmFactory& factory) const = 0;
  virtual void removeFromFactory(AlgorithmFactory& factory) const = 0;

  static std::vector<std::shared_ptr<FactoryAlgorithmProvider>> getProviders()
  {
    return _installedProviders;
  }

protected:
  static std::vector<std::shared_ptr<FactoryAlgorithmProvider>> _installedProviders;
};
