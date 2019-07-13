#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif
matrix3D::matrix3D(const vector3D& axis, double angle) : unit(1), reel(1)
{
  //  data=new double[9];
  vector3D as(axis);
  as.normalize();
  double a = as.X();
  double b = as.Y();
  double c = as.Z();
  double s = sin((double)angle);
  double o = cos((double)angle);
  data[0] = o + a * a * (1 - o);
  data[1] = -c * s + a * b * (1 - o);
  data[2] = b * s + a * c * (1 - o);
  data[3] = c * s + a * b * (1 - o);
  data[4] = o + b * b * (1 - o);
  data[5] = -a * s + b * c * (1 - o);
  data[6] = -b * s + a * c * (1 - o);
  data[7] = a * s + b * c * (1 - o);
  data[8] = o + c * c * (1 - o);
}

matrix3D::matrix3D(const vector3D& l1, const vector3D& l2, const vector3D& l3)
{
  //  data=new double[9];
  data[0] = l1.X();
  data[1] = l1.Y();
  data[2] = l1.Z();
  data[3] = l2.X();
  data[4] = l2.Y();
  data[5] = l2.Z();
  data[6] = l3.X();
  data[7] = l3.Y();
  data[8] = l3.Z();
}

matrix3D::~matrix3D()
{
  //  delete []data;
}
vector3D matrix3D::operator[](int num) const // returns a line of the matrix
{
  if (num > 2 || num < 0)
    return vector3D();
  return vector3D(data[num * 3 + 0], data[num * 3 + 1], data[num * 3 + 2]);
}

void matrix3D::operator=(const matrix3D& m)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      data[i * 3 + j] = m.getValue(i, j);
    }
  }
  unit = m.Unitary();
  reel = m.Orthogonal();
}

vector3D matrix3D::operator*(const vector3D& v) const
{
  vector3D temp;
  if (v.isLine()) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
      sum = 0;
      for (int j = 0; j < 3; j++)
        sum += data[j * 3 + i] * v.getValue(j);
      temp.setValue(i, sum);
    }
  } else {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
      sum = 0;
      for (int j = 0; j < 3; j++)
        sum += data[i * 3 + j] * v.getValue(j);
      temp.setValue(i, sum);
    }
  }
  return vector3D(temp.X(), temp.Y(), temp.Z());
}

matrix3D matrix3D::operator*(const matrix3D& m) const
{
  matrix3D temp;
  double sum;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      sum = 0;
      for (int k = 0; k < 3; k++)
        sum = sum + data[i * 3 + k] * m.getValue(k, j);
      temp.setValue(i, j, sum);
    }
  return temp;
}

matrix3D matrix3D::operator*(double v) const
{
  matrix3D temp;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.setValue(i, j, data[i * 3 + j] * v);
  return temp;
}

matrix3D matrix3D::operator+(const matrix3D& m) const
{
  matrix3D temp;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.setValue(i, j, data[i * 3 + j] + m.getValue(i, j));
  return temp;
}

matrix3D matrix3D::operator-(const matrix3D& m) const
{
  matrix3D temp;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.setValue(i, j, data[i * 3 + j] - m.getValue(i, j));
  return temp;
}

matrix3D matrix3D::operator!() const
{
  matrix3D temp;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      temp.setValue(i, j, data[j * 3 + i]);
  return temp;
} // transpose

double matrix3D::Determinant() const
{
  double value = 0;
  value += data[0 * 3 + 0] * data[1 * 3 + 1] * data[2 * 3 + 2];
  value += data[0 * 3 + 1] * data[1 * 3 + 2] * data[2 * 3 + 0];
  value += data[0 * 3 + 2] * data[1 * 3 + 0] * data[2 * 3 + 1];
  value += -data[0 * 3 + 0] * data[1 * 3 + 2] * data[2 * 3 + 1];
  value += -data[0 * 3 + 1] * data[1 * 3 + 0] * data[2 * 3 + 2];
  value += -data[0 * 3 + 2] * data[1 * 3 + 1] * data[2 * 3 + 0];
  return value;
}
matrix3D matrix3D::operator-() const
{
  double det = Determinant();
  if (det == 0)
    return matrix3D();
  matrixNxM tmp(3, 3);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      tmp.setValue(i, j, this->getValue(i, j));
  tmp = -tmp;
  matrix3D ret;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ret.setValue(i, j, tmp.getValue(i, j));
  //   ret.setValue(0,0,data[4]*data[8]-data[5]*data[7]);
  //   ret.setValue(0,1,data[2]*data[7]-data[1]*data[8]);
  //   ret.setValue(0,2,data[1]*data[5]-data[2]*data[4]);
  //   ret.setValue(1,0,data[5]*data[6]-data[3]*data[8]);
  //   ret.setValue(1,1,data[0]*data[8]-data[4]*data[6]);
  //   ret.setValue(1,2,data[2]*data[3]-data[0]*data[5]);
  //   ret.setValue(2,0,data[3]*data[7]-data[4]*data[6]);
  //   ret.setValue(2,1,data[1]*data[6]-data[0]*data[7]);
  //   ret.setValue(2,2,data[0]*data[4]-data[1]*data[3]);
  return ret; //*(1/det);
} // invert
//#ifdef USEQT
string matrix3D::toString(int pt) const
{
  string ret = "(";
  ret += string_number(getValue(0, 0)) + ", " + string_number(getValue(0, 1)) + ", " +
         string_number(getValue(0, 2)) + "\n ";
  ret += string_number(getValue(1, 0)) + ", " + string_number(getValue(1, 1)) + ", " +
         string_number(getValue(1, 2)) + "\n ";
  ret += string_number(getValue(2, 0)) + ", " + string_number(getValue(2, 1)) + ", " +
         string_number(getValue(2, 2)) + ")";
  return ret;
}
//#endif
ostream& operator<<(ostream& o, const matrix3D& m)
{
  o << "(" << m.getValue(0, 0) << ", " << m.getValue(0, 1) << ", " << m.getValue(0, 2) << "\n "
    << m.getValue(1, 0) << ", " << m.getValue(1, 1) << ", " << m.getValue(1, 2) << "\n "
    << m.getValue(2, 0) << ", " << m.getValue(2, 1) << ", " << m.getValue(2, 2) << ")";
  return o;
}
matrix3D::matrix3D() : /*v_0(),v_1(),v_2(),*/ unit(0), reel(0)
{
  //  data=new double[9];
  Undefined();
}

matrix3D::matrix3D(const matrix3D& m) : geomObject(), unit(m.Unitary())
{
  //  data=new double[9];
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      data[i * 3 + j] = m.getValue(i, j);
  //     v_0.setValues(m.getValue(0,0),m.getValue(0,1),m.getValue(0,2));
  //     v_1.setValues(m.getValue(1,0),m.getValue(1,1),m.getValue(1,2));
  //     v_2.setValues(m.getValue(2,0),m.getValue(2,1),m.getValue(2,2));
}

matrix3D::matrix3D(const vector3D& v1, const vector3D& v2)
{
  //  data=new double[9];
  data[0] = v1.X() * v2.X();
  data[1] = v1.X() * v2.Y();
  data[2] = v1.X() * v2.Z();
  data[3] = v1.Y() * v2.X();
  data[4] = v1.Y() * v2.Y();
  data[5] = v1.Y() * v2.Z();
  data[6] = v1.Z() * v2.X();
  data[7] = v1.Z() * v2.Y();
  data[8] = v1.Z() * v2.Z();
  //     v_0.setValues(v1.X()*v2.X(),v1.X()*v2.Y(),v1.X()*v2.Z());
  //     v_1.setValues(v1.Y()*v2.X(),v1.Y()*v2.Y(),v1.Y()*v2.Z());
  //     v_2.setValues(v1.Z()*v2.X(),v1.Z()*v2.Y(),v1.Z()*v2.Z());
}

double matrix3D::getValue(int line, int column) const
{
  if (line < 0 || line > 2 || column < 0 || column > 2)
    return 0;
  return data[line * 3 + column];
  //     switch(line)
  //     {
  // 	case 0:return v_0.getValue(column);
  // 	case 1:return v_1.getValue(column);
  // 	case 2:return v_2.getValue(column);
  //     }
  return 0;
}

void matrix3D::setValue(int line, int column, double value)
{
  if (line < 0 || line > 2 || column < 0 || column > 2)
    return;
  data[line * 3 + column] = value;
  //    switch(line)
  //    {
  //	case 0:v_0.setValue(column,value);break;
  //	case 1:v_1.setValue(column,value);break;
  //	case 2:v_2.setValue(column,value);break;
  //    }
  unit = 0;
  reel = 0;
}

void matrix3D::Zero()
{
  data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  data[5] = 0;
  data[6] = 0;
  data[7] = 0;
  data[8] = 0;
  //    v_0.setValues(0,0,0);
  //    v_1.setValues(0,0,0)
  //    v_2.setValues(0,0,0);
  geomObject::Zero();
}

matrix3D matrix3D::Unity()
{
  matrix3D ret;
  //    unit=1;
  ret.setValue(0, 0, 1);
  ret.setValue(1, 0, 0);
  ret.setValue(2, 0, 0);
  ret.setValue(0, 1, 0);
  ret.setValue(1, 1, 1);
  ret.setValue(2, 1, 0);
  ret.setValue(0, 2, 0);
  ret.setValue(1, 2, 0);
  ret.setValue(2, 2, 1);
  //     reel=1;
  //     geomObject::Regular();
  //   data[0]=1;
  //   data[1]=0;
  //   data[2]=0;
  //   data[3]=0;
  //   data[4]=1;
  //   data[5]=0;
  //   data[6]=0;
  //   data[7]=0;
  //   data[8]=1;
  //     v_0.setValues(1,0,0);
  //     v_0.setValues(0,1,0);
  //     v_0.setValues(0,0,1);
  return ret;
}

int matrix3D::Unitary() const { return unit; }

int matrix3D::Orthogonal() const { return (unit && reel); }

matrix3D matrix3D::Rotate(const vector3D& fromX, const vector3D& fromY, const vector3D& fromZ,
                          const vector3D& toX, const vector3D& toY, const vector3D& toZ)
{
  vector3D l1, l2, l3, c;
  vector3D res1, res2, res3;
  double tmp1, tmp2;
  // linear eq. system first line of matrix:
  l1.setValues(fromX.X(), fromX.Y(), fromX.Z());
  l2.setValues(fromY.X(), fromY.Y(), fromY.Z());
  l3.setValues(fromZ.X(), fromZ.Y(), fromZ.Z());
  c.setValues(toX.X(), toY.X(), toZ.X());
  if (l1.X() == 0) {
    vector3D tmp(l1);
    if (l2.X() == 0) {
      l1 = l3;
      l3 = tmp;
    } else {
      l1 = l2;
      l2 = tmp;
    }
  }
  l2 = l2 - l1 * (l2.X() / l1.X());
  l3 = l3 - l1 * (l3.X() / l1.X());
  c.setValues(c.X(), c.Y() - c.X() * l2.X() / l1.X(), c.Z() - c.X() * l3.X() / l1.X());
  if (l2.Y() == 0) {
    vector3D tmp(l2);
    l2 = l3;
    l3 = tmp;
  } else {
    if (l3.Y() != 0) {
      l3 = l3 - l2 * (l3.Y() / l2.Y());
      c.setValues(c.X(), c.Y(), c.Z() - c.Y() * l3.Y() / l2.Y());
    }
  }
  tmp1 = c.Z() / l3.Z();
  tmp2 = (c.Y() - l2.Z() * tmp1) / l2.Y();
  res1.setValues((c.X() - l1.Z() * tmp1 - l1.Y() * tmp2) / l1.X(), tmp2, tmp1);
  // linear eq. system second line of matrix:
  l1.setValues(fromX.X(), fromX.Y(), fromX.Z());
  l2.setValues(fromY.X(), fromY.Y(), fromY.Z());
  l3.setValues(fromZ.X(), fromZ.Y(), fromZ.Z());
  c.setValues(toX.Y(), toY.Y(), toZ.Y());
  if (l1.X() == 0) {
    vector3D tmp(l1);
    if (l2.X() == 0) {
      l1 = l3;
      l3 = tmp;
    } else {
      l1 = l2;
      l2 = tmp;
    }
  }
  l2 = l2 - l1 * (l2.X() / l1.X());
  l3 = l3 - l1 * (l3.X() / l1.X());
  c.setValues(c.X(), c.Y() - c.X() * l2.X() / l1.X(), c.Z() - c.X() * l3.X() / l1.X());
  if (l2.Y() == 0) {
    vector3D tmp(l2);
    l2 = l3;
    l3 = tmp;
  } else {
    if (l3.Y() != 0) {
      l3 = l3 - l2 * (l3.Y() / l2.Y());
      c.setValues(c.X(), c.Y(), c.Z() - c.Y() * l3.Y() / l2.Y());
    }
  }
  tmp1 = c.Z() / l3.Z();
  tmp2 = (c.Y() - l2.Z() * tmp1) / l2.Y();
  res2.setValues((c.X() - l1.Z() * tmp1 - l1.Y() * tmp2) / l1.X(), tmp2, tmp1);
  // linear eq. system third line of matrix:
  l1.setValues(fromX.X(), fromX.Y(), fromX.Z());
  l2.setValues(fromY.X(), fromY.Y(), fromY.Z());
  l3.setValues(fromZ.X(), fromZ.Y(), fromZ.Z());
  c.setValues(toX.Z(), toY.Z(), toZ.Z());
  if (l1.X() == 0) {
    vector3D tmp(l1);
    if (l2.X() == 0) {
      l1 = l3;
      l3 = tmp;
    } else {
      l1 = l2;
      l2 = tmp;
    }
  }
  l2 = l2 - l1 * (l2.X() / l1.X());
  l3 = l3 - l1 * (l3.X() / l1.X());
  c.setValues(c.X(), c.Y() - c.X() * l2.X() / l1.X(), c.Z() - c.X() * l3.X() / l1.X());
  if (l2.Y() == 0) {
    vector3D tmp(l2);
    l2 = l3;
    l3 = tmp;
  } else {
    if (l3.Y() != 0) {
      l3 = l3 - l2 * (l3.Y() / l2.Y());
      c.setValues(c.X(), c.Y(), c.Z() - c.Y() * l3.Y() / l2.Y());
    }
  }
  tmp1 = c.Z() / l3.Z();
  tmp2 = (c.Y() - l2.Z() * tmp1) / l2.Y();
  res3.setValues((c.X() - l1.Z() * tmp1 - l1.Y() * tmp2) / l1.X(), tmp2, tmp1);
  return matrix3D(res1, res2, res3);
}
