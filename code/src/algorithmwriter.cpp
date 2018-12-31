#include "algorithmwriter.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
string algorithmWriter::fmakefile = string(getenv("KTOFPACK")) + "/algorithms/makefile";
string algorithmWriter::fiomakefile =
    string(getenv("KTOFPACK")) + "/gui/io_algorithm_widgets/makefile";
string algorithmWriter::fheader = string(getenv("KTOFPACK")) + "/algorithms/include/algorithms.h";
string algorithmWriter::fw_header =
    string(getenv("KTOFPACK")) + "/gui/io_algorithm_widgets/include/ioalgorithms.h";
string algorithmWriter::fiwidget =
    string(getenv("KTOFPACK")) + "/gui/typeCase/src/addInputAlgoWidget.h";
string algorithmWriter::fowidget =
    string(getenv("KTOFPACK")) + "/gui/typeCase/src/addOutputAlgoWidget.h";
string algorithmWriter::fanalyser =
    string(getenv("KTOFPACK")) + "/analyser/src/tofAnalysis_algorithm_t.cpp";

bool algorithmWriter::libAvail() const { return false; }
algorithmWriter::algorithmWriter()
    : fheader_file(""), fclass_in_header(0), fneeds_moc(false), fcopy_or_link(0), fis_io(false),
      finput_algo(false), fwidget_name(""), fwidget_header(""), fwidget_source(""),
      fversion_dependent_header(false), fioversion_dependent_header(false), ffrequency(0),
      ffrequency_string(0), fnuncall(0), fid(0), fcomment_on_init("")
{
}
algorithmWriter::~algorithmWriter() {}
void algorithmWriter::init(string makefileIn, string iomakefileIn, string headerIn,
                           string widgetHeaderIn, string iwidgetIn, string owidgetIn,
                           string analyserIn)
{
  fmakefile = makefileIn;
  fiomakefile = iomakefileIn;
  fheader = headerIn;
  fiwidget = iwidgetIn;
  fowidget = owidgetIn;
  fanalyser = analyserIn;
  fw_header = widgetHeaderIn;
}
vector<string> algorithmWriter::makeFileCall()
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
  if (fneeds_moc) {
    if (fversion_dependent_header) {
      ret.push_back(string("\tobj/moc_") + shortHeader + "$(QT_VERSION)$(PCCODE).cpp \\");
      ret.push_back(string("\tobj/moc_") + shortHeader + "$(QT_VERSION)$(PCCODE).o \\");
    } else {
      ret.push_back(string("\tobj/moc_") + shortHeader + "$(PCCODE).cpp \\");
      ret.push_back(string("\tobj/moc_") + shortHeader + "$(PCCODE).o \\");
    }
  }
  return ret;
}
vector<string> algorithmWriter::headerCall()
{
  vector<string> ret;
  string shortHeader = fheader_file;
  if (shortHeader.find("/") < shortHeader.npos)
    shortHeader = shortHeader.substr(shortHeader.rfind("/") + 1,
                                     shortHeader.length() - shortHeader.rfind("/") - 1);
  ret.push_back(string("#include \"") + shortHeader + "\"");
  return ret;
}
vector<string> algorithmWriter::ioHeaderCall()
{
  vector<string> ret;
  string shortHeader = fwidget_header;
  if (shortHeader.find("/") < shortHeader.npos)
    shortHeader = shortHeader.substr(shortHeader.rfind("/") + 1,
                                     shortHeader.length() - shortHeader.rfind("/") - 1);
  ret.push_back(string("#include \"") + shortHeader + "\"");
  return ret;
}
vector<string> algorithmWriter::ioCall()
{
  vector<string> ret;
  if (!fis_io)
    return ret;
  if (finput_algo) {
    ret.push_back(string("w=new ") + fwidget_name + "(inStack);");
    ret.push_back(string("w->setID(") + string_number(fid) + ");");
    ret.push_back(string("inStack->addWidget(w,\"") + fparameter.getName() + "\");");
  } else {
    ret.push_back(string("w=new ") + fwidget_name + "(outStack);");
    ret.push_back(string("w->setID(") + string_number(fid) + ");");
    ret.push_back(string("outStack->addWidget(w,\"") + fparameter.getName() + "\");");
  }
  ret.push_back(string("connect(w,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT("
                       "algorithmAdd(algorithm_parameter *)));"));
  return ret;
}
vector<string> algorithmWriter::iomakeFileCall()
{
  vector<string> ret;
  if (!fis_io)
    return ret;
  string sHeader = fwidget_header;
  if (sHeader.find("/") < sHeader.npos)
    sHeader = sHeader.substr(sHeader.rfind("/") + 1, sHeader.length() - sHeader.rfind("/") - 1);
  sHeader = sHeader.substr(0, sHeader.rfind("."));
  ret.push_back(string("\tinclude/") + sHeader + ".h \\");
  string sSource = fwidget_source;
  if (sSource.find("/") < sSource.npos)
    sSource = sSource.substr(sSource.rfind("/") + 1, sSource.length() - sSource.rfind("/") - 1);
  ret.push_back(string("\tsrc/") + sSource + " \\");
  sSource = sSource.substr(0, sSource.rfind("."));
  ret.push_back(string("\tobj/") + sSource + "$(PCCODE).o \\");
  if (fioversion_dependent_header) {
    ret.push_back(string("\tobj/moc_") + sHeader + "$(QT_VERSION)$(PCCODE).cpp \\");
    ret.push_back(string("\tobj/moc_") + sHeader + "$(QT_VERSION)$(PCCODE).o \\");
  } else {
    ret.push_back(string("\tobj/moc_") + sHeader + "$(PCCODE).cpp \\");
    ret.push_back(string("\tobj/moc_") + sHeader + "$(PCCODE).o \\");
  }
  return ret;
}
string algorithmWriter::getFrequency()
{
  string fs = "";
  int n = -1;
  switch (ffrequency) {
  case 0:
    fs = "1";
    break;
  case 1:
    fs = "NumberOfTracks";
    break;
  case 2:
    fs = "NumberOfDetectors";
    break;
  case 3: {
    fs = string("param.getParam<vector<");
    if (ffrequency_string < fparameter.getNumberOfParam<vector<int>>()) {
      fs += "int";
      n = ffrequency_string;
    } else if (ffrequency_string < fparameter.getNumberOfParam<vector<float>>() +
                                       fparameter.getNumberOfParam<vector<int>>()) {
      fs += "float";
      n = ffrequency_string - fparameter.getNumberOfParam<vector<int>>();
    } else if (ffrequency_string < fparameter.getNumberOfParam<vector<string>>() +
                                       fparameter.getNumberOfParam<vector<float>>() +
                                       fparameter.getNumberOfParam<vector<int>>()) {
      fs += "string";
      n = ffrequency_string - fparameter.getNumberOfParam<vector<float>>() -
          fparameter.getNumberOfParam<vector<int>>();
    } else
      fs = "1";
    if (n >= 0)
      fs += string("> >(") + string_number(n) + ").size()";
    break;
  }
  }
  return fs;
}
vector<string> algorithmWriter::constrCall()
{
  vector<string> ret;
  string pre = "";
  string fs = getFrequency();
  ret.push_back(pre + "anaLog<<\"" + fcomment_on_init + ":\";");
  ret.push_back(pre + "nAlgos=" + fs + "+" + string_number(fnuncall) + ";");
  ret.push_back(pre + "(*out)=new AAlgorithm*[nAlgos];");
  string s = string("(*out)[0]=new ") + fdefinition.Name() + "(";
  if (fcall_parameter.size() > 0)
    s += fcall_parameter[0];
  if (fcall_parameter.size() > 1)
    s += ",";
  else
    s += ");";
  ret.push_back(pre + s);
  for (unsigned int i = 1; i < fcall_parameter.size(); i++)
    ret.push_back(pre + pre + fcall_parameter[i] + (i == fcall_parameter.size() - 1 ? ");" : ","));
  for (unsigned int i = 0; i < fconnections.size() / 4; i++) {
    string sender = fconnections[i * 4], receiver = fconnections[i * 4 + 2],
           signal = fconnections[i * 4 + 1], slot = fconnections[i * 4 + 3];
    if (sender == "analyser" || sender == "tofAnalysis" || sender == "ANALYSER")
      sender = "stearIt";
    if (receiver == "algorithm" || receiver == "ALGORITHM" || receiver == "algo" ||
        receiver == fdefinition.Name())
      receiver = "(*out)[0]";
    ret.push_back(pre + "connect(" + sender + ", SIGNAL(" + signal + "), " + receiver + ", SLOT(" +
                  slot + "));");
  }
  ret.push_back("anaLog<<\"done\"<<endli;");
  return ret;
}

void algorithmWriter::insertToMakeFile()
{
  if (makeAvail())
    return;
  vector<string> tmp, backup;
  vector<string> code = makeFileCall();
  string s;
  char c;
  ifstream input;
  input.open(algorithmWriter::fmakefile.data());
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
    else if (hasA(s, "MOCSRC=") || hasA(s, "MOCSRC =")) {
      if (fneeds_moc)
        tmp.push_back(code[1 + fsource_files.size() * 2]);
    } else if (hasA(s, "MOCOBJ=") || hasA(s, "MOCOBJ =")) {
      if (fneeds_moc)
        tmp.push_back(code[2 + fsource_files.size() * 2]);
    } else if (hasA(s, "SRC=") || hasA(s, "SRC ="))
      for (unsigned int i = 0; i < fsource_files.size(); i++)
        tmp.push_back(code[1 + i]);
    else if (hasA(s, "OBJS=") || hasA(s, "OBJS ="))
      for (unsigned int i = 0; i < fsource_files.size(); i++)
        tmp.push_back(code[1 + i + fsource_files.size()]);
  }
  input.close();
  ofstream output;
  output.open(algorithmWriter::fmakefile.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output.close();
  ofstream backUp;
  backUp.open((algorithmWriter::fmakefile + ".bck").data());
  for (unsigned int i = 0; i < backup.size(); i++)
    backUp << backup[i].data() << endl;
  backUp.close();
}
void algorithmWriter::insertToIOMakeFile()
{
  cout << "insert to io makefile " << algorithmWriter::fiomakefile.data() << endl;
  if (ioMakeAvail())
    return;
  vector<string> tmp, backup;
  vector<string> code = iomakeFileCall();
  string s;
  char c;
  ifstream input;
  input.open(algorithmWriter::fiomakefile.data());
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
    else if (hasA(s, "MOCSRC=") || hasA(s, "MOCSRC ="))
      tmp.push_back(code[3]);
    else if (hasA(s, "MOCOBJ=") | hasA(s, "MOCOBJ ="))
      tmp.push_back(code[4]);
    else if (hasA(s, "SRC=") || hasA(s, "SRC ="))
      tmp.push_back(code[1]);
    else if (hasA(s, "OBJS=") || hasA(s, "OBJS ="))
      tmp.push_back(code[2]);
  }
  input.close();
  ofstream output;
  output.open(algorithmWriter::fiomakefile.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output.close();
  ofstream backUp;
  backUp.open((algorithmWriter::fiomakefile + ".bck").data());
  for (unsigned int i = 0; i < backup.size(); i++)
    backUp << backup[i].data() << endl;
  backUp.close();
}
void algorithmWriter::insertToHeader()
{
  if (headerAvail())
    return;
  vector<string> tmp;
  vector<string> code = headerCall();
  string s;
  char c;
  ifstream input;
  input.open(algorithmWriter::fheader.data());
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
  output.open(algorithmWriter::fheader.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output << code[0].data() << endl;
  output.close();
}
void algorithmWriter::insertToIOHeader()
{
  cout << "insert to io header " << algorithmWriter::fw_header.data() << endl;
  if (ioHeaderAvail())
    return;
  vector<string> tmp;
  vector<string> code = ioHeaderCall();
  string s;
  char c;
  ifstream input;
  input.open(algorithmWriter::fw_header.data());
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
  output.open(algorithmWriter::fw_header.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output << code[0].data() << endl;
  output.close();
}
void algorithmWriter::insertToIO()
{
  cout << "insert to io " << algorithmWriter::fiwidget.data() << " "
       << algorithmWriter::fowidget.data() << endl;
  if (ioAvail())
    return;
  vector<string> tmp;
  vector<string> code = ioCall();
  string s;
  char c;
  ifstream input;
  if (finput_algo)
    input.open(algorithmWriter::fiwidget.data());
  else
    input.open(algorithmWriter::fowidget.data());
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
  if (finput_algo)
    output.open(algorithmWriter::fiwidget.data());
  else
    output.open(algorithmWriter::fowidget.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  for (unsigned int i = 0; i < code.size(); i++)
    output << code[i].data() << endl;
  output.close();
}
void algorithmWriter::insertToAnalyser()
{
  if (analyserAvail())
    return;
  vector<string> tmp, backup;
  string s;
  string pre = "    ";
  char c;
  if (fconstructor_call.size() == 0)
    setConstructorCall(constrCall());
  ifstream input;
  input.open(algorithmWriter::fanalyser.data());
  int function = 0;
  string s1 = "", s2 = "";
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    backup.push_back(s);
    if ((hasA(s1, "case ") || hasA(s2, "case ")) && hasA(s, "}")) {
      switch (function) {
      case 1: // count
        tmp.push_back(pre + "case " + string_number(fid) + ": numf=numf+" + getFrequency() +
                      ";break;//" + fdefinition.Name());
        function = 0;
        break;
      case 2: // parameter
        tmp.push_back(pre + "case " + string_number(fid) + ": return " + fdefinition.Name() +
                      "::getDescription();");
        function = 0;
        break;
      case 3: // definition
        tmp.push_back(pre + "case " + string_number(fid) + ": ");
        tmp.push_back(pre + "  {");
        for (unsigned int i = 0; i < fconstructor_call.size(); i++)
          tmp.push_back(pre + "    " + fconstructor_call[i]);
        tmp.push_back(pre + "    break;");
        tmp.push_back(pre + "  }");
        function = 0;
        break;
      }
    } else if (hasA(s, "return nAlgos;") && function == 3) {
      tmp.pop_back();
      tmp.push_back(pre + "case " + string_number(fid) + ": ");
      tmp.push_back(pre + "  {");
      for (unsigned int i = 0; i < fconstructor_call.size(); i++)
        tmp.push_back(pre + "    " + fconstructor_call[i]);
      tmp.push_back(pre + "    break;");
      tmp.push_back(pre + "  }");
      tmp.push_back(pre + "}");
      function = 0;
    } else if (hasA(s, "void tofAnalysis::initAlgorithms("))
      function = 1;
    else if (hasA(s, "algorithm_parameter tofAnalysis::getAlgorithmParameter("))
      function = 2;
    else if (hasA(s, "int tofAnalysis::getAlgorithm("))
      function = 3;
    tmp.push_back(s);
    s2 = s1;
    s1 = s;
  }
  input.close();
  ofstream output;
  output.open(algorithmWriter::fanalyser.data());
  for (unsigned int i = 0; i < tmp.size(); i++)
    output << tmp[i].data() << endl;
  output.close();
  ofstream backUp;
  backUp.open((algorithmWriter::fanalyser + ".bck").data());
  for (unsigned int i = 0; i < backup.size(); i++)
    backUp << backup[i].data() << endl;
  backUp.close();
}

bool algorithmWriter::analyserAvail() const
{
  string search = fdefinition.Name();
  ifstream input;
  string s;
  char c;
  input.open(algorithmWriter::fanalyser.data());
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
bool algorithmWriter::ioAvail() const
{
  string search = fwidget_name;
  string s;
  char c;
  ifstream input;
  if (finput_algo)
    input.open(algorithmWriter::fiwidget.data());
  else
    input.open(algorithmWriter::fowidget.data());
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
bool algorithmWriter::headerAvail() const
{
  string search = fheader_file;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(algorithmWriter::fheader.data());
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
bool algorithmWriter::ioHeaderAvail() const
{
  string search = fwidget_header;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(algorithmWriter::fw_header.data());
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
bool algorithmWriter::makeAvail() const
{
  string search = fheader_file;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(algorithmWriter::fmakefile.data());
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
bool algorithmWriter::ioMakeAvail() const
{
  string search = fwidget_header;
  string s;
  char c;
  if (search.find("/") < search.npos)
    search = search.substr(search.rfind("/") + 1, search.length() - search.rfind("/") - 1);
  ifstream input;
  input.open(algorithmWriter::fiomakefile.data());
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

int algorithmWriter::getNextID()
{
  int newid = 0, n;
  ifstream input;
  string s;
  char c;
  input.open(algorithmWriter::fanalyser.data());
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    if (hasA(s, "case ")) {
      n = toInt(s.substr(s.find("case ") + 5, s.find(":") - s.find("case ") - 5));
      if (n > newid)
        newid = n;
    }
  }
  input.close();
  return newid + 1;
}
int algorithmWriter::getInstalledID()
{
  if (!analyserAvail())
    return -1;
  ifstream input;
  string s;
  char c;
  string search = fdefinition.Name();
  input.open(algorithmWriter::fanalyser.data());
  int n;
  while (!input.eof()) {
    s = "";
    input.get(c);
    while (c != '\n' && !input.eof()) {
      s = s + c;
      input.get(c);
    }
    if (hasA(s, "case ")) {
      n = toInt(s.substr(s.find("case ") + 5, s.find(":") - s.find("case ") - 5));
    }
    if (hasA(s, search)) {
      input.close();
      return n;
    }
  }
  input.close();
  return -1;
}

ostream& operator<<(ostream& o, const algorithmWriter& wr)
{
  o << "###### algorithm name ########" << endl;
  o << wr.parameter().getName().data() << endl; // GUI algorithm name
  o << "###### description ############" << endl;
  o << wr.parameter() << "=" << endl;
  o << "###### logging output on init #####" << endl;
  o << wr.commentOnInit().data() << endl; // anaLog output on init
  o << "###### class definition: file and position in file ############" << endl;
  string sHeader = wr.headerFile();
  if (sHeader.find("/") < sHeader.npos)
    sHeader = sHeader.substr(sHeader.rfind("/") + 1, sHeader.length() - sHeader.rfind("/") - 1);
  o << sHeader.data() << endl; // header file name
  o << wr.classInHeader() << " " << wr.needsMOC() << " " << wr.versionDependentHeader() << " "
    << wr.copyOrLink() << endl; // Class name of algorithm, index of class in header file
  string sSource;
  for (int i = 0; i < wr.nSourceFiles(); i++) {
    sSource = wr.sourceFiles(i);
    if (sSource.find("/") < sSource.npos)
      sSource = sSource.substr(sSource.rfind("/") + 1, sSource.length() - sSource.rfind("/") - 1);
    o << sSource.data() << endl;
  }
  o << "###### category sorting in GUI #########################" << endl;
  o << 0 << " " << wr.parameter().getCategory() << " " << 0 << endl;
  o << "###### call frequency and connections ########" << endl;
  o << wr.frequency() << " " << wr.frequencyString() << " " << wr.nunCall() << " "
    << wr.nConnections();
  for (int i = 0; i < wr.nConnections(); i++)
    o << wr.connections(i * 4).data() << endl
      << wr.connections(i * 4 + 1).data() << endl
      << wr.connections(i * 4 + 2).data() << endl
      << wr.connections(i * 4 + 3).data() << endl;
  if (wr.nConnections() == 0)
    o << endl;
  o << "###### constructor call variables ############" << endl;
  for (int i = 0; i < wr.definition().Constructor(0).nParameters(); i++)
    o << wr.callParameter(i).data() << endl;
  o << "###### special constructor call ##############" << endl;
  o << (wr.constructorCall().size() > 0 ? 1 : 0) << endl; // use special constructor call
  if (wr.constructorCall().size() > 0)                    // constructor call
  {
    o << wr.constructorCall().size() << endl;
    for (unsigned int i = 0; i < wr.constructorCall().size(); i++)
      o << wr.constructorCall().at(i).data() << endl;
  }
  o << "###### IO-algorithm ##########################" << endl;
  if (wr.isIo()) {
    o << 1 << " " << wr.inputAlgo() << " " << wr.ioVersionDependentHeader() << " ";
    sHeader = wr.widgetHeader();
    if (sHeader.find("/") < sHeader.npos)
      sHeader = sHeader.substr(sHeader.rfind("/") + 1, sHeader.length() - sHeader.rfind("/") - 1);
    o << sHeader.data() << endl; // widget header file name
    sHeader = wr.widgetSource();
    if (sHeader.find("/") < sHeader.npos)
      sHeader = sHeader.substr(sHeader.rfind("/") + 1, sHeader.length() - sHeader.rfind("/") - 1);
    o << sHeader.data() << endl; // widget source file name
    o << wr.widgetName().data() << endl;
  } else
    o << 0 << endl;
  o << "###### finished ##########################" << endl;
  return o;
}
istream& operator>>(istream& o, algorithmWriter& wr)
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
  algorithm_parameter pa;
  o >> pa;
  wr.setParameter(pa);
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
  s = "";
  o.get(c);
  while (!(c == '\n' || o.eof())) {
    s = s + c;
    o.get(c);
  }
  wr.setCommentOnInit(s);
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
  o >> n[0] >> n[1] >> n[2] >> n[3];
  wr.setClassInHeader(n[0]);
  wr.setNeedsMOC(n[1] == 1);
  wr.setVersionDependentHeader(n[2] == 1);
  wr.setCopyOrLink(n[3]);
  if (o.eof())
    return o;
  o.get(c);
  while (!(c == '\n' || o.eof()))
    o.get(c);
  if (o.eof())
    return o;
  o.get(c);
  wr.clearSourceFiles();
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
  o >> n[0] >> n[1] >> n[2];
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
  o >> n[0] >> n[1] >> n[2] >> n[3];
  wr.setFrequency(n[0]);
  wr.setFrequencyString(n[1]);
  wr.setNunCall(n[2]);
  wr.clearConnections();
  for (int i = 0; i < n[3]; i++) {
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    if (o.eof())
      return o;
    string s1 = s;
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    if (o.eof())
      return o;
    string s2 = s;
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    if (o.eof())
      return o;
    string s3 = s;
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    if (o.eof()) {
      return o;
    }
    wr.addConnection(s1, s2, s3, s);
  }
  if (n[3] == 0) {
    o.get(c);
    while (!(c == '\n' || o.eof()))
      o.get(c);
    if (o.eof())
      return o;
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
  o >> n[0];
  wr.setIo(n[0] == 1);
  if (wr.isIo()) {
    o >> n[1] >> n[2];
    o.get(c);
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    wr.setWidgetHeader(s);
    if (o.eof())
      return o;
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    wr.setWidgetSource(s);
    if (o.eof())
      return o;
    s = "";
    o.get(c);
    while (!(c == '\n' || o.eof())) {
      s = s + c;
      o.get(c);
    }
    wr.setWidgetName(s);
    if (o.eof())
      return o;
  }
  return o;
}
