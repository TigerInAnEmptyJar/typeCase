#include "algorithm.h"

#include "algorithmparameter.h"

AAlgorithm::AAlgorithm(std::string n) : name(n) {}

std::string AAlgorithm::getName() const { return name; }

algorithm_parameter AAlgorithm::getDescription()
{
  return algorithm_parameter("none", algorithm_parameter::Category::ELSE, 0);
}
