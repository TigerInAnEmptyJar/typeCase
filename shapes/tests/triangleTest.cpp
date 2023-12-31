/*!
 * \file triangleTest.cpp
 * Contains tests for shape triangle.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid triangle_id{{0xf2, 0x04, 0x0a, 0xb6, 0x14, 0x3f, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(TriangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(triangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(triangle_id));

  auto parameter = factory.shapeParameter(triangle_id);
  parameter.value(0) = point3D{1, 1, 0};
  parameter.value(1) = point3D{1, -1, 0};
  parameter.value(2) = point3D{-1, -1, 0};
  auto normal = vector3D{0, 0, -1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("triangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<triangle>(shape));

  auto triangle_sh = dynamic_pointer_cast<triangle>(shape);
  EXPECT_EQ(normal, triangle_sh->getNormal());
  EXPECT_EQ(parameter.value(0).value<point3D>(), triangle_sh->A());
  EXPECT_EQ(parameter.value(1).value<point3D>(), triangle_sh->B());
  EXPECT_EQ(parameter.value(2).value<point3D>(), triangle_sh->C());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(TriangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(triangle_id);

  EXPECT_EQ("triangle", parameter.getName());
  EXPECT_EQ(triangle_id, parameter.id());

  ASSERT_EQ(3, parameter.numberOfValues());

  EXPECT_EQ("A", parameter.valueName(0));
  EXPECT_EQ("B", parameter.valueName(1));
  EXPECT_EQ("C", parameter.valueName(2));

  Shape::removeShapesFromFactory(factory);
}
