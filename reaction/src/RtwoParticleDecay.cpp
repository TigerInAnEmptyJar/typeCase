#include "RtwoParticleDecay.h"
RtwoParticleDecay::RtwoParticleDecay():TBase("RtwoParticleDecay")
{
  fname="RtwoParticleDecay";
  possible=false;
}
RtwoParticleDecay::RtwoParticleDecay(const RtwoParticleDecay &react):TBase("RtwoParticleDecay")
{
  fname="RtwoParticleDecay";
  m_1=react.mass1();
  m_2=react.mass2();
  m_i=react.massI();
  if(m_1+m_2>m_i)//decay forbidden: Energy conservation
    {
      possible=false;
      return;
    }
  p_1_cm=react.pInit();
  p_2_cm=p_1_cm;
  E_1_cm=sqrt(m_1*m_1+p_1_cm*p_1_cm);
  E_2_cm=sqrt(m_2*m_2+p_2_cm*p_2_cm);
  beta_1_cm=p_1_cm/E_1_cm;
  beta_2_cm=p_2_cm/E_2_cm;
  excess=E_1_cm-m_1+E_2_cm-m_2;
  possible=true;
}
RtwoParticleDecay::RtwoParticleDecay(double mass1, double mass2, double mass3):TBase("RtwoParticleDecay")
{
  fname="RtwoParticleDecay";
  m_i=mass1;
  m_1=mass2;
  m_2=mass3;
  if(m_1+m_2>m_i)//decay forbidden: Energy conservation
    {
      possible=false;
      return;
    }
  p_1_cm=(m_i*m_i-(m_1+m_2)*(m_1+m_2))
    *(m_i*m_i-(m_1-m_2)*(m_1-m_2));
  p_1_cm=sqrt(p_1_cm)/2./m_i;
  p_2_cm=p_1_cm;
  E_1_cm=sqrt(m_1*m_1+p_1_cm*p_1_cm);
  E_2_cm=sqrt(m_2*m_2+p_2_cm*p_2_cm);
  beta_1_cm=p_1_cm/E_1_cm;
  beta_2_cm=p_2_cm/E_2_cm;
  excess=E_1_cm-m_1+E_2_cm-m_2;
  possible=true;
}
void RtwoParticleDecay::setMasses(double mass1, double mass2, double mass3)
{
  m_i=mass1;
  m_1=mass2;
  m_2=mass3;
  if(m_1+m_2>m_i)//decay forbidden: Energy conservation
    {
      possible=false;
      return;
    }
  p_1_cm=(m_i*m_i-(m_1+m_2)*(m_1+m_2))
    *(m_i*m_i-(m_1-m_2)*(m_1-m_2));
  p_1_cm=sqrt(p_1_cm)/(2.*m_i);
  p_2_cm=p_1_cm;
  E_1_cm=(m_i*m_i-m_2*m_2+m_1*m_1)/(2.*m_i);//sqrt(m_1*m_1+p_1_cm*p_1_cm);
  E_1_cm=(m_i*m_i-m_1*m_1+m_2*m_2)/(2.*m_i);//  E_2_cm=sqrt(m_2*m_2+p_2_cm*p_2_cm);
  beta_1_cm=p_1_cm/E_1_cm;
  beta_2_cm=p_2_cm/E_2_cm;
  excess=E_1_cm-m_1+E_2_cm-m_2;
  possible=true;
}
void RtwoParticleDecay::check(const vector3D &v_in, const vector3D& v_out1, const vector3D& v_out2)
{
  if(!possible)return;
  double theta_1=acos((v_in*v_out1)/v_in.R()/v_out1.R());
  double theta_2=acos((v_in*v_out2)/v_in.R()/v_out2.R());
  vector3D vv=v_out1^v_out2;
  if(vv.R()<0)
    {
      valid_angles=false;
      return;
    }
  vv.normalize();
  vv=v_in-(vv*(v_in*vv));
  vv.normalize();
  theta_1=acos((vv*v_out1)/vv.R()/v_out1.R());
  theta_2=acos((vv*v_out2)/vv.R()/v_out2.R());
  check(theta_1,theta_2);
  initDirection=vv;
  direction1=v_out1;direction2=v_out2;
  momentum4D p(v_out1*p_1_lab,E_1_lab);
  dir1CM=p.boost(v_in*beta_i_lab);
}
void RtwoParticleDecay::check(TTrack *decayingTrack)
{
  if(decayingTrack==NULL)
    {valid_angles=false;return;}
  if(decayingTrack->getNumberOfSecondaryTracks()!=2)
    {valid_angles=false;return;}
  vector3D diri=decayingTrack->getPath().Direction();
  vector3D dir1=decayingTrack->getSecondary(0)->getPath().Direction();
  vector3D dir2=decayingTrack->getSecondary(1)->getPath().Direction();
  //////test////////

  double tanth1=tan(acos((diri*dir1)/diri.R()/dir1.R()));
  double tanth2=tan(acos((diri*dir2)/diri.R()/dir2.R()));
  double sum=tanth1+tanth2;
  tanth1=tanth1*tanth1;
  tanth2=tanth2*tanth2;
  vector2D p12i(sqrt(tanth2+tanth1*tanth2)/sum,sqrt(tanth1+tanth1*tanth2)/sum);
  /////////////////////////////
  double tmpm1,tmpm2;
  bool v1,v2;
  vector2D p12a,p12b;
  check(diri,dir1,dir2);
  tmpm1=(P1Lab(dir1)+P2Lab(dir2)).Mass();
  v1=valid_angles;
  double part1=0,part2=0;
  if(valid_angles)
    {
      part1=p_i_lab;
      p12a.setValues(p_1_lab,p_2_lab);
    }
  check(diri,dir2,dir1);
  tmpm2=(P1Lab(dir1)+P2Lab(dir2)).Mass();
  v2=valid_angles;
  if(valid_angles)
    {
      part2=p_i_lab;
      p12b.setValues(p_2_lab,p_1_lab);
    }
  p12a=p12a/part1;
  p12b=p12b/part2;
  //  cout<<p12a<<p12b<<p12i<<(p12a - p12i).R()<<(p12b-p12i).R()<<part1<<" "<<part2<<endl;
  if(v1 xor v2)
    {
      if(v1) check(diri,dir1,dir2);
      else check(diri,dir2,dir1);
    }
  else if(abs(tmpm1-m_i)<=abs(tmpm2-m_i))
    check(diri,dir1,dir2);
  else
    check(diri,dir2,dir1);
}
void RtwoParticleDecay::check(float theta_1, float theta_2)
{
  if(!possible)return;
  bool useFunction=false;//true;
  double tt1=tan(theta_1),tt2=tan(theta_2);
  if(useFunction)
    {
      float f=m_i*m_i;
      string formula="[0]+[1]-x*x*(2*sqrt(pow([3],4)+2*[3]*[3]*[2]*[2]+pow([2],4)))/pow([2]+[3],2)+2*sqrt([0]*[1]+x*x*([0]*([3]*[3]+pow([2]*[3],2))+[1]*([2]*[2]+pow([2]*[3],2)))/pow([2]+[3],2)+pow(x,4)*([3]*[3]+2*[3]*[3]*[2]*[2]+[2]*[2])/pow([2]+[3],4))-[4]";
      //(string("[0]+[1]-x*x*[1]/pow([2]+[3],2)*(2*[2]*[3]*sqrt((1+[2])*(1+[3])))+2*sqrt([0]*[1]+x*x*([0]*([2]*[2]+pow([2]*[3],2))+[1]*([3]*[3]+pow([2]*[3],2)))/pow([3]+[2],2)+x*x*x*x*([3]*[3]+2*pow([2]*[2],2)+[2]*[2])/pow([2]+[3],4))-")+string::number(f)).data()
      p_i_lab=0;
      E_i_lab=sqrt(f+p_i_lab*p_i_lab);
      beta_i_lab=p_i_lab/E_i_lab;
      cout<<p_i_lab<<" "<<flush;
    }
  else
    {
      initDirection=vector3D();
      direction1=vector3D();
      direction2=vector3D();
//       double ang1=tt1/tt1/tt2;
//       double ang2=tt2/tt1/tt2;
      double ang1_2=(tt1+tt2)/(tt1*tt2);
      double num;
      double a,b,c;
      c=p_1_cm*p_1_cm;
      a=c/m_i*((tt1+tt2)/(tt1*tt2));
      a=-a*a;
      num=-tt2/(tt1+tt2)+E_1_cm/m_i;
      b=c*(ang1_2*ang1_2*(c/(m_i*m_i)-num*num)-1);
      double th1=(-b+sqrt(b*b-4.*a*c))/2./a;
      double th2=(-b-sqrt(b*b-4.*a*c))/2./a;
      //   cout<<theta_1*180/M_PI<<" "<<theta_2*180/M_PI<<endl;
      //   cout<<a<<" "<<b<<" "<<c<<endl;
      //   cout<<b*b<<" "<<a*c*4.<<" "<<b*b-a*c*4.<<" "<<sqrt(b*b-a*c*4.)<<" "<<a*2.<<" "<<1/(a*2.)<<endl;
      //   cout<<-b+sqrt(b*b-a*c*4.)<<" "<<-b-sqrt(b*b-a*c*4.)<<endl;
      //   cout<<th1<<" "<<th2<<endl;
      //   cout<<sqrt(abs(th1))<<" "<<sqrt(abs(th2))<<endl;
      //   cout<<asin(sqrt(abs(th1)))<<" "<<asin(sqrt(abs(th2)))<<endl;
      //   cout<<asin(sqrt(abs(th1)))*180/M_PI<<" "<<asin(sqrt(abs(th2)))*180/M_PI<<endl;
      int nSolutions=0;
      if(th1>=0&&th1<=1)nSolutions++;
      if(th2>=0&&th2<=1)nSolutions++;
      if(nSolutions==0)
	{
	  valid_angles=false;
	  cout<<"no solutions"<<endl;
	  return;
	}
      if(nSolutions==2)
	{
	  cout<<"2 solutions found!"<<endl;
	  valid_angles=false;
	  return;
	}
      if(nSolutions==1)
	{
	  valid_angles=true;
	}
      if(th1>=0&&th1<=1)
	theta_cm=asin(sqrt(th1));
      else 
	theta_cm=asin(sqrt(th2));
      num=p_1_cm/m_i*sin(theta_cm)*(tt1+tt2)/(tt1*tt2);
      beta_i_lab=sqrt(num*num/(1+num*num));
      //  cout<<"theta_cm: "<<theta_cm<<" = "<<theta_cm*180/M_PI<<" ß= "<<beta_i_lab<<endl;
      E_i_lab=m_i/sqrt(1-beta_i_lab*beta_i_lab);
      p_i_lab=beta_i_lab*E_i_lab;
    }
  //    cout<<p_i_lab<<" "<<endl;
//   double gamma=1/sqrt(1-beta_i_lab*beta_i_lab);
  //p_i_lab=p_1_cm*sin(theta_cm)*(1+(beta_i_lab)*gamma)+beta_i_lab*gamma*E_1_cm;
  p_1_lab=p_i_lab/(tt1+tt2)*sqrt((tt1*tt1*tt2*tt2)+tt2*tt2);
  p_2_lab=p_i_lab/(tt1+tt2)*sqrt((tt1*tt1*tt2*tt2)+tt1*tt1);
  E_1_lab=sqrt(m_1*m_1+p_1_lab*p_1_lab);
  E_2_lab=sqrt(m_2*m_2+p_2_lab*p_2_lab);
  beta_1_lab=p_1_lab/E_1_lab;
  beta_2_lab=p_2_lab/E_2_lab;
}
float RtwoParticleDecay::betaInit()const
{
  return beta_i_lab;
}

float RtwoParticleDecay::beta1Lab()const
{
  return beta_1_lab;
}
float RtwoParticleDecay::beta2Lab()const
{
  return beta_2_lab;
}
float RtwoParticleDecay::beta1CM()const
{
  return beta_1_cm;
}
float RtwoParticleDecay::beta2CM()const
{
  return beta_2_cm;
}
float RtwoParticleDecay::pCM()const
{
  return p_1_cm;
}
float RtwoParticleDecay::p1Lab()const
{
  return p_1_lab;
}
float RtwoParticleDecay::p2Lab()const
{
  return p_2_lab;
}
float RtwoParticleDecay::pInit()const
{
  return p_i_lab;
}
momentum4D RtwoParticleDecay::initCM()const
{
  return momentum4D(0,0,0,m_i);
}
momentum4D RtwoParticleDecay::initLab(const vector3D& direction)const
{
  if(initDirection.getState()!=_regular_)
    {
      momentum4D ret;
      ret.setVM(direction*beta_i_lab,m_i);
      return ret;
    }
  momentum4D ret;
  ret.setPM(initDirection*p_i_lab,m_i);
  return ret;
}
momentum4D RtwoParticleDecay::P1Lab(const vector3D& direction)const
{
  if(direction1.getState()!=_regular_)
    return momentum4D(direction*p_1_lab,E_1_lab);
  return momentum4D(direction1*p_1_lab,E_1_lab);
}
momentum4D RtwoParticleDecay::P2Lab(const vector3D&direction)const
{
  if(direction2.getState()!=_regular_)
    return momentum4D(direction*p_2_lab,E_2_lab);
  return momentum4D(direction2*p_2_lab,E_2_lab);
}
momentum4D RtwoParticleDecay::P1CM(const vector3D &dir1, const vector3D &initDir)const
{
  if(initDirection.getState()!=_regular_||direction1.getState()!=_regular_)
    {
      momentum4D ret(dir1*p_1_lab,E_1_lab);
      return ret.boost(initDir*(-beta_i_lab));
    }
  momentum4D ret(direction1*p_1_lab,E_1_lab);
  return ret.boost(initDirection*(-beta_i_lab));
}
momentum4D RtwoParticleDecay::P2CM(const vector3D &dir2, const vector3D &initDir)const
{
  if(initDirection.getState()!=_regular_||direction2.getState()!=_regular_)
    {
      momentum4D ret(dir2*p_2_lab,E_2_lab);
      return ret.boost(initDir*(-beta_i_lab));
    }
  momentum4D ret(direction2*p_2_lab,E_2_lab);
  return ret.boost(initDirection*(-beta_i_lab));
}
bool RtwoParticleDecay::isPossible()const
{
  return possible;
}
float RtwoParticleDecay::mass1()const
{
  return m_1;
}
float RtwoParticleDecay::mass2()const
{
  return m_2;
}
float RtwoParticleDecay::massI()const
{
  return m_i;
}
float RtwoParticleDecay::mass(int m)const
{
  switch(m)
    {
    case 0:return m_1;
    case 1:return m_2;
    case -1:return m_i;
    }
  return -1;
}
bool RtwoParticleDecay::valid()const
{
  return valid_angles;
}
float RtwoParticleDecay::getInvariantMass(float mass1, float mass2, float theta1, float theta2, float decayingMomentum)
{
  //theta1*=1.1;
  //theta2*=1.1;
  double tt1=tan(theta1), tt2=tan(theta2), sum=tan(theta1)+tan(theta2);
  double m1=mass1*mass1,m2=mass2*mass2, md=decayingMomentum*decayingMomentum;
  tt1=tt1*tt1;
  tt2=tt2*tt2;
  sum=sum*sum;
  double tmp=md*md*((tt1*tt2*(tt1*tt2+tt2+tt1+1))/(sum*sum));
  double rrt=m1*m2+md*((tt1*tt2*(m1+m2)+m2*tt2+m1*tt1)/sum)+tmp;
  tmp=md*((2*tt1*tt2+tt1+tt2)/sum -1);
  double tret=m1+m2+tmp+2*sqrt(rrt);
  return sqrt(tret);
}
momentum4D RtwoParticleDecay::getPrimary(float mass_p, float mass1, float mass2, vector3D dir_p, vector3D dir1, vector3D dir2)
{
  double mp=mass_p*mass_p, m1=mass1*mass1, m2=mass2*mass2;
  double theta1=acos((dir_p*dir1)/dir_p.R()/dir1.R());//*1.1;
  double theta2=acos((dir_p*dir2)/dir_p.R()/dir2.R());//*1.1;
  double tt1=tan(theta1),tt2=tan(theta2);
  double sum=(tt1+tt2)*(tt1+tt2);
  tt1=tt1*tt1;
  tt2=tt2*tt2;
  double a=(2*tt1*tt2+tt1+tt2)/sum-1;
  a=4*(tt1*tt2*(tt1*tt2+tt1+tt2+1)/(sum*sum))-a*a;
  double b=2*(tt1*tt2*(m1+m2)+m1*tt1+m2*tt2)/sum;
  b=b+(2*tt1*tt2+tt1+tt2)/sum*(mp-m2-m1)-(mp-m1-m2);
  double c=4*m1*m2-(mp-m1-m2)*(mp-m1-m2);
  double tmp=b*b-4*a*c;
  if(tmp<0)return momentum4D();
  if(tmp==0)
    {
      double s1=-b/a/2.;
      if(s1<0)return momentum4D();
      momentum4D ret;
      ret.setPM(dir_p*(sqrt(s1)/dir_p.R()),mass_p);
      return ret;
    }
  double s1=(-b+sqrt(tmp))/a/2.;
  double s2=(-b-sqrt(tmp))/a/2.;
  if(s1<0)
    {
      if(s2<0)return momentum4D();
      momentum4D ret;
      ret.setPM(dir_p*(sqrt(s2)/dir_p.R()),mass_p);
      return ret;
    }
  else if(s2<0)
    {
      if(s1<0)return momentum4D();
      momentum4D ret;
      ret.setPM(dir_p*(sqrt(s1)/dir_p.R()),mass_p);
      return ret;
    }
  else
    {
      if(s1==0)
	{
	  momentum4D ret;
	  ret.setPM(dir_p*(sqrt(s2)/dir_p.R()),mass_p);
	  return ret;
	}
      else if(s2==0)
	{
	  momentum4D ret;
	  ret.setPM(dir_p*(sqrt(s1)/dir_p.R()),mass_p);
	  return ret;
	}
      else
	{
	  cout<<"RtwoParticleDecay::getPrimary():2 solutions for incomming momentum"<<endl;
	  if(s1<s2)
	    {
	      momentum4D ret;
	      ret.setPM(dir_p*(sqrt(s1)/dir_p.R()),mass_p);
	      return ret;
	    }
	  else
	    {
	      momentum4D ret;
	      ret.setPM(dir_p*(sqrt(s1)/dir_p.R()),mass_p);
	      return ret;
	    }
	}
    }
  return momentum4D();
}
momentum4D RtwoParticleDecay::getPrimary(float mass1, float mass2, vector3D dir_p, vector3D dir1, vector3D dir2,float beta1,int ptt)
{
  switch(ptt)
    {
    case 0://beta1=beta1
      {
	double th1=acos(dir_p*dir1/dir_p.R()/dir1.R()),th2=acos(dir_p*dir2/dir_p.R()/dir2.R());
	double p_1=mass1*beta1/sqrt(1-beta1*beta1);
	double p_d=p_1*cos(th1)*(tan(th1)+tan(th2))/tan(th2);
	double p_2=p_1*sqrt(sin(th1)*sin(th1)+cos(th1)*cos(th1)*tan(th1)*tan(th1)/tan(th2)/tan(th2));
	double e_d=sqrt(mass1*mass1+p_1*p_1)+sqrt(mass2*mass2+p_2*p_2);
	return momentum4D((p_d/dir_p.R())*dir_p,e_d);
      }
    case 1://beta1=decaying momentum
      {
	double th1=acos(dir_p*dir1/dir_p.R()/dir1.R()),th2=acos(dir_p*dir2/dir_p.R()/dir2.R());
	double sum=tan(th1)+tan(th2);
	double p_1=beta1*beta1*(tan(th2)*tan(th2)*(1+tan(th1)*tan(th1))/(sum*sum));
 	double p_2=beta1*beta1*(tan(th1)*tan(th1)*(1+tan(th2)*tan(th2))/(sum*sum));
	double e_1=sqrt(p_1+mass1*mass1);
	double e_2=sqrt(p_2+mass2*mass2);
	double e=e_1+e_2;
	return momentum4D(beta1*dir_p/dir_p.R(),e);
     }
    case 2://beta1=mass_d
      {
	return RtwoParticleDecay::getPrimary(beta1, mass1, mass2, dir_p, dir1, dir2);
      }
    }
  return momentum4D();
}
momentum4D RtwoParticleDecay::getSecondary(float mass1, float mass2, vector3D dir_p, vector3D dir1, vector3D dir2, float value, int ptt)
{
  switch(ptt)
    {
    case 0://value=beta1
      {
	momentum4D ret;
	ret.setVM((value/dir1.R())*dir1,mass1);
	return ret;
      }
    case 1://value=decaying momentum
      {
	double th1=acos((dir1*dir_p)/dir1.R()/dir_p.R()),th2=acos((dir2*dir_p)/dir2.R()/dir_p.R());
	double sum=tan(th1)+tan(th2);
	double p=sqrt(tan(th2)*tan(th2)*(1+tan(th1)*tan(th1)))/sum;
	momentum4D ret;
	ret.setPM((value*p/dir1.R())*dir1,mass1);
	return ret;
      }
    case 2://value=decaying mass
      {
	momentum4D tmp=RtwoParticleDecay::getPrimary(value, mass1, mass2, dir_p, dir1, dir2);
	return RtwoParticleDecay::getSecondary(mass1, mass2, dir_p, dir1, dir2, tmp.Momentum().R(),1);
      }
    case 3://value=beta2
      {
	momentum4D p2;p2.setVM(dir2*value,mass2);
	double th1=acos(dir_p*dir1/dir_p.R()/dir1.R()),th2=acos(dir_p*dir2/dir_p.R()/dir2.R());
	double p_2=p2.Momentum().R();//mass2*value/sqrt(1-value*value);
	//	double p_d=p_1*cos(th1)*(tan(th1)+tan(th2))/tan(th2);
	double p_1=p_2*sin(th2)*sqrt(1/(tan(th1)*tan(th1))+1);//sqrt(sin(th2)*sin(th2)+cos(th2)*cos(th2)*tan(th2)*tan(th2)/tan(th1)/tan(th1));
	momentum4D ret;
	ret.setPM((p_1/dir1.R())*dir1,mass1);
	return ret;
      }
    }
  return momentum4D();
}

