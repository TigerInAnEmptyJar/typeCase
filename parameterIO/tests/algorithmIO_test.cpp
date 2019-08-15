#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.algorithm_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, algorithm_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    algorithm_parameter a(*std::dynamic_pointer_cast<algorithm_parameter>(result[i]));
    algorithm_parameter b(*std::dynamic_pointer_cast<algorithm_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.algorithm_read_0
 * This test checks whether a previously written algorithm file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, algorithm_read_0)
{
  auto result =
      io.readParameter("data/algorithms_0.data", version_0, ParameterIO::FileType::ALGORITHM);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<algorithm_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(10, a->getID());
  EXPECT_EQ(true, a->IsUsed());
  EXPECT_EQ(0, a->getLevel());
  EXPECT_EQ(1, a->getCategory());
  EXPECT_EQ("testAlgorithm 1", a->getName());
  EXPECT_EQ("This is a test algorithm.", a->getDescription());
  ASSERT_EQ(6, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_FLOAT, a->value(5).valueType());
  EXPECT_EQ(0, a->value(0).value<int>());
  EXPECT_EQ(10, a->value(1).value<int>());
  EXPECT_EQ(5.5, a->value(2).value<float>());
  EXPECT_EQ("foo", a->value(3).value<std::string>());
  EXPECT_THAT(a->value(4).value<std::vector<int>>(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->value(5).value<std::vector<float>>(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->valueName(0));
  EXPECT_EQ("param2", a->valueName(1));
  EXPECT_EQ("param3", a->valueName(2));
  EXPECT_EQ("param4", a->valueName(3));
  EXPECT_EQ("param5", a->valueName(4));
  EXPECT_EQ("param6", a->valueName(5));

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(11, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, b->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(5).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, b->value(6).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_INT, b->value(7).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_INT, b->value(8).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_FLOAT, b->value(9).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_STRING, b->value(10).valueType());
  EXPECT_EQ(0, b->value(0).value<int>());
  EXPECT_EQ(10, b->value(1).value<int>());
  EXPECT_EQ(7, b->value(2).value<int>());
  EXPECT_EQ(5.5, b->value(3).value<float>());
  EXPECT_EQ(5.5, b->value(4).value<float>());
  EXPECT_EQ("foo", b->value(5).value<std::string>());
  EXPECT_EQ("bar", b->value(6).value<std::string>());
  EXPECT_THAT(b->value(7).value<std::vector<int>>(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(b->value(8).value<std::vector<int>>(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(b->value(9).value<std::vector<float>>(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_THAT(b->value(10).value<std::vector<std::string>>(),
              testing::ContainerEq(std::vector<std::string>{"a", "b", "c"}));
  EXPECT_EQ("param1", b->valueName(0));
  EXPECT_EQ("param2", b->valueName(1));
  EXPECT_EQ("param-1", b->valueName(2));
  EXPECT_EQ("param3", b->valueName(3));
  EXPECT_EQ("param-3", b->valueName(4));
  EXPECT_EQ("param4", b->valueName(5));
  EXPECT_EQ("param-4", b->valueName(6));
  EXPECT_EQ("param5", b->valueName(7));
  EXPECT_EQ("param-5", b->valueName(8));
  EXPECT_EQ("param6", b->valueName(9));
  EXPECT_EQ("param-6", b->valueName(10));
}

/*!
 * \brief ParameterIoTest.algorithm_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, algorithm_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    algorithm_parameter a(*std::dynamic_pointer_cast<algorithm_parameter>(result[i]));
    algorithm_parameter b(*std::dynamic_pointer_cast<algorithm_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.algorithm_read_1
 * This test checks whether a previously written algorithm file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, algorithm_read_1)
{
  auto result =
      io.readParameter("data/algorithms_1.data", version_1, ParameterIO::FileType::ALGORITHM);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<algorithm_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(10, a->getID());
  EXPECT_EQ(true, a->IsUsed());
  EXPECT_EQ(0, a->getLevel());
  EXPECT_EQ(1, a->getCategory());
  EXPECT_EQ("testAlgorithm 1", a->getName());
  EXPECT_EQ("This is a test algorithm.", a->getDescription());
  ASSERT_EQ(6, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_FLOAT, a->value(5).valueType());
  EXPECT_EQ(0, a->value(0).value<int>());
  EXPECT_EQ(10, a->value(1).value<int>());
  EXPECT_EQ(5.5, a->value(2).value<float>());
  EXPECT_EQ("foo", a->value(3).value<std::string>());
  EXPECT_THAT(a->value(4).value<std::vector<int>>(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->value(5).value<std::vector<float>>(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->valueName(0));
  EXPECT_EQ("param2", a->valueName(1));
  EXPECT_EQ("param3", a->valueName(2));
  EXPECT_EQ("param4", a->valueName(3));
  EXPECT_EQ("param5", a->valueName(4));
  EXPECT_EQ("param6", a->valueName(5));

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(4, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::BOOLEAN, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::BOOLEAN, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, b->value(3).valueType());
  EXPECT_EQ(false, b->value(0).value<bool>());
  EXPECT_EQ(true, b->value(1).value<bool>());
  EXPECT_EQ(point3D(1, 2, 3), b->value(2).value<point3D>());
  EXPECT_EQ(vector3D(4, 5, 6), b->value(3).value<vector3D>());
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
}

/*!
 * \brief ParameterIoTest.algorithm_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, algorithm_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    algorithm_parameter a(*std::dynamic_pointer_cast<algorithm_parameter>(result[i]));
    algorithm_parameter b(*std::dynamic_pointer_cast<algorithm_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the algorithm.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/* \brief ParameterIoTest.algorithm_read_2
 * This test checks whether a previously written algorithm file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, algorithm_read_2)
{
  auto result =
      io.readParameter("data/algorithms_2.data", version_2, ParameterIO::FileType::ALGORITHM);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<algorithm_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(10, a->getID());
  EXPECT_EQ(true, a->IsUsed());
  EXPECT_EQ(0, a->getLevel());
  EXPECT_EQ(1, a->getCategory());
  EXPECT_EQ("testAlgorithm 1", a->getName());
  EXPECT_EQ("This is a test algorithm.", a->getDescription());
  ASSERT_EQ(6, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::STRING, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_INT, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR_FLOAT, a->value(5).valueType());
  EXPECT_EQ(0, a->value(0).value<int>());
  EXPECT_EQ(10, a->value(1).value<int>());
  EXPECT_EQ(5.5, a->value(2).value<float>());
  EXPECT_EQ("foo", a->value(3).value<std::string>());
  EXPECT_THAT(a->value(4).value<std::vector<int>>(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->value(5).value<std::vector<float>>(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->valueName(0));
  EXPECT_EQ("param2", a->valueName(1));
  EXPECT_EQ("param3", a->valueName(2));
  EXPECT_EQ("param4", a->valueName(3));
  EXPECT_EQ("param5", a->valueName(4));
  EXPECT_EQ("param6", a->valueName(5));

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(4, b->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::BOOLEAN, b->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::BOOLEAN, b->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, b->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, b->value(3).valueType());
  EXPECT_EQ(false, b->value(0).value<bool>());
  EXPECT_EQ(true, b->value(1).value<bool>());
  EXPECT_EQ(point3D(1, 2, 3), b->value(2).value<point3D>());
  EXPECT_EQ(vector3D(4, 5, 6), b->value(3).value<vector3D>());
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
}

TEST_F(ParameterIoTest, algorithm_id_recovery_0)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}

TEST_F(ParameterIoTest, algorithm_id_recovery_1)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}

TEST_F(ParameterIoTest, algorithm_id_recovery_2)
{
  installRecovery();
  QTemporaryDir tempPath;
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    EXPECT_EQ(parameters[i]->id(), result[i]->id());
  }
}
