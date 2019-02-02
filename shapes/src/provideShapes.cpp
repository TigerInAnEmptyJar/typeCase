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
#include "shapes.h"
void initEveryThn()
{
  circle c{{}};
  cylinder cy;
  fiber f;
  sphericRectangle sr;
  strawTube s;
  rectangle r;
}
