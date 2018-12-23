#include <stdio.h>
int InvertDiagonal(int lines, int columns, long double *matIn,long double* out)
{
  int i,j;
  for(i=0;i<lines;i++)
    for(j=0;j<lines;j++)
      {
	if(i!=j&&matIn[i*columns+j]!=0)return 0;
	if(i==j)
	  out[i*columns+j]=1/matIn[i*columns+j];
	else
	  out[i*columns+j]=matIn[i*columns+j];
      }
  return 1;
}
int InvertUTriangular(int lines, int columns, long double *matIn,long double* out)
{
  //inverts an upper triangular quadratic matrix with determinant!=0 (non-singular)
  long double value=1;
  int i,j,k;
  if(!(lines==columns))return 0;
  for(i=0;i<lines;i++)value*=matIn[i*columns+i];
  if(value==0)return 0;
  long double tmp[lines*lines];
  for(i=lines-1;i>=0;i--)
    {
      tmp[i*columns+i]=1/matIn[i*columns+i];
      for(j=0;j<lines;j++)
	{
	  if(i==j)
	    tmp[i*columns+j]=1/matIn[i*columns+j];
	  else if(i>j)
	    tmp[i*columns+j]=0;
	  else 
	    {
	      value=0;
	      for(k=i+1;k<=j;k++)
		value+=matIn[i*columns+k]*tmp[k*columns+j];
	      tmp[i*columns+j]=-value*tmp[i*columns+i];
	    }
	}
    }
  for(i=0;i<lines;i++)for(j=0;j<lines;j++)out[i*columns+j]=tmp[i*columns+j];
  return 1;
}
int InvertLTriangular(int lines, int columns, long double *matIn,long double* out)
{
  //inverts a lower triangular quadratic matrix with determinant!=0 (non-singular)
  long double value;
  int i,j,k;
  if(!(lines==columns))return 0;
  value=1;for(i=0;i<lines;i++)value*=matIn[i*columns+i];
  if(value==0)return 0;
  long double tmp[lines][lines];
  for(i=0;i<lines;i++)
    {
      tmp[i][i]=1/matIn[i*columns+i];
      for(j=lines-1;j>=0;j--)
	{
	  if(i<j)
	    tmp[i][j]=0;
	  else if(i==j)
	    tmp[i][j]=1/matIn[i*columns+j];
	  else //i>j
	    {
	      value=0;
	      for(k=j;k<i;k++)
		value+=matIn[i*columns+k]*tmp[k][j];
	      tmp[i][j]=-value*tmp[i][i];
	    }
	}
    }
  for(i=0;i<lines;i++)for(j=0;j<lines;j++)out[i*columns+j]=tmp[i][j];
  return 1;
}
