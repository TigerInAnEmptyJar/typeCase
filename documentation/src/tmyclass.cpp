#include "tmyclass.h"
#include <fstream>
#include "string_utilities.h"
#include <stdlib.h>
void minimizeString(string &s);
bool addSuperFromString(TMyClass & cls, string str);
extern void addToKnownClasses(const TMyClass &classIn);
extern TMyClass* getKnownClass(const string &name);
extern string knownTypesDoc(const string &type);
extern bool existing(string filename);
vector<functionNameTypeDoc> qobjectFunctions;
vector<functionNameTypeDoc> linkdefFunctions;
TMyClass::TMyClass(string name, string doc)
{
  className=name;
  classDoc=doc;
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
}
TMyClass::TMyClass(const TMyClass &n)
{
  className=n.Name();
  classDoc=n.Doc();
  headerfile=n.HeaderFile();
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  functionType.push_back(vector<int>());
  for(int i=0;i<n.nSuperClasses();i++)
    addSuperClass(n.SuperClass(i),n.SuperClassDoc(i),n.InherritanceType(i),n.Super(i));

  for(int j=0;j<3;j++)for(int i=0;i<n.nVariables(j);i++)addVariable(j,n.getVariable(i,j));
  for(int j=0;j<7;j++)for(int i=0;i<n.nMethods(j);i++)addMethod(j,n.getMethod(i,j));
  for(int i=0;i<n.nConstructors();i++)addConstructor(n.getConstructor(i));
  for(int i=0;i<n.DescriptionLines();i++)description.push_back(n.Description(i));
}
TMyClass::~TMyClass(){}
TMyClass &TMyClass::operator=(const TMyClass& n)
{
  clear();
  className=n.Name();
  classDoc=n.Doc();
  headerfile=n.HeaderFile();
  for(int i=0;i<n.nSuperClasses();i++)
    addSuperClass(n.SuperClass(i),n.SuperClassDoc(i),n.InherritanceType(i),n.Super(i));

  for(int j=0;j<3;j++)for(int i=0;i<n.nVariables(j);i++)addVariable(j,n.getVariable(i,j));
  for(int j=0;j<7;j++)for(int i=0;i<n.nMethods(j);i++)addMethod(j,n.getMethod(i,j));
  for(int i=0;i<n.nConstructors();i++)addConstructor(n.getConstructor(i));
  for(int i=0;i<n.DescriptionLines();i++)description.push_back(n.Description(i));
  return *this;
}
void TMyClass::clear()
{
    clearDescription();
    clearSuperClasses();
    clearConstructor();
    for(int i=0;i<3;i++)
      clearVariable(i);
    for(int i=0;i<7;i++)
      clearMethod(i);
}
string TMyClass::Name()const
{
  return className;
}
void TMyClass::setName(const string &s)
{
  className=s;
}
string TMyClass::Doc()const
{
  return classDoc;
}
void TMyClass::setDoc(const string &s)
{
  classDoc=s;
}
string TMyClass::Description(int i)const
{
  if(i<0||i>(int)description.size())return "";
  return description[i];
}
int TMyClass::DescriptionLines()const
{
  return description.size();
}
void TMyClass::setDescription(const vector<string> &input)
{
  clearDescription();
  for(unsigned int i=0;i<input.size();i++)description.push_back(input[i]);
}
void TMyClass::clearDescription()
{
  while(!description.empty())description.pop_back();
}
void TMyClass::addDescriptionLine(const string&s)
{
  description.push_back(s);
}
string TMyClass::HeaderFile()const
{
  return headerfile;
}
void TMyClass::setHeaderFile(const string &s)
{
  headerfile=s;
}
int TMyClass::nSuperClasses()const
{
  return supers.size();
}
string TMyClass::SuperClass(int i)const
{
  if(i<0||i>(int)supers.size())return "";
  return superClasses[i];
}
string TMyClass::SuperClassDoc(int i)const
{
  if(i<0||i>(int)supers.size())return "";
  return superClassDocs[i];
}
int TMyClass::InherritanceType(int i)const
{
  if(i<0||i>(int)supers.size())return -1;
  return inherritance[i];
}
string TMyClass::Inherritance(int i)const
{
  if(i<0||i>(int)supers.size())return "";
  switch(inherritance[i])
    {
    case 0:return "public";
    case 1:return "protected";
    case 2:return "private";
    }
  return "";
}
TMyClass *TMyClass::Super(int i)const
{
  if(i<0||i>(int)supers.size())return NULL;
  return supers[i];
}
bool TMyClass::inherritsFrom(const string &clsn)
{
  for(unsigned int i=0;i<supers.size();i++)
    if(clsn==superClasses[i])return true;
  vector<TMyClass*> allSuperClasses;
  for(unsigned int k=0;k<supers.size();k++)
    {
      if(supers[k]!=NULL)
	allSuperClasses.push_back(supers[k]);
    }
  int pos=0;
  while(pos<(int)allSuperClasses.size())
    {
      for(int i=0;i<allSuperClasses[pos]->nSuperClasses();i++)
	if(allSuperClasses[pos]->Super(i)!=NULL)
	  allSuperClasses.push_back(allSuperClasses[pos]->Super(i));
      pos++;
    }
  for(unsigned int i=0;i<allSuperClasses.size();i++)
    if(allSuperClasses[i]->Name()==clsn)
      return true;
  return false;
}
int TMyClass::nVariables(int t)const
{
  switch(t)
    {
    case 0:return variablesPublic.size();
    case 1:return variablesProtected.size();
    case 2:return variablesPrivate.size();
    }
  return 0;
}
nameTypeDoc& TMyClass::Variable(int i, int t)
{
  switch(t)
    {
    case 0:
      if(i<0||i>=(int)variablesPublic.size())return variablesPublic[0];
      return variablesPublic[i];
    case 1:
      if(i<0||i>=(int)variablesProtected.size())return variablesProtected[0];
      return variablesProtected[i];
    case 2:
      if(i<0||i>=(int)variablesPrivate.size())return variablesPrivate[0];
      return variablesPrivate[i];
    }
  return variablesPublic[0];
}
nameTypeDoc TMyClass::getVariable(int i, int t)const
{
  switch(t)
    {
    case 0:
      if(i<0||i>=(int)variablesPublic.size())return nameTypeDoc();
      return variablesPublic[i];
    case 1:
      if(i<0||i>=(int)variablesProtected.size())return nameTypeDoc();
      return variablesProtected[i];
    case 2:
      if(i<0||i>=(int)variablesPrivate.size())return nameTypeDoc();
      return variablesPrivate[i];
    }
  return variablesPublic[0];
}
int TMyClass::nMethods(int t)const
{
  switch(t)
    {
    case 0:return functionsPublic.size();
    case 1:return functionsProtected.size();
    case 2:return functionsPrivate.size();
    case 3:return slotsPublic.size();
    case 4:return slotsProtected.size();
    case 5:return slotsPrivate.size();
    case 6:return thesignals.size();
    }
  return 0;
}
functionNameTypeDoc& TMyClass::Method(int i, int t)
{
  switch(t)
    {
    case 0:
      if(i<0||i>=(int)functionsPublic.size())return functionsPublic[0];
      return functionsPublic[i];
    case 1:
      if(i<0||i>=(int)functionsProtected.size())return functionsProtected[0];
      return functionsProtected[i];
    case 2:
      if(i<0||i>=(int)functionsPrivate.size())return functionsPrivate[0];
      return functionsPrivate[i];
    case 3:
      if(i<0||i>=(int)slotsPublic.size())return slotsPublic[0];
      return slotsPublic[i];
    case 4:
      if(i<0||i>=(int)slotsProtected.size())return slotsProtected[0];
      return slotsProtected[i];
    case 5:
      if(i<0||i>=(int)slotsPrivate.size())return slotsPrivate[0];
      return slotsPrivate[i];
    case 6:
      if(i<0||i>=(int)thesignals.size())return thesignals[0];
      return thesignals[i];
    }
  return functionsPublic[0];
}
functionNameTypeDoc TMyClass::getMethod(int i, int t)const
{
  switch(t)
    {
    case 0:
      if(i<0||i>=(int)functionsPublic.size())return functionNameTypeDoc();
      return functionsPublic[i];
    case 1:
      if(i<0||i>=(int)functionsProtected.size())return functionNameTypeDoc();
      return functionsProtected[i];
    case 2:
      if(i<0||i>=(int)functionsPrivate.size())return functionNameTypeDoc();
      return functionsPrivate[i];
    case 3:
      if(i<0||i>=(int)slotsPublic.size())return slotsPublic[0];
      return slotsPublic[i];
    case 4:
      if(i<0||i>=(int)slotsProtected.size())return slotsProtected[0];
      return slotsProtected[i];
    case 5:
      if(i<0||i>=(int)slotsPrivate.size())return slotsPrivate[0];
      return slotsPrivate[i];
    case 6:
      if(i<0||i>=(int)thesignals.size())return thesignals[0];
      return thesignals[i];
    }
  return functionNameTypeDoc();//sPublic[0];
}
int TMyClass::nSlots(int i)const 
{
  switch(i)
    {
    case 0: return slotsPublic.size();
    case 1: return slotsProtected.size();
    case 2: return slotsPrivate.size();
    }
  return 0;
}
int TMyClass::nSignals()const
{
  return thesignals.size();
}
functionNameTypeDoc &TMyClass::Slot(int i, int t)
{
  if(i<0||i>=nSlots(t))return slotsPublic[0];
  switch(t)
    {
    case 0:
      if(i<(int)slotsPublic.size())
	return slotsPublic[i];
      break;
    case 1:
      if(i<(int)slotsProtected.size())
	return slotsProtected[i];
      break;
    case 2:
      if(i<(int)slotsPrivate.size())
	return slotsPrivate[i];
      break;
    }
  return slotsPublic[0];
}
functionNameTypeDoc TMyClass::getSlot(int i, int t)const
{
  if(i<0||i>=nSlots(t))return functionNameTypeDoc();//slotsPublic[0];
  switch(t)
    {
    case 0:
      if(i<(int)slotsPublic.size())
	return slotsPublic[i];
      break;
    case 1:
      if(i<(int)slotsProtected.size())
	return slotsProtected[i];
      break;
    case 2:
      if(i<(int)slotsPrivate.size())
	return slotsPrivate[i];
      break;
    }
  return functionNameTypeDoc();//slotsPublic[0];
}
functionNameTypeDoc &TMyClass::Signal(int i)
{
  if(i<0||i>=(int)thesignals.size())return thesignals[0];
  return thesignals[i];
}
functionNameTypeDoc TMyClass::getSignal(int i)const
{
  if(i<0||i>=(int)thesignals.size())return functionNameTypeDoc();
  return thesignals[i];
}
int TMyClass::nConstructors()const
{
  return constructors.size();
}
functionNameTypeDoc TMyClass::getConstructor(int i)const
{
  if(i<0||i>(int)constructors.size())return functionNameTypeDoc();
  return constructors[i];
}
functionNameTypeDoc& TMyClass::Constructor(int i)
{
  if(i<0||i>(int)constructors.size())return constructors[0];
  return constructors[i];
}
void TMyClass::clearSuperClasses()
{
  while(!supers.empty())
    {
      supers.pop_back();
      superClasses.pop_back();
      superClassDocs.pop_back();
      inherritance.pop_back();
    }
}
void TMyClass::addSuperClass(const string &superClassName, const string& superClassDoc, int derived, TMyClass *super)
{
  supers.push_back(super);
  superClasses.push_back(superClassName);
  superClassDocs.push_back(superClassDoc);
  inherritance.push_back(derived);
}
void TMyClass::addSuperClass(TMyClass *super, int derived)
{
  if(super==NULL)return;
  supers.push_back(super);
  superClasses.push_back(super->Name());
  superClassDocs.push_back(super->Doc());
  inherritance.push_back(derived);
}
void TMyClass::removeSuperClass(const string &n)
{
  int p=-1;
  for(unsigned int i=0;i<supers.size();i++)
    if(superClasses[i]==n)
      p=i;
  if(p==-1)return;
  removeSuperClass(p);
}
void TMyClass::removeSuperClass(int num)
{
  if(num<0||num>=(int)supers.size())return;
  if(num==(int)supers.size()-1)
    {
      supers.pop_back();
      inherritance.pop_back();
      superClasses.pop_back();
      superClassDocs.pop_back();
      return;
    }
  vector<string>tmp1,tmp2;
  vector<TMyClass*>tmp3;
  vector<int>tmp4;
  while((int)supers.size()>num+1)
    {
      tmp1.push_back(superClasses.back());
      tmp2.push_back(superClassDocs.back());
      tmp3.push_back(supers.back());
      tmp4.push_back(inherritance.back());
      supers.pop_back();
      superClasses.pop_back();
      superClassDocs.pop_back();
      inherritance.pop_back();
    }
  supers.pop_back();
  superClasses.pop_back();
  superClassDocs.pop_back();
  inherritance.pop_back();
  while(!tmp1.empty())
    {
      superClasses.push_back(tmp1.back());
      superClassDocs.push_back(tmp2.back());
      supers.push_back(tmp3.back());
      inherritance.push_back(tmp4.back());
      tmp1.pop_back();
      tmp2.pop_back();
      tmp3.pop_back();
      tmp4.pop_back();
    }
}
void TMyClass::clearVariable(int t)
{
  if(t<0||t>2)return;
  switch(t)
    {
    case 0:
      while(!variablesPublic.empty())variablesPublic.pop_back();
      break;
    case 1:
      while(!variablesProtected.empty())variablesProtected.pop_back();
      break;
    case 2:
      while(!variablesPrivate.empty())variablesPrivate.pop_back();
      break;
    }
}
void TMyClass::addVariable(int t, const nameTypeDoc &var)
{
  if(t<0||t>2)return;
    switch(t)
    {
    case 0:
      variablesPublic.push_back(var);
      variablesPublic.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    case 1:
      variablesProtected.push_back(var);
      variablesProtected.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    case 2:
      variablesPrivate.push_back(var);
      variablesPrivate.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    }
}
void TMyClass::addVariable(int t, string nameIn,string typeIn,int typeIdIn,string specifier,string docIn,string typeDocIn)
{
  if(t<0||t>2)return;
  switch(t)
    {
    case 0:
      variablesPublic.push_back(nameTypeDoc(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn));
      variablesPublic.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    case 1:
      variablesProtected.push_back(nameTypeDoc(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn));
      variablesProtected.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    case 2:
      variablesPrivate.push_back(nameTypeDoc(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn));
      variablesPrivate.back().setDoc(string("#v")+string_number((int)(variablesPublic.size()+variablesProtected.size()+variablesPrivate.size()+1)));
      break;
    }
}
void TMyClass::removeVariable(int t, int i)
{
  if(t<0||t>2)return;
  switch(t)
    {
    case 0:
      {
	if(i<0||i>=(int)variablesPublic.size())return;
	removeFromVector<nameTypeDoc>(variablesPublic,i);
	break;
      }
    case 1:
      {
	if(i<0||i>=(int)variablesProtected.size())return;
	removeFromVector<nameTypeDoc>(variablesProtected,i);
	break;
      }
    case 2:
      {
	if(i<0||i>=(int)variablesPrivate.size())return;
	removeFromVector<nameTypeDoc>(variablesPrivate,i);
	break;
      }
    }
}
void TMyClass::removeVariable(const string &n)
{
  int p=-1;
  for(unsigned int i=0;i<variablesPublic.size();i++)
    if(n==variablesPublic[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<nameTypeDoc>(variablesPublic,p);
      return;
    }
  for(unsigned int i=0;i<variablesProtected.size();i++)
    if(n==variablesProtected[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<nameTypeDoc>(variablesProtected,p);
      return;
    }
  for(unsigned int i=0;i<variablesPrivate.size();i++)
    if(n==variablesPrivate[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<nameTypeDoc>(variablesPrivate,p);
      return;
    }
}
void TMyClass::clearMethod(int t)
{
  if(t<0||t>7)return;
  while(!functionType[t].empty())functionType[t].pop_back();
  switch(t)
    {
    case 0:
      while(!functionsPublic.empty())functionsPublic.pop_back();
      break;
    case 1:
      while(!functionsProtected.empty())functionsProtected.pop_back();
      break;
    case 2:
      while(!functionsPrivate.empty())functionsPrivate.pop_back();
      break;
    case 3:
      while(!slotsPublic.empty())slotsPublic.pop_back();
      break;
    case 4:
      while(!slotsProtected.empty())slotsProtected.pop_back();
      break;
    case 5:
      while(!slotsPrivate.empty())slotsPrivate.pop_back();
      break;
    case 6:
      while(!thesignals.empty())thesignals.pop_back();
      break;
    }
}
void TMyClass::addMethod(int t, const functionNameTypeDoc &var)
{
  if(t<0||t>7)return;
  if(t>2&&t<7&&var.TypeId()!=7)t=t-3;
  if(t>2&&var.TypeId()!=7)t=0;
  functionType[t].push_back(0);
  switch(t)
    {
    case 0:
      functionsPublic.push_back(var);
      functionsPublic.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 1:
      functionsProtected.push_back(var);
      functionsProtected.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 2:
      functionsPrivate.push_back(var);
      functionsPrivate.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 3:
      slotsPublic.push_back(var);
      slotsPublic.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 4:
      slotsProtected.push_back(var);
      slotsProtected.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 5:
      slotsPrivate.push_back(var);
      slotsPrivate.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 6:
      thesignals.push_back(var);
      thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
      break;
    }
}
void TMyClass::addMethod(int t, string nameIn, string typeIn,int typeIdIn,string specifier,string docIn,string typeDocIn)
{
  if(t<0||t>7)return;
  functionNameTypeDoc var(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn);
  if(t>2&&t<7&&var.TypeId()!=7)t=t-3;
  if(t>2&&var.TypeId()!=7)t=0;
  functionType[t].push_back(0);
  switch(t)
    {
    case 0:
      functionsPublic.push_back(var);
      functionsPublic.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 1:
      functionsProtected.push_back(var);
      functionsProtected.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 2:
      functionsPrivate.push_back(var);
      functionsPrivate.back().setDoc(string("#f")+string_number((int)(functionsPublic.size()+functionsProtected.size()+functionsPrivate.size()+1)));
      break;
    case 3:
      slotsPublic.push_back(var);
      slotsPublic.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 4:
      slotsProtected.push_back(var);
      slotsProtected.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 5:
      slotsPrivate.push_back(var);
      slotsPrivate.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 6:
      thesignals.push_back(var);
      thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
      break;
    }
}
int TMyClass::MethodType(int i, int t)const
{
  if(t<0||t>7)return -1;
  if(i<0||i>=(int)functionType[t].size())return -1;
  return functionType[t][i];
}
void TMyClass::removeMethod(int t, int i)
{
  if(t<0||t>7)return;
  switch(t)
    {
    case 0:
      {
	if(i<0||i>=(int)functionsPublic.size())return;
	removeFromVector<functionNameTypeDoc>(functionsPublic,i);
	break;
      }
    case 1:
      {
	if(i<0||i>=(int)functionsProtected.size())return;
	removeFromVector<functionNameTypeDoc>(functionsProtected,i);
	break;
      }
    case 2:
      {
	if(i<0||i>=(int)functionsPrivate.size())return;
	removeFromVector<functionNameTypeDoc>(functionsPrivate,i);
	break;
      }
    case 3:
      {
	if(i<0||i>=(int)slotsPublic.size())return;
	removeFromVector<functionNameTypeDoc>(slotsPublic,i);
	break;
      }
    case 4:
      {
	if(i<0||i>=(int)slotsProtected.size())return;
	removeFromVector<functionNameTypeDoc>(slotsProtected,i);
	break;
      }
    case 5:
      {
	if(i<0||i>=(int)slotsPrivate.size())return;
	removeFromVector<functionNameTypeDoc>(slotsPrivate,i);
	break;
      }
    case 6:
      {
	if(i<0||i>=(int)thesignals.size())return;
	removeFromVector<functionNameTypeDoc>(thesignals,i);
	break;
      }
    }
  removeFromVector<int>(functionType[t],i);
}
void TMyClass::removeMethod(const string &n)
{
  int p=-1;
  for(unsigned int i=0;i<functionsPublic.size();i++)
    if(n==functionsPublic[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<functionNameTypeDoc>(functionsPublic,p);
      removeFromVector<int>(functionType[0],p);
      return;
    }
  for(unsigned int i=0;i<functionsProtected.size();i++)
    if(n==functionsProtected[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[1],p);
      removeFromVector<functionNameTypeDoc>(functionsProtected,p);
      return;
    }
  for(unsigned int i=0;i<functionsPrivate.size();i++)
    if(n==functionsPrivate[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[2],p);
      removeFromVector<functionNameTypeDoc>(functionsPrivate,p);
      return;
    }
  for(unsigned int i=0;i<slotsPublic.size();i++)
    if(n==slotsPublic[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<functionNameTypeDoc>(slotsPublic,p);
      removeFromVector<int>(functionType[3],p);
      return;
    }
  for(unsigned int i=0;i<slotsProtected.size();i++)
    if(n==slotsProtected[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[4],p);
      removeFromVector<functionNameTypeDoc>(slotsProtected,p);
      return;
    }
  for(unsigned int i=0;i<slotsPrivate.size();i++)
    if(n==slotsPrivate[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[5],p);
      removeFromVector<functionNameTypeDoc>(slotsPrivate,p);
      return;
    }
  for(unsigned int i=0;i<thesignals.size();i++)
    if(n==thesignals[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[6],p);
      removeFromVector<functionNameTypeDoc>(thesignals,p);
      return;
    }
}
void TMyClass::clearSlots(int t)
{
  if(t<0||t>4)return;
  while(!functionType[t+3].empty())functionType[t+3].pop_back();
  switch(t)
    {
    case 0:
      while(!slotsPublic.empty())slotsPublic.pop_back();
      break;
    case 1:
      while(!slotsProtected.empty())slotsProtected.pop_back();
      break;
    case 2:
      while(!slotsPrivate.empty())slotsPrivate.pop_back();
      break;
    case 3:
      while(!thesignals.empty())thesignals.pop_back();
      break;
    }
}
void TMyClass::addSlot(int t, const functionNameTypeDoc &var)
{
  if(t<0||t>3)return;
  if(t<=2&&var.TypeId()!=7)
    {
      addMethod(t,var);
      return;
    }
  if(t>2&&var.TypeId()!=7)
    {
      addMethod(0,var);
      return;
    }
  functionType[t+3].push_back(0);
  switch(t)
    {
    case 0:
      slotsPublic.push_back(var);
      slotsPublic.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 1:
      slotsProtected.push_back(var);
      slotsProtected.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 2:
      slotsPrivate.push_back(var);
      slotsPrivate.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 3:
      thesignals.push_back(var);
      thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
      break;
    }
}
void TMyClass::addSlot(int t, string nameIn, string typeIn,int typeIdIn,string specifier,string docIn,string typeDocIn)
{
  if(t<0||t>3)return;
  functionNameTypeDoc var(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn);
  if(t<=2&&var.TypeId()!=7)
    {
      addMethod(t,var);
      return;
    }
  if(t>2&&var.TypeId()!=7)
    {
      addMethod(0,var);
      return;
    }
  functionType[t+3].push_back(0);
  switch(t)
    {
    case 0:
      slotsPublic.push_back(var);
      slotsPublic.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 1:
      slotsProtected.push_back(var);
      slotsProtected.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 2:
      slotsPrivate.push_back(var);
      slotsPrivate.back().setDoc(string("#l")+string_number((int)(slotsPublic.size()+slotsProtected.size()+slotsPrivate.size()+1)));
      break;
    case 3:
      thesignals.push_back(var);
      thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
      break;
    }
}
void TMyClass::removeSlot(int t, int i)
{
  if(t<0||t>4)return;
  switch(t)
    {
    case 0:
      {
	if(i<0||i>=(int)slotsPublic.size())return;
	removeFromVector<functionNameTypeDoc>(slotsPublic,i);
	break;
      }
    case 1:
      {
	if(i<0||i>=(int)slotsProtected.size())return;
	removeFromVector<functionNameTypeDoc>(slotsProtected,i);
	break;
      }
    case 2:
      {
	if(i<0||i>=(int)slotsPrivate.size())return;
	removeFromVector<functionNameTypeDoc>(slotsPrivate,i);
	break;
      }
    case 3:
      {
	if(i<0||i>=(int)thesignals.size())return;
	removeFromVector<functionNameTypeDoc>(thesignals,i);
	break;
      }
    }
  removeFromVector<int>(functionType[t+3],i);
}
void TMyClass::removeSlot(const string &n)
{
  int p=-1;
  for(unsigned int i=0;i<slotsPublic.size();i++)
    if(n==slotsPublic[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<functionNameTypeDoc>(slotsPublic,p);
      removeFromVector<int>(functionType[3],p);
      return;
    }
  for(unsigned int i=0;i<slotsProtected.size();i++)
    if(n==slotsProtected[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[4],p);
      removeFromVector<functionNameTypeDoc>(slotsProtected,p);
      return;
    }
  for(unsigned int i=0;i<slotsPrivate.size();i++)
    if(n==slotsPrivate[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[5],p);
      removeFromVector<functionNameTypeDoc>(slotsPrivate,p);
      return;
    }
  for(unsigned int i=0;i<thesignals.size();i++)
    if(n==thesignals[i].Name())
      p=i;
  if(p!=-1)
    {
      removeFromVector<int>(functionType[6],p);
      removeFromVector<functionNameTypeDoc>(thesignals,p);
      return;
    }
}
void TMyClass::clearSignals()
{
  while(!functionType[6].empty())functionType[6].pop_back();
  while(!thesignals.empty())thesignals.pop_back();
}
void TMyClass::addSignal(const functionNameTypeDoc &var)
{
  thesignals.push_back(var);
  thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
}
void TMyClass::addSignal(string nameIn,string typeIn,int typeIdIn,string specifier,string docIn,string typeDocIn)
{
  thesignals.push_back(functionNameTypeDoc(nameIn,typeIn,typeIdIn,specifier,docIn,typeDocIn));
  thesignals.back().setDoc(string("#s")+string_number((int)(thesignals.size()+1)));
}
void TMyClass::clearConstructor()
{
  while(!constructors.empty())constructors.pop_back();
}
void TMyClass::addConstructor(const functionNameTypeDoc &var)
{
  if(var.Name()!=className)
    {
      addMethod(0,var);
      return;
    }
  constructors.push_back(var);
  constructors.back().setDoc(string("#c")+string_number((int)(constructors.size()+1)));
}
void TMyClass::addConstructor(string typeIn,int typeIdIn,string specifier,string docIn,string typeDocIn)
{
  constructors.push_back(functionNameTypeDoc(className,typeIn,typeIdIn,specifier,docIn,typeDocIn));
  constructors.back().setDoc(string("#c")+string_number((int)(constructors.size()+1)));
}
void TMyClass::removeConstructor(int i)
{
  removeFromVector<functionNameTypeDoc>(constructors,i);
}
void TMyClass::removeConstructor(const string &n)
{
  int p=-1;
  for(unsigned int i=0;i<constructors.size();i++)
    if(constructors[i].Name()==n)
      p=i;
  if(p!=-1)
    removeFromVector<functionNameTypeDoc>(constructors,p);
}
void TMyClass::checkFunctionType()
{
  bool found;
  for(int i=0;i<3;i++)for(unsigned int j=0;j<functionType[i].size();j++)functionType[i][j]=0;
  for(unsigned int k=0;k<functionsPublic.size();k++)
    {
      found=false;
      for(unsigned int i=0;i<supers.size();i++)
	{
	  if(supers[i]==NULL)continue;
	  for(int j=0;j<supers[i]->nMethods(0);j++)
	    {
	      if(functionsPublic[k]==supers[i]->Method(j,0))
		found=true;
	    }
	  for(int j=0;j<supers[i]->nMethods(1);j++)
	    {
	      if(functionsPublic[k]==supers[i]->Method(j,1))
		found=true;
	    }
	}
      cout<<"method \""<<functionsPublic[k].Name().data()<<"\" is "<<(found?"overwritten":"new")<<endl;
      if(found)functionType[0][k]=1;
    }
  for(unsigned int k=0;k<functionsProtected.size();k++)
    {
      found=false;
      for(unsigned int i=0;i<supers.size();i++)
	{
	  if(supers[i]==NULL)continue;
	  for(int j=0;j<supers[i]->nMethods(0);j++)
	    {
	      if(functionsProtected[k]==supers[i]->Method(j,0))
		found=true;
	    }
	  for(int j=0;j<supers[i]->nMethods(1);j++)
	    {
	      if(functionsProtected[k]==supers[i]->Method(j,1))
		found=true;
	    }
	}
      cout<<"method \""<<functionsProtected[k].Name().data()<<"\" is "<<(found?"overwritten":"new")<<endl;
      if(found)functionType[1][k]=1;
    }
  while(functionType.size()>3)functionType.pop_back();
  for(unsigned int i=0;i<supers.size();i++)
    {
      if(supers[i]==NULL)continue;
      vector<int> tmp;
      for(int j=0;j<supers[i]->nMethods(0);j++)
	{
	  found=false;
	  for(unsigned int k=0;k<functionsPublic.size();k++)
	    if(functionsPublic[k]==supers[i]->Method(j,0))
	      found=true;
	  for(unsigned int k=0;k<functionsProtected.size();k++)
	    if(functionsProtected[k]==supers[i]->Method(j,0))
	      found=true;
	  cout<<"method \""<<supers[i]->Name().data()<<"::"<<supers[i]->Method(j,0).Name().data()<<"\" is "<<(found?"overwritten":"inherrited")<<endl;
	  tmp.push_back((found?1:0));
	}
      for(int j=0;j<supers[i]->nMethods(1);j++)
	{
	  found=false;
	  for(unsigned int k=0;k<functionsPublic.size();k++)
	    if(functionsPublic[k]==supers[i]->Method(j,1))
	      found=true;
	  for(unsigned int k=0;k<functionsProtected.size();k++)
	    if(functionsProtected[k]==supers[i]->Method(j,1))
	      found=true;
	  cout<<"method \""<<supers[i]->Name().data()<<"::"<<supers[i]->Method(j,1).Name().data()<<"\" is "<<(found?"overwritten":"inherrited")<<endl;
	  tmp.push_back((found?1:0));
	}
      functionType.push_back(tmp);
    }
}
void TMyClass::addSuperDescriptions()
{
  vector<TMyClass*> allSuperClasses;
  for(unsigned int k=0;k<supers.size();k++)
    {
      if(supers[k]!=NULL)
	allSuperClasses.push_back(supers[k]);
    }
  int pos=0;
  while(pos<(int)allSuperClasses.size())
    {
      for(int i=0;i<allSuperClasses[pos]->nSuperClasses();i++)
	if(allSuperClasses[pos]->Super(i)!=NULL)
	  allSuperClasses.push_back(allSuperClasses[pos]->Super(i));
      pos++;
    }
  for(int j=0;j<7;j++)
    {
      for(unsigned int k=0;k<allSuperClasses.size();k++)
	{
	  if(allSuperClasses[k]==NULL)continue;
	  for(int i=0;i<allSuperClasses[k]->nMethods(j);i++)
	    {
	      if(allSuperClasses[k]->Method(i,j).Name()==string("~")+allSuperClasses[k]->Name())continue;
	      int found=-1;
	      for(int m=0;m<7;m++)
		for(int l=0;l<nMethods(m);l++)
		  if(allSuperClasses[k]->Method(i,j)==getMethod(l,m))
		    found=l*8+m;
	      if(found!=-1)
		{
		  for(int l=0;l<allSuperClasses[k]->Method(i,j).DescriptionLines();l++)
		    Method(found/8,found%8).addDescriptionLine(allSuperClasses[k]->Method(i,j).Description(l));
		}
	    }
	}
    }
}
vector<string> TMyClass::toHTML()const
{
  vector<functionNameTypeDoc> tmp;
  return toHTML(tmp);
}
vector<string> TMyClass::toHTML(const vector<functionNameTypeDoc> & associatedFunctions)const
{
  vector<vector<functionNameTypeDoc> > inherritedOverwritten;
  vector<vector<functionNameTypeDoc> > inherrited;
  vector<vector<functionNameTypeDoc> > newFunctions;
  vector<TMyClass*> allSuperClasses;
  for(unsigned int k=0;k<supers.size();k++)
    {
      if(supers[k]!=NULL)
	allSuperClasses.push_back(supers[k]);
    }
  int pos=0;
  while(pos<(int)allSuperClasses.size())
    {
      for(int i=0;i<allSuperClasses[pos]->nSuperClasses();i++)
	if(allSuperClasses[pos]->Super(i)!=NULL)
	  allSuperClasses.push_back(allSuperClasses[pos]->Super(i));
      pos++;
    }
  for(int j=0;j<7;j++)
    {
      vector<functionNameTypeDoc> tmpO;
      vector<functionNameTypeDoc> tmpI;
      for(unsigned int k=0;k<allSuperClasses.size();k++)
	{
	  if(allSuperClasses[k]==NULL)continue;
	  for(int i=0;i<allSuperClasses[k]->nMethods(j);i++)
	    {
	      if(allSuperClasses[k]->Method(i,j).Name()==string("~")+allSuperClasses[k]->Name())continue;
	      int found=-1;
	      for(int m=0;m<3;m++)
		for(int l=0;l<nMethods(m);l++)
		  if(allSuperClasses[k]->Method(i,j)==getMethod(l,m))
		    found=l*3+m;
	      if(found==-1)
		{
		  tmpI.push_back(allSuperClasses[k]->Method(i,j));
		  tmpI.back().setName(allSuperClasses[k]->Name()+"::"+tmpI.back().Name());
		  tmpI.back().setDoc(allSuperClasses[k]->Doc()+tmpI.back().Doc());
		}
	      else
		{
		  tmpO.push_back(getMethod(found/3,found%3));
		}
	    }
	}
      inherritedOverwritten.push_back(tmpO);
      inherrited.push_back(tmpI);
    }
  for(int j=0;j<7;j++)
    {
      vector<functionNameTypeDoc> tmpN;
      for(int i=0;i<nMethods(j);i++)
	{
	  int found=-1;
	  for(int k=0;k<3;k++)
	    for(unsigned int l=0;l<inherritedOverwritten[k].size();l++)
	      if(inherritedOverwritten[k][l]==getMethod(i,j))
		found=l*3+k;
	  if(found==-1)
	    tmpN.push_back(getMethod(i,j));
	}
      newFunctions.push_back(tmpN);
    }
   vector<vector<int> >ft;
  vector<string> ret;
  ret.push_back("<HTML>");
  ret.push_back("<link rel=\"stylesheet\" type=\"text/css\" href=\"mystylesheet.css\"/>");
  ret.push_back("<HEAD>");
  ret.push_back(string("<TITLE>Class ")+className+"</TITLE>");
  ret.push_back("</HEAD>");
  ret.push_back("<BODY>");// TEXT=\"blue\" LINK=\"black\" VLINK=\"black\">");
  //  ret.push_back("<FONT FACE=\"luxi sans\">");
  if(hasA(headerfile,"/"))
      ret.push_back(string("<H1><A NAME=\"top\">Class ")+className+" in \""+headerfile.substr(headerfile.rfind("/")+1,headerfile.length()-headerfile.rfind("/")-1)+"\"</A></H1>");
  else
      ret.push_back(string("<H1><A NAME=\"top\">Class ")+className+" in \""+headerfile+"\"</A></H1>");
  ret.push_back("<P> <a href=\"\">back</a>");
  ret.push_back("<hr/>");
  for(unsigned int i=0;i<description.size();i++)ret.push_back(description[i]);
  ret.push_back("<DL>");
  if(supers.size()>0)
    {
      if(supers.size()>1)
	ret.push_back("<DT>Superclasses");
      else
	ret.push_back("<DT>Superclass");
      for(unsigned int i=0;i<supers.size();i++)
	{
	  string td=superClassDocs[i];
	  if(td=="")
	    td=knownTypesDoc(superClasses[i]);
	  if(td=="")
	    {
	      TMyClass* tmp=getKnownClass(superClasses[i]);
	      if(tmp!=NULL)
		td=tmp->Doc();
	    }
	  ret.push_back(string("<DD><A HREF=\"")+td+"\">"+superClasses[i]+"</A>");
	}
    }
  if(constructors.size()>0)
    {
      ret.push_back("<DT>Constructor");
      for(unsigned int i=0;i<constructors.size();i++)
      {
	ret.push_back(constructors[i].htmlHREF());//"<DD><A HREF="#c1"> ");
      }
    }
  //  ret.push_back("<DT>Destructor");
  //  ret.push_back("<DD><A HREF="#d1"> ");
  int nF=0;
  for(int i=0;i<2;i++)
    nF+=newFunctions[i].size()+inherrited[i].size()+inherritedOverwritten[i].size();
  if(nF>0)
    {
      ret.push_back("<DT>Functions");
      nF=0;
      if(newFunctions[0].size()>0)
	{
	  ret.push_back("<DD>new public<ul>");
	  for(unsigned int i=0;i<newFunctions[0].size();i++)
	    if(!hasA(newFunctions[0][i].Specifier(),"static"))
	      {nF++;ret.push_back(newFunctions[0][i].htmlHREF());}
	  ret.push_back("</UL>");
	}
      if(newFunctions[0].size()>0&&nF!=(int)newFunctions[0].size())
	{
	  ret.push_back("<DD>static<ul>");
	  for(unsigned int i=0;i<newFunctions[0].size();i++)
	    if(hasA(newFunctions[0][i].Specifier(),"static"))
	      ret.push_back(newFunctions[0][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherritedOverwritten[0].size()>0)
	{
	  ret.push_back("<DD>public overwritten inherrited<UL>");
	  for(unsigned int i=0;i<inherritedOverwritten[0].size();i++)
	    {
	    if(!hasA(inherritedOverwritten[0][i].Specifier(),"static"))
	      ret.push_back(inherritedOverwritten[0][i].htmlHREF());
	    }
	  ret.push_back("</UL>");
	}
      if(inherrited[0].size()>0)
	{
	  ret.push_back("<DD>public inherrited<UL>");
	  for(unsigned int i=0;i<inherrited[0].size();i++)
	    if(!hasA(inherrited[0][i].Specifier(),"static"))
	      ret.push_back(inherrited[0][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(newFunctions[1].size()>0)
	{
	  ret.push_back("<DD>new protected<ul>");
	  for(unsigned int i=0;i<newFunctions[1].size();i++)
	    if(hasA(newFunctions[1][i].Specifier(),"static"))
	      ret.push_back(newFunctions[1][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherritedOverwritten[1].size()>0)
	{
	  ret.push_back("<DD>protected overwritten inherrited<UL>");
	  for(unsigned int i=0;i<inherritedOverwritten[1].size();i++)
	    {
	      if(!hasA(inherritedOverwritten[1][i].Specifier(),"static"))
	      ret.push_back(inherritedOverwritten[1][i].htmlHREF());
	    }
	  ret.push_back("</UL>");
	}
      if(inherrited[1].size()>0)
	{
	  ret.push_back("<DD>protected inherrited<UL>");
	  for(unsigned int i=0;i<inherrited[1].size();i++)
	    if(!hasA(inherrited[1][i].Specifier(),"static"))
	      ret.push_back(inherrited[1][i].htmlHREF());
	  ret.push_back("</UL>");
	}
    }
  nF=0;
  for(int i=0;i<2;i++)
    nF+=newFunctions[i+3].size()+inherrited[i+3].size()+inherritedOverwritten[i+3].size();
  if(nF>0)
    {
      ret.push_back("<DT>Slots");
      nF=0;
      if(newFunctions[3].size()>0)
	{
	  ret.push_back("<DD>new public<ul>");
	  for(unsigned int i=0;i<newFunctions[3].size();i++)
	    if(!hasA(newFunctions[3][i].Specifier(),"static"))
	      {nF++;ret.push_back(newFunctions[3][i].htmlHREF());}
	  ret.push_back("</UL>");
	}
      if(newFunctions[3].size()>0&&nF!=(int)newFunctions[3].size())
	{
	  ret.push_back("<DD>static<ul>");
	  for(unsigned int i=0;i<newFunctions[3].size();i++)
	    if(hasA(newFunctions[3][i].Specifier(),"static"))
	      ret.push_back(newFunctions[3][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherritedOverwritten[3].size()>0)
	{
	  ret.push_back("<DD>public overwritten inherrited<UL>");
	  for(unsigned int i=0;i<inherritedOverwritten[3].size();i++)
	    {
	    if(!hasA(inherritedOverwritten[3][i].Specifier(),"static"))
	      ret.push_back(inherritedOverwritten[3][i].htmlHREF());
	    }
	  ret.push_back("</UL>");
	}
      if(inherrited[3].size()>0)
	{
	  ret.push_back("<DD>public inherrited<UL>");
	  for(unsigned int i=0;i<inherrited[3].size();i++)
	    if(!hasA(inherrited[3][i].Specifier(),"static"))
	      ret.push_back(inherrited[3][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(newFunctions[4].size()>0)
	{
	  ret.push_back("<DD>new protected<ul>");
	  for(unsigned int i=0;i<newFunctions[4].size();i++)
	    if(hasA(newFunctions[4][i].Specifier(),"static"))
	      ret.push_back(newFunctions[4][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherritedOverwritten[4].size()>0)
	{
	  ret.push_back("<DD>protected overwritten inherrited<UL>");
	  for(unsigned int i=0;i<inherritedOverwritten[4].size();i++)
	    {
	      if(!hasA(inherritedOverwritten[4][i].Specifier(),"static"))
		ret.push_back(inherritedOverwritten[4][i].htmlHREF());
	    }
	  ret.push_back("</UL>");
	}
      if(inherrited[4].size()>0)
	{
	  ret.push_back("<DD>protected inherrited<UL>");
	  for(unsigned int i=0;i<inherrited[4].size();i++)
	    if(!hasA(inherrited[4][i].Specifier(),"static"))
	      ret.push_back(inherrited[4][i].htmlHREF());
	  ret.push_back("</UL>");
	}
    }
  nF=newFunctions[6].size()+inherrited[6].size()+inherritedOverwritten[6].size();
  if(nF>0)
    {
      ret.push_back("<DT>Signals");
      if(newFunctions[6].size()>0)
	{
	  ret.push_back("<DD>new public<ul>");
	  for(unsigned int i=0;i<newFunctions[6].size();i++)
	    ret.push_back(newFunctions[6][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherritedOverwritten[6].size()>0)
	{
	  ret.push_back("<DD>public overwritten inherrited<UL>");
	  for(unsigned int i=0;i<inherritedOverwritten[6].size();i++)
	    ret.push_back(inherritedOverwritten[6][i].htmlHREF());
	  ret.push_back("</UL>");
	}
      if(inherrited[6].size()>0)
	{
	  ret.push_back("<DD>public inherrited<UL>");
	  for(unsigned int i=0;i<inherrited[6].size();i++)
	    ret.push_back(inherrited[6][i].htmlHREF());
	  ret.push_back("</UL>");
	}
    }
  if(variablesPublic.size()>0)
    {
      ret.push_back("<DT>Public Variables");
      for(unsigned int i=0;i<variablesPublic.size();i++)
	ret.push_back(string("<DD>")+variablesPublic[i].htmlHREF());//"<DD><A HREF="#v1"></A>");
    }
  if(variablesProtected.size()>0)
    {
      ret.push_back("<DT>Protected Variables");
      for(unsigned int i=0;i<variablesProtected.size();i++)
	ret.push_back(string("<DD>")+variablesProtected[i].htmlHREF());//"<DD><A HREF="#v1"></A>");
    }
  int  nVP=0,nVr=0;
  for(unsigned int i=0;i<allSuperClasses.size();i++)
    if(allSuperClasses[i]!=NULL)
      {
	nVP+=allSuperClasses[i]->nVariables(0);
	nVr+=allSuperClasses[i]->nVariables(1);
      }
  if(nVP>0)
    {
      ret.push_back("<DT>Public inherrited variables");
      for(unsigned int i=0;i<allSuperClasses.size();i++)
	{
	  if(allSuperClasses[i]==NULL)continue;
	  for(int j=0;j<allSuperClasses[i]->nVariables(0);j++)
	    {
	      nameTypeDoc tmp(allSuperClasses[i]->Variable(j,0));
	      tmp.setDoc(allSuperClasses[i]->Doc()+tmp.Doc());
	      tmp.setName(allSuperClasses[i]->Name()+"::"+tmp.Name());
	      ret.push_back(string("<DD>")+tmp.htmlHREF());
	    }
	}
    }
  if(nVr>0)
    {
      ret.push_back("<DT>Protected inherrited variables");
      for(unsigned int i=0;i<allSuperClasses.size();i++)
	{
	  if(allSuperClasses[i]==NULL)continue;
	  for(int j=0;j<allSuperClasses[i]->nVariables(1);j++)
	    {
	      nameTypeDoc tmp(allSuperClasses[i]->Variable(j,1));
	      tmp.setDoc(allSuperClasses[i]->Doc()+tmp.Doc());
	      tmp.setName(allSuperClasses[i]->Name()+"::"+tmp.Name());
	      ret.push_back(string("<DD>")+tmp.htmlHREF());
	    }
	}
    }
  ret.push_back("</DL>");
  if(associatedFunctions.size()>0)
    {
      ret.push_back("Associated functions:");
      ret.push_back("<ul>");
      for(unsigned int i=0;i<associatedFunctions.size();i++)
	{
	  functionNameTypeDoc tmp(associatedFunctions[i]);
	  tmp.setDoc(string("#assf")+string_number((int)(i)));
	  ret.push_back(tmp.htmlHREF());
	}
      ret.push_back("</ul>");
    }
  ret.push_back("<hr/>");
  ret.push_back("<H2>Descriptions</H2>");
  ret.push_back("<UL TYPE=square>");
  for(unsigned int i=0;i<constructors.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=constructors[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<variablesPublic.size();i++)
    {
      vector<string> tmp=variablesPublic[i].htmlNAME();
      ret.push_back("<li>");
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<variablesProtected.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=variablesProtected[i].htmlNAME();
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<functionsPublic.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=functionsPublic[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      if(i==functionsPublic.size()-1&&functionsProtected.size()==0&&slotsPublic.size()==0&&slotsProtected.size()==0&&thesignals.size()==0)
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
      else
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<functionsProtected.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=functionsProtected[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      if(i==functionsProtected.size()-1&&slotsPublic.size()==0&&slotsProtected.size()==0&&thesignals.size()==0)
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
      else
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<slotsPublic.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=slotsPublic[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      if(i==slotsPublic.size()-1&&slotsProtected.size()==0&&thesignals.size()==0)
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
      else
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<slotsProtected.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=slotsProtected[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      if(i==slotsProtected.size()-1&&thesignals.size()==0)
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
      else
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  for(unsigned int i=0;i<thesignals.size();i++)
    {
      ret.push_back("<li>");
      vector<string> tmp=thesignals[i].htmlNAME(className);
      for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
      if(i==thesignals.size()-1)
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
      else
	ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
    }
  if(associatedFunctions.size()>0)
    {
      ret.push_back("<hr/>");
      for(unsigned int i=0;i<associatedFunctions.size();i++)
	{
	  functionNameTypeDoc tmp1(associatedFunctions[i]);
	  tmp1.setDoc(string("#assf")+string_number((int)(i)));
	  vector<string> tmp=tmp1.htmlNAME();
	  for(unsigned int j=0;j<tmp.size();j++)ret.push_back(tmp[j]);
	  if(i==associatedFunctions.size()-1)
	    ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop>");
	  else
	    ret.push_back("<BR/><ttop><a href=\"#top\">top</a></ttop><hr/>");
	}
    }
  ret.push_back("</UL>");
  ret.push_back("<hr><table width=\"30%\"><tr><td>");
  ret.push_back("<a rel=\"license\" href=\"http://creativecommons.org/licenses/by-nc-sa/3.0/\"><img");
  ret.push_back("alt=\"Creative Commons License\" style=\"border-width:0\"");
  ret.push_back("src=\"http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png\" /></a><br />This");
  ret.push_back("work is licensed under a <a rel=\"license\"");
  ret.push_back("href=\"http://creativecommons.org/licenses/by-nc-sa/3.0/\">cc by-nc-sa 3.0");
  ret.push_back("Unported License</a> unless marked otherwise.</td></tr></table>");
  ret.push_back("</body>\n</html>");
  return ret;
}
template<class X> void TMyClass::removeFromVector(vector<X>&v,int pos)
{
  if(pos<0||pos>=(int)v.size())return;
  if(pos==(int)v.size()-1)
    {
      v.pop_back();
      return;
    }
  vector<X>tmp;
  while((int)v.size()>pos+1)
    {
      tmp.push_back(v.back());
      v.pop_back();
    }
  v.pop_back();
  while(!tmp.empty())
    {
      v.push_back(tmp.back());
      tmp.pop_back();
    }
}
string smallerPath(const string& path)
{
  string s=path;
  if(hasA(s,getenv("KTOFPACK")))
    s=replace(s,getenv("KTOFPACK"),"$(KTOFPACK)");
  if(hasA(s,getenv("HOME")))
    s=replace(s,getenv("HOME"),"$(HOME)");
  return s;
}
string expandPath(const string& path)
{
  string s=path;
  string s1,s2;
  while(s.find("$(")<s.npos&&s.find("$(")<s.find(")"))
    {
      s1=s.substr(0,s.find("$("));
      s=s.substr(s.find("$(")+2,s.length()-s.find("$(")-2);
      s2=s.substr(0,s.find(")"));
      s=s.substr(s.find(")")+1,s.length()-s.find(")")-1);
      s=s1+getenv(s2.data())+s;
    }
  return s;
}
ostream &operator << (ostream & o, const TMyClass & t)
{
  o<<t.Name().data()<<endl;
  o<<smallerPath(t.Doc()).data()<<endl;
  o<<smallerPath(t.HeaderFile()).data()<<endl;
  char varOpt[3]={'V','v','g'};
  char methOpt[7]={'F','f','g','L','l','n','N'};
  for(int i=0;i<t.nConstructors();i++)
      o<<"C"<<t.getConstructor(i)<<flush;
  for(int j=0;j<3;j++)
    for(int i=0;i<t.nVariables(j);i++)
      o<<varOpt[j]<<t.getVariable(i,j)<<flush;
//   for(int i=0;i<t.nVariables(1);i++)
//     o<<"v"<<t.getVariable(i,1)<<flush;
//   for(int i=0;i<t.nVariables(2);i++)
//     o<<"g"<<t.getVariable(i,2)<<flush;
  for(int j=0;j<7;j++)
    for(int i=0;i<t.nMethods(j);i++)
      o<<methOpt[j]<<t.getMethod(i,j)<<flush;
//   for(int i=0;i<t.nMethods(1);i++)
//     o<<"f"<<t.getMethod(i,1)<<flush;
//   for(int i=0;i<t.nMethods(2);i++)
//     o<<"G"<<t.getMethod(i,2)<<flush;
//   for(int i=0;i<t.nSlots(0);i++)
//     o<<"L"<<t.getSlot(i,0)<<flush;
//   for(int i=0;i<t.nSlots(1);i++)
//     o<<"l"<<t.getSlot(i,1)<<flush;
//   for(int i=0;i<t.nSlots(2);i++)
//     o<<"n"<<t.getSlot(i,2)<<flush;
//   for(int i=0;i<t.nSignals();i++)
//     o<<"N"<<t.getSignal(i)<<flush;
  for(int i=0;i<t.DescriptionLines();i++)
    o<<"D"<<t.Description(i).data()<<endl;
  for(int i=0;i<t.nSuperClasses();i++)
    {
      o<<"S"<<t.InherritanceType(i)<<" "<<(t.Super(i)==NULL?0:1)<<" "<<t.SuperClass(i).data()<<endl<<smallerPath(t.SuperClassDoc(i).data())<<endl;
      if(t.Super(i)!=NULL)
	o<<t.SuperClass(i).data()<<endl;
    }
  o<<"="<<endl;
  return o;
}
istream &operator >> (istream & o, TMyClass & t)
{
  char li[1000],c;
  int n,m;
  if(o.eof())return o;
  string l1,l2;
  o.getline(li,1000);
  if(o.eof()){return o;}
  t.setName(li);
  o.getline(li,1000);
  if(o.eof()){return o;}
  t.setDoc(expandPath(li));
  o.getline(li,1000);
  if(o.eof()){return o;}
  t.setHeaderFile(expandPath(li));
  o.get(c);
  if(o.eof()){return o;}
  int ty;
  while(o.good()&&c!='=')
    {
      functionNameTypeDoc tmpf;
      nameTypeDoc tmpn;
      ty=0;
      if(c=='C'||c=='F'||c=='f'||c=='G'||c=='L'||c=='l'||c=='n'||c=='N')
	o>>tmpf;
      else if(c=='V'||c=='v'||c=='g')
	o>>tmpn;
      switch(c)
	{
	case 'C':
	  t.addConstructor(tmpf);
	  break;
	case 'N':ty++;
	case 'n':ty++;
	case 'l':ty++;
	case 'L':ty++;
	case 'G':ty++;
	case 'f':ty++;
	case 'F':
	  t.addMethod(ty,tmpf);
	  break;
	case 'g':ty++;
	case 'v':ty++;
	case 'V':
	  t.addVariable(ty,tmpn);
	  break;
	case 'D':
	  {
	    o.getline(li,1000);
	    t.addDescriptionLine(li);
	    break;
	  }
	case 'S':
	  {
	    o>>n>>m;
	    o.get(c);
	    o.getline(li,1000);
	    l1=li;
	    o.getline(li,1000);
	    l2=li;
	    if(m!=0)o.getline(li,1000);
	    TMyClass *tmp=NULL;
	    if(m!=0)tmp=getKnownClass(li);
	    else tmp=getKnownClass(l1);
	    if(tmp==NULL&&l2=="")
	      {
		string s=knownTypesDoc(l1);
		t.addSuperClass(l1,s,n,tmp);
	      }
	    else
	      t.addSuperClass(l1,l2,n,tmp);
	    break;
	  }
	}
      o.get(c);
    }
  addToKnownClasses(t);
  return o;
}
bool TMyClass::readFromFile(int filetype, const string &filename, const string &nameIn)
{
    if(filetype==0)
      {
	if(!existing(filename))return false;
	ifstream input;
	input.open(filename.data());
	while(!input.eof()&&input.good())
	  {
	    TMyClass tmp;
	    input>>tmp;
	    if(nameIn=="")
	      {
		if(tmp.Name()!="")
		  {
		    *this=tmp;
		    return true;
		  }
	      }
	    else if(tmp.Name()==nameIn)
	      {
		*this=tmp;
		return true;
	      }
	  }
	return false;
      }
    vector<string>tmp;
    char li[1000];
    ifstream in;
    in.open(filename.data());
    while(in.good()&&!in.eof())
    {
	in.getline(li,1000);
	tmp.push_back(li);
    }
    return readFromData(filetype,tmp,nameIn);
}
bool TMyClass::readFromData(int datatype, const vector<string> &data, const string &nameIn)
{
    vector<TMyClass> tmp;
    if(TMyClass::content(datatype,data,tmp)<=0)return false;
    if(nameIn=="")
    {
	(*this)=tmp[0];
	return true;
    }
    for(unsigned int i=0;i<tmp.size();i++)
    {
	if(tmp[i].Name()==nameIn)
	{
	    (*this)=tmp[i];
	    return true;
	}
    }
    return false;
}
int TMyClass::fileContent(int fileType, const string &filename, vector<TMyClass> &content)
{
    if(fileType==0)
      {
	if(!existing(filename))return 0;
	ifstream input;
	input.open(filename.data());
	while(!input.eof()&&input.good())
	  {
	    TMyClass tmp;
	    input>>tmp;
	    if(tmp.Name()!="")
	      content.push_back(tmp);
	  }
	return content.size();
      }
    vector<string>tmp;
    char li[1000];
    ifstream in;
    in.open(filename.data());
    while(in.good()&&!in.eof())
    {
	in.getline(li,1000);
	tmp.push_back(li);
    }
    return TMyClass::content(fileType,tmp,content);
}
extern void addKnownFunction(const functionNameTypeDoc &func);

class datastreamer
{
private:
  vector<string> fdata;
  int line;
  int col;
public:
  datastreamer(const vector<string> &dataIn){for(unsigned int i=0;i<dataIn.size();i++)fdata.push_back(dataIn[i]);line=0;col=0;}
  bool eof(){if(line>=(int)fdata.size())return true;
  return (line==(int)fdata.size()-1&&eol());}
  bool eol(){return (col>=(int)fdata[line].length());}
  char get(){
    if(eof())return '\0';
    if(eol()){line++;col=0;return '\n';}
    return fdata[line][col++];}
  string getline()
  {
    if(eof())return "";
    while(!eof()&&fdata[line][col]=='#'&&col==0)line++;
    string ret="";
    char c=get();
    while(c!='\n'&&!eof()){ret+=c;c=get();}
    if(hasA(ret,"//"))ret=ret.substr(0,ret.find("//"));
    while(contains(ret,"/*")!=contains(ret,"*/")&&!eof())
      {c=get();while(c!='\n'&&!eof()){ret+=c;c=get();}}
    while(hasA(ret,"/*")&&hasA(ret,"/*"))
      ret=ret.substr(ret.find("/*"),ret.find("*/")-ret.find("/*"));
    return ret;
  }
};

// string eliminateCommentPreproc(string dta,string *comment=NULL);
// string eliminateCommentPreproc(datastreamer &dta,string *comment=NULL);

string eliminateCommentPreproc(datastreamer &dta,string *comment)
{
  return dta.getline();
}
string eliminateCommentPreproc(string dta,string *comment)
{
  string s=dta;
  string s1;
  if(comment!=NULL)(*comment)="";
  while(hasA(s,"//"))
  {
    s1=s.substr(0,s.find("//"));
    s=s.substr(s.find("//")+2,s.length()-s.find("//")-2);
    if(comment!=NULL)*comment=(*comment)+s.substr(0,s.find('\n')+1);
    s=s.substr(s.find('\n'),s.length()-s.find('\n'));
    s=s1+s;
  }
  while(hasA(s,"#"))
  {
    s1=s.substr(0,s.find("#"));
    s=s.substr(s.find("#")+1,s.length()-s.find("#")-1);
    s=s.substr(s.find('\n'),s.length()-s.find('\n'));
    s=s1+s;
  }
  while(hasA(s,"/*")&&hasA(s,"*/"))s=s.substr(s.find("/*"),s.find("*/")-s.find("/*"));
  return s;
}
int TMyClass::content(int dataType, const vector<string> &data, vector<TMyClass> &contens)
{
  //  cerr<<"TMyClass::content()"<<endl;
    if(dataType!=0&&dataType!=1&&dataType!=2)return 0;
    if(dataType==0)
      {
// 	if(!existing(s))return;
// 	ifstream input;
// 	input.open(s.data());
// 	while(!input.eof()&&input.good())
// 	  {
// 	    TMyClass tmp;
// 	    input>>tmp;
// 	    if(tmp.Name()!="")
// 	      {
// 		classes.push_back(tmp);
// 		files.push_back(s);
// 		comboBox2->insertItem(tmp.Name());
// 	      }
// 	  }
// 	input.close();
      }
    int visibility=2;
    int ftype=0;
    int /*brackets=0,*/curvedBrackets=0;
    int i=0;
    int nfound=0;
    string tmp,tmp1,classNameString,superString,propertyString;
    string comment;
    datastreamer streamer(data);
    char c;
    bool finished=false;
    //    cout<<"start"<<endl;
    while(!streamer.eof())
      {
	tmp=streamer.getline();//eliminateCommentPreproc(streamer,NULL);
	if(streamer.eof()||tmp=="")continue;
	while(!(hasA(tmp,"class")||streamer.eof()))
	  {
	    tmp=streamer.getline();//eliminateCommentPreproc(streamer,NULL);
// 	    if(hasA(tmp,"template<"))
// 	      {		
// 		tmp1=tmp.substr(0,tmp.find("template<"));
// 		tmp=tmp.substr(tmp.find("template<"),tmp.length()-tmp.find("template<"));
// 		tmp=tmp.substr(tmp.find(">")+1,tmp.length()-tmp.find(">")-1);
// 		tmp=tmp1+tmp;
// 	      }
// 	    if(hasA(tmp,"{"))brck+=contains(tmp,"{");
// 	    if(hasA(tmp,"}"))brck-=contains(tmp,"}");
// 	    if(hasA(tmp,"(")&&hasA(tmp,")")&&hasA(tmp,";"))
// 	      {
// 		if(hasA(tmp,"{"))brck+=contains(tmp,"{");
// 		if(hasA(tmp,"}"))brck-=contains(tmp,"}");
// 		if(hasA(tmp,"{"))
// 		  {
// 		    if(contains(tmp,"{")==contains(tmp,"}"))
// 		      tmp=tmp.substr(0,tmp.find("{"))+tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
// 		    else if(contains(tmp,"{")<contains(tmp,"}"))
// 		      tmp=tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
// 		    else
// 		      tmp=tmp.substr(0,tmp.find("{"));
// 		  }
// 		else if(hasA(tmp,"}"))
// 		  tmp=tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
// 		if(brck==0)
// 		  while(hasA(tmp,";"))
// 		    {
// 		      functionNameTypeDoc tmpf(tmp.substr(0,tmp.find(";")+1));
// 		      tmp=tmp.substr(tmp.find(";")+1,tmp.length()-tmp.find(";")-1);
// 		      cout<<tmpf.Name().data()<<endl;
// 		      if(!hasA(tmpf.Name(),"::"))
// 			addKnownFunction(tmpf);
// 		    }
// 	      }
	  }//while(!hasA(tmp,"class")&&!streamer.eof())
	if(streamer.eof())continue;
	while(!(hasA(tmp,'{')//class definition
		||(hasA(tmp,';')&&!hasA(tmp,'{'))//forward declaration
		||(hasA(tmp,"<")&&hasA(tmp,">")&&tmp.find("<")<tmp.find("class")&&tmp.find(">")>tmp.find("class"))//template definition
		||streamer.eof()))
	  {tmp=tmp+streamer.get();}
	//	  cout<<tmp.data()<<endl;}
	if(!hasA(tmp,"{")&&!hasA(tmp,"}")&&hasA(tmp,";"))//forward declaration
	  continue;
	if((hasA(tmp,"<")&&hasA(tmp,">")&&tmp.find("<")<tmp.find("class")&&tmp.find(">")>tmp.find("class")))//template definition
	  continue;
	tmp1=tmp.substr(0,tmp.find("class"));
	tmp=tmp.substr(tmp.find("class"),tmp.length()-tmp.find("class"));
	while(!streamer.eof()&&!hasA(tmp,"{"))tmp=tmp+streamer.get();
	if(streamer.eof())continue;
	tmp=eliminateCommentPreproc(tmp,NULL);
	classNameString=tmp.substr(tmp.find("class")+6,tmp.find("{")-tmp.find("class")-6);
	minimizeString(classNameString);
	superString="";
	finished=false;
	if(hasA(classNameString,":"))
	{
	    superString=classNameString.substr(classNameString.find(":")+1,classNameString.length()-classNameString.find(":")-1);
	    minimizeString(superString);
	    classNameString=classNameString.substr(0,classNameString.find(":"));
	    minimizeString(classNameString);
	}
// 	cout<<"define class:"<<classNameString.data()<<endl;
	TMyClass newClass(classNameString);
	if(tmp1!="") newClass.addDescriptionLine(tmp1);
	addSuperFromString(newClass,superString);
	visibility=2;
	ftype=0;
	tmp="";
	c=streamer.get();while(c=='\n'){c=streamer.get();}
	tmp=c;
	while(!finished&&!streamer.eof())
	  {
	    while(!(hasA(tmp,'{')//inline functions
		    ||(hasA(tmp,';')&&!hasA(tmp,'{'))//non inline functions
		    ||hasA(tmp,"Q_OBJECT")||hasA(tmp,"ClassDef(")//makros
		    ||(hasA(tmp,':')&&!hasA(tmp,"("))//visibility change
		    ||contains(tmp,'}')>contains(tmp,'{')//end of class
		    ||streamer.eof()))
	      {tmp=tmp+streamer.get();}
	    tmp=eliminateCommentPreproc(tmp,NULL);
	    minimizeString(tmp);
	    if(hasA(tmp,"Q_OBJECT"))//Q_OBJECT is a makro for objects derived from QObject. it provides additional functions
	      {
		remove(tmp,"Q_OBJECT");
		for(unsigned int j=0;j<qobjectFunctions.size();j++)
		  newClass.addMethod(0,qobjectFunctions[j]);
		tmp="";
	      }
	    if(hasA(tmp,"ClassDef"))//TObject is a makro for objects derived from TObject. it provides additional functions
	      {
		tmp1=tmp.substr(tmp.find("ClassDef"),tmp.length()-tmp.find("ClassDef"));
		tmp=tmp.substr(0,tmp.find("ClassDef"));
		tmp=tmp+tmp1.substr(tmp1.find(")")+1,tmp1.length()-tmp1.find(")")-1);
		tmp="";
	      }
	    if(hasA(tmp,':')&&!hasA(tmp,"("))//visibility change
	      {
		//cout<<"visibility change :"<<tmp.data()<<endl;
		ftype=0;
		tmp1=tmp.substr(0,tmp.find(":"));
		if(hasA(tmp1,"public"))visibility=0;
		if(hasA(tmp1,"protected"))visibility=1;
		if(hasA(tmp1,"private"))visibility=2;
		if(hasA(tmp1,"slots"))ftype=1;
		if(hasA(tmp1,"signal"))ftype=2;
		tmp=remove(remove(remove(remove(remove(tmp,"public"),"protected"),"private"),"slots"),"signal");
		tmp=tmp.substr(0,tmp.length()-1);
		minimizeString(tmp);
		tmp="";
	      }
	    if(hasA(tmp,'{'))//inline functions
	      {
		//cout<<"inline function :"<<tmp.data()<<"\""<<endl;
		while(contains(tmp,'{')!=contains(tmp,'}')&&!streamer.eof())
		  {c=streamer.get();while(c!='\n'&&!streamer.eof()){tmp=tmp+c;c=streamer.get();}}
		functionNameTypeDoc tmpF(tmp.substr(0,tmp.find("{")-1));
		tmpF.addDescriptionLine(tmp.substr(tmp.find("{"),tmp.find("}")-tmp.find("{")+1));
		if(tmpF.Name()==newClass.Name())
		  newClass.addConstructor(tmpF);
		else if(tmpF.Name()==string("~")+newClass.Name())
		  tmpF.addDescriptionLine("Destructor");
		else
		  {
		    if(tmpF.TypeId()!=7)
			newClass.addMethod(visibility,tmpF);
		    else 
		      switch(ftype)
			{
			case 0:
			  newClass.addMethod(visibility,tmpF);
			  break;
			case 1:
			  newClass.addSlot(visibility,tmpF);
			  break;
			case 2:
			  newClass.addSignal(tmpF);
			  break;
			} 
		  }
		tmp="";
	      }
	    if(contains(tmp,'}')>contains(tmp,'{'))//end of class
	      {
		finished=true;
		//cout<<"end of class"<<endl;
		tmp="";
	      }
	    if(hasA(tmp,';')&&!hasA(tmp,'{'))//non inline functions or variables
	      {
		if(hasA(tmp,'('))//function
		  {
		    //cout<<"function :"<<tmp.data()<<"\""<<endl;
		    functionNameTypeDoc tmpF(tmp);
		    if(tmpF.Name()==newClass.Name())
		      newClass.addConstructor(tmpF);
		    else if(tmpF.Name()==string("~")+newClass.Name())
		      tmpF.addDescriptionLine("Destructor");
		    else
		      {
			if(tmpF.TypeId()!=7)
			    newClass.addMethod(visibility,tmpF);
			else 
			  switch(ftype)
			    {
			    case 0:
			      newClass.addMethod(visibility,tmpF);
			      break;
			    case 1:
			      newClass.addSlot(visibility,tmpF);
			      break;
			    case 2:
			      newClass.addSignal(tmpF);
			      break;
			    } 
		      }
		    tmp="";
		  }
		else //variable
		  {
		    if(hasA(tmp,','))
		      {
			//cout<<"variables :"<<tmp.data()<<"\""<<endl;
			nameTypeDoc tmpn(tmp.substr(0,tmp.find(",")));
			newClass.addVariable(visibility,tmpn);
			while(hasA(tmp,","))
			  {
			    tmp=tmpn.Type()+" "+tmp.substr(tmp.find(","),tmp.length()-tmp.find(","));
			    nameTypeDoc tmpn1(tmp.substr(0,tmp.find(",")));
			    tmpn=tmpn1;
			    newClass.addVariable(visibility,tmpn);
			  }
		      }
		    else
		      {
			// cout<<"variable :"<<tmp.data()<<"\""<<endl;
			nameTypeDoc tmpn(tmp);
			newClass.addVariable(visibility,tmpn);
		      }
		    tmp="";
		  }
	      }
	  }//search for class properties
	if(finished)
	  {
	    c=streamer.get();while(c!=';'&&!streamer.eof())c=streamer.get();
	    newClass.addSuperDescriptions();
	    contens.push_back(newClass);
	    addToKnownClasses(newClass);
	    nfound++;
	  }
      }//////////////////////////////////////////////////
    return nfound;
    while(i<(int)data.size())
    {
      tmp=data[i];
      if(tmp[0]=='#'){i++;continue;}//preprocessor command
      if(hasA(tmp,"//"))
	tmp=tmp.substr(0,tmp.find("//"));
      while(hasA(tmp,"/*")&&(int)data.size()>i+1)
	{
	  if(hasA(tmp,"*/"))
	    {
	      string s=tmp.substr(0,tmp.find("/*"));
	      tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
	      tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
	    }
	  else
	    {
	      while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
	    }
	}
      if(hasA(tmp,"//"))
	tmp=tmp.substr(0,tmp.find("//"));
      
      int brck=0;
      while(!hasA(tmp,"class ")&&(int)data.size()>i+1)
	  {
	    i++;
	    tmp=data[i];
	    if(hasA(tmp,"template<"))
	      {
		tmp1=tmp.substr(0,tmp.find("template<"));
		tmp=tmp.substr(tmp.find("template<"),tmp.length()-tmp.find("template<"));
		tmp=tmp.substr(tmp.find(">")+1,tmp.length()-tmp.find(">")-1);
		tmp=tmp1+tmp;
	      }
	    if(tmp[0]=='#')continue;
	    if(hasA(tmp,"//"))
	      tmp=tmp.substr(0,tmp.find("//"));
	    while(hasA(tmp,"/*")&&(int)data.size()>i+1)
	      {
		if(hasA(tmp,"*/"))
		  {
		    string s=tmp.substr(0,tmp.find("/*"));
		    tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
		    tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
		  }
		else
		  {
		    while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
		  }
	      }
	    if(hasA(tmp,"//"))
	      tmp=tmp.substr(0,tmp.find("//"));
	    if(hasA(tmp,"{"))brck+=contains(tmp,"{");
	    if(hasA(tmp,"}"))brck-=contains(tmp,"}");
	    if(hasA(tmp,"(")&&hasA(tmp,")")&&hasA(tmp,";"))
	      {
		if(hasA(tmp,"{"))brck+=contains(tmp,"{");
		if(hasA(tmp,"}"))brck-=contains(tmp,"}");
		if(hasA(tmp,"{"))
		  {
		    if(contains(tmp,"{")==contains(tmp,"}"))
		      tmp=tmp.substr(0,tmp.find("{"))+tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
		    else if(contains(tmp,"{")<contains(tmp,"}"))
		      tmp=tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
		    else
		      tmp=tmp.substr(0,tmp.find("{"));
		  }
		else if(hasA(tmp,"}"))
		  tmp=tmp.substr(tmp.rfind("}")+1,tmp.length()-tmp.rfind("}")-1);
		if(brck==0)
		  while(hasA(tmp,";"))
		    {
		      functionNameTypeDoc tmpf(tmp.substr(0,tmp.find(";")+1));
		      tmp=tmp.substr(tmp.find(";")+1,tmp.length()-tmp.find(";")-1);
		      cout<<tmpf.Name().data()<<endl;
		      if(!hasA(tmpf.Name(),"::"))
			addKnownFunction(tmpf);
		    }
	      }
	  }
	if(i>=(int)data.size()||!hasA(tmp,"class"))return nfound;
	//	cout<<i<<"|"<<data.size()<<" class: "<<tmp.data()<<endl;
	if(tmp.find("class")>0&&!isSpace(tmp[tmp.find("class")-1]))
	  {
	    if(!(tmp[tmp.find("class")-1]=='>'&&tmp.find("template<")<tmp.find("class")))
	      {
		//		cout<<"nv:"<<tmp.data()<<endl;
		i++;
		continue;
	      }
	    //	    cout<<"vv:"<<tmp.data()<<endl;
	  }
	if(hasA(tmp,"{")==0&&hasA(tmp,"}")==0&&hasA(tmp,";"))//forward declaration
	{i++;continue;}

	classNameString=tmp.substr(tmp.find("class")+6,tmp.length()-tmp.find("class")-6);
	minimizeString(classNameString);
	superString="";
	if(hasA(classNameString,":"))
	{
	    superString=classNameString.substr(classNameString.find(":")+1,classNameString.length()-classNameString.find(":")-1);
	    minimizeString(superString);
	    classNameString=classNameString.substr(0,classNameString.find(":"));
	    minimizeString(classNameString);
	}
	TMyClass newClass(classNameString);
	if(hasA(superString,","))
	{
	    while(superString.rfind(",")==superString.length()-1&&curvedBrackets<=0&&(int)data.size()>i+1)
	    {
		i++;
		tmp=data[i];
		if(tmp[0]=='#')continue;
		if(hasA(tmp,"//"))
		  tmp=tmp.substr(0,tmp.find("//"));
		while(hasA(tmp,"/*")&&(int)data.size()>i+1)
		  {
		    if(hasA(tmp,"*/"))
		      {
			string s=tmp.substr(0,tmp.find("/*"));
			tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
			tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
		      }
		    else
		      {
			while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
		      }
		  }
		if(hasA(tmp,"//"))
		  tmp=tmp.substr(0,tmp.find("//"));
		if(hasA(tmp,"{"))
		{
//		    curvedBrackets++;
		    superString=superString+tmp.substr(0,tmp.find("{"));
		    tmp=tmp.substr(tmp.find("{"),tmp.length()-tmp.find("{"));
		}
		else
		    superString=superString+tmp;
		minimizeString(superString);
	    }
	    if(i>=(int)data.size())return nfound;
	}
	cout<<"class name: "<<classNameString.data()<<endl;
	//	cout<<"superClasses: "<<superString.data()<<endl;
	if(superString!="")
	    addSuperFromString(newClass, superString);
	for(int k=0;k<newClass.nSuperClasses();k++)
	  cout<<" "<<newClass.SuperClass(k).data()<<endl;
	visibility=2;
	ftype=0;
	while(!hasA(tmp,"{")&&(int)data.size()>i+1)
	  { 
	    i++;
	    tmp=data[i];
	    if(tmp[0]=='#')continue;
	    if(hasA(tmp,"//"))
	      tmp=tmp.substr(0,tmp.find("//"));
	    while(hasA(tmp,"/*")&&(int)data.size()>i+1)
	      {
		if(hasA(tmp,"*/"))
		  {
		    string s=tmp.substr(0,tmp.find("/*"));
		    tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
		    tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
		  }
		else
		  {
		    while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
		  }
	      }
	    if(hasA(tmp,"//"))
	      tmp=tmp.substr(0,tmp.find("//"));
	  }
	if(i>=(int)data.size())return nfound;
	curvedBrackets=hasA(tmp,"{");
	tmp=tmp.substr(tmp.find("{")+1,tmp.length()-tmp.find("{")-1);
	minimizeString(tmp);
	while(tmp==""&&(int)data.size()>i+1)
	{
	  i++;tmp=data[i];
	  if(tmp[0]=='#')continue;
	  if(hasA(tmp,"//"))
	    tmp=tmp.substr(0,tmp.find("//"));
	  while(hasA(tmp,"/*")&&(int)data.size()>i+1)
	    {
	      if(hasA(tmp,"*/"))
		{
		  string s=tmp.substr(0,tmp.find("/*"));
		  tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
		  tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
		}
		else
		  {
		    while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
		  }
	    }
	  if(hasA(tmp,"//"))
	    tmp=tmp.substr(0,tmp.find("//"));
	  minimizeString(tmp);}
	if(i>=(int)data.size())return nfound;
	//	cout<<tmp.data()<<endl;
	bool finished=false;
	while(!finished && i<(int)data.size())
	  {
	    comment="";
	    if(hasA(tmp,"//"))
	      {
		comment=tmp.substr(tmp.find("//"),tmp.length()-tmp.find("//"));
		tmp=tmp.substr(0,tmp.find("//"));
	      }
	    if(hasA(tmp,"Q_OBJECT"))//Q_OBJECT is a makro for objects derived from QObject. it provides additional functions
	      {
		remove(tmp,"Q_OBJECT");
		for(unsigned int j=0;j<qobjectFunctions.size();j++)
		  newClass.addMethod(0,qobjectFunctions[j]);
	      }
	    if(hasA(tmp,"ClassDef"))//TObject is a makro for objects derived from TObject. it provides additional functions
	      {
		tmp1=tmp.substr(tmp.find("ClassDef"),tmp.length()-tmp.find("ClassDef"));
		tmp=tmp.substr(0,tmp.find("ClassDef"));
		tmp=tmp+tmp1.substr(tmp1.find(")")+1,tmp1.length()-tmp1.find(")")-1);
	      }
	    if(hasA(tmp,";"))
	      {
		propertyString=tmp.substr(0,tmp.find(";"));
		if(hasA(propertyString,":")&&!hasA(propertyString,"::"))
		  {
		    ftype=0;
		    tmp1=propertyString.substr(0,propertyString.find(":"));
		    if(hasA(tmp1,"public"))visibility=0;
		    if(hasA(tmp1,"protected"))visibility=1;
		    if(hasA(tmp1,"private"))visibility=2;
		    if(hasA(tmp1,"slots"))ftype=1;
		    if(hasA(tmp1,"signal"))ftype=2;
		    propertyString=propertyString.substr(propertyString.find(":")+1,propertyString.length()-propertyString.find(":")-1);
		  }
		if(hasA(propertyString,",")||(hasA(propertyString,"(")&&hasA(propertyString,")")))
		  {
		    if(hasA(propertyString,","))//more than one variable of that type or function
		      {
			if(hasA(propertyString,"(")==0)//more than one variable
			  {
			    nameTypeDoc tmpN(propertyString.substr(0,propertyString.find(",")));
			    newClass.addVariable(visibility,tmpN);
			    tmp1=propertyString;
			    string cn,def;
			    while(hasA(tmp1,","))
			      {
				cn="";def="";
				tmp1=tmp1.substr(tmp1.find(",")+1,tmp1.length()-tmp1.find(",")-1);
				tmp=tmp1.substr(0,tmp1.find(","));
				if(hasA(tmp,"="))
				  {
				    def=tmp.substr(tmp.find("=")+1,tmp.length()-tmp.find("=")-1);
				    tmp=tmp.substr(0,tmp.find("="));
				  }
				for(int i=0;i<hasA(tmp,"*");i++)cn+="*";
				for(int i=0;i<hasA(tmp,"&");i++)cn+="&";
				for(int i=0;i<hasA(tmp,"[");i++)cn+="*";
				remove(tmp,"*");
				remove(tmp,"&");
				remove(tmp,"[");
				remove(tmp,"]");
				nameTypeDoc tmpNN(tmp,tmpN.Type()+cn,tmpN.TypeId(),tmpN.Specifier(),tmpN.Doc(),tmpN.TypeDoc());
				tmpNN.setDefault(def);
				newClass.addVariable(visibility,tmpNN);
			      }
			  }
			else
			  {
			    functionNameTypeDoc tmpF(propertyString);
			    tmpF.addDescriptionLine(comment);
			    if(tmpF.Name()==newClass.Name())
			      {
				newClass.addConstructor(tmpF);
			      }
			    else
			      {
				if(tmpF.Name()==string("~")+newClass.Name())
				  tmpF.addDescriptionLine("Destructor");
				if(tmpF.TypeId()!=7)
				  {
				    //cout<<"function found "<<tmpF.Name().data()<<endl;
				      newClass.addMethod(visibility,tmpF);
				  }
				else 
				  switch(ftype)
				    {
				    case 0:
				      //cout<<"function found "<<tmpF.Name().data()<<endl;
				      newClass.addMethod(visibility,tmpF);
				      break;
				    case 1:
				      //cout<<"slot found "<<tmpF.Name().data()<<endl;
				      newClass.addSlot(visibility,tmpF);
				      break;
				    case 2:
				      //cout<<"signal found "<<tmpF.Name().data()<<endl;
				      newClass.addSignal(tmpF);
				      break;
				    } 
			      }
			  }
		      }
		    else
		      {
			functionNameTypeDoc tmpF(propertyString);
			tmpF.addDescriptionLine(comment);
			if(tmpF.Name()==string("~")+newClass.Name())
			  tmpF.addDescriptionLine("Destructor");
			if(tmpF.Name()==newClass.Name())
			  {
			    //cout<<"constructor found "<<endl;
			    if(tmpF.nParameters()==0)
			      tmpF.addDescriptionLine("Default constructor.");
			    newClass.addConstructor(tmpF);
			  }
			else
			  {
			    if(tmpF.TypeId()!=7)
			      {
				//cout<<"function found "<<tmpF.Name().data()<<endl;
				newClass.addMethod(visibility,tmpF);
			      }
			    else 
			      switch(ftype)
				{
				case 0:
				  //cout<<"function found "<<tmpF.Name().data()<<endl;
				  newClass.addMethod(visibility,tmpF);
				  break;
				case 1:
				  //cout<<"slot found "<<tmpF.Name().data()<<endl;
				  newClass.addSlot(visibility,tmpF);
				  break;
				case 2:
				  //cout<<"signal found "<<tmpF.Name().data()<<endl;
				  newClass.addSignal(tmpF);
				  break;
				} 
			  }
		      }
		  }
		else 
		  {
		    //cout<<"variable found :"<<propertyString.data()<<endl;
		    nameTypeDoc tmpName(propertyString);
		    tmpName.addDescriptionLine(comment);
		    newClass.addVariable(visibility,tmpName);
		  }
		curvedBrackets+=-contains((tmp.substr(0,tmp.find(";"))),"}");
		curvedBrackets+=contains((tmp.substr(0,tmp.find(";"))),"{");
		tmp=tmp.substr(tmp.find(";")+1,tmp.length()-tmp.find(";")-1);
		minimizeString(tmp);
		if(tmp==""){
		  i++;tmp=data[i];
		  while(tmp[0]=='#'&&(int)data.size()>i+1){i++;tmp=data[i];}
		  if(hasA(tmp,"//"))
		    tmp=tmp.substr(0,tmp.find("//"));
		  while(hasA(tmp,"/*")&&(int)data.size()>i+1)
		    {
		      if(hasA(tmp,"*/"))
			{
			  string s=tmp.substr(0,tmp.find("/*"));
			  tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
			  tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
			}
		      else
			{
			  while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
			}
		    }
		  if(hasA(tmp,"//"))
		    tmp=tmp.substr(0,tmp.find("//"));
		  minimizeString(tmp);}
		//		cout<<"go along with: "<<tmp.data()<<endl;
	      }
	    else if(hasA(tmp,":"))
	      {
		propertyString=tmp.substr(0,tmp.find(":"));
		tmp=tmp.substr(tmp.find(":")+1,tmp.length()-tmp.find(":")-1);
		minimizeString(tmp);
		if(hasA(propertyString,"public"))visibility=0;
		if(hasA(propertyString,"protected"))visibility=1;
		if(hasA(propertyString,"private"))visibility=2;
		if(hasA(propertyString,"slots"))ftype=1;
		if(hasA(propertyString,"signal"))ftype=2;
		if(tmp==""){
		  i++;tmp=data[i];
		  while(tmp[0]=='#'&&(int)data.size()>i+1){i++;tmp=data[i];}
		  if(hasA(tmp,"//"))
		    tmp=tmp.substr(0,tmp.find("//"));
		  while(hasA(tmp,"/*")&&(int)data.size()>i+1)
		    {
		      if(hasA(tmp,"*/"))
			{
			  string s=tmp.substr(0,tmp.find("/*"));
			  tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
			  tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
			}
		      else
			{
			  while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
			}
		    }
		  if(hasA(tmp,"//"))
		    tmp=tmp.substr(0,tmp.find("//"));
		  minimizeString(tmp);}
	      }
	    else
	      {
		while(tmp[i+1]=='#'&&(int)data.size()>i+2){i++;}
		tmp=tmp+data[++i];
		if(hasA(tmp,"//"))
		  tmp=tmp.substr(0,tmp.find("//"));
		while(hasA(tmp,"/*")&&(int)data.size()>i+1)
		  {
		    if(hasA(tmp,"*/"))
		      {
			string s=tmp.substr(0,tmp.find("/*"));
			tmp=tmp.substr(tmp.find("/*")+2,tmp.length()-tmp.find("/*")-2);
			tmp=s+tmp.substr(tmp.find("*/")+2,tmp.length()-tmp.find("*/")-2);
		      }
		    else
		      {
			while(!hasA(tmp,"*/")&&(int)data.size()>i+1){i++;tmp+=data[i];}
		      }
		  }
		if(hasA(tmp,"//"))
		  tmp=tmp.substr(0,tmp.find("//"));
		minimizeString(tmp);
	      }
	    if(curvedBrackets<=0&&hasA(tmp,";"))finished=true;
	    if(curvedBrackets-hasA(tmp,"}")<=0&&tmp.rfind(";")>tmp.rfind("}"))finished=true;
	  }
	if(finished)
	  {
	    newClass.addSuperDescriptions();
	    contens.push_back(newClass);
	    addToKnownClasses(newClass);
	    nfound++;
	  }
    }
    return nfound;
}

void minimizeString(string &s)
{
  s=remove(s,'\n');
    while(isSpace(s[0])&&s.length()>0)s=s.substr(1,s.length()-1);
    while(isSpace(s[s.length()-1])&&s.length()>0)s=s.substr(0,s.length()-1);
    for(unsigned int i=0;i<s.length();i++)
    {
	if(isSpace(s[i])&&isSpace(s[i-1]))
	{
	    s=s.substr(0,i)+s.substr(i+1,s.length()-i-1);
	    i--;
	}
    }
}

bool addSuperFromString(TMyClass & cls, string str)
{
    string tmp;
    int visibility=0;
    while(hasA(str,","))
    {
	tmp=str.substr(0,str.find(","));
	str=str.substr(str.find(",")+1,str.length()-str.find(",")-1);
	if(hasA(tmp,"public")){tmp=remove(tmp,"public");visibility=0;}
	if(hasA(tmp,"protected")){tmp=remove(tmp,"protected");visibility=1;}
	if(hasA(tmp,"private")){tmp=remove(tmp,"private");visibility=2;}
	tmp=remove(tmp," ");
	TMyClass *tmpClass=NULL;
	tmpClass=getKnownClass(tmp);
	if(tmpClass==NULL)
	  {
	    string s=knownTypesDoc(tmp);
	    cls.addSuperClass(tmp,s,visibility,tmpClass);
	  }
	else
	  cls.addSuperClass(tmp,(tmpClass==NULL?"":tmpClass->Doc()),visibility,tmpClass);
    }
    if(str!="")
    {
	if(hasA(str,"public")){str=remove(str,"public");visibility=0;}
	if(hasA(str,"protected")){str=remove(str,"protected");visibility=1;}
	if(hasA(str,"private")){str=remove(str,"private");visibility=2;}
	str=remove(str," ");
	TMyClass *tmpClass=NULL;
	tmpClass=getKnownClass(str);
	cls.addSuperClass(str,(tmpClass==NULL?"":tmpClass->Doc()),visibility,tmpClass);
    }
    return true;
}
int TMyClass::update()
{
  int modified=0;
  vector<TMyClass> cont;
  fileContent(1, headerfile, cont);
  int n=-1;
  for(unsigned int i=0;i<cont.size();i++)
    if(cont[i].Name()==className)
      n=i;
  if(n==-1)return -1;
  bool found;
  for(int i=0;i<cont[n].nSuperClasses();i++)
    {
      for(unsigned int j=0;j<supers.size();j++)
	if(superClasses[j]==cont[n].SuperClass(i))
	  {
	    found=true;
	    if(supers[j]==NULL&&cont[n].Super(i)!=NULL)
	      {supers[j]=cont[n].Super(i);}
	  }
      if(!found)
	{
	  modified++;
	  if(cont[n].Super(i)!=NULL)
	    addSuperClass(cont[n].Super(i),cont[n].InherritanceType(i));
	  else
	    addSuperClass(cont[n].SuperClass(i),cont[n].SuperClassDoc(i),cont[n].InherritanceType(i));
	}
    }
  int nn,m;
  for(int j=0;j<3;j++)
    {
      for(int i=0;i<cont[n].nVariables(j);i++)
	{
	  nn=-1;m=-1;
	  for(int k=0;k<3;k++)
	    for(int l=0;l<nVariables(k);l++)
	      if(cont[n].Variable(i,j)==Variable(l,k))
		{nn=k;m=l;}
	  if(nn==-1)
	    {
	      modified++;
	      addVariable(j,cont[n].getVariable(i,j));
	    }
	  else if(nn!=j)
	    {
	      modified++;
	      addVariable(j,Variable(m,nn));
	      Variable(m,nn).addDescriptionLine("<i>obsolete</i> new visibility!!");
	    }
	}
      for(int i=0;i<nVariables(j);i++)
	{
	  nn=-1;
	  for(int k=0;k<3;k++)
	    for(int l=0;l<cont[n].nVariables(k);l++)
	      if(cont[n].Variable(l,k)==Variable(i,j))
		nn=l;
	  if(nn==-1)
	    {
	      modified++;
	      Variable(i,j).addDescriptionLine("<i>obsolete</i> removed from class");
	    }
	}
    }
  for(int j=0;j<7;j++)
    {
      for(int i=0;i<cont[n].nMethods(j);i++)
	{
	  nn=-1;m=-1;
	  for(int k=0;k<7;k++)
	    for(int l=0;l<nMethods(k);l++)
	      if(cont[n].Method(i,j)==Method(l,k))
		{nn=k;m=l;}
	  if(nn==-1)
	    {
	      modified++;
	      addMethod(j,cont[n].getMethod(i,j));
	      cout<<"Method added: "<<Method(nMethods(j)-1,j).toString().data()<<endl;
	    }
	  else if(nn!=j)
	    {
	      modified++;
	      addMethod(j,Method(m,nn));
	      Method(m,nn).addDescriptionLine("<i>obsolete</i> new visibility!!");
	      cout<<"Method visibility changed: "<<endl;//Method(m,nn).toString().data()<<endl;
	    }
	}
      for(int i=0;i<nMethods(j);i++)
	{
	  nn=-1;
	  for(int k=0;k<7;k++)
	    for(int l=0;l<cont[n].nMethods(k);l++)
	      if(cont[n].Method(l,k)==Method(i,j))
		nn=l;
	  if(nn==-1)
	    {
	      modified++;
	      Method(i,j).addDescriptionLine("<i>obsolete</i> removed from class");
	    }
	}
    }
  for(int i=0;i<cont[n].nConstructors();i++)
    {
      found=false;
      for(unsigned int j=0;j<constructors.size();j++)
	if(cont[n].Constructor(i)==constructors[j])
	  found=true;
      if(!found)
	{
	  modified++;
	  cout<<"constructor added"<<endl;
	  addConstructor(cont[n].getConstructor(i));
	}
    }
  return modified;
}
vector<string> TMyClass::toHeader()const
{
  vector<string> ret;
  string line="class ";
  line+=Name();
  if(nSuperClasses()>0)
    {
      line+=":";
      for(int i=0;i<nSuperClasses()-1;i++)
	line+=SuperClass(i)+", ";
      line+=SuperClass(nSuperClasses()-1);
    }
    ret.push_back(line);
    ret.push_back("{");
    if(nVariables(2)>0)
    {
      ret.push_back("private:");
      for(int i=0;i<nVariables(2);i++)
	ret.push_back(string("  ")+getVariable(i,2).toString()+";");
    }
    if(nVariables(1)>0)
    {
      ret.push_back("protected:");
      for(int i=0;i<nVariables(1);i++)
	ret.push_back(string("  ")+getVariable(i,1).toString()+";");
    }
    if(nVariables(0)>0||nConstructors()>0||nMethods(0)>0)
      {
	ret.push_back("public:");
	for(int i=0;i<nVariables(0);i++)
	  ret.push_back(string("  ")+getVariable(i,0).toString()+";");
	for(int i=0;i<nConstructors();i++)
	  ret.push_back(string("  ")+getConstructor(i).toString()+";");
	for(int i=0;i<nMethods(0);i++)
	  ret.push_back(string("  ")+getMethod(i,0).toString()+";");
    }
    if(nMethods(1)>0)
      {
	ret.push_back("protected:");
	for(int i=0;i<nMethods(1);i++)
	  ret.push_back(string("  ")+getMethod(i,1).toString()+";");
      }
    if(nMethods(2)>0)
      {
	ret.push_back("private:");
	for(int i=0;i<nMethods(2);i++)
	  ret.push_back(string("  ")+getMethod(i,2).toString()+";");
      }
    if(nMethods(3)>0)
      {
	ret.push_back("public slots:");
	for(int i=0;i<nMethods(3);i++)
	  ret.push_back(string("  ")+getMethod(i,3).toString()+";");
      }
    if(nMethods(4)>0)
      {
	ret.push_back("protected slots:");
	for(int i=0;i<nMethods(4);i++)
	  ret.push_back(string("  ")+getMethod(i,4).toString()+";");
      }
    if(nMethods(5)>0)
      {
	ret.push_back("private slots:");
	for(int i=0;i<nMethods(5);i++)
	  ret.push_back(string("  ")+getMethod(i,5).toString()+";");
      }
    if(nSignals()>0)
      {
	ret.push_back("signals:");
	for(int i=0;i<nSignals();i++)
	  ret.push_back(string("  ")+getSignal(i).toString()+";");
      }
    ret.push_back("};");
    return ret;
}

void init_Q_and_T_object()
{
  functionNameTypeDoc classN("className","char*", 5, "virtual const");
  classN.addDescriptionLine("The className() function that returns the class name as a string at runtime, ");
  classN.addDescriptionLine("without requiring native runtime type information (RTTI) support through the C++ compiler.");
  functionNameTypeDoc inher("inherrits","bool",6,"const");
  inher.addParameter(nameTypeDoc("clname","char*",5,"const"));
  inher.addDescriptionLine("the inherits() function that returns whether an object is an instance of a class ");
  inher.addDescriptionLine("that inherits a specified class within the QObject inheritance tree. ");
  functionNameTypeDoc tr1("static QString tr(const char* sourceText, const char* comment )" );
  tr1.addDescriptionLine("Returns a translated version of sourceText, or sourceText itself if there is no "); 
  tr1.addDescriptionLine("appropriate translated version. The translation context is QObject with comment "); 
  tr1.addDescriptionLine("(0 by default). All QObject subclasses using the Q_OBJECT macro automatically have "); 
  tr1.addDescriptionLine("a reimplementation of this function with the subclass name as context."); 
  tr1.addDescriptionLine("Warning: This method is reentrant only if all translators are installed before calling "); 
  tr1.addDescriptionLine("this method. Installing or removing translators while performing translations is not "); 
  tr1.addDescriptionLine("supported. Doing so will probably result in crashes or other undesirable behavior. ");
  functionNameTypeDoc tr2("static QString trUtf8( const char* sourceText, const char* comment)");
  tr2.addDescriptionLine("Returns a translated version of sourceText, or QString::fromUtf8(sourceText) if there "); 
  tr2.addDescriptionLine("is no appropriate version. It is otherwise identical to tr(sourceText, comment). "); 
  tr2.addDescriptionLine("Warning: This method is reentrant only if all translators are installed before calling "); 
  tr2.addDescriptionLine("this method. Installing or removing translators while performing translations is not "); 
  tr2.addDescriptionLine("supported. Doing so will probably result in crashes or other undesirable behavior. ");
  functionNameTypeDoc prop1("virtual bool setProperty(const char* name, const QVariant& value)");
  prop1.addDescriptionLine("Sets the value of the object's name property to value.");
  prop1.addDescriptionLine("Returns TRUE if the operation was successful; otherwise returns FALSE. ");
  prop1.addDescriptionLine("Information about all available properties is provided through the metaObject()");
  functionNameTypeDoc prop2("virtual QVariant property(const char* name ) const");
  prop2.addDescriptionLine("Returns the value of the object\'s name property. ");
  prop2.addDescriptionLine("If no such property exists, the returned variant is invalid. ");
  prop2.addDescriptionLine("Information about all available properties are provided through the metaObject(). ");
  functionNameTypeDoc metaO("virtual QMetaObject* metaObject() const" );
  metaO.addDescriptionLine("Returns a pointer to the meta object of this object. ");
  metaO.addDescriptionLine("A meta object contains information about a class that inherits QObject, e.g. class name,");
  metaO.addDescriptionLine("superclass name, properties, signals and slots. Every class that contains the ");
  metaO.addDescriptionLine("Q_OBJECT macro will also have a meta object. ");
  metaO.addDescriptionLine("The meta object information is required by the signal/slot connection mechanism ");
  metaO.addDescriptionLine("and the property system. The functions isA() and inherits() also make use of the meta object. ");
  qobjectFunctions.push_back(classN);
  qobjectFunctions.push_back(inher);
  qobjectFunctions.push_back(tr1);
  qobjectFunctions.push_back(tr2);
  qobjectFunctions.push_back(prop1);
  qobjectFunctions.push_back(prop2);
  qobjectFunctions.push_back(metaO);

  functionNameTypeDoc cls("static TClass *Class()");
  cls.setTypeDoc("root.cern.ch/html/root/TClass.html");
  cls.addDescriptionLine("Returns a TClass representation of the class type.");
  functionNameTypeDoc cls1("static const char *Class_Name()");
  cls1.addDescriptionLine("Returns the classes name.");
  functionNameTypeDoc cls2("static Version_t Class_Version() { return id; } inline //");
  cls2.addDescriptionLine("Returns the version of the class.");

  functionNameTypeDoc cls3("static void Dictionary()");
  cls3.addDescriptionLine("");
  functionNameTypeDoc cls4("virtual TClass *IsA() const { return name::Class(); }");
  cls4.addDescriptionLine("Returns a TClass representation of the objects class.");
  cls4.setTypeDoc("root.cern.ch/html/root/TClass.html");
  functionNameTypeDoc cls5("virtual void ShowMembers(TMemberInspector &insp, char *parent)");
  cls5.addDescriptionLine("");
  functionNameTypeDoc cls6("virtual void Streamer(TBuffer &b)");
  cls6.addDescriptionLine("Streams or writes the property of the class to root-file.");
  functionNameTypeDoc cls7("void StreamerNVirtual(TBuffer &b) { name::Streamer(b); }");
  cls7.addDescriptionLine("");
  functionNameTypeDoc cls8("static const char *DeclFileName() { return __FILE__; }");
  cls8.addDescriptionLine("");
  functionNameTypeDoc cls9("static int ImplFileLine()");
  cls9.addDescriptionLine("");
  functionNameTypeDoc cls0("static const char *ImplFileName()");
  cls0.addDescriptionLine("");
  functionNameTypeDoc cls10("friend TBuffer &operator>>(TBuffer &buf, name *&obj)");
  cls10.addDescriptionLine("Streaming operator. Streams the objects properties to buffer.");
  cls10.setTypeDoc("root.cern.ch/html/root/TBuffer.html");
  functionNameTypeDoc cls11("friend TBuffer &operator>>(TBuffer &buf, const name *&obj)");
  cls11.addDescriptionLine("Streaming operator. Streams the objects properties to buffer");
  cls11.setTypeDoc("root.cern.ch/html/root/TBuffer.html");
  functionNameTypeDoc cls12("static int DeclFileLine() { return __LINE__; }");
  cls12.addDescriptionLine("");
  linkdefFunctions.push_back(cls);
  linkdefFunctions.push_back(cls1);
  linkdefFunctions.push_back(cls2);
  linkdefFunctions.push_back(cls3);
  linkdefFunctions.push_back(cls4);
  linkdefFunctions.push_back(cls5);
  linkdefFunctions.push_back(cls6);
  linkdefFunctions.push_back(cls7);
  linkdefFunctions.push_back(cls8);
  linkdefFunctions.push_back(cls9);
  linkdefFunctions.push_back(cls0);
  linkdefFunctions.push_back(cls10);
  linkdefFunctions.push_back(cls11);
  linkdefFunctions.push_back(cls12);
}

