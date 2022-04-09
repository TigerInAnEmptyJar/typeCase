/*!
 * \file circleTest.cpp
 * Contains tests for shape circle.
 */

#include "circle.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid circle_id = {{0x14, 0x8b, 0xec, 0xc6, 0x11, 0xbf, 0x11, 0xe9, 0xab, 0x14,
                                       0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(CircleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(circle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(circle_id));

  auto parameter = factory.shapeParameter(circle_id);

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = vector3D{0, 0, 1};
  parameter.value(2) = static_cast<float>(101.1f);

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("circle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<circle>(shape));
  auto circle_sh = dynamic_pointer_cast<circle>(shape);

  EXPECT_EQ(parameter.value(0).value<point3D>(), circle_sh->getCenter());
  EXPECT_EQ(parameter.value(1).value<vector3D>(), circle_sh->getNormal());
  EXPECT_EQ(parameter.value(2).value<float>(), circle_sh->radius());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(CircleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(circle_id);

  EXPECT_EQ("circle", parameter.getName());
  EXPECT_EQ(circle_id, parameter.id());

  ASSERT_EQ(3, parameter.numberOfValues());
  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("normal", parameter.valueName(1));
  EXPECT_EQ("radius", parameter.valueName(2));

  Shape::removeShapesFromFactory(factory);
}
