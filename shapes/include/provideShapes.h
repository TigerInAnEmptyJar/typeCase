#pragma once
#include "shapeFactory.h"

#include <memory>

namespace Shape {
extern std::vector<std::shared_ptr<FactoryShapeProvider>> innerShapeProviders;
/*!
 * \brief adds a shape-provider
 * This function adds a shape-provider. For library internal use only.
 * \param provider the provider to add.
 */
// void addProvider(std::shared_ptr<FactoryShapeProvider> provider);

/*!
 * \brief adds provided shapes to factory
 * \param factory the factory to add the shapes to
 */
void addShapesToFactory(ShapeFactory& factory);

/*!
 * \brief remove provided shapes from factory
 * \param factory the factory to remove the shapes from
 */
void removeShapesFromFactory(ShapeFactory& factory);
} // namespace Shape
