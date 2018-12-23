#include "baseShape.h"
#include "shapeparameter.h"

vector3D base_shape::distance(const sLine3D &line)
{
    return vector3D(line.Foot());
}
void base_shape::setName(string nme)
{
  fname=nme;
}

sLine3D base_shape::Normal(const sLine3D &line)
{
    return line;
}

Vector base_shape::HitParams(const sLine3D &line)
{
  return Vector(line.Foot());
}
#include "fiber.h"
bool base_shape::cut(const fiber &f)
{
  if(f.getCorner().getState()==_undefined_)
    return false;
  return false;
}
point3D base_shape::entrance(const sLine3D &line)
{
  return point3D(line.Foot());
}
base_shape::base_shape(string n)
{
    center.setValues(0,0,0);
    setName(n);
}

base_shape::base_shape(const base_shape &s)
{
    setName(s.getName());
    center=s.getCenter();
}

void base_shape::setCenter(point3D p)
{
    center=p;
}

string base_shape::getName() const
{
    return fname;
}

point3D base_shape::getCenter() const
{
    return center;
}
void base_shape::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor, int fStyle)const
{
  if(eye==plane.Foot())return;
  if(lColor==fColor&&lColor==fStyle&&boundingBox==NULL)return;
}
void base_shape::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, TObject **ident, int lColor, int fColor, int fStyle)const
{
  if(eye==plane.Foot())return;
  if(lColor==fColor&&lColor==fStyle&&boundingBox==NULL)return;
  if(ident==NULL)return;
}
shape_parameter base_shape::description()const
{
  shape_parameter sh;
  sh.setName(fname);
  return sh;
}
shape_parameter base_shape::getDescription()
{
  shape_parameter sh;
  sh.setName("shape");
  return sh;
}
/*#include <TPolyLine.h>
void DrawPoints(int nPoints, point3D* points,const point3D &eye, const plane3D &plane, vector4D* boundingBox, TObject **ident, int lColor, int fColor, int fStyle)
{
  int nPointsPerEdge=20;
  if(eye==plane.Foot())nPointsPerEdge=20;
  //float d=(eye-plane.Foot()).R();
  vector3D v[nPoints];for(int i=0;i<nPoints-1;i++)v[i]=points[i+1]-points[i];
  v[nPoints-1]=points[0]-points[nPoints-1];
  point2D dp[nPoints*nPointsPerEdge];
  Double_t pox[nPoints*nPointsPerEdge+1],poy[nPoints*nPointsPerEdge+1];
  for(int i=0;i<nPoints;i++)
    {
      for(int j=0;j<nPointsPerEdge;j++)
	{
 	  dp[i*nPointsPerEdge+j]=projector::project(points[i]+v[i]*(((double)j)/((double)nPointsPerEdge)));
	  pox[i*nPointsPerEdge+j]=dp[i*nPointsPerEdge+j].X();
	  poy[i*nPointsPerEdge+j]=dp[i*nPointsPerEdge+j].Y();
	}
    }
  if(nPoints>6)
    {
      for(int i=0;i<3;i++)
	{
	  pox[2*i]=dp[i].X();
	  poy[2+i]=dp[i].Y();
	  pox[2*i+1]=dp[i+3].X();
	  poy[2*i+1]=dp[i+3].Y();
	}
    }
  pox[nPoints*nPointsPerEdge]=dp[0].X();
  poy[nPoints*nPointsPerEdge]=dp[0].Y();
  TPolyLine pl(nPoints*nPointsPerEdge+1,pox,poy);
  pl.SetFillColor(fColor);
  pl.SetLineColor(lColor);
  pl.SetFillStyle(fStyle);
  pl.DrawPolyLine(nPoints*nPointsPerEdge+1,pox,poy,"f");
  if(ident!=NULL)(*ident)=pl.Clone();
  if(boundingBox!=NULL)
    {
      boundingBox->setValues(dp[0].X(),dp[0].Y(),dp[0].X(),dp[0].Y());
      for(int i=1;i<nPoints;i++)
	{
	  if(boundingBox->X()>dp[i*nPointsPerEdge].X())boundingBox->setValue(0,dp[i*nPointsPerEdge].X());
	  if(boundingBox->Y()>dp[i*nPointsPerEdge].Y())boundingBox->setValue(1,dp[i*nPointsPerEdge].Y());
	  if(boundingBox->Z()<dp[i*nPointsPerEdge].X())boundingBox->setValue(2,dp[i*nPointsPerEdge].X());
	  if(boundingBox->W()<dp[i*nPointsPerEdge].Y())boundingBox->setValue(3,dp[i*nPointsPerEdge].Y());
	}
    }

}
*/
