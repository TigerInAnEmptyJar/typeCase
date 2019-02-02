/*!
 * \file cylinderTest.cpp
 * Contains tests for shape cylinder.
 */

#include "cylinder.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid cylinder_id = {{0xea, 0x32, 0xc9, 0x4c, 0x11, 0xc1, 0x11, 0xe9, 0xab, 0x14,
                                         0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(CylinderTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(cylinder_id));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(cylinder_id));

  auto parameter = factory.getShapeParameter(cylinder_id);

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<float>(0, 10.1f);

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("cylinder", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<cylinder>(shape));
  auto cylinder_sh = dynamic_pointer_cast<cylinder>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), cylinder_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), cylinder_sh->getDirection());
  EXPECT_EQ(parameter.getParam<float>(0), cylinder_sh->getRadius());

  // for cylinders no series generation or envelope is defined
  auto nextShape = factory.getNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.getEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(CylinderTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(cylinder_id);

  EXPECT_EQ("cylinder", parameter.getName());
  EXPECT_EQ(cylinder_id, parameter.getId());

  ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(1, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  ASSERT_EQ(1, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("direction", parameter.getParamName<vector3D>(0));
  EXPECT_EQ("radius", parameter.getParamName<float>(0));

  Shape::removeShapesFromFactory(factory);
}
