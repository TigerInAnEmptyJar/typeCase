#include "fixture.h"
#include <boost/uuid/random_generator.hpp>

void ParameterIoTest::appendSomeAlgorithms(std::vector<std::shared_ptr<base_parameter>>& items)
{
  using namespace std::string_literals;
  algorithm_parameter algo1;
  algo1.setID(10);
  algo1.setUse(true);
  algo1.setLevel(0);
  algo1.setCategory(algorithm_parameter::Category::OUTPUT);
  algo1.setId(_algorithm1_id);
  algo1.setName("testAlgorithm 1"s);
  algo1.setDescription("This is a test algorithm."s);
  algo1.addValue("param1"s, static_cast<int>(0));
  algo1.addValue("param2"s, static_cast<int>(10));
  algo1.addValue("param3"s, 5.5f);
  algo1.addValue("param4"s, "foo"s);
  algo1.addValue("param5"s, std::vector<int>{0, 1, 2, 3, 4, 5, 6});
  algo1.addValue("param6"s, std::vector<float>{0.7, 1.45, 5.7});
  items.push_back(std::make_shared<algorithm_parameter>(algo1));

  algorithm_parameter algo2;
  algo2.setID(34);
  algo2.setUse(false);
  algo2.setLevel(5);
  algo2.setCategory(algorithm_parameter::Category::CALIBRATION);
  algo2.setId(_algorithm2_id);
  algo2.setName("testAlgorithm 2"s);
  algo2.setDescription("This is a new test algorithm."s);
  algo2.addValue("param-1"s, false);
  algo2.addValue("param-2"s, true);
  algo2.addValue("param-3"s, point3D(1, 2, 3));
  algo2.addValue("param-4"s, vector3D(4, 5, 6));
  items.push_back(std::make_shared<algorithm_parameter>(algo2));
}

void ParameterIoTest::appendSomeShapes(std::vector<std::shared_ptr<base_parameter>>& items)
{
  using namespace std::string_literals;
  shape_parameter shape1;
  shape1.setId(_shape1_id);
  shape1.setName("testShape 1");
  shape1.setDescription("This is a test shape.");
  shape1.setCompleteWrite(true);
  shape1.addValue("param4", point3D(1, 2, 3));
  shape1.addValue("param5", point3D(4, 5, 6));
  shape1.addValue("param6", vector3D(1, 3, 5));
  shape1.addValue("param7", vector3D(11, 22, 33));
  shape1.addValue("param1", static_cast<int>(10));
  shape1.addValue("param2", static_cast<int>(26));
  shape1.addValue("param3", 123.5f);
  items.push_back(std::make_shared<shape_parameter>(shape1));

  shape_parameter shape2;
  shape2.setId(_shape2_id);
  shape2.setName("testShape 2");
  shape2.setDescription("This is yet another test shape.");
  shape2.setCompleteWrite(true);
  shape2.addValue("param-1", static_cast<int>(10));
  shape2.addValue("param-2", 26.25f);
  shape2.addValue("param-3", 123.5f);
  shape2.addValue("param-4", "foo"s);
  shape2.addValue("param-5", "bar"s);
  items.push_back(std::make_shared<shape_parameter>(shape2));
}

void ParameterIoTest::appendSomeMaterials(std::vector<std::shared_ptr<base_parameter>>& items)
{
  material_parameter material1;
  material1.setId(boost::uuids::random_generator()());
  material1.setName("testMaterial 1");
  material1.setDescription("This is a test material.");
  material1.setRadiationLength(101.5);
  material1.setSpeed(0.75);
  material1.setActive(true);
  material1.setDensity(13.67f);
  material1.addElement({});
  material1.addElement({});
  material1.setEnergyLoss(0, 1.2);
  material1.setEnergyLoss(1, 2.2);
  material1.setEnergyLoss(2, 3.2);
  items.push_back(std::make_shared<material_parameter>(material1));

  material_parameter material2;
  material2.setId(boost::uuids::random_generator()());
  material2.setName("testMaterial 2");
  material2.setDescription("This is yet another test material.");
  material2.setRadiationLength(10.5);
  material2.setSpeed(0.88);
  material2.setActive(false);
  material2.setDensity(19.12f);
  material2.addElement({});
  material2.setEnergyLoss(0, 19.2);
  material2.setEnergyLoss(1, 0.2);
  items.push_back(std::make_shared<material_parameter>(material2));
}

void ParameterIoTest::appendSomeDetectors(std::vector<std::shared_ptr<base_parameter>>& items)
{
  appendSomeShapes(items);
  const auto mat_start = items.size();
  appendSomeMaterials(items);
  auto mat1 = items[mat_start];
  auto mat2 = items[mat_start + 1];

  // when writing setup, not only the detector is important, but also the reaction description.
  reaction_parameter reaction;
  reaction.setName("Reaction description");
  reaction.setId(boost::uuids::random_generator()());
  reaction.setDescription("This is a reaction description.");
  reaction.setTwoBeams(false);
  reaction.setBeamMomentum(1000.f);
  reaction.setTargetMaterial(0);
  reaction.setMaterialId(0, items[mat_start]->id());
  reaction.setTargetShape(*std::dynamic_pointer_cast<shape_parameter>(items[0]));
  items.insert(items.begin() + 2, std::make_shared<reaction_parameter>(reaction));

  detector_parameter detector1;
  detector1.setName("testDetector 1");
  detector1.setDescription("This is a test detector.");
  detector1.setId(boost::uuids::random_generator()());
  detector1.setID(10);
  detector1.setCircular(true);
  detector1.setMaterial(1);
  detector1.setMaterial(std::dynamic_pointer_cast<material_parameter>(mat2).get());
  detector1.setStackType(5);
  detector1.setMaxDistance(10.5f);
  detector1.setNumberOfElements(100);
  detector1.setShape(*std::dynamic_pointer_cast<shape_parameter>(items[0]));
  items.insert(items.begin() + 3, std::make_shared<detector_parameter>(detector1));

  detector_parameter detector2;
  detector2.setName("testDetector 2");
  detector2.setDescription("This is a second test detector.");
  detector2.setId(boost::uuids::random_generator()());
  detector2.setID(15);
  detector2.setCircular(false);
  detector2.setMaterial(0);
  detector2.setMaterial(std::dynamic_pointer_cast<material_parameter>(mat1).get());
  detector2.setStackType(1);
  detector2.setMaxDistance(1.5f);
  detector2.setNumberOfElements(150);
  detector2.setShape(*std::dynamic_pointer_cast<shape_parameter>(items[1]));
  items.insert(items.begin() + 4, std::make_shared<detector_parameter>(detector2));
}

void ParameterIoTest::appendSomeDatabase(std::vector<std::shared_ptr<base_parameter>>& items)
{
  beamTime_parameter beamtime1;
  beamtime1.setName("beamtime 1");
  beamtime1.setDescription("This is a test beamtime.");
  beamtime1.setId(boost::uuids::random_generator()());
  beamtime1.setData(12, 2015);
  beamtime1.setFileName("data/run1.data");
  beamtime1.setSetupFile("data/setup1.data");
  beamtime1.addCalibrationFile("data/calibration1.data");
  beamtime1.addCalibrationFile("data/calibration2.data");
  beamtime1.addCalibrationFile("data/calibration3.data");
  beamtime1.addCalibrationFile("data/calibration4.data");
  items.push_back(std::make_shared<beamTime_parameter>(beamtime1));
  auto bt1 = items.back();

  beamTime_parameter beamtime2;
  beamtime2.setName("beamtime 2");
  beamtime2.setDescription("This is another test beamtime.");
  beamtime2.setId(boost::uuids::random_generator()());
  beamtime2.setData(1, 2015);
  beamtime2.setFileName("data/run2.data");
  beamtime2.setSetupFile("data/setup2.data");
  beamtime2.addCalibrationFile("data/calibration5.data");
  beamtime2.addCalibrationFile("data/calibration6.data");
  beamtime2.addCalibrationFile("data/calibration7.data");
  beamtime2.addCalibrationFile("data/calibration8.data");
  items.push_back(std::make_shared<beamTime_parameter>(beamtime2));
  auto bt2 = items.back();

  run_parameter run1_1;
  run1_1.setName("run 1-1");
  run1_1.setDescription("This is a test run of beamtime 1.");
  run1_1.setId(boost::uuids::random_generator()());
  run1_1.setType(run_parameter::RunType::BACKGROUND);
  run1_1.setParent(std::dynamic_pointer_cast<beamTime_parameter>(bt1));
  run1_1.setParentNumber(bt1->id());
  run1_1.setRunNumber(1000);
  run1_1.setStartTime(2015, 12, 1, 12, 0, 0);
  run1_1.setStopTime(2015, 12, 1, 13, 0, 0);
  run1_1.setOwnCalibration(false);
  run1_1.setAdditionalCalibration(false);
  run1_1.addFile("data/datafile1_1.data", 4);
  items.push_back(std::make_shared<run_parameter>(run1_1));

  run_parameter run1_2;
  run1_2.setName("run 1-2");
  run1_2.setDescription("This is a second test run of beamtime 1.");
  run1_2.setId(boost::uuids::random_generator()());
  run1_2.setType(run_parameter::RunType::ELASTIC);
  run1_2.setParent(std::dynamic_pointer_cast<beamTime_parameter>(bt1));
  run1_2.setParentNumber(bt1->id());
  run1_2.setRunNumber(1001);
  run1_2.setStartTime(2015, 12, 1, 14, 0, 0);
  run1_2.setStopTime(2015, 12, 1, 15, 0, 0);
  run1_2.setOwnCalibration(false);
  run1_2.setAdditionalCalibration(false);
  run1_2.addFile("data/datafile1_2.data", 7);
  items.push_back(std::make_shared<run_parameter>(run1_2));

  run_parameter run2_1;
  run2_1.setName("run 2-1");
  run2_1.setDescription("This is a test run of beamtime 2.");
  run2_1.setId(boost::uuids::random_generator()());
  run2_1.setType(run_parameter::RunType::REGULAR);
  run2_1.setParent(std::dynamic_pointer_cast<beamTime_parameter>(bt2));
  run2_1.setParentNumber(bt2->id());
  run2_1.setRunNumber(2000);
  run2_1.setStartTime(2015, 1, 1, 12, 0, 0);
  run2_1.setStopTime(2015, 1, 1, 13, 0, 0);
  run2_1.setOwnCalibration(false);
  run2_1.setAdditionalCalibration(true);
  run2_1.addCalibrationFile("data/calibration7.data");
  run2_1.addFile("data/datafile2_1-1.data", 4);
  run2_1.addFile("data/datafile2_1-2.data", 8);
  items.push_back(std::make_shared<run_parameter>(run2_1));

  run_parameter run2_2;
  run2_2.setName("run 2-2");
  run2_2.setDescription("This is a second test run of beamtime 2.");
  run2_2.setId(boost::uuids::random_generator()());
  run2_2.setType(run_parameter::RunType::CALIBRATION);
  run2_2.setParent(std::dynamic_pointer_cast<beamTime_parameter>(bt2));
  run2_2.setParentNumber(bt2->id());
  run2_2.setRunNumber(2000);
  run2_2.setStartTime(2015, 1, 1, 12, 0, 0);
  run2_2.setStopTime(2015, 1, 1, 13, 0, 0);
  run2_2.setOwnCalibration(true);
  run2_2.setAdditionalCalibration(true);
  run2_2.addCalibrationFile("data/calibration8.data");
  run2_2.addCalibrationFile("data/calibration9.data");
  run2_2.addCalibrationFile("data/calibration10.data");
  run2_2.addFile("data/datafile2_2-1.data", 4);
  run2_2.addFile("data/datafile2_2-2.data", 8);
  run2_2.setOwnSetup(true);
  run2_2.setSetupFile("data/ownSetup2_2.data");
  items.push_back(std::make_shared<run_parameter>(run2_2));
}

void ParameterIoTest::installRecovery()
{
  _shape1_id = boost::uuids::random_generator()();
  _shape2_id = boost::uuids::random_generator()();
  _algorithm1_id = boost::uuids::random_generator()();
  _algorithm2_id = boost::uuids::random_generator()();

  io.setShapeIdRecoveryFunction([this](std::shared_ptr<base_parameter>& item) {
    if (item->getName() == "testShape 1") {
      item->setId(_shape1_id);
    } else if (item->getName() == "testShape 2") {
      item->setId(_shape2_id);
    }
  });
  io.setAlgorithmIdRecoveryFunction([this](std::shared_ptr<base_parameter>& item) {
    if (item->getName() == "testAlgorithm 1") {
      item->setId(_algorithm1_id);
    } else if (item->getName() == "testAlgorithm 2") {
      item->setId(_algorithm2_id);
    }
  });
}
