#include "geometry.h"
#include <iostream>

matrixNxM::matrixNxM(int n, int m):geomObject()
{
    numberOfLines=n;
    numberOfColumns=m;
    data=new double[n*m];
}

matrixNxM::matrixNxM(const matrixNxM &mat):geomObject()
{
    numberOfLines=mat.Lines();
    numberOfColumns=mat.Columns();
    data=new double[numberOfColumns*numberOfLines];
    for(int i=0;i<numberOfLines;i++)
    {
      for(int j=0;j<numberOfColumns;j++)
	data[i*numberOfColumns+j]=mat.getValue(i,j);
    }
}
#ifdef MATRIX_T
matrixNxM::matrixNxM(const matrixNxMT<double> &mat):geomObject()
{
    numberOfLines=mat.Lines();
    numberOfColumns=mat.Columns();
    data=new double[numberOfColumns*numberOfLines];
    for(int i=0;i<numberOfLines;i++)
    {
      for(int j=0;j<numberOfColumns;j++)
	data[i*numberOfColumns+j]=mat.getValue(i,j);
    }
}
matrixNxM::matrixNxM(const matrixNxMT<long double> &mat):geomObject()
{
    numberOfLines=mat.Lines();
    numberOfColumns=mat.Columns();
    data=new double[numberOfColumns*numberOfLines];
    for(int i=0;i<numberOfLines;i++)
    {
      for(int j=0;j<numberOfColumns;j++)
	data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
    }
}
matrixNxM::matrixNxM(const matrixNxMT<float> &mat):geomObject()
{
    numberOfLines=mat.Lines();
    numberOfColumns=mat.Columns();
    data=new double[numberOfColumns*numberOfLines];
    for(int i=0;i<numberOfLines;i++)
    {
      for(int j=0;j<numberOfColumns;j++)
	data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
    }
}
#endif
matrixNxM::matrixNxM(int n, int m, double *datas):geomObject()
{
    numberOfLines=n;
    numberOfColumns=m;
    data=new double[n*m];
    for(int i=0;i<numberOfLines;i++)
      {
	for(int j=0;j<numberOfColumns;j++)
	  data[i*numberOfColumns+j]=datas[i*numberOfColumns+j];
      }
}

matrixNxM::~matrixNxM()
{
    delete [] data;
}

double matrixNxM::getValue(int line, int column) const
{
    if((line<0)||(line>=numberOfLines)||(column<0)||(column>=numberOfColumns)) return -1;
    return data[line*numberOfColumns+column];
}

void matrixNxM::setValue(int line, int column, double value)
{
    if((line<0)||(line>=numberOfLines)||(column<0)||(column>=numberOfColumns)) return;
    data[line*numberOfColumns+column]=value;
}

Vector matrixNxM::operator[](int num)const// line of matrix
{
  if(num<0&&num>=numberOfLines)return Vector();
  Vector ret(numberOfColumns);
  for(int i=0;i<numberOfColumns;i++)
    ret.setValue(i,data[i+num*numberOfColumns]);
  return ret;
}
Vector matrixNxM::operator()(int num)const//column of matrix
{
  if(num<0&&num>numberOfColumns)return Vector();
  Vector v(numberOfLines);
  for(int i=0;i<numberOfLines;i++)
    v.setValue(i,data[i*numberOfColumns+num]);
  return v;
}
int matrixNxM::Columns()const
{
    return numberOfColumns;
}

int matrixNxM::Lines()const
{
    return numberOfLines;
}

void matrixNxM::Unity()
{
    if(numberOfLines!=numberOfColumns)
    {
	delete []data;
	int max=(numberOfLines<numberOfColumns?numberOfColumns:numberOfLines);
	numberOfLines=max;
	numberOfColumns=max;
	data=new double[max*max];
    }
    for(int i=0;i<numberOfLines;i++)
    {
      for(int j=0;j<numberOfLines;j++)
	data[i*numberOfColumns+j]=(i==j?1:0);
    }
}

void matrixNxM::transpose()
{
  int tmp=numberOfLines*numberOfColumns;
  double tmpv[tmp];
  for(int i=0;i<tmp;i++)
    tmpv[i]=data[i];
  for(int i=0;i<numberOfLines;i++)
    for(int j=0;j<numberOfColumns;j++)
      data[j*numberOfLines+i]=tmpv[i*numberOfColumns+j];
  tmp=numberOfLines;
  numberOfLines=numberOfColumns;
  numberOfColumns=tmp;
}

void matrixNxM::operator =(const matrixNxM &mat)
{
  if(!(((numberOfLines==mat.Lines())&&(numberOfColumns=mat.Columns()))||
       ((numberOfLines=mat.Columns())&&(numberOfColumns==mat.Lines()))))
    {
	delete []data;
	numberOfLines=mat.Lines();
	numberOfColumns=mat.Columns();
	data=new double[numberOfLines*numberOfColumns];
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
    else if(numberOfLines==mat.Lines())
    {
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
    else
    {
      int tmp=numberOfLines;
      numberOfLines=numberOfColumns;
      numberOfColumns=tmp;
      for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
	    data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
}   
#ifdef MATRIX_T
void matrixNxM::operator =(const matrixNxMT<double> &mat)
{
    if(!((numberOfLines==mat.Lines())&&(numberOfColumns=mat.Columns())||(numberOfLines=mat.Columns())&&(numberOfColumns==mat.Lines())))
    {
	delete []data;
	numberOfLines=mat.Lines();
	numberOfColumns=mat.Columns();
	data=new double[numberOfLines*numberOfColumns];
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
    else if(numberOfLines==mat.Lines())
    {
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
    else
    {
      int tmp=numberOfLines;
      numberOfLines=numberOfColumns;
      numberOfColumns=tmp;
      for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
	    data[i*numberOfColumns+j]=mat.getValue(i,j);
	}
    }
}   
void matrixNxM::operator =(const matrixNxMT<long double> &mat)
{
    if(!((numberOfLines==mat.Lines())&&(numberOfColumns=mat.Columns())||(numberOfLines=mat.Columns())&&(numberOfColumns==mat.Lines())))
    {
	delete []data;
	numberOfLines=mat.Lines();
	numberOfColumns=mat.Columns();
	data=new double[numberOfLines*numberOfColumns];
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
    else if(numberOfLines==mat.Lines())
    {
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
    else
    {
      int tmp=numberOfLines;
      numberOfLines=numberOfColumns;
      numberOfColumns=tmp;
      for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
	    data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
}   
void matrixNxM::operator =(const matrixNxMT<float> &mat)
{
    if(!((numberOfLines==mat.Lines())&&(numberOfColumns=mat.Columns())||(numberOfLines=mat.Columns())&&(numberOfColumns==mat.Lines())))
    {
	delete []data;
	numberOfLines=mat.Lines();
	numberOfColumns=mat.Columns();
	data=new double[numberOfLines*numberOfColumns];
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
    else if(numberOfLines==mat.Lines())
    {
	for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
		data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
    else
    {
      int tmp=numberOfLines;
      numberOfLines=numberOfColumns;
      numberOfColumns=tmp;
      for(int i=0;i<numberOfLines;i++)
	{
	  for(int j=0;j<numberOfColumns;j++)
	    data[i*numberOfColumns+j]=(double)mat.getValue(i,j);
	}
    }
}   
#endif
Vector operator *(const Vector &v, const matrixNxM &m)
{
    int numberOfLines=m.Lines();
    int numberOfColumns=m.Columns();
    if(v.isColumn())
    {
	if((numberOfLines>1)||(numberOfLines==0)) return Vector(0);
	if(v.getDimension()!=numberOfColumns) return Vector(0);
	double tmp=v*m(0);
	Vector ret(1);
	ret.setValue(0,tmp);
	return ret;
    }
    else
    {
	if(v.getDimension()!=numberOfLines) return Vector(0);
	Vector ret(numberOfColumns);
	for(int i=0;i<numberOfColumns;i++)
	{
	    ret.setValue(i,v*m(i));
	}
	return ret;
    }
}

Vector matrixNxM::operator *(const Vector &v)const
{
    if(v.isLine())
    {
	if((numberOfColumns>1)||(numberOfColumns==0)) return Vector(0);
	if(v.getDimension()!=numberOfLines) return Vector(0);
	Vector ret(1);
	ret.setValue(0,v*(*this)[0]);
	return ret;
    }
    else
    {
	if(v.getDimension()!=numberOfColumns) return Vector(0);
	Vector ret(numberOfLines);
	for(int i=0;i<numberOfLines;i++)
	    ret.setValue(i,v*(*this)[i]);
	ret.transpose();
	return ret;
    }
}

matrixNxM matrixNxM::operator *(const matrixNxM &m)const
{
    if(numberOfColumns!=m.Lines())return matrixNxM(0,0);
    matrixNxM ret(numberOfLines,m.Columns());
    double tmp;
    for(int i=0;i<numberOfLines;i++)
    {
	for(int j=0;j<m.Columns();j++)
	{
 	    tmp=0;
 	    for(int k=0;k<numberOfColumns;k++)
 		tmp=tmp+getValue(i,k)*m.getValue(k,j);
	    ret.setValue(i,j,tmp);
// 	    ret.setValue(i,j,(*this)[i]*m(j));
	}
    }
    return ret;
}

double matrixNxM::Determinant()const
{
  double sum=0,sum1=1,sum2=1;
  int n1,n2;
  for(int i=0;i<numberOfColumns;i++)
    {
      sum1=1;
      sum2=1;
      n1=i;
      n2=i;
      for(int j=0;j<numberOfLines;j++)
	{
	  sum1=sum1*getValue(j,n1);
	  sum2=sum1*getValue(j,n2);
	  n1++;
	  n2--;
	  if(n1>=numberOfColumns)n1=0;
	  if(n2<0)n2=numberOfColumns-1;
	}
      sum=sum+sum1-sum2;
    }
  return sum;
}
bool matrixNxM::Diagonal()const
{
  for(int i=0;i<numberOfLines;i++)
    for(int j=0;j<numberOfColumns;j++)
      if(i!=j&&abs(getValue(i,j)/*data[i*numberOfColumns+j]*/)>__prec)
	{
	  //	  cout<<i<<" "<<j<<" "<<data[i*numberOfColumns+j]<<endl;
	  return false;
	}
  return true;
}
matrixNxM matrixNxM::subset(int lineBegin, int lineEnd, int columnBegin, int columnEnd)const
{
  if(lineBegin>lineEnd||columnBegin>columnEnd
     ||columnBegin<0||columnEnd<0||lineBegin<0||lineEnd<0
     ||columnBegin>=numberOfColumns||columnEnd>=numberOfColumns
     ||lineBegin>=numberOfLines||lineEnd>=numberOfLines)return matrixNxM(0,0);
  matrixNxM ret(lineEnd-lineBegin+1,columnEnd-columnBegin+1);
  for(int i=lineBegin;i<=lineEnd;i++)
    for(int j=columnBegin;j<=columnEnd;j++)
      ret.setValue(i-lineBegin,j-columnBegin,getValue(i,j));
  return ret;
}
bool matrixNxM::UpperTriangular()const
{
  for(int i=0;i<numberOfLines;i++)
    for(int j=0;j<numberOfColumns;j++)
      if(i>j&&abs(data[i*numberOfColumns+j])>__prec)return false;
  return true;
}
bool matrixNxM::LowerTriangular()const 
{
  for(int i=0;i<numberOfLines;i++)
    for(int j=0;j<numberOfColumns;j++)
      if(i<j&&abs(data[i*numberOfColumns+j])>__prec)return false;
  return true;
}
void matrixNxM::decomposeQR(matrixNxM &Q, matrixNxM &R)const{
  if(Q.Columns()!=numberOfLines)return;
  if(R.Lines()!=numberOfColumns)return;
}
//#include  "LU_Decomposition.cpp"
//extern "C" void dinv_(int*,void*,int*,double[],int*); 
extern "C" int LUDecomposition(int lines, int columns, long double *matIn, long double *outL, long double *outU, long double *permutation);
extern "C" int LDUDecomposition(int lines, int columns, long double *matIn, long double *outL, long double *outD, long double *outU, long double *permutation);
extern "C" int InvertDiagonal(int lines, int columns, long double *matIn,long double* out);
extern "C" int InvertUTriangular(int lines, int columns, long double *matIn,long double* out);
extern "C" int InvertLTriangular(int lines, int columns, long double *matIn,long double* out);

void matrixNxM::decomposeLU(matrixNxM &L, matrixNxM &U)const
{
  long double LL[numberOfLines*numberOfLines],UU[numberOfLines*numberOfLines],MM[numberOfLines*numberOfLines],PP[numberOfLines*numberOfLines];
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)MM[i*numberOfLines+j]=data[i*numberOfLines+j];
  //  matrixNxM P(numberOfLines,numberOfLines);
  LUDecomposition(numberOfLines,numberOfColumns,MM,LL, UU, PP);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)L.setValue(i,j,LL[i*numberOfLines+j]);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)U.setValue(i,j,UU[i*numberOfLines+j]);
}
void matrixNxM::decomposeLDU(matrixNxM &L, matrixNxM &D, matrixNxM &U)const
{
  long double LL[numberOfLines*numberOfLines],UU[numberOfLines*numberOfLines],MM[numberOfLines*numberOfLines],PP[numberOfLines*numberOfLines],DD[numberOfLines*numberOfLines];
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)MM[i*numberOfLines+j]=data[i*numberOfLines+j];
  //  matrixNxM P(numberOfLines,numberOfLines);
  LDUDecomposition(numberOfLines, numberOfColumns, MM, LL, DD, UU, PP);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)L.setValue(i,j,LL[i*numberOfLines+j]);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)U.setValue(i,j,UU[i*numberOfLines+j]);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)D.setValue(i,j,DD[i*numberOfLines+j]);
//   matrixNxM P(numberOfLines,numberOfLines);
//   LDUDec(*this,L, D, U, P);
}
matrixNxM matrixNxM::operator-()const
{
  double det=Determinant();
  if(det==0){return matrixNxM(0,0);}
  if(Diagonal())
    {
      //cout<<"invert diagonal"<<flush;
      matrixNxM ret(*this);
      for(int i=0;i<numberOfColumns;i++)
	ret.setValue(i,i,(getValue(i,i)!=0?1/getValue(i,i):0));
      //cout<<"-"<<endl;
      return ret;
    }
  if(UpperTriangular())
    {
      //cout<<"invert upper Triangular"<<flush;
      long double UU[numberOfLines*numberOfLines],MM[numberOfLines*numberOfLines];
      for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)MM[i*numberOfLines+j]=data[i*numberOfLines+j];
      if(!InvertUTriangular(numberOfLines,numberOfColumns,MM,UU))
	return matrixNxM(0,0);
      matrixNxM U_1(numberOfLines,numberOfLines);
      for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)U_1.setValue(i,j,UU[i*numberOfLines+j]);
      //cout<<"-"<<endl;
      return U_1;
    }
  if(LowerTriangular())
    {
      //cout<<"invert lower triangular"<<flush;
      long double UU[numberOfLines*numberOfLines],MM[numberOfLines*numberOfLines];
      for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)MM[i*numberOfLines+j]=data[i*numberOfLines+j];
      if(!InvertLTriangular(numberOfLines,numberOfColumns,MM,UU))
	return matrixNxM(0,0);
      matrixNxM L_1(numberOfLines,numberOfLines);
      for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)L_1.setValue(i,j,UU[i*numberOfLines+j]);
      //cout<<"-"<<endl;
      return L_1;
    }
  if(numberOfColumns==2&&numberOfLines==2)
    {
      matrixNxM ret(2,2);
      ret.setValue(0,0,getValue(1,1));
      ret.setValue(0,1,-getValue(0,1));
      ret.setValue(1,0,-getValue(1,0));
      ret.setValue(1,1,getValue(0,0));
      return ret*(1/det);
    }
  if(numberOfColumns==3&&numberOfLines==3)
    {
      matrixNxM ret(3,3);
      ret.setValue(0,0,getValue(1,1)*getValue(2,2)-getValue(1,2)*getValue(2,1));
      ret.setValue(0,1,getValue(0,2)*getValue(2,1)-getValue(0,1)*getValue(2,2));
      ret.setValue(0,2,getValue(0,1)*getValue(1,2)-getValue(0,2)*getValue(1,1));
      ret.setValue(1,0,getValue(1,2)*getValue(2,0)-getValue(1,0)*getValue(2,2));
      ret.setValue(1,1,getValue(0,0)*getValue(2,2)-getValue(0,2)*getValue(2,0));
      ret.setValue(1,2,getValue(0,2)*getValue(1,0)-getValue(0,0)*getValue(1,2));
      ret.setValue(2,0,getValue(1,0)*getValue(2,1)-getValue(1,1)*getValue(2,0));
      ret.setValue(2,1,getValue(0,1)*getValue(2,0)-getValue(0,0)*getValue(2,1));
      ret.setValue(2,2,getValue(0,0)*getValue(1,1)-getValue(0,1)*getValue(1,0));
      return ret*(1/det);
    }
  long double LL[numberOfLines*numberOfLines],UU[numberOfLines*numberOfLines],MM[numberOfLines*numberOfLines],PP[numberOfLines*numberOfLines],L_L[numberOfLines*numberOfLines],U_U[numberOfLines*numberOfLines];
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)MM[i*numberOfLines+j]=data[i*numberOfLines+j];
  //  matrixNxM P(numberOfLines,numberOfLines);
  //cout<<"LUDec "<<flush;
  if(!LUDecomposition(numberOfLines,numberOfColumns,MM,LL, UU, PP))
    {
      cout<<"decomposition failed"<<endl;
      return matrixNxM(0,0);
    }
  //  cout<<"invL "<<flush;
  if(!InvertLTriangular(numberOfLines,numberOfColumns,LL,L_L))
    {
      cout<<"U-inversion failed"<<endl;
    return matrixNxM(0,0);
    }
  //  cout<<"invU "<<flush;
  if(!InvertUTriangular(numberOfLines,numberOfColumns,UU,U_U))
    {
      cout<<"L-inversion failed"<<endl;
    return matrixNxM(0,0);
    }
  //  cout<<"."<<flush;
  matrixNxM L_1(numberOfLines,numberOfLines),U_1(numberOfLines,numberOfLines),P(numberOfLines,numberOfLines);
  matrixNxM ret(numberOfLines,numberOfLines);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)L_1.setValue(i,j,L_L[i*numberOfLines+j]);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)U_1.setValue(i,j,U_U[i*numberOfLines+j]);
  for(int i=0;i<numberOfLines;i++)for(int j=0;j<numberOfLines;j++)P.setValue(i,j,PP[i*numberOfLines+j]);
//   ret=P*U_1*L_1;
//   cout<<"-"<<flush;
//   matrixNxM test=(*this)*ret;
//   if(test.Determinant()>2)
//     cout<<"inversion Failed:\n"<<test<<*this<<ret<<endl;
//  cout<<"-"<<endl;
  return P*U_1*L_1;
}

matrixNxM matrixNxM::operator *(double value)const
{
    matrixNxM ret(numberOfLines,numberOfColumns);
    for(int i=0;i<numberOfLines;i++)
    {
	for(int j=0;j<numberOfColumns;j++)
	    ret.setValue(i,j,data[i*numberOfColumns+j]*value);
    }
    return ret;
}

matrixNxM matrixNxM::operator+(const matrixNxM &m)const
{
    if((numberOfLines!=m.Lines())||(numberOfColumns!=m.Columns()))
	return matrixNxM(0,0);
    matrixNxM ret(numberOfLines,numberOfColumns);
    for(int i=0;i<numberOfLines;i++)
    {
	for(int j=0;j<numberOfColumns;j++)
	    ret.setValue(i,j,data[i*numberOfColumns+j]+m.getValue(i,j));
    }
    return ret;
}

matrixNxM matrixNxM::operator-(const matrixNxM &m)const
{
    if((numberOfLines!=m.Lines())||(numberOfColumns!=m.Columns()))
	return matrixNxM(0,0);
    matrixNxM ret(numberOfLines,numberOfColumns);
    for(int i=0;i<numberOfLines;i++)
    {
	for(int j=0;j<numberOfColumns;j++)
	    ret.setValue(i,j,data[i*numberOfColumns+j]-m.getValue(i,j));
    }
    return ret;
}

matrixNxM matrixNxM::operator!()const
{
    matrixNxM ret(numberOfColumns,numberOfLines);
    for(int i=0;i<numberOfLines;i++)
      for(int j=0;j<numberOfColumns;j++)
	ret.setValue(j,i,data[i*numberOfColumns+j]);
    return ret;
}
//#ifdef USEQT
string matrixNxM::toString()const
{
  string ret="(";
  for(int i=0;i<Lines();i++)
    {
      for(int j=0;j<Columns();j++)
	ret+=string_number(data[i*numberOfColumns+j],'e',5)+"\t";
      ret=ret.substr(0,ret.length()-1)+"\n ";
    }
  ret=ret.substr(0,ret.length()-2)+")";
  return ret;
}
//#endif
ostream & operator << (ostream & o, const matrixNxM &m)
{
  o.precision(5);
  o<<"(";
  o.width(7);
  for(int i=0;i<m.Lines();i++)
    {
      for(int j=0;j<m.Columns();j++)
	{
	  o.width(7);
	  o<< string_number(m.getValue(i,j),'e',5).data()<<"\t";
	}
      if(i==m.Lines()-1) o<<")";
      else o<<"\n ";
    }
  o<<"\n";
  return o;
}

