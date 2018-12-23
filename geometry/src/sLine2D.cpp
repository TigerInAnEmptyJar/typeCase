#include "geometry.h"
// sLine2D::sLine2D():geomObject()
// {
// }
sLine2D::sLine2D(const point2D &p, const point2D &q):geomObject()
{
    d=(q-p);
    setRep((p.isCartesic()?_cartesic_:_polar_));
    if ((p.getState()==_undefined_)||(q.getState()==_undefined_))
    {
	Undefined();
	return;
    }
    if (d.getState()==_zero_) 
    {
	Zero();
	return;
    }
    foot = p;
    d.normalize();
}
sLine2D::sLine2D(const point2D &p, const vector2D &v):geomObject()
{
    Regular();
    setRep((v.isCartesic()?_cartesic_:_polar_));
    //    setRep(v.getRep());
    if ((p.getState()==_undefined_)||(v.getState()==_undefined_))
    {
	Undefined();
	return;
    }
    if (v.getState()==_zero_) 
    {
	Zero();
	return;
    }
    d=v;
    foot = p;
    d.normalize();
}
sLine2D::sLine2D(const sLine2D &l):geomObject()
{
    foot = l.Foot();
    d=l.Direction();
    d.normalize();
    setRep(l.getRep());
    if (l.getState()==_regular_)
    {
	Regular();
	return;
    }
    if (l.getState()==_zero_) Zero();
    if (l.getState()==_undefined_) Undefined();
    if (l.getState()==_infinite_) Infinite();
}
// point2D sLine2D::Foot() const
// {return foot;}
// vector2D sLine2D::Direction() const
// {return d;}
// void sLine2D::shiftFoot(double t)
// {foot=foot+d*t;}
void sLine2D::newLine(const point2D &p, const vector2D &v)
{
    Regular();
    setRep((v.isCartesic()?_cartesic_:_polar_));
    //setRep(v.getRep());
    if ((p.getState()==_undefined_)||(v.getState()==_undefined_))
    {
	Undefined();
	return;
    }
    if (v.getState()==_zero_) 
    {
	Zero();
	return;
    }
    d=v;
    foot = p;
    d.normalize();
}
int sLine2D::isIn(const point2D &p) const
{
   if ((getState()==_undefined_)||
       (getState()==_infinite_)||
       (foot.getState()==_undefined_)||
       (foot.getState()==_infinite_))
       return 0;
   if (getState()==_zero_) 
       return (p==foot);
   vector2D fp=p-foot;
   vector2D n=fp-d*(d*fp);
   if(n.length()<=-__prec) return 1;
   else return 0;
}
// repTD sLine2D::getRep() const
// {return (d.isCartesic()?_cartesic_:_polar_);}
void sLine2D::setRep(repTD r)
{
  if((d.isCartesic()&&r==_polar_)||(r==_cartesic_&&!d.isCartesic()))
    d.toggleRep();
  if((foot.isCartesic()&&r==_polar_)||(r==_cartesic_&&!foot.isCartesic()))
    foot.toggleRep();
}
// int sLine2D::isLine() const
// {return d.isLine();}
// int sLine2D::isColumn() const
// {return d.isColumn();}
// void sLine2D::transpose()
// {d.transpose();}
// geomState sLine2D::getState() const
// {return geomObject::getState();}
// int sLine2D::getDimension() const
// {return 2;}
// void sLine2D::Zero()
// {
//   geomObject::Zero();
//   d.setValues(0,0);
//   foot.Zero();
// }
vector2D sLine2D::operator -(const point2D &p)const //distance line - point
{
  vector2D v=p-foot;
  vector2D n=v-d*(d*v);
  return n;
}
point2D sLine2D::operator&&(const sLine2D &l)const //common point of 2 lines
{
  double t=((foot.X()-l.Foot().X())/l.Direction().X()-(foot.Y()-l.Foot().Y())/l.Direction().Y())
    /(d.Y()/l.Direction().Y()-d.X()/l.Direction().X());
  return foot+t*d;
}
double sLine2D::operator&(const sLine2D &l)const //distance of two lines
{
  if(1-abs(l.Direction()*Direction())<__prec)return (l-foot).R();
  return 0;
}
lLine2D sLine2D::operator -(const sLine2D &l)const//closest approach of 2 lines
{
  point2D p=l&&(*this);
  return lLine2D(p,p);
}
lLine2D sLine2D::operator -(const lLine2D &l)const //closest approach of 2 lines
{
  if(l.length()<__prec)
    {
      vector2D vv=(*this)-l.P();
      return lLine2D(l.P()-vv,l.P());
    }
  sLine2D tmpl(l.P(),l.Q());
  point2D tmpp=(*this)&&tmpl;
  if((tmpp-l.P()).R()<l.length()&&(tmpp-l.P())*l.Direction()>0)
    return lLine2D(tmpp,tmpp);
  if((tmpp-l.P())*l.Direction()>0)
    {
      tmpp=(*this)-l.Q();
      return lLine2D(tmpp,l.Q());
    }
  tmpp=(*this)-l.P();
  return lLine2D(tmpp,l.P());
}
void sLine2D::operator=(const sLine2D &l)
{
  foot = l.Foot();
  d=l.Direction();
  d.normalize();
  setRep(l.getRep());
  if (l.getState()==_regular_)
    {
      Regular();
      return;
    }
  if (l.getState()==_zero_) Zero();
  if (l.getState()==_undefined_) Undefined();
  if (l.getState()==_infinite_) Infinite();
} 
//#ifdef USEQT
string sLine2D::toString(int pt)const
{
  if(getState()!=_regular_)return geomObject::toString();
  int ptt=-1;
  if(ptt<=0)
    {
      ptt=(pt&3);
      pt=pt<<2;
    }
  return (string("( p = ")+foot.toString(pt)+", v = "+d.toString(ptt)+")");
}
void sLine2D::Infinite(){geomObject::Infinite();}              
void sLine2D::Undefined(){geomObject::Undefined();}
void sLine2D::Regular(){geomObject::Regular();}

ostream & operator << (ostream &o, const sLine2D & l)
{
  o<<"( p = "<<l.Foot()<<", v = "<<l.Direction()<<")";
  return o;
}
//#endif
