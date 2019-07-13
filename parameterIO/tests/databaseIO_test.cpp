#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.database_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, database_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/databases_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));
  std::for_each(parameters.begin(), parameters.end(), [&tempPath](auto element) {
    auto bt = std::dynamic_pointer_cast<beamTime_parameter>(element);
    if (bt && std::dynamic_pointer_cast<run_parameter>(element) == nullptr) {
      bt->setFileName(std::filesystem::path{tempPath.path().toStdString()} / bt->getFileName());
    }
  });

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::DATABASE);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::DATABASE);

  auto expected = parameters;

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    auto aa(std::dynamic_pointer_cast<run_parameter>(result[i]));
    auto bb(std::dynamic_pointer_cast<run_parameter>(expected[i]));

    if (!aa && !bb) {
      auto a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
      auto b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else if (aa && bb) {
      auto a = *aa;
      auto b = *bb;
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else {
      FAIL();
    }
  }
}

/*!
 * \brief ParameterIoTest.database_read_0
 * This test checks whether a previously written database file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, database_read_0)
{
  auto result =
      io.readParameter("data/databases_0.data", version_0, ParameterIO::FileType::DATABASE);
  ASSERT_THAT(result, testing::SizeIs(6));
  {
    auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("beamtime 1", a->getName());
    EXPECT_EQ(12, a->getMonth());
    EXPECT_EQ(2015, a->getYear());
    EXPECT_EQ("data/database_runs1_0.data", a->getFileName().native());
    EXPECT_EQ("data/setup1.data", a->getSetupFile().native());
    ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3).native());

    auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("beamtime 2", b->getName());
    EXPECT_EQ(1, b->getMonth());
    EXPECT_EQ(2015, b->getYear());
    EXPECT_EQ("data/database_runs2_0.data", b->getFileName().native());
    EXPECT_EQ("data/setup2.data", b->getSetupFile().native());
    ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3).native());
  }
  {
    auto a = std::dynamic_pointer_cast<run_parameter>(result[2]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("run 1-1", a->getName());
    EXPECT_EQ("This is a test run of beamtime 1.", a->getDescription());
    EXPECT_EQ(run_parameter::RunType::BACKGROUND, a->getType());
    EXPECT_EQ(1000, a->getRunNumber());
    // this version cannot handle times
    ASSERT_EQ(1, a->getNumberOfFiles());
    EXPECT_EQ("data/datafile1_1.data", a->getFile(0).native());
    EXPECT_EQ(4, a->getFileType(0));
    EXPECT_EQ(false, a->hasOwnSetup());
    EXPECT_EQ(false, a->hasOwnCalibration());
    EXPECT_EQ(false, a->hasAdditionalCalibration());

    auto b = std::dynamic_pointer_cast<run_parameter>(result[3]);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("run 1-2", b->getName());
    EXPECT_EQ("This is a second test run of beamtime 1.", b->getDescription());
    EXPECT_EQ(1001, b->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::ELASTIC, b->getType());
    // this version cannot handle times
    ASSERT_EQ(1, b->getNumberOfFiles());
    EXPECT_EQ("data/datafile1_2.data", b->getFile(0).native());
    EXPECT_EQ(7, b->getFileType(0));
    EXPECT_EQ(false, b->hasOwnSetup());
    EXPECT_EQ(false, b->hasOwnCalibration());
    EXPECT_EQ(false, b->hasAdditionalCalibration());

    auto c = std::dynamic_pointer_cast<run_parameter>(result[4]);
    ASSERT_NE(nullptr, c);
    EXPECT_EQ("run 2-1", c->getName());
    EXPECT_EQ("This is a test run of beamtime 2.", c->getDescription());
    EXPECT_EQ(2000, c->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::REGULAR, c->getType());
    // this version cannot handle times
    ASSERT_EQ(2, c->getNumberOfFiles());
    EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0).native());
    EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1).native());
    EXPECT_EQ(4, c->getFileType(0));
    EXPECT_EQ(8, c->getFileType(1));
    EXPECT_EQ(false, c->hasOwnSetup());
    EXPECT_EQ(false, c->hasOwnCalibration());
    // this version cannot handle additional calibration
    EXPECT_EQ(false, c->hasAdditionalCalibration());
    EXPECT_EQ(0, c->getNumberOfCalibrationFiles());

    auto d = std::dynamic_pointer_cast<run_parameter>(result[5]);
    ASSERT_NE(nullptr, d);
    EXPECT_EQ("run 2-2", d->getName());
    EXPECT_EQ("This is a second test run of beamtime 2.", d->getDescription());
    EXPECT_EQ(2000, d->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::CALIBRATION, d->getType());
    // this version cannot handle times
    ASSERT_EQ(2, d->getNumberOfFiles());
    EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0).native());
    EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1).native());
    EXPECT_EQ(4, d->getFileType(0));
    EXPECT_EQ(8, d->getFileType(1));
    EXPECT_EQ(true, d->hasOwnSetup());
    EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile().native());
    EXPECT_EQ(true, d->hasOwnCalibration());
    EXPECT_EQ(false, d->hasAdditionalCalibration());
    EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2).native());
  }
}

/*!
 * \brief ParameterIoTest.database_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, database_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/databases_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));
  std::for_each(parameters.begin(), parameters.end(), [&tempPath](auto element) {
    auto bt = std::dynamic_pointer_cast<beamTime_parameter>(element);
    if (bt && std::dynamic_pointer_cast<run_parameter>(element) == nullptr) {
      bt->setFileName(std::filesystem::path{tempPath.path().toStdString()} / bt->getFileName());
    }
  });

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::DATABASE);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::DATABASE);

  auto expected = parameters;

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    auto aa(std::dynamic_pointer_cast<run_parameter>(result[i]));
    auto bb(std::dynamic_pointer_cast<run_parameter>(expected[i]));

    if (!aa && !bb) {
      auto a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
      auto b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else if (aa && bb) {
      auto a = *aa;
      auto b = *bb;
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else {
      FAIL();
    }
  }
}

/*!
 * \brief ParameterIoTest.database_read_1
 * This test checks whether a previously written database file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, database_read_1)
{
  auto result =
      io.readParameter("data/databases_1.data", version_1, ParameterIO::FileType::DATABASE);
  ASSERT_THAT(result, testing::SizeIs(6));
  {
    auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("beamtime 1", a->getName());
    EXPECT_EQ(12, a->getMonth());
    EXPECT_EQ(2015, a->getYear());
    EXPECT_EQ("data/database_runs1_1.data", a->getFileName().native());
    EXPECT_EQ("data/setup1.data", a->getSetupFile().native());
    ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3).native());

    auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("beamtime 2", b->getName());
    EXPECT_EQ(1, b->getMonth());
    EXPECT_EQ(2015, b->getYear());
    EXPECT_EQ("data/database_runs2_1.data", b->getFileName().native());
    EXPECT_EQ("data/setup2.data", b->getSetupFile().native());
    ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3).native());
  }
  {
    auto a = std::dynamic_pointer_cast<run_parameter>(result[2]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("run 1-1", a->getName());
    EXPECT_EQ("This is a test run of beamtime 1.", a->getDescription());
    EXPECT_EQ(run_parameter::RunType::BACKGROUND, a->getType());
    EXPECT_EQ(1000, a->getRunNumber());
    // this version cannot handle times
    ASSERT_EQ(1, a->getNumberOfFiles());
    EXPECT_EQ("data/datafile1_1.data", a->getFile(0).native());
    EXPECT_EQ(4, a->getFileType(0));
    EXPECT_EQ(false, a->hasOwnSetup());
    EXPECT_EQ(false, a->hasOwnCalibration());
    EXPECT_EQ(false, a->hasAdditionalCalibration());

    auto b = std::dynamic_pointer_cast<run_parameter>(result[3]);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("run 1-2", b->getName());
    EXPECT_EQ("This is a second test run of beamtime 1.", b->getDescription());
    EXPECT_EQ(1001, b->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::ELASTIC, b->getType());
    // this version cannot handle times
    ASSERT_EQ(1, b->getNumberOfFiles());
    EXPECT_EQ("data/datafile1_2.data", b->getFile(0).native());
    EXPECT_EQ(7, b->getFileType(0));
    EXPECT_EQ(false, b->hasOwnSetup());
    EXPECT_EQ(false, b->hasOwnCalibration());
    EXPECT_EQ(false, b->hasAdditionalCalibration());

    auto c = std::dynamic_pointer_cast<run_parameter>(result[4]);
    ASSERT_NE(nullptr, c);
    EXPECT_EQ("run 2-1", c->getName());
    EXPECT_EQ("This is a test run of beamtime 2.", c->getDescription());
    EXPECT_EQ(2000, c->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::REGULAR, c->getType());
    // this version cannot handle times
    ASSERT_EQ(2, c->getNumberOfFiles());
    EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0).native());
    EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1).native());
    EXPECT_EQ(4, c->getFileType(0));
    EXPECT_EQ(8, c->getFileType(1));
    EXPECT_EQ(false, c->hasOwnSetup());
    EXPECT_EQ(false, c->hasOwnCalibration());
    // this version cannot handle additional calibration
    EXPECT_EQ(false, c->hasAdditionalCalibration());
    EXPECT_EQ(0, c->getNumberOfCalibrationFiles());

    auto d = std::dynamic_pointer_cast<run_parameter>(result[5]);
    ASSERT_NE(nullptr, d);
    EXPECT_EQ("run 2-2", d->getName());
    EXPECT_EQ("This is a second test run of beamtime 2.", d->getDescription());
    EXPECT_EQ(2000, d->getRunNumber());
    EXPECT_EQ(run_parameter::RunType::CALIBRATION, d->getType());
    // this version cannot handle times
    ASSERT_EQ(2, d->getNumberOfFiles());
    EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0).native());
    EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1).native());
    EXPECT_EQ(4, d->getFileType(0));
    EXPECT_EQ(8, d->getFileType(1));
    EXPECT_EQ(true, d->hasOwnSetup());
    EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile().native());
    EXPECT_EQ(true, d->hasOwnCalibration());
    EXPECT_EQ(false, d->hasAdditionalCalibration());
    EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2).native());
  }
}

/*!
 * \brief ParameterIoTest.database_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, database_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/databases_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));
  std::for_each(parameters.begin(), parameters.end(), [&tempPath](auto element) {
    auto bt = std::dynamic_pointer_cast<beamTime_parameter>(element);
    if (bt && std::dynamic_pointer_cast<run_parameter>(element) == nullptr) {
      bt->setFileName(std::filesystem::path{tempPath.path().toStdString()} / bt->getFileName());
    }
  });

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::DATABASE);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::DATABASE);

  auto expected = parameters;

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    auto aa(std::dynamic_pointer_cast<run_parameter>(result[i]));
    auto bb(std::dynamic_pointer_cast<run_parameter>(expected[i]));

    if (!aa && !bb) {
      auto a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
      auto b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else if (aa && bb) {
      auto a = *aa;
      auto b = *bb;
      // This IO-version doesn't preserve the uuid of the run.
      EXPECT_TRUE(a.id().is_nil());
      a.setId(expected[i]->id());
      EXPECT_EQ(a, b);
    } else {
      FAIL();
    }
  }
}

/*! \brief ParameterIoTest.database_read_2
 * This test checks whether a previously written database file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, database_read_2)
{
  // Note: the years in type tm are in years since 1900 (so 115 equals 2015).
  auto result =
      io.readParameter("data/databases_2.data", version_2, ParameterIO::FileType::DATABASE);
  ASSERT_THAT(result, testing::SizeIs(6));
  {
    auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("beamtime 1", a->getName());
    EXPECT_EQ(12, a->getMonth());
    EXPECT_EQ(2015, a->getYear());
    EXPECT_EQ("data/database_runs1_2.data", a->getFileName().native());
    EXPECT_EQ("data/setup1.data", a->getSetupFile().native());
    ASSERT_EQ(4, a->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration1.data", a->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration2.data", a->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration3.data", a->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration4.data", a->getCalibrationFile(3).native());

    auto b = std::dynamic_pointer_cast<beamTime_parameter>(result[1]);
    ASSERT_NE(nullptr, b);
    EXPECT_EQ("beamtime 2", b->getName());
    EXPECT_EQ(1, b->getMonth());
    EXPECT_EQ(2015, b->getYear());
    EXPECT_EQ("data/database_runs2_2.data", b->getFileName().native());
    EXPECT_EQ("data/setup2.data", b->getSetupFile().native());
    ASSERT_EQ(4, b->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration5.data", b->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration6.data", b->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration7.data", b->getCalibrationFile(2).native());
    EXPECT_EQ("data/calibration8.data", b->getCalibrationFile(3).native());
  }
  {
    auto a = std::dynamic_pointer_cast<run_parameter>(result[2]);
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
    EXPECT_EQ("data/datafile1_1.data", a->getFile(0).native());
    EXPECT_EQ(4, a->getFileType(0));
    EXPECT_EQ(false, a->hasOwnSetup());
    EXPECT_EQ(false, a->hasOwnCalibration());
    EXPECT_EQ(false, a->hasAdditionalCalibration());

    auto b = std::dynamic_pointer_cast<run_parameter>(result[3]);
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
    EXPECT_EQ("data/datafile1_2.data", b->getFile(0).native());
    EXPECT_EQ(7, b->getFileType(0));
    EXPECT_EQ(false, b->hasOwnSetup());
    EXPECT_EQ(false, b->hasOwnCalibration());
    EXPECT_EQ(false, b->hasAdditionalCalibration());

    auto c = std::dynamic_pointer_cast<run_parameter>(result[4]);
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
    EXPECT_EQ("data/datafile2_1-1.data", c->getFile(0).native());
    EXPECT_EQ("data/datafile2_1-2.data", c->getFile(1).native());
    EXPECT_EQ(4, c->getFileType(0));
    EXPECT_EQ(8, c->getFileType(1));
    EXPECT_EQ(false, c->hasOwnSetup());
    EXPECT_EQ(false, c->hasOwnCalibration());
    EXPECT_EQ(true, c->hasAdditionalCalibration());
    EXPECT_EQ(1, c->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration7.data", c->getCalibrationFile(0).native());

    auto d = std::dynamic_pointer_cast<run_parameter>(result[5]);
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
    EXPECT_EQ("data/datafile2_2-1.data", d->getFile(0).native());
    EXPECT_EQ("data/datafile2_2-2.data", d->getFile(1).native());
    EXPECT_EQ(4, d->getFileType(0));
    EXPECT_EQ(8, d->getFileType(1));
    EXPECT_EQ(true, d->hasOwnSetup());
    EXPECT_EQ("data/ownSetup2_2.data", d->getSetupFile().native());
    EXPECT_EQ(true, d->hasOwnCalibration());
    EXPECT_EQ(true, d->hasAdditionalCalibration());
    EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2).native());
  }
}

TEST_F(ParameterIoTest, database_write)
{
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);

  auto beamtime1 = std::dynamic_pointer_cast<beamTime_parameter>(parameters[0]);
  auto beamtime2 = std::dynamic_pointer_cast<beamTime_parameter>(parameters[1]);

  beamtime1->setFileName("data/database_runs1_0.data");
  beamtime2->setFileName("data/database_runs2_0.data");
  std::filesystem::path path1{"data/databases_0.data"};
  io.writeParameter(path1, parameters, version_0, ParameterIO::FileType::DATABASE);
  beamtime1->setFileName("data/database_runs1_1.data");
  beamtime2->setFileName("data/database_runs2_1.data");
  std::filesystem::path path2{"data/databases_1.data"};
  io.writeParameter(path2, parameters, version_1, ParameterIO::FileType::DATABASE);
  beamtime1->setFileName("data/database_runs1_2.data");
  beamtime2->setFileName("data/database_runs2_2.data");
  std::filesystem::path path3{"data/databases_2.data"};
  io.writeParameter(path3, parameters, version_2, ParameterIO::FileType::DATABASE);
}
