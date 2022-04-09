#include "provideShapes.h"

#include "shapes.h"

namespace Shape {

std::vector<std::shared_ptr<FactoryShapeProvider>> innerShapeProviders;

void addShapesToFactory(ShapeFactory& factory)
{
  std::for_each(std::begin(innerShapeProviders), std::end(innerShapeProviders),
                [&factory](auto provider) { provider->addToFactory(factory); });
}

void removeShapesFromFactory(ShapeFactory& factory)
{
  std::for_each(std::begin(innerShapeProviders), std::end(innerShapeProviders),
                [&factory](auto provider) { provider->removeFromFactory(factory); });
}
}

auto result __attribute__((__used__)) = [] {
  // In case of static linkage, as with most stuff in C++: you only pay for what you use. So only
  // used symbols are linked to the final binary. This translation unit is used, because it contains
  // the two functions to add and remove the basic shapes to and from a factory.
  // But even though the innerShapeProviders structure is used in the translation units of all
  // shape sources, it will contain only the providers of used translation units.
  // This method forces all shapes of this static library to be loaded, which forces the factory
  // providers to be created.
  circle c{{}, {}, {}};
  cylinder cy;
  fiber f;
  hexPrism hp{{}, {}, {}, {}, {}};
  triangle tr;
  sphericRectangle sr;
  sphericTriangle st{{}, {}, {}, {}};
  hexagon he;
  quadrangle q;
  ring r;
  spiral sp;
  strawTube str{{}, {}, {}, {}, {}, {}, {}, {}};
  wedge w;
  return true;
}();
