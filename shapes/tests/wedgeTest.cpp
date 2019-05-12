/*!
 * \file wedgeTest.cpp
 * Contains tests for shape wedge.
 */

#include "wedge.h"
#include "provideShapes.h"
#include "ring.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid wedge_id1 = {{0x2e, 0x50, 0x1b, 0x2e, 0x11, 0xf5, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid wedge_id2 = {{0x33, 0x2c, 0xf4, 0x8c, 0x11, 0xf5, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(WedgeTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(wedge_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(wedge_id1));
  ASSERT_TRUE(factory.isShapeDefined(wedge_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(wedge_id2));

  // wedge type 2
  /// \todo move to own test
  {
    auto parameter = factory.shapeParameter(wedge_id2);

    parameter.setParam<point3D>(0, {0, 0, 0});
    parameter.setParam<vector3D>(0, {0, 0, 1});
    parameter.setParam<vector3D>(1, {1, 0, 0});
    parameter.setParam<float>(0, 1.f);
    parameter.setParam<float>(1, 10.f);
    parameter.setParam<float>(2, static_cast<float>(30. * M_PI / 180.));
    parameter.setParam<float>(3, 1.f);
    parameter.setParam<float>(4, 5.f);

    auto shape = factory.createVolume(parameter);
    EXPECT_EQ("wedge", shape->getName());
    ASSERT_NE(nullptr, dynamic_pointer_cast<wedge>(shape));
    auto wedge_sh = dynamic_pointer_cast<wedge>(shape);

    EXPECT_EQ(parameter.getParam<point3D>(0), wedge_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), wedge_sh->getNormal());
    EXPECT_EQ(parameter.getParam<vector3D>(1), wedge_sh->getLowerPhiEdge());
    EXPECT_EQ(parameter.getParam<float>(0), wedge_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), wedge_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), wedge_sh->getPhiRange());
    EXPECT_EQ(parameter.getParam<float>(3), wedge_sh->getThickness());
    EXPECT_EQ(parameter.getParam<float>(4), wedge_sh->getDistToBarrelCenter());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(WedgeTest, wedge1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(wedge_id1);

  // let's create a stack of 12 wedges to form a pie.

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<vector3D>(1, {1, 0, 0});
  parameter.setParam<float>(0, 1.f);
  parameter.setParam<float>(1, 10.f);
  parameter.setParam<float>(2, static_cast<float>(30. * M_PI / 180.));
  parameter.setParam<float>(3, 1.f);
  parameter.setParam<float>(4, 1.f);

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

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("wedge", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<wedge>(shape));
  auto wedge_sh = dynamic_pointer_cast<wedge>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), wedge_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), wedge_sh->getNormal());
  EXPECT_EQ(parameter.getParam<vector3D>(1), wedge_sh->getLowerPhiEdge());
  EXPECT_EQ(parameter.getParam<float>(0), wedge_sh->getInnerRadius());
  EXPECT_EQ(parameter.getParam<float>(1), wedge_sh->getOuterRadius());
  EXPECT_EQ(parameter.getParam<float>(2), wedge_sh->getPhiRange());
  EXPECT_EQ(parameter.getParam<float>(3), wedge_sh->getThickness());
  EXPECT_EQ(parameter.getParam<float>(4), wedge_sh->getDistToBarrelCenter());

  for (size_t i = 1; i < 12; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<wedge>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_THAT(static_cast<float>(acos(next_sh->getLowerPhiEdge() * vector3D(1, 0, 0))),
                testing::FloatNear(angles[i], 0.0001));
    EXPECT_EQ(parameter.getParam<point3D>(0), next_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), next_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), next_sh->getPhiRange());
    EXPECT_EQ(parameter.getParam<float>(3), next_sh->getThickness());
    EXPECT_EQ(parameter.getParam<float>(4), next_sh->getDistToBarrelCenter());
  }

  // pie with one piece missing
  {
    auto envelopeShape = factory.createEnvelope(parameter, 11);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("wedge", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<wedge>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2) * 11, envelope_sh->getPhiRange());
    EXPECT_EQ(parameter.getParam<float>(3), envelope_sh->getThickness());
  }

  // un cut pie
  {
    auto envelopeShape = factory.createEnvelope(parameter, 12);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(1), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(3), envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(WedgeTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);
  // wedge type 1
  {
    auto parameter = factory.shapeParameter(wedge_id1);

    EXPECT_EQ("wedge", parameter.getName());
    EXPECT_EQ(wedge_id1, parameter.getId());

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
    // shouldn't be here; is irrelevant for this kind of wedge
    EXPECT_EQ("distance to barrel center", parameter.getParamName<float>(4));
  }
  // wedge type 2
  {
    auto parameter = factory.shapeParameter(wedge_id2);

    EXPECT_EQ("wedge", parameter.getName());
    EXPECT_EQ(wedge_id2, parameter.getId());

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
    EXPECT_EQ("distance to barrel center", parameter.getParamName<float>(4));
  }
  Shape::removeShapesFromFactory(factory);
}
