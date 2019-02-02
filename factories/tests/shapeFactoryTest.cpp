#include "shapeFactory.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;
using testing::Gt;
using testing::SizeIs;

/// \todo the signals still need testing
namespace test {
/// Test class for testing plane installation
class TestShape1 : public planeShape
{
  using planeShape::planeShape;
};

/// Test class for testing volume installation
class TestShape2 : public volumeShape
{
  using volumeShape::volumeShape;
};

/// Mock class to monitor the call to the installed functors
class messenger
{
public:
  MOCK_METHOD1(create, void(shape_parameter const&));
  MOCK_METHOD2(next, void(shape_parameter const&, size_t));
  MOCK_METHOD2(envelope, void(shape_parameter const&, size_t));
};
}
class unitTest : public testing::Test
{
public:
  void SetUp() override
  {
    planeDescription.setId(plane_id);
    volumeDescription.setId(volume_id);
  }

  void installPlane(ShapeFactory& factory)
  {
    EXPECT_TRUE(factory.addShapeToFactory(
        planeDescription, ShapeType::PlanarShape,
        [this](shape_parameter const& p) -> std::shared_ptr<planeShape> {
          plane_messenger.create(p);
          return std::shared_ptr<planeShape>(new test::TestShape1(planeDescription));
        },
        [this](shape_parameter const& p, size_t n) -> shape_parameter {
          plane_messenger.next(p, n);
          return {};
        },
        [this](shape_parameter const& p, size_t n) -> shape_parameter {
          plane_messenger.envelope(p, n);
          return {};
        }));
  }
  void uninstallPlane(ShapeFactory& factory) { factory.removeShapeFromFactory(plane_id); }

  void installVolume(ShapeFactory& factory)
  {
    EXPECT_TRUE(
        factory.addShapeToFactory(volumeDescription, ShapeType::VolumeShape,
                                  [this](shape_parameter const& p) -> std::shared_ptr<volumeShape> {
                                    volume_messenger.create(p);
                                    return std::shared_ptr<volumeShape>(new test::TestShape2("volume"));
                                  },
                                  [this](shape_parameter const& p, size_t n) -> shape_parameter {
                                    volume_messenger.next(p, n);
                                    return volumeDescription;
                                  },
                                  [this](shape_parameter const& p, size_t n) -> shape_parameter {
                                    volume_messenger.envelope(p, n);
                                    return volumeDescription;
                                  }));
  }

  void uninstallVolume(ShapeFactory& factory) { factory.removeShapeFromFactory(volume_id); }

  testing::StrictMock<test::messenger> plane_messenger;
  testing::StrictMock<test::messenger> volume_messenger;

  shape_parameter planeDescription;
  shape_parameter volumeDescription;
  // 2ee3fb50-1448-11e9-ab14-d663bd873d93
  const boost::uuids::uuid plane_id{{0x2e, 0xe3, 0xfb, 0x50, 0x14, 0x48, 0x11, 0xe9, 0xab, 0x14,
                                     0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
  // 385cc522-1448-11e9-ab14-d663bd873d93
  const boost::uuids::uuid volume_id{{0x38, 0x5c, 0xc5, 0x22, 0x14, 0x48, 0x11, 0xe9, 0xab, 0x14,
                                      0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
};

TEST(BasicTest, instanceTest)
{
  auto& factory = ShapeFactory::getInstance();
  EXPECT_EQ(size_t{0}, factory.getDefinedShapes().size());
}

TEST_F(unitTest, installTest)
{
  ShapeFactory factory;
  EXPECT_EQ(size_t{0}, factory.getDefinedShapes().size());
  EXPECT_FALSE(factory.isShapeDefined(plane_id));
  EXPECT_FALSE(factory.isShapeDefined(volume_id));
  EXPECT_FALSE(factory.removeShapeFromFactory(plane_id));
  EXPECT_FALSE(factory.removeShapeFromFactory(volume_id));
  // planes
  installPlane(factory);
  {
    EXPECT_TRUE(factory.isShapeDefined(plane_id));
    EXPECT_FALSE(factory.isShapeDefined(volume_id));
    EXPECT_EQ(ShapeType::PlanarShape, factory.getShapeType(plane_id));
    ASSERT_EQ(size_t{1}, factory.getDefinedShapes().size());
    EXPECT_EQ(plane_id, factory.getDefinedShapes()[0].getId());
    // volume is not available
    EXPECT_EQ(nullptr, factory.getVolume(volumeDescription));
    EXPECT_EQ(nullptr, factory.getPlane(volumeDescription));
    EXPECT_EQ(nullptr, factory.getShape(volumeDescription));
  }
  uninstallPlane(factory);
  EXPECT_FALSE(factory.isShapeDefined(plane_id));
  EXPECT_FALSE(factory.isShapeDefined(volume_id));
  EXPECT_EQ(size_t{0}, factory.getDefinedShapes().size());
  // volumes
  installVolume(factory);
  {
    EXPECT_FALSE(factory.isShapeDefined(plane_id));
    EXPECT_TRUE(factory.isShapeDefined(volume_id));
    EXPECT_EQ(ShapeType::VolumeShape, factory.getShapeType(volume_id));
    ASSERT_EQ(size_t{1}, factory.getDefinedShapes().size());
    EXPECT_EQ(volume_id, factory.getDefinedShapes()[0].getId());
    // plane is not available
    EXPECT_EQ(nullptr, factory.getVolume(planeDescription));
    EXPECT_EQ(nullptr, factory.getPlane(planeDescription));
    EXPECT_EQ(nullptr, factory.getShape(planeDescription));
  }
  uninstallVolume(factory);
  EXPECT_FALSE(factory.isShapeDefined(plane_id));
  EXPECT_FALSE(factory.isShapeDefined(volume_id));
  EXPECT_EQ(size_t{0}, factory.getDefinedShapes().size());

  installPlane(factory);
  installVolume(factory);
  EXPECT_TRUE(factory.isShapeDefined(plane_id));
  EXPECT_TRUE(factory.isShapeDefined(volume_id));
  EXPECT_EQ(size_t{2}, factory.getDefinedShapes().size());
  uninstallPlane(factory);
  uninstallVolume(factory);
}

TEST_F(unitTest, creationTest)
{
  ShapeFactory factory;
  installPlane(factory);
  installVolume(factory);
  // planes
  {
    // well, a plane isn't a volume
    EXPECT_EQ(nullptr, factory.getVolume(planeDescription));

    EXPECT_CALL(plane_messenger, create(_));
    auto shape = factory.getPlane(planeDescription);
    EXPECT_NE(nullptr, shape);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape1>(shape));

    EXPECT_CALL(plane_messenger, create(_));
    auto shape2 = factory.getShape(planeDescription);
    EXPECT_NE(nullptr, shape2);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape1>(shape2));

    // for planes there is no such thing as next or envelope
    EXPECT_EQ(nullptr, factory.getNext(planeDescription, 1));
    EXPECT_EQ(nullptr, factory.getEnvelope(planeDescription, 1));
  }
  // volumes
  {
    // well, a volume isn't a plane
    EXPECT_EQ(nullptr, factory.getPlane(volumeDescription));

    // creation as volume explicitly
    EXPECT_CALL(volume_messenger, create(_));
    auto shape = factory.getVolume(volumeDescription);
    EXPECT_NE(nullptr, shape);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape2>(shape));
    // creation as general shape
    EXPECT_CALL(volume_messenger, create(_));
    auto shape2 = factory.getShape(volumeDescription);
    EXPECT_NE(nullptr, shape2);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape2>(shape2));
    // series generation
    EXPECT_CALL(volume_messenger, next(_, 1));
    EXPECT_CALL(volume_messenger, create(_));
    shape = factory.getNext(volumeDescription, 1);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape2>(shape));
    // envelope generation
    EXPECT_CALL(volume_messenger, envelope(_, 1));
    EXPECT_CALL(volume_messenger, create(_));
    shape = factory.getEnvelope(volumeDescription, 1);
    EXPECT_NE(nullptr, dynamic_pointer_cast<test::TestShape2>(shape));
  }
  uninstallPlane(factory);
  uninstallVolume(factory);
}
