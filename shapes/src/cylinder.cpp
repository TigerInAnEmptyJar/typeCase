#include "shapes.h"
#include "planeShape.h"
#include "shapeparameter.h"
point3D cylinder::entrance(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  if(norm.getState()!=_zero_)
    return enter;
  return point3D();
}    
	
cylinder::cylinder(const volumeShape &s):volumeShape("cylinder")
{
    if((s.getName()!="cylinder")&&(s.getName()!="wedge")&&(s.getName()!="spiral")&&(s.getName()!="circular")) return;
    center=s.getCenter();
    if(s.getName()=="cylinder") 
    {
	direction=((cylinder)s).getDirection();
	radius=((cylinder)s).getRadius();
    }
    else if(s.getName()=="wedge")
    {
	direction=((wedge)s).getNormal();
	radius=((wedge)s).getOuterRadius();
    }
    else if(s.getName()=="spiral")
    {
	direction=((spiral)s).getNormal();
	radius=((spiral)s).getOuterRadius();
    }
//     else if(s.getName()=="circular")
//     {
// 	direction=((circular_envelope)s).getNormal();
// 	radius=((circular_envelope)s).getOuterRadius();
//     }
}
cylinder::cylinder(const shape_parameter &description):volumeShape("cylinder")
{
  if(description.getName()!="cylinder")return;
  if(description.NumberOfParams<point3D>() <1||
     description.NumberOfParams<vector3D>() <1||
     description.NumberOfParams<float>() <1)return;
  center=description.getParam<point3D>(0);
  direction=description.getParam<vector3D>(0);
  radius=description.getParam<float>(0);
}

void cylinder::operator=(const volumeShape &s)
{
    if((s.getName()!="cylinder")&&(s.getName()!="wedge")&&(s.getName()!="spiral")&&(s.getName()!="circular")) return;
    center=s.getCenter();
    if(s.getName()=="cylinder") 
    {
	direction=((cylinder)s).getDirection();
	radius=((cylinder)s).getRadius();
    }
    else if(s.getName()=="wedge")
    {
	direction=((wedge)s).getNormal();
	radius=((wedge)s).getOuterRadius();
    }
    else if(s.getName()=="spiral")
    {
	direction=((spiral)s).getNormal();
	radius=((spiral)s).getOuterRadius();
    }
//     else if(s.getName()=="circular")
//     {
// 	direction=((circular_envelope)s).getNormal();
// 	radius=((circular_envelope)s).getOuterRadius();
//     }
}

Vector cylinder::HitParams(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  vector3D pitch;
  vector3D npc;
  vector3D dist_;
  float sigma=hit.getValue(9);
  pitch=enter-(center-direction*0.5);
  pitch.normalize();
//   npc=enter+pitch*(sigma-r);

  npc=enter+dist;
  Vector ret(13);
  ret.setValue(0,sigma);
  ret.setValue(1,enter.X());
  ret.setValue(2,enter.Y());
  ret.setValue(3,enter.Z());
  ret.setValue(4,pitch.X());
  ret.setValue(5,pitch.Y());
  ret.setValue(6,pitch.Z());
  ret.setValue(7,npc.X());
  ret.setValue(8,npc.Y());
  ret.setValue(9,npc.Z());
  ret.setValue(10,dist.X());
  ret.setValue(11,dist.Y());
  ret.setValue(12,dist.Z());
  return ret;
}
   
vector3D cylinder::distance(const sLine3D &l)
{
  Vector hit(Hitting(l));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  return dist;
}

sLine3D cylinder::Normal(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  if(norm.getState()!=_zero_)
    return sLine3D(enter,norm);
  return sLine3D();
}
volumeShape* cylinder::getClone()
{
    cylinder *sh=new cylinder(*this);
    sh->setMaxDistance(getMaxDistance());
    return sh;
}
cylinder::cylinder(point3D c, vector3D d, float v):volumeShape("cylinder")
{
    center=c;
    direction=d;
    radius=v;
}
cylinder::~cylinder()
{
}

vector3D cylinder::getDirection(int num) const
{
  if(num==0)
    return direction;
  return direction;
}

lLine3D cylinder::getLine() const
{
    vector3D h(direction*0.5);
    return lLine3D(center-h,center+h);
}

float cylinder::getRadius()const
{
    return radius;
}

void cylinder::setLine(const lLine3D &l)
{
    direction=l.P()-l.Q();
    center=l.P()-direction*0.5;
}

void cylinder::setLine(point3D p, point3D q)
{
    direction=p-q;
    center=p-direction*0.5;
}

void cylinder::setLine(point3D c, vector3D d)
{
    direction=d;
    center=c;
}

void cylinder::setRadius(float r)
{ 
    radius=(r<0?-r:r);
}

#include "fiber.h"
bool cylinder::cut(const fiber &f){if(f.getCorner()==center)return true;return false;}
volumeShape *cylinder::getNext(int times,int stackType){if(times==stackType)return this;return this;}
volumeShape *cylinder::getEnvelope(int stackType){if(stackType==0)return this;return this;}

Vector cylinder::Hitting(const sLine3D &line)
{
  Vector ret(10);
  float sigma;
  //check front / back plane
  plane3D plate;
  point3D c;
  vector3D no=direction;
  no.normalize();
  if(no*line.Direction()<0)
    {
      plate=plane3D(center+0.5*direction,no);
      c=center+0.5*direction;
    }
  else
    {
      plate=plane3D(center-0.5*direction,no*(-1));
      c=center-0.5*direction;
    }
  point3D platePoint=plate-line;
  vector3D ctopp=platePoint-c;
  float r_=ctopp.length();
  sigma=radius;
  if((r_<=radius))
    {
      ret.setValues(platePoint.X(),platePoint.Y(),platePoint.Z(),0,0,0,plate.Normal().X(),plate.Normal().Y(),plate.Normal().Z(),sigma);
      return ret;
    }
  //check if hit on curved area
  float n=(c-line.Foot())*(c-line.Foot())-radius*radius;
  float m=-2*(vector3D(c)*line.Direction()+vector3D(line.Foot())*line.Direction());
  float o=(line.Direction()*line.Direction())*(-1);
  float l=m*m-4*o*n;
  if(l>0)
    {
      float t1=-m+sqrt(l)/2/o,t2=-m-sqrt(l)/2/o;
      point3D p;
      if(t1<t2)
	p=line.Foot()+t2*line.Direction();
      else
	p=line.Foot()+t1*line.Direction();
      vector3D v=c-p;
      vector3D v1_=v-(v*no)*no;
      vector3D v2_=v-v1_;
      v1_.normalize();
      if(v2_*no<0&&v2_.length()<=direction.length())
	{
	  ret.setValues(p.X(),p.Y(),p.Z(),0,0,0,v1_.X(),v1_.Y(),v1_.Z(),0.5*direction.length());
	  return ret;
	}
    }
  //no hit => calculate distance
  //distance to front / back plane shape
  vector3D dist1;
  if(radius<r_)
    dist1=ctopp*((radius-r_)/r_);
  else
    dist1=ctopp*((radius-r_)/r_);
  //distance to curved area
  vector3D dist2;
  lLine3D ml(center-direction*0.5,center+direction*0.5);
  lLine3D conn=(line-ml);
  dist2=(conn).Direction();
  float d=dist2.length()-radius;
  if(dist1.length()<d||d<0)
    ret.setValues(platePoint.X(),platePoint.X(),platePoint.X(),dist1.X(),dist1.Y(),dist1.Z(),0,0,0,sigma);
  else
    ret.setValues(conn.P().X(),conn.P().X(),conn.P().Y(),dist2.X(),dist2.Y(),dist2.Z(),0,0,0,sigma);
  return ret;
}
Vector cylinder::HitParams(const planeShape &shape, point3D origin)
{
  sLine3D cline(origin,shape.getCenter()-origin);
  Vector hit(Hitting(cline));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  point3D enter;
  vector3D norm;
  point3D npc;
  vector3D atm(res);
  float sigma=hit.getValue(9);
  if(dist.length()<__prec)
    {
      enter.setValues(hit.getValue(0),hit.getValue(1),hit.getValue(2));
      norm.setValues(hit.getValue(6),hit.getValue(7),hit.getValue(8));
      npc=enter+dist;
    }
  else
    {
      float dp=shape.angularRange_Phi(origin);
      float dt=shape.angularRange_Theta(origin);
      vector3D e(hit.getValue(0),hit.getValue(1),hit.getValue(2));
      vector3D n=e+dist;
      e=e-origin;
      n=n-origin;
      float dp1=abs(e.Phi()-n.Phi()),dt1=abs(e.Theta()-n.Theta());
      if(dp1<dp&&dt1<dt)
	{
	  int numCorners=shape.getNumberOfPoints();
	  point3D corners[numCorners];
	  vector3D dists[numCorners];
	  point3D npcs[numCorners];
	  point3D enters[numCorners];
	  vector3D norms[numCorners];
	  bool init=false;
	  for(int i=0;i<numCorners;i++)
	    {
	      corners[i]=shape.getPoint(i);
	    }
	  for(int i=0;i<numCorners;i++)
	    {
	      sLine3D lin(origin,corners[i]-origin);
	      Vector hit1(Hitting(lin));
	      dists[i].setValues(hit1.getValue(3),hit1.getValue(4),hit1.getValue(5));
	      enters[i].setValues(hit1.getValue(0),hit1.getValue(1),hit1.getValue(2));
	      npcs[i]=enters[i]+dists[i];
	      norms[i].setValues(hit1.getValue(6),hit1.getValue(7),hit1.getValue(8));
	      if(dists[i].length()<__prec)
		{
		  enter=enters[i];
		  npc=npcs[i];
		  norm=norms[i];
		  dist=dists[i];
		  init=true;
		  break;
		}
	    }
	  if(!init)
	    {
	      bool passing=false;
	      for(int i=0;i<numCorners;i++)
		for(int j=0;j<numCorners;j++)
		  {
		    if(dists[i]*dists[j]<0)
		      passing=true;
		  }
	      for(int i=0;i<numCorners;i++)
		{
		  if(dist.length()>dists[i].length())
		    {
		      dist=dists[i];
		      enter=enters[i];
		      norm=norms[i];
		      npc=npcs[i];
		    }
		}
	      if(passing)
		{
		  dist.setValues(0,0,0);
		}
	    }
	}
      else
	{
	  enter.setValues(hit.getValue(0),hit.getValue(1),hit.getValue(2));
	  norm.setValues(hit.getValue(6),hit.getValue(7),hit.getValue(8));
	  npc=enter+dist;
	}
    }
  //  if(((enter-center)^(matrix3D(normal,-dphi/2)*lpe))*normal>0) atm=atm*(-1);

  atm.normalize();
  Vector ret(13);
  ret.setValue(0,sigma);
  ret.setValue(1,enter.X());
  ret.setValue(2,enter.Y());
  ret.setValue(3,enter.Z());
  ret.setValue(4,atm.X());
  ret.setValue(5,atm.Y());
  ret.setValue(6,atm.Z());
  ret.setValue(7,npc.X());
  ret.setValue(8,npc.Y());
  ret.setValue(9,npc.Z());
  ret.setValue(10,dist.X());
  ret.setValue(11,dist.Y());
  ret.setValue(12,dist.Z());
  return ret;
}
//#include "circle.h"
//#include "quadrangle.h"
void cylinder::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, TObject **ident, int lColor, int fColor, int fStyle)const
{
  vector3D viewDir=plane.Foot()-eye;
  viewDir.normalize();
  if(viewDir*direction==0)
    {
      vector3D v2;
      v2=direction^plane.Normal();
      v2.normalize();
      quadrangle q(center+direction+v2*radius, center+direction-v2*radius, center-v2*radius, center+v2*radius);
      q.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
    }
  if((viewDir*direction)/direction.R()==1)
    {
      circle c(center-direction,direction*-1,radius);
      c.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
    }
  if(viewDir*direction>0)
    {
      circle c(center-direction,direction*-1,radius);
      vector3D v2;
      v2=direction^plane.Normal();
      v2.normalize();
      quadrangle q(center+direction+v2*radius, center+direction-v2*radius, center-v2*radius, center+v2*radius);
      if(radius>v2.R())
	{
	  c.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
	  q.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
	}
      else
	{
	  c.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
	  q.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
	}
    }
  else
    {
      circle c(center+direction,direction,radius);
      vector3D v2;
      v2=direction^plane.Normal();
      v2.normalize();
      quadrangle q(center+direction+v2*radius, center+direction-v2*radius, center-v2*radius, center+v2*radius);
      if(radius>v2.R())
	{
	  c.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
	  q.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
	}
      else
	{
	  c.Draw(eye,plane,boundingBox,lColor,fColor,fStyle);
	  q.Draw(eye,plane,boundingBox,ident,lColor,fColor,fStyle);
	}
    }
}
void cylinder::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor, int fStyle)const
{
  Draw(eye,plane,boundingBox,NULL,lColor,fColor,fStyle);
}
shape_parameter cylinder::description()const
{
  shape_parameter sh;
  sh.setName("cylinder");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(center,"center");
  sh.addParam<vector3D>(direction,"direction");
  sh.addParam<float>(radius,"radius");
  return sh;
}
shape_parameter cylinder::getDescription()
{
  shape_parameter sh;
  sh.setName("cylinder");
  sh.addParam<point3D>(point3D(),"center");
  sh.addParam<vector3D>(vector3D(),"direction");
  sh.addParam<float>(0,"radius");
  sh.setCompleteWrite(false);
  return sh;
}
