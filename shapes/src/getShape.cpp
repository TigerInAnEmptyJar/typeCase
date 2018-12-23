#include "shapes.h"
#include "shapeparameter.h"

base_shape *getShape(const shape_parameter&sp)
{
    base_shape*shape;
    if(sp.getName()=="wedge")
      {
	if(sp.NumberOfParams<float>()>4)
	  shape=new wedge(sp.getParam<float>(0),sp.getParam<float>(1),
			  sp.getParam<float>(2),sp.getParam<float>(3),
			  sp.getParam<point3D>(0),sp.getParam<vector3D>(0),
			  sp.getParam<vector3D>(1),sp.getParam<float>(4));
	else
	  shape=new wedge(sp.getParam<float>(0),sp.getParam<float>(1),
			  sp.getParam<float>(2),sp.getParam<float>(3),
			  sp.getParam<point3D>(0),sp.getParam<vector3D>(0),
			  sp.getParam<vector3D>(1),-1);
      }
    else if(sp.getName()=="wedge-cone")
      {
	point3D centerOfBase=sp.getParam<point3D>(0);
	vector3D symmetryAxis=sp.getParam<vector3D>(0);
	vector3D lpOnBase=sp.getParam<vector3D>(1);
	float baseRadius=sp.getParam<float>(0);
	float topRadius=sp.getParam<float>(1);
	float length=sp.getParam<float>(2);
	float dp=sp.getParam<float>(3);
	float thickness=sp.getParam<float>(4);
	float Ro,Ri,z=length*baseRadius/(baseRadius-topRadius);
	Ro=sqrt(z*z+baseRadius*baseRadius);
	Ri=Ro*topRadius/baseRadius;
	point3D center=centerOfBase-symmetryAxis*(z/symmetryAxis.R());
	vector3D lpe=symmetryAxis*(z/symmetryAxis.R())+lpOnBase*(baseRadius/lpOnBase.R());
	lpOnBase=matrix3D(symmetryAxis,dp*0.5)*lpOnBase;
	vector3D normal=(symmetryAxis*baseRadius-lpOnBase*z)/(Ro);
	lpe.normalize();
	normal.normalize();
	//	float alpha=2*asin(baseRadius/Ro*sin(dp*0.5));
	shape=new wedge(Ri,Ro,dp,thickness,center,normal,lpe,baseRadius);
      }
    else if(sp.getName()=="cylinder")
	shape=new cylinder(sp.getParam<point3D>(0),sp.getParam<vector3D>(0),
			   sp.getParam<float>(0));
    else if(sp.getName()=="spiral")
	shape=new spiral(sp.getParam<float>(0),sp.getParam<float>(1),
			 sp.getParam<float>(4),sp.getParam<float>(2),
			 sp.getParam<float>(3),sp.getParam<point3D>(0),
			 sp.getParam<vector3D>(0),sp.getParam<vector3D>(1));
    else if(sp.getName()=="fiber")
      {
	vector3D shift=0.5*sp.getParam<vector3D>(0)+sp.getParam<vector3D>(1)*(sp.getParam<int>(1)+0.5*sp.getParam<int>(0));
	shape=new fiber(sp.getParam<point3D>(0)-shift,//0.5*sp.getParam<vector3D>(0)-sp.getParam<vector3D>(1)*(sp.getParam<int>(1)+((int)(1.5*sp.getParam<int>(0)))),
		      sp.getParam<vector3D>(0),	sp.getParam<vector3D>(1),sp.getParam<vector3D>(2),
		      sp.getParam<int>(0),sp.getParam<int>(1));
      }
    else  if(sp.getName()=="hexPrism")
	shape=new hexPrism(sp.getParam<point3D>(0), sp.getParam<vector3D>(0),
			   sp.getParam<vector3D>(1), sp.getParam<vector3D>(2),
			   sp.getParam<int>(0));
    else  if(sp.getName()=="ring")
	shape=new ring(sp.getParam<point3D>(0),sp.getParam<vector3D>(0),
		       sp.getParam<float>(0), sp.getParam<float>(0)+sp.getParam<float>(1),
		       sp.getParam<float>(2));
    else if(sp.getName()=="strawTube")
    shape=new strawTube(sp.getParam<point3D>(0),
        sp.getParam<vector3D>(0), sp.getParam<float>(0),
        sp.getParam<vector3D>(1), sp.getParam<vector3D>(2),
        sp.getParam<float>(1), sp.getParam<int>(0),
        sp.getParam<int>(1));
    else if(sp.getName()=="sphericRectangle")
      shape=new sphericRectangle(sp.getParam<point3D>(0),sp.getParam<vector3D>(0),
				 sp.getParam<vector3D>(1),sp.getParam<float>(0),
				 sp.getParam<float>(1), sp.getParam<float>(2));
    else if(sp.getName()=="quadrangle")
      shape=new quadrangle(sp.getParam<point3D>(0), sp.getParam<point3D>(1), 
			   sp.getParam<point3D>(2), sp.getParam<point3D>(3));
    else if(sp.getName()=="triangle")
      shape=new triangle(sp.getParam<point3D>(0), sp.getParam<point3D>(1),
			 sp.getParam<point3D>(2));
    else if(sp.getName()=="sphericTriangle")
      shape=new sphericTriangle(sp.getParam<point3D>(0), sp.getParam<point3D>(1),
				sp.getParam<point3D>(2), sp.getParam<point3D>(3));
    else if(sp.getName()=="rectangle")
      shape=new rectangle(sp.getParam<point3D>(0), sp.getParam<point3D>(1),
			  sp.getParam<vector3D>(0), sp.getParam<float>(0));
    else if(sp.getName()=="hexagon")
      shape=new hexagon(sp.getParam<point3D>(0),sp.getParam<point3D>(1),sp.getParam<vector3D>(0));
    else if(sp.getName()=="circle")
      shape=new circle(sp.getParam<point3D>(0), sp.getParam<vector3D>(0), sp.getParam<float>(0));
    else	
	shape=new base_shape("none");
    return shape;

}


