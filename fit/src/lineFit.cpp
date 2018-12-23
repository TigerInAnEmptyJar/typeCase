#include <stdlib.h>
#include "fittingRoutines.h"
lineFit::lineFit():sLine3D(), maxPoints(100)
{
  sigma=new float[maxPoints];
  closestPointToLine=new vector3D[maxPoints];
  resolution=new vector3D[maxPoints];
  numPoints=0;
  fitted=false;
}
lineFit::lineFit(const lineFit& line):sLine3D(),maxPoints(line.MaxPoints())
{
  sigma=new float[maxPoints];
  closestPointToLine=new vector3D[maxPoints];
  resolution=new vector3D[maxPoints];
  numPoints=0;
  fitted=false;
}
lineFit::lineFit(int maxPointsIn):sLine3D(),maxPoints(maxPointsIn)
{
  sigma=new float[maxPoints];
  closestPointToLine=new vector3D[maxPoints];
  resolution=new vector3D[maxPoints];
  numPoints=0;
  fitted=false;
}
lineFit::~lineFit()
{
  delete []sigma;
  delete []closestPointToLine;
  delete []resolution;
}
void lineFit::clear()
{
  numPoints=0;
  fitted=false;
  Undefined();
}
int lineFit::MaxPoints()const
{
  return maxPoints;
}
int lineFit::Points()const
{
  return numPoints;
}
vector3D lineFit::Point(int num)const
{
  if(num<0||num>=numPoints)return vector3D();
  return closestPointToLine[num];
}
vector3D lineFit::ResolutionVector(int num)const
{
  if(num<0||num>=numPoints)return vector3D();
  return resolution[num];
}
float lineFit::Sigma(int num)const
{
  if(num<0||num>=numPoints)return -1;
  return sigma[num];
}
bool lineFit::Fitted()const
{
  return fitted;
}
float lineFit::Chi()const
{
  return chi2;
}
void lineFit::addPoint(const vector3D &point,const vector3D& res, float sig)
{
  if(numPoints>=maxPoints)return;
  closestPointToLine[numPoints]=point;
  resolution[numPoints]=res;
  sigma[numPoints]=sig;
  numPoints++;
  fitted=false;
  Undefined();
}
sLine3D lineFit::Result()
{
  if(!fitted)return sLine3D();
  return *this;
}
float lineFit::fit()
{
  if(fitted)return chi2;
  if(numPoints<=3)return -1;
  matrixNxM A(numPoints,4),At(4,numPoints);
  Vector b(numPoints),ret(4);
  b.transpose();
  ret.transpose();
  for(int i=0;i<numPoints;i++)
    {
      b.setValue(i,(resolution[i][0]*closestPointToLine[i][0]+resolution[i][1]*closestPointToLine[i][1])/sigma[i]);
      A.setValue(i,0,resolution[i][0]/sigma[i]);
      A.setValue(i,1,resolution[i][1]/sigma[i]);
      A.setValue(i,2,resolution[i][0]*closestPointToLine[i][2]/sigma[i]);
      A.setValue(i,3,resolution[i][1]*closestPointToLine[i][2]/sigma[i]);
    }
  At=!A;
  matrixNxM A_1(At*A);
  A_1=-A_1;
  if(A_1.Lines()<=0)return -1;
  ret=A_1*(At*b);
  chi2=((A*ret)-b).length();
  chi2=chi2*chi2;
  if(numPoints>4)chi2=chi2/abs(4-numPoints);
  newLine(point3D(ret[0],ret[1],0),vector3D(ret[2],ret[3],1));
  Regular();
  fitted=true;
  return chi2;
}
float lineFit::fit(int nPoints, vector3D *points,vector3D *resolutions, float *sigmas, sLine3D &result)
{
  if(nPoints<=3)return -1;
  matrixNxM A(nPoints,4),At(4,nPoints);
  Vector b(nPoints),ret(4);
  b.transpose();
  ret.transpose();
  for(int i=0;i<nPoints;i++)
    {
      b.setValue(i,(resolutions[i][0]*points[i][0]+resolutions[i][1]*points[i][1])/sigmas[i]);
      A.setValue(i,0,resolutions[i][0]/sigmas[i]);
      A.setValue(i,1,resolutions[i][1]/sigmas[i]);
      A.setValue(i,2,resolutions[i][0]*points[i][2]/sigmas[i]);
      A.setValue(i,3,resolutions[i][1]*points[i][2]/sigmas[i]);
    }
  At=!A;
  matrixNxM A_1(At*A);
  A_1=-A_1;
  if(A_1.Lines()<=0)return -1;
  ret=A_1*(At*b);
  float chi=((A*ret)-b).length();
  chi=chi*chi;
  if(nPoints>4)chi=chi/abs(4-nPoints);
  result=sLine3D(point3D(ret[0],ret[1],0),vector3D(ret[2],ret[3],1));
  return chi;
}
