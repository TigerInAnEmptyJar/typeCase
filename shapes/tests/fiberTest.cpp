/*!
 * \file fiberTest.cpp
 * Contains tests for shape fiber.
 */

#include "fiber.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(fiber_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(fiber_id2));

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, fiber1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(fiber_id1);

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

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {10, 0, 0});
  parameter.setParam<vector3D>(1, {0, 1, 0});
  parameter.setParam<vector3D>(2, {0, 0, 1});
  parameter.setParam<int>(0, 1);
  parameter.setParam<int>(1, 4);

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

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("fiber", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<fiber>(shape));
  auto fiber_sh = dynamic_pointer_cast<fiber>(shape);

  EXPECT_EQ(corners[0], fiber_sh->getCorner());
  EXPECT_EQ(parameter.getParam<vector3D>(0), fiber_sh->getDirection(0));
  EXPECT_EQ(parameter.getParam<vector3D>(1), fiber_sh->getDirection(1));
  EXPECT_EQ(parameter.getParam<vector3D>(2), fiber_sh->getDirection(2));
  EXPECT_EQ(parameter.getParam<int>(0), fiber_sh->getHalved());
  EXPECT_EQ(parameter.getParam<int>(1), fiber_sh->getHalvedAt());

  // still a bug until booleans are enabled
  parameter.setParam<point3D>(0, point3D{0, 0, 0} - (corners[0] - point3D{0, 0, 0}));
  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<fiber>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(corners[i], next_sh->getCorner());
    if (i == 4 || i == 5) {
      EXPECT_EQ(parameter.getParam<vector3D>(0) * 0.5, next_sh->getDirection(0));
    } else {
      EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getDirection(0));
    }
    EXPECT_EQ(parameter.getParam<vector3D>(1), next_sh->getDirection(1));
    EXPECT_EQ(parameter.getParam<vector3D>(2), next_sh->getDirection(2));
    EXPECT_EQ(parameter.getParam<int>(0), next_sh->getHalved());
    EXPECT_EQ(parameter.getParam<int>(1), next_sh->getHalvedAt());
  }

  auto envelopeShape = factory.getEnvelope(parameter, 10);
  EXPECT_NE(nullptr, envelopeShape);

  auto envelope_sh = dynamic_pointer_cast<fiber>(shape);
  ASSERT_NE(nullptr, envelope_sh);
  auto envelopeCorner = parameter.getParam<point3D>(0) - vector3D{10, 9, 0};
  EXPECT_EQ(envelopeCorner, envelope_sh->getCorner());
  EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getDirection(0));
  //    EXPECT_EQ(parameter.getParam<vector3D>(1) *9, envelope_sh->getDirection(1));
  EXPECT_EQ(parameter.getParam<vector3D>(2), envelope_sh->getDirection(2));
  EXPECT_EQ(parameter.getParam<int>(0), envelope_sh->getHalved());
  EXPECT_EQ(parameter.getParam<int>(1), envelope_sh->getHalvedAt());

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, fiber2)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(fiber_id2);

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

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {10, 0, 0});
  parameter.setParam<vector3D>(1, {0, 1, 0});
  parameter.setParam<vector3D>(2, {0, 0, 1});
  parameter.setParam<int>(0, 1);
  parameter.setParam<int>(1, 4);

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

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("fiber", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<fiber>(shape));
  auto fiber_sh = dynamic_pointer_cast<fiber>(shape);

  EXPECT_EQ(corners[0], fiber_sh->getCorner());
  EXPECT_EQ(parameter.getParam<vector3D>(0), fiber_sh->getDirection(0));
  EXPECT_EQ(parameter.getParam<vector3D>(1), fiber_sh->getDirection(1));
  EXPECT_EQ(parameter.getParam<vector3D>(2), fiber_sh->getDirection(2));
  EXPECT_EQ(parameter.getParam<int>(0), fiber_sh->getHalved());
  EXPECT_EQ(parameter.getParam<int>(1), fiber_sh->getHalvedAt());

  // still a bug until booleans are enabled
  parameter.setParam<point3D>(0, point3D{0, 0, 0} - (corners[0] - point3D{0, 0, 0}));
  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
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
    EXPECT_EQ(parameter.getParam<vector3D>(1), next_sh->getDirection(1));
    EXPECT_EQ(parameter.getParam<vector3D>(2), next_sh->getDirection(2));
    EXPECT_EQ(parameter.getParam<int>(0), next_sh->getHalved());
    EXPECT_EQ(parameter.getParam<int>(1), next_sh->getHalvedAt());
  }

  auto envelopeShape = factory.getEnvelope(parameter, 10);
  EXPECT_NE(nullptr, envelopeShape);

  auto envelope_sh = dynamic_pointer_cast<fiber>(shape);
  ASSERT_NE(nullptr, envelope_sh);
  auto envelopeCorner = parameter.getParam<point3D>(0) - vector3D{10, 9, 0};
  EXPECT_EQ(envelopeCorner, envelope_sh->getCorner());
  EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getDirection(0));
  //    EXPECT_EQ(parameter.getParam<vector3D>(1) *9, envelope_sh->getDirection(1));
  EXPECT_EQ(parameter.getParam<vector3D>(2), envelope_sh->getDirection(2));
  EXPECT_EQ(parameter.getParam<int>(0), envelope_sh->getHalved());
  EXPECT_EQ(parameter.getParam<int>(1), envelope_sh->getHalvedAt());

  Shape::removeShapesFromFactory(factory);
}

TEST(FiberTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  // fiber 1
  {
    auto parameter = factory.getShapeParameter(fiber_id1);

    EXPECT_EQ("fiber", parameter.getName());
    EXPECT_EQ(fiber_id1, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(3, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(2, parameter.NumberOfParams<int>());
    ASSERT_EQ(0, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    //        EXPECT_EQ(1, parameter.NumberOfParams<bool>());
    EXPECT_EQ("center of envelope", parameter.getParamName<point3D>(0));
    EXPECT_EQ("length", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("width", parameter.getParamName<vector3D>(1));
    EXPECT_EQ("thickness", parameter.getParamName<vector3D>(2));
    EXPECT_EQ("number of halved elements", parameter.getParamName<int>(0));
    EXPECT_EQ("halved at", parameter.getParamName<int>(1));
    //        EXPECT_EQ("useCorner", parameter.getParamName<bool>(0));
  }

  // fiber 2
  {
    auto parameter = factory.getShapeParameter(fiber_id2);

    EXPECT_EQ("fiber", parameter.getName());
    EXPECT_EQ(fiber_id2, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(3, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(2, parameter.NumberOfParams<int>());
    ASSERT_EQ(0, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    //        EXPECT_EQ(1, parameter.NumberOfParams<bool>());
    EXPECT_EQ("center of envelope", parameter.getParamName<point3D>(0));
    EXPECT_EQ("length", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("width", parameter.getParamName<vector3D>(1));
    EXPECT_EQ("thickness", parameter.getParamName<vector3D>(2));
    EXPECT_EQ("number of halved elements", parameter.getParamName<int>(0));
    EXPECT_EQ("halved at", parameter.getParamName<int>(1));
    //        EXPECT_EQ("useCorner", parameter.getParamName<bool>(0));
  }

  Shape::removeShapesFromFactory(factory);
}
