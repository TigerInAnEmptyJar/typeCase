#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
// momentum4D::momentum4D(): vector4D()
// {
//   Undefined();
// }

momentum4D::momentum4D(const point4D &p): vector4D(p)
{
  mass=sqrt(abs(W()*W()-X()*X()-Y()*Y()-Z()*Z()));
  velocity=vector3D(X()/W(),Y()/W(),Z()/W());
  gamma=1/sqrt(1-velocity.R()*velocity.R());
}

momentum4D::momentum4D(const vector4D &vec): vector4D(vec)
{
  mass=sqrt(abs(W()*W()-X()*X()-Y()*Y()-Z()*Z()));
  velocity=vector3D(X()/W(),Y()/W(),Z()/W());
  gamma=1/sqrt(1-velocity.R()*velocity.R());
}
#ifdef USEROOT
momentum4D::momentum4D(const momentum4D & m): vector4D()
#else
momentum4D::momentum4D(const momentum4D & m): vector4D()
#endif
{
  setVE(m.Velocity(),m.Energy());
}

momentum4D::momentum4D(const point3D &p, double energy): vector4D(p.X(), p.Y(), p.Z(), energy)
{
  mass=sqrt(abs(energy*energy-p.R()*p.R()));
  velocity=vector3D(p)*(1/energy);
  gamma=1/sqrt(1-velocity.R()*velocity.R());
}

momentum4D::momentum4D(double x, double y, double z, double e): vector4D(x,y,z,e)
{
  mass=sqrt(abs(e*e-x*x-y*y-z*z));
  velocity=vector3D(x,y,z)*(1/e);
  gamma=1/sqrt(1-velocity.R()*velocity.R());
}

void momentum4D::setPM(const vector3D &p, double m)
{
  mass=m;
  point4D::setValues(p.X(),p.Y(),p.Z(),sqrt(p.R()*p.R() + m*m));
  velocity=p*(1/point4D::value4);
  gamma=1/sqrt(1-velocity.R()*velocity.R());
}

void momentum4D::setVM(const vector3D &v, double m)
{
  mass=m;
  velocity=v;
  gamma=1/sqrt(1-velocity.R()*velocity.R());
  double e = m/sqrt(1-v.R()*v.R());
  point4D::setValues(e*v.X(), e*v.Y(), e*v.Z(), e);
}

void momentum4D::setVE(const vector3D &vec, double e)
{
  velocity=vec;
  gamma=1/sqrt(1-velocity.R()*velocity.R());
  mass=e/gamma;//sqrt(abs(1-vec.R()*vec.R()-Y()*Y()-Z()*Z()))*e;
  point4D::setValues(e*vec.X(), e*vec.Y(), e*vec.Z(), e);
}

// void momentum4D::setPE(const vector3D &p, double e)
// {
//   mass=sqrt(abs(e*e-p.R()*p.R()));
//   velocity=p*(1/e);
//   gamma=1/sqrt(1-velocity.R()*velocity.R());
//   point4D::setValues(p.X(), p.Y(), p.Z(), e);
// }

// double momentum4D::Energy() const
// {
//     return point4D::value4;
// }

// double momentum4D::Mass() const
// {
//     return mass;
// }

// double momentum4D::Beta() const
// {
//   return velocity.R();
// }

// double momentum4D::Gamma() const
// {
//   return gamma;
// }

// vector3D momentum4D::Velocity() const
// {
//     return velocity;
// }

// vector3D momentum4D::Momentum()const
// {
//   return vector3D(point4D::value1,point4D::value2,point4D::value3);
// }
// vector3D momentum4D::Direction()const
// {
//   vector3D ret(point4D::value1,point4D::value2,point4D::value3);
//   ret.normalize();
//   return ret;
// }

momentum4D momentum4D::boost(const vector3D &be)const
{
  momentum4D ret;
  vector3D mom=Momentum();
  float gam=1/sqrt(1-be*be);
  ret.setPE(mom+
	    be*((gam-1)/(be*be)*(be*mom))+
	    be*gam*W(),
	    gam*(W()+be*mom));
  return ret;
}

momentum4D momentum4D::CM(const momentum4D &v1,const momentum4D &v2,const momentum4D &v3,const momentum4D &v4,const momentum4D &v5)
{
  vector3D u[5];
  double g[5],m[5];
  int n=0;
  if(v1.getState()==_regular_)
    {
      u[n]=v1.Velocity();
      m[n]=v1.Mass();
      g[n]=1/sqrt(1-u[n]*u[n]);
      n++;
    }
  if(v2.getState()==_regular_)
    {
      u[n]=v2.Velocity();
      m[n]=v2.Mass();
      g[n]=1/sqrt(1-u[n]*u[n]);
      n++;
    }
  if(v3.getState()==_regular_)
    {
      u[n]=v3.Velocity();
      m[n]=v3.Mass();
      g[n]=1/sqrt(1-u[n]*u[n]);
      n++;
    }
  if(v4.getState()==_regular_)
    {
      u[n]=v4.Velocity();
      m[n]=v4.Mass();
      g[n]=1/sqrt(1-u[n]*u[n]);
      n++;
    }
  if(v5.getState()==_regular_)
    {
      u[n]=v5.Velocity();
      m[n]=v5.Mass();
      g[n]=1/sqrt(1-u[n]*u[n]);
      n++;
    }
  vector3D U(0,0,0);
  double mas=0;
  double M=0;
  for(int i=0;i<n;i++)
    {
      U=U+(u[i]*m[i]*g[i]);
      mas+=m[i]*g[i];
      M+=m[i]*m[i];
      for(int j=0;j<n;j++)
	if(i!=j)
	  M+=m[i]*m[j]*g[i]*g[j]*(1-u[i]*u[j]);
    }
  U=U*(1/mas);
  M=sqrt(abs(M));
  momentum4D ret;
  ret.setVM(U,M);
  return ret;
}
//#ifdef USEQT
string momentum4D::toString(int pt)const
{
  if(getState()!=_regular_&&getState()!=_zero_)return point4D::toString();
  int ptt=pt&3;
  pt=pt-ptt;
  point3D p=Momentum();
  double q=Energy();
  string pp="p",qq="e";
  switch(pt)
    {
    case -1:
    case 0:
      p=Momentum();
      q=Energy();
      pp="p";
      qq="e";
      break;
    case 4:
      p=Momentum();
      q=Mass();
      pp="p";
      qq="m";
      break;
    case 8:
      p=Velocity();
      q=Energy();
      pp="v";
      qq="e";
      break;
    case 12:
      p=Velocity();
      q=Mass();
      pp="v";
      qq="m";
      break;
    }
  return (string("(")+pp+" = "+p.toString(ptt)+", "+qq+" = "+string_number(q)+")");
}
//#endif
ostream & operator << (ostream &o,const  momentum4D & m)
{
    vector3D tmp(m.Momentum());
    o << "(p = " << tmp;
    o << ", e = " << m.Energy() << ")";
    return o;
}

// void momentum4D::operator =(const momentum4D &vec)
// {
//   setPE(vector3D(vec[0],vec[1],vec[2]),vec[3]);
// }

// int momentum4D::operator == (const momentum4D &m)const
// {
//     return ((Velocity()==m.Velocity())&&(Energy()==m.Energy()));
// }

// momentum4D momentum4D::operator! ()const
// {
//     momentum4D m(*this);
//     m.transpose();
//     return m;
// }

// momentum4D momentum4D::operator+(const momentum4D &vec) const
// {
//   return momentum4D(Momentum()+vec.Momentum(),vec.Energy()+Energy());
// }

// momentum4D momentum4D::operator-(const momentum4D &vec) const
// {
//   return momentum4D(Momentum()-vec.Momentum(),Energy()-vec.Energy());
// }
// double momentum4D::operator*(const momentum4D &vec) const
// {
//   return (vec.Energy()*point4D::value4-Momentum()*vec.Momentum());
// }

// momentum4D momentum4D::operator*(double factor)const
// {
//   return momentum4D(Momentum()*factor,point4D::value4*factor);
// }

momentum4D operator*(double factor,const momentum4D &vec)
{
  return momentum4D(vec.Momentum()*factor,vec.Energy()*factor);
}

// double momentum4D::operator[](int pos)const
// {
//   return vector4D::operator[](pos);
// }
// double momentum4D::Rapidity()const
// {
//   return 0.5*(log((point4D::value4+point4D::value3)/(point4D::value4-point4D::value3)));
// }
double momentum4D::Rapidity(const vector3D &axis)const
{
  double t_pt=pt(axis);
  return 0.5*(log((point4D::value4+t_pt)/(point4D::value4-t_pt)));
}
// double momentum4D::px()const
// {
//   return point4D::value1;
// }
// double momentum4D::py()const
// {
//   return point4D::value2;
// }
// double momentum4D::pz()const
// {
//   return point4D::value3;
// }
// double momentum4D::e()const
// {
//   return point4D::value4;
// }
// double momentum4D::Px()const
// {
//   return point4D::value1;
// }
// double momentum4D::Py()const
// {
//   return point4D::value2;
// }
// double momentum4D::Pz()const
// {
//   return point4D::value3;
// }
// double momentum4D::E()const
// {
//   return point4D::value4;
// }
// double momentum4D::Pt()const
// {
//   return point4D::value3;
// }
// double momentum4D::Pt(const vector3D &axis)const
// {
//   return (Momentum()-axis*(Momentum()*axis)/(axis.R()*axis.R())).R();
// }
// vector3D momentum4D::Pt_v(const vector3D &axis)const
// {
//   return Momentum()-axis*(Momentum()*axis)/(axis.R()*axis.R());
// }
// double momentum4D::Pl()const
// {
//   return f_rho;
// }
// double momentum4D::Pl(const vector3D &axis)const
// {
//   return (axis*(Momentum()*axis)/(axis.R()*axis.R())).R();
// }
// vector3D momentum4D::Pl_v(const vector3D &axis)const
// {
//   return axis*(Momentum()*axis)/(axis.R()*axis.R());
// }
// double momentum4D::pt()const
// {
//   return point4D::value3;
// }
// double momentum4D::pt(const vector3D &axis)const
// {
//   return (Momentum()-axis*(Momentum()*axis)/(axis.R()*axis.R())).R();
// }
// vector3D momentum4D::pt_v(const vector3D &axis)const
// {
//   return Momentum()-axis*(Momentum()*axis)/(axis.R()*axis.R());
// }
// double momentum4D::pl()const
// {
//   return f_rho;
// }
// double momentum4D::pl(const vector3D &axis)const
// {
//   return (axis*(Momentum()*axis)/(axis.R()*axis.R())).R();
// }
// vector3D momentum4D::pl_v(const vector3D &axis)const
// {
//   return axis*(Momentum()*axis)/(axis.R()*axis.R());
// }
