/*!
 * \file circleTest.cpp
 * Contains tests for shape circle.
 */

#include "circle.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid circle_id = {{0x14, 0x8b, 0xec, 0xc6, 0x11, 0xbf, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(CircleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(circle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(circle_id));

  auto parameter = factory.shapeParameter(circle_id);

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<float>(0, 101.1f);

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("circle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<circle>(shape));
  auto circle_sh = dynamic_pointer_cast<circle>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), circle_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), circle_sh->getNormal());
  EXPECT_EQ(parameter.getParam<float>(0), circle_sh->radius());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(CircleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(circle_id);

  EXPECT_EQ("circle", parameter.getName());
  EXPECT_EQ(circle_id, parameter.getId());

  ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(1, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  ASSERT_EQ(1, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));
  EXPECT_EQ("radius", parameter.getParamName<float>(0));

  Shape::removeShapesFromFactory(factory);
}
