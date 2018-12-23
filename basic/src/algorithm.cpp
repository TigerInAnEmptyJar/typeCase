#include "algorithm.h"
AAlgorithm::AAlgorithm(string n):QObject(),name(n)
{
}

AAlgorithm::AAlgorithm(const AAlgorithm &a):QObject(),name(a.getName())
{
}
AAlgorithm::~AAlgorithm()
{
}

void *AAlgorithm::process(void*ptr)
{
  return ptr;
}
string AAlgorithm::getName()const
{
  return name;
}
vector<string> AAlgorithm::histogramNames()
{
  vector<string>tmp;
  tmp.push_back(name);
  return tmp;
}
TH1* AAlgorithm::histogram(string histoName)
{
  if(histoName=="")return NULL;
  return NULL;
}
vector<string> AAlgorithm::treeNames()
{
  vector<string>tmp;
  tmp.push_back(name);
  return tmp;
}
TTree* AAlgorithm::tree(string treename)
{
  if(treename=="")return NULL;
  return NULL;
}
#include "algorithmparameter.h"
algorithm_parameter AAlgorithm::getDescription()
{
  return algorithm_parameter("none",0,0);
}
string toString(const QString &str)
{
#if QT_VERSION < 0x040000
  return string(str.ascii());
#else
  string ret="";
  for(int i=0;i<(int)str.length();i++)
    ret+=str[i].toLatin1();
  return ret;
#endif
}
