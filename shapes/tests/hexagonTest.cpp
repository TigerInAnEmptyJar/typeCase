/*!
 * \file hexagonTest.cpp
 * Contains tests for shape hexagon.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testingUtilities.h"

const boost::uuids::uuid hexagon_id = {{0x49, 0xde, 0xdb, 0xfa, 0x11, 0xeb, 0x11, 0xe9, 0xab, 0x14,
                                        0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(HexagonTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(hexagon_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(hexagon_id));

  auto parameter = factory.shapeParameter(hexagon_id);

  parameter.value(0) = point3D{0, 0, 0};
  parameter.value(1) = point3D{1, 0, 0};
  parameter.value(2) = vector3D{0, 0, 1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("hexagon", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<hexagon>(shape));

  auto hexagon_sh = dynamic_pointer_cast<hexagon>(shape);
  EXPECT_EQ(parameter.value(0).value<point3D>(), hexagon_sh->getCenter());
  EXPECT_EQ(parameter.value(2).value<vector3D>(), hexagon_sh->getNormal());
  EXPECT_EQ(parameter.value(1).value<point3D>(), hexagon_sh->A());

  auto nextShape = factory.createNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.createEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(HexagonTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.shapeParameter(hexagon_id);

  EXPECT_EQ("hexagon", parameter.getName());
  EXPECT_EQ(hexagon_id, parameter.id());

  ASSERT_EQ(3, parameter.numberOfValues());

  EXPECT_EQ("center", parameter.valueName(0));
  EXPECT_EQ("A", parameter.valueName(1));
  EXPECT_EQ("normal", parameter.valueName(2));

  Shape::removeShapesFromFactory(factory);
}
