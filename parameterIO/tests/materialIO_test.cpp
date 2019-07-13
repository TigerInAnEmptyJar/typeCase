#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fixture.h"
#include "parameterio.h"
#include "versions.h"

#include <QTemporaryDir>

using namespace parameter::IO;

/*!
 * \brief ParameterIoTest.material_write_read_0
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, material_write_read_0)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/materials_0.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeMaterials(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_0, ParameterIO::FileType::MATERIAL);
  auto result = io.readParameter(path, version_0, ParameterIO::FileType::MATERIAL);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    material_parameter a(*std::dynamic_pointer_cast<material_parameter>(result[i]));
    material_parameter b(*std::dynamic_pointer_cast<material_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the material.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.material_read_0
 * This test checks whether a previously written material file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, material_read_0)
{
  auto result =
      io.readParameter("data/materials_0.data", version_0, ParameterIO::FileType::MATERIAL);
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
 * \brief ParameterIoTest.material_write_read_1
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, material_write_read_1)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/materials_1.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeMaterials(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_1, ParameterIO::FileType::MATERIAL);
  auto result = io.readParameter(path, version_1, ParameterIO::FileType::MATERIAL);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    material_parameter a(*std::dynamic_pointer_cast<material_parameter>(result[i]));
    material_parameter b(*std::dynamic_pointer_cast<material_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the material.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/*!
 * \brief ParameterIoTest.material_read_1
 * This test checks whether a previously written material file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, material_read_1)
{
  auto result =
      io.readParameter("data/materials_1.data", version_1, ParameterIO::FileType::MATERIAL);
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
 * \brief ParameterIoTest.material_write_read_2
 * This test checks whether parameters written to a file can be restored again.
 */
TEST_F(ParameterIoTest, material_write_read_2)
{
  QTemporaryDir tempPath;
  // Adding the intermediate directory "/data/" on purpose: the IO must be able to create the file
  // even if the directory doesn't exist yet.
  std::filesystem::path path{tempPath.path().toStdString() + "/data/materials_2.data"};
  std::vector<std::shared_ptr<base_parameter>> parameters;
  appendSomeMaterials(parameters);
  EXPECT_THAT(parameters, testing::SizeIs(2));

  io.writeParameter(path, parameters, version_2, ParameterIO::FileType::MATERIAL);
  auto result = io.readParameter(path, version_2, ParameterIO::FileType::MATERIAL);

  ASSERT_THAT(result, testing::SizeIs(parameters.size()));
  for (size_t i = 0; i < parameters.size(); ++i) {
    material_parameter a(*std::dynamic_pointer_cast<material_parameter>(result[i]));
    material_parameter b(*std::dynamic_pointer_cast<material_parameter>(parameters[i]));
    // This IO-version doesn't preserve the uuid of the material.
    EXPECT_TRUE(a.id().is_nil());
    a.setId(parameters[i]->id());
    EXPECT_EQ(a, b);
  }
}

/* \brief ParameterIoTest.material_read_2
 * This test checks whether a previously written material file is still legible. This test is
 * different from the test above, since it explicitly checks backward compatibility.
 */
TEST_F(ParameterIoTest, material_read_2)
{
  auto result =
      io.readParameter("data/materials_2.data", version_2, ParameterIO::FileType::MATERIAL);
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
