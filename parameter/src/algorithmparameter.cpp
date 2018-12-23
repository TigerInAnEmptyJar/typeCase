#include "algorithmparameter.h"
#include <typeinfo>
#if __GNUC__ < 4
// template class single_parameter<QBitArray>;
template class single_parameter<string>;
template class single_parameter<point3D>;
template class single_parameter<vector3D>;
template class single_parameter<vector<int> >;
template class single_parameter<vector<float> >;
template class single_parameter<vector<string> >;
template class single_parameter<int>;
template class single_parameter<bool>;
template class single_parameter<float>;
// template class single_parameter<TH1**>;
// template class single_parameter<TH1*>;
template class single_parameter<algorithm_parameter>;
#endif
template<class X>single_parameter<X>::single_parameter()
{
}

template<class X>single_parameter<X>::single_parameter(const string &Name, X Data)
{
    data=Data;
    name=Name;
}
template<class X>single_parameter<X>::single_parameter(const single_parameter & sp)
{
    data=sp.getData();
    name=sp.getName();
}
template<class X>bool single_parameter<X>::operator==(const single_parameter&s)const
{
  return (name==s.getName());
}
template<class X>bool single_parameter<X>::operator==(const string&s)const
{
  return (name==s);
}

template<class X>single_parameter<X>& single_parameter<X>::operator=(const single_parameter& s)
{
    data=s.getData();
    name=s.getName();
    return *this;
}
template<class X>single_parameter<X>::~single_parameter()
{
}

template<class X>string single_parameter<X>::getName() const
{
    return name;
}

template<class X>void single_parameter<X>::setName(const string& n)
{
    name=n;
}

template<class X>X single_parameter<X>::getData() const
{
    return data;
}

template<class X>void single_parameter<X>::setData(X d)
{
    data=d;
}

#if __GNUC__ > 3
//template class single_parameter<QBitArray>;
template class single_parameter<string>;
template class single_parameter<point3D>;
template class single_parameter<vector3D>;
template class single_parameter<int>;
template class single_parameter<bool>;
template class single_parameter<vector<int> >;
template class single_parameter<vector<float> >;
template class single_parameter<vector<string> >;
template class single_parameter<float>;
//template class single_parameter<TH1**>;
template class single_parameter<algorithm_parameter>;
#endif
#if __GNUC__ < 4
template void algorithm_parameter::changeParam<int>(string name, int data);
template void algorithm_parameter::changeParam<int>(string name, int data, int pos);
template void algorithm_parameter::popParam<int>();
template int algorithm_parameter::getNumberOfParam<int>()const;
template single_parameter<int> algorithm_parameter::getParam<int>(int i)const;
template single_parameter<int> algorithm_parameter::getParam<int>(const string &nme)const;
template void algorithm_parameter::addParam<int>(single_parameter<int> data);

template void algorithm_parameter::changeParam<float>(string name, float data);
template void algorithm_parameter::changeParam<float>(string name, float data, int pos);
template void algorithm_parameter::popParam<float>();
template int algorithm_parameter::getNumberOfParam<float>()const;
template single_parameter<float> algorithm_parameter::getParam<float>(int i)const;
template single_parameter<float> algorithm_parameter::getParam<float>(const string &nme)const;
template void algorithm_parameter::addParam<float>(single_parameter<float> data);

template void algorithm_parameter::changeParam<bool>(string name, bool data);
template void algorithm_parameter::changeParam<bool>(string name, bool data, int pos);
template void algorithm_parameter::popParam<bool>();
template int algorithm_parameter::getNumberOfParam<bool>()const;
template single_parameter<bool> algorithm_parameter::getParam<bool>(int i)const;
template single_parameter<bool> algorithm_parameter::getParam<bool>(const string &nme)const;
template void algorithm_parameter::addParam<bool>(single_parameter<bool> data);

template void algorithm_parameter::changeParam<point3D>(string name, point3D data);
template void algorithm_parameter::changeParam<point3D>(string name, point3D data, int pos);
template void algorithm_parameter::popParam<point3D>();
template int algorithm_parameter::getNumberOfParam<point3D>()const;
template single_parameter<point3D> algorithm_parameter::getParam<point3D>(int i)const;
template single_parameter<point3D> algorithm_parameter::getParam<point3D>(const string &nme)const;
template void algorithm_parameter::addParam<point3D>(single_parameter<point3D> data);

template void algorithm_parameter::changeParam<vector3D>(string name, vector3D data);
template void algorithm_parameter::changeParam<vector3D>(string name, vector3D data, int pos);
template void algorithm_parameter::popParam<vector3D>();
template int algorithm_parameter::getNumberOfParam<vector3D>()const;
template single_parameter<vector3D> algorithm_parameter::getParam<vector3D>(int i)const;
template single_parameter<vector3D> algorithm_parameter::getParam<vector3D>(const string &nme)const;
template void algorithm_parameter::addParam<vector3D>(single_parameter<vector3D> data);

template void algorithm_parameter::changeParam<string>(string name, string data);
template void algorithm_parameter::changeParam<string>(string name, string data, int pos);
template void algorithm_parameter::popParam<string>();
template int algorithm_parameter::getNumberOfParam<string>()const;
template single_parameter<string> algorithm_parameter::getParam<string>(int i)const;
template single_parameter<string> algorithm_parameter::getParam<string>(const string &nme)const;
template void algorithm_parameter::addParam<string>(single_parameter<string> data);

//template void algorithm_parameter::changeParam<QBitArray>(string name, QBitArray data);
//template void algorithm_parameter::changeParam<QBitArray>(string name, QBitArray data, int pos);

template void algorithm_parameter::changeParam<vector<int> >(string name, vector<int> data);
template void algorithm_parameter::changeParam<vector<int> >(string name, vector<int> data, int pos);
template void algorithm_parameter::popParam<vector<int> >();
template int algorithm_parameter::getNumberOfParam<vector<int> >()const;
template single_parameter<vector<int> > algorithm_parameter::getParam<vector<int> >(int i)const;
template single_parameter<vector<int> > algorithm_parameter::getParam<vector<int> >(const string &nme)const;
template void algorithm_parameter::addParam<vector<int> >(single_parameter<vector<int> > data);

template void algorithm_parameter::changeParam<vector<float> >(string name, vector<float> data);
template void algorithm_parameter::changeParam<vector<float> >(string name, vector<float> data, int pos);
template void algorithm_parameter::popParam<vector<float> >();
template int algorithm_parameter::getNumberOfParam<vector<float> >()const;
template single_parameter<vector<float> > algorithm_parameter::getParam<vector<float> >(int i)const;
template single_parameter<vector<float> > algorithm_parameter::getParam<vector<float> >(const string &nme)const;
template void algorithm_parameter::addParam<vector<float> >(single_parameter<vector<float> > data);

template void algorithm_parameter::changeParam<vector<string> >(string name, vector<string> data);
template void algorithm_parameter::changeParam<vector<string> >(string name, vector<string> data, int pos);
template void algorithm_parameter::popParam<vector<string> >();
template int algorithm_parameter::getNumberOfParam<vector<string> >()const;
template single_parameter<vector<string> > algorithm_parameter::getParam<vector<string> >(int i)const;
template single_parameter<vector<string> > algorithm_parameter::getParam<vector<string> >(const string &nme)const;
template void algorithm_parameter::addParam<vector<string> >(single_parameter<vector<string> > data);

template void algorithm_parameter::changeParam<algorithm_parameter>(string name, algorithm_parameter data);
template void algorithm_parameter::changeParam<algorithm_parameter>(string name, algorithm_parameter data,int pos);
template void algorithm_parameter::popParam<algorithm_parameter>();
template int algorithm_parameter::getNumberOfParam<algorithm_parameter>()const;
template single_parameter<algorithm_parameter> algorithm_parameter::getParam<algorithm_parameter>(int i)const;
template single_parameter<algorithm_parameter> algorithm_parameter::getParam<algorithm_parameter>(const string &nme)const;
template void algorithm_parameter::addParam<algorithm_parameter>(single_parameter<algorithm_parameter> data);
#endif

algorithm_parameter::algorithm_parameter()
{
    inUse=false;
    ID=-1;
}

algorithm_parameter::algorithm_parameter(string n, int cat=0, int lev=0, int id)
{
    ID=id;
    setName(n);
    inUse=false;
    category=cat;
    level=lev;
}
algorithm_parameter::algorithm_parameter(string n, int id)
{
    ID=id;
    setName(n);
    inUse=false;
}
    
int algorithm_parameter::getID() const
{
    return ID;
}

void algorithm_parameter::setID(int id)
{
    ID=id;
}

algorithm_parameter::algorithm_parameter(string n, bool u, int cat, int lev, int id)
{
    ID=id;
    setName(n);
    inUse=u;
    category=cat;
    level=lev;
}
algorithm_parameter::algorithm_parameter(const algorithm_parameter &p):base_parameter()
{
    ID=p.getID();
    setName(p.getName());
    inUse=p.IsUsed();
    category=p.getCategory();
    level=p.getLevel();
    setDescription(p.getDescription());
    for(int i=0;i<p.getNumberOfParam<int>();i++)
	addParam<int>(p.getParam<int>(i));
    for(int i=0;i<p.getNumberOfParam<float>();i++)
	addParam<float>(p.getParam<float>(i));
    for(int i=0;i<p.getNumberOfParam<bool>();i++)
	addParam<bool>(p.getParam<bool>(i));
    for(int i=0;i<p.getNumberOfParam<point3D>();i++)
	addParam<point3D>(p.getParam<point3D>(i));
    for(int i=0;i<p.getNumberOfParam<vector3D>();i++)
	addParam<vector3D>(p.getParam<vector3D>(i));
    //    for(int i=0;i<p.getNumberOfParam<QBitArray>();i++)
    //	addParam<QBitArray>(p.getParam<QBitArray>(i));
    for(int i=0;i<p.getNumberOfParam<string>();i++)
	addParam<string>(p.getParam<string>(i));
    for(int i=0;i<p.getNumberOfParam<vector<int> >();i++)
	addParam<vector<int> >(p.getParam<vector<int> >(i));
    for(int i=0;i<p.getNumberOfParam<vector<float> >();i++)
	addParam<vector<float> >(p.getParam<vector<float> >(i));
    for(int i=0;i<p.getNumberOfParam<vector<string> >();i++)
	addParam<vector<string> >(p.getParam<vector<string> >(i));
    for(int i=0;i<p.getNumberOfParam<algorithm_parameter>();i++)
	addParam<algorithm_parameter>(p.getParam<algorithm_parameter>(i));
}

algorithm_parameter::~algorithm_parameter()
{
}

void algorithm_parameter::setUse(bool in)
{
    inUse=in;
}

bool algorithm_parameter::IsUsed() const
{
    return inUse;
}

void algorithm_parameter::setCategory(int cat)
{
    category=cat;
}

int algorithm_parameter::getCategory() const  
{
    return category;
}

void algorithm_parameter::setLevel(int lev)
{
    level=lev;
}

int algorithm_parameter::getLevel() const
{
    return level;
}

template<class X> void algorithm_parameter::changeParam(string name, X data)
{
  void *vec=NULL;
  if(typeid(X)==typeid(int))
    vec=&integers;
  else if(typeid(X)==typeid(float))
    vec=&floats;
  else if(typeid(X)==typeid(point3D))
    vec=&points;
  else if(typeid(X)==typeid(bool))
    vec=&switches;
  else if(typeid(X)==typeid(string))
    vec=&stringes;
  else if(typeid(X)==typeid(vector3D))
    vec=&vectors;
  //  else if(typeid(X)==typeid(QBitArray))
  //    vec=&bitmaps;
  else if(typeid(X)==typeid(vector<int>))
    vec=&intVectors;
  else if(typeid(X)==typeid(vector<float>))
    vec=&floatVectors;
  else if(typeid(X)==typeid(vector<string>))
    vec=&stringVectors;
  else if(typeid(X)==typeid(algorithm_parameter))
    vec=&algos;
  if(vec==0)return;
  for(unsigned int i=0;i<((vector<single_parameter<X> >*)vec)->size();i++)
    if(((vector<single_parameter<X> >*)vec)->at(i).getName()==name)
      ((vector<single_parameter<X> >*)vec)->at(i).setData(data);
}

template<class X> void algorithm_parameter::changeParam(string name, const bool data[], int num)
{
  //  if(typeid(X)==typeid(QBitArray))
//     {
//       for(unsigned int i=0;i<bitmaps.size();i++)
// 	{
// 	  if(name==bitmaps[i].getName())
// 	    {	
// 	      QBitArray tmp;
// 	      for (int j=0;j<num;j++)
// 		tmp.fill(data[j]);
// 	      bitmaps[i].setData(tmp);
// 	    }
// 	}
//     }
//   else
    return;
}

template<class X> void algorithm_parameter::changeParam(string name, X data, int pos)
{
  if(pos<0)return;
  void *vec=NULL;
  if(typeid(X)==typeid(int))
    vec=&integers;
  else if(typeid(X)==typeid(float))
    vec=&floats;
  else if(typeid(X)==typeid(point3D))
    vec=&points;
  else if(typeid(X)==typeid(bool))
    vec=&switches;
  else if(typeid(X)==typeid(string))
    vec=&stringes;
  else if(typeid(X)==typeid(vector3D))
    vec=&vectors;
//   else if(typeid(X)==typeid(QBitArray))
//     vec=&bitmaps;
  else if(typeid(X)==typeid(vector<int>))
    vec=&intVectors;
  else if(typeid(X)==typeid(vector<float>))
    vec=&floatVectors;
  else if(typeid(X)==typeid(vector<string>))
    vec=&stringVectors;
  else if(typeid(X)==typeid(algorithm_parameter))
  if(vec==NULL)return;
  if(pos>=(int)((vector<single_parameter<X> >*)vec)->size())return;
  ((vector<single_parameter<X> >*)vec)->at(pos).setData(data);
  ((vector<single_parameter<X> >*)vec)->at(pos).setName(name);
}

template<class X> int algorithm_parameter::getNumberOfParam()const
{
  if(typeid(X)==typeid(int))
    return integers.size();
  else if(typeid(X)==typeid(float))
    return floats.size();
  else if(typeid(X)==typeid(point3D))
    return points.size();
  else if(typeid(X)==typeid(bool))
    return switches.size();
  else if(typeid(X)==typeid(string))
    return stringes.size();
  else if(typeid(X)==typeid(vector3D))
    return vectors.size();
//   else if(typeid(X)==typeid(QBitArray))
//     return bitmaps.size();
  else if(typeid(X)==typeid(vector<int>))
    return intVectors.size();
  else if(typeid(X)==typeid(vector<float>))
    return floatVectors.size();
  else if(typeid(X)==typeid(vector<string>))
    return stringVectors.size();
  else if(typeid(X)==typeid(algorithm_parameter))
    return algos.size();
  else 
    return -1;
}
template<class X> single_parameter<X> algorithm_parameter::getParam(const string &nme) const
{
  const void *p=0;
  if(typeid(X)==typeid(int)) {
    for(int i=integers.size()-1;i>=0;i--)
      if(integers[i].getName()==nme)
	p=&integers.at(i);
  }else if(typeid(X)==typeid(float)){
    for(int i=floats.size()-1;i>=0;i--)
      if(floats[i].getName()==nme)
	p=&floats.at(i);
  }else if(typeid(X)==typeid(point3D)){
    for(int i=points.size()-1;i>=0;i--)
      if(points[i].getName()==nme)
	p=&points.at(i);
  }else if(typeid(X)==typeid(bool)){
    for(int i=switches.size()-1;i>=0;i--)
      if(switches[i].getName()==nme)
	p=&switches.at(i);
  }else if(typeid(X)==typeid(string)){
    for(int i=stringes.size()-1;i>=0;i--)
      if(stringes[i].getName()==nme)
	p=&stringes.at(i);
  }else if(typeid(X)==typeid(vector3D)){
    for(int i=vectors.size()-1;i>=0;i--)
      if(vectors[i].getName()==nme)
	p=&vectors.at(i);
//   else if(typeid(X)==typeid(QBitArray))
//     for(int i=bitmaps.size()-1;i>=0;i--)
//       if(bitmaps[i].getName()==nme)
// 	p=&bitmaps.at(i);
  }else if(typeid(X)==typeid(vector<int>)){
    for(int i=intVectors.size()-1;i>=0;i--)
      if(intVectors[i].getName()==nme)
	p=&intVectors.at(i);
  }else if(typeid(X)==typeid(vector<float>)){
     for(int i=floatVectors.size()-1;i>=0;i--){
      if(floatVectors[i].getName()==nme)
	p=&floatVectors.at(i);
     }
  }else if(typeid(X)==typeid(vector<string>)){
    for(int i=stringVectors.size()-1;i>=0;i--) {
      if(stringVectors[i].getName()==nme)
	p=&stringVectors.at(i);
    }
  }else if(typeid(X)==typeid(algorithm_parameter)){
    for(int i=algos.size()-1;i>=0;i--)
      if(algos[i].getName()==nme)
	p=&algos.at(i);
  }
  if(p!=0) return *((single_parameter<X>*)p);
  return single_parameter<X>();
}
template<class X> single_parameter<X> algorithm_parameter::getParam(int i) const
{
  if(i<0||i>=getNumberOfParam<X>()) return single_parameter<X>();
  const void *p=0;
  if(typeid(X)==typeid(int))
    p=&integers.at(i);
  else if(typeid(X)==typeid(float))
    p=&floats.at(i);
  else if(typeid(X)==typeid(point3D))
    p=&points.at(i);
  else if(typeid(X)==typeid(bool))
    p=&switches.at(i);
  else if(typeid(X)==typeid(string))
    p=&stringes.at(i);
  else if(typeid(X)==typeid(vector3D))
    p=&vectors.at(i);
//   else if(typeid(X)==typeid(QBitArray))
//     p=&bitmaps.at(i);
  else if(typeid(X)==typeid(vector<int>))
    p=&intVectors.at(i);
  else if(typeid(X)==typeid(vector<float>))
    p=&floatVectors.at(i);
  else if(typeid(X)==typeid(vector<string>))
    p=&stringVectors.at(i);
  else if(typeid(X)==typeid(algorithm_parameter))
    p=&algos.at(i);
  if(p!=0) return *((single_parameter<X>*)p);
  return single_parameter<X>();
}

template<class X> void algorithm_parameter::addParam(single_parameter<X> data)
{
  void* p=0;
  if(typeid(X)==typeid(int))
    p=&integers;
  else if(typeid(X)==typeid(float))
    p=&floats;
  else if(typeid(X)==typeid(point3D))
    p=&points;
  else if(typeid(X)==typeid(bool))
    p=&switches;
  else if(typeid(X)==typeid(string))
    p=&stringes;
  else if(typeid(X)==typeid(vector3D))
    p=&vectors;
//   else if(typeid(X)==typeid(QBitArray))
//     p=&bitmaps;
  else if(typeid(X)==typeid(vector<int>))
    p=&intVectors;
  else if(typeid(X)==typeid(vector<float>))
    p=&floatVectors;
  else if(typeid(X)==typeid(vector<string>))
    p=&stringVectors;
  else if(typeid(X)==typeid(algorithm_parameter))
    p=&algos;
  if(p!=0)((vector<single_parameter<X> >*)p)->push_back(data);
}

template<class X> void algorithm_parameter::popParam()
{
  if(typeid(X)==typeid(int))
    integers.pop_back();
  else if(typeid(X)==typeid(float))
    floats.pop_back();
  else if(typeid(X)==typeid(point3D))
    points.pop_back();
  else if(typeid(X)==typeid(bool))
    switches.pop_back();
  else if(typeid(X)==typeid(string))
    stringes.pop_back();
  else if(typeid(X)==typeid(vector3D))
    vectors.pop_back();
//   else if(typeid(X)==typeid(QBitArray))
//     bitmaps.pop_back();
  else if(typeid(X)==typeid(vector<int>))
    intVectors.pop_back();
  else if(typeid(X)==typeid(vector<float>))
    floatVectors.pop_back();
  else if(typeid(X)==typeid(vector<string>))
    stringVectors.pop_back();
  else if(typeid(X)==typeid(algorithm_parameter))
    algos.pop_back();
}

void algorithm_parameter::removeParam(const string &paramName)
{
  for(int i=0;i<getNumberOfParam<int>();i++)
    if(integers[i]==paramName)
      {
	vector<single_parameter<int> > tmp;
	while((int)integers.size()>i-1)
	  {
	    tmp.push_back(integers.back());
	    integers.pop_back();
	  }
	integers.pop_back();
	while(!tmp.empty())
	  {
	    integers.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<float>();i++)
    if(floats[i]==paramName)
      {
	vector<single_parameter<float> > tmp;
	while((int)floats.size()>i-1)
	  {
	    tmp.push_back(floats.back());
	    floats.pop_back();
	  }
	floats.pop_back();
	while(!tmp.empty())
	  {
	    floats.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<bool>();i++)
    if(switches[i]==paramName)
      {
	vector<single_parameter<bool> > tmp;
	while((int)switches.size()>i-1)
	  {
	    tmp.push_back(switches.back());
	    switches.pop_back();
	  }
	switches.pop_back();
	while(!tmp.empty())
	  {
	    switches.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<point3D>();i++)
    if(points[i]==paramName)
      {
	vector<single_parameter<point3D> > tmp;
	while((int)points.size()>i-1)
	  {
	    tmp.push_back(points.back());
	    points.pop_back();
	  }
	points.pop_back();
	while(!tmp.empty())
	  {
	    points.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<vector3D>();i++)
    if(vectors[i]==paramName)
      {
	vector<single_parameter<vector3D> > tmp;
	while((int)vectors.size()>i-1)
	  {
	    tmp.push_back(vectors.back());
	    vectors.pop_back();
	  }
	vectors.pop_back();
	while(!tmp.empty())
	  {
	    vectors.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
//   for(int i=0;i<getNumberOfParam<QBitArray>();i++)
//     if(bitmaps[i]==paramName)
//       {
// 	vector<single_parameter<QBitArray> > tmp;
// 	while((int)bitmaps.size()>i-1)
// 	  {
// 	    tmp.push_back(bitmaps.back());
// 	    bitmaps.pop_back();
// 	  }
// 	bitmaps.pop_back();
// 	while(!tmp.empty())
// 	  {
// 	    bitmaps.push_back(tmp.back());
// 	    tmp.pop_back();
// 	  }
// 	return;
//       }
  for(int i=0;i<getNumberOfParam<string>();i++)
    if(stringes[i]==paramName)
      {
	vector<single_parameter<string> > tmp;
	while((int)stringes.size()>i-1)
	  {
	    tmp.push_back(stringes.back());
	    stringes.pop_back();
	  }
	stringes.pop_back();
	while(!tmp.empty())
	  {
	    stringes.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<vector<int> >();i++)
    if(intVectors[i]==paramName)
      {
	vector<single_parameter<vector<int> > > tmp;
	while((int)intVectors.size()>i-1)
	  {
	    tmp.push_back(intVectors.back());
	    intVectors.pop_back();
	  }
	intVectors.pop_back();
	while(!tmp.empty())
	  {
	    intVectors.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<vector<float> >();i++)
    if(floatVectors[i]==paramName)
      {
	vector<single_parameter<vector<float> > > tmp;
	while((int)floatVectors.size()>i-1)
	  {
	    tmp.push_back(floatVectors.back());
	    floatVectors.pop_back();
	  }
	floatVectors.pop_back();
	while(!tmp.empty())
	  {
	    floatVectors.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<vector<string> >();i++)
    if(stringVectors[i]==paramName)
      {
	vector<single_parameter<vector<string> > > tmp;
	while((int)stringVectors.size()>i-1)
	  {
	    tmp.push_back(stringVectors.back());
	    stringVectors.pop_back();
	  }
	stringVectors.pop_back();
	while(!tmp.empty())
	  {
	    stringVectors.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
  for(int i=0;i<getNumberOfParam<algorithm_parameter>();i++)
    if(algos[i]==paramName)
      {
	vector<single_parameter<algorithm_parameter> > tmp;
	while((int)algos.size()>i-1)
	  {
	    tmp.push_back(algos.back());
	    algos.pop_back();
	  }
	algos.pop_back();
	while(!tmp.empty())
	  {
	    algos.push_back(tmp.back());
	    tmp.pop_back();
	  }
	return;
      }
}

bool algorithm_parameter::operator<(algorithm_parameter p)
{
    return ID<p.getID();
}
bool algorithm_parameter::operator<=(algorithm_parameter p)
{
    return ID<=p.getID();
}
bool algorithm_parameter::operator>(algorithm_parameter p)
{
    return ID>p.getID();
}
bool algorithm_parameter::operator>=(algorithm_parameter p)
{
    return ID>=p.getID();
}

void algorithm_parameter::operator=(const algorithm_parameter & p)
{
    ID=p.getID();
    setName(p.getName());
    inUse=p.IsUsed();
    category=p.getCategory();
    level=p.getLevel();
    setDescription(p.getDescription());
    while (!(integers.empty()))
	integers.pop_back();
    while (!(floats.empty()))
	floats.pop_back();
    while (!(switches.empty()))
	switches.pop_back();
    while (!(points.empty()))
	points.pop_back();
    while (!(vectors.empty()))
	vectors.pop_back();
//     while (!(bitmaps.empty()))
// 	bitmaps.pop_back();
    while (!(stringes.empty()))
	stringes.pop_back();
    while (!(intVectors.empty()))
	intVectors.pop_back();
    while (!(floatVectors.empty()))
	floatVectors.pop_back();
    while (!(stringVectors.empty()))
	stringVectors.pop_back();
    while (!(algos.empty()))
      algos.pop_back();
    for(int i=0;i<p.getNumberOfParam<int>();i++)
	addParam<int>(p.getParam<int>(i));
    for(int i=0;i<p.getNumberOfParam<float>();i++)
	addParam<float>(p.getParam<float>(i));
    for(int i=0;i<p.getNumberOfParam<bool>();i++)
	addParam<bool>(p.getParam<bool>(i));
    for(int i=0;i<p.getNumberOfParam<point3D>();i++)
	addParam<point3D>(p.getParam<point3D>(i));
    for(int i=0;i<p.getNumberOfParam<vector3D>();i++)
	addParam<vector3D>(p.getParam<vector3D>(i));
//     for(int i=0;i<p.getNumberOfParam<QBitArray>();i++)
// 	addParam<QBitArray>(p.getParam<QBitArray>(i));
    for(int i=0;i<p.getNumberOfParam<string>();i++)
	addParam<string>(p.getParam<string>(i));
    for(int i=0;i<p.getNumberOfParam<vector<int> >();i++)
	addParam<vector<int> >(p.getParam<vector<int> >(i));
    for(int i=0;i<p.getNumberOfParam<vector<float> >();i++)
	addParam<vector<float> >(p.getParam<vector<float> >(i));
    for(int i=0;i<p.getNumberOfParam<vector<string> >();i++)
	addParam<vector<string> >(p.getParam<vector<string> >(i));
    for(int i=0;i<p.getNumberOfParam<algorithm_parameter>();i++)
	addParam<algorithm_parameter>(p.getParam<algorithm_parameter>(i));
}

bool algorithm_parameter::operator==(const algorithm_parameter &ap)
{
    if(ID!=ap.getID()) return false;
    if(getName()!=ap.getName()) return false;
    if(getNumberOfParam<int>()!=ap.getNumberOfParam<int>()) return false;
    if(getNumberOfParam<float>()!=ap.getNumberOfParam<float>()) return false;
    if(getNumberOfParam<bool>()!=ap.getNumberOfParam<bool>()) return false;
    if(getNumberOfParam<point3D>()!=ap.getNumberOfParam<point3D>()) return false;
    if(getNumberOfParam<vector3D>()!=ap.getNumberOfParam<vector3D>()) return false;
//     if(getNumberOfParam<QBitArray>()!=ap.getNumberOfParam<QBitArray>()) return false;
    if(getNumberOfParam<vector<int> >()!=ap.getNumberOfParam<vector<int> >()) return false;
    if(getNumberOfParam<vector<float> >()!=ap.getNumberOfParam<vector<float> >()) return false;
    if(getNumberOfParam<vector<string> >()!=ap.getNumberOfParam<vector<string> >()) return false;
    if(getNumberOfParam<algorithm_parameter>()!=ap.getNumberOfParam<algorithm_parameter>())return false;
    return true;    
}
	
istream & operator>>(istream & i, algorithm_parameter & a)
{
    int zahl, z2;
    float fz1,fz2,fz3;
    char c , _tmp[100], _tmp2[100];
    i.get(c);
    while(c!='#'&&i.good())i.get(c);
    i >> zahl;
    a.setID(zahl);
    i >> zahl;
    a.setUse(zahl==1);
    i.get(c);
    i.getline(_tmp,100);
    a.setName(_tmp);
    i.get(c);
    i >> zahl;
    a.setCategory(zahl);
    i >> zahl;
    a.setLevel(zahl);
    i.getline(_tmp,100);
    i.get(c);
    vector<string> d;	
//     while(c=='D')
//       {
// 	i.getline(_tmp,100);
// 	i.get(c);
// 	d.push_back(_tmp);
//       }
    bool finish=(c=='='||i.eof());
    while(!finish)
      {
	switch(c)
	  {
	  case 'D'://description
	    {
	      i.getline(_tmp,100);
	      i.get(c);
	      d.push_back(_tmp);
	      break;
	    }
	  case 'B'://bool
	    {
	      i>>zahl;
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<bool>(single_parameter<bool>(_tmp,(zahl==1)));
	      break;
	    }
	  case 'P'://point3D
	    {
	      i>>fz1>>fz2>>fz3;
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<point3D>(single_parameter<point3D>(_tmp,point3D(fz1,fz2,fz3)));
	      break;
	    }
	  case 'V'://vector3D
	    {
	      i>>fz1>>fz2>>fz3;
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<vector3D>(single_parameter<vector3D>(_tmp,vector3D(fz1,fz2,fz3)));
	      break;
	    }
	  case 'I'://integer
	    {
	      i>>zahl;
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<int>(single_parameter<int>(_tmp,zahl));
	      break;
	    }
	  case 'F'://float
	    {
	      i>>fz1;
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<float>(single_parameter<float>(_tmp,fz1));
	      break;
	    }
	  case 'S'://string
	    {
	      i.getline(_tmp,100);
	      i.getline(_tmp2,100);
	      i.get(c);
	      a.addParam<string>(single_parameter<string>(_tmp2,_tmp));
	      break;
	    }
	  case 'N'://vector<int>
	    {
	      vector<int> tmp;
	      i>>zahl;
	      for(int j=0;j<zahl;j++)
		{
		  i>>z2;
		  tmp.push_back(z2);
		}
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<vector<int> >(single_parameter<vector<int> >(_tmp,tmp));
	      break;
	    }
	  case 'T'://vector<float>
	    {
	      vector<float> tmp;
	      i>>zahl;
	      for(int j=0;j<zahl;j++)
		{
		  i>>fz2;
		  tmp.push_back(fz2);
		}
	      i.get(c);
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<vector<float> >(single_parameter<vector<float> >(_tmp,tmp));
	      break;
	    }
	  case 'R'://vector<string>
	    {
	      vector<string> tmp;
	      i>>zahl;
	      i.get(c);
	      for(int j=0;j<zahl;j++)
		{
		  i.getline(_tmp2,100);
		  tmp.push_back(_tmp2);
		}
	      i.getline(_tmp,100);
	      i.get(c);
	      a.addParam<vector<string> >(single_parameter<vector<string> >(_tmp,tmp));
	      break;
	    }
	  case 'A'://algorithm
	    {
	      algorithm_parameter ap;
	      i>>ap;
	      i.get(c);
	      a.addParam<algorithm_parameter>(single_parameter<algorithm_parameter>(ap.getName(),ap));
	      break;
	    }
	  case '=':
	  default:
	    finish=true;
	  }
      }
    a.setDescription(d);
    return i;
}

string algorithm_parameter::toString(int pattern)const
{
  string ret;
  if(pattern==1||pattern==2)
    {
      ret+=string(pattern==2?"Category: ":"")+string_number(category);
      ret+=string(pattern==2?"\nLevel: ":" ")+string_number(level);
      ret+=string(pattern==2?"\nID: ":" ")+string_number(ID);
      ret+=string(pattern==2?"\nis used: ":" ")+(inUse?"1":"0")+"\n";
    }
  else
    {
      ret+=string(pattern==3?"\nID: ":" ")+string_number(ID)+"\n";
    }
  ret+="Parameters:\n";
  for(unsigned int i=0;i<switches.size();i++)
    ret+=switches[i].getName()+" :"+(switches[i].getData()?"yes":"no")+"\n";
  for(unsigned int i=0;i<integers.size();i++)
    ret+=integers[i].getName()+" :"+string_number(integers[i].getData())+"\n";
  for(unsigned int i=0;i<floats.size();i++)
    ret+=floats[i].getName()+" :"+string_number(floats[i].getData())+"\n";
  for(unsigned int i=0;i<points.size();i++)
    ret+=points[i].getName()+" :"+points[i].getData().toString()+"\n";
  for(unsigned int i=0;i<vectors.size();i++)
    ret+=vectors[i].getName()+" :"+vectors[i].getData().toString()+"\n";
  for(unsigned int i=0;i<stringes.size();i++)
    ret+=stringes[i].getName()+" :"+stringes[i].getData()+"\n";
  for(unsigned int i=0;i<intVectors.size();i++)
    {
      ret+=intVectors[i].getName()+" : (";
      for(unsigned int j=0;j<intVectors[i].getData().size();j++)
	ret+=string_number(intVectors[i].getData().at(j))+", ";
      ret=ret.substr(0,ret.length()-2);
      ret+=")\n";
    }
  for(unsigned int i=0;i<floatVectors.size();i++)
    {
      ret+=floatVectors[i].getName()+" : (";
      for(unsigned int j=0;j<floatVectors[i].getData().size();j++)
	ret+=string_number(floatVectors[i].getData().at(j))+", ";
      ret=ret.substr(0,ret.length()-2);
      ret+=")\n";
    }
  for(unsigned int i=0;i<stringVectors.size();i++)
    {
      ret+=stringVectors[i].getName()+" : (";
      for(unsigned int j=0;j<stringVectors[i].getData().size();j++)
	ret+=stringVectors[i].getData().at(j)+", ";
      ret=ret.substr(0,ret.length()-2);
      ret+=")\n";
    }
  for(unsigned int i=0;i<algos.size();i++)
    {
      ret+=algos[i].getName()+": "+algos[i].getData().getName()+"\n";
    }
  return ret;
}
ostream & operator<<(ostream & o, const algorithm_parameter & a)
{
  if(o==cout)
    {
      string s;
      o<<a.getName().data()<<":"<<endl;
      o<<" Category/Level:"<<a.getCategory()<<"/"<<a.getLevel()<<endl;
      o<<" Use:"<<(a.IsUsed()?"true":"false")<<endl;
      o<<" ID:"<<a.getID()<<endl;
      o<<" bool Parameter:"<<a.getNumberOfParam<bool>()<<endl;
      for(int i =0;i<a.getNumberOfParam<bool>();i++)
	o<<"  "<<(a.getParam<bool>(i).getName().data())<<": "<<(a.getParam<bool>(i).getData()?"true":"false")<<endl;    
      o<<" point3D Parameter:"<<a.getNumberOfParam<point3D>()<<endl;
      for(int i =0;i<a.getNumberOfParam<point3D>();i++)
	o<<"  "<<(a.getParam<point3D>(i).getName().data())<<": "<<a.getParam<point3D>(i).getData()<<endl;    
      o<<" vector3D Parameter:"<<a.getNumberOfParam<vector3D>()<<endl;
      for(int i =0;i<a.getNumberOfParam<vector3D>();i++)
	o<<"  "<<(a.getParam<vector3D>(i).getName().data())<<": "<<a.getParam<vector3D>(i).getData()<<endl;    
      o<<" int Parameter:"<<a.getNumberOfParam<int>()<<endl;
      for(int i =0;i<a.getNumberOfParam<int>();i++)
	o<<"  "<<(a.getParam<int>(i).getName().data())<<": "<<a.getParam<int>(i).getData()<<endl;    
      o<<" float Parameter:"<<a.getNumberOfParam<float>()<<endl;
      for(int i =0;i<a.getNumberOfParam<float>();i++)
	o<<"  "<<(a.getParam<float>(i).getName().data())<<": "<<a.getParam<float>(i).getData()<<endl;    
      o<<" string Parameter:"<<a.getNumberOfParam<string>()<<endl;
      for(int i =0;i<a.getNumberOfParam<string>();i++)
	o<<"  "<<(a.getParam<string>(i).getName().data())<<": \""<<a.getParam<string>(i).getData().data()<<"\""<<endl;    

      o<<" vector<int> Parameter:"<<a.getNumberOfParam<vector<int> >()<<endl;
      for(int i =0;i<a.getNumberOfParam<vector<int> >();i++)
	{
	  s="(";
	  for(unsigned int j=0;j<a.getParam<vector<int> >(i).getData().size();j++)
	    s+=string_number(a.getParam<vector<int> >(i).getData().at(j))+", ";
	  if(s.length()>2)s=s.substr(0,s.length()-2);
	  s+=")";
	  o<<"  "<<(a.getParam<vector<int> >(i).getName().data())<<": "<<s.data()<<endl;
	}
      o<<" vector<float> Parameter:"<<a.getNumberOfParam<vector<float> >()<<endl;
      for(int i =0;i<a.getNumberOfParam<vector<float> >();i++)
	{
	  s="(";
	  for(unsigned int j=0;j<a.getParam<vector<float> >(i).getData().size();j++)
	    s+=string_number(a.getParam<vector<float> >(i).getData().at(j))+", ";
	  if(s.length()>2)s=s.substr(0,s.length()-2);
	  s+=")";
	  o<<"  "<<(a.getParam<vector<float> >(i).getName().data())<<": "<<s.data()<<endl;
	}
      o<<" vector<string> Parameter:"<<a.getNumberOfParam<vector<string> >()<<endl;
      for(int i =0;i<a.getNumberOfParam<vector<string> >();i++)
	{
	  s="(";
	  for(unsigned int j=0;j<a.getParam<vector<string> >(i).getData().size();j++)
	    s+=string("\"")+a.getParam<vector<string> >(i).getData().at(j)+"\", ";
	  if(s.length()>2)s=s.substr(0,s.length()-2);
	  s+=")";
	  o<<"  "<<(a.getParam<vector<string> >(i).getName().data())<<": "<<s.data()<<endl;
	}
      o<<" algorithm Parameter:"<<a.getNumberOfParam<algorithm_parameter>()<<endl;
      for(int i =0;i<a.getNumberOfParam<algorithm_parameter>();i++)
	o<<"  "<<a.getParam<algorithm_parameter>(i).getData()<<endl;
      o << " description:"<<endl;
      for(unsigned int j=0;j<a.getDescription().size();j++)
	{
	  o<<"  "<<a.getDescription(j).data()<<endl;
	}
    }
  else
    {
      o<<"===== "<<a.getName().data()<<" ====="<<endl;
      o<<"#"<<a.getID()<<" ";
      o<<(a.IsUsed()?1:0)<<" ";
      o<<a.getName().data()<<endl;
      o <<"#"<<a.getCategory()<<" ";
      o <<a.getLevel()<<" "<<endl;
      for(unsigned int j=0;j<a.getDescription().size();j++)
	{
	  o<<"D"<<a.getDescription(j).data()<<endl;
	}
      //      o<<a.getNumberOfParam<bool>() <<" ";
      for(int i =0;i<a.getNumberOfParam<bool>();i++)
	o<<"B"<<(a.getParam<bool>(i).getData()?"1":"0")<<" "<<(a.getParam<bool>(i).getName().data())<<endl;    
      //o<<a.getNumberOfParam<point3D>()<<" ";
      for(int i =0;i<a.getNumberOfParam<point3D>();i++)
	{
	  point3D tmpP=a.getParam<point3D>(i).getData();
	  o<<"P"<<tmpP.X()<<" "<<tmpP.Y()<<" "<<tmpP.Z()<<" "<<a.getParam<point3D>(i).getName().data()<<endl;
	}
      //o<<a.getNumberOfParam<vector3D>()<<" ";
      for(int i =0;i<a.getNumberOfParam<vector3D>();i++)
	{	
	  vector3D tmpP=a.getParam<vector3D>(i).getData();
	  o<<"V"<<tmpP.X()<<" "<<tmpP.Y()<<" "<<tmpP.Z()<<" "<<a.getParam<vector3D>(i).getName().data()<<endl;
	}
      //o<<a.getNumberOfParam<int>()<<" ";
      for(int i =0;i<a.getNumberOfParam<int>();i++)
	o<<"I"<<a.getParam<int>(i).getData()<<" "<<a.getParam<int>(i).getName().data()<<endl;
      //o<<a.getNumberOfParam<float>()<<" ";
      for(int i =0;i<a.getNumberOfParam<float>();i++)
	o<<"F"<<a.getParam<float>(i).getData()<<" "<<a.getParam<float>(i).getName().data()<<endl;
      //o<<a.getNumberOfParam<string>()<<" ";
      for(int i=0;i<a.getNumberOfParam<string>();i++)
	o<<"S"<<a.getParam<string>(i).getData().data()<<"\n"<<a.getParam<string>(i).getName().data()<<endl;
      //o<< a.getNumberOfParam<vector<int> >()<<" ";
      for(int j=0;j<a.getNumberOfParam<vector<int> >();j++)
	{
	  a.getParam<vector<int> >(j).getData();
	  o<<"N"<<a.getParam<vector<int> >(j).getData().size()<<" ";
	  for(unsigned int k=0;k<a.getParam<vector<int> >(j).getData().size();k++)
	    {
	      o<<a.getParam<vector<int> >(j).getData().at(k)<<" ";
	    }
	  o<<a.getParam<vector<int> >(j).getName().data()<<endl;
	}
      //o<< a.getNumberOfParam<vector<float> >()<<" ";
      for(int j=0;j<a.getNumberOfParam<vector<float> >();j++)
	{
	  a.getParam<vector<float> >(j).getData();
	  o<<"T"<<a.getParam<vector<float> >(j).getData().size()<<" ";
	  for(unsigned int k=0;k<a.getParam<vector<float> >(j).getData().size();k++)
	    {
	      o<<a.getParam<vector<float> >(j).getData().at(k)<<" ";
	    }
	  o<<a.getParam<vector<float> >(j).getName().data()<<endl;
	}
      //o<< a.getNumberOfParam<vector<string> >()<<" ";
      for(int j=0;j<a.getNumberOfParam<vector<string> >();j++)
	{
	  a.getParam<vector<string> >(j).getData();
	  o<<"R"<<a.getParam<vector<string> >(j).getData().size()<<" ";
	  for(unsigned int k=0;k<a.getParam<vector<string> >(j).getData().size();k++)
	    {
	      o<<a.getParam<vector<string> >(j).getData().at(k)<<"\n";
	    }
	  o<<a.getParam<vector<string> >(j).getName().data()<<endl;
	}
      //o<<a.getNumberOfParam<algorithm_parameter>() <<" ";
      for(int i =0;i<a.getNumberOfParam<algorithm_parameter>();i++)
	o<<"A"<<a.getParam<algorithm_parameter>(i).getData();//<<" "<<(a.getParam<algorithm_parameter>(i).getName().data())<<endl;    
      
      //      if(a.getNumberOfParam<algorithm_parameter>()!=0) o<<" ";
      //o << a.getName().data()<<endl;
      //o << a.getDescription().size()<<" ";
//       for(unsigned int j=0;j<a.getDescription().size();j++)
// 	{
// 	  o<<a.getDescription(j).data()<<endl;
// 	}
    }
  return o;
}
// #include <TClass.h>
// void algorithm_parameter::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	algorithm_parameter::Class()->WriteBuffer(b, this);
// 	b<<integers.size();
// 	for(unsigned int i=0;i<integers.size();i++)
// 	    b<<integers[i].getData()<<integers[i].getName().data();
// 	b<<floats.size();
// 	for(unsigned int i=0;i<floats.size();i++)
// 	    b<<floats[i].getData()<<floats[i].getName().data();
// 	b<<points.size();
// 	for(unsigned int i=0;i<points.size();i++)
// 	    b<<points[i].getData().X()<<points[i].getData().Y()<<points[i].getData().Z()<<points[i].getName().data();
// 	b<<switches.size();
// 	for(unsigned int i=0;i<switches.size();i++)
// 	    b<<switches[i].getData()<<switches[i].getName().data();
// 	b<<stringes.size();
// 	for(unsigned int i=0;i<stringes.size();i++)
// 	    b<<stringes[i].getData().data()<<stringes[i].getName().data();
// 	b<<vectors.size();
// 	for(unsigned int i=0;i<vectors.size();i++)
// 	    b<<vectors[i].getData().X()<<vectors[i].getData().Y()<<vectors[i].getData().Z()<<vectors[i].getName().data();
// // 	b<<bitmaps.size();
// // 	for(unsigned int i=0;i<bitmaps.size();i++)
// // 	{
// // 	    b<<bitmaps[i].width()<<bitmaps[i].height();
// // 	    for(unsigned int j=0;j<bitmaps[i].width();j++)
// // 		for(unsigned int k=0;k<bitmaps[i].height();k++)
// // 		    b<<bitmaps[i];
// // 	}
// 	b<<intVectors.size();
// 	for(unsigned int i=0;i<intVectors.size();i++)
// 	{
// 	    b<<intVectors[i].getData().size();
// 	    for(unsigned int j=0;j<intVectors[i].getData().size();j++)
// 		b<<intVectors[i].getData().at(j);
// 	    b<<intVectors[i].getName().data();
// 	}
//     }
//     else
//     {
// 	algorithm_parameter::Class()->ReadBuffer(b, this);
// 	int inum,inum2;
// 	float fnum,fnum2,fnum3;
// 	bool bnum;
// 	char *line=0;
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>inum>>line;
// 	    integers.push_back(single_parameter<int>(line,inum));
// 	}
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>line;
// 	    floats.push_back(single_parameter<float>(line,fnum));
// 	}
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>fnum2>>fnum3>>line;
// 	    points.push_back(single_parameter<point3D>(line,point3D(fnum,fnum2,fnum3)));
// 	}
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>bnum>>line;
// 	    switches.push_back(single_parameter<bool>(line,bnum));
// 	}
// 	b>>inum2;
// 	char *l=0;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>l>>line;
// 	    stringes.push_back(single_parameter<string>(line,string(l)));
// 	}
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>fnum2>>fnum3>>line;
// 	    vectors.push_back(single_parameter<vector3D>(line,vector3D(fnum,fnum2,fnum3)));
// 	}
// // 	b<<bitmaps.size();
// // 	for(unsigned int i=0;i<bitmaps.size();i++)
// // 	{
// // 	    b<<bitmaps[i].width()<<bitmaps[i].height();
// // 	    for(unsigned int j=0;j<bitmaps[i].width();j++)
// // 		for(unsigned int k=0;k<bitmaps[i].height();k++)
// // 		    b<<bitmaps[i];
// // 	}
// 	int inum3;
// 	b>>inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>inum3;
// 	    vector<int> tmp;
// 	    for(int j=0;j<inum3;j++)
// 	    {
// 		b>>inum;
// 		tmp.push_back(inum);
// 	    }
// 	    b>>line;
// 	    intVectors.push_back(single_parameter<vector<int> >(line,tmp));
// 	}
//     }
// }
#if __GNUC__ > 3
//template<class X> void algorithm_parameter::changeParam(string name, X data, int pos)
template void algorithm_parameter::changeParam<int>(string name, int data);
template void algorithm_parameter::changeParam<int>(string name, int data, int pos);
template void algorithm_parameter::popParam<int>();
template int algorithm_parameter::getNumberOfParam<int>()const;
template single_parameter<int> algorithm_parameter::getParam<int>(int i)const;
template void algorithm_parameter::addParam<int>(single_parameter<int> data);

template void algorithm_parameter::changeParam<float>(string name, float data);
template void algorithm_parameter::changeParam<float>(string name, float data, int pos);
template void algorithm_parameter::popParam<float>();
template int algorithm_parameter::getNumberOfParam<float>()const;
template single_parameter<float> algorithm_parameter::getParam<float>(int i)const;
template void algorithm_parameter::addParam<float>(single_parameter<float> data);

template void algorithm_parameter::changeParam<bool>(string name, bool data);
template void algorithm_parameter::changeParam<bool>(string name, bool data, int pos);
template void algorithm_parameter::popParam<bool>();
template int algorithm_parameter::getNumberOfParam<bool>()const;
template single_parameter<bool> algorithm_parameter::getParam<bool>(int i)const;
template void algorithm_parameter::addParam<bool>(single_parameter<bool> data);

template void algorithm_parameter::changeParam<point3D>(string name, point3D data);
template void algorithm_parameter::changeParam<point3D>(string name, point3D data, int pos);
template void algorithm_parameter::popParam<point3D>();
template int algorithm_parameter::getNumberOfParam<point3D>()const;
template single_parameter<point3D> algorithm_parameter::getParam<point3D>(int i)const;
template void algorithm_parameter::addParam<point3D>(single_parameter<point3D> data);

template void algorithm_parameter::changeParam<vector3D>(string name, vector3D data);
template void algorithm_parameter::changeParam<vector3D>(string name, vector3D data, int pos);
template void algorithm_parameter::popParam<vector3D>();
template int algorithm_parameter::getNumberOfParam<vector3D>()const;
template single_parameter<vector3D> algorithm_parameter::getParam<vector3D>(int i)const;
template void algorithm_parameter::addParam<vector3D>(single_parameter<vector3D> data);

template void algorithm_parameter::changeParam<string>(string name, string data);
template void algorithm_parameter::changeParam<string>(string name, string data, int pos);
template void algorithm_parameter::popParam<string>();
template int algorithm_parameter::getNumberOfParam<string>()const;
template single_parameter<string> algorithm_parameter::getParam<string>(int i)const;
template void algorithm_parameter::addParam<string>(single_parameter<string> data);

// template void algorithm_parameter::changeParam<QBitArray>(string name, QBitArray data);
// template void algorithm_parameter::changeParam<QBitArray>(string name, QBitArray data, int pos);

template void algorithm_parameter::changeParam<vector<int> >(string name, vector<int> data);
template void algorithm_parameter::changeParam<vector<int> >(string name, vector<int> data, int pos);
template void algorithm_parameter::popParam<vector<int> >();
template int algorithm_parameter::getNumberOfParam<vector<int> >()const;
template single_parameter<vector<int> > algorithm_parameter::getParam<vector<int> >(int i)const;
template void algorithm_parameter::addParam<vector<int> >(single_parameter<vector<int> > data);

template void algorithm_parameter::changeParam<vector<float> >(string name, vector<float> data);
template void algorithm_parameter::changeParam<vector<float> >(string name, vector<float> data, int pos);
template void algorithm_parameter::popParam<vector<float> >();
template int algorithm_parameter::getNumberOfParam<vector<float> >()const;
template single_parameter<vector<float> > algorithm_parameter::getParam<vector<float> >(int i)const;
template void algorithm_parameter::addParam<vector<float> >(single_parameter<vector<float> > data);

template void algorithm_parameter::changeParam<vector<string> >(string name, vector<string> data);
template void algorithm_parameter::changeParam<vector<string> >(string name, vector<string> data, int pos);
template void algorithm_parameter::popParam<vector<string> >();
template int algorithm_parameter::getNumberOfParam<vector<string> >()const;
template single_parameter<vector<string> > algorithm_parameter::getParam<vector<string> >(int i)const;
template void algorithm_parameter::addParam<vector<string> >(single_parameter<vector<string> > data);

template void algorithm_parameter::changeParam<algorithm_parameter>(string name, algorithm_parameter data);
template void algorithm_parameter::changeParam<algorithm_parameter>(string name, algorithm_parameter data,int pos);
template void algorithm_parameter::popParam<algorithm_parameter>();
template int algorithm_parameter::getNumberOfParam<algorithm_parameter>()const;
template single_parameter<algorithm_parameter> algorithm_parameter::getParam<algorithm_parameter>(int i)const;
template void algorithm_parameter::addParam<algorithm_parameter>(single_parameter<algorithm_parameter> data);
template single_parameter<int> algorithm_parameter::getParam<int>(const string &nme)const;
template single_parameter<float> algorithm_parameter::getParam<float>(const string &nme)const;
template single_parameter<bool> algorithm_parameter::getParam<bool>(const string &nme)const;
template single_parameter<point3D> algorithm_parameter::getParam<point3D>(const string &nme)const;
template single_parameter<vector3D> algorithm_parameter::getParam<vector3D>(const string &nme)const;
template single_parameter<string> algorithm_parameter::getParam<string>(const string &nme)const;
template single_parameter<vector<int> > algorithm_parameter::getParam<vector<int> >(const string &nme)const;
template single_parameter<vector<float> > algorithm_parameter::getParam<vector<float> >(const string &nme)const;
template single_parameter<vector<string> > algorithm_parameter::getParam<vector<string> >(const string &nme)const;
template single_parameter<algorithm_parameter> algorithm_parameter::getParam<algorithm_parameter>(const string &nme)const;

/*
template void algorithm_parameter::changeParam<>(string name,  data);
template void algorithm_parameter::changeParam<>(string name,  data, int pos);
template void algorithm_parameter::popParam<>();
template int algorithm_parameter::getNumberOfParam<>()const;
template single_parameter<> algorithm_parameter::getParam<>(int i)const;
template void algorithm_parameter::addParam<>(single_parameter<> data);
template single_parameter<> algorithm_parameter::getParam<>(const string &nme)const;
*/
#endif
