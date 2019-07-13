#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.run_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, run_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/runs_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::RUN);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::RUN);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return !std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    run_parameter a(*std::dynamic_pointer_cast<run_parameter>(result[i]));
    run_parameter b(*std::dynamic_pointer_cast<run_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the run.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.run_read_0
 * This test checks whether a previously written run file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, run_read_0)
{
  auto result = io.readParameter("data/runs_0.data", version_0, ParameterIO::FileType::RUN);
  ASSERT_THAT(result, testing::SizeIs(4));
  auto a = std::dynamic_pointer_cast<run_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("run 1-1", a->getName());
  EXPECT_EQ("This is a test run of beamtime 1.", a->getDescription());
  EXPECT_EQ(run_parameter::RunType::BACKGROUND, a->getType());
  EXPECT_EQ(1000, a->getRunNumber());
  // this version cannot handle times
  ASSERT_EQ(1, a->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_1.data", a->getFile(0));
  EXPECT_EQ(4, a->getFileType(0));
  EXPECT_EQ(false, a->hasOwnSetup());
  EXPECT_EQ(false, a->hasOwnCalibration());
  EXPECT_EQ(false, a->hasAdditionalCalibration());

  auto b = std::dynamic_pointer_cast<run_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("run 1-2", b->getName());
  EXPECT_EQ("This is a second test run of beamtime 1.", b->getDescription());
  EXPECT_EQ(1001, b->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::ELASTIC, b->getType());
  // this version cannot handle times
  ASSERT_EQ(1, b->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_2.data", b->getFile(0));
  EXPECT_EQ(7, b->getFileType(0));
  EXPECT_EQ(false, b->hasOwnSetup());
  EXPECT_EQ(false, b->hasOwnCalibration());
  EXPECT_EQ(false, b->hasAdditionalCalibration());

  auto c = std::dynamic_pointer_cast<run_parameter>(result[2]);
  ASSERT_NE(nullptr, c);
  EXPECT_EQ("run 2-1", c->getName());
  EXPECT_EQ("This is a test run of beamtime 2.", c->getDescription());
  EXPECT_EQ(2000, c->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::REGULAR, c->getType());
  // this version cannot handle times
  ASSERT_EQ(2, c->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0));
  EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1));
  EXPECT_EQ(4, c->getFileType(0));
  EXPECT_EQ(8, c->getFileType(1));
  EXPECT_EQ(false, c->hasOwnSetup());
  EXPECT_EQ(false, c->hasOwnCalibration());
  // this version cannot handle additional calibration
  EXPECT_EQ(false, c->hasAdditionalCalibration());
  EXPECT_EQ(0, c->getNumberOfCalibrationFiles());

  auto d = std::dynamic_pointer_cast<run_parameter>(result[3]);
  ASSERT_NE(nullptr, d);
  EXPECT_EQ("run 2-2", d->getName());
  EXPECT_EQ("This is a second test run of beamtime 2.", d->getDescription());
  EXPECT_EQ(2000, d->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::CALIBRATION, d->getType());
  // this version cannot handle times
  ASSERT_EQ(2, d->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0));
  EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1));
  EXPECT_EQ(4, d->getFileType(0));
  EXPECT_EQ(8, d->getFileType(1));
  EXPECT_EQ(true, d->hasOwnSetup());
  EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile());
  EXPECT_EQ(true, d->hasOwnCalibration());
  EXPECT_EQ(false, d->hasAdditionalCalibration());
  EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0));
  EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1));
  EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2));
}

/*!
 * \brief ParameterIoTest.run_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, run_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/runs_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::RUN);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::RUN);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return !std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    run_parameter a(*std::dynamic_pointer_cast<run_parameter>(result[i]));
    run_parameter b(*std::dynamic_pointer_cast<run_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the run.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.run_read_1
 * This test checks whether a previously written run file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, run_read_1)
{
  auto result = io.readParameter("data/runs_1.data", version_1, ParameterIO::FileType::RUN);
  ASSERT_THAT(result, testing::SizeIs(4));
  auto a = std::dynamic_pointer_cast<run_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("run 1-1", a->getName());
  EXPECT_EQ("This is a test run of beamtime 1.", a->getDescription());
  EXPECT_EQ(run_parameter::RunType::BACKGROUND, a->getType());
  EXPECT_EQ(1000, a->getRunNumber());
  // this version cannot handle times
  ASSERT_EQ(1, a->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_1.data", a->getFile(0));
  EXPECT_EQ(4, a->getFileType(0));
  EXPECT_EQ(false, a->hasOwnSetup());
  EXPECT_EQ(false, a->hasOwnCalibration());
  EXPECT_EQ(false, a->hasAdditionalCalibration());

  auto b = std::dynamic_pointer_cast<run_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("run 1-2", b->getName());
  EXPECT_EQ("This is a second test run of beamtime 1.", b->getDescription());
  EXPECT_EQ(1001, b->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::ELASTIC, b->getType());
  // this version cannot handle times
  ASSERT_EQ(1, b->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_2.data", b->getFile(0));
  EXPECT_EQ(7, b->getFileType(0));
  EXPECT_EQ(false, b->hasOwnSetup());
  EXPECT_EQ(false, b->hasOwnCalibration());
  EXPECT_EQ(false, b->hasAdditionalCalibration());

  auto c = std::dynamic_pointer_cast<run_parameter>(result[2]);
  ASSERT_NE(nullptr, c);
  EXPECT_EQ("run 2-1", c->getName());
  EXPECT_EQ("This is a test run of beamtime 2.", c->getDescription());
  EXPECT_EQ(2000, c->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::REGULAR, c->getType());
  // this version cannot handle times
  ASSERT_EQ(2, c->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0));
  EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1));
  EXPECT_EQ(4, c->getFileType(0));
  EXPECT_EQ(8, c->getFileType(1));
  EXPECT_EQ(false, c->hasOwnSetup());
  EXPECT_EQ(false, c->hasOwnCalibration());
  // this version cannot handle additional calibration
  EXPECT_EQ(false, c->hasAdditionalCalibration());
  EXPECT_EQ(0, c->getNumberOfCalibrationFiles());

  auto d = std::dynamic_pointer_cast<run_parameter>(result[3]);
  ASSERT_NE(nullptr, d);
  EXPECT_EQ("run 2-2", d->getName());
  EXPECT_EQ("This is a second test run of beamtime 2.", d->getDescription());
  EXPECT_EQ(2000, d->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::CALIBRATION, d->getType());
  // this version cannot handle times
  ASSERT_EQ(2, d->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0));
  EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1));
  EXPECT_EQ(4, d->getFileType(0));
  EXPECT_EQ(8, d->getFileType(1));
  EXPECT_EQ(true, d->hasOwnSetup());
  EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile());
  EXPECT_EQ(true, d->hasOwnCalibration());
  EXPECT_EQ(false, d->hasAdditionalCalibration());
  EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0));
  EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1));
  EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2));
}

/*!
 * \brief ParameterIoTest.run_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, run_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/runs_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::RUN);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::RUN);

  auto expected = parameters;
  expected.erase(std::remove_if(expected.begin(), expected.end(),
                                [](auto element) {
                                  return !std::dynamic_pointer_cast<run_parameter>(element);
                                }),
                 expected.end());

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    run_parameter a(*std::dynamic_pointer_cast<run_parameter>(result[i]));
    run_parameter b(*std::dynamic_pointer_cast<run_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the run.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*! \brief ParameterIoTest.run_read_2
 * This test checks whether a previously written run file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, run_read_2)
{
  // Note: the years in type tm are in years since 1900 (so 115 equals 2015).
  auto result = io.readParameter("data/runs_2.data", version_2, ParameterIO::FileType::RUN);
  ASSERT_THAT(result, testing::SizeIs(4));
  auto a = std::dynamic_pointer_cast<run_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("run 1-1", a->getName());
  EXPECT_EQ("This is a test run of beamtime 1.", a->getDescription());
  EXPECT_EQ(run_parameter::RunType::BACKGROUND, a->getType());
  EXPECT_EQ(1000, a->getRunNumber());
  EXPECT_EQ(115, a->getStartTime().tm_year);
  EXPECT_EQ(12, a->getStartTime().tm_mon);
  EXPECT_EQ(1, a->getStartTime().tm_mday);
  EXPECT_EQ(12, a->getStartTime().tm_hour);
  EXPECT_EQ(0, a->getStartTime().tm_min);
  EXPECT_EQ(0, a->getStartTime().tm_sec);
  EXPECT_EQ(115, a->getStopTime().tm_year);
  EXPECT_EQ(12, a->getStopTime().tm_mon);
  EXPECT_EQ(1, a->getStopTime().tm_mday);
  EXPECT_EQ(13, a->getStopTime().tm_hour);
  EXPECT_EQ(0, a->getStopTime().tm_min);
  EXPECT_EQ(0, a->getStopTime().tm_sec);
  ASSERT_EQ(1, a->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_1.data", a->getFile(0));
  EXPECT_EQ(4, a->getFileType(0));
  EXPECT_EQ(false, a->hasOwnSetup());
  EXPECT_EQ(false, a->hasOwnCalibration());
  EXPECT_EQ(false, a->hasAdditionalCalibration());

  auto b = std::dynamic_pointer_cast<run_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("run 1-2", b->getName());
  EXPECT_EQ("This is a second test run of beamtime 1.", b->getDescription());
  EXPECT_EQ(1001, b->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::ELASTIC, b->getType());
  EXPECT_EQ(115, b->getStartTime().tm_year);
  EXPECT_EQ(12, b->getStartTime().tm_mon);
  EXPECT_EQ(1, b->getStartTime().tm_mday);
  EXPECT_EQ(14, b->getStartTime().tm_hour);
  EXPECT_EQ(0, b->getStartTime().tm_min);
  EXPECT_EQ(0, b->getStartTime().tm_sec);
  EXPECT_EQ(115, b->getStopTime().tm_year);
  EXPECT_EQ(12, b->getStopTime().tm_mon);
  EXPECT_EQ(1, b->getStopTime().tm_mday);
  EXPECT_EQ(15, b->getStopTime().tm_hour);
  EXPECT_EQ(0, b->getStopTime().tm_min);
  EXPECT_EQ(0, b->getStopTime().tm_sec);
  ASSERT_EQ(1, b->getNumberOfFiles());
  EXPECT_EQ("data/datafile1_2.data", b->getFile(0));
  EXPECT_EQ(7, b->getFileType(0));
  EXPECT_EQ(false, b->hasOwnSetup());
  EXPECT_EQ(false, b->hasOwnCalibration());
  EXPECT_EQ(false, b->hasAdditionalCalibration());

  auto c = std::dynamic_pointer_cast<run_parameter>(result[2]);
  ASSERT_NE(nullptr, c);
  EXPECT_EQ("run 2-1", c->getName());
  EXPECT_EQ("This is a test run of beamtime 2.", c->getDescription());
  EXPECT_EQ(2000, c->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::REGULAR, c->getType());
  EXPECT_EQ(115, c->getStartTime().tm_year);
  EXPECT_EQ(1, c->getStartTime().tm_mon);
  EXPECT_EQ(1, c->getStartTime().tm_mday);
  EXPECT_EQ(12, c->getStartTime().tm_hour);
  EXPECT_EQ(0, c->getStartTime().tm_min);
  EXPECT_EQ(0, c->getStartTime().tm_sec);
  EXPECT_EQ(115, c->getStopTime().tm_year);
  EXPECT_EQ(1, c->getStopTime().tm_mon);
  EXPECT_EQ(1, c->getStopTime().tm_mday);
  EXPECT_EQ(13, c->getStopTime().tm_hour);
  EXPECT_EQ(0, c->getStopTime().tm_min);
  EXPECT_EQ(0, c->getStopTime().tm_sec);
  ASSERT_EQ(2, c->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0));
  EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1));
  EXPECT_EQ(4, c->getFileType(0));
  EXPECT_EQ(8, c->getFileType(1));
  EXPECT_EQ(false, c->hasOwnSetup());
  EXPECT_EQ(false, c->hasOwnCalibration());
  EXPECT_EQ(true, c->hasAdditionalCalibration());
  EXPECT_EQ(1, c->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration7.data", c->getCalibrationFile(0));

  auto d = std::dynamic_pointer_cast<run_parameter>(result[3]);
  ASSERT_NE(nullptr, d);
  EXPECT_EQ("run 2-2", d->getName());
  EXPECT_EQ("This is a second test run of beamtime 2.", d->getDescription());
  EXPECT_EQ(2000, d->getRunNumber());
  EXPECT_EQ(run_parameter::RunType::CALIBRATION, d->getType());
  EXPECT_EQ(115, d->getStartTime().tm_year);
  EXPECT_EQ(1, d->getStartTime().tm_mon);
  EXPECT_EQ(1, d->getStartTime().tm_mday);
  EXPECT_EQ(12, d->getStartTime().tm_hour);
  EXPECT_EQ(0, d->getStartTime().tm_min);
  EXPECT_EQ(0, d->getStartTime().tm_sec);
  EXPECT_EQ(115, d->getStopTime().tm_year);
  EXPECT_EQ(1, d->getStopTime().tm_mon);
  EXPECT_EQ(1, d->getStopTime().tm_mday);
  EXPECT_EQ(13, d->getStopTime().tm_hour);
  EXPECT_EQ(0, d->getStopTime().tm_min);
  EXPECT_EQ(0, d->getStopTime().tm_sec);
  ASSERT_EQ(2, d->getNumberOfFiles());
  EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0));
  EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1));
  EXPECT_EQ(4, d->getFileType(0));
  EXPECT_EQ(8, d->getFileType(1));
  EXPECT_EQ(true, d->hasOwnSetup());
  EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile());
  EXPECT_EQ(true, d->hasOwnCalibration());
  EXPECT_EQ(true, d->hasAdditionalCalibration());
  EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0));
  EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1));
  EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2));
}

TEST_F(ParameterIoTest, run_write)
{
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);

  std::filesystem::path path1{"data/runs_0.data"};
  io.writeParameter(path1, parameters, version_0, ParameterIO::FileType::RUN);
  std::filesystem::path path2{"data/runs_1.data"};
  io.writeParameter(path2, parameters, version_1, ParameterIO::FileType::RUN);
  std::filesystem::path path3{"data/runs_2.data"};
  io.writeParameter(path3, parameters, version_2, ParameterIO::FileType::RUN);
}
