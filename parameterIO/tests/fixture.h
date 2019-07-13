#pragma once

#include "parameterio.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace parameter::IO;

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

MATCHER_P(IsShapeEqual, shape1, "")
{
  auto& sh1 = static_cast<shape_parameter const&>(shape1);
  auto& sh2 = static_cast<shape_parameter const&>(arg);
  bool pointsEqual = (sh1.NumberOfParams<point3D>() == sh2.NumberOfParams<point3D>());
  if (pointsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<point3D>(); i++) {
      if ((!(sh1.getParam<point3D>(i) == sh2.getParam<point3D>(i))) ||
          sh1.getParamName<point3D>(i) != sh2.getParamName<point3D>(i)) {
        return false;
      }
    }
  }
  bool vectorsEqual = (sh1.NumberOfParams<vector3D>() == sh2.NumberOfParams<vector3D>());
  if (vectorsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<vector3D>(); i++) {
      if ((sh1.getParam<vector3D>(i) - sh2.getParam<vector3D>(i)).length() > 0.0001 ||
          sh1.getParamName<vector3D>(i) != sh2.getParamName<vector3D>(i)) {
        return false;
      }
    }
  }
  bool intsEqual = (sh1.NumberOfParams<int>() == sh2.NumberOfParams<int>());
  if (intsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<int>(); i++) {
      if ((!(sh1.getParam<int>(i) == sh2.getParam<int>(i))) ||
          sh1.getParamName<int>(i) != sh2.getParamName<int>(i)) {
        return false;
      }
    }
  }
  bool floatsEqual = (sh1.NumberOfParams<float>() == sh2.NumberOfParams<float>());
  if (floatsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<float>(); i++) {
      if ((!(sh1.getParam<float>(i) == sh2.getParam<float>(i))) ||
          sh1.getParamName<float>(i) != sh2.getParamName<float>(i)) {
        return false;
      }
    }
  }
  bool stringsEqual = (sh1.NumberOfParams<std::string>() == sh2.NumberOfParams<std::string>());
  if (stringsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<std::string>(); i++) {
      if ((!(sh1.getParam<std::string>(i) == sh2.getParam<std::string>(i))) ||
          sh1.getParamName<std::string>(i) != sh2.getParamName<std::string>(i)) {
        return false;
      }
    }
  }
  return shape1.getName() == arg.getName() && pointsEqual && vectorsEqual && intsEqual &&
         floatsEqual && stringsEqual;
}

MATCHER_P(IsShapeEqual_0, shape1, "")
{
  auto& sh1 = static_cast<shape_parameter const&>(shape1);
  auto& sh2 = static_cast<shape_parameter const&>(arg);
  bool pointsEqual = (sh1.NumberOfParams<point3D>() == sh2.NumberOfParams<point3D>());
  if (pointsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<point3D>(); i++) {
      if (sh1.getParamName<point3D>(i) != sh2.getParamName<point3D>(i)) {
        return false;
      }
    }
  }
  bool vectorsEqual = (sh1.NumberOfParams<vector3D>() == sh2.NumberOfParams<vector3D>());
  if (vectorsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<vector3D>(); i++) {
      if (sh1.getParamName<vector3D>(i) != sh2.getParamName<vector3D>(i)) {
        return false;
      }
    }
  }
  bool intsEqual = (sh1.NumberOfParams<int>() == sh2.NumberOfParams<int>());
  if (intsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<int>(); i++) {
      if (sh1.getParamName<int>(i) != sh2.getParamName<int>(i)) {
        return false;
      }
    }
  }
  bool floatsEqual = (sh1.NumberOfParams<float>() == sh2.NumberOfParams<float>());
  if (floatsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<float>(); i++) {
      if (sh1.getParamName<float>(i) != sh2.getParamName<float>(i)) {
        return false;
      }
    }
  }
  bool stringsEqual = (sh1.NumberOfParams<std::string>() == sh2.NumberOfParams<std::string>());
  if (stringsEqual) {
    for (int i = 0; i < sh1.NumberOfParams<std::string>(); i++) {
      if (sh1.getParamName<std::string>(i) != sh2.getParamName<std::string>(i)) {
        return false;
      }
    }
  }
  return shape1.getName() == arg.getName() && pointsEqual && vectorsEqual && intsEqual &&
         floatsEqual && stringsEqual;
}
