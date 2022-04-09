/*!
 * \file ringTest.cpp
 * Contains tests for shape ring.
 */

#include "ring.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid ring_id1 = {{0x9b, 0xe4, 0xd8, 0x92, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid ring_id2 = {{0xa0, 0xa9, 0x51, 0xf0, 0x11, 0xef, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(RingTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(ring_id1));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(ring_id1));
  ASSERT_TRUE(factory.isShapeDefined(ring_id2));
  EXPECT_EQ(ShapeType::VolumeShape, factory.shapeType(ring_id2));

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, ring1)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(ring_id1);

  // let's create a stack of 10 rings totalling to a ring with outer radius 11.

  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("normal", parameter.valueName(1));
  EXPECT_EQ("inner radius", parameter.valueName(2));
  EXPECT_EQ("delta radius", parameter.valueName(3));
  EXPECT_EQ("thickness", parameter.valueName(4));

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = static_cast<float>(1.f);
  parameter.value(3) = static_cast<float>(1.f);
  parameter.value(4) = static_cast<float>(1.f);

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("ring", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<ring>(shape));
  auto ring_sh = dynamic_pointer_cast<ring>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), ring_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), ring_sh->getNormal());
  EXPECT_EQ(parameter.value(2).value<float>(), ring_sh->getInnerRadius());
  EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>(),
            ring_sh->getOuterRadius());
  EXPECT_EQ(parameter.value(4).value<float>(), ring_sh->getThickness());

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<ring>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.value(0).value<point3D>(), next_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getNormal());
    EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>() * i,
              next_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>() * (i + 1),
              next_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), next_sh->getThickness());
  }

  {
    auto envelopeShape = factory.createEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getNormal());
    EXPECT_EQ(parameter.value(2).value<float>(), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>() * 10,
              envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, ring2)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(ring_id2);

  // let's create a stack of 10 rings totalling to a ring with a thickness of 10.

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = static_cast<float>(1.f);
  parameter.value(3) = static_cast<float>(1.f);
  parameter.value(4) = static_cast<float>(1.f);

  auto shape = factory.createVolume(parameter);
  EXPECT_EQ("ring", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<ring>(shape));
  auto ring_sh = dynamic_pointer_cast<ring>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), ring_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), ring_sh->getNormal());
  EXPECT_EQ(parameter.value(2).value<float>(), ring_sh->getInnerRadius());
  EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>(),
            ring_sh->getOuterRadius());
  EXPECT_EQ(parameter.value(4).value<float>(), ring_sh->getThickness());

  for (size_t i = 1; i < 10; i++) {
    auto nextShape = factory.createNext(parameter, i);
    ASSERT_NE(nullptr, nextShape);
    auto next_sh = dynamic_pointer_cast<ring>(nextShape);
    ASSERT_NE(nullptr, next_sh);

    EXPECT_EQ(parameter.value(0).value<point3D>() -
                  parameter.value(1).value<vector3D>() * parameter.value(4).value<float>() * i,
              next_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), next_sh->getNormal());
    EXPECT_EQ(parameter.value(2).value<float>(), next_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>(),
              next_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(4).value<float>(), next_sh->getThickness());
  }

  {
    auto envelopeShape = factory.createEnvelope(parameter, 10);
    EXPECT_NE(nullptr, envelopeShape);
    EXPECT_EQ("ring", envelopeShape->getName());

    auto envelope_sh = dynamic_pointer_cast<ring>(envelopeShape);
    ASSERT_NE(nullptr, envelope_sh);
    EXPECT_EQ(point3D(0, 0, 0), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(0).value<point3D>(), envelope_sh->getCenter());
    EXPECT_EQ(parameter.value(1).value<vector3D>(), envelope_sh->getNormal());
    EXPECT_EQ(parameter.value(2).value<float>(), envelope_sh->getInnerRadius());
    EXPECT_EQ(parameter.value(2).value<float>() + parameter.value(3).value<float>(),
              envelope_sh->getOuterRadius());
    EXPECT_EQ(parameter.value(4).value<float>() * 10, envelope_sh->getThickness());
  }

  Shape::removeShapesFromFactory(factory);
}

TEST(RingTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);
  // ring type 1
  {
    auto parameter = factory.shapeParameter(ring_id1);

    EXPECT_EQ("ring", parameter.getName());
    EXPECT_EQ(ring_id1, parameter.id());

    ASSERT_EQ(5, parameter.numberOfValues());
    EXPECT_EQ("center", parameter.valueName(0));
    EXPECT_EQ("normal", parameter.valueName(1));
    EXPECT_EQ("inner radius", parameter.valueName(2));
    EXPECT_EQ("delta radius", parameter.valueName(3));
    EXPECT_EQ("thickness", parameter.valueName(4));
  }
  // ring type 2
  {
    auto parameter = factory.shapeParameter(ring_id2);

    EXPECT_EQ("ring", parameter.getName());
    EXPECT_EQ(ring_id2, parameter.id());

    ASSERT_EQ(5, parameter.numberOfValues());
    EXPECT_EQ("center", parameter.valueName(0));
    EXPECT_EQ("normal", parameter.valueName(1));
    EXPECT_EQ("inner radius", parameter.valueName(2));
    EXPECT_EQ("delta radius", parameter.valueName(3));
    EXPECT_EQ("thickness", parameter.valueName(4));
  }
  Shape::removeShapesFromFactory(factory);
}
