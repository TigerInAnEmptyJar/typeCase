#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif

#ifdef USEROOT
vector2D::vector2D(const vector2D &v):point2D(), Vector(0)
#else
vector2D::vector2D(const vector2D &v):point2D(), Vector(0)
#endif
{
    if (v.isColumn()) transpose();
    Regular();
    switch(v.getState())
    {
	case _zero_:
	{
	    Zero();
	    break;
	}
	case _undefined_:
	{
	    Zero();
	    Undefined();
	    break;
	}
	case _infinite_:	
	{
	  point2D::infinityPos = (v[0]<0||v[0]>=0)?1:0;
	  Infinite();
	}
	case _regular_:
	{
	    setValues(v.X(), v.Y());
	}
    }
    if(v.isCartesic()!=isCartesic()) toggleRep();
}

void vector2D::operator = (const point &p)
{
    if (p.getDimension()!=2) return;
    switch (p.getState())
      {
      case _zero_:
	{
	  Zero();
	  return;
	}
      case _undefined_:
	{
	  Zero();
	  Undefined();
	  return;
	}
      case _infinite_: point2D::infinityPos = (p[0]<0||p[0]>=0)?1:0;
      case _regular_:setValues(p[0], p[1]);
    }
}

void vector2D::operator = (const Vector &v)
{
    if (v.getDimension()!=2) return;
    if (v.isColumn()) transpose();
    switch (v.getState())
    {
    case _zero_:
      {
	Zero();
	return;
      }
    case _undefined_:
      {
	Zero();
	Undefined();
	return;
      }
    case _infinite_: point2D::infinityPos = (v[0]<0||v[0]>=0)?1:0;
    case _regular_:setValues(v[0], v[1]);
    }
}

void vector2D::operator = (const point2D &p)
{
    if (p.getState()==_undefined_)
    {
	Zero();
	Undefined();
	return;
    }
    setValues(p.X(), p.Y());
}

void vector2D::operator = (const vector2D &v)
{
    if (v.isColumn()) transpose();
    if (v.getState()==_undefined_)
    {
	Zero();
	Undefined();
	return;
    }
    setValues(v.X(), v.Y());
}
//#ifdef USEQT
string vector2D::toString(int pt)const
{
  string ret=point2D::toString(pt);
  if(!isLine())ret+="T";
  return ret;
}
//#endif
ostream &operator << (ostream &o, const vector2D &v)
{
    switch (v.getState())
    {
	case _infinite_:
	{
	    o << "(infinite)";
	    break;
	}
	case _undefined_:
	{
	    o << "(undefined)";
	    break;
	}
	case _zero_:
	case _regular_:
	{
	    if (v.isCartesic())
		o << "(" << v.X() << ", " << v.Y() << ")";
	    else
		o << "(" << v.Phi() << ", " << v.R() << ")";
	}
    }
    if (v.isColumn()) o << "T";
    return o;
}

// vector2D::vector2D(): point2D(), Vector(0)
// {
// }

// vector2D::vector2D(const point2D &p): point2D(p), Vector(0)
// {
// }

// vector2D::vector2D(const point &p): point2D(p), Vector(0)
// {
// }

vector2D::vector2D(const Vector &v): point2D(), Vector(0)
{
  if(v.getDimension()>=2)
    setValues(v[0],v[1]);
  else if(v.getDimension()==1)
    setValues(v[0],0);
  else
    setValues(0,0);
}

// vector2D::vector2D(double x, double y): point2D(x,y), Vector(0)
// {
// }

// vector2D vector2D::operator + (const vector2D &w)const
// {
//     return vector2D(point2D::value1+w.X(), point2D::value2+w.Y());
// }

// vector2D vector2D::operator - (const vector2D &w)const
// {
//     return vector2D(point2D::value1-w.X(), point2D::value2-w.Y());
// }

// vector2D vector2D::operator * (double f)const
// {
//     return vector2D(f*point2D::value1, f*point2D::value2);
// }

// double vector2D::operator * (const vector2D &w)const
// {
//     return (point2D::value1*w.X()+point2D::value2*w.Y());
// }

// int vector2D::operator ==(const vector2D &p)const
// {
//     return ((point2D::value1==p.X())&&(point2D::value2==p.Y()));
// }

// vector2D vector2D::operator !()const
// {
//     vector2D v(*this);
//     v.transpose();
//     return v;
// }

// void vector2D::setValues(double x, double y)
// {
//     point2D::setValues(x,y);
// }

// void vector2D::normalize()
// {
//     if(point2D::f_r==0) return;
//     point2D::setValues(point2D::value1/point2D::f_r,point2D::value2/point2D::f_r);
// }
// double vector2D::operator[](int pos)const
// {
//   switch(pos)
//     {
//     case 0: return point2D::value1;
//     case 1: return point2D::value2;
//     }
//   return -1;
// }
            
// double vector2D::length() const
// {
//     return point2D::f_r;
// }
// geomState vector2D::getState() const
// {
//     return point2D::getState();
// } 

// void vector2D::Zero()   
// {
//     point2D::Zero();
// }
       
// void vector2D::Infinite()              
// {
//     point2D::Infinite();
// }
       
// void vector2D::Undefined()     
// {
//     point2D::Undefined();
// }
       
// void vector2D::Regular()             
// {
//     point2D::Regular();
// }
       
// double vector2D::getValue(int pos) const
// {
//     return point2D::getValue(pos);
// }      

// void vector2D::setValue(int pos, double valueIn)
// {
//     point2D::setValue(pos,valueIn);
// }
// int vector2D::getDimension() const{return 2;}           

