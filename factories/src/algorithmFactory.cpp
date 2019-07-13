#include "algorithmFactory.h"

std::vector<algorithm_parameter> AlgorithmFactory::definedAlgorithms() const
{
  std::vector<algorithm_parameter> result;
  std::transform(std::begin(_installed_algorithms), std::end(_installed_algorithms),
                 std::back_inserter(result),
                 [](std::pair<boost::uuids::uuid, AlgorithmFactory::Configuration> const& element) {
                   return element.second.parameter;
                 });
  return result;
}

algorithm_parameter AlgorithmFactory::algorithmParameter(boost::uuids::uuid id) const
{
  auto theAlgorithm = _installed_algorithms.find(id);
  if (theAlgorithm == _installed_algorithms.end()) {
    return {};
  }
  return theAlgorithm->second.parameter;
}

AlgorithmType AlgorithmFactory::algorithmType(boost::uuids::uuid id) const
{
  auto theAlgorithm = _installed_algorithms.find(id);
  if (theAlgorithm == _installed_algorithms.end()) {
    return AlgorithmType::Undefined;
  }
  return theAlgorithm->second.algorithm_type;
}

bool AlgorithmFactory::isAlgorithmDefined(boost::uuids::uuid id) const
{
  return _installed_algorithms.find(id) != _installed_algorithms.end();
}

std::shared_ptr<AAlgorithm> AlgorithmFactory::create(const algorithm_parameter& description,
                                                     TEvent& event, const TSetup& setup) const
{
  auto theAlgorithm = _installed_algorithms.find(description.id());
  if (theAlgorithm == _installed_algorithms.end()) {
    return nullptr;
  }

  return theAlgorithm->second.creation(description, event, setup);
}

bool AlgorithmFactory::addAlgorithmToFactory(const algorithm_parameter& description,
                                             AlgorithmType algorithmType,
                                             AlgorithmFactory::AlgorithmCreation_t creationFunction)
{
  if (isAlgorithmDefined(description.id())) {
    return false;
  }

  Configuration element{creationFunction, description, algorithmType};
  _installed_algorithms.insert(std::make_pair(description.id(), element));

  _algorithmAdded(description.id());

  return true;
}

bool AlgorithmFactory::removeAlgorithmFromFactory(boost::uuids::uuid id)
{
  auto theAlgorithm = _installed_algorithms.find(id);
  if (theAlgorithm == _installed_algorithms.end()) {
    return false;
  }

  _installed_algorithms.erase(theAlgorithm);

  _algorithmRemoved(id);

  return true;
}

boost::signals2::connection AlgorithmFactory::subscribeAlgorithmAdded(
    AlgorithmFactory::AlgorithmAddedSignal_t subscriber)
{
  return _algorithmAdded.connect(subscriber);
}

boost::signals2::connection AlgorithmFactory::subscribeAlgorithmRemoved(
    AlgorithmFactory::AlgorithmRemovedSignal_t subscriber)
{
  return _algorithmRemoved.connect(subscriber);
}

AlgorithmFactory& AlgorithmFactory::getInstance()
{
  static AlgorithmFactory instance;
  return instance;
}

std::vector<std::shared_ptr<FactoryAlgorithmProvider>>
    FactoryAlgorithmProvider::_installedProviders;
