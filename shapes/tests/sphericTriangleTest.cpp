/*!
 * \file sphericTriangleTest.cpp
 * Contains tests for shape sphericTriangle.
 */

#include "planarShapes.h"
#include "provideShapes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

const boost::uuids::uuid sphericTriangle_id = {{0x17, 0x1f, 0x0f, 0x58, 0x11, 0xed, 0x11, 0xe9,
                                                0xab, 0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

TEST(SphericTriangleTest, provider)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  ASSERT_TRUE(factory.isShapeDefined(sphericTriangle_id));
  EXPECT_EQ(ShapeType::PlanarShape, factory.shapeType(sphericTriangle_id));

  auto parameter = factory.shapeParameter(sphericTriangle_id);

  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<point3D>(1, {1, 0, 0});
  parameter.setParam<point3D>(2, {2, 0, 0});
  parameter.setParam<point3D>(3, {1.5, 0.5, 0});
  parameter.setParam<int>(0, 3);
  auto normal = vector3D{0, 0, 1};

  auto shape = factory.createPlane(parameter);
  EXPECT_EQ("spheric triangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<sphericTriangle>(shape));
  auto sphericTriangle_sh = dynamic_pointer_cast<sphericTriangle>(shape);
  EXPECT_EQ(parameter.getParam<point3D>(0), sphericTriangle_sh->getCenter());
  EXPECT_EQ(normal, sphericTriangle_sh->getNormal());
  EXPECT_EQ(parameter.getParam<point3D>(1), sphericTriangle_sh->A());
  EXPECT_EQ(parameter.getParam<point3D>(2), sphericTriangle_sh->B());
  EXPECT_EQ(parameter.getParam<point3D>(3), sphericTriangle_sh->C());
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
  EXPECT_EQ(sphericTriangle_id, parameter.getId());

  ASSERT_EQ(7, parameter.NumberOfParams<point3D>());
  ASSERT_EQ(0, parameter.NumberOfParams<vector3D>());
  EXPECT_EQ(1, parameter.NumberOfParams<int>());
  EXPECT_EQ(0, parameter.NumberOfParams<float>());
  EXPECT_EQ(0, parameter.NumberOfParams<std::string>());

  EXPECT_EQ("center", parameter.getParamName<point3D>(0));
  EXPECT_EQ("A", parameter.getParamName<point3D>(1));
  EXPECT_EQ("B", parameter.getParamName<point3D>(2));
  EXPECT_EQ("C", parameter.getParamName<point3D>(3));
  EXPECT_EQ("D", parameter.getParamName<point3D>(4));
  EXPECT_EQ("E", parameter.getParamName<point3D>(5));
  EXPECT_EQ("F", parameter.getParamName<point3D>(6));
  EXPECT_EQ("number of points", parameter.getParamName<int>(0));

  Shape::removeShapesFromFactory(factory);
}
