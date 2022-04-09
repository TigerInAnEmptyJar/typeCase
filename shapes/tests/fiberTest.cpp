/*!
 * \file fiberTest.cpp
 * Contains tests for shape fiber.
 */

#include "fiber.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid fiber_id1 = {{0xff, 0x39, 0x63, 0x7e, 0x11, 0xd9, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid fiber_id2 = {{0x06, 0x45, 0x30, 0xbc, 0x11, 0xda, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(FiberTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(fiber_id1));
  ASSERT_TRUE(factory.isShapeDefined(fiber_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(fiber_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(fiber_id2));

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, fiber1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(fiber_id1);

  // lets create a stack of fibers of the following shape:
  //         _  _  _  _  _  _  _  _  _
  // 5 |   /| || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || ||_|| || || || |
  // 0 |   || || || || | _ | || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   ||_||_||_||_||_||_||_||_||_|
  //-5 |   |/_//_//_//_//_//_//_//_//_/
  //   |
  //   |___________________________________
  //         -5          0           5

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{10, 0, 0};
  parameter.value(2) = vector3D{0, 1, 0};
  parameter.value(3) = vector3D{0, 0, 1};
  parameter.value(4) = static_cast<int>(1);
  parameter.value(5) = static_cast<int>(4);

  std::vector<point3D> corners;
  corners.push_back({-5, -4.5, 0});
  corners.push_back({-5, -3.5, 0});
  corners.push_back({-5, -2.5, 0});
  corners.push_back({-5, -1.5, 0});
  corners.push_back({-5, -0.5, 0});
  corners.push_back({0, -0.5, 0});
  corners.push_back({-5, 0.5, 0});
  corners.push_back({-5, 1.5, 0});
  corners.push_back({-5, 2.5, 0});
  corners.push_back({-5, 3.5, 0});

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("fiber", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<fiber>(shape));
  auto fiber_sh = dynamic_pointer_cast<fiber>(shape);

  EXPECT_EQ(corners[0], fiber_sh->getCorner());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), fiber_sh->getDirection(0));
  EXPECT_EQ(parameter.value(2).value<vector3D>(), fiber_sh->getDirection(1));
  EXPECT_EQ(parameter.value(3).value<vector3D>(), fiber_sh->getDirection(2));
  EXPECT_EQ(parameter.value(4).value<int>(), fiber_sh->getHalved());
  EXPECT_EQ(parameter.value(5).value<int>(), fiber_sh->getHalvedAt());

  // still a bug until booleans are enabled
  parameter.value(0) = point3D{0, 0, 0} - (corners[0] - point3D{0, 0, 0});
  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<fiber>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(corners[i], next_sh->getCorner());
    if (i == 4 || i == 5) {
      EXPECT_EQ(parameter.value(1).value<vector3D>() * 0.5, next_sh->getDirection(0));
    } else {
      EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getDirection(0));
    }
    EXPECT_EQ(parameter.value(2).value<vector3D>(), next_sh->getDirection(1));
    EXPECT_EQ(parameter.value(3).value<vector3D>(), next_sh->getDirection(2));
    EXPECT_EQ(parameter.value(4).value<int>(), next_sh->getHalved());
    EXPECT_EQ(parameter.value(5).value<int>(), next_sh->getHalvedAt());
  }

  auto envelopeShape = factory.createEnvelope(parameter, 10);
  EXPECT_NE(nullptr, envelopeShape);

  auto envelope_sh = dynamic_pointer_cast<fiber>(shape);
  ASSERT_NE(nullptr, envelope_sh);
  auto envelopeCorner = parameter.value(0).value<point3D>() - vector3D{10, 9, 0};
  EXPECT_EQ(envelopeCorner, envelope_sh->getCorner());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getDirection(0));
  //    EXPECT_EQ(parameter.value(2).value<vector3D>() *9, envelope_sh->getDirection(1));
  EXPECT_EQ(parameter.value(3).value<vector3D>(), envelope_sh->getDirection(2));
  EXPECT_EQ(parameter.value(4).value<int>(), envelope_sh->getHalved());
  EXPECT_EQ(parameter.value(5).value<int>(), envelope_sh->getHalvedAt());

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, fiber2)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(fiber_id2);

  // lets create a stack of fibers of the following shape:
  //                     _
  //                  _ | | _
  //               _ | || || | _
  //            _ | || || || || | _
  //         _ | || || || || || || | _
  // 5 |   /| || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || ||_|| || || || |
  // 0 |   || || || || | _ | || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   || || || || || || || || || |
  //   |   ||_|| || || || || || || ||_|
  //-5 |   |/_/|_|| || || || || ||_|/_/
  //   |       /_/|_|| || || ||_|/_/
  //   |          /_/|_|| ||_|/_/
  //   |             /_/|_|/_/
  //   |                /_/
  //   |
  //   |___________________________________
  //         -5          0           5

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{10, 0, 0};
  parameter.value(2) = vector3D{0, 1, 0};
  parameter.value(3) = vector3D{0, 0, 1};
  parameter.value(4) = static_cast<int>(1);
  parameter.value(5) = static_cast<int>(4);

  std::vector<point3D> corners;
  corners.push_back({-5, -4.5, 0});
  corners.push_back({-6, -3.5, 0});
  corners.push_back({-7, -2.5, 0});
  corners.push_back({-8, -1.5, 0});
  corners.push_back({-9, -0.5, 0});
  corners.push_back({0, -0.5, 0});
  corners.push_back({-8, 0.5, 0});
  corners.push_back({-7, 1.5, 0});
  corners.push_back({-6, 2.5, 0});
  corners.push_back({-5, 3.5, 0});

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("fiber", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<fiber>(shape));
  auto fiber_sh = dynamic_pointer_cast<fiber>(shape);

  EXPECT_EQ(corners[0], fiber_sh->getCorner());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), fiber_sh->getDirection(0));
  EXPECT_EQ(parameter.value(2).value<vector3D>(), fiber_sh->getDirection(1));
  EXPECT_EQ(parameter.value(3).value<vector3D>(), fiber_sh->getDirection(2));
  EXPECT_EQ(parameter.value(4).value<int>(), fiber_sh->getHalved());
  EXPECT_EQ(parameter.value(5).value<int>(), fiber_sh->getHalvedAt());

  // still a bug until booleans are enabled
  parameter.value(0) = point3D{0, 0, 0} - (corners[0] - point3D{0, 0, 0});
  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<fiber>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    /// \todo fix with boolean parameters
    //        EXPECT_EQ(corners[i], next_sh->getCorner());
    //        if (i == 4 || i == 5) {
    //            EXPECT_EQ(parameter.getParam<vector3D>(0) *0.5, next_sh->getDirection(0));
    //        }else{
    //            EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getDirection(0));
    //        }
    EXPECT_EQ(parameter.value(2).value<vector3D>(), next_sh->getDirection(1));
    EXPECT_EQ(parameter.value(3).value<vector3D>(), next_sh->getDirection(2));
    EXPECT_EQ(parameter.value(4).value<int>(), next_sh->getHalved());
    EXPECT_EQ(parameter.value(5).value<int>(), next_sh->getHalvedAt());
  }

  auto envelopeShape = factory.createEnvelope(parameter, 10);
  EXPECT_NE(nullptr, envelopeShape);

  auto envelope_sh = dynamic_pointer_cast<fiber>(shape);
  ASSERT_NE(nullptr, envelope_sh);
  auto envelopeCorner = parameter.value(0).value<point3D>() - vector3D{10, 9, 0};
  EXPECT_EQ(envelopeCorner, envelope_sh->getCorner());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getDirection(0));
  //    EXPECT_EQ(parameter.value(2).value<vector3D>() *9, envelope_sh->getDirection(1));
  EXPECT_EQ(parameter.value(3).value<vector3D>(), envelope_sh->getDirection(2));
  EXPECT_EQ(parameter.value(4).value<int>(), envelope_sh->getHalved());
  EXPECT_EQ(parameter.value(5).value<int>(), envelope_sh->getHalvedAt());

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  // fiber 1
  {
    auto parameter = factory.shapeParameter(fiber_id1);

    EXPECT_EQ("fiber", parameter.getName());
    EXPECT_EQ(fiber_id1, parameter.id());

    ASSERT_EQ(7, parameter.numberOfValues());
    EXPECT_EQ("center of envelope", parameter.valueName(0));
    EXPECT_EQ("length", parameter.valueName(1));
    EXPECT_EQ("width", parameter.valueName(2));
    EXPECT_EQ("thickness", parameter.valueName(3));
    EXPECT_EQ("number of halved elements", parameter.valueName(4));
    EXPECT_EQ("halved at", parameter.valueName(5));
    EXPECT_EQ("useCorner", parameter.valueName(6));
  }

  // fiber 2
  {
    auto parameter = factory.shapeParameter(fiber_id2);

    EXPECT_EQ("fiber", parameter.getName());
    EXPECT_EQ(fiber_id2, parameter.id());

    ASSERT_EQ(7, parameter.numberOfValues());
    EXPECT_EQ("center of envelope", parameter.valueName(0));
    EXPECT_EQ("length", parameter.valueName(1));
    EXPECT_EQ("width", parameter.valueName(2));
    EXPECT_EQ("thickness", parameter.valueName(3));
    EXPECT_EQ("number of halved elements", parameter.valueName(4));
    EXPECT_EQ("halved at", parameter.valueName(5));
    EXPECT_EQ("useCorner", parameter.valueName(6));
  }

  Shape::removeShapesFromFactory(factory);
}
