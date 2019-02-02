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
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(hexPrism_id1));
  ASSERT_TRUE(factory.isShapeDefined(hexPrism_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(hexPrism_id2));
  ASSERT_TRUE(factory.isShapeDefined(hexPrism_id3));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(hexPrism_id3));

  Shape::removeShapesFromFactory(factory);
}

TEST(HexprismTest, hexprism1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(hexPrism_id1);

  // let's create a stack of 10 spirals .

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, -1});
  parameter.setParam<vector3D>(1, matrix3D{{0, 0, -1}, 60. * M_PI / 180.} * vector3D{1, 0, 0});
  parameter.setParam<vector3D>(2, {1, 0, 0});
  parameter.setParam<int>(0, 1);

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("hexPrism", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<hexPrism>(shape));
  auto hexprism_sh = dynamic_pointer_cast<hexPrism>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), hexprism_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), hexprism_sh->getDirection(0));
  EXPECT_EQ(parameter.getParam<vector3D>(1), hexprism_sh->getDirection(1));
  EXPECT_EQ(parameter.getParam<vector3D>(2), hexprism_sh->getDirection(2));

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<hexPrism>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.getParam<point3D>(0) + i * parameter.getParam<vector3D>(2),
              next_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getDirection(0));
    EXPECT_EQ(parameter.getParam<vector3D>(1), next_sh->getDirection(1));
    EXPECT_EQ(parameter.getParam<vector3D>(2), next_sh->getDirection(2));
  }

  {
    auto envelopeShape = factory.getEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("hexPrism", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<hexPrism>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getDirection(0));
    EXPECT_EQ(parameter.getParam<vector3D>(1), envelope_sh->getDirection(1));
    EXPECT_EQ(parameter.getParam<vector3D>(2) * 10, envelope_sh->getDirection(2));
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
    auto parameter = factory.getShapeParameter(hexPrism_id1);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id1, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(3, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(1, parameter.NumberOfParams<int>());
    ASSERT_EQ(0, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    EXPECT_EQ("center of front", parameter.getParamName<point3D>(0));
    EXPECT_EQ("thickness vector", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("vector to first point", parameter.getParamName<vector3D>(1));
    EXPECT_EQ("key width vector", parameter.getParamName<vector3D>(2));
    EXPECT_EQ("stack parameter", parameter.getParamName<int>(0));
  }

  // hexprism type 2
  {
    auto parameter = factory.getShapeParameter(hexPrism_id3);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id3, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(3, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(1, parameter.NumberOfParams<int>());
    ASSERT_EQ(0, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    EXPECT_EQ("center of front", parameter.getParamName<point3D>(0));
    EXPECT_EQ("thickness vector", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("vector to first point", parameter.getParamName<vector3D>(1));
    EXPECT_EQ("key width vector", parameter.getParamName<vector3D>(2));
    EXPECT_EQ("stack parameter", parameter.getParamName<int>(0));
  }

  // hexprism type 3
  {
    auto parameter = factory.getShapeParameter(hexPrism_id3);

    EXPECT_EQ("hexPrism", parameter.getName());
    EXPECT_EQ(hexPrism_id3, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(3, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(1, parameter.NumberOfParams<int>());
    ASSERT_EQ(0, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    EXPECT_EQ("center of front", parameter.getParamName<point3D>(0));
    EXPECT_EQ("thickness vector", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("vector to first point", parameter.getParamName<vector3D>(1));
    EXPECT_EQ("key width vector", parameter.getParamName<vector3D>(2));
    EXPECT_EQ("stack parameter", parameter.getParamName<int>(0));
  }

  Shape::removeShapesFromFactory(factory);
}
