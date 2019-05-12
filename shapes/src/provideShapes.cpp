#include "provideShapes.h"

namespace Shape {

std::vector<std::shared_ptr<FactoryShapeProvider>> innerShapeProviders;

void addShapesToFactory(ShapeFactory& factory)
{
  auto& providers = innerShapeProviders;
  std::for_each(std::begin(providers), std::end(providers),
                [&factory](auto provider) { provider->addToFactory(factory); });
}

void removeShapesFromFactory(ShapeFactory& factory)
{
  auto& providers = innerShapeProviders;
  std::for_each(std::begin(providers), std::end(providers),
                [&factory](auto provider) { provider->removeFromFactory(factory); });
}
}
#include "strawTube.h"
void initEveryThn()
{
  strawTube s;
}
