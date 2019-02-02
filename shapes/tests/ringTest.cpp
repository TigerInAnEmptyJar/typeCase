/*!
 * \file ringTest.cpp
 * Contains tests for shape ring.
 */

#include "ring.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid ring_id1 = {{0x9b, 0xe4, 0xd8, 0x92, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid ring_id2 = {{0xa0, 0xa9, 0x51, 0xf0, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(RingTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(ring_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(ring_id1));
  ASSERT_TRUE(factory.isShapeDefined(ring_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(ring_id2));

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, ring1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(ring_id1);

  // let's create a stack of 10 rings totalling to a ring with outer radius 11.

  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));
  EXPECT_EQ("inner radius", parameter.getParamName<float>(0));
  EXPECT_EQ("delta radius", parameter.getParamName<float>(1));
  EXPECT_EQ("thickness", parameter.getParamName<float>(2));

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<float>(0, 1.f);
  parameter.setParam<float>(1, 1.f);
  parameter.setParam<float>(2, 1.f);

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("ring", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<ring>(shape));
  auto ring_sh = dynamic_pointer_cast<ring>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), ring_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), ring_sh->getNormal());
  EXPECT_EQ(parameter.getParam<float>(0), ring_sh->getInnerRadius());
  EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1), ring_sh->getOuterRadius());
  EXPECT_EQ(parameter.getParam<float>(2), ring_sh->getThickness());

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<ring>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.getParam<point3D>(0), next_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1) * i,
              next_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1) * (i + 1),
              next_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), next_sh->getThickness());
  }

  {
    auto envelopeShape = factory.getEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1) * 10,
              envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, ring2)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(ring_id2);

  // let's create a stack of 10 rings totalling to a ring with a thickness of 10.

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});
  parameter.setParam<float>(0, 1.f);
  parameter.setParam<float>(1, 1.f);
  parameter.setParam<float>(2, 1.f);

  auto shape = factory.getVolume(parameter);
  EXPECT_EQ("ring", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<ring>(shape));
  auto ring_sh = dynamic_pointer_cast<ring>(shape);

  EXPECT_EQ(parameter.getParam<point3D>(0), ring_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), ring_sh->getNormal());
  EXPECT_EQ(parameter.getParam<float>(0), ring_sh->getInnerRadius());
  EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1), ring_sh->getOuterRadius());
  EXPECT_EQ(parameter.getParam<float>(2), ring_sh->getThickness());

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.getNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<ring>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.getParam<point3D>(0) -
                  parameter.getParam<vector3D>(0) * parameter.getParam<float>(2) * i,
              next_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), next_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1),
              next_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2), next_sh->getThickness());
  }

  {
    auto envelopeShape = factory.getEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<point3D>(0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.getParam<vector3D>(0), envelope_sh->getNormal());
    EXPECT_EQ(parameter.getParam<float>(0), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.getParam<float>(0) + parameter.getParam<float>(1),
              envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.getParam<float>(2) * 10, envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);
  // ring type 1
  {
    auto parameter = factory.getShapeParameter(ring_id1);

    EXPECT_EQ("ring", parameter.getName());
    EXPECT_EQ(ring_id1, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(1, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(0, parameter.NumberOfParams<int>());
    ASSERT_EQ(3, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    EXPECT_EQ("center", parameter.getParamName<point3D>(0));
    EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("inner radius", parameter.getParamName<float>(0));
    EXPECT_EQ("delta radius", parameter.getParamName<float>(1));
    EXPECT_EQ("thickness", parameter.getParamName<float>(2));
  }
  // ring type 2
  {
    auto parameter = factory.getShapeParameter(ring_id2);

    EXPECT_EQ("ring", parameter.getName());
    EXPECT_EQ(ring_id2, parameter.getId());

    ASSERT_EQ(1, parameter.NumberOfParams<point3D>());
    ASSERT_EQ(1, parameter.NumberOfParams<vector3D>());
    EXPECT_EQ(0, parameter.NumberOfParams<int>());
    ASSERT_EQ(3, parameter.NumberOfParams<float>());
    EXPECT_EQ(0, parameter.NumberOfParams<std::string>());
    EXPECT_EQ("center", parameter.getParamName<point3D>(0));
    EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));
    EXPECT_EQ("inner radius", parameter.getParamName<float>(0));
    EXPECT_EQ("delta radius", parameter.getParamName<float>(1));
    EXPECT_EQ("thickness", parameter.getParamName<float>(2));
  }
  Shape::removeShapesFromFactory(factory);
}
