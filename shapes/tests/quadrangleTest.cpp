/*!
 * \file quadrangleTest.cpp
 * Contains tests for shape quadrangle.
 */

#include "quadrangle.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid quadrangle_id = {{0xeb, 0xc6, 0x86, 0x98, 0x11, 0xeb, 0x11, 0xe9, 0xab,
                                           0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(QuadrangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(quadrangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(quadrangle_id));

  auto parameter = factory.shapeParameter(quadrangle_id);
  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = point3D{1, 1, 0};
  parameter.value(2) = point3D{1, -1, 0};
  parameter.value(3) = point3D{-1, -1, 0};
  parameter.value(4) = point3D{-1, 1, 0};
  auto normal = vector3D{0, 0, -1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("quadrangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<quadrangle>(shape));

  auto quadrangle_sh = dynamic_pointer_cast<quadrangle>(shape);
  EXPECT_EQ(parameter.value(0).value<point3D>(), quadrangle_sh->getCenter());
  EXPECT_EQ(normal, quadrangle_sh->getNormal());
  EXPECT_EQ(parameter.value(1).value<point3D>(), quadrangle_sh->A());
  EXPECT_EQ(parameter.value(2).value<point3D>(), quadrangle_sh->B());
  EXPECT_EQ(parameter.value(3).value<point3D>(), quadrangle_sh->C());
  EXPECT_EQ(parameter.value(4).value<point3D>(), quadrangle_sh->D());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(QuadrangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(quadrangle_id);

  EXPECT_EQ("quadrangle", parameter.getName());
  EXPECT_EQ(quadrangle_id, parameter.id());

  ASSERT_EQ(5, parameter.numberOfValues());

  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("A", parameter.valueName(1));
  EXPECT_EQ("B", parameter.valueName(2));
  EXPECT_EQ("C", parameter.valueName(3));
  EXPECT_EQ("D", parameter.valueName(4));

  Shape::removeShapesFromFactory(factory);
}
