#include "tmyclass.h"
#include <fstream>
#include <vector>
extern bool existing(std::string filename);
std::vector<TMyClass*> knownClasses;
std::vector<std::string> knownTypes;
std::vector<std::string> knownTypesURL;
std::vector<functionNameTypeDoc> knownFunctions;
TMyClass* getKnownClass(const std::string& name);
std::string knownTypesDoc(const std::string& type);
void addToKnownTypes(const std::string& classIn, const std::string& url);
void addToKnownClasses(const TMyClass& classIn);
void initKnownClasses(const std::string& filename);
void initKnownTypes(const std::string& filename);
void writeKnownTypes(const std::string& filename);
std::ofstream logg("log.txt");
#ifndef JUSTDEFINE
TMyClass* getKnownClass(const std::string& name)
{
  for (unsigned int i = 0; i < knownClasses.size(); i++) {
    if (name == knownClasses[i]->Name())
      return knownClasses[i];
  }
  return NULL;
}
std::string knownTypesDoc(const std::string& type)
{
  for (unsigned int i = 0; i < knownTypes.size(); i++) {
    if (knownTypes[i] == type)
      return knownTypesURL[i];
  }
  return "";
}
void addToKnownTypes(const std::string& classIn, const std::string& url)
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
void initKnownClasses(const std::string& filename)
{
  if (!existing(filename))
    return;
  std::ifstream input;
  input.open(filename.data());
  std::vector<std::string> toread;
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
      std::cout << "file \"" << toread[i].data() << "\" not found" << std::endl;
      continue;
    }
    std::ifstream input1;
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
void initKnownTypes(const std::string& filename)
{
  if (!existing(filename))
    return;
  char li[1000], li1[1000];
  std::ifstream input;
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
void writeKnownTypes(const std::string& filename)
{
  std::ofstream out;
  out.open(filename.data());
  for (unsigned int i = 0; i < knownTypes.size(); i++)
    out << knownTypes[i].data() << std::endl << knownTypesURL[i].data() << std::endl;
  out.close();
}
void addKnownFunction(const functionNameTypeDoc& func)
{
  bool init = false;
  for (unsigned int i = 0; i < knownFunctions.size(); i++)
    if (knownFunctions[i] == func) {
      init = true;
      std::cout << knownFunctions[i].toString().data() << std::endl;
    }
  if (!init)
    knownFunctions.push_back(func);
  else
    std::cout << func.toString().data() << " already defined" << std::endl;
}
void initKnownFunctions(const std::string& filename)
{
  if (!existing(filename))
    return;
  std::ifstream input;
  input.open(filename.data());
  while (!input.eof()) {
    functionNameTypeDoc tmp;
    input >> tmp;
    if (tmp.Name() != "")
      addKnownFunction(tmp);
  }
  input.close();
}
void writeKnownFunctions(const std::string& filename)
{
  std::ofstream output;
  output.open(filename.data());
  for (unsigned int i = 0; i < knownFunctions.size(); i++)
    output << knownFunctions[i];
  output.close();
}
#endif
