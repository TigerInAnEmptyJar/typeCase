#include "circle.h"
#include "shapeparameter.h"
circle::circle(point3D cen, vector3D nor, float rIn):planeShape("circle")
{
  center=cen;
  normal=nor;
  rad=rIn;
  circRadius=rIn;
}
circle::circle(const shape_parameter &description):planeShape("circle")
{
  if(description.getName()!="circle")return;
  if(description.NumberOfParams<point3D>() <1||
     description.NumberOfParams<vector3D>() <1||
     description.NumberOfParams<float>() <1)return;
  center=description.getParam<point3D>(0);
  normal=description.getParam<vector3D>(0);
  rad=description.getParam<float>(0);
  circRadius=rad;
}
circle::~circle()
{
}
int circle::getNumberOfPoints()const
{
  return 0;
}
point3D circle::getPoint(int num)const
{
  if(num==0)return center;
  return point3D();
}
float circle::area()const
{
  return 2*rad*M_PI;
}
float circle::angularRange_Phi(const point3D &origin, const vector3D &zDirection)const
{
  float dist=((origin-center)*zDirection);
  float xy=((origin-center)-(zDirection*dist)).length();
  return atan((xy-rad)/dist)-atan((xy+rad)/dist);
}

float circle::angularRange_Theta(const point3D &origin, const vector3D &zDirection)const
{
  float dist=((origin-center)*zDirection);
  float xy=((origin-center)-(zDirection*dist)).length();
  return atan((xy-rad)/dist)-atan((xy+rad)/dist);
}
float circle::radius()const
{
  return rad;
}
void circle::setRadius(float rIn)
{
  rad=rIn;
}

vector3D circle::distancePlane(const plane3D &p)
{
  if((p.Normal()*normal)-1<=__prec)
    return p&center;
  vector3D v=p&center;
  float m=v*normal;
  vector3D vv=normal*m;
  vector3D v2=v-vv;
  if(v2.length()<rad)return vector3D(0,0,0);
  return v-(v*((v2*v)/v.length()));
}
vector3D circle::distance(const sLine3D &line)
{
  point3D hit(getPlane()-line);
  vector3D l=hit-center;
  if(l.R()<=rad)return vector3D(0,0,0);
  return l*(l.R()-rad);
}

//#include <TPolyLine.h>
void circle::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, TObject **ident, int lColor, int fColor, int fStyle)const
{
//  double angle=plane.Normal()*normal;
//  if(angle<__prec)return;
//  vector3D viewDir=plane.Foot()-eye;
//  viewDir.normalize();
//  if(angle==1)
//    {
//      point3D pc=plane-sLine3D(eye,center-eye);
//      vector2D pcv(plane.Direction(0)*pc, plane.Direction(1)*pc);
//      float d1=(eye-pc).R(),d2=(eye-center).R();
//      int numPoints=100;
////       point3D ps[numPoints];
//      point2D dp[numPoints];
//      Double_t pox[numPoints+1],poy[numPoints+1];
//      vector3D start(plane.Direction(0));
//      start.normalize();
//      matrix3D turn(normal,M_PI*2./(double)numPoints);
//      for(int i=0;i<numPoints;i++)
//	{
//	  dp[i]=projector::project(center+start*rad);//ps[i]=center+start*rad;
//	  //ps[i]=plane-sLine3D(eye,ps[i]-eye);
//	  //dp[i].setValues(plane.Direction(0)*ps[i],plane.Direction(1)*ps[i]);
//	  pox[i]=dp[i][0];
//	  poy[i]=dp[i][1];
//	  start=turn*start;
//	}
//      pox[numPoints]=pox[0];
//      poy[numPoints]=poy[0];
//      TPolyLine pl(numPoints+1,pox,poy);
//      pl.SetFillColor(fColor);
//      pl.SetLineColor(lColor);
//      pl.SetFillStyle(fStyle);
//      pl.DrawPolyLine(numPoints+1,pox,poy,"f");
//      if(ident!=NULL)(*ident)=pl.Clone();
//      if(boundingBox!=NULL)
//	boundingBox->setValues(pcv.X()-rad*(d1/(d1+d2)),pcv.Y()-rad*(d1/(d1+d2)),pcv.Y()+rad*(d1/(d1+d2)),pcv.Y()+rad*(d1/(d1+d2)));
//    }
//  else
//    {
//      int numPoints=100;
////       point3D ps[numPoints];
//      point2D dp[numPoints];
//      Double_t pox[numPoints+1],poy[numPoints+1];
//      vector3D start(plane.Direction(0));
//      start.normalize();
//      matrix3D turn(normal,M_PI*2./(double)numPoints);
//      for(int i=0;i<numPoints;i++)
//	{
//	  dp[i]=projector::project(center+start*rad);//getPoint(i)+v[i]*(((double)j)/((double)nPointsPerEdge)))	  ps[i]=center+start*rad;
//	  //ps[i]=plane-sLine3D(eye,ps[i]-eye);
//	  //dp[i].setValues(plane.Direction(0)*ps[i],plane.Direction(1)*ps[i]);
//	  pox[i]=dp[i][0];
//	  poy[i]=dp[i][1];
//	  start=turn*start;
//	}
//      pox[numPoints]=pox[0];
//      poy[numPoints]=poy[0];
//      TPolyLine pl(numPoints+1,pox,poy);
//      pl.SetFillColor(fColor);
//      pl.SetLineColor(lColor);
//      pl.SetFillStyle(fStyle);
//      pl.DrawPolyLine(numPoints+1,pox,poy,"f");
//      if(ident!=NULL)(*ident)=pl.Clone();
//      point3D pc=plane-sLine3D(eye,center-eye);
//      vector2D pcv(plane.Direction(0)*pc, plane.Direction(1)*pc);
//      float d1=(eye-pc).R(),d2=(eye-center).R();
//      vector3D n=normal-plane.Normal()*(normal*plane.Normal());
////       float m1=n*plane.Direction(0);
//      if(boundingBox!=NULL)
//	{
//	  boundingBox->setValues(pcv.X()-rad*(d1/(d1+d2)),pcv.Y()-rad*(d1/(d1+d2)),pcv.Y()+rad*(d1/(d1+d2)),pcv.Y()+rad*(d1/(d1+d2)));
//	}
//    }
}
void circle::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor, int fStyle)const
{
  Draw(eye,plane,boundingBox,NULL,lColor,fColor,fStyle);
}
shape_parameter circle::description()const
{
  shape_parameter sh;
  sh.setName("circle");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(center,"center");
  sh.addParam<vector3D>(normal,"normal");
  sh.addParam<float>(rad,"radius");
  return sh;
}
shape_parameter circle::getDescription()
{
  shape_parameter sh;
  sh.setName("circle");
  sh.addParam<point3D>(point3D(),"center");
  sh.addParam<vector3D>(vector3D(),"normal");
  sh.addParam<float>(0,"radius");
  sh.setCompleteWrite(false);
  return sh;
}
