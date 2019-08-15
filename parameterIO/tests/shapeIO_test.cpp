#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.shape_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, shape_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(result[i]));
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    shape_parameter a(*std::dynamic_pointer_cast<shape_parameter>(result[i]));
    shape_parameter b(*std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(result[i]->id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_THAT(a, IsShapeEqual_0(b));
  }
}

/*!
 * \brief ParameterIoTest.shape_read_0
 * This test checks whether a previously written shape file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, shape_read_0)
{
  auto result = io.readParameter("data/shapes_0.data", version_0, ParameterIO::FileType::SHAPE);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<shape_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testShape 1", a->getName());
  ASSERT_EQ(7, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(5).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(6).valueType());
  EXPECT_EQ("param4", a->valueName(0));
  EXPECT_EQ("param5", a->valueName(1));
  EXPECT_EQ("param6", a->valueName(2));
  EXPECT_EQ("param7", a->valueName(3));
  EXPECT_EQ("param1", a->valueName(4));
  EXPECT_EQ("param2", a->valueName(5));
  EXPECT_EQ("param3", a->valueName(6));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(5, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(4).valueType());
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
  EXPECT_EQ("param-5", b->valueName(4));
}

/*!
 * \brief ParameterIoTest.shape_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, shape_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(result[i]));
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    shape_parameter a(*std::dynamic_pointer_cast<shape_parameter>(result[i]));
    shape_parameter b(*std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(result[i]->id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_THAT(a, IsShapeEqual(b));
  }
}

/*!
 * \brief ParameterIoTest.shape_read_1
 * This test checks whether a previously written shape file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, shape_read_1)
{
  auto result = io.readParameter("data/shapes_1.data", version_1, ParameterIO::FileType::SHAPE);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<shape_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(true, a->completeWrite());
  EXPECT_EQ("testShape 1", a->getName());
  ASSERT_EQ(7, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(5).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(6).valueType());
  EXPECT_EQ("param4", a->valueName(0));
  EXPECT_EQ("param5", a->valueName(1));
  EXPECT_EQ("param6", a->valueName(2));
  EXPECT_EQ("param7", a->valueName(3));
  EXPECT_EQ("param1", a->valueName(4));
  EXPECT_EQ("param2", a->valueName(5));
  EXPECT_EQ("param3", a->valueName(6));
  EXPECT_EQ(point3D(1, 2, 3), a->value<point3D>(0));
  EXPECT_EQ(point3D(4, 5, 6), a->value<point3D>(1));
  EXPECT_LE((vector3D(1.2, 3.4, 5.6) - a->value<vector3D>(2)).length(), 0.0001);
  EXPECT_LE((vector3D(11, 22, 33) - a->value<vector3D>(3)).length(), 0.0001);
  EXPECT_EQ(10, a->value<int>(4));
  EXPECT_EQ(26, a->value<int>(5));
  EXPECT_EQ(123.456f, a->value<float>(6));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(true, b->completeWrite());
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(5, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(4).valueType());
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
  EXPECT_EQ("param-5", b->valueName(4));
  EXPECT_EQ(10, b->value<int>(0));
  EXPECT_EQ(26.7f, b->value<float>(1));
  EXPECT_EQ(123.456f, b->value<float>(2));
  EXPECT_EQ("foo", b->value<std::string>(3));
  EXPECT_EQ("bar", b->value<std::string>(4));
}

/*!
 * \brief ParameterIoTest.shape_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, shape_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(result[i]));
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    shape_parameter a(*std::dynamic_pointer_cast<shape_parameter>(result[i]));
    shape_parameter b(*std::dynamic_pointer_cast<shape_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(result[i]->id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_THAT(b, IsShapeEqual(a));
  }
}

/*!
 * \brief ParameterIoTest.shape_read_2
 * This test checks whether a previously written shape file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, shape_read_2)
{
  auto result = io.readParameter("data/shapes_2.data", version_2, ParameterIO::FileType::SHAPE);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<shape_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(true, a->completeWrite());
  EXPECT_EQ("testShape 1", a->getName());
  ASSERT_EQ(7, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(5).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(6).valueType());
  EXPECT_EQ("param4", a->valueName(0));
  EXPECT_EQ("param5", a->valueName(1));
  EXPECT_EQ("param6", a->valueName(2));
  EXPECT_EQ("param7", a->valueName(3));
  EXPECT_EQ("param1", a->valueName(4));
  EXPECT_EQ("param2", a->valueName(5));
  EXPECT_EQ("param3", a->valueName(6));
  EXPECT_EQ(point3D(1, 2, 3), a->value<point3D>(0));
  EXPECT_EQ(point3D(4, 5, 6), a->value<point3D>(1));
  EXPECT_LE((vector3D(1.2, 3.4, 5.6) - a->value<vector3D>(2)).length(), 0.0001);
  EXPECT_LE((vector3D(11, 22, 33) - a->value<vector3D>(3)).length(), 0.0001);
  EXPECT_EQ(10, a->value<int>(4));
  EXPECT_EQ(26, a->value<int>(5));
  EXPECT_EQ(123.456f, a->value<float>(6));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(true, b->completeWrite());
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(5, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(4).valueType());
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
  EXPECT_EQ("param-5", b->valueName(4));
  EXPECT_EQ(10, b->value<int>(0));
  EXPECT_EQ(26.7f, b->value<float>(1));
  EXPECT_EQ(123.456f, b->value<float>(2));
  EXPECT_EQ("foo", b->value<std::string>(3));
  EXPECT_EQ("bar", b->value<std::string>(4));
}

TEST_F(ParameterIoTest, shape_id_recovery_0)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}

TEST_F(ParameterIoTest, shape_id_recovery_1)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}

TEST_F(ParameterIoTest, shape_id_recovery_2)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::SHAPE);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::SHAPE);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}
