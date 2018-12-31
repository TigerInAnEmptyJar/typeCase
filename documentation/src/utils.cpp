#include "tmyclass.h"
#include <fstream>
#include <vector>
extern bool existing(string filename);
vector<TMyClass*> knownClasses;
vector<string> knownTypes;
vector<string> knownTypesURL;
vector<functionNameTypeDoc> knownFunctions;
TMyClass* getKnownClass(const string& name);
string knownTypesDoc(const string& type);
void addToKnownTypes(const string& classIn, const string& url);
void addToKnownClasses(const TMyClass& classIn);
void initKnownClasses(const string& filename);
void initKnownTypes(const string& filename);
void writeKnownTypes(const string& filename);
ofstream logg("log.txt");
#ifndef JUSTDEFINE
TMyClass* getKnownClass(const string& name)
{
  for (unsigned int i = 0; i < knownClasses.size(); i++) {
    if (name == knownClasses[i]->Name())
      return knownClasses[i];
  }
  return NULL;
}
string knownTypesDoc(const string& type)
{
  for (unsigned int i = 0; i < knownTypes.size(); i++) {
    if (knownTypes[i] == type)
      return knownTypesURL[i];
  }
  return "";
}
void addToKnownTypes(const string& classIn, const string& url)
{
  int found = -1;
  for (unsigned int i = 0; i < knownTypes.size(); i++)
    if (knownTypes[i] == classIn)
      found = i;
  if (found == -1) {
    knownTypes.push_back(classIn);
    knownTypesURL.push_back(url);
  } else if (url != "")
    knownTypesURL[found] = url;
}
void addToKnownClasses(const TMyClass& classIn)
{
  int found = -1;
  for (unsigned int i = 0; i < knownClasses.size(); i++)
    if (knownClasses[i]->Name() == classIn.Name())
      found = i;
  if (found == -1)
    knownClasses.push_back(new TMyClass(classIn));
  else
    (*knownClasses[found]) = classIn, addToKnownTypes(classIn.Name(), classIn.Doc());
}
void initKnownClasses(const string& filename)
{
  if (!existing(filename))
    return;
  ifstream input;
  input.open(filename.data());
  vector<string> toread;
  char li[200];
  while (input.good() && !input.eof()) {
    input.getline(li, 200);
    toread.push_back(li);
    if (toread.back() == "")
      toread.pop_back();
  }
  input.close();
  for (unsigned int i = 0; i < toread.size(); i++) {
    if (!existing(toread[i])) {
      cout << "file \"" << toread[i].data() << "\" not found" << endl;
      continue;
    }
    ifstream input1;
    input1.open(toread[i].data());
    bool finished = false;
    ;
    while (!finished && (!input1.eof()) && input1.good()) {
      TMyClass tmp1;
      input1 >> tmp1;
      if (tmp1.Name() != "") {
        TMyClass* tmp = new TMyClass(tmp1);
        knownClasses.push_back(tmp);
      } else {
        finished = true;
      }
    }
    input1.close();
  }
}
void initKnownTypes(const string& filename)
{
  if (!existing(filename))
    return;
  char li[1000], li1[1000];
  ifstream input;
  input.open(filename.data());
  while (!input.eof() && input.good()) {
    input.getline(li, 1000);
    if (input.eof())
      return;
    input.getline(li1, 1000);
    knownTypes.push_back(li);
    knownTypesURL.push_back(li1);
    if (knownTypes.back() == "" || knownTypesURL.back() == "") {
      knownTypes.pop_back();
      knownTypesURL.pop_back();
    }
  }
  input.close();
}
void writeKnownTypes(const string& filename)
{
  ofstream out;
  out.open(filename.data());
  for (unsigned int i = 0; i < knownTypes.size(); i++)
    out << knownTypes[i].data() << endl << knownTypesURL[i].data() << endl;
  out.close();
}
void addKnownFunction(const functionNameTypeDoc& func)
{
  bool init = false;
  for (unsigned int i = 0; i < knownFunctions.size(); i++)
    if (knownFunctions[i] == func) {
      init = true;
      cout << knownFunctions[i].toString().data() << endl;
    }
  if (!init)
    knownFunctions.push_back(func);
  else
    cout << func.toString().data() << " already defined" << endl;
}
void initKnownFunctions(const string& filename)
{
  if (!existing(filename))
    return;
  ifstream input;
  input.open(filename.data());
  while (!input.eof()) {
    functionNameTypeDoc tmp;
    input >> tmp;
    if (tmp.Name() != "")
      addKnownFunction(tmp);
  }
  input.close();
}
void writeKnownFunctions(const string& filename)
{
  ofstream output;
  output.open(filename.data());
  for (unsigned int i = 0; i < knownFunctions.size(); i++)
    output << knownFunctions[i];
  output.close();
}
#endif
