#include "shapewriter.h"
#include "baseShape.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
string shapeWriter::fmakefile = string(getenv("KTOFPACK")) + "/shapes/makefile";
string shapeWriter::fheader = string(getenv("KTOFPACK")) + "/shapes/include/shapes.h";
string shapeWriter::fanalyser = string(getenv("KTOFPACK")) + "/shapes/src/getShape.cpp";
extern base_shape* getShape(const shape_parameter& sp);
bool shapeWriter::libAvail() const
{
  base_shape* bs = getShape(fparameter);
  if (bs == NULL)
    return false;
  if (bs->getName() == fparameter.getName() || bs->getName() != "none") {
    delete bs;
    return true;
  }
  delete bs;
  return false;
}
shapeWriter::shapeWriter() : fheader_file(""), fclass_in_header(0), fcopy_or_link(0) {}
shapeWriter::~shapeWriter() {}
void shapeWriter::init(string makefileIn, string headerIn, string getShapeFileIn)
{
  fmakefile = makefileIn;
  fheader = headerIn;
  fanalyser = getShapeFileIn;
}
vector<string> shapeWriter::makeFileCall()
{
  vector<string> ret;
  string shortHeader = fheader_file;
  if (shortHeader.find("/") < shortHeader.npos)
    shortHeader = shortHeader.substr(shortHeader.rfind("/") + 1,
                                     shortHeader.length() - shortHeader.rfind("/") - 1);
  shortHeader = shortHeader.substr(0, shortHeader.rfind("."));
  ret.push_back(string("\tinclude/") + shortHeader + ".h \\");
  string sSource;
  for (unsigned int i = 0; i < fsource_files.size(); i++) {
    sSource = fsource_files[i];
    if (sSource.find("/") < sSource.npos)
      sSource = sSource.substr(sSource.rfind("/") + 1, sSource.length() - sSource.rfind("/") - 1);
    ret.push_back(string("\tsrc/") + sSource + " \\");
  }
  for (unsigned int i = 0; i < fsource_files.size(); i++) {
    sSource = fsource_files[i];
    if (sSource.find("/") < sSource.npos)
      sSource = sSource.substr(sSource.rfind("/") + 1, sSource.length() - sSource.rfind("/") - 1);
    sSource = sSource.substr(0, sSource.rfind("."));
    ret.push_back(string("\tobj/") + sSource + "$(PCCODE).o \\");
  }
  return ret;
}
vector<string> shapeWriter::headerCall()
{
  vector<string> ret;
  string shortHeader = fheader_file;
  if (shortHeader.find("/") < shortHeader.npos)
    shortHeader = shortHeader.substr(shortHeader.rfind("/") + 1,
                                     shortHeader.length() - shortHeader.rfind("/") - 1);
  ret.push_back(string("#include \"") + shortHeader + "\"");
  return ret;
}
vector<string> shapeWriter::constrCall()
{
  vector<string> ret;
  string pre = "    ";
  string s = string("shape=new ") + fdefinition.Name() + "(";
  if (fcall_parameter.size() > 0)
    s += fcall_parameter[0];
  if (fcall_parameter.size() > 1)
    s += ",";
  else
    s += ");";
  ret.push_back(pre + s);
  for (unsigned int i = 1; i < fcall_parameter.size(); i++)
    ret.push_back(pre + pre + fcall_parameter[i] + (i == fcall_parameter.size() - 1 ? ");" : ","));
  return ret;
}

void shapeWriter::insertToMakeFile()
{
  if (makeAvail())
    return;
  vector<string> tmp, backup;
  vector<string> code = makeFileCall();
  string s;
  char c;
  ifstream input;
  input.open(shapeWriter::fmakefile.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    tmp.push_back(s);
    backup.push_back(s);
    if (hasA(s, "HDR=") || hasA(s, "HDR ="))
      tmp.push_back(code[0]);
    else if (hasA(s, "SRC=") || hasA(s, "SRC ="))
      for (unsigned int i = 0; i < fsource_files.size(); i++)
        tmp.push_back(code[1 + i]);
    else if (hasA(s, "OBJS=") || hasA(s, "OBJS ="))
      for (unsigned int i = 0; i < fsource_files.size(); i++)
        tmp.push_back(code[1 + i + fsource_files.size()]);
  }
  input.close();
  ofstream output;
  output.open(shapeWriter::fmakefile.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output.close();
  ofstream backUp;
  backUp.open((shapeWriter::fmakefile + ".bck").data());
  for (unsigned int i = 0; i < backup.size(); i++)
    backUp << backup[i].data() << endl;
  backUp.close();
}
void shapeWriter::insertToHeader()
{
  if (headerAvail())
    return;
  vector<string> tmp;
  vector<string> code = headerCall();
  string s;
  char c;
  ifstream input;
  input.open(shapeWriter::fheader.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    tmp.push_back(s);
  }
  input.close();
  ofstream output;
  output.open(shapeWriter::fheader.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output << code[0].data() << endl;
  output.close();
}
void shapeWriter::insertToShapes()
{
  if (shapeAvail())
    return;
  vector<string> tmp, backup;
  string s;
  string pre = "    ";
  char c;
  if (fconstructor_call.size() == 0)
    setConstructorCall(constrCall());
  ifstream input;
  input.open(shapeWriter::fanalyser.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    backup.push_back(s);
    if (s.find("if(sp.getName()==") < s.npos &&
        ((!hasA(s, "else")) || s.find("if(sp.getName()==") < s.find("else"))) {
      tmp.push_back(pre + "if(sp.getName()==\"" + fparameter.getName() + "\")");
      tmp.push_back(pre + "  {");
      for (unsigned int i = 0; i < fconstructor_call.size(); i++)
        tmp.push_back(pre + fconstructor_call[i]);
      tmp.push_back(pre + "  }");
      s = pre + "else " +
          s.substr(s.find("if(sp.getName()=="), s.length() - s.find("if(sp.getName()=="));
    }
    tmp.push_back(s);
  }
  input.close();
  ofstream output;
  output.open(shapeWriter::fanalyser.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output.close();
  ofstream backUp;
  backUp.open((shapeWriter::fanalyser + ".bck").data());
  for (unsigned int i = 0; i < backup.size(); i++)
    backUp << backup[i].data() << endl;
  backUp.close();
}

bool shapeWriter::shapeAvail() const
{
  if (libAvail())
    return true;
  string search = fdefinition.Name();
  ifstream input;
  string s;
  char c;
  input.open(shapeWriter::fanalyser.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    if (hasA(s, search)) {
      input.close();
      return true;
    }
  }
  input.close();
  return false;
}
bool shapeWriter::headerAvail() const
{
  string search = fheader_file;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(shapeWriter::fheader.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    if (hasA(s, search)) {
      input.close();
      return true;
    }
  }
  input.close();
  return false;
}

bool shapeWriter::makeAvail() const
{
  string search = fheader_file;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(shapeWriter::fmakefile.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    if (hasA(s, search)) {
      input.close();
      return true;
    }
  }
  input.close();
  return false;
}

ostream& operator<<(ostream& o, const shapeWriter& wr)
{
  o << "###### shape name ########" << endl;
  o << wr.parameter().getName().data() << endl; // GUI shape name
  o << "###### description ############" << endl;
  o << wr.parameter() << endl;
  o << "###### class definition: file and position in file ############" << endl;
  string sHeader = wr.headerFile();
  if (sHeader.find("/") < sHeader.npos)
    sHeader = sHeader.substr(sHeader.rfind("/") + 1, sHeader.length() - sHeader.rfind("/") - 1);
  o << sHeader.data() << endl; // header file name
  o << wr.classInHeader() << " " << wr.constructorInClass() << " " << wr.copyOrLink()
    << endl; // Class name of shape, index of class in header file
  string sSource;
  for (int i = 0; i < wr.nSourceFiles(); i++) {
    sSource = wr.sourceFiles(i);
    if (sSource.find("/") < sSource.npos)
      sSource = sSource.substr(sSource.rfind("/") + 1, sSource.length() - sSource.rfind("/") - 1);
    o << sSource.data() << endl;
  }
  o << "###### constructor call variables ############" << endl;
  for (int i = 0; i < wr.definition().Constructor(wr.constructorInClass()).nParameters(); i++)
    o << wr.callParameter(i).data() << endl;
  o << "###### special constructor call ##############" << endl;
  o << (wr.constructorCall().size() > 0 ? 1 : 0) << endl; // use special constructor call
  if (wr.constructorCall().size() > 0)                    // constructor call
  {
    o << wr.constructorCall().size() << endl;
    for (unsigned int i = 0; i < wr.constructorCall().size(); i++)
      o << wr.constructorCall().at(i).data() << endl;
  }
  o << "###### finished ##########################" << endl;
  return o;
}
istream& operator>>(istream& o, shapeWriter& wr)
{
  string s;
  char c;
  int n[10];
  o.get(c);
  while (!(c == '\n' || o.eof())) {
    o.get(c);
  }
  if (o.eof())
    return o;
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  shape_parameter pa;
  o >> pa;
  wr.setParameter(pa);
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  s = "";
  o.get(c);
  while (!(c == '\n' || o.eof())) {
    s = s + c;
    o.get(c);
  }
  wr.setHeaderFile(s);
  if (o.eof())
    return o;
  o >> n[0] >> n[1] >> n[3];
  wr.setClassInHeader(n[0]);
  wr.setConstructorInClass(n[1]);
  wr.setCopyOrLink(n[3]);
  if (o.eof())
    return o;
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  o.get(c);
  while (c != '#' && !o.eof()) {
    s = c;
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    wr.addSourceFiles(s);
    o.get(c);
  }
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  n[0] = 0;
  o.get(c);
  while (!(c == '#' || o.eof())) {
    s = c;
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    wr.setCallParameter(n[0]++, s);
    o.get(c);
  }
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  o >> n[0];
  vector<string> tmp;
  for (int i = 0; i < n[0]; i++) {
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    tmp.push_back(s);
    if (o.eof()) {
      wr.setConstructorCall(tmp);
      return o;
    }
  }
  wr.setConstructorCall(tmp);
  return o;
}
