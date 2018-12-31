#ifndef ALGORITHMWRITER_H
#define ALGORITHMWRITER_H
#include "algorithmparameter.h"
#include "tmyclass.h"
#include <iostream>
#include <vector>
using namespace std;

class algorithmWriter
{
private:
  static string fmakefile;
  static string fiomakefile;
  static string fheader;
  static string fw_header;
  static string fiwidget;
  static string fowidget;
  static string fanalyser;
  string fheader_file;
  TMyClass fdefinition;
  int fclass_in_header;
  vector<string> fsource_files;
  algorithm_parameter fparameter;
  bool fneeds_moc;
  int fcopy_or_link;
  bool fis_io;
  bool finput_algo;
  string fwidget_name;
  string fwidget_header;
  string fwidget_source;
  bool fversion_dependent_header;
  bool fioversion_dependent_header;
  int ffrequency;
  int ffrequency_string;
  int fnuncall;
  int fid;
  vector<string> fconnections;
  vector<string> fcall_parameter;
  string fcomment_on_init;
  vector<string> fconstructor_call;

public:
  algorithmWriter();
  ~algorithmWriter();
  static void init(string makefileIn, string iomakefileIn, string headerIn, string widgetHeaderIn,
                   string iwidgetIn, string owidgetIn, string analyserIn);
  static string makefile() { return fmakefile; }
  static string header() { return fheader; }
  static string iwidget() { return fiwidget; }
  static string owidget() { return fowidget; }

  static string analyser() { return fanalyser; }

  static string IOmakefile() { return fiomakefile; }
  static string IOwidgetHeader() { return fw_header; }
  int ID() const { return fid; }
  string headerFile() const { return fheader_file; }
  TMyClass definition() const { return fdefinition; }
  int classInHeader() const { return fclass_in_header; }
  string sourceFiles(int num) const
  {
    if (num < 0 || num > (int)fsource_files.size())
      return "";
    return fsource_files[num];
  }
  int nSourceFiles() const { return fsource_files.size(); }
  algorithm_parameter parameter() const { return fparameter; }
  bool needsMOC() const { return fneeds_moc; }
  int copyOrLink() const { return fcopy_or_link; }
  bool isIo() const { return fis_io; }
  bool inputAlgo() const { return finput_algo; }
  string widgetHeader() const { return fwidget_header; }
  string widgetName() const { return fwidget_name; }
  string widgetSource() const { return fwidget_source; }
  bool versionDependentHeader() const { return fversion_dependent_header; }
  bool ioVersionDependentHeader() const { return fioversion_dependent_header; }
  int frequency() const { return ffrequency; }
  int frequencyString() const
  {
    return ffrequency_string;
    ;
  }
  int nunCall() const { return fnuncall; }
  string connections(int num) const
  {
    if (num < 0 || num > (int)fconnections.size())
      return "";
    return fconnections[num];
  }

  int nConnections() const { return fconnections.size() / 4; }
  string callParameter(int num) const
  {
    if (num < 0 || num > (int)fcall_parameter.size())
      return "";
    return fcall_parameter[num];
  }
  string commentOnInit() const { return fcomment_on_init; }
  vector<string> constructorCall() const { return fconstructor_call; }

  void setID(int n) { fid = n; }
  void setHeaderFile(string s) { fheader_file = s; }
  void setDefinition(const TMyClass& t) { fdefinition = t; }
  void setClassInHeader(int n) { fclass_in_header = n; }
  void addSourceFiles(string fn) { fsource_files.push_back(fn); }
  void setSourceFiles(int num, string fn)
  {
    if (num < 0 || num >= (int)fsource_files.size())
      return;
    fsource_files[num] = fn;
  }
  void clearSourceFiles()
  {
    while (!fsource_files.empty())
      fsource_files.pop_back();
  }
  void removeSourceFile(string fn)
  {
    vector<string> tmp;
    while (!fsource_files.empty()) {
      if (fsource_files.back() != fn)
        tmp.push_back(fsource_files.back());
      fsource_files.pop_back();
    }
    while (!tmp.empty()) {
      fsource_files.push_back(tmp.back());
      tmp.pop_back();
    }
  }
  void setParameter(const algorithm_parameter& p) { fparameter = p; }
  void setCopyOrLink(int n) { fcopy_or_link = n; }
  void setNeedsMOC(bool v) { fneeds_moc = v; }
  void setIo(bool v) { fis_io = v; }
  void setInputAlgo(bool v) { finput_algo = v; }
  void setWidgetHeader(string s) { fwidget_header = s; }
  void setWidgetName(string s) { fwidget_name = s; }
  void setWidgetSource(string s) { fwidget_source = s; }
  void setVersionDependentHeader(bool v) { fversion_dependent_header = v; }
  void setIoVersionDependentHeader(bool v) { fioversion_dependent_header = v; }
  void setFrequency(int n) { ffrequency = n; }
  void setFrequencyString(int n) { ffrequency_string = n; }
  void setNunCall(int n) { fnuncall = n; }
  void addConnection(string sender, string signal, string receiver, string slot)
  {
    fconnections.push_back(sender);
    fconnections.push_back(signal);
    fconnections.push_back(receiver);
    fconnections.push_back(slot);
  }
  void clearConnections()
  {
    while (!fconnections.empty())
      fconnections.pop_back();
  }
  void clearCallParameter()
  {
    while (!fcall_parameter.empty())
      fcall_parameter.pop_back();
  }
  void setCallParameter(int num, string param)
  {
    if (num < 0)
      return;
    while (num > (int)fcall_parameter.size() - 1)
      fcall_parameter.push_back("");
    fcall_parameter[num] = param;
  }
  void setCommentOnInit(string s) { fcomment_on_init = s; }
  void setConstructorCall(vector<string> s) { fconstructor_call = s; }

  vector<string> makeFileCall();
  vector<string> iomakeFileCall();
  vector<string> headerCall();
  vector<string> ioHeaderCall();
  vector<string> ioCall();
  vector<string> constrCall();
  string getFrequency();
  void insertToMakeFile();
  void insertToIOMakeFile();
  void insertToHeader();
  void insertToIOHeader();
  void insertToIO();
  void insertToAnalyser();

  bool analyserAvail() const;
  bool ioAvail() const;
  bool ioMakeAvail() const;
  bool ioHeaderAvail() const;
  bool headerAvail() const;
  bool makeAvail() const;
  bool libAvail() const;

  int getNextID();
  int getInstalledID();
};
ostream& operator<<(ostream& o, const algorithmWriter& wr);
istream& operator>>(istream& o, algorithmWriter& wr);
#endif
