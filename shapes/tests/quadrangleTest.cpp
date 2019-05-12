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
  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<point3D>(1, {1, 1, 0});
  parameter.setParam<point3D>(2, {1, -1, 0});
  parameter.setParam<point3D>(3, {-1, -1, 0});
  parameter.setParam<point3D>(4, {-1, 1, 0});
  auto normal = vector3D{0, 0, -1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("quadrangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<quadrangle>(shape));

  auto quadrangle_sh = dynamic_pointer_cast<quadrangle>(shape);
  EXPECT_EQ(parameter.getParam<point3D>(0), quadrangle_sh->getCenter());
  EXPECT_EQ(normal, quadrangle_sh->getNormal());
  EXPECT_EQ(parameter.getParam<point3D>(1), quadrangle_sh->A());
  EXPECT_EQ(parameter.getParam<point3D>(2), quadrangle_sh->B());
  EXPECT_EQ(parameter.getParam<point3D>(3), quadrangle_sh->C());
  EXPECT_EQ(parameter.getParam<point3D>(4), quadrangle_sh->D());

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
  EXPECT_EQ(quadrangle_id, parameter.getId());

  ASSERT_EQ(5, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(0, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(0, parameter.NumberOfParams<int>());
  EXPECT_EQ(0, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());

  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("A", parameter.getParamName<point3D>(1));
  EXPECT_EQ("B", parameter.getParamName<point3D>(2));
  EXPECT_EQ("C", parameter.getParamName<point3D>(3));
  EXPECT_EQ("D", parameter.getParamName<point3D>(4));

  Shape::removeShapesFromFactory(factory);
}
