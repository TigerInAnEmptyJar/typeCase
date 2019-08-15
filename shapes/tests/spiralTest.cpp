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
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(spiral_id));

  Shape::removeShapesFromFactory(factory);
}

TEST(SpiralTest, spiral)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(spiral_id);

  // let's create a stack of 10 spirals .

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = vector3D{1, 0, 0};
  parameter.value(3) = static_cast<float>(1.f);
  parameter.value(4) = static_cast<float>(10.f);
  parameter.value(5) = static_cast<float>(30. * M_PI / 180.);
  parameter.value(6) = static_cast<float>(1.f);
  parameter.value(7) = static_cast<float>(1.f);

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("spiral", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<spiral>(shape));
  auto spiral_sh = dynamic_pointer_cast<spiral>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), spiral_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), spiral_sh->getNormal());
  EXPECT_EQ(parameter.value(2).value<vector3D>(), spiral_sh->getLowerPhiEdge());
  EXPECT_EQ(parameter.value(3).value<float>(), spiral_sh->getInnerRadius());
  EXPECT_EQ(parameter.value(4).value<float>(), spiral_sh->getOuterRadius());
  EXPECT_EQ(parameter.value(5).value<float>(), spiral_sh->getPhiRange());
  EXPECT_EQ(parameter.value(6).value<float>(), spiral_sh->getThickness());
  EXPECT_EQ(parameter.value(7).value<float>(), spiral_sh->getBending());

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
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<spiral>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_THAT(static_cast<float>(acos(next_sh->getLowerPhiEdge() * vector3D(1, 0, 0))),
                testing::FloatNear(angles[i], 0.0001));
    EXPECT_EQ(parameter.value(0).value<point3D>(), next_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getNormal());
    EXPECT_EQ(parameter.value(3).value<float>(), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), next_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(5).value<float>(), next_sh->getPhiRange());
    EXPECT_EQ(parameter.value(6).value<float>(), next_sh->getThickness());
    EXPECT_EQ(parameter.value(7).value<float>(), next_sh->getBending());
  }

  {
    auto envelopeShape = factory.createEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("spiral", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<spiral>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getNormal());
    EXPECT_EQ(parameter.value(3).value<float>(), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(5).value<float>() * 10, envelope_sh->getPhiRange());
    EXPECT_EQ(parameter.value(6).value<float>(), envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(SpiralTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(spiral_id);

  EXPECT_EQ("spiral", parameter.getName());
  EXPECT_EQ(spiral_id, parameter.id());

  ASSERT_EQ(8, parameter.numberOfValues());
  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("normal", parameter.valueName(1));
  EXPECT_EQ("lower phi edge", parameter.valueName(2));
  EXPECT_EQ("inner radius", parameter.valueName(3));
  EXPECT_EQ("outer radius", parameter.valueName(4));
  EXPECT_EQ("angle between edges", parameter.valueName(5));
  EXPECT_EQ("thickness", parameter.valueName(6));
  EXPECT_EQ("bending", parameter.valueName(7));

  Shape::removeShapesFromFactory(factory);
}
