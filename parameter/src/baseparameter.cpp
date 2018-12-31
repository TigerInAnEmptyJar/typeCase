//#include "tofParam.h"
// #include <TClass.h>
// #include <TFile.h>
#include "baseparameter.h"
base_parameter::base_parameter() {}

base_parameter::~base_parameter() {}

void base_parameter::setName(const string& n) { name = n; }

void base_parameter::setDescription(vector<string> d)
{
  while (!description.empty())
    description.pop_back();
  for (unsigned int i = 0; i < d.size(); i++)
    description.push_back(d[i]);
}

void base_parameter::setDescription(const string& d, int line)
{
  if (line >= (int)description.size())
    description.push_back(d);
  else
    description[line] = d;
}

string base_parameter::getName() const { return name; }

string base_parameter::getDescription(int line) const
{
  string ret;
  ret = description[line];
  return ret;
}

vector<string> base_parameter::getDescription() const { return description; }

// void base_parameter::setCategory(int cat)
// {
//     cat++;
// }

// int base_parameter::getCategory() const
// {
//     return -1;
// }

// void base_parameter::setLevel(int lev)
// {
//     lev++;
// }

// int base_parameter::getLevel() const
// {
//     return -1;
// }

// void base_parameter::setUse(bool in)
// {
//     in=true;
// }

// bool base_parameter::IsUsed() const
// {
//     return false;
// }

bool base_parameter::operator<(base_parameter p) { return (name < p.getName()); }

bool base_parameter::operator>(base_parameter p) { return (name > p.getName()); }

bool base_parameter::operator==(base_parameter p) { return (name == p.getName()); }

bool base_parameter::operator<=(base_parameter p) { return (name <= p.getName()); }

bool base_parameter::operator>=(base_parameter p) { return (name >= p.getName()); }

// void base_parameter::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	base_parameter::Class()->WriteBuffer(b, this);
// 	b<<name.data();
// 	b<<description.size();
// 	for(unsigned int i=0;i<description.size();i++)
// 	    b<<description[i].data();
//     }
//     else
//     {
// 	base_parameter::Class()->ReadBuffer(b, this);
// 	char* line=0;
// 	b>>line;
// 	name=string(line);
// 	unsigned int num;
// 	b>>num;
// 	for(unsigned int i=0;i<num;i++)
// 	{
// 	    b<<line;
// 	    description.push_back(string(line));
// 	}
//     }
// }
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "shapeparameter.h"
template bool takeItemFromVector(vector<algorithm_parameter>& vect, algorithm_parameter& item);
template bool moveItemToPosVector(vector<algorithm_parameter>& vect, algorithm_parameter& item,
                                  int pos);
template bool takeItemFromVector(vector<base_parameter>& vect, base_parameter& item);
template bool moveItemToPosVector(vector<base_parameter>& vect, base_parameter& item, int pos);
template bool takeItemFromVector(vector<beamTime_parameter>& vect, beamTime_parameter& item);
template bool moveItemToPosVector(vector<beamTime_parameter>& vect, beamTime_parameter& item,
                                  int pos);
template bool takeItemFromVector(vector<run_parameter>& vect, run_parameter& item);
template bool moveItemToPosVector(vector<run_parameter>& vect, run_parameter& item, int pos);
template bool takeItemFromVector(vector<algorithm_parameter*>& vect, algorithm_parameter*& item);
template bool moveItemToPosVector(vector<algorithm_parameter*>& vect, algorithm_parameter*& item,
                                  int pos);
template bool takeItemFromVector(vector<base_parameter*>& vect, base_parameter*& item);
template bool moveItemToPosVector(vector<base_parameter*>& vect, base_parameter*& item, int pos);
template bool takeItemFromVector(vector<beamTime_parameter*>& vect, beamTime_parameter*& item);
template bool moveItemToPosVector(vector<beamTime_parameter*>& vect, beamTime_parameter*& item,
                                  int pos);
template bool takeItemFromVector(vector<run_parameter*>& vect, run_parameter*& item);
template bool moveItemToPosVector(vector<run_parameter*>& vect, run_parameter*& item, int pos);
template bool takeItemFromVector(vector<material_parameter>& vect, material_parameter& item);
template bool moveItemToPosVector<material_parameter>(vector<material_parameter>& vect,
                                                      material_parameter& item, int pos);
template bool takeItemFromVector(vector<material_parameter*>& vect, material_parameter*& item);
template bool moveItemToPosVector(vector<material_parameter*>& vect, material_parameter*& item,
                                  int pos);
template bool takeItemFromVector(vector<string>& vect, string& item);
// template bool takeItemFromVector(vector<QString> &vect,QString &item);
template bool moveItemToPosVector(vector<int>& vect, int& item, int pos);
template bool takeItemFromVector(vector<int>& vect, int& item);
// template bool moveItemToPosVector(vector<TObject*>& vect, TObject* &item,int
// pos);
// template bool takeItemFromVector(vector<TObject*> &vect,TObject* &item);
// template bool moveItemToPosVector(vector<TFile*>& vect,TFile* &item,int pos);
// template bool takeItemFromVector(vector<TFile*> &vect,TFile* &item);
//#include "detectorparameter.h"
template bool takeItemFromVector(vector<detector_parameter>& vect, detector_parameter& item);
template bool moveItemToPosVector(vector<detector_parameter>& vect, detector_parameter& item,
                                  int pos);
template bool takeItemFromVector(vector<detector_parameter*>& vect, detector_parameter*& item);
template bool moveItemToPosVector(vector<detector_parameter*>& vect, detector_parameter*& item,
                                  int pos);
//#include "materialparameter.h"
// #include <TCanvas.h>
// template bool moveItemToPosVector(vector<TCanvas*>& vect,TCanvas* &item,int
// pos);
// template bool takeItemFromVector(vector<TCanvas*> &vect,TCanvas* &item);
//#include <qcanvaswidget.h>
// class QCanvasWidget;
// template bool moveItemToPosVector(vector<QCanvasWidget*>& vect,QCanvasWidget*
// &item,int pos);
// template bool takeItemFromVector(vector<QCanvasWidget*> &vect,QCanvasWidget*
// &item);
// #include <TH1.h>
// template bool moveItemToPosVector(vector<TH1*>& vect,TH1* &item,int pos);
// template bool takeItemFromVector(vector<TH1*> &vect,TH1* &item);
// #include <qwidget.h>
// template bool takeItemFromVector(vector<QWidget*> &vect,QWidget* &item);

// template bool moveItemToPosVector(vector<run_parameter*>& vect,run_parameter*
// &item,int pos);

template <class X>
bool takeItemFromVector(vector<X>& vect, X& item)
{
  vector<X> tmp;
  bool ret = false;
  while ((!(vect.empty())) && (!(vect.back() == item))) {
    tmp.push_back(vect.back());
    vect.pop_back();
  }
  if (!(vect.empty())) {
    vect.pop_back();
    ret = true;
  }
  while (!(tmp.empty())) {
    vect.push_back(tmp.back());
    tmp.pop_back();
  }
  return ret;
}

template <class X>
bool moveItemToPosVector(vector<X>& vect, X& item, int pos)
{
  X tmpI = item;
  int p1 = -1;
  bool ret;
  for (unsigned int i = 0; i < vect.size(); i++) {
    if (item == vect[i])
      p1 = i;
  }
  if (p1 == -1) {
    vector<X> tmp;
    while (((int)vect.size() != pos) && (!(vect.empty()))) {
      tmp.push_back(vect.back());
      vect.pop_back();
    }
    if ((vect.empty()) && ((int)tmp.size() < pos))
      ret = false;
    else
      ret = true;
    vect.push_back(item);
    while (!(tmp.empty())) {
      vect.push_back(tmp.back());
      tmp.pop_back();
    }
    return ret;
  } else if (p1 > pos) {
    vector<X> tmp;
    while ((int)vect.size() > p1 + 1) {
      tmp.push_back(vect.back());
      vect.pop_back();
    }
    vect.pop_back();
    while ((int)vect.size() > pos) {
      tmp.push_back(vect.back());
      vect.pop_back();
    }
    tmp.push_back(tmpI);
    while (!(tmp.empty())) {
      vect.push_back(tmp.back());
      tmp.pop_back();
    }
    return true;
  } else if (p1 < pos) {
    vector<X> tmp;
    while ((int)vect.size() > pos) {
      tmp.push_back(vect.back());
      vect.pop_back();
    }
    tmp.push_back(tmpI);
    while ((int)vect.size() > p1 + 1) {
      tmp.push_back(vect.back());
      vect.pop_back();
    }
    vect.pop_back();
    while (!(tmp.empty())) {
      vect.push_back(tmp.back());
      tmp.pop_back();
    }
    return true;
  }
  return false;
}
