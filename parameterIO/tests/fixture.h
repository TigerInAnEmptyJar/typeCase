#pragma once

#include "parameterio.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace parameter::IO;

std::ostream& operator<<(std::ostream& o, algorithm_parameter::Category c);
std::ostream& operator<<(std::ostream& o, run_parameter::RunType const& s);
std::ostream& operator<<(std::ostream& o, ParameterValue::ValueType const& s);
std::ostream& operator<<(std::ostream& o, base_parameter const& s);
std::ostream& operator<<(std::ostream& o, std::shared_ptr<base_parameter> const& s);
std::ostream& operator<<(std::ostream& o, algorithm_parameter const& s);
std::ostream& operator<<(std::ostream& o, std::shared_ptr<algorithm_parameter> const& s);

class ParameterIoTest : public testing::Test
{
public:
  void SetUp() override { io.setup(); }

  void appendSomeAlgorithms(std::vector<std::shared_ptr<base_parameter>>& items);
  void appendSomeShapes(std::vector<std::shared_ptr<base_parameter>>& items);
  void appendSomeMaterials(std::vector<std::shared_ptr<base_parameter>>& items);
  void appendSomeDetectors(std::vector<std::shared_ptr<base_parameter>>& items);
  void appendSomeDatabase(std::vector<std::shared_ptr<base_parameter>>& items);

  void installRecovery();

  ParameterIO io;
  boost::uuids::uuid _shape1_id;
  boost::uuids::uuid _shape2_id;
  boost::uuids::uuid _algorithm1_id;
  boost::uuids::uuid _algorithm2_id;
};

MATCHER_P(IsValueEqual, value1, "")
{
  auto& v1 = static_cast<ParameterValue const&>(value1);
  auto& v2 = static_cast<ParameterValue const&>(arg);
  EXPECT_EQ(v1.valueType(), v2.valueType());
  if (v1.valueType() == v2.valueType()) {
    switch (v1.valueType()) {
    case ParameterValue::ValueType::BOOLEAN:
    case ParameterValue::ValueType::INT:
    case ParameterValue::ValueType::FLOAT:
    case ParameterValue::ValueType::DOUBLE:
    case ParameterValue::ValueType::POINT3D:
    case ParameterValue::ValueType::STRING:
    case ParameterValue::ValueType::VECTOR_INT:
    case ParameterValue::ValueType::VECTOR_FLOAT:
    case ParameterValue::ValueType::VECTOR_DOUBLE:
    case ParameterValue::ValueType::VECTOR_STRING:
      EXPECT_EQ(v1, v2);
      break;
    case ParameterValue::ValueType::VECTOR3D:
      EXPECT_GE(0.0001, (v1.value<vector3D>() - v2.value<vector3D>()).length());
      break;
    default:
      break;
    }
  }
  return v1.valueType() == v2.valueType();
}

MATCHER_P(IsShapeEqual, shape1, "")
{
  auto& sh1 = static_cast<shape_parameter const&>(shape1);
  auto& sh2 = static_cast<shape_parameter const&>(arg);
  if (sh1.numberOfValues() != sh2.numberOfValues()) {
    return false;
  }
  for (size_t i = 0; i < sh1.numberOfValues(); i++) {
    EXPECT_EQ(sh1.valueName(i), sh2.valueName(i));
    EXPECT_THAT(sh1.value(i), IsValueEqual(sh2.value(i)));
  }
  return shape1.getName() == arg.getName();
}

MATCHER_P(IsShapeEqual_0, shape1, "")
{
  auto& sh1 = static_cast<shape_parameter const&>(shape1);
  auto& sh2 = static_cast<shape_parameter const&>(arg);
  if (sh1.numberOfValues() != sh2.numberOfValues()) {
    return false;
  }
  for (size_t i = 0; i < sh1.numberOfValues(); i++) {
    EXPECT_EQ(sh1.valueName(i), sh2.valueName(i));
    EXPECT_EQ(sh1.value(i).valueType(), sh2.value(i).valueType());
  }
  return shape1.getName() == arg.getName();
}

MATCHER_P(IsDetectorEqual, detector1, "")
{
  auto& d1 = static_cast<detector_parameter const&>(detector1);
  auto& d2 = static_cast<detector_parameter const&>(arg);
  EXPECT_THAT(d1.getMaxDistance(), testing::FloatNear(d2.getMaxDistance(), 0.01f));
  EXPECT_THAT(*d1.getShape(), IsShapeEqual(*d2.getShape()));
  EXPECT_EQ(d1.getName(), d2.getName());
  EXPECT_EQ(d1.getDescription(), d2.getDescription());
  EXPECT_EQ(d1.getNumberOfElements(), d2.getNumberOfElements());
  EXPECT_EQ(d1.getStackType(), d2.getStackType());
  EXPECT_EQ(d1.getMaterial(), d2.getMaterial());
  EXPECT_EQ(d1.getMaterialId(), d2.getMaterialId());
  return true;
}
