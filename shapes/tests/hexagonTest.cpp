/*!
 * \file hexagonTest.cpp
 * Contains tests for shape hexagon.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid hexagon_id = {{0x49, 0xde, 0xdb, 0xfa, 0x11, 0xeb, 0x11, 0xe9, 0xab, 0x14,
                                        0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(HexagonTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(hexagon_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(hexagon_id));

  auto parameter = factory.shapeParameter(hexagon_id);

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<point3D>(1, {1, 0, 0});
  parameter.setParam<vector3D>(0, {0, 0, 1});

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("hexagon", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<hexagon>(shape));

  auto hexagon_sh = dynamic_pointer_cast<hexagon>(shape);
  EXPECT_EQ(parameter.getParam<point3D>(0), hexagon_sh->getCenter());
  EXPECT_EQ(parameter.getParam<vector3D>(0), hexagon_sh->getNormal());
  EXPECT_EQ(parameter.getParam<point3D>(1), hexagon_sh->A());

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
  EXPECT_EQ(hexagon_id, parameter.getId());

  ASSERT_EQ(2, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(1, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  EXPECT_EQ(0, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());

  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("A", parameter.getParamName<point3D>(1));
  EXPECT_EQ("normal", parameter.getParamName<vector3D>(0));

  Shape::removeShapesFromFactory(factory);
}
