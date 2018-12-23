#include <stdio.h>
int LDUDecomposition(int lines, int columns, long double *matIn,long double *outL, long double *outD, long double *outU, long double *permutation);
int multiplyMatrix(int lines, int columns,long double *m1, long double *m2, long double *result);
void permutationMatrix_L(int lines,int columns, int line1, int line2, int line, long double *result);
void printMatrix(int lines, int columns, long double *matr);
int LUDecomposition(int lines, int columns, long double *matIn, long double *outL, long double *outU, long double *permutation)
{
  if(lines!=columns)return 0;
  int i,j,k;
  //initialize
  for(i=0;i<lines;i++)for(j=0;j<columns;j++)permutation[i*columns+j]=(i==j?1:0);
  for(i=0;i<lines;i++)for(j=0;j<columns;j++)outL[i*columns+j]=(i==j?1:0);
  for(i=0;i<lines;i++)for(j=0;j<columns;j++)outU[i*columns+j]=matIn[i*columns+j];
  //working structures
  long double tmpL[lines*columns];
  long double tmpL_1[lines*columns];
  long double ptmp[lines*columns];
  long double ptmp1[lines*columns];
  //permutation before decomposition; diagonal elements != 0
  int permutated=0;
  for(i=0;i<columns;i++)
    {
      if(outU[i*columns+i]==0)
	{
	  permutated=1;
	  int p=-1;
	  for(j=0;j<columns;j++)
	    if(outU[i*columns+j]!=0&&outU[j*columns+i]!=0)
	      {
		p=j;
		break;
	      }
	  if(p==-1)
	    return 0;
	  permutationMatrix_L(lines,lines,i, p,0,ptmp1);
	  multiplyMatrix(lines,columns,permutation,ptmp1,ptmp);
	  for(i=0;i<lines;i++)for(k=0;k<columns;k++)permutation[i*columns+k]=ptmp[i*columns+k];
	}
    }
  //multiply permutation matrix mat = P-¹ * mat
  if(permutated==1)
    {
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)ptmp[i*columns+k]=permutation[k*columns+i];
      multiplyMatrix(lines,columns,outU,ptmp,ptmp1);
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)outU[i*columns+k]=ptmp1[i*columns+k];
    }
  //decompose: Doolittle Algorithm
  for(j=0;j<columns-1;j++)
    {
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)tmpL[i*columns+k]=(i==k?1:0);
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)tmpL_1[i*columns+k]=(i==k?1:0);
      for(i=j+1;i<lines;i++)
	{
	  tmpL[i*columns+j]=-outU[i*columns+j]/outU[j*columns+j];
	  tmpL_1[i*columns+j]=-tmpL[i*columns+j];
	}
      multiplyMatrix(lines,columns,tmpL,outU,ptmp1);
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)outU[i*columns+k]=ptmp1[i*columns+k];
      multiplyMatrix(lines,columns,outL,tmpL_1,ptmp1);
      for(i=0;i<lines;i++)for(k=0;k<columns;k++)outL[i*columns+k]=ptmp1[i*columns+k];
    }
  return 1;
}
int LDUDecomposition(int lines, int columns, long double *matIn,long double *outL, long double *outD, long double *outU, long double *permutation)
{
  int i,j;
  if(!LUDecomposition(lines, columns,matIn,outL, outU, permutation))return 0;
  for(i=0;i<lines;i++)
    {
      for(j=i+1;j<lines;j++)
	outD[i*columns+j]=0;
      for(j=0;j<i;j++)
	outD[i*columns+j]=0;
      outD[i*columns+i]=outU[i*columns+i];
      outU[i*columns+i]=1;
    }
  return 1;
}
int multiplyMatrix(int lines, int columns,long double *m1, long double *m2, long double *result)
{
  long double tmp;
  int i,j,k;
  for(i=0;i<lines;i++)
    for(j=0;j<columns;j++)
      {
	tmp=0;
	for(k=0;k<columns;k++)
	  tmp+=m1[i*columns+k]*m2[k*columns+j];
	result[i*columns+j]=tmp;
      }
  return 1;
}
void permutationMatrix_L(int lines,int columns, int line1, int line2, int line, long double *result)
{
  if(line1==line2||line1<0||line2<0||((line1>=lines||line2>=lines)&&line)
     ||((line1>=columns||line2>=columns)&&!line))return;
  long double tmp=1;
  int i,j,dl=line1-line2;
  if(dl<0)dl=dl*-1;
  if((dl%2)==1)tmp=-1;
  for(i=0;i<lines;i++)for(j=0;j<columns;j++)result[i*columns+j]=(i==j?tmp:0.);
  if(line)
    {
      for(i=0;i<columns;i++)
	{
	  result[line1*columns+i]=(i==line2?tmp:0);
	  result[line2*columns+i]=(i==line1?tmp:0);
	}
    }
  else
    {
      for(i=0;i<lines;i++)
	{
	  result[i*columns+line1]=(i==line2?tmp:0);
	  result[i*columns+line2]=(i==line1?tmp:0);
	}
    }
}
void printMatrix(int lines, int columns, long double *matr)
{
  int i,j;
  for(i=0;i<lines;i++)
    {
      for(j=0;j<columns;j++)
	{
	  printf("%f\t",(double)matr[i*columns+j]);
	}
      printf("\n");
    }
  printf("\n");
}
