#include "spiral.h"
#include "wedge.h"
#include "planeShape.h"
#include "shapeparameter.h"
spiral::spiral(float i, float a, float b, float d,float t, point3D c, vector3D n, vector3D l):volumeShape("spiral")
{
    innerR=i;
    outerR=a;
    bending=b;
    dphi=d;
    thickness=t;
    center.setValues(c.X(),c.Y(),c.Z());
    normal.setValues(n.X(),n.Y(),n.Z());
    normal.normalize();
    lpe.setValues(l.X(),l.Y(),l.Z());
    hpe=matrix3D(normal,dphi)*lpe;
}

spiral::spiral(const volumeShape &s):volumeShape("spiral")
{
    if((s.getName()!="spiral")&&(s.getName()!="circular")&&(s.getName()!="wedge")) return;
    if(s.getName()=="spiral")
    {
	innerR=((spiral)s).getInnerRadius();
	outerR=((spiral)s).getOuterRadius();
	bending=((spiral)s).getBending();
	dphi=((spiral)s).getPhiRange();
	thickness=((spiral)s).getThickness();
	lpe=((spiral)s).getLowerPhiEdge();
	normal=((spiral)s).getNormal();
    }
    else if(s.getName()=="wedge")
    {
	innerR=((wedge)s).getInnerRadius();
	outerR=((wedge)s).getOuterRadius();
	bending=0;
	dphi=((wedge)s).getPhiRange();
	thickness=((wedge)s).getThickness();
	lpe=((wedge)s).getLowerPhiEdge();
	normal=((wedge)s).getNormal();
    }
    else
    {
// 	innerR=s.getInnerRadius();
// 	outerR=s.getOuterRadius();
// 	bending=s.getBending();
// 	dphi=s.getPhiRange();
// 	thickness=s.getThickness();
	lpe=vector3D(1,0,0);
// 	normal=s.getNormal();
    }
    hpe=matrix3D(normal,dphi)*lpe;
    setMaxDistance(s.getMaxDistance());
    center=s.getCenter();
}
spiral::spiral(const shape_parameter &description):volumeShape("spiral")
{
  if(description.getName()!="spiral")return;
  if(description.NumberOfParams<point3D>()<1||
     description.NumberOfParams<vector3D>()<2||
     description.NumberOfParams<float>()<5)return;
    innerR=description.getParam<float>(0);
    outerR=description.getParam<float>(1);
    bending=description.getParam<float>(4);
    dphi=description.getParam<float>(2);
    thickness=description.getParam<float>(3);
    center=description.getParam<point3D>(0);
    normal=description.getParam<vector3D>(0);
    normal.normalize();
    lpe=description.getParam<vector3D>(1);
    hpe=matrix3D(normal,dphi)*lpe;
}

void spiral::operator=(volumeShape *s)
{
    //cout<<s->getName().ascii()<<endl;
    if((s->getName()!="spiral")&&(s->getName()!="circular")&&(s->getName()!="wedge")) return;
    if(s->getName()=="spiral")
    {
	innerR=((spiral*)s)->getInnerRadius();
	outerR=((spiral*)s)->getOuterRadius();
	bending=((spiral*)s)->getBending();
	dphi=((spiral*)s)->getPhiRange();
	thickness=((spiral*)s)->getThickness();
	lpe=((spiral*)s)->getLowerPhiEdge();
	normal=((spiral*)s)->getNormal();
    }
    else if(s->getName()=="wedge")
    {
	innerR=((wedge*)s)->getInnerRadius();
	outerR=((wedge*)s)->getOuterRadius();
	bending=0;
	dphi=((wedge*)s)->getPhiRange();
	thickness=((wedge*)s)->getThickness();
	lpe=((wedge*)s)->getLowerPhiEdge();
	normal=((wedge*)s)->getNormal();
    }
    else
    {
// 	innerR=(s)->getInnerRadius();
// 	outerR=(s)->getOuterRadius();
// 	bending=(s)->getBending();
// 	dphi=(s)->getPhiRange();
// 	thickness=(s)->getThickness();
// 	normal=(s)->getNormal();
    }
    setMaxDistance(s->getMaxDistance());
    center=s->getCenter();
    hpe=matrix3D(normal,dphi)*lpe;
}

vector3D spiral::distance(const sLine3D &line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  return dist;
}
float spiral::getFlightPathInShape(const sLine3D &line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  if(dist.R()>__prec)
    return 0;
  Vector hit2(Hitting(sLine3D(line.Foot(),line.Direction()*-1)));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  point3D out(hit2.getValue(0),hit2.getValue(1),hit2.getValue(2));
  return (out-enter).R();
}
#include "ring.h"
volumeShape *spiral::getEnvelope(int times, int stackType)
{
  if(stackType==2) return (new spiral(*this));
  if(dphi*times-M_PI*2<-0.1*M_PI)
    {
      spiral *ret=new spiral(*this);
      ret->setPhiRange(dphi*times);
      return ret;
    }
  else
    {
      ring *ret=new ring(center, normal, innerR,outerR,thickness);
      return ret;
    }
  return this;
}
sLine3D spiral::Normal(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  if(norm.getState()!=_zero_)
    return sLine3D(enter,norm);
  return sLine3D();
}
#include "fiber.h"
bool spiral::cut(const fiber &f)
{
  if(f.getCorner()==center)return false;
    return false;
}

Vector spiral::HitParams(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  vector3D pitch;
  vector3D npc;
  vector3D atm;
  float sigma=hit.getValue(9);

  float radius = (enter-center).length();
  matrix3D turn1 (normal, (radius>outerR?outerR:(radius<innerR?innerR:radius))/bending);//-
  vector3D v = turn1 * lpe;

  atm=(center+(v*radius))-enter;
  npc=enter+dist;
  atm.normalize();
  sigma=sigma*fabs(atm*(enter-center));

  vector3D r1=npc-center;
  r1.normalize();
  float cosphi=(lpe*r1);
  v=lpe^r1;
  float sinphi=v.length();
  if(sinphi==0)
    v=normal;
  v.normalize();
  sinphi=sinphi*(v*normal);
  float phi=atan2(cosphi,sinphi);
  pitch=vector3D(-sin(phi)-cos(phi)*phi,cos(phi)-phi*sin(phi),0)*bending;
  cosphi=vector3D(1,0,0)*lpe;
  matrix3D turn_1(normal,acos(cosphi));//-
  pitch=turn_1*pitch;

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
point3D spiral::entrance(const sLine3D &line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D norm(hit.getValue(6),hit.getValue(7),hit.getValue(8));
    return enter;
}
spiral::~spiral()
{
}

float spiral::getInnerRadius()const
{
    return innerR;
}

float spiral::getOuterRadius()const
{
    return outerR;
}

float spiral::getBending() const
{
    return bending;
}

float spiral::getPhiRange() const
{
    return dphi;
}

float spiral::getThickness()const
{
    return thickness;
}

vector3D spiral::getNormal() const
{
    return normal;
}

vector3D spiral::getLowerPhiEdge() const
{
    return lpe;
}

void spiral::setInnerRadius(float v)
{
    innerR=(v<0?-v:v);
}

void spiral::setOuterRadius(float v)
{
    outerR=(v<0?-v:v);
}

void spiral::setBending(float v)
{
    bending=v;
}

void spiral::setPhiRange(float v)
{
    dphi=v;
}

void spiral::setThickness(float t)
{
    thickness=(t<0?-t:t);
}

void spiral::setCenter(point3D c)
{
    center=c;
}
 
void spiral::setNormal(vector3D v)
{
    normal=v;
    normal.normalize();
}

void spiral::setLowerPhiEdge(vector3D v)
{
    lpe=v;
}

volumeShape *spiral::getNext(int times,int stackType)
{
  if(stackType==2) return (new spiral(*this));
    spiral *ret=new spiral(*this);
    matrix3D turn(normal,dphi*((float)times));//-
    vector3D tmp=lpe;
    tmp=turn*tmp;
    ret->setLowerPhiEdge(tmp);
    return ret;
}

volumeShape* spiral::getClone()
{
    spiral *sh=new spiral(*this);
    sh->setMaxDistance(getMaxDistance());
    return sh;
}

//#include <TPolyLine.h>

Vector spiral::Hitting(const sLine3D &line)
{
  //  cout<<"\nspiral::Hitting(): ";
  Vector ret(10);
  float sigma;
  //check front / back plane
  plane3D plate;
  point3D c;
  if(normal*line.Direction()<0)
    {
      plate=plane3D(center,normal);
      c=center;
    }
  else
    {
      plate=plane3D(center-normal*thickness,normal*(-1));
      c=center-normal*thickness;
    }
  if(lpe.isColumn())lpe.transpose();
  point3D platePoint=plate-line;
  vector3D ctopp=platePoint-c;
  //  cout<<"\t"<<platePoint.toString(_cylindric_).ascii();
  matrix3D turnTo(normal,dphi);//-
  vector3D hpe;
  hpe=turnTo*lpe;
  float r=ctopp.R();
  matrix3D turn(normal,r/bending);//-
  vector3D lp1,hp1;
  lp1=turn*lpe;
  hp1=turn*hpe;
  float pl,ph;
  pl=acos(ctopp*lp1/r);
  ph=acos(ctopp*hp1/r);
  sigma=fabs(dphi*bending/2.0);
  //  cout<<"spiral::Hitting():"<<normal<<innerR<<" "<<r<<" "<<outerR<<" "<<pl<<" "<<ph<<endl;
  //  cout<<ctopp.toString(_spheric_).ascii()<<lp1.toString(_spheric_).ascii()<<hp1.toString(_spheric_).ascii()<<endl;
  //  cout<<"\t"<<innerR<<"<"<<r<<"<"<<outerR<<" "<<pl<<" "<<ph<<" "<<dphi<<"|"<<lp1.Phi()<<"<"<<platePoint.Phi()<<"<"<<hp1.Phi()<<endl;
  //  cout<<"\t"<<lpe.Phi()<<"|"<<lp1.Phi()<<" "<<hpe.Phi()<<"|"<<hp1.Phi()<<endl;
  if((r<=outerR)&&(r>=innerR)&&(pl<=dphi)&&(ph<=dphi))
    {
      ret.setValues(platePoint.X(),platePoint.Y(),platePoint.Z(),0,0,0,plate.Normal().X(),plate.Normal().Y(),plate.Normal().Z(),sigma);
      //      cout<<ret<<endl;
      return ret;
    }
  //check side planes  that's a bit more difficult for spirals so we leave that for now
//   vector3D ns1=normal^lpe;
//   ns1.normalize();
//   plane3D side;
//   vector3D dir1;
//   if(ns1*line.Direction()<0)
//     {
//       side=plane3D(center, ns1);
//       dir1=lpe;
//     }
//   else
//     {
//       side=plane3D(center,matrix3D(normal,-dphi)*ns1);
//       dir1=hpe;
//     }
//   point3D sidePoint=side-line;
//   vector3D cTsp=sidePoint-center;
//   vector3D v1=dir1*(dir1*cTsp);
//   vector3D v2=cTsp-v1;
//   float d=v1.length();
//   float t=v2.length();
//   if(d<=outerR&&d>=innerR&&v2*normal<0&&t<=thickness)
//     {
//       ret.setValues(sidePoint.X(),sidePoint.Y(),sidePoint.Z(),0,0,0,side.Normal().X(),side.Normal().Y(),side.Normal().Z(),0.5*thickness);
//       return ret;
//     }
  //check if hit on curved areas
  if((pl<dphi)&&(ph<dphi))
    {
      if (r<innerR)//check if hit from inside
	{
	  float n=(center-line.Foot())*(center-line.Foot())-innerR*innerR;
	  float m=-2*(vector3D(center)*line.Direction()+vector3D(line.Foot())*line.Direction());
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
	      vector3D v=center-p;
	      vector3D v1_=v-(v*normal)*normal;
	      vector3D v2_=v-v1_;
	      v1_.normalize();
	      if(v2_*normal<0&&v2_.length()<=thickness&&
		 (acos(v1_*lp1)<dphi&&
		  (acos(v1_*hp1)<dphi)))
		{
		  ret.setValues(p.X(),p.Y(),p.Z(),0,0,0,v1_.X(),v1_.Y(),v1_.Z(),0.5*thickness);
		  return ret;
		}
	    }
	}
      else if(r>outerR)//check if hit from outside
	{
	  float n=(center-line.Foot())*(center-line.Foot())-outerR*outerR;
	  float m=-2*(vector3D(center)*line.Direction()+vector3D(line.Foot())*line.Direction());
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
	      vector3D v=center-p;
	      vector3D v1_=v-(v*normal)*normal;
	      vector3D v2_=v-v1_;
	      v1_.normalize();
	      if(v2_*normal<0&&v2_.length()<=thickness&&
		 (acos(v1_*lp1)<dphi&&
		  (acos(v1_*hp1)<dphi)))
		{
		  ret.setValues(p.X(),p.Y(),p.Z(),0,0,0,v1_.X(),v1_.Y(),v1_.Z(),0.5*thickness);
		  return ret;
		}
	    }
	}
    }
  //no hit => calculate distance
  //distance to front / back plane shape
  vector3D dist1;
  if((pl<dphi)&&(ph<dphi))
    {
      if(r<innerR)
	dist1=ctopp*((innerR-r)/r);
      else
	dist1=ctopp*((outerR-r)/r);
    }
  else 
    {
      if(pl>dphi)
	dist1=lLine3D(center+hp1*innerR,center+hp1*outerR)-platePoint;
      else
	dist1=lLine3D(center+lp1*innerR,center+lp1*outerR)-platePoint;
    }
  //distance to side plane shape
//   vector3D dist2;
//   if(d<=outerR&&d>=innerR)
//     {
//       if(t<=thickness)
// 	dist2=lLine3D(center+dir1*innerR,center+dir1*outerR)-sidePoint;
//       else
// 	dist2=lLine3D(center+dir1*innerR-normal*thickness,center+dir1*outerR-normal*thickness)-sidePoint;
//     }
//   else
//     {
//       if(v2*normal>0)
// 	{
// 	  if(d>outerR)
// 	    dist2=center+dir1*outerR-sidePoint;
// 	  else
// 	    dist2=center+dir1*innerR-sidePoint;
// 	}
//       else if(t>thickness)
// 	{
// 	  if(d>outerR)
// 	    dist2=center+dir1*outerR-normal*thickness-sidePoint;
// 	  else
// 	    dist2=center+dir1*innerR-normal*thickness-sidePoint;
// 	}
//       else
// 	{
// 	  if(d>outerR)
// 	    dist2=v1-dir1*outerR;
// 	  else
// 	    dist2=v1-dir1*innerR;
// 	}
//     }
//   if(dist1.length()<dist2.length())
    ret.setValues(platePoint.X(),platePoint.Y(),platePoint.Z(),dist1.X(),dist1.Y(),dist1.Z(),0,0,0,sigma);
//     else
//     ret.setValues(sidePoint.X(),sidePoint.Y(),sidePoint.Z(),dist2.X(),dist2.Y(),dist2.Z(),0,0,0,0.5*thickness);
  return ret;
}
Vector spiral::HitParams(const planeShape &shape, point3D origin)
{  
  Vector hit(Hitting(sLine3D(origin,shape.getCenter()-origin)));
  point3D enter;//(hit.getValue(0),hit.getValue(1),hit.getValue(2));
  vector3D dist(hit.getValue(3),hit.getValue(4),hit.getValue(5));
  vector3D norm;//(hit.getValue(6),hit.getValue(7),hit.getValue(8));
  vector3D pitch;
  vector3D npc;
  vector3D atm;
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
	      hit=Vector(Hitting(sLine3D(origin,corners[i]-origin)));
	      dists[i].setValues(hit.getValue(3),hit.getValue(4),hit.getValue(5));
	      enters[i].setValues(hit.getValue(0),hit.getValue(1),hit.getValue(2));
	      npcs[i]=enters[i]+dists[i];
	      norms[i].setValues(hit.getValue(6),hit.getValue(7),hit.getValue(8));
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
  float radius = (enter-center).length();
  matrix3D turn1 (normal, (radius>outerR?outerR:(radius<innerR?innerR:radius))/bending);//-
  vector3D v = turn1 * lpe;

  atm=(center+(v*radius))-enter;
  npc=enter+dist;
  atm.normalize();
  sigma=sigma*(atm*(enter-center));

  vector3D r1=npc-center;
  r1.normalize();
  float cosphi=(lpe*r1);
  v=lpe^r1;
  float sinphi=v.length();
  if(sinphi==0)
    v=normal;
  v.normalize();
  sinphi=sinphi*(v*normal);
  float phi=atan2(cosphi,sinphi);
  pitch=vector3D(-sin(phi)-cos(phi)*phi,cos(phi)-phi*sin(phi),0)*bending;
  cosphi=vector3D(1,0,0)*lpe;
  matrix3D mmt(normal,acos(cosphi));//-
  pitch=mmt*pitch;

  //  pitch=atm;
  //  cout<<"spiral: "<<enter<<dist<<norm<<npc<<pitch<<endl;
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

void spiral::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, TObject **ident, int lColor, int fColor, int fStyle)const
{
/*  int numberOfPointsPerEdge=100;
  if(eye==plane.Foot()) numberOfPointsPerEdge=100;
  point3D **edges=new point3D*[8];
  for(int i=0;i<8;i++)edges[i]=new point3D[numberOfPointsPerEdge];
  point2D **edges2=new point2D*[8];
  for(int i=0;i<8;i++)edges2[i]=new point2D[numberOfPointsPerEdge];
  matrix3D turnSt(normal,innerR/bending);//-
  matrix3D turnSt1(normal,innerR/bending+dphi);//-
  matrix3D turn_i(normal,innerR/(bending));//-
  matrix3D turn_o(normal,outerR/(bending));//-
  vector3D tmp1=turnSt*lpe;
  vector3D tmp2=turnSt1*lpe;
  vector3D tmp3=turn_i*lpe;//inner edge
  vector3D tmp4=turn_o*lpe;//outer edge
  Double_t pox[numberOfPointsPerEdge*4+1],poy[numberOfPointsPerEdge*4+1];
  double addPhi=(outerR/bending-innerR/bending)/(numberOfPointsPerEdge-1);
  double phist=innerR/bending;
  matrix3D turn(normal,addPhi);//-
  matrix3D turn1(normal,dphi/(numberOfPointsPerEdge-1));//-
  for(int i=0;i<numberOfPointsPerEdge;i++)
    {
      edges[0][i]=center+tmp3*innerR;
      edges[1][i]=center+tmp1*abs(phist*bending);
      edges[2][numberOfPointsPerEdge-i-1]=center+tmp4*outerR;
      edges[3][numberOfPointsPerEdge-i-1]=center+tmp2*abs(phist*bending);
      phist+=addPhi;
      tmp1=turn*tmp1;
      tmp2=turn*tmp2;
      tmp3=turn1*tmp3;
      tmp4=turn1*tmp4;
    }
  for(int i=0;i<numberOfPointsPerEdge;i++)
    {
      edges[4][numberOfPointsPerEdge-i-1]=edges[0][i]+normal*(-thickness);
      edges[5][numberOfPointsPerEdge-i-1]=edges[1][i]+normal*(-thickness);
      edges[6][numberOfPointsPerEdge-i-1]=edges[2][i]+normal*(-thickness);
      edges[7][numberOfPointsPerEdge-i-1]=edges[3][i]+normal*(-thickness);
    }
  for(int i=0;i<8;i++)
    for(int j=0;j<numberOfPointsPerEdge;j++)
      {
	edges2[i][j]=projector::project(edges[i][j]);
      }
  int useEdge[4]={0,1,2,3};
  //front-back-plane
  if(normal*(center-eye)>0)
    {
      useEdge[0]=6;useEdge[1]=5;useEdge[2]=4;useEdge[3]=7;}
  //    for(int i=0;i<4;i++)
  //  useEdge[i]=7-i;
  bool bf;
  for(int i=0;i<4;i++)
      {
	bf=(i%2==1);
	if(useEdge[i]>3)
	  bf=(i%2==0);
	for(int j=0;j<numberOfPointsPerEdge;j++)
	  {
	    pox[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][(bf?j:numberOfPointsPerEdge-1-j)].X();
	    poy[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][(bf?j:numberOfPointsPerEdge-1-j)].Y();
	  }
      }
  pox[numberOfPointsPerEdge*4]=pox[0];
  poy[numberOfPointsPerEdge*4]=poy[0];
  if(boundingBox!=NULL)
    {
      vector4D bb(pox[0],poy[0],pox[0],poy[0]);
      for(int i=1;i<numberOfPointsPerEdge*4;i++)
	{
	  if(bb.X()>pox[i])bb.setValue(0,pox[i]);
	  if(bb.Y()>poy[i])bb.setValue(1,poy[i]);
	  if(bb.Z()<pox[i])bb.setValue(2,pox[i]);
	  if(bb.W()<poy[i])bb.setValue(3,poy[i]);
	}
      *boundingBox=bb;
    }
  TPolyLine pl(numberOfPointsPerEdge*4+1,pox,poy);
  pl.SetFillColor(fColor);
  pl.SetLineColor(lColor);
  pl.SetFillStyle(fStyle);
  pl.DrawPolyLine(numberOfPointsPerEdge*4+1,pox,poy,"f");
  if(ident!=NULL)
    (*ident)=pl.Clone();
  //out-insides
//   if()
//     {
//       for(int i=0;i<4;i++)
// 	for(int j=0;j<numberOfPointsPerEdge;j++)
// 	  {
// 	    pox[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].X();
// 	    poy[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].Y();
// 	  }
//       pox[numberOfPointsPerEdge*4]=pox[0];
//       poy[numberOfPointsPerEdge*4]=poy[0];
//       pl.DrawPolyLine(numberOfPointsPerEdge*4+1,pox,poy,"f");
//     }
//   if()
    {
      useEdge[0]=0;
      useEdge[1]=4;
      for(int i=0;i<2;i++)
	for(int j=0;j<numberOfPointsPerEdge;j++)
	  {
	    pox[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].X();
	    poy[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].Y();
	  }
      pox[numberOfPointsPerEdge*2]=pox[0];
      poy[numberOfPointsPerEdge*2]=poy[0];
      pl.DrawPolyLine(numberOfPointsPerEdge*2+1,pox,poy,"f");
    }
  //curved sides
  if(normal*(center-eye)>__prec)
    {
      useEdge[0]=3;
      useEdge[1]=7;
      for(int i=0;i<2;i++)
	for(int j=0;j<numberOfPointsPerEdge;j++)
	  {
	    pox[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].X();
	    poy[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].Y();
	  }
      pox[numberOfPointsPerEdge*2]=pox[0];
      poy[numberOfPointsPerEdge*2]=poy[0];
      pl.DrawPolyLine(numberOfPointsPerEdge*2+1,pox,poy,"f");
      useEdge[0]=1;
      useEdge[1]=5;
      for(int i=0;i<2;i++)
	for(int j=0;j<numberOfPointsPerEdge;j++)
	  {
	    pox[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].X();
	    poy[i*numberOfPointsPerEdge+j]=edges2[useEdge[i]][j].Y();
	  }
      pox[numberOfPointsPerEdge*2]=pox[0];
      poy[numberOfPointsPerEdge*2]=poy[0];
      pl.DrawPolyLine(numberOfPointsPerEdge*2+1,pox,poy,"f");
    }
  for(int i=0;i<8;i++)
    {
      delete []edges[i];
      delete []edges2[i];
    }
  delete []edges;
  delete []edges2;*/
}
void spiral::Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor, int fStyle)const
{
  Draw(eye,plane,boundingBox,NULL,lColor,fColor,fStyle);
}
int spiral::suspect(const sLine3D &line, int stackType)
{
  plane3D ps;
  if(stackType==0)ps=plane3D(center,normal);
  if(line.Direction()*normal<0)
    ps=plane3D(center,normal);
  else 
    ps=plane3D(center-(normal*thickness),-1*normal);
  point3D hit=ps-line;
  vector3D len=hit-center;
  if(len.R()>outerR||len.R()<innerR)return -1;
  float phOff=len.R()/bending;
  matrix3D turn(normal,-phOff);
  len=turn*len;
  float al,ah;
  al=acos(len*lpe/len.R()/lpe.R());
  ah=acos(len*hpe/len.R()/hpe.R());
  if(al>ah)
    return ((int)(al/dphi));
  else 
    return ((int)((2*M_PI-al)/dphi));
}
shape_parameter spiral::description()const
{
  shape_parameter sh;
  sh.setName("spiral");
  sh.addParam<point3D>(center,"center");
  sh.addParam<vector3D>(normal,"normal");
  sh.addParam<vector3D>(lpe,"lower phi edge");
  sh.addParam<float>(innerR,"inner radius");
  sh.addParam<float>(outerR,"outer radius");
  sh.addParam<float>(dphi,"angle between edges");
  sh.addParam<float>(thickness,"thickness");
  sh.addParam<float>(bending,"bending");
  sh.setCompleteWrite(false);
  return sh;
}
shape_parameter spiral::getDescription()
{
  shape_parameter sh;
  sh.setName("spiral");
  sh.addParam<point3D>(point3D(),"center");
  sh.addParam<vector3D>(vector3D(),"normal");
  sh.addParam<vector3D>(vector3D(),"lower phi edge");
  sh.addParam<float>(0,"inner radius");
  sh.addParam<float>(0,"outer radius");
  sh.addParam<float>(0,"angle between edges");
  sh.addParam<float>(0,"thickness");
  sh.addParam<float>(0,"bending");
  sh.setCompleteWrite(false);
  return sh;
}
