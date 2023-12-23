#include <geometry_new.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

TEST(GeomObject, basic)
{
  geo::GeomObject object;
  EXPECT_EQ(geo::GeomState::Undefined, object.state());
  EXPECT_TRUE(object.isUndefined());
  EXPECT_FALSE(object.isNull());
  EXPECT_FALSE(object.isInfinite());
  EXPECT_FALSE(object.isRegular());
}

TEST(Point, basic)
{
  geo::Point<float, 1> p{0};
  geo::Point<float, 1> q(1.1f);

  EXPECT_EQ(geo::GeomState::Undefined, p.state());
  EXPECT_EQ(geo::GeomState::Regular, q.state());
  EXPECT_EQ(1, p.dimension());
  EXPECT_THAT(q[0], testing::FloatEq(1.1f));

  EXPECT_THROW(q[1], std::out_of_range);

  EXPECT_NE(q[0], p[0]);
  p = q;
  EXPECT_EQ(q[0], p[0]);
  EXPECT_EQ(geo::GeomState::Regular, p.state());
  p.setValues(2.0f);
  EXPECT_EQ(2.0f, p[0]);

  q[0] = 5.f;
  EXPECT_EQ(5.0f, q[0]);
}

TEST(Point, basic2)
{
  geo::Point<float, 2> p ;
  geo::Point<float, 2> q(1.1f, 2.2f);

  EXPECT_EQ(geo::GeomState::Undefined, p.state());
  EXPECT_EQ(geo::GeomState::Regular, q.state());
  EXPECT_EQ(2, p.dimension());
  EXPECT_THAT(q[0], testing::FloatEq(1.1f));
  EXPECT_THAT(q[1], testing::FloatEq(2.2f));

  EXPECT_THROW(q[2], std::out_of_range);

  EXPECT_NE(q[0], p[0]);
  p = q;
  EXPECT_EQ(q[0], p[0]);
  EXPECT_EQ(geo::GeomState::Regular, p.state());
  p.setValues(2.0f, 4.f);
  EXPECT_EQ(2.0f, p[0]);
  EXPECT_EQ(4.0f, p[1]);

  q[1] = 5.f;
  EXPECT_EQ(5.0f, q[1]);
}

TEST(Vector, basic)
{
  using VectorKind = geo::Vector<double, 3>::VectorKind;
  geo::Vector<double, 3> v;
  geo::Vector<double, 3> w{1.1, 2.2, 3.3};
  geo::Vector<int, 1> vv(5);
  geo::Vector<double, 2> ww{4, 3};

  EXPECT_EQ(geo::GeomState::Undefined, v.state());
  EXPECT_EQ(geo::GeomState::Regular, w.state());
  EXPECT_EQ(geo::GeomState::Regular, vv.state());
  EXPECT_EQ(3, v.dimension());

  EXPECT_EQ(VectorKind::Line, w.kind());
  EXPECT_TRUE(w.isLine());
  EXPECT_FALSE(w.isColumn());
  w.transpose();
  EXPECT_EQ(VectorKind::Column, w.kind());
  EXPECT_FALSE(w.isLine());
  EXPECT_TRUE(w.isColumn());
  auto a = w.transposed();
  EXPECT_EQ(VectorKind::Line, a.kind());
  EXPECT_TRUE(a.isLine());
  EXPECT_FALSE(a.isColumn());

  EXPECT_EQ(5.0, ww.length());
  auto b = ww.normalized();
  EXPECT_EQ(1.0, b.length());
  EXPECT_EQ(0.8, b[0]);
  EXPECT_EQ(0.6, b[1]);
  ww.normalize();
  EXPECT_EQ(1.0, ww.length());
  EXPECT_EQ(1., ww.scalar_product(ww));

  geo::Vector<double, 2> o{4, 3};
  geo::Vector<double, 2> oo{3, 2};
  EXPECT_EQ(18., o.scalar_product(oo));
  oo = o;
  EXPECT_EQ(o, oo);

  geo::Vector<double, 3> wx{1., 7., 15};

  auto parallel = w.projectOn(wx);
  auto perpendicular = w.perpendicularToProject(wx);
  EXPECT_THAT(wx[0], testing::DoubleEq((parallel + perpendicular)[0]));
  EXPECT_THAT(wx[1], testing::DoubleEq((parallel + perpendicular)[1]));
  EXPECT_THAT(wx[2], testing::DoubleEq((parallel + perpendicular)[2]));
  EXPECT_THAT(parallel[1], testing::DoubleEq(parallel[0] * 2.));
  EXPECT_THAT(parallel[2], testing::DoubleEq(parallel[0] * 3.));
  EXPECT_THAT(perpendicular * w, testing::DoubleNear(0, 1e-10));
  EXPECT_THAT(perpendicular * parallel, testing::DoubleNear(0, 1e-10));
  EXPECT_EQ(wx.length(), sqrt(parallel.length() * parallel.length() +
                              perpendicular.length() * perpendicular.length()));
}

TEST(Point2D, basic)
{
  geo::Point2D<double> p;
  EXPECT_EQ(geo::GeomState::Undefined, p.state());

  geo::Point2D<double> p1{3., 4.};
  EXPECT_EQ(geo::GeomState::Regular, p1.state());
  EXPECT_EQ(3., p1[0]);
  EXPECT_EQ(4., p1[1]);
  EXPECT_EQ(5., p1.r());
  EXPECT_THAT(p1.phi(), testing::DoubleNear(0.927295, 0.000001));

  geo::Point2D<double> p1a{3., -4.};
  EXPECT_EQ(geo::GeomState::Regular, p1a.state());
  EXPECT_EQ(3., p1a[0]);
  EXPECT_EQ(-4., p1a[1]);
  EXPECT_EQ(5., p1a.r());
  EXPECT_EQ(p1.phi(), -p1a.phi());

  geo::Point2D<double> p2 = p1;
  EXPECT_EQ(geo::GeomState::Regular, p2.state());
  EXPECT_EQ(3., p2[0]);
  EXPECT_EQ(4., p2[1]);
  EXPECT_EQ(5., p2.r());
  EXPECT_THAT(p2.phi(), testing::DoubleNear(0.927295, 0.000001));

  geo::Point2D<double> p3{0., 0.};
  EXPECT_EQ(geo::GeomState::Null, p3.state());
  EXPECT_EQ(0., p3[0]);
  EXPECT_EQ(0., p3[1]);
  EXPECT_EQ(0., p3.r());
  EXPECT_EQ(0., p3.phi());
  auto res = std::is_base_of<geo::Point<int, 2>, geo::Point2D<int>>::value;
  EXPECT_TRUE(res);
}

TEST(Vector2D, basic)
{
  geo::Vector2D<double> v;
  EXPECT_EQ(geo::GeomState::Undefined, v.state());

  geo::Vector2D<double> v1{3., 4.};
  EXPECT_EQ(geo::GeomState::Regular, v1.state());
  EXPECT_EQ(3., v1[0]);
  EXPECT_EQ(4., v1[1]);
  EXPECT_EQ(5., v1.r());
  EXPECT_EQ(v1.length(), v1.r());
  EXPECT_THAT(v1.phi(), testing::DoubleNear(0.927295, 0.000001));
  EXPECT_TRUE(v1.isLine());

  v1.transpose();
  auto v2 = v1;
  EXPECT_EQ(geo::GeomState::Regular, v2.state());
  EXPECT_EQ(3., v2[0]);
  EXPECT_EQ(4., v2[1]);
  EXPECT_EQ(5., v2.r());
  EXPECT_FALSE(v2.isLine());
  EXPECT_THAT(v2.phi(), testing::DoubleNear(0.927295, 0.000001));

  geo::Vector2D<double> v3{0., 0.};
  EXPECT_EQ(geo::GeomState::Null, v3.state());
  EXPECT_EQ(0., v3[0]);
  EXPECT_EQ(0., v3[1]);
  EXPECT_EQ(0., v3.r());
  EXPECT_EQ(0., v3.phi());
  auto res = std::is_base_of<geo::Vector<int, 2>, geo::Vector2D<int>>::value;
  EXPECT_TRUE(res);
}

TEST(Point3D, basic)
{
  geo::Point3D<double> p;
  EXPECT_EQ(geo::GeomState::Undefined, p.state());

  geo::Point3D<double> p1{1., 1., 1.};
  EXPECT_EQ(geo::GeomState::Regular, p1.state());
  EXPECT_EQ(1., p1[0]);
  EXPECT_EQ(1., p1[1]);
  EXPECT_EQ(1., p1[2]);
  EXPECT_THAT(p1.r(), testing::DoubleNear(sqrt(3.), 0.000001));
  EXPECT_THAT(p1.rho(), testing::DoubleNear(sqrt(2.), 0.000001));
  EXPECT_THAT(p1.phi(), testing::DoubleNear(M_PI / 4, 0.000001));
  EXPECT_THAT(p1.theta(), testing::DoubleNear(0.955317, 0.000001));

  geo::Point3D<double> p2 = p1;
  EXPECT_EQ(geo::GeomState::Regular, p2.state());
  EXPECT_EQ(1., p2[0]);
  EXPECT_EQ(1., p2[1]);
  EXPECT_EQ(1., p2[2]);
  EXPECT_THAT(p2.r(), testing::DoubleNear(sqrt(3.), 0.000001));
  EXPECT_THAT(p2.rho(), testing::DoubleNear(sqrt(2.), 0.000001));
  EXPECT_THAT(p2.phi(), testing::DoubleNear(M_PI / 4, 0.000001));
  EXPECT_THAT(p2.theta(), testing::DoubleNear(0.955317, 0.000001));

  geo::Point3D<double> p3{0., 0., 0.};
  EXPECT_EQ(geo::GeomState::Null, p3.state());
  EXPECT_EQ(0., p3[0]);
  EXPECT_EQ(0., p3[1]);
  EXPECT_EQ(0., p3.r());
  EXPECT_EQ(0., p3.phi());
  auto res = std::is_base_of<geo::Point<int, 3>, geo::Point3D<int>>::value;
  EXPECT_TRUE(res);
}

TEST(Vector3D, basic)
{
  geo::Vector3D<double> v;
  EXPECT_EQ(geo::GeomState::Undefined, v.state());

  geo::Vector3D<double> v1{1., 1., 1.};
  EXPECT_EQ(geo::GeomState::Regular, v1.state());
  EXPECT_EQ(1., v1[0]);
  EXPECT_EQ(1., v1[1]);
  EXPECT_EQ(1., v1[2]);
  EXPECT_THAT(v1.r(), testing::DoubleNear(sqrt(3.), 0.000001));
  EXPECT_THAT(v1.rho(), testing::DoubleNear(sqrt(2.), 0.000001));
  EXPECT_THAT(v1.length(), testing::DoubleNear(v1.r(), 0.0000001));
  EXPECT_THAT(v1.phi(), testing::DoubleNear(M_PI / 4, 0.000001));
  EXPECT_THAT(v1.theta(), testing::DoubleNear(0.955317, 0.000001));
  EXPECT_TRUE(v1.isLine());

  v1.transpose();
  geo::Vector3D<double> v2 = v1;
  EXPECT_EQ(geo::GeomState::Regular, v2.state());
  EXPECT_EQ(1., v2[0]);
  EXPECT_EQ(1., v2[1]);
  EXPECT_EQ(1., v2[2]);
  EXPECT_THAT(v2.r(), testing::DoubleNear(sqrt(3.), 0.000001));
  EXPECT_THAT(v2.rho(), testing::DoubleNear(sqrt(2.), 0.000001));
  EXPECT_THAT(v2.length(), testing::DoubleNear(v2.r(), 0.000001));
  EXPECT_THAT(v2.phi(), testing::DoubleNear(M_PI / 4, 0.000001));
  EXPECT_THAT(v2.theta(), testing::DoubleNear(0.955317, 0.000001));
  EXPECT_FALSE(v2.isLine());

  geo::Vector3D<double> v3{0., 0., 0.};
  EXPECT_EQ(geo::GeomState::Null, v3.state());
  EXPECT_EQ(0., v3[0]);
  EXPECT_EQ(0., v3[1]);
  EXPECT_EQ(0., v3[2]);
  EXPECT_EQ(0., v3.r());
  EXPECT_EQ(0., v3.rho());
  EXPECT_EQ(0., v3.phi());
  EXPECT_EQ(0., v3.theta());

  auto res = std::is_base_of<geo::Vector<int, 3>, geo::Vector3D<int>>::value;
  EXPECT_TRUE(res);

  // Cross product
  {
    auto v1 = geo::Vector3D<double>{1., 5., 0};
    auto v2 = geo::Vector3D<double>{70., -5., 0};
    auto cross = v1.cross(v2);
    EXPECT_THAT(cross[0], testing::DoubleNear(0, 1e-7));
    EXPECT_THAT(cross[1], testing::DoubleNear(0, 1e-7));
    EXPECT_THAT(cross[2], testing::DoubleEq(-v1.length() * v2.length() *
                                            sin(acos((v1 * v2) / v1.length() / v2.length()))));
  }
}

TEST(Functions, basic)
{
  geo::Point<double, 2> p{5., 3.};
  geo::Point<double, 2> q{1., 2.};
  auto v = p - q;
  EXPECT_EQ(4, v[0]);
  EXPECT_EQ(1, v[1]);

  auto pp = p + 2. * v;
  EXPECT_EQ(13., pp[0]);
  EXPECT_EQ(5., pp[1]);

  EXPECT_EQ(17., v * v);

  geo::Point2D<double> qq = q - v;
  EXPECT_EQ(-3., qq[0]);
  EXPECT_EQ(1., qq[1]);

  geo::Vector<double, 3> v1{1, 0, 0};
  geo::Vector<double, 3> v2{1, 1, 2};

  EXPECT_THAT(geo::cosA(v1, v2), testing::DoubleEq((v1 * v2) / v1.length() / v2.length()));
}

TEST(MatrixNxM, 2D)
{
  // 1 2
  // 3 4
  geo::Matrix<int, 2, 2> m22 = {geo::Vector<int, 2>{1, 2}, geo::Vector<int, 2>{3, 4}};

  auto m22T = m22.transpose();

  EXPECT_EQ(m22.value(0, 0), m22T.value(0, 0));
  EXPECT_EQ(m22.value(1, 0), m22T.value(0, 1));
  EXPECT_EQ(m22.value(0, 1), m22T.value(1, 0));
  EXPECT_EQ(m22.value(1, 1), m22T.value(1, 1));

  EXPECT_EQ(1 * 4 - 2 * 3, m22.determinant());
}

TEST(MatrixNxM, 3D)
{
  // 1 2 3
  // 4 5 6
  // 7 8 9
  geo::Matrix<int, 3, 3> m33 = {geo::Vector<int, 3>{1, 2, 3}, geo::Vector<int, 3>{4, 5, 6},
                                geo::Vector<int, 3>{7, 8, 9}};

  auto m33T = m33.transpose();

  EXPECT_EQ(m33.value(0, 0), m33T.value(0, 0));
  EXPECT_EQ(m33.value(1, 0), m33T.value(0, 1));
  EXPECT_EQ(m33.value(2, 0), m33T.value(0, 2));
  EXPECT_EQ(m33.value(0, 1), m33T.value(1, 0));
  EXPECT_EQ(m33.value(1, 1), m33T.value(1, 1));
  EXPECT_EQ(m33.value(2, 1), m33T.value(1, 2));
  EXPECT_EQ(m33.value(0, 2), m33T.value(2, 0));
  EXPECT_EQ(m33.value(1, 2), m33T.value(2, 1));
  EXPECT_EQ(m33.value(2, 2), m33T.value(2, 2));

  geo::Matrix<int, 2, 2> m221 = {geo::Vector<int, 2>{5, 6}, geo::Vector<int, 2>{8, 9}};
  geo::Matrix<int, 2, 2> m222 = {geo::Vector<int, 2>{4, 6}, geo::Vector<int, 2>{7, 9}};
  geo::Matrix<int, 2, 2> m223 = {geo::Vector<int, 2>{4, 5}, geo::Vector<int, 2>{7, 8}};

  EXPECT_EQ(1 * m221.determinant() - 2 * m222.determinant() + 3 * m223.determinant(),
            m33.determinant());

  EXPECT_EQ(1 * 5 * 9 + 2 * 6 * 7 + 3 * 4 * 8 - 1 * 6 * 8 - 2 * 4 * 9 - 3 * 5 * 7,
            m33.determinant());
}

TEST(Matrix3D, basic)
{
  geo::Matrix3D<double> m{geo::Vector3D<double>{1, 0, 0}, M_PI_2};

  geo::Vector<double, 3> v{0, 1, 0};

  auto vr = m * v;

  EXPECT_EQ(0., vr[0]);
  EXPECT_EQ(0., vr[1]);
  EXPECT_EQ(1., vr[2]);
}

TEST(MatrixNxM, functions)
{
  geo::Matrix<int, 2, 2> m221 = {geo::Vector<int, 2>{1, 2}, geo::Vector<int, 2>{3, 4}};
  geo::Matrix<int, 2, 2> m222 = {geo::Vector<int, 2>{2, 3}, geo::Vector<int, 2>{4, 5}};

  auto m22s = m221 + m222;
  EXPECT_EQ(2, m22s.lines());
  EXPECT_EQ(2, m22s.columns());
  EXPECT_EQ(3, m22s.value(0, 0));
  EXPECT_EQ(5, m22s.value(0, 1));
  EXPECT_EQ(7, m22s.value(1, 0));
  EXPECT_EQ(9, m22s.value(1, 1));

  geo::Matrix<int, 2, 3> m23 = {geo::Vector<int, 3>{1, 2, 3}, geo::Vector<int, 3>{4, 5, 6}};
  auto m23p = m221 * m23;

  EXPECT_EQ(2, m23p.lines());
  EXPECT_EQ(3, m23p.columns());
  EXPECT_EQ(9, m23p.value(0, 0));
  EXPECT_EQ(12, m23p.value(0, 1));
  EXPECT_EQ(15, m23p.value(0, 2));
  EXPECT_EQ(19, m23p.value(1, 0));
  EXPECT_EQ(26, m23p.value(1, 1));
  EXPECT_EQ(33, m23p.value(1, 2));

  geo::Vector<int, 2> v1{11, 12};

  auto vp1 = v1 * m23;
  EXPECT_EQ(3, vp1.dimension());
  EXPECT_EQ(59, vp1[0]);
  EXPECT_EQ(82, vp1[1]);
  EXPECT_EQ(105, vp1[2]);

  geo::Vector<int, 3> v3{1, 2, 3};
  auto vp2 = m23 * v3;
  EXPECT_EQ(2, vp2.dimension());
  EXPECT_EQ(14, vp2[0]);
  EXPECT_EQ(32, vp2[1]);
}

TEST(LimitedLine, basic)
{
  auto p1 = geo::Point3D<float>{0, 0, 0};
  auto p2 = geo::Point3D<float>{10, 10, 10};
  geo::LimitedLine<float, 3> line{p1, p2};

  EXPECT_EQ(p1, line.start());
  EXPECT_EQ(p2, line.stop());
  EXPECT_EQ(p2 - p1, line.length());
}

TEST(LimitedLine, distanceToPoint)
{
  auto p1 = geo::Point3D<float>{0, 0, 0};
  auto p2 = geo::Point3D<float>{10, 10, 10};
  geo::LimitedLine<float, 3> line{p1, p2};

  // left of start
  {
    auto px1 = geo::Point3D<float>{-10, -2, -5};
    EXPECT_EQ(px1 - p1, distance(line, px1));
  }
  // on line
  {
    auto px2 = geo::Point3D<float>{5, 5, 5};
    EXPECT_THAT(distance(line, px2).length(), testing::FloatNear(0, 1e-6f));
  }
  // distance to line
  {
    auto px3 = geo::Point3D<float>{5, 6, 5};
    auto d = distance(line, px3);
    EXPECT_THAT(distance(line, px3 - d).length(), testing::FloatNear(0, 1e-6f));
    EXPECT_THAT(d * line.length(), testing::FloatNear(0, 1e-5f));
  }
  // right of stop
  {
    auto px4 = geo::Point3D<float>{20, 70, 30};
    EXPECT_EQ(px4 - p2, distance(line, px4));
  }
}

TEST(StraightLine, basic)
{
  auto p = geo::Point3D<float>{0, 0, 0};
  auto v = geo::Vector3D<float>{10, 10, 10};
  geo::StraightLine<float, 3> line{p, v};

  EXPECT_EQ(p, line.foot());
  EXPECT_EQ(v, line.direction());
}

TEST(StraightLine, distanceToPoint)
{
  auto p = geo::Point3D<double>{0, 0, 0};
  auto v = geo::Vector3D<double>{10, 10, 10};
  geo::StraightLine<double, 3> line{p, v};

  // on line
  {
    auto px2 = geo::Point3D<double>{5, 5, 5};
    EXPECT_THAT(distance(line, px2).length(), testing::DoubleNear(0, 1e-6));
  }
  // distance to line
  {
    auto px3 = geo::Point3D<double>{5, 6, 10};
    auto d = distance(line, px3);
    EXPECT_THAT(distance(line, px3 - d).length(), testing::DoubleNear(0, 1e-6));
    EXPECT_THAT(d * line.direction(), testing::DoubleNear(0, 1e-5));
  }
}

TEST(StraightLine, distanceToLine)
{
  // lines do intersect
  {
    auto p = geo::Point3D<double>{0, 0, 0};
    auto v1 = geo::Vector3D<double>{10, 10, 10};
    auto v2 = geo::Vector3D<double>{0, -1, 5};
    geo::StraightLine<double, 3> line1{p + v1, v1};
    geo::StraightLine<double, 3> line2{p - v2, v2};
    auto i = intersection(line1, line2);
    EXPECT_THAT(i.length().length(), testing::DoubleNear(0, 1e-6));
    EXPECT_THAT(i.start()[0], testing::DoubleNear(p[0], 1e-6));
    EXPECT_THAT(i.start()[1], testing::DoubleNear(p[1], 1e-6));
    EXPECT_THAT(i.start()[2], testing::DoubleNear(p[2], 1e-6));
  }
  // distance to line
  {
    auto p1 = geo::Point3D<double>{0, 0, 0};
    auto p2 = geo::Point3D<double>{0, 10, 0};
    auto v1 = geo::Vector3D<double>{10, 0, 10};
    auto v2 = geo::Vector3D<double>{-1, 0, 5};
    geo::StraightLine<double, 3> line1{p1 + v1, v1};
    geo::StraightLine<double, 3> line2{p2 - v2, v2};
    auto d = intersection(line1, line2);
    EXPECT_THAT(d.length().length(), testing::DoubleNear(10, 1e-6));
    EXPECT_THAT(d.start()[0], testing::DoubleNear(p1[0], 1e-6));
    EXPECT_THAT(d.start()[1], testing::DoubleNear(p1[1], 1e-6));
    EXPECT_THAT(d.start()[2], testing::DoubleNear(p1[2], 1e-6));
    EXPECT_THAT(d.stop()[0], testing::DoubleNear(p2[0], 1e-6));
    EXPECT_THAT(d.stop()[1], testing::DoubleNear(p2[1], 1e-6));
    EXPECT_THAT(d.stop()[2], testing::DoubleNear(p2[2], 1e-6));
  }

  // distance to line segment
  // left of start
  {
    geo::LimitedLine<double, 3> l{{0, 0, 0}, {1, 0, 0}};
    geo::StraightLine<double, 3> line{{0, 1, 0}, {0, 0, 1}};
    auto d = intersection(line, l);
    EXPECT_EQ(d.stop(), l.start());
    EXPECT_EQ(d.start(), line.foot());
  }
  // on line
  {
    geo::LimitedLine<double, 3> l{{-10, 0, 0}, {10, 0, 0}};
    geo::StraightLine<double, 3> line{{0, 0, -5}, {0, 0, 1}};
    auto d = intersection(line, l);
    EXPECT_THAT(d.length().length(), testing::DoubleNear(0, 1e-6));
  }
  // distance to line
  {
    geo::LimitedLine<double, 3> l{{-10, 0, 0}, {10, 0, 0}};
    geo::StraightLine<double, 3> line{{0, 1, -5}, {0, 0, 1}};
    auto d = intersection(line, l);
    geo::Point<double, 3> p1{0, 0, 0};
    geo::Point<double, 3> p2{0, 1, 0};
    EXPECT_EQ(d.stop(), p1);
    EXPECT_EQ(d.start(), p2);
  }
  // right of stop
  {
    geo::LimitedLine<double, 3> l{{-5, 0, 0}, {-1, 0, 0}};
    geo::StraightLine<double, 3> line{{0, 1, 0}, {0, 0, 1}};
    auto d = intersection(line, l);
    EXPECT_EQ(d.stop(), l.stop());
    EXPECT_EQ(d.start(), line.foot());
  }
}

TEST(Plane, basic)
{
  {
    geo::Plane<int, 4> pl({0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 2});
    EXPECT_EQ(geo::GeomState::Null, pl.state());
  }
  {
    geo::Plane<int, 4> pl({}, {1, 0, 0, 0}, {0, 1, 0, 2});
    EXPECT_EQ(geo::GeomState::Undefined, pl.state());
  }
  {
    geo::Plane<int, 4> pl({0, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0});
    EXPECT_EQ(geo::GeomState::Null, pl.state());
  }
  {
    geo::Plane<int, 4> pl({0, 0, 0, 0}, {0, 5, 0, 10}, {0, 1, 0, 2});
    EXPECT_EQ(geo::GeomState::Null, pl.state());
  }
  {
    geo::Point<int, 4> f{0, 0, 0, 0};
    geo::Vector<int, 4> d1{1, 0, 0, 0};
    geo::Vector<int, 4> d2{0, 1, 0, 2};
    geo::Plane<int, 4> pl({0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 2});
    EXPECT_EQ(geo::GeomState::Regular, pl.state());
    EXPECT_EQ(f, pl.foot());
    EXPECT_EQ(d1, pl.direction1());
    EXPECT_EQ(d2, pl.direction2());
  }
}

TEST(Plane, intersection)
{
  geo::Point<double, 3> f{0, 0, 0};
  geo::Vector<double, 3> d1{1, 0, 0};
  geo::Vector<double, 3> d2{0, 1, 0};
  geo::Plane<double, 3> pl(f, d1, d2);

  // Point out of plane
  {
    geo::Point<double, 3> p{1, 1, 5};
    auto d = distance(pl, p);
    EXPECT_THAT(d[0], testing::DoubleNear(0, 1e-7));
    EXPECT_THAT(d[1], testing::DoubleNear(0, 1e-7));
    EXPECT_THAT(d[2], testing::DoubleNear(5, 1e-7));
  }
  // Point on plane
  {
    geo::Point<double, 3> p{1, 1, 0};
    auto d = distance(pl, p);
    EXPECT_THAT(d.length(), testing::DoubleNear(0, 1e-7));
  }
  // Intersection with straight line
  {
    geo::Point<double, 3> p{1, 1, 0};
    geo::Vector<double, 3> v{1.5, 7.3, 9.04};
    geo::StraightLine<double, 3> line(p - 13.7 * v, v);
    auto d = intersection(pl, line);
    ASSERT_EQ(geo::GeomState::Regular, d.state());
    EXPECT_THAT(d[0], testing::DoubleNear(p[0], 1e-7));
    EXPECT_THAT(d[1], testing::DoubleNear(p[1], 1e-7));
    EXPECT_THAT(d[2], testing::DoubleNear(p[2], 1e-7));
  }
  // Intersection with allmost parallel straight line
  {
    geo::Point<double, 3> p{1, 1, 0};
    geo::Vector<double, 3> v{1, 1, 0.001};
    geo::StraightLine<double, 3> line(p - 13.7 * v, v);
    auto d = intersection(pl, line);
    ASSERT_EQ(geo::GeomState::Undefined, d.state());
  }
  // Intersection with limited line (left)
  {
    geo::Point<double, 3> p{1, 1, 1};
    geo::Point<double, 3> p1{1, 1, 0};
    geo::Vector<double, 3> v{0, 0, 9.04};
    geo::LimitedLine<double, 3> line(p + v, p);
    auto d = intersection(pl, line);
    EXPECT_THAT(d.start()[0], testing::DoubleNear(p1[0], 1e-7));
    EXPECT_THAT(d.start()[1], testing::DoubleNear(p1[1], 1e-7));
    EXPECT_THAT(d.start()[2], testing::DoubleNear(p1[2], 1e-7));
    EXPECT_THAT(d.stop()[0], testing::DoubleNear(p[0], 1e-7));
    EXPECT_THAT(d.stop()[1], testing::DoubleNear(p[1], 1e-7));
    EXPECT_THAT(d.stop()[2], testing::DoubleNear(p[2], 1e-7));
  }
  // Intersection with limited line (right)
  {
    geo::Point<double, 3> p{1, 1, 1};
    geo::Point<double, 3> p1{1, 1, 0};
    geo::Vector<double, 3> v{0, 0, 9.04};
    geo::LimitedLine<double, 3> line(p, p + v);
    auto d = intersection(pl, line);
    EXPECT_THAT(d.start()[0], testing::DoubleNear(p1[0], 1e-7));
    EXPECT_THAT(d.start()[1], testing::DoubleNear(p1[1], 1e-7));
    EXPECT_THAT(d.start()[2], testing::DoubleNear(p1[2], 1e-7));
    EXPECT_THAT(d.stop()[0], testing::DoubleNear(p[0], 1e-7));
    EXPECT_THAT(d.stop()[1], testing::DoubleNear(p[1], 1e-7));
    EXPECT_THAT(d.stop()[2], testing::DoubleNear(p[2], 1e-7));
  }
  // Intersection with limited line (hit)
  {
    geo::Point<double, 3> p{1, 1, 0};
    geo::Vector<double, 3> v{1.5, 7.3, 9.04};
    geo::LimitedLine<double, 3> line(p - v, p + v);
    auto d = intersection(pl, line);
    EXPECT_THAT(d.start()[0], testing::DoubleNear(p[0], 1e-7));
    EXPECT_THAT(d.start()[1], testing::DoubleNear(p[1], 1e-7));
    EXPECT_THAT(d.start()[2], testing::DoubleNear(p[2], 1e-7));
    EXPECT_THAT(d.length().length(), testing::DoubleNear(0, 1e-7));
  }
  // Intersection with limited line (almost parallel)
  {
    geo::Point<double, 3> p{1, 1, 0};
    geo::Vector<double, 3> v{5.5, 5.5, 0.001};
    geo::LimitedLine<double, 3> line(p - v, p + v);
    auto d = intersection(pl, line);
    EXPECT_EQ(geo::GeomState::Undefined, d.state());
  }
  // Intersection with other plane
  {
    geo::Point<double, 3> f2{0, 0, 0};
    geo::Vector<double, 3> d21{0, 0, 1};
    geo::Vector<double, 3> d22{1.5, 1, 0};
    geo::Plane<double, 3> pl2(f2 + d21 + d22 * 3.1, d21, d22);
    auto inter = intersection(pl, pl2);
    EXPECT_EQ(geo::GeomState::Regular, inter.state());
    EXPECT_EQ(inter.foot(), f);
    auto normal1 = cross(d1, d2);
    auto normal2 = cross(d21, d22);
    EXPECT_THAT(normal1 * inter.direction(), testing::DoubleNear(0, 1e-7));
    EXPECT_THAT(normal2 * inter.direction(), testing::DoubleNear(0, 1e-7));
  }
  // Intersection with other plane that is almost parallel
  {
    geo::Point<double, 3> f2{0, 4, 0};
    geo::Vector<double, 3> d21{1, 0, 0};
    geo::Vector<double, 3> d22{0, 1.01, 0};
    geo::Plane<double, 3> pl2(f2 + d21 + d22 * 3.1, d21, d22);
    auto inter = intersection(pl, pl2);
    EXPECT_EQ(geo::GeomState::Undefined, inter.state());
  }
}
