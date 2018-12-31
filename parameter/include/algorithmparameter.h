#ifndef __ALGPARAM
#define __ALGPARAM
#include "baseparameter.h"
#include "geometry.h"
using namespace std;
class algorithm_parameter : public base_parameter
{
private:
  bool inUse;
  int category;
  int level;
  int ID;
  vector<single_parameter<int>> integers;     //!
  vector<single_parameter<float>> floats;     //!
  vector<single_parameter<point3D>> points;   //!
  vector<single_parameter<bool>> switches;    //!
  vector<single_parameter<string>> stringes;  //!
  vector<single_parameter<vector3D>> vectors; //!
  //    vector<single_parameter<QBitArray> > bitmaps; //!
  vector<single_parameter<vector<int>>> intVectors;       //!
  vector<single_parameter<vector<float>>> floatVectors;   //!
  vector<single_parameter<vector<string>>> stringVectors; //!
  vector<single_parameter<algorithm_parameter>> algos;

public:
  algorithm_parameter();
  algorithm_parameter(const algorithm_parameter& ap);
  algorithm_parameter(string, int id);
  algorithm_parameter(string, int, int, int id = -1);
  algorithm_parameter(string, bool, int, int, int id = -1);
  virtual ~algorithm_parameter();
  virtual void setCategory(int cat);
  virtual int getCategory() const;
  virtual void setLevel(int lev);
  virtual int getLevel() const;
  virtual void setUse(bool in);
  virtual bool IsUsed() const;
  virtual int getID() const;
  virtual void setID(int id);
  template <class X>
  void changeParam(string name, X data);
  template <class X>
  void changeParam(string name, const bool data[], int num);
  template <class X>
  void changeParam(string name, X data, int pos);
  template <class X>
  int getNumberOfParam() const;
  template <class X>
  single_parameter<X> getParam(int i) const;
  template <class X>
  single_parameter<X> getParam(const string& nme) const;
  template <class X>
  void addParam(single_parameter<X> data);
  template <class X>
  void popParam();
  void removeParam(const string& paramName);
  bool operator<(algorithm_parameter p);
  bool operator>(algorithm_parameter p);
  bool operator==(const algorithm_parameter& p);
  bool operator<=(algorithm_parameter p);
  bool operator>=(algorithm_parameter p);
  void operator=(const algorithm_parameter& p);
  string toString(int pattern = 1) const;
};
istream& operator>>(istream& i, algorithm_parameter& a);
ostream& operator<<(ostream& o, const algorithm_parameter& a);
#endif
