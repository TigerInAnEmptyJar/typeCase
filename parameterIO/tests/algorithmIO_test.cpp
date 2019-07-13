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
  ASSERT_EQ(2, a->getNumberOfParam<int>());
  ASSERT_EQ(1, a->getNumberOfParam<float>());
  ASSERT_EQ(1, a->getNumberOfParam<std::string>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<int>>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<float>>());
  EXPECT_EQ(0, a->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(0, a->getNumberOfParam<point3D>());
  EXPECT_EQ(0, a->getNumberOfParam<vector3D>());
  EXPECT_EQ(0, a->getNumberOfParam<bool>());
  EXPECT_EQ(0, a->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(0, a->getParam<int>(0).getData());
  EXPECT_EQ(10, a->getParam<int>(1).getData());
  EXPECT_EQ(5.5, a->getParam<float>(0).getData());
  EXPECT_EQ("foo", a->getParam<std::string>(0).getData());
  EXPECT_THAT(a->getParam<std::vector<int>>(0).getData(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->getParam<std::vector<float>>(0).getData(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->getParam<int>(0).getName());
  EXPECT_EQ("param2", a->getParam<int>(1).getName());
  EXPECT_EQ("param3", a->getParam<float>(0).getName());
  EXPECT_EQ("param4", a->getParam<std::string>(0).getName());
  EXPECT_EQ("param5", a->getParam<std::vector<int>>(0).getName());
  EXPECT_EQ("param6", a->getParam<std::vector<float>>(0).getName());

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(3, b->getNumberOfParam<int>());
  ASSERT_EQ(2, b->getNumberOfParam<float>());
  ASSERT_EQ(2, b->getNumberOfParam<std::string>());
  ASSERT_EQ(2, b->getNumberOfParam<std::vector<int>>());
  EXPECT_EQ(1, b->getNumberOfParam<std::vector<float>>());
  ASSERT_EQ(1, b->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(0, b->getNumberOfParam<point3D>());
  EXPECT_EQ(0, b->getNumberOfParam<vector3D>());
  EXPECT_EQ(0, b->getNumberOfParam<bool>());
  EXPECT_EQ(0, b->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(0, b->getParam<int>(0).getData());
  EXPECT_EQ(10, b->getParam<int>(1).getData());
  EXPECT_EQ(7, b->getParam<int>(2).getData());
  EXPECT_EQ(5.5, b->getParam<float>(0).getData());
  EXPECT_EQ(5.5, b->getParam<float>(1).getData());
  EXPECT_EQ("foo", b->getParam<std::string>(0).getData());
  EXPECT_EQ("bar", b->getParam<std::string>(1).getData());
  EXPECT_THAT(b->getParam<std::vector<int>>(0).getData(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(b->getParam<std::vector<int>>(1).getData(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(b->getParam<std::vector<float>>(0).getData(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_THAT(b->getParam<std::vector<std::string>>(0).getData(),
              testing::ContainerEq(std::vector<std::string>{"a", "b", "c"}));
  EXPECT_EQ("param1", b->getParam<int>(0).getName());
  EXPECT_EQ("param2", b->getParam<int>(1).getName());
  EXPECT_EQ("param-1", b->getParam<int>(2).getName());
  EXPECT_EQ("param3", b->getParam<float>(0).getName());
  EXPECT_EQ("param-3", b->getParam<float>(1).getName());
  EXPECT_EQ("param4", b->getParam<std::string>(0).getName());
  EXPECT_EQ("param-4", b->getParam<std::string>(1).getName());
  EXPECT_EQ("param5", b->getParam<std::vector<int>>(0).getName());
  EXPECT_EQ("param-5", b->getParam<std::vector<int>>(1).getName());
  EXPECT_EQ("param6", b->getParam<std::vector<float>>(0).getName());
  EXPECT_EQ("param-6", b->getParam<std::vector<std::string>>(0).getName());
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
  ASSERT_EQ(2, a->getNumberOfParam<int>());
  ASSERT_EQ(1, a->getNumberOfParam<float>());
  ASSERT_EQ(1, a->getNumberOfParam<std::string>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<int>>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<float>>());
  EXPECT_EQ(0, a->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(0, a->getNumberOfParam<point3D>());
  EXPECT_EQ(0, a->getNumberOfParam<vector3D>());
  EXPECT_EQ(0, a->getNumberOfParam<bool>());
  EXPECT_EQ(0, a->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(0, a->getParam<int>(0).getData());
  EXPECT_EQ(10, a->getParam<int>(1).getData());
  EXPECT_EQ(5.5, a->getParam<float>(0).getData());
  EXPECT_EQ("foo", a->getParam<std::string>(0).getData());
  EXPECT_THAT(a->getParam<std::vector<int>>(0).getData(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->getParam<std::vector<float>>(0).getData(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->getParam<int>(0).getName());
  EXPECT_EQ("param2", a->getParam<int>(1).getName());
  EXPECT_EQ("param3", a->getParam<float>(0).getName());
  EXPECT_EQ("param4", a->getParam<std::string>(0).getName());
  EXPECT_EQ("param5", a->getParam<std::vector<int>>(0).getName());
  EXPECT_EQ("param6", a->getParam<std::vector<float>>(0).getName());

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(0, b->getNumberOfParam<int>());
  ASSERT_EQ(0, b->getNumberOfParam<float>());
  ASSERT_EQ(0, b->getNumberOfParam<std::string>());
  ASSERT_EQ(0, b->getNumberOfParam<std::vector<int>>());
  EXPECT_EQ(0, b->getNumberOfParam<std::vector<float>>());
  ASSERT_EQ(0, b->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(1, b->getNumberOfParam<point3D>());
  EXPECT_EQ(1, b->getNumberOfParam<vector3D>());
  EXPECT_EQ(2, b->getNumberOfParam<bool>());
  EXPECT_EQ(0, b->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(false, b->getParam<bool>(0).getData());
  EXPECT_EQ(true, b->getParam<bool>(1).getData());
  EXPECT_EQ(point3D(1, 2, 3), b->getParam<point3D>(0).getData());
  EXPECT_EQ(vector3D(4, 5, 6), b->getParam<vector3D>(0).getData());
  EXPECT_EQ("param-1", b->getParam<bool>(0).getName());
  EXPECT_EQ("param-2", b->getParam<bool>(1).getName());
  EXPECT_EQ("param-3", b->getParam<point3D>(0).getName());
  EXPECT_EQ("param-4", b->getParam<vector3D>(0).getName());
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
  ASSERT_EQ(2, a->getNumberOfParam<int>());
  ASSERT_EQ(1, a->getNumberOfParam<float>());
  ASSERT_EQ(1, a->getNumberOfParam<std::string>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<int>>());
  ASSERT_EQ(1, a->getNumberOfParam<std::vector<float>>());
  EXPECT_EQ(0, a->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(0, a->getNumberOfParam<point3D>());
  EXPECT_EQ(0, a->getNumberOfParam<vector3D>());
  EXPECT_EQ(0, a->getNumberOfParam<bool>());
  EXPECT_EQ(0, a->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(0, a->getParam<int>(0).getData());
  EXPECT_EQ(10, a->getParam<int>(1).getData());
  EXPECT_EQ(5.5, a->getParam<float>(0).getData());
  EXPECT_EQ("foo", a->getParam<std::string>(0).getData());
  EXPECT_THAT(a->getParam<std::vector<int>>(0).getData(),
              testing::ContainerEq(std::vector<int>{0, 1, 2, 3, 4, 5, 6}));
  EXPECT_THAT(a->getParam<std::vector<float>>(0).getData(),
              testing::ContainerEq(std::vector<float>{0.7, 1.45, 5.7}));
  EXPECT_EQ("param1", a->getParam<int>(0).getName());
  EXPECT_EQ("param2", a->getParam<int>(1).getName());
  EXPECT_EQ("param3", a->getParam<float>(0).getName());
  EXPECT_EQ("param4", a->getParam<std::string>(0).getName());
  EXPECT_EQ("param5", a->getParam<std::vector<int>>(0).getName());
  EXPECT_EQ("param6", a->getParam<std::vector<float>>(0).getName());

  auto b = std::dynamic_pointer_cast<algorithm_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ(34, b->getID());
  EXPECT_EQ(false, b->IsUsed());
  EXPECT_EQ(5, b->getLevel());
  EXPECT_EQ(2, b->getCategory());
  EXPECT_EQ("testAlgorithm 2", b->getName());
  EXPECT_EQ("This is a new test algorithm.", b->getDescription());
  ASSERT_EQ(0, b->getNumberOfParam<int>());
  ASSERT_EQ(0, b->getNumberOfParam<float>());
  ASSERT_EQ(0, b->getNumberOfParam<std::string>());
  ASSERT_EQ(0, b->getNumberOfParam<std::vector<int>>());
  EXPECT_EQ(0, b->getNumberOfParam<std::vector<float>>());
  ASSERT_EQ(0, b->getNumberOfParam<std::vector<std::string>>());
  EXPECT_EQ(1, b->getNumberOfParam<point3D>());
  EXPECT_EQ(1, b->getNumberOfParam<vector3D>());
  EXPECT_EQ(2, b->getNumberOfParam<bool>());
  EXPECT_EQ(0, b->getNumberOfParam<algorithm_parameter>());
  EXPECT_EQ(false, b->getParam<bool>(0).getData());
  EXPECT_EQ(true, b->getParam<bool>(1).getData());
  EXPECT_EQ(point3D(1, 2, 3), b->getParam<point3D>(0).getData());
  EXPECT_EQ(vector3D(4, 5, 6), b->getParam<vector3D>(0).getData());
  EXPECT_EQ("param-1", b->getParam<bool>(0).getName());
  EXPECT_EQ("param-2", b->getParam<bool>(1).getName());
  EXPECT_EQ("param-3", b->getParam<point3D>(0).getName());
  EXPECT_EQ("param-4", b->getParam<vector3D>(0).getName());
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
