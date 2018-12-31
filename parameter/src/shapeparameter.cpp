#include "shapeparameter.h"
#include <typeinfo>
shape_parameter::shape_parameter() : base_parameter(), fcompleteWrite(false) {}
shape_parameter::shape_parameter(const shape_parameter& sp)
    : base_parameter(sp), fcompleteWrite(sp.completeWrite())
{
  setName(sp.getName());
  setDescription(sp.getDescription());
  for (int i = 0; i < sp.NumberOfParams<point3D>(); i++)
    addParam<point3D>(sp.getParam<point3D>(i), sp.getParamName<point3D>(i));
  for (int i = 0; i < sp.NumberOfParams<vector3D>(); i++)
    addParam<vector3D>(sp.getParam<vector3D>(i), sp.getParamName<vector3D>(i));
  for (int i = 0; i < sp.NumberOfParams<int>(); i++)
    addParam<int>(sp.getParam<int>(i), sp.getParamName<int>(i));
  for (int i = 0; i < sp.NumberOfParams<float>(); i++)
    addParam<float>(sp.getParam<float>(i), sp.getParamName<float>(i));
  for (int i = 0; i < sp.NumberOfParams<string>(); i++)
    addParam<string>(sp.getParam<string>(i), sp.getParamName<string>(i));
}

shape_parameter::~shape_parameter() {}

template <class T>
T shape_parameter::getParam(int i) const
{
  if (i < 0 || i >= NumberOfParams<T>())
    return T();
  const void* pt = 0;
  if (typeid(T) == typeid(int))
    pt = &integers;
  if (typeid(T) == typeid(float))
    pt = &floats;
  if (typeid(T) == typeid(point3D))
    pt = &points;
  if (typeid(T) == typeid(vector3D))
    pt = &vectors;
  if (typeid(T) == typeid(string))
    pt = &strings_;
  if (pt == 0)
    return T();
  return ((vector<single_parameter<T>>*)pt)->at(i).getData();
}

template <class T>
string shape_parameter::getParamName(int i) const
{
  if (i < 0 || i >= NumberOfParams<T>())
    return "";
  if (typeid(T) == typeid(int))
    return integers[i].getName();
  if (typeid(T) == typeid(float))
    return floats[i].getName();
  if (typeid(T) == typeid(point3D))
    return points[i].getName();
  if (typeid(T) == typeid(vector3D))
    return vectors[i].getName();
  if (typeid(T) == typeid(string))
    return strings_[i].getName();
  return "";
}

template <class T>
void shape_parameter::setParam(int i, const T& p)
{
  if (i < 0 || i >= NumberOfParams<T>())
    return;
  void* pt = 0;
  if (typeid(T) == typeid(int))
    pt = &integers;
  if (typeid(T) == typeid(float))
    pt = &floats;
  if (typeid(T) == typeid(point3D))
    pt = &points;
  if (typeid(T) == typeid(vector3D))
    pt = &vectors;
  if (typeid(T) == typeid(string))
    pt = &strings_;
  if (pt == 0)
    return;
  ((vector<single_parameter<T>>*)pt)->at(i).setData(p);
}

template <class T>
void shape_parameter::setParamName(int i, string n)
{
  if (i < 0 || i >= NumberOfParams<T>())
    return;
  void* p = 0;
  if (typeid(T) == typeid(int))
    p = &integers;
  if (typeid(T) == typeid(float))
    p = &floats;
  if (typeid(T) == typeid(point3D))
    p = &points;
  if (typeid(T) == typeid(vector3D))
    p = &vectors;
  if (typeid(T) == typeid(string))
    p = &strings_;
  if (p == 0)
    return;
  ((vector<single_parameter<T>>*)p)->at(i).setName(n);
}

template <class T>
void shape_parameter::addParam(T p, string n)
{
  void* pt = 0;
  if (typeid(T) == typeid(int))
    pt = &integers;
  if (typeid(T) == typeid(float))
    pt = &floats;
  if (typeid(T) == typeid(point3D))
    pt = &points;
  if (typeid(T) == typeid(vector3D))
    pt = &vectors;
  if (typeid(T) == typeid(string))
    pt = &strings_;
  if (pt != 0)
    ((vector<single_parameter<T>>*)pt)->push_back(single_parameter<T>(n, p));
}

template <class T>
int shape_parameter::NumberOfParams() const
{
  if (typeid(T) == typeid(int))
    return integers.size();
  if (typeid(T) == typeid(float))
    return floats.size();
  if (typeid(T) == typeid(point3D))
    return points.size();
  if (typeid(T) == typeid(vector3D))
    return vectors.size();
  if (typeid(T) == typeid(string))
    return strings_.size();
  return -1;
}

void shape_parameter::operator=(const shape_parameter& sh)
{
  setName(sh.getName());
  setDescription(sh.getDescription());
  clearProperties();
  fcompleteWrite = sh.completeWrite();
  for (int i = 0; i < sh.NumberOfParams<point3D>(); i++)
    addParam<point3D>(sh.getParam<point3D>(i), sh.getParamName<point3D>(i));
  for (int i = 0; i < sh.NumberOfParams<vector3D>(); i++)
    addParam<vector3D>(sh.getParam<vector3D>(i), sh.getParamName<vector3D>(i));
  for (int i = 0; i < sh.NumberOfParams<int>(); i++)
    addParam<int>(sh.getParam<int>(i), sh.getParamName<int>(i));
  for (int i = 0; i < sh.NumberOfParams<float>(); i++)
    addParam<float>(sh.getParam<float>(i), sh.getParamName<float>(i));
  for (int i = 0; i < sh.NumberOfParams<string>(); i++)
    addParam<string>(sh.getParam<string>(i), sh.getParamName<string>(i));
}
bool shape_parameter::completeWrite() const { return fcompleteWrite; }
void shape_parameter::setCompleteWrite(bool value) { fcompleteWrite = value; }

void shape_parameter::clearProperties()
{
  while (!points.empty())
    points.pop_back();
  while (!vectors.empty())
    vectors.pop_back();
  while (!integers.empty())
    integers.pop_back();
  while (!floats.empty())
    floats.pop_back();
  while (!strings_.empty())
    strings_.pop_back();
}
istream& operator>>(istream& i, shape_parameter& sh)
{
  char c, ch;
  float f[3];
  string s = "";
  i.get(c);
  while (c != '\n') {
    s = s + c;
    i.get(c);
  }
  sh.setName(s);
  i.get(c);
  while (c != '=' && !i.eof()) {
    i.get(ch);
    switch (c) {
    case 'P': {
      point3D p;
      if (ch != ' ') {
        i.unget();
        for (int j = 0; j < 3; j++)
          i >> f[j];
        p.setValues(f[0], f[1], f[2]);
        sh.setCompleteWrite(true);
      }
      s = "";
      i.get(c);
      while (c != '\n') {
        s = s + c;
        i.get(c);
      }
      sh.addParam<point3D>(p, s);
      break;
    }
    case 'V': {
      vector3D p;
      if (ch != ' ') {
        i.unget();
        for (int j = 0; j < 3; j++)
          i >> f[j];
        p.setValues(sin(f[0]) * cos(f[1]) * f[2], sin(f[0]) * sin(f[1]) * f[2], cos(f[0]) * f[2]);
        sh.setCompleteWrite(true);
      }
      s = "";
      i.get(c);
      while (c != '\n') {
        s = s + c;
        i.get(c);
      }
      sh.addParam<vector3D>(p, s);
      break;
    }
    case 'v': {
      vector3D p;
      if (ch != ' ') {
        i.unget();
        for (int j = 0; j < 3; j++)
          i >> f[j];
        p.setValues(sin(f[0] / 180. * M_PI) * cos(f[1] / 180. * M_PI) * f[2],
                    sin(f[0] / 180. * M_PI) * sin(f[1] / 180. * M_PI) * f[2],
                    cos(f[0] / 180. * M_PI) * f[2]);
        sh.setCompleteWrite(true);
      }
      s = "";
      i.get(c);
      while (c != '\n') {
        s = s + c;
        i.get(c);
      }
      sh.addParam<vector3D>(p, s);
      break;
    }
    case 'F': {
      float p = 0;
      if (ch != ' ') {
        i.unget();
        i >> p;
        sh.setCompleteWrite(true);
      }
      s = "";
      i.get(c);
      while (c != '\n') {
        s = s + c;
        i.get(c);
      }
      sh.addParam<float>(p, s);
      break;
    }
    case 'I': {
      int p;
      if (ch != ' ') {
        i.unget();
        i >> p;
        sh.setCompleteWrite(true);
      }
      s = "";
      i.get(c);
      while (c != '\n') {
        s = s + c;
        i.get(c);
      }
      sh.addParam<int>(p, s);
      break;
    }
      // 	case 'S':
      // 	  {
      // 	    string p="";
      // 	    if(ch!=' ')
      // 	      {
      // 		i.unget();
      // 		i.get(c);while(c!='\n'){p=p+c;i.get(c);}
      // 		sh.setCompleteWrite(true);
      // 	      }
      // 	    s="";i.get(c);while(c!='\n'){s=s+c;i.get(c);}
      // 	    sh.addParam<point3D>(p,s);
      // 	    break;
      // 	  }
    }
    i.get(c);
  }
  i.get(c);
  while (c != '\n' && !i.eof()) {
    i.get(c);
  }
  return i;
}

ostream& operator<<(ostream& o, const shape_parameter& sh)
{
  o << sh.getName().data() << endl;
  if (sh.completeWrite()) {
    for (int k = 0; k < sh.NumberOfParams<point3D>(); k++)
      o << "P" << sh.getParam<point3D>(k).x() << " " << sh.getParam<point3D>(k).y() << " "
        << sh.getParam<point3D>(k).z() << " " << sh.getParamName<point3D>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++)
      o << "v" << sh.getParam<vector3D>(k).Theta() * 180. / M_PI << " "
        << sh.getParam<vector3D>(k).Phi() * 180. / M_PI << " " << sh.getParam<vector3D>(k).R()
        << " " << sh.getParamName<vector3D>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<float>(); k++)
      o << "F" << sh.getParam<float>(k) << " " << sh.getParamName<float>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<int>(); k++)
      o << "I" << sh.getParam<int>(k) << " " << sh.getParamName<int>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<string>(); k++)
      o << "S" << sh.getParam<string>(k) << " " << sh.getParamName<string>(k).data() << endl;
  } else {
    for (int k = 0; k < sh.NumberOfParams<point3D>(); k++)
      o << "P " << sh.getParamName<point3D>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++)
      o << "V " << sh.getParamName<vector3D>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<float>(); k++)
      o << "F " << sh.getParamName<float>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<int>(); k++)
      o << "I " << sh.getParamName<int>(k).data() << endl;
    for (int k = 0; k < sh.NumberOfParams<string>(); k++)
      o << "S " << sh.getParamName<string>(k).data() << endl;
  }
  o << "=" << endl;
  return o;
  int zahl = sh.NumberOfParams<point3D>();
  o << (zahl) << " ";
  zahl = sh.NumberOfParams<vector3D>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<int>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<float>();
  o << zahl << " ";
  zahl = sh.NumberOfParams<string>();
  o << zahl << sh.getName().data() << endl;
  for (int k = 0; k < sh.NumberOfParams<point3D>(); k++) {
    o << sh.getParamName<point3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++) {
    o << sh.getParamName<vector3D>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<int>(); k++) {
    o << sh.getParamName<int>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<float>(); k++) {
    o << sh.getParamName<float>(k).data() << endl;
  }
  for (int k = 0; k < sh.NumberOfParams<string>(); k++) {
    o << sh.getParamName<string>(k).data() << endl;
  }
  return o;
}
// #include <TClass.h>
// void shape_parameter::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	shape_parameter::Class()->WriteBuffer(b, this);
// 	b<<points.size();
// 	for(unsigned int i=0;i<points.size();i++)
// 	    b<<points[i].getData().X()<<points[i].getData().Y()<<points[i].getData().Z()<<points[i].getName().data();
// 	b<<vectors.size();
// 	for(unsigned int i=0;i<vectors.size();i++)
// 	    b<<vectors[i].getData().X()<<vectors[i].getData().Y()<<vectors[i].getData().Z()<<vectors[i].getName().data();
// 	b<<integers.size();
// 	for(unsigned int i=0;i<integers.size();i++)
// 	    b<<integers[i].getData()<<integers[i].getName().data();
// 	b<<floats.size();
// 	for(unsigned int i=0;i<floats.size();i++)
// 	    b<<floats[i].getData()<<floats[i].getName().data();
// 	b<<strings_.size();
// 	for(unsigned int i=0;i<strings_.size();i++)
// 	    b<<strings_[i].getData().data()<<strings_[i].getName().data();

//     }
//     else
//     {
// 	shape_parameter::Class()->ReadBuffer(b, this);
// 	char* line=0;
// 	int inum,inum2=0;
// 	float fnum,fnum2,fnum3;
// 	b<<inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>fnum2>>fnum3>>line;
// 	    points.push_back(single_parameter<point3D>(string(line),point3D(fnum,fnum2,fnum3)));
// 	}
// 	b<<inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>fnum2>>fnum3>>line;
// 	    vectors.push_back(single_parameter<vector3D>(string(line),vector3D(fnum,fnum2,fnum3)));
// 	}
// 	b<<inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>inum>>line;
// 	    integers.push_back(single_parameter<int>(string(line),inum));
// 	}
// 	b<<inum2;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>fnum>>line;
// 	    floats.push_back(single_parameter<float>(string(line),fnum));
// 	}
// 	b<<inum2;
// 	char *l1=0;
// 	for(int i=0;i<inum2;i++)
// 	{
// 	    b>>line;
// 	    b>>l1;
// 	    strings_.push_back(single_parameter<string>(string(l1),string(line)));
// 	}
//     }
// }

template void shape_parameter::setParam<int>(int i, const int& p);
template void shape_parameter::setParamName<int>(int i, string n);

template void shape_parameter::setParam<float>(int i, const float& p);
template void shape_parameter::setParamName<float>(int i, string n);

template void shape_parameter::setParam<point3D>(int i, const point3D& p);
template void shape_parameter::setParamName<point3D>(int i, string n);

template void shape_parameter::setParam<vector3D>(int i, const vector3D& p);
template void shape_parameter::setParamName<vector3D>(int i, string n);

template void shape_parameter::setParam<string>(int i, const string& p);
template void shape_parameter::setParamName<string>(int i, string n);
