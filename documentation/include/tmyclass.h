#include <iostream>
#include <vector>
#ifndef TMYCLASS
#define TMYCLASS
//#define OLDINPUT
using namespace std;
string smallerPath(const string& path);
string expandPath(const string& path);

class nameTypeDoc
{
private:
  string name;
  string type;
  int typeId;
  string doc;
  string typeDoc;
  vector<string> description;
  string specifier;
  string defaultvalue;

public:
  nameTypeDoc();
  nameTypeDoc(string nameIn, string typeIn, int typeIdIn = 0, string specifierIn = "",
              string docIn = "", string typeDocIn = "");
  nameTypeDoc(const nameTypeDoc& n);
  nameTypeDoc(const string& s);
  virtual ~nameTypeDoc();
  string Name() const;
  string Type() const;
  string Doc() const;
  string TypeDoc() const;
  string Description(int i) const;
  int DescriptionLines() const;
  string Specifier() const;
  string Default() const;
  int TypeId() const;
  void setType(const string& s);
  void setName(const string& s);
  void setDoc(const string& s);
  void setTypeId(int v);
  void setTypeDoc(const string& s);
  void setDescription(const vector<string>& input);
  void setSpecifier(const string& s);
  void setDefault(const string& s);
  void clearDescription();
  void addDescriptionLine(const string& s);
  void parse(const string& expression);
  virtual string toString(int ptt = 0) const;
  virtual string htmlHREF() const;
  virtual vector<string> htmlNAME() const;
  nameTypeDoc& operator=(const nameTypeDoc& nt);
  bool operator==(const nameTypeDoc& nt);
  static int typeIDS(const string& s);
  static nameTypeDoc parseIt(const string& expression);
};
ostream& operator<<(ostream& o, const nameTypeDoc& t);
istream& operator>>(istream& o, nameTypeDoc& t);
class functionNameTypeDoc : public nameTypeDoc
{
private:
  vector<nameTypeDoc> parameter;
  string limiter;
  string comment;

public:
  functionNameTypeDoc();
  functionNameTypeDoc(string nameIn, string typeIn, int typeIdIn = 0, string specifierIn = "",
                      string docIn = "", string typeDocIn = "");
  functionNameTypeDoc(const functionNameTypeDoc& n);
  functionNameTypeDoc(const string& s);
  virtual ~functionNameTypeDoc();
  string Limiter() const;
  void setLimiter(const string& s);
  nameTypeDoc& Parameter(int i);
  nameTypeDoc getParameter(int i) const;
  int nParameters() const;
  void clearParameter();
  void addParameter(const nameTypeDoc& input);
  void addParameter(string nameIn, string typeIn, int typeIdIn, string specifierIn = "",
                    string docIn = "", string typeDocIn = "");
  void parse(const string& s);
  virtual string toString(int ptt = 0) const;
  virtual string ParameterList(int ptt = 0) const;
  virtual string htmlHREF() const;
  virtual vector<string> htmlNAME(const string& classname = "") const;
  functionNameTypeDoc& operator=(const functionNameTypeDoc& fn);
  bool operator==(const functionNameTypeDoc& fn);
  static functionNameTypeDoc parseIt(const string& expression);
};
ostream& operator<<(ostream& o, const functionNameTypeDoc& t);
istream& operator>>(istream& o, functionNameTypeDoc& t);

class TMyClass
{
private:
  string className;
  string classDoc;
  string headerfile;
  vector<string> description;
  vector<string> superClasses;
  vector<string> superClassDocs;
  vector<TMyClass*> supers;
  vector<int> inherritance;
  vector<nameTypeDoc> variablesPrivate;
  vector<nameTypeDoc> variablesProtected;
  vector<nameTypeDoc> variablesPublic;
  vector<functionNameTypeDoc> functionsPrivate;
  vector<functionNameTypeDoc> functionsProtected;
  vector<functionNameTypeDoc> functionsPublic;
  vector<functionNameTypeDoc> thesignals;
  vector<functionNameTypeDoc> slotsPublic;
  vector<functionNameTypeDoc> slotsProtected;
  vector<functionNameTypeDoc> slotsPrivate;
  vector<vector<int>> functionType;
  vector<functionNameTypeDoc> constructors;
  template <class X>
  void removeFromVector(vector<X>& v, int pos);
  void checkFunctionType();

public:
  TMyClass(string name = "", string doc = "");
  TMyClass(const TMyClass& n);
  virtual ~TMyClass();
  TMyClass& operator=(const TMyClass& n);
  //  bool operator==(const TMyClass&n);
  void clear();
  string Name() const;
  void setName(const string& s);
  string Doc() const;
  void setDoc(const string& s);
  bool inherritsFrom(const string& clsn);
  string Description(int i) const;
  int DescriptionLines() const;
  void setDescription(const vector<string>& input);
  void clearDescription();
  void addDescriptionLine(const string& s);
  string HeaderFile() const;
  void setHeaderFile(const string& s);
  int nSuperClasses() const;
  string SuperClass(int i) const;
  string SuperClassDoc(int i) const;
  TMyClass* Super(int i) const;
  int InherritanceType(int i) const;
  string Inherritance(int i) const;
  int nVariables(int t) const;
  nameTypeDoc& Variable(int i, int t);
  nameTypeDoc getVariable(int i, int t) const;
  int nMethods(int t) const;
  functionNameTypeDoc& Method(int i, int t);
  functionNameTypeDoc getMethod(int i, int t) const;
  int MethodType(int i, int t) const;
  int nSlots(int i) const;
  functionNameTypeDoc& Slot(int i, int t);
  functionNameTypeDoc getSlot(int i, int t) const;
  int nSignals() const;
  functionNameTypeDoc& Signal(int i);
  functionNameTypeDoc getSignal(int i) const;
  int nConstructors() const;
  functionNameTypeDoc getConstructor(int i) const;
  functionNameTypeDoc& Constructor(int i);
  void clearSuperClasses();
  void addSuperClass(const string& superClassName, const string& superClassDoc, int derived = 0,
                     TMyClass* super = NULL);
  void addSuperClass(TMyClass* super, int derived = 0);
  void removeSuperClass(const string& n);
  void removeSuperClass(int num);
  void clearVariable(int t);
  void addVariable(int t, const nameTypeDoc& var);
  void addVariable(int t, string nameIn, string typeIn, int typeIdIn, string specifier,
                   string docIn = "", string typeDocIn = "");
  void removeVariable(int t, int i);
  void removeVariable(const string& n);
  void clearMethod(int t);
  void addMethod(int t, const functionNameTypeDoc& var);
  void addMethod(int t, string nameIn, string typeIn, int typeIdIn, string specifier,
                 string docIn = "", string typeDocIn = "");
  void removeMethod(int t, int i);
  void removeMethod(const string& n);
  void clearSlots(int i);
  void addSlot(int t, const functionNameTypeDoc& var);
  void addSlot(int t, string nameIn, string typeIn, int typeIdIn, string specifier,
               string docIn = "", string typeDocIn = "");
  void removeSlot(int t, int i);
  void removeSlot(const string& n);
  void clearSignals();
  void addSignal(const functionNameTypeDoc& var);
  void addSignal(string nameIn, string typeIn, int typeIdIn, string specifier, string docIn = "",
                 string typeDocIn = "");
  void clearConstructor();
  void addConstructor(const functionNameTypeDoc& var);
  void addConstructor(string typeIn, int typeIdIn, string specifier, string docIn = "",
                      string typeDocIn = "");
  void removeConstructor(int i);
  void removeConstructor(const string& n);
  void addSuperDescriptions();
  vector<string> toHTML() const;
  vector<string> toHTML(const vector<functionNameTypeDoc>& associatedFunctions) const;
  vector<string> toHeader() const;
  bool readFromFile(int filetype, const string& filename, const string& nameIn = "");
  bool readFromData(int datatype, const vector<string>& data, const string& nameIn = "");
  static int fileContent(int fileType, const string& filename, vector<TMyClass>& content);
  static int content(int dataType, const vector<string>& data, vector<TMyClass>& content);
  int update();
};
ostream& operator<<(ostream& o, const TMyClass& t);
istream& operator>>(istream& o, TMyClass& t);
#endif
