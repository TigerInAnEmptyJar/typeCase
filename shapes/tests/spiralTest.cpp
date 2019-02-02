/*!
 * \file spiralTest.cpp
 * Contains tests for shape spiral.
 */

#include "spiral.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid spiral_id = {{0x6d, 0x8e, 0xdc, 0x56, 0x11, 0xf2, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(SpiralTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(spiral_id));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(spiral_id));

  Shape::removeShapesFromFactory(factory);
}

TEST(SpiralTest, spiral)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(spiral_id);

  // let's create a stack of 10 spirals .

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<vector3D>(1, {1, 0, 0});
  parameter.setParam<float>(0, 1.f);
  parameter.setParam<float>(1, 10.f);
  parameter.setParam<float>(2, static_cast<float>(30. * M_PI / 180.));
  parameter.setParam<float>(3, 1.f);
  parameter.setParam<float>(4, 1.f);

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("spiral", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<spiral>(shape));
  auto spiral_sh = dynamic_pointer_cast<spiral>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), spiral_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), spiral_sh->getNormal());
  EXPECT_EQ(parameter.getParam<vector3D>(1), spiral_sh->getLowerPhiEdge());
  EXPECT_EQ(parameter.getParam<float>(0), spiral_sh->getInnerRadius());
  EXPECT_EQ(parameter.getParam<float>(1), spiral_sh->getOuterRadius());
  EXPECT_EQ(parameter.getParam<float>(2), spiral_sh->getPhiRange());
  EXPECT_EQ(parameter.getParam<float>(3), spiral_sh->getThickness());
  EXPECT_EQ(parameter.getParam<float>(4), spiral_sh->getBending());

  std::vector<float> angles;
  angles.push_back(0);
  angles.push_back(static_cast<float>(30. * M_PI / 180.));
  angles.push_back(static_cast<float>(60. * M_PI / 180.));
  angles.push_back(static_cast<float>(90. * M_PI / 180.));
  angles.push_back(static_cast<float>(120. * M_PI / 180.));
  angles.push_back(static_cast<float>(150. * M_PI / 180.));
  angles.push_back(static_cast<float>(180. * M_PI / 180.));
  angles.push_back(static_cast<float>(150. * M_PI / 180.));
  angles.push_back(static_cast<float>(120. * M_PI / 180.));
  angles.push_back(static_cast<float>(90. * M_PI / 180.));
  angles.push_back(static_cast<float>(60. * M_PI / 180.));
  angles.push_back(static_cast<float>(30. * M_PI / 180.));

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<spiral>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_THAT(static_cast<float>(acos(next_sh->getLowerPhiEdge() * vector3D(1, 0, 0))),
                testing::FloatNear(angles[i], 0.0001));
    EXPECT_EQ(parameter.getParam<point3D>(0), next_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), next_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), next_sh->getPhiRange());
    EXPECT_EQ(parameter.getParam<float>(3), next_sh->getThickness());
    EXPECT_EQ(parameter.getParam<float>(4), next_sh->getBending());
  }

  {
    auto envelopeShape = factory.getEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("spiral", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<spiral>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2) * 10, envelope_sh->getPhiRange());
    EXPECT_EQ(parameter.getParam<float>(3), envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(SpiralTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(spiral_id);

  EXPECT_EQ("spiral", parameter.getName());
  EXPECT_EQ(spiral_id, parameter.getId());

  ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(2, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  ASSERT_EQ(5, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));
  EXPECT_EQ("lower phi edge", parameter.getParamName<vector3D>(1));
  EXPECT_EQ("inner radius", parameter.getParamName<float>(0));
  EXPECT_EQ("outer radius", parameter.getParamName<float>(1));
  EXPECT_EQ("angle between edges", parameter.getParamName<float>(2));
  EXPECT_EQ("thickness", parameter.getParamName<float>(3));
  EXPECT_EQ("bending", parameter.getParamName<float>(4));

  Shape::removeShapesFromFactory(factory);
}
