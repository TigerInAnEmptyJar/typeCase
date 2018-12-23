#ifndef ALGORITHM
#define ALGORITHM
#include <QtCore/QObject>
#include <vector>
#include <string>
class algorithm_parameter;
class TH1;
class TTree;
using namespace std;
class AAlgorithm: public QObject
{
 private:
  string name;//!
 public:
  AAlgorithm(string n);
  AAlgorithm(const AAlgorithm &a);
  ~AAlgorithm();
  string getName()const;
  virtual void *process(void*ptr);
  virtual vector<string>histogramNames();
  virtual TH1* histogram(string histoName);
  virtual vector<string>treeNames();
  virtual TTree* tree(string treename);
  static algorithm_parameter getDescription();
};
string toString(const QString &str);
#endif
