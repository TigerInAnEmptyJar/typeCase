#include <stdlib.h>
#include "geometry.h"
matrixNxM permutationMatrix(int,int,int,int,bool);
void permutationMatrix_L(int lines,int columns, int line1, int line2, bool line, long double *result);
bool LDUDec(const matrixNxM &matIn,matrixNxM &outL, matrixNxM &outD, matrixNxM &outU, matrixNxM &permutation);
bool InvertDiagonal(const matrixNxM &matIn,matrixNxM &out);
bool InvertUTriangular(const matrixNxM &matIn, matrixNxM &out);
bool InvertLTriangular(const matrixNxM &matIn, matrixNxM &out);
#ifdef MATRIX_T
template<class elements>matrixNxMT<elements> permutationMatrix_T(int,int,int,int,bool);
template<class elements> bool LDUDec(const matrixNxMT<elements> &matIn,matrixNxMT<elements> &outL, matrixNxMT<elements> &outD, matrixNxMT<elements> &outU, matrixNxMT<elements> &permutation);
template<class elements> bool InvertDiagonal(const matrixNxMT<elements> &matIn,matrixNxMT<elements> &out);
template<class elements> bool InvertUTriangular(const matrixNxMT<elements> &matIn, matrixNxMT<elements> &out);
template<class elements> bool InvertLTriangular(const matrixNxMT<elements> &matIn, matrixNxMT<elements> &out);
#if __GNUC__ < 4
template<> bool LDUDec<int>(const matrixNxMT<int> &matIn,matrixNxMT<int> &outL, matrixNxMT<int> &outD, matrixNxMT<int> &outU, matrixNxMT<int> &permutation);
template<> bool InvertDiagonal<int>(const matrixNxMT<int> &matIn,matrixNxMT<int> &out);
template<> bool InvertUTriangular<int>(const matrixNxMT<int> &matIn, matrixNxMT<int> &out);
template<> bool InvertLTriangular<int>(const matrixNxMT<int> &matIn, matrixNxMT<int> &out);
template<> bool LDUDec<float>(const matrixNxMT<float> &matIn,matrixNxMT<float> &outL, matrixNxMT<float> &outD, matrixNxMT<float> &outU, matrixNxMT<float> &permutation);
template<> bool InvertDiagonal<float>(const matrixNxMT<float> &matIn,matrixNxMT<float> &out);
template<> bool InvertUTriangular<float>(const matrixNxMT<float> &matIn, matrixNxMT<float> &out);
template<> bool InvertLTriangular<float>(const matrixNxMT<float> &matIn, matrixNxMT<float> &out);
template<> bool LDUDec<double>(const matrixNxMT<double> &matIn,matrixNxMT<double> &outL, matrixNxMT<double> &outD, matrixNxMT<double> &outU, matrixNxMT<double> &permutation);
template<> bool InvertDiagonal<double>(const matrixNxMT<double> &matIn,matrixNxMT<double> &out);
template<> bool InvertUTriangular<double>(const matrixNxMT<double> &matIn, matrixNxMT<double> &out);
template<> bool InvertLTriangular<double>(const matrixNxMT<double> &matIn, matrixNxMT<double> &out);
template<> bool LDUDec<long double>(const matrixNxMT<long double> &matIn,matrixNxMT<long double> &outL, matrixNxMT<long double> &outD, matrixNxMT<long double> &outU, matrixNxMT<long double> &permutation);
template<> bool InvertDiagonal<long double>(const matrixNxMT<long double> &matIn,matrixNxMT<long double> &out);
template<> bool InvertUTriangular<long double>(const matrixNxMT<long double> &matIn, matrixNxMT<long double> &out);
template<> bool InvertLTriangular<long double>(const matrixNxMT<long double> &matIn, matrixNxMT<long double> &out);
#endif
#endif
bool multiplyMatr(int lines, int columns,long double *m1, long double *m2, long double *result);
void printMatr(int lines,int columns, long double *mat);
bool LUDec(const matrixNxM &matIn,matrixNxM &outL, matrixNxM &outU, matrixNxM &permutation)
{
  if(matIn.Lines()!=matIn.Columns())return false;
  int lines=matIn.Lines();
  int columns=matIn.Lines();
//   matrixNxMT<long double> OL(lines,columns);
//   matrixNxMT<long double> OU(lines,columns);
//   OL.Unity();
//   for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OU.setValue(i,j,matIn.getValue(i,j));
//   matrixNxMT<long double> tmpL(OL);
//   matrixNxMT<long double> tmpL_1(OL);
//   matrixNxMT<long double> ptmp(lines,lines);
//   ptmp.Unity();
  permutation.Unity();
  long double OL[lines*columns];
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OL[i*columns+j]=(i==j?1:0);
  long double OU[lines*columns];
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OU[i*columns+j]=matIn.getValue(i,j);
  long double tmpL[lines*columns];
  long double tmpL_1[lines*columns];
  long double ptmp[lines*columns];
  long double ptmp1[lines*columns];
  long double ptmp2[lines*columns];
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)ptmp[i*columns+j]=(i==j?1:0);
  bool permut=false;
  for(int j=0;j<columns-1;j++)
    {
      for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)tmpL[i*columns+j]=(i==j?1:0);
      for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)tmpL_1[i*columns+j]=(i==j?1:0);
      //      tmpL.Unity();
      //      tmpL_1.Unity();
      if(OU[j*columns+j]/*.getValue(j,j)*/==0)
	{
	  permut=true;
	  //	  cout<<"shit permutation necessary"<<endl;
	  int p=-1;
	  for(int i=j+1;i<columns;i++)
	    if(OU[j*columns+i]/*.getValue(i,j)*/!=0)
	      {
		p=i;
		break;
	      }
	  if(p==-1){
	    //	    cout<<"shit permutation failed"<<endl;
	    return false;
	  }
	  permutationMatrix_L(lines,lines,j, p,false,ptmp1);
// 	  printMatr(lines,columns,OU);
// 	  printMatr(lines,columns,ptmp1);
	  //P=P*P'
	  multiplyMatr(lines,columns,ptmp,ptmp1,ptmp2);
	  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)ptmp[i*columns+j]=ptmp2[i*columns+j];
	  //OL=OL*P-¹
// 	  multiplyMatr(lines,columns,ptmp1,OL,ptmp2);
// 	  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OL[i*columns+j]=ptmp2[i*columns+j];
// 	  //	  multiplyMatr(lines,columns,ptmp1,OL,ptmp2);
// 	  printMatr(lines,columns,OL);
	  //OU=P*OU
	  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)ptmp2[i*columns+j]=ptmp1[j*columns+i];
	  multiplyMatr(lines,columns,OU,ptmp2,ptmp1);
	  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OU[i*columns+j]=ptmp1[i*columns+j];
	  //	  printMatr(lines,columns,OU);
// 	  OU=ptmp*OU;
// 	  OL=OL*!ptmp;
	  //	  permutation=ptmp*permutation;
	}
      for(int i=j+1;i<lines;i++)
	{
// 	  tmpL.setValue(i,j,-OU.getValue(i,j)/OU.getValue(j,j));
// 	  tmpL_1.setValue(i,j,-tmpL.getValue(i,j));
	  tmpL[i*columns+j]=-OU[i*columns+j]/OU[j*columns+j];
	  tmpL_1[i*columns+j]=-tmpL[i*columns+j];
	}
//       if(permut)
// 	{
// 	  cout<<"permutation: iteration "<<j<<endl;
// 	  printMatr(lines,columns,OU);
// 	  printMatr(lines,columns,tmpL);
// 	  printMatr(lines,columns,OL);
// 	}
      multiplyMatr(lines,columns,tmpL,OU,ptmp1);
      for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OU[i*columns+j]=ptmp1[i*columns+j];
      multiplyMatr(lines,columns,OL,tmpL_1,ptmp1);
      for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)OL[i*columns+j]=ptmp1[i*columns+j];
//       OU=(tmpL)*OU;
//       OL=tmpL_1*OL;
    }
//   for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)outU.setValue(i,j,OU.getValue(i,j));
//   for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)outL.setValue(i,j,OL.getValue(i,j));
//   for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)permutation.setValue(i,j,ptmp.getValue(i,j));
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)outU.setValue(i,j,OU[i*columns+j]);
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)outL.setValue(i,j,OL[i*columns+j]);
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)permutation.setValue(i,j,ptmp[j*columns+i]);
  return true;
}

bool LDUDec(const matrixNxM &matIn,matrixNxM &outL, matrixNxM &outD, matrixNxM &outU, matrixNxM &permutation)
{
  if(!LUDec(matIn,outL, outU, permutation))return false;
  outD=outU;
  for(int i=0;i<matIn.Lines();i++)
    {
      outU.setValue(i,i,1);
      for(int j=i+1;j<matIn.Lines();j++)
	outD.setValue(i,j,0);
    }
  return true;
}

bool InvertDiagonal(const matrixNxM &matIn,matrixNxM &out)
{
  if(!matIn.Diagonal())return false;
  out=matIn;
  for(int i=0;i<matIn.Lines();i++)
    if(out.getValue(i,i)!=0)
      out.setValue(i,i,1/out.getValue(i,i));
  return true;
}
#ifdef MATRIX_T
template<class elements> bool InvertDiagonal(const matrixNxMT<elements> &matIn,matrixNxMT<elements> &out)
{
  if(!matIn.Diagonal())return false;
  out=matIn;
  for(int i=0;i<matIn.Lines();i++)
    if(out.getValue(i,i)!=0)
      out.setValue(i,i,1/out.getValue(i,i));
  return true;
}
#endif
bool InvertUTriangular(const matrixNxM &matIn, matrixNxM &out)
{
  //inverts an upper triangular quadratic matrix with determinant!=0 (non-singular)
  out=!matIn;
  long double value;
  int lines=matIn.Lines();
  if(!(matIn.Lines()==matIn.Columns()))return false;
  value=1;for(int i=0;i<matIn.Lines();i++)value*=matIn.getValue(i,i);
  if(value==0)return false;
  long double tmp[lines][lines];
  for(int i=matIn.Lines()-1;i>=0;i--)
    {
      tmp[i][i]=1/matIn.getValue(i,i);
      for(int j=0;j<matIn.Lines();j++)
	{
	  if(i==j)
	    tmp[i][j]=1/matIn.getValue(i,j);
	  else if(i>j)
	    tmp[i][j]=0;
	  else 
	    {
	      value=0;
	      for(int k=i+1;k<=j;k++)
		value+=matIn.getValue(i,k)*tmp[k][j];
	      tmp[i][j]=-value*tmp[i][i];
	    }
	}
    }
  for(int i=0;i<lines;i++)for(int j=0;j<lines;j++)out.setValue(i,j,tmp[i][j]);
  return true;
}
#ifdef MATRIX_T
template<class elements> bool InvertUTriangular(const matrixNxMT<elements> &matIn, matrixNxMT<elements> &out)
{
  //inverts an upper triangular quadratic matrix with determinant!=0 (non-singular)
  out=!matIn;
  long double value;
  int lines=matIn.Lines();
  if(!(matIn.Lines()==matIn.Columns()))return false;
  value=1;for(int i=0;i<matIn.Lines();i++)value*=matIn.getValue(i,i);
  if(value==0)return false;
  long double tmp[lines][lines];
  for(int i=matIn.Lines()-1;i>=0;i--)
    {
      tmp[i][i]=1/matIn.getValue(i,i);
      for(int j=0;j<matIn.Lines();j++)
	{
	  if(i==j)
	    tmp[i][j]=1/matIn.getValue(i,j);
	  else if(i>j)
	    tmp[i][j]=0;
	  else 
	    {
	      value=0;
	      for(int k=i+1;k<=j;k++)
		value+=matIn.getValue(i,k)*tmp[k][j];
	      tmp[i][j]=-value*tmp[i][i];
	    }
	}
    }
  for(int i=0;i<lines;i++)for(int j=0;j<lines;j++)out.setValue(i,j,tmp[i][j]);
  return true;
}
#endif
bool InvertLTriangular(const matrixNxM &matIn, matrixNxM &out)
{
  //inverts a lower triangular quadratic matrix with determinant!=0 (non-singular)
  out=!matIn;
  long double value;
  int lines=matIn.Lines();
  if(!(matIn.Lines()==matIn.Columns()))return false;
  value=1;for(int i=0;i<matIn.Lines();i++)value*=matIn.getValue(i,i);
  if(value==0)return false;
  long double tmp[lines][lines];
  for(int i=0;i<lines;i++)
    {
      tmp[i][i]=1/matIn.getValue(i,i);
      for(int j=lines-1;j>=0;j--)
	{
	  if(i<j)
	    tmp[i][j]=0;
	  else if(i==j)
	    tmp[i][j]=1/matIn.getValue(i,j);
	  else //i>j
	    {
	      value=0;
	      for(int k=j;k<i;k++)
		value+=matIn.getValue(i,k)*tmp[k][j];
	      tmp[i][j]=-value*tmp[i][i];
	    }
	}
    }
  for(int i=0;i<lines;i++)for(int j=0;j<lines;j++)out.setValue(i,j,tmp[i][j]);
  return true;
}
#ifdef MATRIX_T
template<class elements>bool InvertLTriangular(const matrixNxMT<elements> &matIn, matrixNxMT<elements> &out)
{
  //inverts a lower triangular quadratic matrix with determinant!=0 (non-singular)
  out=!matIn;
  long double value;
  int lines=matIn.Lines();
  if(!(matIn.Lines()==matIn.Columns()))return false;
  value=1;for(int i=0;i<matIn.Lines();i++)value*=matIn.getValue(i,i);
  if(value==0)return false;
  long double tmp[lines][lines];
  for(int i=0;i<lines;i++)
    {
      tmp[i][i]=1/matIn.getValue(i,i);
      for(int j=lines-1;j>=0;j--)
	{
	  if(i<j)
	    tmp[i][j]=0;
	  else if(i==j)
	    tmp[i][j]=1/matIn.getValue(i,j);
	  else //i>j
	    {
	      value=0;
	      for(int k=j;k<i;k++)
		value+=matIn.getValue(i,k)*tmp[k][j];
	      tmp[i][j]=-value*tmp[i][i];
	    }
	}
    }
  for(int i=0;i<lines;i++)for(int j=0;j<lines;j++)out.setValue(i,j,tmp[i][j]);
  return true;
}
template<class elements>matrixNxMT<elements> permutationMatrix_T(int lines,int columns, int line1, int line2, bool line)
{
  matrixNxMT<elements> ret(lines,columns);
  ret.Unity();
  if(line1==line2||line1<0||line2<0||((line1>=lines||line2>=lines)&&line)
     ||((line1>=columns||line2>=columns)&&!line))return ret;
  if(line)
    {
      for(int i=0;i<columns;i++)
	{
	  ret.setValue(line1,i,(i==line2?1:0));
	  ret.setValue(line2,i,(i==line1?1:0));
	}
    }
  else
    {
      for(int i=0;i<lines;i++)
	{
	  ret.setValue(i,line1,(i==line2?1:0));
	  ret.setValue(i,line2,(i==line1?1:0));
	}
    }
  if((abs(line1-line2)%2)==1)return ret*-1;
  return ret;
}
#endif
void permutationMatrix_L(int lines,int columns, int line1, int line2, bool line, long double *result)
{
//   matrixNxMT<elements> ret(lines,columns);
//   ret.Unity();
  if(line1==line2||line1<0||line2<0||((line1>=lines||line2>=lines)&&line)
     ||((line1>=columns||line2>=columns)&&!line))return ;
  long double tmp=1;
  if((abs(line1-line2)%2)==1)tmp=-1;
  for(int i=0;i<lines;i++)for(int j=0;j<columns;j++)result[i*columns+j]=(i==j?tmp:0.);
  if(line)
    {
      for(int i=0;i<columns;i++)
	{
	  result[line1*columns+i]=(i==line2?tmp:0);
	  result[line2*columns+i]=(i==line1?tmp:0);
	}
    }
  else
    {
      for(int i=0;i<lines;i++)
	{
	  result[i*columns+line1]=(i==line2?tmp:0);
	  result[i*columns+line2]=(i==line1?tmp:0);
	}
    }
}
bool multiplyMatr(int lines, int columns,long double *m1, long double *m2, long double *result)
{
  long double tmp;
  for(int i=0;i<lines;i++)
    for(int j=0;j<columns;j++)
      {
	tmp=0;
	for(int k=0;k<columns;k++)
	  tmp+=m1[i*columns+k]*m2[k*columns+j];
	result[i*columns+j]=tmp;
      }
  return true;
}
void printMatr(int lines,int columns, long double *mat)
{
  for(int i=0;i<lines;i++)
    {
      for(int j=0;j<columns;j++)
	{
	  cout.width(6);
	  cout<<mat[i*columns+j]<<"\t";
	}
      cout<<endl;
    }
  cout<<endl;
}
#ifdef MATRIX_T
#if __GNUC__ > 3
template<> bool LDUDec<int>(const matrixNxMT<int> &matIn,matrixNxMT<int> &outL, matrixNxMT<int> &outD, matrixNxMT<int> &outU, matrixNxMT<int> &permutation);
template<> bool InvertDiagonal<int>(const matrixNxMT<int> &matIn,matrixNxMT<int> &out);
template<> bool InvertUTriangular<int>(const matrixNxMT<int> &matIn, matrixNxMT<int> &out);
template<> bool InvertLTriangular<int>(const matrixNxMT<int> &matIn, matrixNxMT<int> &out);
template<> bool LDUDec<long>(const matrixNxMT<long> &matIn,matrixNxMT<long> &outL, matrixNxMT<long> &outD, matrixNxMT<long> &outU, matrixNxMT<long> &permutation);
template<> bool InvertDiagonal<long>(const matrixNxMT<long> &matIn,matrixNxMT<long> &out);
template<> bool InvertUTriangular<long>(const matrixNxMT<long> &matIn, matrixNxMT<long> &out);
template<> bool InvertLTriangular<long>(const matrixNxMT<long> &matIn, matrixNxMT<long> &out);
template<> bool LDUDec<float>(const matrixNxMT<float> &matIn,matrixNxMT<float> &outL, matrixNxMT<float> &outD, matrixNxMT<float> &outU, matrixNxMT<float> &permutation);
template<> bool InvertDiagonal<float>(const matrixNxMT<float> &matIn,matrixNxMT<float> &out);
template<> bool InvertUTriangular<float>(const matrixNxMT<float> &matIn, matrixNxMT<float> &out);
template<> bool InvertLTriangular<float>(const matrixNxMT<float> &matIn, matrixNxMT<float> &out);
template<> bool LDUDec<double>(const matrixNxMT<double> &matIn,matrixNxMT<double> &outL, matrixNxMT<double> &outD, matrixNxMT<double> &outU, matrixNxMT<double> &permutation);
template<> bool InvertDiagonal<double>(const matrixNxMT<double> &matIn,matrixNxMT<double> &out);
template<> bool InvertUTriangular<double>(const matrixNxMT<double> &matIn, matrixNxMT<double> &out);
template<> bool InvertLTriangular<double>(const matrixNxMT<double> &matIn, matrixNxMT<double> &out);
template<> bool LDUDec<long double>(const matrixNxMT<long double> &matIn,matrixNxMT<long double> &outL, matrixNxMT<long double> &outD, matrixNxMT<long double> &outU, matrixNxMT<long double> &permutation);
template<> bool InvertDiagonal<long double>(const matrixNxMT<long double> &matIn,matrixNxMT<long double> &out);
template<> bool InvertUTriangular<long double>(const matrixNxMT<long double> &matIn, matrixNxMT<long double> &out);
template<> bool InvertLTriangular<long double>(const matrixNxMT<long double> &matIn, matrixNxMT<long double> &out);
#endif
#endif
