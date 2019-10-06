#include "fitAlgorithm.h"
AFitAlgorithm::AFitAlgorithm(const string& name) : AAlgorithm(name) {}
AFitAlgorithm::~AFitAlgorithm() {}
float AFitAlgorithm::fit() { return -1; }
void AFitAlgorithm::clear() {}
void AFitAlgorithm::add(Vector& data)
{
  if (data.getDimension() == 0)
    return;
}
void AFitAlgorithm::getAddPoint(Vector& data)
{
  if (data.getDimension() == 0)
    return;
}
Vector AFitAlgorithm::getResult() { return Vector(0); }
