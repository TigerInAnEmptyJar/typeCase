#include "sphericRectangle.h"
#include "shapeparameter.h"
sphericRectangle::sphericRectangle(point3D a, point3D b, point3D c, point3D d)
    : planeShape("sphericRectangle")
{
  vector3D a_b(b - a);
  vector3D c_d(c - d);
  float l1 = a_b.R(), l2 = c_d.R();
  if (l1 - l2 > __prec)
    return;
  sLine3D AB(a, a_b);
  sLine3D DC(d, c_d);
  lLine3D hit = AB - DC;
  if ((hit.P() - hit.Q()).R() > __prec)
    return;
  A_ = a;
  B_ = b;
  C_ = c;
  D_ = d;
  float ri = (hit.P() - A_).length();
  float ra = (hit.P() - B_).length();
  if (ri > ra) {
    float sw = ri;
    ri = ra;
    ra = sw;
  }
  vector3D lp = B_ - A_;
  vector3D hp = C_ - D_;
  lp.normalize();
  hp.normalize();
  normal = lp ^ hp;
  normal.normalize();
  //  matrix3D turn(normal,acos(lp*hp)*0.5);
  center = hit.P() + (lp + hp) * 0.5 * ((ra + ri) * 0.5);
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}
sphericRectangle::sphericRectangle(point3D mid, vector3D lp, vector3D hp, float ra, float ri)
    : planeShape("sphericRectangle")
{
  lp.normalize();
  hp.normalize();
  normal = lp ^ hp;
  normal.normalize();
  A_ = mid + lp * ri;
  B_ = mid + lp * ra;
  vector3D tmp = hp + lp;
  tmp.normalize();
  center = mid + (tmp * (ri + ra) * 0.5);
  C_ = mid + hp * ra;
  D_ = mid + hp * ri;
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  //  cout<<((vector3D(A_)+vector3D(B_)+vector3D(C_)+vector3D(D_))*0.25).toString(_spheric_).ascii()<<center.toString(_spheric_).ascii()<<endl;
}

sphericRectangle::sphericRectangle(point3D mid, vector3D lp, vector3D nor, float ra, float ri,
                                   float dp)
    : planeShape("sphericRectangle")
{
  //   nor.normalize();
  lp.normalize();
  //   normal=nor^lp;
  normal = nor;
  normal.normalize();
  A_ = mid + lp * ri;
  B_ = mid + lp * ra;
  matrix3D turn(normal, -dp * 0.5);
  vector3D tmp;
  tmp = turn * lp;
  center = mid + (tmp * (ri + ra) * 0.5);
  tmp = turn * tmp;
  C_ = mid + tmp * ra;
  D_ = mid + tmp * ri;
  //  float th,ph,r;
  //  r=(ri+ra)/2.0;
  //  th=(nor.Theta()+lp.Theta())/2.0;
  //  ph=(nor.Phi()+lp.Phi())/2.0;
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}
sphericRectangle::sphericRectangle(const shape_parameter& description)
    : planeShape("sphericRectangle")
{
  if (description.getName() != "sphericRectangle")
    return;
  if (description.NumberOfParams<point3D>() < 5)
    return;
  center = description.getParam<point3D>(0);
  A_ = description.getParam<point3D>(0);
  B_ = description.getParam<point3D>(1);
  C_ = description.getParam<point3D>(2);
  D_ = description.getParam<point3D>(3);
  vector3D lp = B_ - A_;
  vector3D hp = C_ - D_;
  lp.normalize();
  hp.normalize();
  normal = lp ^ hp;
  normal.normalize();
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}
sphericRectangle::~sphericRectangle() {}

void sphericRectangle::setPoint(int num, point3D p)
{
  point3D tmp;
  switch (num) {
  case 0: {
    tmp = A_;
    A_ = p;
    break;
  }
  case 1: {
    tmp = B_;
    B_ = p;
    break;
  }
  case 2: {
    tmp = C_;
    C_ = p;
    break;
  }
  case 3: {
    tmp = D_;
    D_ = p;
    break;
  }
  }
  if (tmp.getState() == _undefined_)
    return;
  if ((B_ - A_).length() != (C_ - D_).length()) {
    switch (num) {
    case 0:
      A_ = tmp;
      return;
    case 1:
      B_ = tmp;
      return;
    case 2:
      C_ = tmp;
      return;
    case 3:
      D_ = tmp;
      return;
    }
    return;
  }
  sLine3D AB(A_, B_ - A_);
  sLine3D DC(D_, C_ - D_);
  lLine3D hit = AB - DC;
  if (!(hit.P() == hit.Q())) {
    switch (num) {
    case 0:
      A_ = tmp;
      return;
    case 1:
      B_ = tmp;
      return;
    case 2:
      C_ = tmp;
      return;
    case 3:
      D_ = tmp;
      return;
    }
    return;
  }
  float ri = (hit.P() - A_).length();
  float ra = (hit.P() - B_).length();
  if (ri > ra) {
    float sw = ri;
    ri = ra;
    ra = sw;
  }
  vector3D lp = B_ - A_;
  vector3D hp = C_ - D_;
  lp.normalize();
  hp.normalize();
  normal = lp ^ hp;
  normal.normalize();
  matrix3D turn(normal, -acos(lp * hp) * 0.5);
  center = hit.P() + (turn * lp) * ((ra + ri) * 0.5);
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}

void sphericRectangle::setPoints(point3D mid, vector3D lp, vector3D nor, float ra, float ri,
                                 float dp)
{
  normal = nor;
  normal.normalize();
  lp.normalize();
  A_ = mid + lp * ri;
  B_ = mid + lp * ra;
  matrix3D turn(normal, -dp * 0.5);
  vector3D tmp;
  tmp = turn * lp;
  center = mid + tmp * ((ra + ri) * 0.5);
  tmp = turn * tmp;
  C_ = mid + tmp * ra;
  D_ = mid + tmp * ri;
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}

void sphericRectangle::setPoints(point3D a, point3D b, point3D c, point3D d)
{
  if ((b - a).length() != (c - d).length())
    return;
  sLine3D AB(a, b - a);
  sLine3D DC(d, c - d);
  lLine3D hit = AB - DC;
  if (!(hit.P() == hit.Q()))
    return;
  A_ = a;
  B_ = b;
  C_ = c;
  D_ = d;
  float ri = (hit.P() - A_).length();
  float ra = (hit.P() - B_).length();
  if (ri > ra) {
    float sw = ri;
    ri = ra;
    ra = sw;
  }
  vector3D lp = B_ - A_;
  vector3D hp = C_ - D_;
  lp.normalize();
  hp.normalize();
  normal = lp ^ hp;
  normal.normalize();
  matrix3D turn(normal, -acos(lp * hp) * 0.5);
  center = hit.P() + (turn * lp) * ((ra + ri) * 0.5);
  vector3D v1(A_ - center), v2(B_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
}

point3D sphericRectangle::A() const { return A_; }

point3D sphericRectangle::B() const { return B_; }

point3D sphericRectangle::C() const { return C_; }

point3D sphericRectangle::D() const { return D_; }
float sphericRectangle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  vector3D n[4], nc;
  nc = vc - zDirection * (vc * zDirection);
  nc.normalize();
  float t[4];
  for (int i = 0; i < 4; i++) {
    //      v[i].normalize();
    n[i] = v[i] - zDirection * (v[i] * zDirection);
    n[i].normalize();
    t[i] = n[i] * nc;
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
float sphericRectangle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  float t[4];
  for (int i = 0; i < 4; i++) {
    v[i].normalize();
    t[i] = acos(v[i] * zDirection);
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}

point3D sphericRectangle::getPoint(int num) const
{
  switch (num) {
  case 0:
    return A();
  case 1:
    return B();
  case 2:
    return C();
  case 3:
    return D();
  }
  return point3D();
}

int sphericRectangle::getNumberOfPoints() const { return 4; }
vector3D sphericRectangle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v[4];
  v[0] = p & A();
  v[1] = p & B();
  v[2] = p & C();
  v[3] = p & D();
  for (int i = 0; i < 4; i++)
    for (int j = i + 1; j < 4; j++)
      if (v[i] * v[j] < 0)
        return vector3D(0, 0, 0);
  int m = 0;
  for (int i = 1; i < 4; i++)
    if (v[i].length() < v[m].length())
      m = i;
  return v[m];
}
vector3D sphericRectangle::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector3D aa = A_ - hit, bb = B_ - hit, cc = C_ - hit, dd = D_ - hit;
  float a = acos((aa * bb) / aa.R() / bb.R()), b = acos((bb * cc) / bb.R() / cc.R()),
        c = acos((cc * dd) / cc.R() / dd.R()), d = acos((aa * dd) / aa.R() / dd.R());
  if ((a + b + c + d - M_PI * 2.) > -0.01)
    return vector3D(0, 0, 0);
  if ((a + b + c + d - M_PI) < 0) {
    if (aa.R() < bb.R() && aa.R() < cc.R() && aa.R() < dd.R())
      return aa;
    if (bb.R() < cc.R() && bb.R() < dd.R())
      return bb;
    if (cc.R() < dd.R())
      return cc;
    return dd;
  }
  if (a > b && a > c && a > d)
    return (bb + aa) * 0.5;
  if (b > c && b > d)
    return (bb + cc) * 0.5;
  if (c > d)
    return (cc + dd) * 0.5;
  return (dd + aa) * 0.5;
  //   vector3D lpe=B_-A_;
  //   vector3D hpe=C_-D_;
  //   vector3D cen=(lpe+hpe);
  //   vector3D l=
  //   float a=acos((l*lpe)/l.R()/lpe.R()),b=acos((l*hpe)/l.R()/hpe.R());

  //   if(l.R()<
}
/*
#include <TPolyLine.h>*/
void sphericRectangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                            TObject** ident, int lColor, int fColor, int fStyle) const
{
  //  point2D dp[4];
  //  if(eye==plane.Foot())
  //    dp[0].setValues(0,0);
  //  Double_t pox[5],poy[5];
  //  vector4D bb(10000,10000,-10000,-10000);
  //  for(int i=0;i<4;i++)
  //    {
  ////       ps[i]=getPoint(i);
  ////       ps[i]=plane-sLine3D(eye,ps[i]-eye);
  ////       dp[i].setValues(plane.Direction(0)*ps[i],plane.Direction(1)*ps[i]);
  //      dp[i]=projector::project(getPoint(i));
  //      pox[i]=dp[i].X();
  //      poy[i]=dp[i].Y();
  //      if(bb.X()>pox[i])bb.setValue(0,pox[i]);
  //      if(bb.Y()>poy[i])bb.setValue(1,poy[i]);
  //      if(bb.Z()<pox[i])bb.setValue(2,pox[i]);
  //      if(bb.W()<poy[i])bb.setValue(3,poy[i]);
  //    }
  //  pox[4]=dp[0].X();
  //  poy[4]=dp[0].Y();
  //  TPolyLine pl(5,pox,poy);
  //  pl.SetFillColor(fColor);
  //  pl.SetLineColor(lColor);
  //  pl.SetFillStyle(fStyle);
  //  pl.DrawPolyLine(5,pox,poy,"f");
  //  if(ident!=NULL)(*ident)=pl.Clone();
  //  if(boundingBox!=NULL)*boundingBox=bb;
}

void sphericRectangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                            int lColor, int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
shape_parameter sphericRectangle::description() const
{
  shape_parameter sh;
  sh.setName("sphericRectangle");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(center, "center");
  sh.addParam<point3D>(A_, "A");
  sh.addParam<point3D>(B_, "B");
  sh.addParam<point3D>(C_, "C");
  sh.addParam<point3D>(D_, "D");
  return sh;
}
shape_parameter sphericRectangle::getDescription()
{
  shape_parameter sh;
  sh.setName("sphericRectangle");
  sh.addParam<point3D>(point3D(), "center");
  sh.addParam<point3D>(point3D(), "A");
  sh.addParam<point3D>(point3D(), "B");
  sh.addParam<point3D>(point3D(), "C");
  sh.addParam<point3D>(point3D(), "D");
  sh.setCompleteWrite(false);
  return sh;
}
