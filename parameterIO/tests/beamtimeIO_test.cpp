#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.beamtime_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, beamtime_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/beamtimes_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::BEAMTIME);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::BEAMTIME);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    beamTime_parameter a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
    beamTime_parameter b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the beamtime.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.beamtime_read_0
 * This test checks whether a previously written beamtime file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, beamtime_read_0)
{
  auto result =
      io.readParameter("data/beamtimes_0.data", version_0, ParameterIO::FileType::BEAMTIME);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("beamtime 1", a->getName());
  EXPECT_EQ(12, a->getMonth());
  EXPECT_EQ(2015, a->getYear());
  EXPECT_EQ("data/run1.data", a->getFileName());
  EXPECT_EQ("data/setup1.data", a->getSetupFile());
  ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0));
  EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1));
  EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2));
  EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3));

  auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("beamtime 2", b->getName());
  EXPECT_EQ(1, b->getMonth());
  EXPECT_EQ(2015, b->getYear());
  EXPECT_EQ("data/run2.data", b->getFileName());
  EXPECT_EQ("data/setup2.data", b->getSetupFile());
  ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0));
  EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1));
  EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2));
  EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3));
}

/*!
 * \brief ParameterIoTest.beamtime_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, beamtime_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/beamtimes_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::BEAMTIME);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::BEAMTIME);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    beamTime_parameter a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
    beamTime_parameter b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the beamtime.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.beamtime_read_1
 * This test checks whether a previously written beamtime file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, beamtime_read_1)
{
  auto result =
      io.readParameter("data/beamtimes_1.data", version_1, ParameterIO::FileType::BEAMTIME);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("beamtime 1", a->getName());
  EXPECT_EQ(12, a->getMonth());
  EXPECT_EQ(2015, a->getYear());
  EXPECT_EQ("data/run1.data", a->getFileName());
  EXPECT_EQ("data/setup1.data", a->getSetupFile());
  ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0));
  EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1));
  EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2));
  EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3));

  auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("beamtime 2", b->getName());
  EXPECT_EQ(1, b->getMonth());
  EXPECT_EQ(2015, b->getYear());
  EXPECT_EQ("data/run2.data", b->getFileName());
  EXPECT_EQ("data/setup2.data", b->getSetupFile());
  ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0));
  EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1));
  EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2));
  EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3));
}

/*!
 * \brief ParameterIoTest.beamtime_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, beamtime_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/beamtimes_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::BEAMTIME);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::BEAMTIME);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    beamTime_parameter a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
    beamTime_parameter b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the beamtime.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*! \brief ParameterIoTest.beamtime_read_2
 * This test checks whether a previously written beamtime file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, beamtime_read_2)
{
  auto result =
      io.readParameter("data/beamtimes_2.data", version_2, ParameterIO::FileType::BEAMTIME);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("beamtime 1", a->getName());
  EXPECT_EQ(12, a->getMonth());
  EXPECT_EQ(2015, a->getYear());
  EXPECT_EQ("data/run1.data", a->getFileName());
  EXPECT_EQ("data/setup1.data", a->getSetupFile());
  ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0));
  EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1));
  EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2));
  EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3));

  auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("beamtime 2", b->getName());
  EXPECT_EQ(1, b->getMonth());
  EXPECT_EQ(2015, b->getYear());
  EXPECT_EQ("data/run2.data", b->getFileName());
  EXPECT_EQ("data/setup2.data", b->getSetupFile());
  ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0));
  EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1));
  EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2));
  EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3));
}

TEST_F(ParameterIoTest, beamtime_write)
{
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);

  std::filesystem::path path1{"data/beamtimes_0.data"};
  io.writeParameter(path1, parameters, version_0, ParameterIO::FileType::BEAMTIME);
  std::filesystem::path path2{"data/beamtimes_1.data"};
  io.writeParameter(path2, parameters, version_1, ParameterIO::FileType::BEAMTIME);
  std::filesystem::path path3{"data/beamtimes_2.data"};
  io.writeParameter(path3, parameters, version_2, ParameterIO::FileType::BEAMTIME);
}
