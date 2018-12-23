#ifndef __BASEPARAM
#define __BASEPARAM

#include <string>
#include <vector>
#include <iostream>

using namespace std;

template<class X>
bool takeItemFromVector(vector<X>&,X&);

template<class X>
bool moveItemToPosVector(vector<X>&,X&,int);


template<class X>class single_parameter
{
private:
  string name; //!
  X data;
public:
    single_parameter();
    single_parameter(const string & Name, X Data);
    single_parameter(const single_parameter & sp);
    ~single_parameter();
    single_parameter& operator=(const single_parameter& s);
    bool operator==(const single_parameter&s)const;
    bool operator==(const string&s)const;
    string getName() const;
    void setName(const string & n); 
    X getData() const;
    void setData(X d);
};

class base_parameter
{
private:
  string name;//!
  vector<string> description;//!
public:
    base_parameter();
    virtual ~base_parameter();
    void setName(const string &n);
    void setDescription(vector<string> d);
    void setDescription(const string &d, int line);
    string getName() const;
    string getDescription(int line) const;
    vector<string> getDescription() const;
    bool operator<(base_parameter p);
    bool operator>(base_parameter p);
    bool operator==(base_parameter p);
    bool operator<=(base_parameter p);
    bool operator>=(base_parameter p);
};

#endif
