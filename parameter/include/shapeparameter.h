#ifndef __SHAPEPARAM
#define __SHAPEPARAM
#include "baseparameter.h"
#include "geometry.h"
#include <vector>
using namespace std;
class shape_parameter : public base_parameter
{
private:
  vector<single_parameter<point3D>> points;   //!
  vector<single_parameter<vector3D>> vectors; //!
  vector<single_parameter<int>> integers;     //!
  vector<single_parameter<float>> floats;     //!
  vector<single_parameter<string>> strings_;  //!
  bool fcompleteWrite;                        //!
public:
  shape_parameter();
  shape_parameter(const shape_parameter& sp);
  ~shape_parameter();
  template <class T>
  T getParam(int i) const;
  template <class T>
  string getParamName(int i) const;
  template <class T>
  void setParam(int i, const T& p);
  template <class T>
  void setParamName(int i, string n);
  template <class T>
  void addParam(T p, string n);
  template <class T>
  int NumberOfParams() const;
  void clearProperties();
  void operator=(const shape_parameter& sh);
  bool completeWrite() const;
  void setCompleteWrite(bool value = true);
};

istream& operator>>(istream& i, shape_parameter& sh);
ostream& operator<<(ostream& o, const shape_parameter& sh);
template <>
bool takeItemFromVector<shape_parameter>(vector<shape_parameter>& vect, shape_parameter& item);
template <>
bool moveItemToPosVector<shape_parameter>(vector<shape_parameter>& vect, shape_parameter& item,
                                          int pos);
#endif
