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
  EXPECT_EQ(ShapeType::PlanarShape, factory.getShapeType(sphericRectangle_id));

  auto parameter = factory.getShapeParameter(sphericRectangle_id);
  parameter.setParam<point3D>(0, {0, 0, 0});
  parameter.setParam<point3D>(1, {2, 0, 0});
  parameter.setParam<point3D>(2, {1, 0, 0});
  parameter.setParam<point3D>(3, {0, 1, 0});
  parameter.setParam<point3D>(4, {0, 2, 0});
  auto normal = vector3D{0, 0, 1};

  auto shape = factory.getPlane(parameter);
  EXPECT_EQ("sphericRectangle", shape->getName());
  ASSERT_NE(nullptr, dynamic_pointer_cast<sphericRectangle>(shape));

  auto sphericRectangle_sh = dynamic_pointer_cast<sphericRectangle>(shape);
  //    EXPECT_EQ(parameter.getParam<point3D>(0), sphericRectangle_sh->getCenter());
  EXPECT_EQ(normal, sphericRectangle_sh->getNormal());
  EXPECT_EQ(parameter.getParam<point3D>(1), sphericRectangle_sh->A());
  EXPECT_EQ(parameter.getParam<point3D>(2), sphericRectangle_sh->B());
  EXPECT_EQ(parameter.getParam<point3D>(3), sphericRectangle_sh->C());
  EXPECT_EQ(parameter.getParam<point3D>(4), sphericRectangle_sh->D());

  auto nextShape = factory.getNext(parameter, 2);
  EXPECT_EQ(nullptr, nextShape);

  auto envelopeShape = factory.getEnvelope(parameter, 2);
  EXPECT_EQ(nullptr, envelopeShape);

  Shape::removeShapesFromFactory(factory);
}

TEST(SphericRectangleTest, parameter)
{
  ShapeFactory factory;
  Shape::addShapesToFactory(factory);

  auto parameter = factory.getShapeParameter(sphericRectangle_id);

  EXPECT_EQ("sphericRectangle", parameter.getName());
  EXPECT_EQ(sphericRectangle_id, parameter.getId());

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
