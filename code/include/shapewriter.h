#ifndef SHAPEWRITER_H
#define SHAPEWRITER_H
#include "shapeparameter.h"
#include "documentation/include/tmyclass.h"
#include <vector>
#include <iostream>
using namespace std;

class shapeWriter
{
 private:
  static string fmakefile;
  static string fheader;
  static string fanalyser;
  string fheader_file;
  TMyClass fdefinition;
  int fclass_in_header;
  int fconstructor_in_class;
  vector<string> fsource_files;
  shape_parameter fparameter;
  int fcopy_or_link;
  vector<string> fcall_parameter;
  vector<string> fconstructor_call;
 public:
  shapeWriter();
  ~shapeWriter();
  static void init(string makefileIn, string headerIn, string getShapeFileIn);
  static string makefile() {return fmakefile;}
  static string header() {return fheader;}
  static string getShapeFile() {return fanalyser;}
  string headerFile()const{return fheader_file;}
  TMyClass definition()const{return fdefinition;}
  int classInHeader()const{return fclass_in_header;}
  int constructorInClass()const{return fconstructor_in_class;}
  string sourceFiles(int num)const
    {if(num<0||num>(int)fsource_files.size())return "";
    return fsource_files[num];}
  int nSourceFiles()const{return fsource_files.size();}
  shape_parameter parameter()const{return fparameter;}
  int copyOrLink()const{return fcopy_or_link;}
  string callParameter(int num)const
    {if(num<0||num>(int)fcall_parameter.size())return "";
    return fcall_parameter[num];}
  vector<string> constructorCall()const{return fconstructor_call;}

  void setHeaderFile(string s){fheader_file=s;}
  void setDefinition(const TMyClass& t){fdefinition=t;}
  void setClassInHeader(int n){fclass_in_header=n;}
  void setConstructorInClass(int n){fconstructor_in_class=n;}
  void addSourceFiles(string fn){fsource_files.push_back(fn);}
  void setSourceFiles(int num, string fn)
    {if(num<0||num>=(int)fsource_files.size())return;
    fsource_files[num]=fn;}
  void clearSourceFiles(){while(!fsource_files.empty())fsource_files.pop_back();}
  void removeSourceFile(string fn)
    {
      vector<string> tmp;
      while(!fsource_files.empty())
	{
	  if(fsource_files.back()!=fn) tmp.push_back(fsource_files.back());
	  fsource_files.pop_back();
	}
      while(!tmp.empty()){fsource_files.push_back(tmp.back());tmp.pop_back();}
    }
  void setParameter(const shape_parameter &p){fparameter=p;}
  void setCopyOrLink(int n){fcopy_or_link=n;}
  void setCallParameter(int num, string param)
    {if(num<0)return;
    while(num>(int)fcall_parameter.size()-1)fcall_parameter.push_back("");
    fcall_parameter[num]=param;}
  void setConstructorCall(vector<string> s){fconstructor_call=s;}

  vector<string> makeFileCall();
  vector<string> headerCall();
  vector<string> constrCall();
  void insertToMakeFile();
  void insertToHeader();
  void insertToShapes();

  bool shapeAvail() const;
  bool headerAvail() const;
  bool makeAvail()const;
  bool libAvail()const;
};
ostream &operator <<(ostream &o, const shapeWriter &wr);
istream &operator >>(istream &o, shapeWriter &wr);
#endif
