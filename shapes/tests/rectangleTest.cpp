/*!
 * \file rectangleTest.cpp
 * Contains tests for shape rectangle.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid rectangle_id = {{0x38, 0x9e, 0x9e, 0x10, 0x11, 0xec, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(RectangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(rectangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(rectangle_id));

  auto parameter = factory.shapeParameter(rectangle_id);

  parameter.setParam<point3D>(0, {1, 1, 0});
  parameter.setParam<point3D>(1, {1, -1, 0});
  parameter.setParam<point3D>(2, {-1, -1, 0});
  parameter.setParam<point3D>(3, {-1, 1, 0});
  auto normal = vector3D{0, 0, 1};
  auto center = point3D{0, 0, 0};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("rectangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<rectangle>(shape));

  auto rectangle_sh = dynamic_pointer_cast<rectangle>(shape);
  EXPECT_EQ(center, rectangle_sh->getCenter());
  EXPECT_EQ(normal, rectangle_sh->getNormal());
  EXPECT_EQ(parameter.getParam<point3D>(0), rectangle_sh->A());
  EXPECT_EQ(parameter.getParam<point3D>(1), rectangle_sh->B());
  EXPECT_EQ(parameter.getParam<point3D>(2), rectangle_sh->C());
  EXPECT_EQ(parameter.getParam<point3D>(3), rectangle_sh->D());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(RectangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(rectangle_id);

  EXPECT_EQ("rectangle", parameter.getName());
  EXPECT_EQ(rectangle_id, parameter.getId());

  ASSERT_EQ(4, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(0, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  EXPECT_EQ(0, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());

  EXPECT_EQ("A", parameter.getParamName<point3D>(0));
  EXPECT_EQ("B", parameter.getParamName<point3D>(1));
  EXPECT_EQ("C", parameter.getParamName<point3D>(2));
  EXPECT_EQ("D", parameter.getParamName<point3D>(3));

  Shape::removeShapesFromFactory(factory);
}
