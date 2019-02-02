/*!
 * \file providerTest.cpp
 * Contains tests for providers for shapes.
 */

#include "provideShapes.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(BasicProviderTest, instanceTest)
{
  auto& factory = ShapeFactory::getInstance();
  EXPECT_EQ(size_t{0}, factory.getDefinedShapes().size());
}

TEST(BasicProviderTest, installationTest)
{
  ShapeFactory factory;
  EXPECT_TRUE(factory.getDefinedShapes().empty());

  Shape::addShapesToFactory(factory);
  auto shapeList = factory.getDefinedShapes();
  ASSERT_EQ(size_t{19}, shapeList.size());

  Shape::removeShapesFromFactory(factory);
  EXPECT_TRUE(factory.getDefinedShapes().empty());
}
