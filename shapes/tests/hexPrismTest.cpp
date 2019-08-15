/*!
 * \file hexPrismTest.cpp
 * Contains tests for shape hexPrism.
 */

#include "hexPrism.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid hexPrism_id1 = {{0x10, 0xd9, 0x58, 0x58, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid hexPrism_id2 = {{0xa2, 0x0c, 0x6c, 0x70, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid hexPrism_id3 = {{0xe2, 0x71, 0x8b, 0x10, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(HexprismTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(hexPrism_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(hexPrism_id1));
  ASSERT_TRUE(factory.isShapeDefined(hexPrism_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(hexPrism_id2));
  ASSERT_TRUE(factory.isShapeDefined(hexPrism_id3));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(hexPrism_id3));

  Shape::removeShapesFromFactory(factory);
}

TEST(HexprismTest, hexprism1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(hexPrism_id1);

  // let's create a stack of 10 spirals .

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, -1};
  parameter.value(2) = vector3D{matrix3D{{0, 0, -1}, 60. * M_PI / 180.} * vector3D{1, 0, 0}};
  parameter.value(3) = vector3D{1, 0, 0};
  parameter.value(4) = static_cast<int>(1);

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("hexPrism", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<hexPrism>(shape));
  auto hexprism_sh = dynamic_pointer_cast<hexPrism>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), hexprism_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), hexprism_sh->getDirection(0));
  EXPECT_EQ(parameter.value(2).value<vector3D>(), hexprism_sh->getDirection(1));
  EXPECT_EQ(parameter.value(3).value<vector3D>(), hexprism_sh->getDirection(2));

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<hexPrism>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.value(0).value<point3D>() + i * parameter.value(3).value<vector3D>(),
              next_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getDirection(0));
    EXPECT_EQ(parameter.value(2).value<vector3D>(), next_sh->getDirection(1));
    EXPECT_EQ(parameter.value(3).value<vector3D>(), next_sh->getDirection(2));
  }

  {
    auto envelopeShape = factory.createEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("hexPrism", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<hexPrism>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getDirection(0));
    EXPECT_EQ(parameter.value(2).value<vector3D>(), envelope_sh->getDirection(1));
    EXPECT_EQ(parameter.value(3).value<vector3D>() * 10, envelope_sh->getDirection(2));
  }

  Shape::removeShapesFromFactory(factory);
}

/// \todo Implement tests for the other two stacking types.

TEST(HexprismTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  // hexprism type 1
  {
    auto parameter = factory.shapeParameter(hexPrism_id1);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id1, parameter.id());

    ASSERT_EQ(5, parameter.numberOfValues());
    EXPECT_EQ("center of front", parameter.valueName(0));
    EXPECT_EQ("thickness vector", parameter.valueName(1));
    EXPECT_EQ("vector to first point", parameter.valueName(2));
    EXPECT_EQ("key width vector", parameter.valueName(3));
    EXPECT_EQ("stack parameter", parameter.valueName(4));
  }

  // hexprism type 2
  {
    auto parameter = factory.shapeParameter(hexPrism_id3);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id3, parameter.id());

    ASSERT_EQ(5, parameter.numberOfValues());
    EXPECT_EQ("center of front", parameter.valueName(0));
    EXPECT_EQ("thickness vector", parameter.valueName(1));
    EXPECT_EQ("vector to first point", parameter.valueName(2));
    EXPECT_EQ("key width vector", parameter.valueName(3));
    EXPECT_EQ("stack parameter", parameter.valueName(4));
  }

  // hexprism type 3
  {
    auto parameter = factory.shapeParameter(hexPrism_id3);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id3, parameter.id());

    ASSERT_EQ(5, parameter.numberOfValues());
    EXPECT_EQ("center of front", parameter.valueName(0));
    EXPECT_EQ("thickness vector", parameter.valueName(1));
    EXPECT_EQ("vector to first point", parameter.valueName(2));
    EXPECT_EQ("key width vector", parameter.valueName(3));
    EXPECT_EQ("stack parameter", parameter.valueName(4));
  }

  Shape::removeShapesFromFactory(factory);
}
