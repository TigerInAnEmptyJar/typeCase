/*!
 * \file cylinderTest.cpp
 * Contains tests for shape cylinder.
 */

#include "cylinder.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid cylinder_id = {{0xea, 0x32, 0xc9, 0x4c, 0x11, 0xc1, 0x11, 0xe9, 0xab, 0x14,
                                         0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(CylinderTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(cylinder_id));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(cylinder_id));

  auto parameter = factory.shapeParameter(cylinder_id);

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = static_cast<float>(10.1f);

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("cylinder", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<cylinder>(shape));
  auto cylinder_sh = dynamic_pointer_cast<cylinder>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), cylinder_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), cylinder_sh->getDirection());
  EXPECT_EQ(parameter.value(2).value<float>(), cylinder_sh->getRadius());

  // for cylinders no series generation or envelope is defined
  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(CylinderTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(cylinder_id);

  EXPECT_EQ("cylinder", parameter.getName());
  EXPECT_EQ(cylinder_id, parameter.id());

  ASSERT_EQ(3, parameter.numberOfValues());
  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("direction", parameter.valueName(1));
  EXPECT_EQ("radius", parameter.valueName(2));

  Shape::removeShapesFromFactory(factory);
}
