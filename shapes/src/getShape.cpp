#include "shapeFactory.h"
#include "shapeparameter.h"
#include "shapes.h"

std::shared_ptr<base_shape> getShape(const shape_parameter& sp)
{
  auto& factory = ShapeFactory::getInstance();
  auto shape = factory.createShape(sp);

  return shape;
}
