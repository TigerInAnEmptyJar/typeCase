#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif

point2D::point2D(const point &p):point(0)
{
    if (p.getDimension()!=2) return;
    Regular();
    switch (p.getState())
    {
	case _undefined_:
	{
	    Zero();
	    Undefined();
	    break;
	}
	case _zero_: 
	{
	    Zero();
	    break;
	}
	case _infinite_:
	{
	  infinityPos = (p[0]<0||p[0]>=0)?1:0;
	  Infinite();
	}
	case _regular_:
	{
	    setValues(p.getValue(0), p.getValue(1));
	}
    }
    repre = _cartesic_;
}

#ifdef USEROOT
point2D::point2D(const point2D &p):point(0)
#else
point2D::point2D(const point2D &p):point(0)
#endif
{
    Regular();
    switch (p.getState())
    {
	case _undefined_:
	{
	    Zero();
	    Undefined();
	    break;
	}
	case _zero_: 
	{
	    Zero();
	    break;
	}
	case _infinite_:
	{
	  infinityPos = (p[0]<0||p[0]>=0)?1:0;
	    Infinite();
	}
	case _regular_:
	{
	    setValues(p.X(), p.Y());
	}
    }
    if(p.isCartesic()!=isCartesic()) toggleRep();
}

void point2D::setValues(double x, double y)
{
    repre = _cartesic_;
    Regular();
    if ((x==0)&&(y==0)) 
    {
	Zero();
	value1=0;
	value2=0;
	f_phi=0;
	f_r=0;
	return;
    }
    if ((!(x<0||x>=0))||(!(y<0||y>=0)))
     {
 	value1=x;
 	value2=y;
 	Infinite();
 	infinityPos = (x<0||x>=0)?1:0;
	f_phi=(infinityPos==0?0:M_PI/2.);
	f_r=nanf("nan");
 	return;
     }
     value1=x;
     value2=y;
     f_r=sqrt(x*x+y*y);
     f_phi=acos(x/f_r);
}

// double point2D::R()const 
// {
//   return f_r;
// }

void point2D::operator = (const point &p)
{
    if (p.getDimension()!=2) return;
    Regular();
    switch (p.getState())
    {
	case _undefined_:
	{
	    Zero();
	    Undefined();
	    break;
	}
	case _zero_: 
	{
	    Zero();
	    break;
	}
	case _infinite_:
	{
	    infinityPos = (p[0]<0||p[0]>=0)?1:0;
	    Infinite();
	}
	case _regular_:
	{
	    setValues(p.getValue(0), p.getValue(1));
	}
    }
}

void point2D::operator = (const point2D &p)
{
    Regular();
    switch (p.getState())
    {
	case _undefined_:
	{
	    Zero();
	    Undefined();
	    break;
	}
	case _zero_: 
	{
	    Zero();
	    break;
	}
	case _infinite_:
	{
	  infinityPos = (p[0]<0||p[0]>=0)?1:0;
	    Infinite();
	}
	case _regular_:
	{
	    setValues(p.getValue(0), p.getValue(1));
	}
    }
}
//#ifdef USEQT
string point2D::toString(int pt)const
{
  if(!getState()==_regular_)return point::toString();
  repTD re=repre;
  switch(pt)
    {
    case _cartesic_:re=_cartesic_;break;
    case _polar_:re=_polar_;break;
    }
  if(re==_cartesic_)return point::toString();
  return (string("(")+string_number(f_phi)+", "+string_number(f_r)+")");
}
//#endif
ostream & operator << (ostream &o, const point2D &p)
{
    switch (p.getState())
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
	    if (p.isCartesic())
		o << "(" << p.X() << ", " << p.Y() << ")";
	    else
		o << "(" << p.Phi() << ", " << p.R() << ")";
	}
    }
    return o;
}
// point2D::point2D():point(0)
// {
// }

point2D::point2D(double x, double y):point(0)
{
    repre = _cartesic_;
    setValues (x, y);
}

// double point2D::X()const 
// {
//     return value1;
// }

// double point2D::Y()const 
// {
//     return value2;
// }


// double point2D::Phi()const 
// {
//   return f_phi;
// }

// int point2D::isCartesic()const 
// {
//     return (repre == _cartesic_);
// }

// void point2D::toggleRep()
// {
//     if (repre == _cartesic_ ) 
// 	repre = _polar_;
//     else
// 	repre = _cartesic_;
// }

// vector2D point2D::operator - (const point2D &q)const
// {
//     return vector2D(value1-q.X(), value2-q.Y());
// }

// point2D point2D::operator - (const vector2D &v)const
// {
//     return vector2D(value1-v.X(), value2-v.Y());
// }

// point2D point2D::operator + (const vector2D &v)const
// {
//     return vector2D(v.X()+value1, v.Y()+value2);
// }

int point2D::operator ==(const point2D &p)const
{
    return ((value1-p.X()<__prec)&&(value2-p.Y()<__prec));
}

// geomState point2D::getState() const
// {
//     return point::getState();
// } 
// int point2D::getDimension() const{return 2;}           
// void point2D::setValue(int pos, double valueIn)
// {
//   switch(pos)
//     {
//     case 0:value1=valueIn;break;
//     case 1:value2=valueIn;break;
//     }
// }
// double point2D::getValue(int pos) const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     }
//   return 0;
// }      
// double point2D::operator [](int pos)const
// {
//   switch(pos)
//     {
//     case 0:return value1;
//     case 1:return value2;
//     }
//   return -1;
// }
// double point2D::x() const
// {
//   return value1;
// }
// double point2D::y() const
// {
//   return value2;
// }
// double point2D::r() const
// {
//   return f_r;
// }
// double point2D::phi() const
// {
//   return f_phi;
// }
