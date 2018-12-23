#ifndef TBASE
#define TBASE
#include <string>
using namespace std;

class TBase
{
 protected:
  string fname;
 public:
  TBase(const string& nme);
  ~TBase();
  string name()const;
};
#endif
