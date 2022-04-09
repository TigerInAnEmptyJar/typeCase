#include "algorithmFactory.h"
#include "event.h"
#include "setup.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;
using testing::Gt;
using testing::SizeIs;

std::ostream& operator<<(std::ostream& o, AlgorithmType a)
{
  o << static_cast<int>(a);
  return o;
}
std::ostream& operator<<(std::ostream& o, TSetup const& a)
{
  o << "Setup: " << a.name();
  return o;
}
std::ostream& operator<<(std::ostream& o, TEvent const& a)
{
  o << "Event: " << a.name();
  return o;
}
/// \todo the signals still need testing
namespace test {
/// Test class for testing algorithm installation
class TestAlgorithm : public AAlgorithm
{
  using AAlgorithm::AAlgorithm;
  void process() {}
};

/// Mock class to monitor the call to the installed functors
class messenger
{
public:
  MOCK_METHOD3(create, void(algorithm_parameter const&, TEvent&, TSetup const&));
};
} // namespace test

class AlgorithmFactoryUnitTest : public testing::Test
{
public:
  void SetUp() override
  {
    description1.setId(algo_id1);
    description2.setId(algo_id2);
  }

  void installAlgo1(AlgorithmFactory& factory)
  {
    EXPECT_TRUE(factory.addAlgorithmToFactory(
        description1, AlgorithmType::Input,
        [this](algorithm_parameter const& p, TEvent& e,
               TSetup const& s) -> std::shared_ptr<AAlgorithm> {
          algo1_messenger.create(p, e, s);
          return std::shared_ptr<AAlgorithm>(new test::TestAlgorithm("Algo1"));
        }));
  }

  void installAlgo2(AlgorithmFactory& factory)
  {
    EXPECT_TRUE(factory.addAlgorithmToFactory(
        description2, AlgorithmType::Processing,
        [this](algorithm_parameter const& p, TEvent& e,
               TSetup const& s) -> std::shared_ptr<AAlgorithm> {
          algo2_messenger.create(p, e, s);
          return std::shared_ptr<AAlgorithm>(new test::TestAlgorithm("Algo2"));
        }));
  }

  testing::StrictMock<test::messenger> algo1_messenger;
  testing::StrictMock<test::messenger> algo2_messenger;

  algorithm_parameter description1;
  algorithm_parameter description2;
  // 2ee3fb50-1448-11e9-ab14-d663bd873d93
  const boost::uuids::uuid algo_id1{{0x2e, 0xe3, 0xfb, 0x50, 0x14, 0x48, 0x11, 0xe9, 0xab, 0x14,
                                     0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
  // 385cc522-1448-11e9-ab14-d663bd873d93
  const boost::uuids::uuid algo_id2{{0x38, 0x5c, 0xc5, 0x22, 0x14, 0x48, 0x11, 0xe9, 0xab, 0x14,
                                     0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
};

TEST(BasicAlgorithmFactoryTest, instanceTest)
{
  auto& factory = AlgorithmFactory::getInstance();
  EXPECT_EQ(size_t{0}, factory.definedAlgorithms().size());
}

TEST_F(AlgorithmFactoryUnitTest, installTest)
{
  AlgorithmFactory factory;
  TEvent event;
  TSetup setup;

  EXPECT_EQ(size_t{0}, factory.definedAlgorithms().size());
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id1));
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id2));
  EXPECT_FALSE(factory.removeAlgorithmFromFactory(algo_id1));
  EXPECT_FALSE(factory.removeAlgorithmFromFactory(algo_id2));
  // algo1
  installAlgo1(factory);
  {
    EXPECT_TRUE(factory.isAlgorithmDefined(algo_id1));
    EXPECT_FALSE(factory.isAlgorithmDefined(algo_id2));
    EXPECT_EQ(AlgorithmType::Input, factory.algorithmType(algo_id1));
    ASSERT_EQ(size_t{1}, factory.definedAlgorithms().size());
    EXPECT_EQ(algo_id1, factory.definedAlgorithms()[0].id());
    // algo2 is not available
    EXPECT_EQ(nullptr, factory.create(description2, event, setup));
  }
  factory.removeAlgorithmFromFactory(algo_id1);
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id1));
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id2));
  EXPECT_EQ(size_t{0}, factory.definedAlgorithms().size());
  // algo2
  installAlgo2(factory);
  {
    EXPECT_FALSE(factory.isAlgorithmDefined(algo_id1));
    EXPECT_TRUE(factory.isAlgorithmDefined(algo_id2));
    EXPECT_EQ(AlgorithmType::Processing, factory.algorithmType(algo_id2));
    ASSERT_EQ(size_t{1}, factory.definedAlgorithms().size());
    EXPECT_EQ(algo_id2, factory.definedAlgorithms()[0].id());
    // algo1 is not available
    EXPECT_EQ(nullptr, factory.create(description1, event, setup));
  }
  factory.removeAlgorithmFromFactory(algo_id2);
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id1));
  EXPECT_FALSE(factory.isAlgorithmDefined(algo_id2));
  EXPECT_EQ(size_t{0}, factory.definedAlgorithms().size());

  installAlgo1(factory);
  installAlgo2(factory);
  EXPECT_TRUE(factory.isAlgorithmDefined(algo_id1));
  EXPECT_TRUE(factory.isAlgorithmDefined(algo_id2));
  EXPECT_EQ(size_t{2}, factory.definedAlgorithms().size());
  factory.removeAlgorithmFromFactory(algo_id1);
  factory.removeAlgorithmFromFactory(algo_id2);
}

TEST_F(AlgorithmFactoryUnitTest, creationTest)
{
  AlgorithmFactory factory;
  TEvent event;
  TSetup setup;

  installAlgo1(factory);
  installAlgo2(factory);

  EXPECT_CALL(algo1_messenger, create(_, _, _));
  auto algo1 = factory.create(description1, event, setup);
  EXPECT_NE(nullptr, algo1);

  factory.removeAlgorithmFromFactory(algo_id1);
  factory.removeAlgorithmFromFactory(algo_id2);
}
