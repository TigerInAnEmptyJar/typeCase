#include "geometry.h"
// lLine2D::lLine2D(): geomObject(),qPoint()
// {
//   geomObject::Undefined(); 
// }
lLine2D::lLine2D(const point2D &p, const point2D &q): geomObject()
{
  pPoint=p;
  qPoint = q;
  geomObject::Regular();
  if((p.getState()==_infinite_)||(q.getState()==_infinite_))
    geomObject::Infinite();
  else if((q.getState()==_undefined_)||(p.getState()==_undefined_))
    geomObject::Undefined();
  else if (((point2D)q-(point2D)p).getState()==_zero_)
    geomObject::Zero();
  else geomObject::Regular();
}
lLine2D::lLine2D(const point2D &p, const vector2D &v): geomObject()
{
  pPoint=p;
  qPoint = p + v;
  geomObject::Regular();
  if((p.getState()==_infinite_)||(v.getState()==_infinite_))
    geomObject::Infinite();
  else if((p.getState()==_undefined_)||(v.getState()==_undefined_))
    geomObject::Undefined();
  else if ((v.getState()==_zero_))
    geomObject::Zero();
  else
    geomObject::Regular();
  setRep((p.isCartesic()?_cartesic_: _polar_));
}
lLine2D::lLine2D(const lLine2D &l): geomObject()
{
  pPoint=l.P();
  qPoint = l.Q();
  geomObject::Regular();
  switch(l.getState())
    {
    case _undefined_:geomObject::Undefined();break;
    case _zero_:geomObject::Zero();break;
    case _regular_:geomObject::Regular();break;
    case _infinite_:geomObject::Infinite();break;
    }
    setRep(l.getRep());
}
// point2D lLine2D::P() const
// {return pPoint;}
// point2D lLine2D::Q() const
// {return qPoint;}
// vector2D lLine2D::Direction() const
// {return (qPoint-pPoint);}
// point2D lLine2D::Mid()const
// {return pPoint+((qPoint-pPoint)*0.5);}
// int lLine2D::isIn(const point2D &p) const
// {
//   vector2D vv=(*this)-p;
//   return ((vv.R()<__prec)?1:0);
//   if ((getState()==_undefined_)||
//       (getState()==_infinite_)||
//       (qPoint.getState()==_undefined_)||
//       (qPoint.getState()==_infinite_))
//     return 0;
//     vector2D pq= qPoint-pPoint;
//     vector2D pInq = Q() - p;
//     if((abs((pq^pInq).X()) > __prec) &&
//        (abs((pq^pInq).Y()) > __prec) &&
//        (abs((pq^pInq).Z()) > __prec)) return 0;
//     double d = pInq * pq/pq.length()/pq.length();
//     return ((d<=1)&&(d>=0));
// }
void lLine2D::newLine(const point2D &p, const point2D &q)
{
  pPoint=p;
  qPoint = q;
  geomObject::Regular();
  if((p.getState()==_infinite_)||(q.getState()==_infinite_))
    geomObject::Infinite();
  else if((p.getState()==_undefined_)||(q.getState()==_undefined_))
    geomObject::Undefined();
  else if (((q-p).getState()==_zero_))
    geomObject::Zero();
  else
    geomObject::Regular();
  setRep(p.isCartesic()?_cartesic_: _polar_);
}
// double lLine2D::length() const
// {return (pPoint-qPoint).R();}
// repTD lLine2D::getRep() const
// {return (pPoint.isCartesic()?_cartesic_:_polar_);}
void lLine2D::setRep(repTD r)
{
  if(!((pPoint.isCartesic()&&r==_cartesic_)||(!pPoint.isCartesic()&&r==_polar_)))
    {
      pPoint.toggleRep();
      qPoint.toggleRep();
    }
}
// geomState lLine2D::getState() const
// {return geomObject::getState();}
// int lLine2D::getDimension() const
// {return 2;}
// void lLine2D::Zero()
// {
//     geomObject::Zero();
//     pPoint.Zero();
//     qPoint.Zero();
// }
void lLine2D::operator =(const lLine2D &l)
{
  pPoint=l.P();
  qPoint=l.Q();
  geomObject::Regular();
  if((pPoint.getState()==_infinite_)||(qPoint.getState()==_infinite_))
    geomObject::Infinite();
  else if((qPoint.getState()==_undefined_)||(pPoint.getState()==_undefined_))
    geomObject::Undefined();
  else if (((point2D)qPoint-(point2D)pPoint).getState()==_zero_)
    geomObject::Zero();
  else geomObject::Regular();
}
vector2D lLine2D::operator -(const point2D &p)const
{
  if(length()<__prec)return (p-pPoint);
  sLine2D tmpl(pPoint,qPoint-pPoint);
  vector2D tmpv=tmpl-p;
  if((p-pPoint).R()<tmpv.R())return (p-pPoint);
  if((p-qPoint).R()<tmpv.R())return (p-qPoint);
  return tmpv;
}
lLine2D lLine2D::operator -(const lLine2D &l)const
{
  if(l.length()<__prec)
    {
      vector2D vv=(*this)-l.P();
      return lLine2D(l.P()-vv,l.P());
    }
  sLine2D tmpl(pPoint,qPoint);
  lLine2D tmpx=tmpl-l;
  if((tmpx.P()-pPoint).R()<length()&&(tmpx.P()-pPoint)*Direction()>0)
    return tmpx;
  if((tmpx.P()-pPoint)*Direction()>0)
    return lLine2D(tmpx.P(),qPoint);
  return lLine2D(tmpx.P(),pPoint);
}
// point2D lLine2D::operator && (const lLine2D &l)const
// {
//   lLine2D tmpc=(*this)-l;
//   return tmpc.Mid();
// }
// double lLine2D::operator & (const lLine2D &l)const
// {return ((*this)-l).length();}
//#ifdef USEQT
string lLine2D::toString(int pt)const
{
 if(getState()!=_regular_&&getState()!=_zero_)return geomObject::toString();
  int ptt=-1;
  if(pt>=0)
    {
      ptt=pt&3;
      pt=pt<<2;
    }
  if(getState()==_zero_)
    return(string("( P = Q = ")+pPoint.toString(ptt)+")");
  return(string("( P = ")+pPoint.toString(ptt)+", Q = "+qPoint.toString(pt)+", length = "+string_number((pPoint-qPoint).R())+")");
}
// void lLine2D::Infinite(){geomObject::Infinite();}              
// void lLine2D::Undefined(){geomObject::Undefined();}       
// void lLine2D::Regular(){geomObject::Regular();}               

//#endif
