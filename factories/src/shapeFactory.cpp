#include "shapeFactory.h"

#include <algorithm>

std::vector<std::shared_ptr<FactoryShapeProvider>> FactoryShapeProvider::installedProviders;

std::vector<shape_parameter> ShapeFactory::getDefinedShapes() const
{
  std::vector<shape_parameter> result;
  std::transform(std::begin(installed_shapes), std::end(installed_shapes),
                 std::back_inserter(result),
                 [](std::pair<boost::uuids::uuid, ShapeFactory::Configuration> const& element) {
                   return element.second.parameter;
                 });
  return result;
}

shape_parameter ShapeFactory::getShapeParameter(boost::uuids::uuid id) const
{
  auto theShape = installed_shapes.find(id);
  if (theShape == installed_shapes.end()) {
    return {};
  }
  return theShape->second.parameter;
}

ShapeType ShapeFactory::getShapeType(boost::uuids::uuid id) const
{
  auto theShape = installed_shapes.find(id);
  if (theShape == installed_shapes.end()) {
    return ShapeType::Undefined;
  }
  return theShape->second.shape_type;
}

bool ShapeFactory::isShapeDefined(boost::uuids::uuid const& id) const
{
  return installed_shapes.find(id) != installed_shapes.end();
}

std::shared_ptr<volumeShape> ShapeFactory::getVolume(shape_parameter const& shape) const
{
  auto theShape = installed_shapes.find(shape.getId());
  if (theShape == installed_shapes.end()) {
    return nullptr;
  }

  if (theShape->second.shape_type != ShapeType::VolumeShape) {
    return nullptr;
  }
  return dynamic_pointer_cast<volumeShape>(theShape->second.creation(shape));
}

std::shared_ptr<volumeShape> ShapeFactory::getNext(shape_parameter const& first_element,
                                                   size_t num) const
{
  auto theShape = installed_shapes.find(first_element.getId());
  if (theShape == installed_shapes.end()) {
    return nullptr;
  }

  if (theShape->second.shape_type != ShapeType::VolumeShape) {
    return nullptr;
  }

  return getVolume(theShape->second.series(first_element, num));
}

std::shared_ptr<volumeShape> ShapeFactory::getEnvelope(shape_parameter const& first_element,
                                                       size_t num) const
{
  auto theShape = installed_shapes.find(first_element.getId());
  if (theShape == installed_shapes.end()) {
    return nullptr;
  }

  if (theShape->second.shape_type != ShapeType::VolumeShape) {
    return nullptr;
  }

  return getVolume(theShape->second.envelope(first_element, num));
}

std::shared_ptr<planeShape> ShapeFactory::getPlane(shape_parameter const& shape) const
{
  auto theShape = installed_shapes.find(shape.getId());
  if (theShape == installed_shapes.end()) {
    return nullptr;
  }

  if (theShape->second.shape_type != ShapeType::PlanarShape) {
    return nullptr;
  }
  return dynamic_pointer_cast<planeShape>(theShape->second.creation(shape));
}

std::shared_ptr<base_shape> ShapeFactory::getShape(shape_parameter const& shape) const
{
  auto theShape = installed_shapes.find(shape.getId());
  if (theShape == installed_shapes.end()) {
    return nullptr;
  }

  return theShape->second.creation(shape);
}

bool ShapeFactory::addShapeToFactory(shape_parameter const& shape, ShapeType shapeType,
                                     ShapeFactory::ShapeCreation_t creationFunction,
                                     ShapeFactory::SeriesCreation_t seriesCreation,
                                     ShapeFactory::EnvelopeCreation_t envelopeCreation)
{
  if (isShapeDefined(shape.getId())) {
    return false;
  }

  Configuration element{creationFunction, seriesCreation, envelopeCreation, shape, shapeType};
  installed_shapes.insert(std::make_pair(shape.getId(), element));

  shapeAdded(shape.getId());

  return true;
}

bool ShapeFactory::removeShapeFromFactory(boost::uuids::uuid const& id)
{
  auto theShape = installed_shapes.find(id);
  if (theShape == installed_shapes.end()) {
    return false;
  }

  installed_shapes.erase(theShape);

  shapeRemoved(id);

  return true;
}

boost::signals2::connection ShapeFactory::subscribeShapeAdded(
    ShapeFactory::ShapeAddedSignal_t subscriber)
{
  return shapeAdded.connect(subscriber);
}

boost::signals2::connection ShapeFactory::subscribeShapeRemoved(
    ShapeFactory::ShapeRemovedSignal_t subscriber)
{
  return shapeRemoved.connect(subscriber);
}

ShapeFactory& ShapeFactory::getInstance()
{
  static ShapeFactory instance;
  return instance;
}
