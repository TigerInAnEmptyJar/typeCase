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

    parameter.value(0) = point3D{0, 0, 0};
    parameter.value(1) = vector3D{0, 0, 1};
    parameter.value(2) = vector3D{1, 0, 0};
    parameter.value(3) = static_cast<float>(1.f);
    parameter.value(4) = static_cast<float>(10.f);
    parameter.value(5) = static_cast<float>(30. * M_PI / 180.);
    parameter.value(6) = static_cast<float>(1.f);
    parameter.value(7) = static_cast<float>(5.f);

    auto shape = factory.createVolume(parameter);
    EXPECT_EQ("wedge", shape->getName());
    ASSERT_NE(nullptr, dynamic_pointer_cast<wedge>(shape));
    auto wedge_sh = dynamic_pointer_cast<wedge>(shape);

    EXPECT_EQ(parameter.value(0).value<point3D>(), wedge_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), wedge_sh->getNormal());
    EXPECT_EQ(parameter.value(2).value<vector3D>(), wedge_sh->getLowerPhiEdge());
    EXPECT_EQ(parameter.value(3).value<float>(), wedge_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), wedge_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(5).value<float>(), wedge_sh->getPhiRange());
    EXPECT_EQ(parameter.value(6).value<float>(), wedge_sh->getThickness());
    EXPECT_EQ(parameter.value(7).value<float>(), wedge_sh->getDistToBarrelCenter());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(WedgeTest, wedge1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(wedge_id1);

  // let's create a stack of 12 wedges to form a pie.

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = vector3D{1, 0, 0};
  parameter.value(3) = static_cast<float>(1.f);
  parameter.value(4) = static_cast<float>(10.f);
  parameter.value(5) = static_cast<float>(30. * M_PI / 180.);
  parameter.value(6) = static_cast<float>(1.f);
  parameter.value(7) = static_cast<float>(1.f);

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

  EXPECT_EQ(parameter.value(0).value<point3D>(), wedge_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), wedge_sh->getNormal());
  EXPECT_EQ(parameter.value(2).value<vector3D>(), wedge_sh->getLowerPhiEdge());
  EXPECT_EQ(parameter.value(3).value<float>(), wedge_sh->getInnerRadius());
  EXPECT_EQ(parameter.value(4).value<float>(), wedge_sh->getOuterRadius());
  EXPECT_EQ(parameter.value(5).value<float>(), wedge_sh->getPhiRange());
  EXPECT_EQ(parameter.value(6).value<float>(), wedge_sh->getThickness());
  EXPECT_EQ(parameter.value(7).value<float>(), wedge_sh->getDistToBarrelCenter());

  for (size_t i = 1; i < 12; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<wedge>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_THAT(static_cast<float>(acos(next_sh->getLowerPhiEdge() * vector3D(1, 0, 0))),
                testing::FloatNear(angles[i], 0.0001f));
    EXPECT_EQ(parameter.value(0).value<point3D>(), next_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getNormal());
    EXPECT_EQ(parameter.value(3).value<float>(), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), next_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(5).value<float>(), next_sh->getPhiRange());
    EXPECT_EQ(parameter.value(6).value<float>(), next_sh->getThickness());
    EXPECT_EQ(parameter.value(7).value<float>(), next_sh->getDistToBarrelCenter());
  }

  // pie with one piece missing
  {
    auto envelopeShape = factory.createEnvelope(parameter, 11);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("wedge", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<wedge>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getNormal());
    EXPECT_EQ(parameter.value(3).value<float>(), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(5).value<float>() * 11, envelope_sh->getPhiRange());
    EXPECT_EQ(parameter.value(6).value<float>(), envelope_sh->getThickness());
  }

  // un cut pie
  {
    auto envelopeShape = factory.createEnvelope(parameter, 12);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getNormal());
    EXPECT_EQ(parameter.value(3).value<float>(), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(6).value<float>(), envelope_sh->getThickness());
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
    EXPECT_EQ(wedge_id1, parameter.id());

    ASSERT_EQ(8, parameter.numberOfValues());
    EXPECT_EQ("center", parameter.valueName(0));
    EXPECT_EQ("normal", parameter.valueName(1));
    EXPECT_EQ("lower phi edge", parameter.valueName(2));
    EXPECT_EQ("inner radius", parameter.valueName(3));
    EXPECT_EQ("outer radius", parameter.valueName(4));
    EXPECT_EQ("angle between edges", parameter.valueName(5));
    EXPECT_EQ("thickness", parameter.valueName(6));
    // shouldn't be here; is irrelevant for this kind of wedge
    EXPECT_EQ("distance to barrel center", parameter.valueName(7));
  }
  // wedge type 2
  {
    auto parameter = factory.shapeParameter(wedge_id2);

    EXPECT_EQ("wedge", parameter.getName());
    EXPECT_EQ(wedge_id2, parameter.id());

    ASSERT_EQ(8, parameter.numberOfValues());
    EXPECT_EQ("center", parameter.valueName(0));
    EXPECT_EQ("normal", parameter.valueName(1));
    EXPECT_EQ("lower phi edge", parameter.valueName(2));
    EXPECT_EQ("inner radius", parameter.valueName(3));
    EXPECT_EQ("outer radius", parameter.valueName(4));
    EXPECT_EQ("angle between edges", parameter.valueName(5));
    EXPECT_EQ("thickness", parameter.valueName(6));
    EXPECT_EQ("distance to barrel center", parameter.valueName(7));
  }
  Shape::removeShapesFromFactory(factory);
}
