#include <iostream>
#include <vector>
#include <string>
#ifndef TMYCLASS
#define TMYCLASS
//#define OLDINPUT
std::string smallerPath(const std::string& path);
std::string expandPath(const std::string& path);

class nameTypeDoc
{
private:
  std::string name;
  std::string type;
  int typeId;
  std::string doc;
  std::string typeDoc;
  std::vector<std::string> description;
  std::string specifier;
  std::string defaultvalue;

public:
  nameTypeDoc();
  nameTypeDoc(std::string nameIn, std::string typeIn, int typeIdIn = 0, std::string specifierIn = "",
              std::string docIn = "", std::string typeDocIn = "");
  nameTypeDoc(const nameTypeDoc& n);
  nameTypeDoc(const std::string& s);
  virtual ~nameTypeDoc();
  std::string Name() const;
  std::string Type() const;
  std::string Doc() const;
  std::string TypeDoc() const;
  std::string Description(int i) const;
  int DescriptionLines() const;
  std::string Specifier() const;
  std::string Default() const;
  int TypeId() const;
  void setType(const std::string& s);
  void setName(const std::string& s);
  void setDoc(const std::string& s);
  void setTypeId(int v);
  void setTypeDoc(const std::string& s);
  void setDescription(const std::vector<std::string>& input);
  void setSpecifier(const std::string& s);
  void setDefault(const std::string& s);
  void clearDescription();
  void addDescriptionLine(const std::string& s);
  void parse(const std::string& expression);
  virtual std::string toString(int ptt = 0) const;
  virtual std::string htmlHREF() const;
  virtual std::vector<std::string> htmlNAME(const std::string& classname = "") const;
  nameTypeDoc& operator=(const nameTypeDoc& nt);
  bool operator==(const nameTypeDoc& nt);
  static int typeIDS(const std::string& s);
  static nameTypeDoc parseIt(const std::string& expression);
};
std::ostream& operator<<(std::ostream& o, const nameTypeDoc& t);
std::istream& operator>>(std::istream& o, nameTypeDoc& t);
class functionNameTypeDoc : public nameTypeDoc
{
private:
  std::vector<nameTypeDoc> parameter;
  std::string limiter;
  std::string comment;

public:
  functionNameTypeDoc();
  functionNameTypeDoc(std::string nameIn, std::string typeIn, int typeIdIn = 0, std::string specifierIn = "",
                      std::string docIn = "", std::string typeDocIn = "");
  functionNameTypeDoc(const functionNameTypeDoc& n);
  functionNameTypeDoc(const std::string& s);
  virtual ~functionNameTypeDoc();
  std::string Limiter() const;
  void setLimiter(const std::string& s);
  nameTypeDoc& Parameter(int i);
  nameTypeDoc getParameter(int i) const;
  int nParameters() const;
  void clearParameter();
  void addParameter(const nameTypeDoc& input);
  void addParameter(std::string nameIn, std::string typeIn, int typeIdIn, std::string specifierIn = "",
                    std::string docIn = "", std::string typeDocIn = "");
  void parse(const std::string& s);
  virtual std::string toString(int ptt = 0) const;
  virtual std::string ParameterList(int ptt = 0) const;
  virtual std::string htmlHREF() const;
  virtual std::vector<std::string> htmlNAME(const std::string& classname = "") const;
  functionNameTypeDoc& operator=(const functionNameTypeDoc& fn);
  bool operator==(const functionNameTypeDoc& fn);
  static functionNameTypeDoc parseIt(const std::string& expression);
};
std::ostream& operator<<(std::ostream& o, const functionNameTypeDoc& t);
std::istream& operator>>(std::istream& o, functionNameTypeDoc& t);

class TMyClass
{
private:
  std::string className;
  std::string classDoc;
  std::string headerfile;
  std::vector<std::string> description;
  std::vector<std::string> superClasses;
  std::vector<std::string> superClassDocs;
  std::vector<TMyClass*> supers;
  std::vector<int> inherritance;
  std::vector<nameTypeDoc> variablesPrivate;
  std::vector<nameTypeDoc> variablesProtected;
  std::vector<nameTypeDoc> variablesPublic;
  std::vector<functionNameTypeDoc> functionsPrivate;
  std::vector<functionNameTypeDoc> functionsProtected;
  std::vector<functionNameTypeDoc> functionsPublic;
  std::vector<functionNameTypeDoc> thesignals;
  std::vector<functionNameTypeDoc> slotsPublic;
  std::vector<functionNameTypeDoc> slotsProtected;
  std::vector<functionNameTypeDoc> slotsPrivate;
  std::vector<std::vector<int>> functionType;
  std::vector<functionNameTypeDoc> constructors;
  template <class X>
  void removeFromVector(std::vector<X>& v, int pos);
  void checkFunctionType();

public:
  TMyClass(std::string name = "", std::string doc = "");
  TMyClass(const TMyClass& n);
  virtual ~TMyClass();
  TMyClass& operator=(const TMyClass& n);
  //  bool operator==(const TMyClass&n);
  void clear();
  std::string Name() const;
  void setName(const std::string& s);
  std::string Doc() const;
  void setDoc(const std::string& s);
  bool inherritsFrom(const std::string& clsn);
  std::string Description(int i) const;
  int DescriptionLines() const;
  void setDescription(const std::vector<std::string>& input);
  void clearDescription();
  void addDescriptionLine(const std::string& s);
  std::string HeaderFile() const;
  void setHeaderFile(const std::string& s);
  int nSuperClasses() const;
  std::string SuperClass(int i) const;
  std::string SuperClassDoc(int i) const;
  TMyClass* Super(int i) const;
  int InherritanceType(int i) const;
  std::string Inherritance(int i) const;
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
  void addSuperClass(const std::string& superClassName, const std::string& superClassDoc, int derived = 0,
                     TMyClass* super = NULL);
  void addSuperClass(TMyClass* super, int derived = 0);
  void removeSuperClass(const std::string& n);
  void removeSuperClass(int num);
  void clearVariable(int t);
  void addVariable(int t, const nameTypeDoc& var);
  void addVariable(int t, std::string nameIn, std::string typeIn, int typeIdIn, std::string specifier,
                   std::string docIn = "", std::string typeDocIn = "");
  void removeVariable(int t, int i);
  void removeVariable(const std::string& n);
  void clearMethod(int t);
  void addMethod(int t, const functionNameTypeDoc& var);
  void addMethod(int t, std::string nameIn, std::string typeIn, int typeIdIn, std::string specifier,
                 std::string docIn = "", std::string typeDocIn = "");
  void removeMethod(int t, int i);
  void removeMethod(const std::string& n);
  void clearSlots(int i);
  void addSlot(int t, const functionNameTypeDoc& var);
  void addSlot(int t, std::string nameIn, std::string typeIn, int typeIdIn, std::string specifier,
               std::string docIn = "", std::string typeDocIn = "");
  void removeSlot(int t, int i);
  void removeSlot(const std::string& n);
  void clearSignals();
  void addSignal(const functionNameTypeDoc& var);
  void addSignal(std::string nameIn, std::string typeIn, int typeIdIn, std::string specifier, std::string docIn = "",
                 std::string typeDocIn = "");
  void clearConstructor();
  void addConstructor(const functionNameTypeDoc& var);
  void addConstructor(std::string typeIn, int typeIdIn, std::string specifier, std::string docIn = "",
                      std::string typeDocIn = "");
  void removeConstructor(int i);
  void removeConstructor(const std::string& n);
  void addSuperDescriptions();
  std::vector<std::string> toHTML() const;
  std::vector<std::string> toHTML(const std::vector<functionNameTypeDoc>& associatedFunctions) const;
  std::vector<std::string> toHeader() const;
  bool readFromFile(int filetype, const std::string& filename, const std::string& nameIn = "");
  bool readFromData(int datatype, const std::vector<std::string>& data, const std::string& nameIn = "");
  static int fileContent(int fileType, const std::string& filename, std::vector<TMyClass>& content);
  static int content(int dataType, const std::vector<std::string>& data, std::vector<TMyClass>& content);
  int update();
};
std::ostream& operator<<(std::ostream& o, const TMyClass& t);
std::istream& operator>>(std::istream& o, TMyClass& t);
#endif
