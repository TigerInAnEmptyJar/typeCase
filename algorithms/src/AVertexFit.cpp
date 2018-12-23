#include"AVertexFit.h"
#include <stdlib.h>
AVertexFit::AVertexFit(int numberOflines):AFitAlgorithm("vertex fit algorithm"),maxLines(numberOflines), numLines1(0),numLines2(0),re(7),re1(4),re2(4)
{
  A_1=new Vector[maxLines];
  for(int i=0;i<maxLines;i++)A_1[i].reDimension(4);
  A_2=new Vector[maxLines];
  for(int i=0;i<maxLines;i++)A_2[i].reDimension(4);
  b_1=new float[maxLines];
  b_2=new float[maxLines];
  z_1=new float[maxLines];
  z_2=new float[maxLines];
}
AVertexFit::~AVertexFit()
{
  delete []A_1;
  delete []A_2;
  delete []b_1;
  delete []b_2;
}
void *AVertexFit::process(void* ptr)
{
  return ptr;
}
void AVertexFit::add(Vector &data)
{
  if(data.getDimension()<7)return;
  int num=(int)data.getValue(6);
  if((num==0&&numLines1==maxLines)||(num==1&&numLines2==maxLines))return;
  if(num==0)
    {
      b_1[numLines1]=data.getValue(0);
      z_1[numLines1]=data.getValue(5);
      for(int i=1;i<5;i++)
	A_1[numLines1].setValue(i-1,data.getValue(i));
      numLines1++;
    }
  else
    {
      b_2[numLines2]=data.getValue(0);
      z_2[numLines2]=data.getValue(5);
      for(int i=1;i<5;i++)
	A_2[numLines2].setValue(i-1,data.getValue(i));
      numLines2++;
    }
}
void AVertexFit::clear()
{
  numLines1=0;
  numLines2=0;
}
Vector AVertexFit::getResult()
{
  return re;
}
void AVertexFit::getAddPoint(Vector &data)
{
  add(data);
}

float AVertexFit::fit()
{
  if(numLines1<4||numLines2<4)return -1;
  matrixNxM A(numLines1+numLines2,6),At(6,numLines1+numLines2);
  Vector b(numLines1+numLines2),ret(6);
  b.transpose();
  ret.transpose();
  fit1();
  fit2();
  vector3D v1(re1.getValue(2),re1.getValue(3),1);
  vector3D v2(re2.getValue(2),re2.getValue(3),1);
  v1.normalize();v2.normalize();v1.setRep(_spheric_);v2.setRep(_spheric_);
  lLine3D poca=sLine3D(point3D(re1.getValue(0),re2.getValue(1),0),v1)
    -sLine3D(point3D(re2.getValue(0),re2.getValue(1),0),v2);
  point3D mid=point3D(0,0,0)+(vector3D(poca.P())+vector3D(poca.Q()))*0.5;
  for(int i=0;i<numLines1;i++)
    {
      b.setValue(i,b_1[i]);
      A.setValue(i,0,A_1[i].getValue(0));
      A.setValue(i,1,A_1[i].getValue(1));
      A.setValue(i,2,(z_1[i]-mid.Z())*A_1[i].getValue(0));
      A.setValue(i,3,(z_1[i]-mid.Z())*A_1[i].getValue(1));
      A.setValue(i,4,0);
      A.setValue(i,5,0);
    }
  for(int i=0;i<numLines2;i++)
    {
      b.setValue(i+numLines1,b_2[i]);
      A.setValue(i+numLines1,0,A_2[i].getValue(0));
      A.setValue(i+numLines1,1,A_2[i].getValue(1));
      A.setValue(i+numLines1,2,0);
      A.setValue(i+numLines1,3,0);
      A.setValue(i+numLines1,4,(z_2[i]-mid.Z())*A_2[i].getValue(0));
      A.setValue(i+numLines1,5,(z_2[i]-mid.Z())*A_2[i].getValue(1));
    }
  At=!A;
  matrixNxM A_1x(At*A);
  A_1x=-A_1x;
  if(A_1x.Lines()==0) return 100000000;
  ret=(A_1x)*(At*b);
  for(int i=0;i<6;i++)
    re.setValue(i,ret.getValue(i));
  re.setValue(6,mid.Z());
  double chi2 = 0;
  //  double tmp;
  Vector cC(numLines1+numLines2);
  cC=A*ret;
  cC=cC-b;
  chi2=cC*cC;
  if(4!=numLines1+numLines2)
    chi2=chi2/abs(4-numLines1-numLines2);
  return chi2;
}

float AVertexFit::fit1()
{
  if(numLines1<=0)return-1;
  matrixNxM A(numLines1,4),At(4,numLines1);
  Vector b(numLines1),ret(4);
  b.transpose();
  ret.transpose();
  for(int i=0;i<numLines1;i++)
    {
      b.setValue(i,b_1[i]);
      A.setValue(i,0,A_1[i].getValue(0));
      A.setValue(i,1,A_1[i].getValue(1));
      A.setValue(i,2,A_1[i].getValue(2));
      A.setValue(i,3,A_1[i].getValue(3));
    }
  At=!A;
  matrixNxM A__1(At*A);
  A__1=-A__1;
  if(A__1.Lines()==0) return 100000000;
  ret=(A__1)*(At*b);
  re1=ret;
  double chi2 = 0;
  //  double tmp;
  Vector cC(numLines1);
  cC=A*ret;
  cC=cC-b;
  chi2=cC*cC;
  if(4!=numLines1)
    chi2=chi2/abs(4-numLines1);
  return chi2;
}
float AVertexFit::fit2()
{
  if(numLines2<=0)return-1;
  matrixNxM A(numLines2,4),At(4,numLines2);
  Vector b(numLines2),ret(4);
  b.transpose();
  ret.transpose();
  for(int i=0;i<numLines2;i++)
    {
      b.setValue(i,b_2[i]);
      A.setValue(i,0,A_2[i].getValue(0));
      A.setValue(i,1,A_2[i].getValue(1));
      A.setValue(i,2,A_2[i].getValue(2));
      A.setValue(i,3,A_2[i].getValue(3));
    }
  At=!A;
  matrixNxM A__1(At*A);
  A__1=-A__1;
  if(A__1.Lines()==0) return 100000000;
  ret=(A__1)*(At*b);
  re2=ret;
  double chi2 = 0;
  //  double tmp;
  Vector cC(numLines2);
  cC=A*ret;
  cC=cC-b;
  chi2=cC*cC;
  if(4!=numLines2)
    chi2=chi2/abs(4-numLines2);
  return chi2;
}
