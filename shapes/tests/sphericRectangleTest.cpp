/*!
 * \file sphericRectangleTest.cpp
 * Contains tests for shape sphericRectangle.
 */

#include "sphericRectangle.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid sphericRectangle_id = {{0xb6, 0x58, 0x46, 0x6c, 0x11, 0xec, 0x11, 0xe9,
                                                 0xab, 0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(SphericRectangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(sphericRectangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(sphericRectangle_id));

  auto parameter = factory.shapeParameter(sphericRectangle_id);
  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = point3D{2, 0, 0};
  parameter.value(2) = point3D{1, 0, 0};
  parameter.value(3) = point3D{0, 1, 0};
  parameter.value(4) = point3D{0, 2, 0};
  auto normal = vector3D{0, 0, 1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("sphericRectangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<sphericRectangle>(shape));

  auto sphericRectangle_sh = dynamic_pointer_cast<sphericRectangle>(shape);
  //    EXPECT_EQ(parameter.getParam<point3D>(0), sphericRectangle_sh->getCenter());
  EXPECT_EQ(normal, sphericRectangle_sh->getNormal());
  EXPECT_EQ(parameter.value(1).value<point3D>(), sphericRectangle_sh->A());
  EXPECT_EQ(parameter.value(2).value<point3D>(), sphericRectangle_sh->B());
  EXPECT_EQ(parameter.value(3).value<point3D>(), sphericRectangle_sh->C());
  EXPECT_EQ(parameter.value(4).value<point3D>(), sphericRectangle_sh->D());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(SphericRectangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(sphericRectangle_id);

  EXPECT_EQ("sphericRectangle", parameter.getName());
  EXPECT_EQ(sphericRectangle_id, parameter.id());

  ASSERT_EQ(5, parameter.numberOfValues());

  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("A", parameter.valueName(1));
  EXPECT_EQ("B", parameter.valueName(2));
  EXPECT_EQ("C", parameter.valueName(3));
  EXPECT_EQ("D", parameter.valueName(4));

  Shape::removeShapesFromFactory(factory);
}
