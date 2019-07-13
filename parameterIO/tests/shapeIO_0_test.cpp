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
  ASSERT_EQ(2, a->NumberOfParams<int>());
  ASSERT_EQ(1, a->NumberOfParams<float>());
  ASSERT_EQ(0, a->NumberOfParams<std::string>());
  EXPECT_EQ(2, a->NumberOfParams<point3D>());
  EXPECT_EQ(2, a->NumberOfParams<vector3D>());
  EXPECT_EQ("param1", a->getParamName<int>(0));
  EXPECT_EQ("param2", a->getParamName<int>(1));
  EXPECT_EQ("param3", a->getParamName<float>(0));
  EXPECT_EQ("param4", a->getParamName<point3D>(0));
  EXPECT_EQ("param5", a->getParamName<point3D>(1));
  EXPECT_EQ("param6", a->getParamName<vector3D>(0));
  EXPECT_EQ("param7", a->getParamName<vector3D>(1));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(1, b->NumberOfParams<int>());
  ASSERT_EQ(2, b->NumberOfParams<float>());
  ASSERT_EQ(2, b->NumberOfParams<std::string>());
  EXPECT_EQ(0, b->NumberOfParams<point3D>());
  EXPECT_EQ(0, b->NumberOfParams<vector3D>());
  EXPECT_EQ("param-1", b->getParamName<int>(0));
  EXPECT_EQ("param-2", b->getParamName<float>(0));
  EXPECT_EQ("param-3", b->getParamName<float>(1));
  EXPECT_EQ("param-4", b->getParamName<std::string>(0));
  EXPECT_EQ("param-5", b->getParamName<std::string>(1));
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
  ASSERT_EQ(2, a->NumberOfParams<int>());
  ASSERT_EQ(1, a->NumberOfParams<float>());
  ASSERT_EQ(0, a->NumberOfParams<std::string>());
  EXPECT_EQ(2, a->NumberOfParams<point3D>());
  EXPECT_EQ(2, a->NumberOfParams<vector3D>());
  EXPECT_EQ(10, a->getParam<int>(0));
  EXPECT_EQ(26, a->getParam<int>(1));
  EXPECT_EQ(123.456f, a->getParam<float>(0));
  EXPECT_EQ(point3D(1, 2, 3), a->getParam<point3D>(0));
  EXPECT_EQ(point3D(4, 5, 6), a->getParam<point3D>(1));
  EXPECT_LE((vector3D(1.2, 3.4, 5.6) - a->getParam<vector3D>(0)).length(), 0.0001);
  EXPECT_LE((vector3D(11, 22, 33) - a->getParam<vector3D>(1)).length(), 0.0001);
  EXPECT_EQ("param1", a->getParamName<int>(0));
  EXPECT_EQ("param2", a->getParamName<int>(1));
  EXPECT_EQ("param3", a->getParamName<float>(0));
  EXPECT_EQ("param4", a->getParamName<point3D>(0));
  EXPECT_EQ("param5", a->getParamName<point3D>(1));
  EXPECT_EQ("param6", a->getParamName<vector3D>(0));
  EXPECT_EQ("param7", a->getParamName<vector3D>(1));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(true, b->completeWrite());
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(1, b->NumberOfParams<int>());
  ASSERT_EQ(2, b->NumberOfParams<float>());
  ASSERT_EQ(2, b->NumberOfParams<std::string>());
  EXPECT_EQ(0, b->NumberOfParams<point3D>());
  EXPECT_EQ(0, b->NumberOfParams<vector3D>());
  EXPECT_EQ(10, b->getParam<int>(0));
  EXPECT_EQ(26.7f, b->getParam<float>(0));
  EXPECT_EQ(123.456f, b->getParam<float>(1));
  EXPECT_EQ("foo", b->getParam<std::string>(0));
  EXPECT_EQ("bar", b->getParam<std::string>(1));
  EXPECT_EQ("param-1", b->getParamName<int>(0));
  EXPECT_EQ("param-2", b->getParamName<float>(0));
  EXPECT_EQ("param-3", b->getParamName<float>(1));
  EXPECT_EQ("param-4", b->getParamName<std::string>(0));
  EXPECT_EQ("param-5", b->getParamName<std::string>(1));
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
  ASSERT_EQ(2, a->NumberOfParams<int>());
  ASSERT_EQ(1, a->NumberOfParams<float>());
  ASSERT_EQ(0, a->NumberOfParams<std::string>());
  EXPECT_EQ(2, a->NumberOfParams<point3D>());
  EXPECT_EQ(2, a->NumberOfParams<vector3D>());
  EXPECT_EQ(10, a->getParam<int>(0));
  EXPECT_EQ(26, a->getParam<int>(1));
  EXPECT_EQ(123.456f, a->getParam<float>(0));
  EXPECT_EQ(point3D(1, 2, 3), a->getParam<point3D>(0));
  EXPECT_EQ(point3D(4, 5, 6), a->getParam<point3D>(1));
  EXPECT_GE(0.0001, (vector3D(1.2, 3.4, 5.6) - a->getParam<vector3D>(0)).length());
  EXPECT_GE(0.0001, (vector3D(11, 22, 33) - a->getParam<vector3D>(1)).length());
  EXPECT_EQ("param1", a->getParamName<int>(0));
  EXPECT_EQ("param2", a->getParamName<int>(1));
  EXPECT_EQ("param3", a->getParamName<float>(0));
  EXPECT_EQ("param4", a->getParamName<point3D>(0));
  EXPECT_EQ("param5", a->getParamName<point3D>(1));
  EXPECT_EQ("param6", a->getParamName<vector3D>(0));
  EXPECT_EQ("param7", a->getParamName<vector3D>(1));

  auto b = std::dynamic_pointer_cast<shape_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(true, b->completeWrite());
  EXPECT_EQ("testShape 2", b->getName());
  ASSERT_EQ(1, b->NumberOfParams<int>());
  ASSERT_EQ(2, b->NumberOfParams<float>());
  ASSERT_EQ(2, b->NumberOfParams<std::string>());
  EXPECT_EQ(0, b->NumberOfParams<point3D>());
  EXPECT_EQ(0, b->NumberOfParams<vector3D>());
  EXPECT_EQ(10, b->getParam<int>(0));
  EXPECT_EQ(26.7f, b->getParam<float>(0));
  EXPECT_EQ(123.456f, b->getParam<float>(1));
  EXPECT_EQ("foo", b->getParam<std::string>(0));
  EXPECT_EQ("bar", b->getParam<std::string>(1));
  EXPECT_EQ("param-1", b->getParamName<int>(0));
  EXPECT_EQ("param-2", b->getParamName<float>(0));
  EXPECT_EQ("param-3", b->getParamName<float>(1));
  EXPECT_EQ("param-4", b->getParamName<std::string>(0));
  EXPECT_EQ("param-5", b->getParamName<std::string>(1));
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
