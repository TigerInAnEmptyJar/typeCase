/*!
 * \file sphericTriangleTest.cpp
 * Contains tests for shape sphericTriangle.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid sphericTriangle_id = {{0x17, 0x1f, 0x0f, 0x58, 0x11, 0xed, 0x11, 0xe9,
                                                0xab, 0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(SphericTriangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(sphericTriangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(sphericTriangle_id));

  auto parameter = factory.shapeParameter(sphericTriangle_id);

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = point3D{1, 0, 0};
  parameter.value(2) = point3D{2, 0, 0};
  parameter.value(3) = point3D{1.5, 0.5, 0};
  parameter.value(7) = static_cast<int>(3);
  auto normal = vector3D{0, 0, 1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("spheric triangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<sphericTriangle>(shape));
  auto sphericTriangle_sh = dynamic_pointer_cast<sphericTriangle>(shape);
  EXPECT_EQ(parameter.value(0).value<point3D>(), sphericTriangle_sh->getCenter());
  EXPECT_EQ(normal, sphericTriangle_sh->getNormal());
  EXPECT_EQ(parameter.value(1).value<point3D>(), sphericTriangle_sh->A());
  EXPECT_EQ(parameter.value(2).value<point3D>(), sphericTriangle_sh->B());
  EXPECT_EQ(parameter.value(3).value<point3D>(), sphericTriangle_sh->C());
  EXPECT_TRUE(sphericTriangle_sh->getNumberOfPoints());
  EXPECT_EQ(geomState::_undefined_, sphericTriangle_sh->D().getState());
  EXPECT_EQ(geomState::_undefined_, sphericTriangle_sh->E().getState());
  EXPECT_EQ(geomState::_undefined_, sphericTriangle_sh->F().getState());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(SphericTriangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(sphericTriangle_id);

  EXPECT_EQ("sphericTriangle", parameter.getName());
  EXPECT_EQ(sphericTriangle_id, parameter.id());

  ASSERT_EQ(8, parameter.numberOfValues());

  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("A", parameter.valueName(1));
  EXPECT_EQ("B", parameter.valueName(2));
  EXPECT_EQ("C", parameter.valueName(3));
  EXPECT_EQ("D", parameter.valueName(4));
  EXPECT_EQ("E", parameter.valueName(5));
  EXPECT_EQ("F", parameter.valueName(6));
  EXPECT_EQ("number of points", parameter.valueName(7));

  Shape::removeShapesFromFactory(factory);
}
