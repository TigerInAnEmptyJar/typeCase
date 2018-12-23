#include <stdlib.h>
#include "fittingRoutines.h"
vertexFit::vertexFit(int maxLinesIn, int maxPointsIn):geomObject(),maxLines(maxLinesIn),maxPoints(maxPointsIn)
{
  numPoints=new int[maxLines];
  points=new vector3D*[maxLines];
  res=new vector3D*[maxLines];
  sigma=new float*[maxLines];
  lines=new vector3D[maxLines];
  for(int i=0;i<maxLines;i++)
    {
      sigma[i]=new float[maxPoints];
      points[i]=new vector3D[maxPoints];
      res[i]=new vector3D[maxPoints];
    }
  numLines=0;
  clear();
}
vertexFit::vertexFit():geomObject(),maxLines(5),maxPoints(100)
{
  numPoints=new int[maxLines];
  points=new vector3D*[maxLines];
  res=new vector3D*[maxLines];
  sigma=new float*[maxLines];
  lines=new vector3D[maxLines];
  for(int i=0;i<maxLines;i++)
    {
      sigma[i]=new float[maxPoints];
      points[i]=new vector3D[maxPoints];
      res[i]=new vector3D[maxPoints];
    }
  numLines=0;
  clear();
}
vertexFit::vertexFit(const vertexFit &v):geomObject(),maxLines(v.MaxLines()),maxPoints(v.MaxPoints())
{
  numPoints=new int[maxLines];
  points=new vector3D*[maxLines];
  res=new vector3D*[maxLines];
  sigma=new float*[maxLines];
  lines=new vector3D[maxLines];
  for(int i=0;i<maxLines;i++)
    {
      sigma[i]=new float[maxPoints];
      points[i]=new vector3D[maxPoints];
      res[i]=new vector3D[maxPoints];
    }
  numLines=0;
  clear();
}
vertexFit::~vertexFit()
{
  for(int i=0;i<maxLines;i++)
    {
      delete []sigma[i];
      delete[] points[i];
      delete []res[i];
    }
  delete[] numPoints;
  delete[] points;
  delete[] res;
  delete[] sigma;
  delete[] lines;
}
bool vertexFit::Fitted()const
{
  return fitted;
}
float vertexFit::Chi()const
{
  return chi2;
}
int vertexFit::MaxPoints()const
{
  return maxPoints;
}
int vertexFit::MaxLines()const
{
  return maxLines;
}

int vertexFit::Points(int line)const
{
  if(line<0||line>=numLines)return -1;
  return numPoints[line];
}
vector3D vertexFit::Point(int line,int num)const
{
  if(line<0||line>=numLines)return vector3D();
  if(num<0||num>=numPoints[line])return vector3D();
  return points[line][num];
}
vector3D vertexFit::Resolution(int line,int num)const
{
  if(line<0||line>=numLines)return vector3D();
  if(num<0||num>=numPoints[line])return vector3D();
  return res[line][num];
}
float vertexFit::Sigma(int line, int num)const
{
  if(line<0||line>=numLines)return -1;
  if(num<0||num>=numPoints[line])return -1;
  return sigma[line][num];
}
vector3D vertexFit::Line(int num)const
{
  if(!fitted)return vector3D();
  if(num<0||num>=numLines)return vector3D();
  return lines[num];
}
point3D vertexFit::Vertex()const
{
  if(!fitted)return point3D();
  return vertex;
}
void vertexFit::clear()
{
  numLines=0;
  for(int i=0;i<maxLines;i++)numPoints[i]=0;
  fitted=false;
}
void vertexFit::addPoint(const vector3D &p, const vector3D &r, float s, int line)
{
  if(line<0||line>=maxLines)return;
  if(numPoints[line]>=maxPoints)return;
  fitted=false;
  points[line][numPoints[line]]=p;
  res[line][numPoints[line]]=r;
  if(fabs(s)>1e-20)
    sigma[line][numPoints[line]]=s;
  else sigma[line][numPoints[line]]=1e-20;
  numPoints[line]++;
  if(numLines<=line)
    numLines=line+1;
}

float vertexFit::fit()
{
  if(fitted)return chi2;
  if(numLines<=0)return -1;
  if(numLines==1) 
    {
      sLine3D s;
      lineFit single(numPoints[0]);
      for(int j=0;j<numPoints[0];j++)
	single.addPoint(points[0][j],res[0][j],sigma[0][j]);
      chi2=single.fit();//lineFit::fit(numPoints[0],points[0],res[0],sigma[0],s);
      vertex=single.Foot();
      lines[0]=single.Direction();
      fitted=true;
      return chi2;
    }
  sLine3D s[numLines];
  for(int i=0;i<numLines;i++)
    {
      lineFit single(numPoints[i]);
      for(int j=0;j<numPoints[i];j++)
	single.addPoint(points[i][j],res[i][j],sigma[i][j]);
      single.fit();
      s[i]=sLine3D(single.Foot(),single.Direction());
    }
  point3D mid;
  int sumLines=0;
  for(int i=0;i<numLines;i++)sumLines+=numPoints[i];
  matrixNxM A(sumLines,2+numLines*2);
  A.Zero();
  Vector b(sumLines),ret(2+numLines*2);
  b.transpose();
  ret.transpose();
  if(numLines==2)
    {
      lLine3D poca=s[0]-s[1];
      mid=poca.Mid();
    }
  else if(numLines==3)
    {
      lLine3D ll1=s[0]-s[1];
      lLine3D ll2=s[1]-s[2];
      lLine3D ll3=s[2]-s[0];
      point3D A_,B_,C_;
      A_=point3D(0,0,0)+(vector3D(ll1.P())+vector3D(ll1.Q()))*0.5;
      B_=point3D(0,0,0)+(vector3D(ll2.P())+vector3D(ll2.Q()))*0.5;
      C_=point3D(0,0,0)+(vector3D(ll3.P())+vector3D(ll3.Q()))*0.5;
      point3D H = A_+((B_ - A_)*0.5);
      mid=H + ((C_ - H)*(1/3.0));
    }
  if(mid.getState()==_undefined_)return 1000000;
  fitted=true;
  int n=0;
  for(int i=0;i<sumLines;i++)
    for(int k=0;k<numLines*2+2;k++)
      A.setValue(i,k,0);
  for(int j=0;j<numLines;j++)
    for(int i=0;i<numPoints[j];i++)
      {
	b.setValue(n,(points[j][i][0]*res[j][i][0]+points[j][i][1]*res[j][i][1])/sigma[j][i]);
	A.setValue(n,0,res[j][i][0]/sigma[j][i]);
	A.setValue(n,1,res[j][i][1]/sigma[j][i]);
	A.setValue(n,2*j+2,(points[j][i][2]-mid.Z())*res[j][i][0]/sigma[j][i]);
	A.setValue(n,2*j+3,(points[j][i][2]-mid.Z())*res[j][i][1]/sigma[j][i]);
	n++;
      }
  matrixNxM At(!A);
  matrixNxM A_1x(At*A);
  A_1x=-A_1x;
  if(A_1x.Lines()==0) return 100000000;
  ret=(A_1x)*(At*b);
  vertex=point3D(ret[0],ret[1],mid.Z());
  for(int i=0;i<numLines;i++)
    {
      lines[i].setValues(ret[2+2*i],ret[3+2*i],1);
      lines[i].normalize();
    }
  Vector cC(A*ret);
  cC=cC-b;
  chi2=cC*cC;
  if(2+2*numLines!=sumLines)
    chi2=chi2/abs(2+2*numLines-sumLines);
  return chi2;
}
