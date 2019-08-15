#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.algorithm_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, algorithm_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/algorithms_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeAlgorithms(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::ALGORITHM);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::ALGORITHM);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    algorithm_parameter a(*std::dynamic_pointer_cast<algorithm_parameter>(result[i]));
    algorithm_parameter b(*std::dynamic_pointer_cast<algorithm_parameter>(parameters[i]));
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.algorithm_read_json
 * This test checks whether a previously written algorithm file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, algorithm_read_json)
{
  auto result =
      io.readParameter("data/algorithms_json.data", version_json, ParameterIO::FileType::ALGORITHM);
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
 * \brief ParameterIoTest.beamtime_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, beamtime_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/beamtimes_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::BEAMTIME);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::BEAMTIME);

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
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.beamtime_read_json
 * This test checks whether a previously written beamtime file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, beamtime_read_json)
{
  auto result =
      io.readParameter("data/beamtimes_json.data", version_json, ParameterIO::FileType::BEAMTIME);
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
 * \brief ParameterIoTest.database_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, database_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/databases_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));
  std::for_each(parameters.begin(), parameters.end(), [&tempPath](auto element) {
    auto bt = std::dynamic_pointer_cast<beamTime_parameter>(element);
    if (bt && std::dynamic_pointer_cast<run_parameter>(element) == nullptr) {
      bt->setFileName(std::filesystem::path{tempPath.path().toStdString()} / bt->getFileName());
    }
  });

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::DATABASE);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::DATABASE);

  auto expected = parameters;

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  for (size_t i = 0; i < expected.size(); ++i) {
    auto aa(std::dynamic_pointer_cast<run_parameter>(result[i]));
    auto bb(std::dynamic_pointer_cast<run_parameter>(expected[i]));

    if (!aa && !bb) {
      auto a(*std::dynamic_pointer_cast<beamTime_parameter>(result[i]));
      auto b(*std::dynamic_pointer_cast<beamTime_parameter>(expected[i]));
      EXPECT_EQ(a, b);
    } else if (aa && bb) {
      auto a = *aa;
      auto b = *bb;
      EXPECT_EQ(a, b);
    } else {
      FAIL();
    }
  }
}

/*!
 * \brief ParameterIoTest.database_read_json
 * This test checks whether a previously written database file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, database_read_json)
{
  auto result =
      io.readParameter("data/database_json.data", version_json, ParameterIO::FileType::DATABASE);
  ASSERT_THAT(result, testing::SizeIs(6));
  {
    auto a = std::dynamic_pointer_cast<beamTime_parameter>(result[0]);
    ASSERT_NE(nullptr, a);
    EXPECT_EQ("beamtime 1", a->getName());
    EXPECT_EQ(12, a->getMonth());
    EXPECT_EQ(2015, a->getYear());
    EXPECT_EQ("data/database_run1.json", a->getFileName().native());
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
    EXPECT_EQ("data/database_run2.json", b->getFileName().native());
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
    EXPECT_EQ(true, c->hasAdditionalCalibration());
    EXPECT_EQ(1, c->getNumberOfCalibrationFiles());

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
    EXPECT_EQ(true, d->hasAdditionalCalibration());
    EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
    EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0).native());
    EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1).native());
    EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2).native());
  }
}

/*!
 * \brief ParameterIoTest.detector_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, detector_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/detectors_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  // 2 materials, 2 shapes, 2 detectors, 1 reaction
  EXPECT_THAT(parameters, testing::SizeIs(7));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::DETECTOR);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::DETECTOR);

  std::vector<std::shared_ptr<base_parameter>> expected = parameters;
  auto it = std::remove_if(expected.begin(), expected.end(), [](auto element) {
    return (std::dynamic_pointer_cast<detector_parameter>(element) == nullptr &&
            std::dynamic_pointer_cast<reaction_parameter>(element) == nullptr);
  });
  expected.erase(it, expected.end());
  ASSERT_THAT(expected, testing::SizeIs(3));

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  {
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<reaction_parameter>(expected[0]));
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<reaction_parameter>(result[0]));
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = std::dynamic_pointer_cast<reaction_parameter>(expected[0])->getTargetShape();
    auto sh2 = std::dynamic_pointer_cast<reaction_parameter>(result[0])->getTargetShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    std::dynamic_pointer_cast<reaction_parameter>(expected[0])->setTargetShape(shape_parameter());
    std::dynamic_pointer_cast<reaction_parameter>(result[0])->setTargetShape(shape_parameter());

    EXPECT_EQ(*std::dynamic_pointer_cast<reaction_parameter>(expected[0]),
              *std::dynamic_pointer_cast<reaction_parameter>(result[0]));
  }

  for (size_t i = 1; i < expected.size(); ++i) {
    detector_parameter a(*std::dynamic_pointer_cast<detector_parameter>(result[i]));
    detector_parameter b(*std::dynamic_pointer_cast<detector_parameter>(expected[i]));
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = a.getShape();
    auto sh2 = b.getShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    a.setShape(shape_parameter());
    b.setShape(shape_parameter());

    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.detector_read_json
 * This test checks whether a previously written detector file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, detector_read_json)
{
  auto result =
      io.readParameter("data/detectors_json.data", version_json, ParameterIO::FileType::DETECTOR);
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);

  ASSERT_THAT(result, testing::SizeIs(3));
  auto a = std::dynamic_pointer_cast<detector_parameter>(result[1]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testDetector 1", a->getName());
  EXPECT_EQ(10, a->getID());
  // circular is not conserved!
  EXPECT_EQ(10.5f, a->getMaxDistance());
  ASSERT_FALSE(a->getMaterialId().is_nil());
  ASSERT_EQ(100, a->getNumberOfElements());
  auto ash = a->getShape();
  EXPECT_THAT(ash, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto b = std::dynamic_pointer_cast<detector_parameter>(result[2]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testDetector 2", b->getName());
  EXPECT_EQ(15, b->getID());
  // circular is not conserved!
  EXPECT_EQ(1.5f, b->getMaxDistance());
  ASSERT_FALSE(b->getMaterialId().is_nil());
  ASSERT_EQ(150, b->getNumberOfElements());
  auto bsh = b->getShape();
  EXPECT_THAT(bsh, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[1])));

  auto c = std::dynamic_pointer_cast<reaction_parameter>(result[0]);
  ASSERT_NE(nullptr, c);
  // name is not conserved!
  EXPECT_EQ(false, c->hasTwoBeams());
  EXPECT_EQ(1000.f, c->getBeamMomentum(0));
  EXPECT_EQ(0, c->getTargetMaterial());
  EXPECT_THAT(c->getTargetShape(),
              IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));
}

/*!
 * \brief ParameterIoTest.material_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, material_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/materials_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeMaterials(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::MATERIAL);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::MATERIAL);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    material_parameter a(*std::dynamic_pointer_cast<material_parameter>(result[i]));
    material_parameter b(*std::dynamic_pointer_cast<material_parameter>(parameters[i]));
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.material_read_json
 * This test checks whether a previously written material file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, material_read_json)
{
  auto result =
      io.readParameter("data/materials_json.data", version_json, ParameterIO::FileType::MATERIAL);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<material_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testMaterial 1", a->getName());
  EXPECT_EQ(101.5f, a->RadiationLength());
  EXPECT_EQ(0.75f, a->Speed());
  EXPECT_EQ(true, a->IsActive());
  EXPECT_EQ(13.67f, a->Density());
  ASSERT_EQ(2, a->NumberOfElements());
  ASSERT_EQ(3, a->NumberOfEnergyLossParams());
  EXPECT_EQ(element_parameter{}, a->Element(0));
  EXPECT_EQ(element_parameter{}, a->Element(1));
  EXPECT_EQ(1.2f, a->EnergyLoss(0));
  EXPECT_EQ(2.2f, a->EnergyLoss(1));
  EXPECT_EQ(3.2f, a->EnergyLoss(2));

  auto b = std::dynamic_pointer_cast<material_parameter>(result[1]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testMaterial 2", b->getName());
  EXPECT_EQ(10.5f, b->RadiationLength());
  EXPECT_EQ(0.88f, b->Speed());
  EXPECT_EQ(false, b->IsActive());
  EXPECT_EQ(19.12f, b->Density());
  ASSERT_EQ(1, b->NumberOfElements());
  ASSERT_EQ(2, b->NumberOfEnergyLossParams());
  EXPECT_EQ(element_parameter{}, b->Element(0));
  EXPECT_EQ(19.2f, b->EnergyLoss(0));
  EXPECT_EQ(0.2f, b->EnergyLoss(1));
}

/*!
 * \brief ParameterIoTest.run_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, run_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/runs_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDatabase(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(6));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::RUN);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::RUN);

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
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.run_read_json
 * This test checks whether a previously written run file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, run_read_json)
{
  auto result = io.readParameter("data/runs_json.data", version_json, ParameterIO::FileType::RUN);
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
  EXPECT_EQ(true, c->hasAdditionalCalibration());
  EXPECT_EQ(1, c->getNumberOfCalibrationFiles());

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
  EXPECT_EQ(true, d->hasAdditionalCalibration());
  EXPECT_EQ(3, d->getNumberOfCalibrationFiles());
  EXPECT_EQ("data/calibration8.data", d->getCalibrationFile(0));
  EXPECT_EQ("data/calibration9.data", d->getCalibrationFile(1));
  EXPECT_EQ("data/calibration10.data", d->getCalibrationFile(2));
}

/*!
 * \brief ParameterIoTest.setup_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, setup_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/setup_json.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  // 2 materials, 2 shapes, 2 detectors, 1 reaction
  EXPECT_THAT(parameters, testing::SizeIs(7));

  io.writeParameter(path, parameters, version_json, ParameterIO::FileType::SETUP);
  auto result = io.readParameter(path, version_json, ParameterIO::FileType::SETUP);

  std::vector<std::shared_ptr<base_parameter>> expected = parameters;
  auto it = std::remove_if(expected.begin(), expected.end(), [](auto element) {
    return (std::dynamic_pointer_cast<detector_parameter>(element) == nullptr &&
            std::dynamic_pointer_cast<reaction_parameter>(element) == nullptr &&
            std::dynamic_pointer_cast<material_parameter>(element) == nullptr);
  });
  expected.erase(it, expected.end());
  ASSERT_THAT(expected, testing::SizeIs(5));

  ASSERT_THAT(result, testing::SizeIs(expected.size()));
  {
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<reaction_parameter>(expected[0]));
    ASSERT_NE(nullptr, std::dynamic_pointer_cast<reaction_parameter>(result[0]));
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = std::dynamic_pointer_cast<reaction_parameter>(expected[0])->getTargetShape();
    auto sh2 = std::dynamic_pointer_cast<reaction_parameter>(result[0])->getTargetShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    std::dynamic_pointer_cast<reaction_parameter>(expected[0])->setTargetShape(shape_parameter());
    std::dynamic_pointer_cast<reaction_parameter>(result[0])->setTargetShape(shape_parameter());

    EXPECT_EQ(*std::dynamic_pointer_cast<reaction_parameter>(expected[0]),
              *std::dynamic_pointer_cast<reaction_parameter>(result[0]));
  }

  for (size_t i = 1; i < 3; ++i) {
    detector_parameter a(*std::dynamic_pointer_cast<detector_parameter>(result[i + 2]));
    detector_parameter b(*std::dynamic_pointer_cast<detector_parameter>(expected[i]));
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = a.getShape();
    auto sh2 = b.getShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    a.setShape(shape_parameter());
    b.setShape(shape_parameter());

    EXPECT_EQ(a, b);
  }
  for (size_t i = 3; i < 5; ++i) {
    material_parameter a(*std::dynamic_pointer_cast<material_parameter>(result[i - 2]));
    material_parameter b(*std::dynamic_pointer_cast<material_parameter>(expected[i]));
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.setup_read_json
 * This test checks whether a previously written setup file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, setup_read_json)
{
  auto result =
      io.readParameter("data/setup_json.data", version_json, ParameterIO::FileType::SETUP);
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);

  ASSERT_THAT(result, testing::SizeIs(5));
  auto a = std::dynamic_pointer_cast<detector_parameter>(result[3]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testDetector 1", a->getName());
  EXPECT_EQ(10, a->getID());
  // circular is not conserved!
  EXPECT_EQ(10.5f, a->getMaxDistance());
  ASSERT_EQ(result[2]->id(), a->getMaterialId());
  ASSERT_EQ(100, a->getNumberOfElements());
  auto ash = a->getShape();
  EXPECT_THAT(ash, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto b = std::dynamic_pointer_cast<detector_parameter>(result[4]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testDetector 2", b->getName());
  EXPECT_EQ(15, b->getID());
  // circular is not conserved!
  EXPECT_EQ(1.5f, b->getMaxDistance());
  ASSERT_EQ(result[1]->id(), b->getMaterialId());
  ASSERT_EQ(150, b->getNumberOfElements());
  auto bsh = b->getShape();
  EXPECT_THAT(bsh, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[1])));

  auto c = std::dynamic_pointer_cast<reaction_parameter>(result[0]);
  ASSERT_NE(nullptr, c);
  // name is not conserved!
  EXPECT_EQ(false, c->hasTwoBeams());
  EXPECT_EQ(1000.f, c->getBeamMomentum(0));
  EXPECT_EQ(0, c->getTargetMaterial());
  EXPECT_THAT(c->getTargetShape(),
              IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto d = std::dynamic_pointer_cast<material_parameter>(result[1]);
  ASSERT_NE(nullptr, d);
  EXPECT_EQ("testMaterial 1", d->getName());
  EXPECT_EQ(101.5f, d->RadiationLength());
  EXPECT_EQ(0.75f, d->Speed());
  EXPECT_EQ(true, d->IsActive());
  EXPECT_EQ(13.67f, d->Density());
  ASSERT_EQ(2, d->NumberOfElements());
  ASSERT_EQ(3, d->NumberOfEnergyLossParams());
  EXPECT_EQ(element_parameter{}, d->Element(0));
  EXPECT_EQ(element_parameter{}, d->Element(1));
  EXPECT_EQ(1.2f, d->EnergyLoss(0));
  EXPECT_EQ(2.2f, d->EnergyLoss(1));
  EXPECT_EQ(3.2f, d->EnergyLoss(2));

  auto e = std::dynamic_pointer_cast<material_parameter>(result[2]);
  ASSERT_NE(nullptr, e);
  EXPECT_EQ("testMaterial 2", e->getName());
  EXPECT_EQ(10.5f, e->RadiationLength());
  EXPECT_EQ(0.88f, e->Speed());
  EXPECT_EQ(false, e->IsActive());
  EXPECT_EQ(19.12f, e->Density());
  ASSERT_EQ(1, e->NumberOfElements());
  ASSERT_EQ(2, e->NumberOfEnergyLossParams());
  EXPECT_EQ(element_parameter{}, e->Element(0));
  EXPECT_EQ(19.2f, e->EnergyLoss(0));
  EXPECT_EQ(0.2f, e->EnergyLoss(1));
}

/*!
 * \brief ParameterIoTest.shape_write_read_json
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, shape_write_read_json)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/shapes_json.data"};
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
 * \brief ParameterIoTest.shape_read_json
 * This test checks whether a previously written shape file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, shape_read_json)
{
  auto result =
      io.readParameter("data/shapes_json.data", version_json, ParameterIO::FileType::SHAPE);
  ASSERT_THAT(result, testing::SizeIs(2));
  auto a = std::dynamic_pointer_cast<shape_parameter>(result[0]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ(true, a->completeWrite());
  EXPECT_EQ("testShape 1", a->getName());
  ASSERT_EQ(7, a->numberOfValues());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(0).valueType());
  ASSERT_EQ(ParameterValue::ValueType::INT, a->value(1).valueType());
  ASSERT_EQ(ParameterValue::ValueType::FLOAT, a->value(2).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(3).valueType());
  ASSERT_EQ(ParameterValue::ValueType::POINT3D, a->value(4).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(5).valueType());
  ASSERT_EQ(ParameterValue::ValueType::VECTOR3D, a->value(6).valueType());
  EXPECT_EQ("param1", a->valueName(0));
  EXPECT_EQ("param2", a->valueName(1));
  EXPECT_EQ("param3", a->valueName(2));
  EXPECT_EQ("param4", a->valueName(3));
  EXPECT_EQ("param5", a->valueName(4));
  EXPECT_EQ("param6", a->valueName(5));
  EXPECT_EQ("param7", a->valueName(6));
  EXPECT_EQ(10, a->value<int>(0));
  EXPECT_EQ(26, a->value<int>(1));
  EXPECT_EQ(123.5f, a->value<float>(2));
  EXPECT_EQ(point3D(1, 2, 3), a->value<point3D>(3));
  EXPECT_EQ(point3D(4, 5, 6), a->value<point3D>(4));
  EXPECT_GE(0.0001, (vector3D(1, 3, 5) - a->value<vector3D>(5)).length());
  EXPECT_GE(0.0001, (vector3D(11, 22, 33) - a->value<vector3D>(6)).length());

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
  EXPECT_EQ(10, b->value<int>(0));
  EXPECT_EQ(26.25f, b->value<float>(1));
  EXPECT_EQ(123.5f, b->value<float>(2));
  EXPECT_EQ("foo", b->value<std::string>(3));
  EXPECT_EQ("bar", b->value<std::string>(4));
  EXPECT_EQ("param-1", b->valueName(0));
  EXPECT_EQ("param-2", b->valueName(1));
  EXPECT_EQ("param-3", b->valueName(2));
  EXPECT_EQ("param-4", b->valueName(3));
  EXPECT_EQ("param-5", b->valueName(4));
}

TEST_F(ParameterIoTest, DISABLED_write_all_json)
{
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  appendSomeAlgorithms(parameters);
  appendSomeDatabase(parameters);
  //  ALGORITHM,
  {
    std::filesystem::path path1{"data/algorithms_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::ALGORITHM);
  }
  //  SHAPE,
  {
    std::filesystem::path path1{"data/shapes_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::SHAPE);
  }
  //  PROCESS,
  {
    std::filesystem::path path1{"data/process_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::PROCESS);
  }
  //  BEAMTIME,
  {
    std::filesystem::path path1{"data/beamtimes_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::BEAMTIME);
  }
  //  RUN,
  {
    std::filesystem::path path1{"data/runs_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::RUN);
  }
  //  DATABASE,
  {
    std::filesystem::path path1{"data/database_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::DATABASE);
  }
  //  MATERIAL,
  {
    std::filesystem::path path1{"data/materials_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::MATERIAL);
  }
  //  DETECTOR,
  {
    std::filesystem::path path1{"data/detectors_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::DETECTOR);
  }
  //  SETUP,
  {
    std::filesystem::path path1{"data/setup_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::SETUP);
  }
  //  ALL,
  {
    std::filesystem::path path1{"data/all_json.data"};
    io.writeParameter(path1, parameters, version_json, ParameterIO::FileType::ALL);
  }
}
