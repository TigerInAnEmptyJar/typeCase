#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.setup_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, setup_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/setup_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  // 2 materials, 2 shapes, 2 detectors, 1 reaction
  EXPECT_THAT(parameters, testing::SizeIs(7));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::SETUP);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::SETUP);

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
    detector_parameter a(*std::dynamic_pointer_cast<detector_parameter>(result[i]));
    detector_parameter b(*std::dynamic_pointer_cast<detector_parameter>(expected[i]));
    // This IO-version doesn't preserve the uuid of the detector.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = a.getShape();
    auto sh2 = b.getShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    a.setShape(shape_parameter());
    b.setShape(shape_parameter());
    // in this version the circular property is not preserved, as well as the description.
    a.setCircular(b.isCircular());
    a.setDescription(b.getDescription());

    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.setup_read_0
 * This test checks whether a previously written setup file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, DISABLED_setup_read_0)
{
  auto result = io.readParameter("data/setups_0.data", version_0, ParameterIO::FileType::SETUP);
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);

  ASSERT_THAT(result, testing::SizeIs(3));
  auto a = std::dynamic_pointer_cast<detector_parameter>(result[1]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testDetector 1", a->getName());
  EXPECT_EQ(10, a->getID());
  // circular is not conserved!
  EXPECT_EQ(10.5f, a->getMaxDistance());
  ASSERT_EQ(1, a->getMaterial());
  ASSERT_EQ(100, a->getNumberOfElements());
  auto ash = a->getShape();
  EXPECT_THAT(ash, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto b = std::dynamic_pointer_cast<detector_parameter>(result[2]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testDetector 2", b->getName());
  EXPECT_EQ(15, b->getID());
  // circular is not conserved!
  EXPECT_EQ(1.5f, b->getMaxDistance());
  ASSERT_EQ(0, b->getMaterial());
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
 * \brief ParameterIoTest.setup_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, DISABLED_setup_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/setups_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  // 2 materials, 2 shapes, 2 detectors, 1 reaction
  EXPECT_THAT(parameters, testing::SizeIs(7));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::SETUP);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::SETUP);

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
    // This IO-version doesn't preserve the uuid of the detector.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = a.getShape();
    auto sh2 = b.getShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    a.setShape(shape_parameter());
    b.setShape(shape_parameter());
    // in this version the circular property is not preserved, as well as the description.
    a.setCircular(b.isCircular());
    a.setDescription(b.getDescription());

    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.setup_read_1
 * This test checks whether a previously written setup file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, DISABLED_setup_read_1)
{
  auto result = io.readParameter("data/setups_1.data", version_1, ParameterIO::FileType::SETUP);
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);

  ASSERT_THAT(result, testing::SizeIs(3));
  auto a = std::dynamic_pointer_cast<detector_parameter>(result[1]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testDetector 1", a->getName());
  EXPECT_EQ(10, a->getID());
  // circular is not conserved!
  EXPECT_EQ(10.5f, a->getMaxDistance());
  ASSERT_EQ(1, a->getMaterial());
  ASSERT_EQ(100, a->getNumberOfElements());
  auto ash = a->getShape();
  EXPECT_THAT(ash, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto b = std::dynamic_pointer_cast<detector_parameter>(result[2]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testDetector 2", b->getName());
  EXPECT_EQ(15, b->getID());
  // circular is not conserved!
  EXPECT_EQ(1.5f, b->getMaxDistance());
  ASSERT_EQ(0, b->getMaterial());
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
 * \brief ParameterIoTest.setup_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, DISABLED_setup_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/setups_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeDetectors(parameters);
  // 2 materials, 2 shapes, 2 detectors, 1 reaction
  EXPECT_THAT(parameters, testing::SizeIs(7));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::SETUP);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::SETUP);

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
    // This IO-version doesn't preserve the uuid of the detector.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(expected[i]->id());
    // comparing shapes is far from trivial, so we extract the shapes, compare them and remove
    // them from the reaction before comparing the rest.
    auto sh1 = a.getShape();
    auto sh2 = b.getShape();
    EXPECT_THAT(sh1, IsShapeEqual(sh2));
    a.setShape(shape_parameter());
    b.setShape(shape_parameter());
    // in this version the circular property is not preserved, as well as the description.
    a.setCircular(b.isCircular());
    a.setDescription(b.getDescription());

    EXPECT_EQ(a, b);
  }
}

/*! \brief ParameterIoTest.setup_read_2
 * This test checks whether a previously written setup file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, DISABLED_setup_read_2)
{
  auto result = io.readParameter("data/setups_2.data", version_2, ParameterIO::FileType::SETUP);
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeShapes(parameters);

  ASSERT_THAT(result, testing::SizeIs(3));
  auto a = std::dynamic_pointer_cast<detector_parameter>(result[1]);
  ASSERT_NE(nullptr, a);
  EXPECT_EQ("testDetector 1", a->getName());
  EXPECT_EQ(10, a->getID());
  // circular is not conserved!
  EXPECT_EQ(10.5f, a->getMaxDistance());
  ASSERT_EQ(1, a->getMaterial());
  ASSERT_EQ(100, a->getNumberOfElements());
  auto ash = a->getShape();
  EXPECT_THAT(ash, IsShapeEqual(*std::dynamic_pointer_cast<shape_parameter>(parameters[0])));

  auto b = std::dynamic_pointer_cast<detector_parameter>(result[2]);
  ASSERT_NE(nullptr, b);
  EXPECT_EQ("testDetector 2", b->getName());
  EXPECT_EQ(15, b->getID());
  // circular is not conserved!
  EXPECT_EQ(1.5f, b->getMaxDistance());
  ASSERT_EQ(0, b->getMaterial());
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
