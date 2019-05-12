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
  EXPECT_EQ(size_t{0}, factory.definedShapes().size());
}

TEST(BasicProviderTest, installationTest)
{
  ShapeFactory factory;
  EXPECT_TRUE(factory.definedShapes().empty());

  Shape::addShapesToFactory(factory);
  auto shapeList = factory.definedShapes();

  auto counter = [](std::vector<shape_parameter> const& shapelist, std::string const& name) {
    return std::count_if(std::begin(shapelist), std::end(shapelist),
                         [&name](auto s) { return s.getName() == name; });
  };

  EXPECT_EQ(1, counter(shapeList, "circle"));
  EXPECT_EQ(1, counter(shapeList, "hexagon"));
  EXPECT_EQ(1, counter(shapeList, "quadrangle"));
  EXPECT_EQ(1, counter(shapeList, "rectangle"));
  EXPECT_EQ(1, counter(shapeList, "sphericRectangle"));
  EXPECT_EQ(1, counter(shapeList, "sphericTriangle"));
  EXPECT_EQ(1, counter(shapeList, "triangle"));

  EXPECT_EQ(1, counter(shapeList, "cylinder"));
  EXPECT_EQ(2, counter(shapeList, "fiber"));
  EXPECT_EQ(3, counter(shapeList, "hexPrism"));
  EXPECT_EQ(2, counter(shapeList, "ring"));
  EXPECT_EQ(1, counter(shapeList, "spiral"));
  EXPECT_EQ(1, counter(shapeList, "strawTube"));
  EXPECT_EQ(2, counter(shapeList, "wedge"));

  ASSERT_EQ(size_t{19}, shapeList.size());

  Shape::removeShapesFromFactory(factory);
  EXPECT_TRUE(factory.definedShapes().empty());
}
