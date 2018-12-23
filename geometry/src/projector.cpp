#include "geometry.h"
projector::projector(){}
projector::~projector(){}
void projector::setProjector(const plane3D &plane, const point3D &point, int mode )
{
  fplane=plane;
  feye=point;
  fopticalAxis=fplane.Foot()-feye;
  fopticalAxis.normalize();
  if(mode==predefined)
    fmode=central;
  else fmode=mode;
}
point2D projector::project(const point3D & point, int mode)
{
  if(mode==predefined)mode=fmode;
  //  enum {parallel = 0, central = 1, fishEye = 2, predefined = 3};
  switch(mode)
    {
    case 0:
      {
	vector3D dist=fplane & point;
	point3D pIn=point-dist;
	float x1,y1;
	x1=fplane.Direction(0)*vector3D(pIn);
	y1=fplane.Direction(1)*vector3D(pIn);
	return point2D(x1,y1);
      }
    case 1:
      {
	sLine3D line(feye,point-feye);
	point3D pIn=fplane-line;
	float x1,y1;
	x1=fplane.Direction(0)*vector3D(pIn);
	y1=fplane.Direction(1)*vector3D(pIn);
	return point2D(x1,y1);
      }
    case 2:
      {
	vector3D dir=point-feye;
	float th=(dir*fopticalAxis)/dir.R()/fopticalAxis.R();
	sLine3D line(feye,point-feye);
	point3D pIn=fplane-line;
	float x1,y1;
	x1=fplane.Direction(0)*vector3D(pIn);
	y1=fplane.Direction(1)*vector3D(pIn);
	x1=x1*(1+sin(th));
	y1=y1*(1+sin(th));
	return point2D(x1,y1);
      }
    }
  return point2D();
}
point2D projector::project(const plane3D &plane, const point3D &eye, const point3D &point, int mode)
{
  if(mode==predefined)mode=fmode;
  //enum {parallel = 0, central = 1, fishEye = 2, predefined = 3};
  switch(mode)
    {
    case 0:
      {
	vector3D dist=plane & point;
	point3D pIn=point-dist;
	float x1,y1;
	x1=plane.Direction(0)*vector3D(pIn);
	y1=plane.Direction(1)*vector3D(pIn);
	return point2D(x1,y1);
      }
    case 1:
      {
	sLine3D line(eye,point-eye);
	point3D pIn=plane-line;
	float x1,y1;
	x1=plane.Direction(0)*vector3D(pIn);
	y1=plane.Direction(1)*vector3D(pIn);
	return point2D(x1,y1);
      }
    case 2:
      {
	vector3D dir=point-eye;
	vector3D opticalAxis=plane.Foot()-eye;
	float th=(dir*opticalAxis)/dir.R()/opticalAxis.R();
	sLine3D line(eye,point-eye);
	point3D pIn=plane-line;
	float x1,y1;
	x1=plane.Direction(0)*vector3D(pIn);
	y1=plane.Direction(1)*vector3D(pIn);
	x1=x1*(1+sin(th/2.));
	y1=y1*(1+sin(th/2.));
	return point2D(x1,y1);
      }
    }
  return point2D();
}


int projector::fmode = projector::central;
point3D projector::feye = point3D(0,0,0);
plane3D projector::fplane = plane3D(point3D(0,0,1), vector3D(1,0,0), vector3D(0,1,0));;
vector3D projector::fopticalAxis = vector3D(0,0,1);
