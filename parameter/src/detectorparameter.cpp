#include "detectorparameter.h"
detector_parameter::detector_parameter()
{
  mat=NULL;
  circular=false;
}

detector_parameter::~detector_parameter()
{
}

float detector_parameter::getMaxDistance()const
{
    return maxDist;
}

void detector_parameter::setMaxDistance(float value)
{
    maxDist=value;
}
    
int detector_parameter::getNumberOfElements()const
{
    return numberOfElements;
}

void detector_parameter::setNumberOfElements(int n)
{
    numberOfElements=n;
}

int detector_parameter::getID() const
{
    return ID;
}

void detector_parameter::setID(int id)
{
    ID=id;
}

int detector_parameter::getStackType()const
{
    return stackType;
}

void detector_parameter::setStackType(int s)
{
    stackType=s;
}

shape_parameter detector_parameter::getShape() const
{
    return shape;
}

void detector_parameter::setShape(const shape_parameter &sh)
{
    shape=sh;
    shape.setCompleteWrite(true);
}

ostream &operator<<(ostream &o, const detector_parameter &d)
{
    o<<d.getNumberOfElements()<<" "<<d.getStackType()<<" "<<d.getID()<<" "<<d.getMaterial()<<" "<<d.getMaxDistance()<<d.getName().data()<<endl;
    shape_parameter sh;
    sh=d.getShape();
    o<<sh;
    return o;
}    

void detector_parameter::setMaterial(int matIn)
{
    fmaterial=matIn;
}

int detector_parameter::getMaterial() const
{
    return fmaterial;
}
material_parameter *detector_parameter::material()
{
  return mat;
}

void detector_parameter::setMaterial(material_parameter *matIn)
{
  mat=matIn;
}

bool detector_parameter::isCircular()const
{
  return circular;
}
void detector_parameter::setCircular(bool circ)
{
  circular=circ;
}
istream &operator>>(istream &i, detector_parameter &d)
{
  //    int zahl1,zahl2,zahl3,zahl4,zahl5,zahl;
  int zahl;
  float value1;//,value2,value3;
  char c; 
  string s;
  i>>zahl;
  d.setNumberOfElements(zahl);
  i>>zahl;
  d.setStackType(zahl);
  i>>zahl;
  d.setID(zahl);
  i>>zahl;
  d.setMaterial(zahl);
  i>>value1;
  d.setMaxDistance(value1);
  s="";i.get(c);while(c!='\n'&&!i.eof()){s=s+c;i.get(c);}
  //  i.getline(li,100);
  d.setName(s);
  shape_parameter sh;
  i>>sh;

    d.setShape(sh);
    return i;
}

void detector_parameter::operator=(const detector_parameter &d)
{
    setName(d.getName());
    setDescription(d.getDescription());
    setNumberOfElements(d.getNumberOfElements());
    setStackType(d.getStackType());
    setShape(d.getShape());
    setMaterial(d.getMaterial());
    //    setMaterial(d.material());
    setID(d.getID());
    setCircular(d.isCircular());
}

reaction_parameter::reaction_parameter(const shape_parameter &target, bool twoBeamIn, int first, int sec)
{
    twoBeams=twoBeamIn;
    firstMat=first;
    secMat=sec;
    targetShape=target;
    firstMom=0;
    secMom=0;
}
reaction_parameter::reaction_parameter(const reaction_parameter& r)
{
  twoBeams=r.hasTwoBeams();
  firstMat=r.getMaterial(0);
  firstMom=r.getBeamMomentum(0);
  if(twoBeams)
    {
      secMat=r.getMaterial(1);
      secMom=r.getBeamMomentum(1);
    }
  else
    {
      secMat=r.getTargetMaterial();
      secMom=0;
    }
  targetShape=r.getTargetShape();
}
reaction_parameter &reaction_parameter::operator=(const reaction_parameter&r)
{
  twoBeams=r.hasTwoBeams();
  firstMat=r.getMaterial(0);
  firstMom=r.getBeamMomentum(0);
  if(twoBeams)
    {
      secMat=r.getMaterial(1);
      secMom=r.getBeamMomentum(1);
    }
  else
    {
      secMat=r.getTargetMaterial();
      secMom=0;
    }
  targetShape=r.getTargetShape();
  return *this;
}

reaction_parameter::~reaction_parameter()
{
}

bool reaction_parameter::hasTwoBeams()const
{
    return twoBeams;
}

void reaction_parameter::setTwoBeams(bool tb)
{
    twoBeams=tb;
}

int reaction_parameter::getMaterial(int num) const
{
    if(num==0) return firstMat;
    if(num==1) return secMat;
    return -1;
}

int reaction_parameter::getTargetMaterial()const
{
    if(!twoBeams) return secMat;
    return -1;
}

void reaction_parameter::setMaterial(int num, int mat)
{
    if(num==0) firstMat=mat;
    else if (num==1) secMat=mat;
}

void reaction_parameter::setTargetMaterial(int mat)
{
    if(!twoBeams) secMat=mat;
}

float reaction_parameter::getBeamMomentum(int num)const
{
    if(num==0) return firstMom;
    if((twoBeams)&&(num==1)) return secMom;
    return -1;
}

void reaction_parameter::setBeamMomentum(float value, int num)
{
    if(num==0) firstMom=value;
    else if((num==1)&&twoBeams) secMom=value;
}

shape_parameter reaction_parameter::getTargetShape()const
{
    return targetShape;
}

void reaction_parameter::setTargetShape(const shape_parameter &s)
{
    targetShape=s;
}

ostream &operator <<(ostream &o, const reaction_parameter &d)
{
    o<<((d.hasTwoBeams())?1:0)<<" "<<d.getMaterial(0)<<" "<<d.getMaterial(1)<<" "<<d.getBeamMomentum();
    if(d.hasTwoBeams()) o<<" "<<d.getBeamMomentum(1);
    o<<d.getName().data()<<endl;
    o<<d.getDescription().size();
    if(d.getDescription().size()==0)o<<" ";
    for(unsigned int i=0;i<d.getDescription().size();i++)
	o<<d.getDescription(i).data()<<endl;
    shape_parameter sh;
    sh=d.getTargetShape();
    o<<sh;
    return o;
    int zahl=sh.NumberOfParams<point3D>();
    o<<(zahl)<<" ";
    zahl=sh.NumberOfParams<vector3D>();
    o<<zahl<<" ";
    zahl=sh.NumberOfParams<int>();
    o<<zahl<<" ";
    zahl=sh.NumberOfParams<float>();
    o<<zahl<<" ";
    zahl=sh.NumberOfParams<string>();
    o<<zahl;
    o<<sh.getName().data();
    o<<endl;
    point3D p;
    vector3D v;
    int pec=o.precision();
    o.precision(10);
    for(int k=0;k<sh.NumberOfParams<point3D>();k++)
    {
	p=sh.getParam<point3D>(k);
	o<<p.X()<<" "<<p.Y()<<" "<<p.Z()<<sh.getParamName<point3D>(k).data()<<endl;
    }
    for(int k=0;k<sh.NumberOfParams<vector3D>();k++)
    {
	v=sh.getParam<vector3D>(k);
	o<<v.Theta()<<" "<<v.Phi()<<" "<<v.R()<<sh.getParamName<vector3D>(k).data()<<endl;
    }
    for(int k=0;k<sh.NumberOfParams<int>();k++)
    {
	o<<sh.getParam<int>(k)<<sh.getParamName<int>(k).data()<<endl;
    }
    for(int k=0;k<sh.NumberOfParams<float>();k++)
    {
	o<<sh.getParam<float>(k)<<sh.getParamName<float>(k).data()<<endl;
    }
    for(int k=0;k<sh.NumberOfParams<string>();k++)
    {
	o<<sh.getParam<string>(k).data()<<"\n"<<sh.getParamName<string>(k).data()<<endl;
    }
    o.precision(pec);
    //    o<<d.getTargetShape()<<endl;
    return o;
}

istream &operator>>(istream &i, reaction_parameter &d)
{
    int zahl;
    float z;
    i>>zahl;
    d.setTwoBeams(zahl==1);
    i>>zahl;
    d.setMaterial(0,zahl);
    i>>zahl;
    d.setMaterial(1,zahl);
    i>>z;
    d.setBeamMomentum(z);
    if(d.hasTwoBeams()) 
    {
	i>>z;
	d.setBeamMomentum(z,1);
    }
    char li[250];
    i.getline(li,250);
    d.setName(string(li));
    i>>zahl;
    vector<string> lis;
    for(int I=0;I<zahl;I++)
    {
	i.getline(li,250);
	lis.push_back(string(li));
   }
    d.setDescription(lis);
    shape_parameter sh;
    i>>sh;
    d.setTargetShape(sh);
    return i;
}
