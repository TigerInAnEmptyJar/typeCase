#ifndef ALGORITHMWRITER_H
#define ALGORITHMWRITER_H
#include "algorithmparameter.h"
#include "tmyclass.h"
#include <iostream>
#include <vector>
using namespace std;

/*!
 * \brief The algorithmWriter class
 * The algorithmWriter class was written to make the installation process of new algorithms a lot
 * easier. Instead of editing all necessary files youself you can supply some principle information
 * about the algorithm, its header- and code-files and the way it should be treated at
 * initialisation. The actual installation is then done by this class. The iostream operators give
 * the possibility to save the installation parameters and reload them at a later time (possibly
 * also by someone else). There exists a graphical front-end, the AlgorithmWizard.
 */
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
  /*!
   * \brief algorithmWriter
   * Default constructor.
   */
  algorithmWriter();

  /*!
   * \brief Destructor
   */
  ~algorithmWriter();

  /*!
   * \brief init
   * Static function. Sets the important files to modify like makefiles (algorithm-package and
   * IO-algorithm-widgets), header-files (for all algorithms and all IO-widgets), code-files for
   * Input-widgets, Output-widgets and
   * \param makefileIn
   * \param iomakefileIn
   * \param headerIn
   * \param widgetHeaderIn
   * \param iwidgetIn
   * \param owidgetIn
   * \param analyserIn
   */
  static void init(string makefileIn, string iomakefileIn, string headerIn, string widgetHeaderIn,
                   string iwidgetIn, string owidgetIn, string analyserIn);

  /*!
   * \brief makefile
   * Static inline function. Returns the makefile for the algorithm-package.
   * \return
   */
  static string makefile() { return fmakefile; }

  /*!
   * \brief header
   * Static inline function. Returns the file where the include commands of the algorithms are
   * written.
   * \return
   */
  static string header() { return fheader; }

  /*!
   * \brief iwidget
   * Static inline function. Returns the file where the allocation-commands for the
   * Input-algorithm-widgets are written.
   * \return
   */
  static string iwidget() { return fiwidget; }

  /*!
   * \brief owidget
   * Static inline function. Returns the file where the allocation-commands for the
   * Output-algorithm-widgets are written.
   * \return
   */
  static string owidget() { return fowidget; }

  /*!
   * \brief analyser
   * Static inline function. Returns the filename of the file where the code of the
   * tofAnalysis::getAlgorithm(...) function is written.
   * \return
   */
  static string analyser() { return fanalyser; }

  /*!
   * \brief IOmakefile
   * Static inline function. Returns the makefile for the IO-algrithm-widgets.
   * \return
   */
  static string IOmakefile() { return fiomakefile; }

  /*!
   * \brief IOwidgetHeader
   * Static inline function. Returns the file with the widgets include-commands.
   * \return
   */
  static string IOwidgetHeader() { return fw_header; }

  /*!
   * \brief ID
   * Inline function. Returns the ID of the algorithm in question in the analysis. Can be set using
   * getNextID() or getInstalledID().
   * \return
   */
  int ID() const { return fid; }

  /*!
   * \brief headerFile
   * Inline function. Returns the header file for the algorithm in question.
   * \return
   */
  string headerFile() const { return fheader_file; }

  /*!
   * \brief definition
   * Inline function. Returns the class-description-object for the algorithm in question.
   * \return
   */
  TMyClass definition() const { return fdefinition; }

  /*!
   * \brief classInHeader
   * Inline function. Returns the position of the class in the header-file.
   * \return
   */
  int classInHeader() const { return fclass_in_header; }

  /*!
   * \brief sourceFiles
   * Inline function. Returns the numth source file in the list.
   * \param num
   * \return
   */
  string sourceFiles(int num) const
  {
    if (num < 0 || num > (int)fsource_files.size())
      return "";
    return fsource_files[num];
  }

  /*!
   * \brief nSourceFiles
   * Inline function. Returns the number of source files for the algorithm.
   * \return
   */
  int nSourceFiles() const { return fsource_files.size(); }

  /*!
   * \brief parameter
   * Inline function. Returns the description of the algorithm in question to be used for the
   * analysis.
   * \return
   */
  algorithm_parameter parameter() const { return fparameter; }

  /*!
   * \brief needsMOC
   * Inline function. Returns true if the algorithm in question contains the Q_OBJECT-makro, which
   * is necessary to handle the algorithms signals and slots properly (only if it defines any). See
   * Qt-Signal-and-Slot-mechanism
   * \return
   */
  bool needsMOC() const { return fneeds_moc; }

  /*!
   * \brief copyOrLink
   * Inline function. Returns the action to be taken at the installation process with the header and
code files.
Value	Action
0	none
1	copy files to $(KTOFPACK)/algorithm/include and $(KTOFPACK)/algorithm/src
2	link files to $(KTOFPACK)/algorithm/include and $(KTOFPACK)/algorithm/src
   * \return
   */
  int copyOrLink() const { return fcopy_or_link; }

  /*!
   * \brief isIo
   * Inline function. Returns true if the algorithm in question is an IO-algorithm.
   * \return
   */
  bool isIo() const { return fis_io; }

  /*!
   * \brief inputAlgo
   * Inline function. Returns true if the IO-algorithm is an Input-algorithm, false if it is an
   * Output-algorithm.
   * \return
   */
  bool inputAlgo() const { return finput_algo; }

  /*!
   * \brief widgetHeader
   * Inline function. If the algorithm in question is an IO-algorithm it returns the filename of the
   * header.
   * \return
   */
  string widgetHeader() const { return fwidget_header; }

  /*!
   * \brief widgetName
   * Inline function. If the algorithm in question is an IO-algorithm it returns the name of the
   * IO-widget class.
   * \return
   */
  string widgetName() const { return fwidget_name; }

  /*!
   * \brief widgetSource
   * Inline function. If the algorithm in question is an IO-algorithm it returns the source file for
   * the IO-widget.
   * \return
   */
  string widgetSource() const { return fwidget_source; }

  /*!
   * \brief versionDependentHeader
   * Inline function. Returns wether the header file of the algorithm in question contains any
   * Qt-version-dependent signals or slots. This is necessary since the Qt-meta-object-compiler
   * doesn't understand preprocessor-commands.
   * \return
   */
  bool versionDependentHeader() const { return fversion_dependent_header; }

  /*!
   * \brief ioVersionDependentHeader
   * Inline function. If the algorithm in question is an IO-algorithm it returns wether the
   * header-file of the widget contains Qt-version-dependent signals or slots. This is necessary
   * since the Qt-meta-object-compiler doesn't understand preprocessor commands.
   * \return
   */
  bool ioVersionDependentHeader() const { return fioversion_dependent_header; }

  /*!
   * \brief frequency
   * Inline function. Returns the frequency-variable see setFrequency().
   * \return
   */
  int frequency() const { return ffrequency; }

  /*!
   * \brief frequencyString
   * line function. Returns the frequencyString variable see setFrequencyString().
   * \return
   */
  int frequencyString() const
  {
    return ffrequency_string;
    ;
  }

  /*!
   * \brief nunCall
   * Inline function. Returns the number of algorithms that are allocated during the init-process
   * but not to be called during the event-loop (e.g. fit-algorithms).
   * \return
   */
  int nunCall() const { return fnuncall; }

  /*!
   * \brief connections
   * Returns the numth part of the connections list. This is the (num/4)th connection and the
num%4
0	sender
1	signal
2	receiver
3	slot
   * \param num
   * \return
   */
  string connections(int num) const
  {
    if (num < 0 || num > (int)fconnections.size())
      return "";
    return fconnections[num];
  }

  /*!
   * \brief nConnections
   * Inline function. Returns the number of connections for the algorithm in question. See
   * Qt-Signal-and-Slot-mechanism
   * \return
   */
  int nConnections() const { return fconnections.size() / 4; }

  /*!
   * \brief callParameter
   * Inline function. Returns the numth parameter from the constructor-call.
   * \param num
   * \return
   */
  string callParameter(int num) const
  {
    if (num < 0 || num > (int)fcall_parameter.size())
      return "";
    return fcall_parameter[num];
  }

  /*!
   * \brief commentOnInit
   * Inline function. Returns the comment that is written to the log-file analysisLog just before
   * the beginning of the initialisation if the algorithm in question.
   * \return
   */
  string commentOnInit() const { return fcomment_on_init; }

  /*!
   * \brief constructorCall
   * Inline function. Returns the constructor call, that is made in tofAnalysis::getAlgorithm(...).
   * \return
   */
  vector<string> constructorCall() const { return fconstructor_call; }

  /*!
   * \brief setID
   * Inline function. Sets the ID of the algorithm in question.
   * \param n
   */
  void setID(int n) { fid = n; }

  /*!
   * \brief setHeaderFile
   * Inline function. Sets the header file of the algorithm in question.
   * \param s
   */
  void setHeaderFile(string s) { fheader_file = s; }

  /*!
   * \brief setDefinition
   * Inline function. Sets the class-description of the algorithm in questuion.
   * \param t
   */
  void setDefinition(const TMyClass& t) { fdefinition = t; }

  /*!
   * \brief setClassInHeader
   * Inline function. Sets the position of the class in the header-file.
   * \param n
   */
  void setClassInHeader(int n) { fclass_in_header = n; }

  /*!
   * \brief addSourceFiles
   * Adds a file fn to the list of source files.
   * \param fn
   */
  void addSourceFiles(string fn) { fsource_files.push_back(fn); }

  /*!
   * \brief setSourceFiles
   * Sets the numth source file to fn.
   * \param num
   * \param fn
   */
  void setSourceFiles(int num, string fn)
  {
    if (num < 0 || num >= (int)fsource_files.size())
      return;
    fsource_files[num] = fn;
  }

  /*!
   * \brief clearSourceFiles
   * Inline function. Removes all source files.
   */
  void clearSourceFiles()
  {
    while (!fsource_files.empty())
      fsource_files.pop_back();
  }

  /*!
   * \brief removeSourceFile
   * Inline function. Removes source file fn from the list.
   * \param fn
   */
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

  /*!
   * \brief setParameter
   * Inline function. Sets the parameter for the algorithm in question. The ID-property will be set
   * in the course of installation.
   * \param p
   */
  void setParameter(const algorithm_parameter& p) { fparameter = p; }

  /*!
   * \brief setCopyOrLink
   * Inline function. Sets the action to be taken for the code and header files at installation.
Value	Action
0	none
1	copy files to $(KTOFPACK)/algorithm/include and $(KTOFPACK)/algorithm/src
2	link files to $(KTOFPACK)/algorithm/include and $(KTOFPACK)/algorithm/src

   * \param n
   */
  void setCopyOrLink(int n) { fcopy_or_link = n; }

  /*!
   * \brief setNeedsMOC
   * Inline function. Sets wether the algorithm in question contains the Q_OBJECT-makro which is
   * necessary to properly include signals and slots for this algorithm.
   * \param v
   */
  void setNeedsMOC(bool v) { fneeds_moc = v; }

  /*!
   * \brief setIo
   * Inline function. Sets wether the algorithm in question is an IO-algorithm.
   * \param v
   */
  void setIo(bool v) { fis_io = v; }

  /*!
   * \brief setInputAlgo
   * Inline function. Sets wether the algorithm in question is an Input-algorithm (false means
   * output-algorithm).
   * \param v
   */
  void setInputAlgo(bool v) { finput_algo = v; }

  /*!
   * \brief setWidgetHeader
   * Inline function. If the algorithm in question is an IO-algorithm it sets the header file for
   * the IO-widget.
   * \param s
   */
  void setWidgetHeader(string s) { fwidget_header = s; }

  /*!
   * \brief setWidgetName
   * Inline function. If the algorithm in question is an IO-algorithm it sets the name of the
   * widget-class for this algorithm.
   * \param s
   */
  void setWidgetName(string s) { fwidget_name = s; }

  /*!
   * \brief setWidgetSource
   * Inline function. If the algorithm in question is an IO-algorithm it sets the source file for
   * the widget.
   * \param s
   */
  void setWidgetSource(string s) { fwidget_source = s; }

  /*!
   * \brief setVersionDependentHeader
   * Inline function. Sets wether the header file of the algorithm in question has Qt-version
   * dependent signals or slots. This is necessary since the Qt-meta-object-compiler doesn't
   * understand preprocessor commands.
   * \param v
   */
  void setVersionDependentHeader(bool v) { fversion_dependent_header = v; }

  /*!
   * \brief setIoVersionDependentHeader
   * Inline function. if the algorithm in question is an IO-algorithm it sets wether the header-file
   * has signals or slots that depend on the Qt-version. This is necessary since the
   * meta-object-compiler of Qt doesn't understand preprocessor-commands.
   * \param v
   */
  void setIoVersionDependentHeader(bool v) { fioversion_dependent_header = v; }

  /*!
   * \brief setFrequency
   * Inline function. Sets the n number of call types for the algorithm:
Value	meaning
0	1
1	once per track
2	once per detector
3	see frequencyString
   * \param n
   */
  void setFrequency(int n) { ffrequency = n; }

  /*!
   * \brief setFrequencyString
   * Inline function. Sets the frequencyString-variable. This variable is only important if the
   * number of algorithm-calls per event depends on the size of a vector-variable in the parameter.
   * Say the parameter has ni integer-vectors, nf float-vectors and ns string-vectors and the size
   * of the mth string-vector (m< ns) then the frequencyString would be fs = ni+nf+m
   * \param n
   */
  void setFrequencyString(int n) { ffrequency_string = n; }

  /*!
   * \brief setNunCall
   * Inline function. Sets the number of algorithms that are allocated but not called by the
   * event-loop, such like fit-algorithms.
   * \param n
   */
  void setNunCall(int n) { fnuncall = n; }

  /*!
   * \brief addConnection
   * Adds a connection of the SIGNAL-and-SLOT type of Qt. Where the sender emits a signal, that is
   * caught by the receiver by its slot. Take care, that the parameter-list of the slot matches the
   * beginning of the parameter-list of signal.
   * \param sender
   * \param signal
   * \param receiver
   * \param slot
   */
  void addConnection(string sender, string signal, string receiver, string slot)
  {
    fconnections.push_back(sender);
    fconnections.push_back(signal);
    fconnections.push_back(receiver);
    fconnections.push_back(slot);
  }

  /*!
   * \brief clearConnections
   * Inline function. Clears the connections-vector.
   */
  void clearConnections()
  {
    while (!fconnections.empty())
      fconnections.pop_back();
  }

  /*!
   * \brief clearCallParameter
   * Inline function. Clears the call-parameter-vector for the constructor-call in getAlgorithm().
   */
  void clearCallParameter()
  {
    while (!fcall_parameter.empty())
      fcall_parameter.pop_back();
  }

  /*!
   * \brief setCallParameter
   * Sets the numth parameter of the constructor-call to param.
   * \param num
   * \param param
   */
  void setCallParameter(int num, string param)
  {
    if (num < 0)
      return;
    while (num > (int)fcall_parameter.size() - 1)
      fcall_parameter.push_back("");
    fcall_parameter[num] = param;
  }

  /*!
   * \brief setCommentOnInit
   * inline function. Sets the comment that is written to the log-file analysisLog on the start of
   * the initialisation of the algorithm in question. Use this, it is good for debugging purposes.
   * \param s
   */
  void setCommentOnInit(string s) { fcomment_on_init = s; }

  /*!
   * \brief setConstructorCall
   * inline function. Sets the call of the constructor of the algorithm in question in the function
   * getAlgorithm(...).
   * \param s
   */
  void setConstructorCall(vector<string> s) { fconstructor_call = s; }

  /*!
   * \brief makeFileCall
   * If the algorithm in question is an IO-algorithm it generates and returns the commands to insert
   * into the IO-widget-makefile. These are the addition to the lists $(HDR), $(SRC), $(OBJ) and
   * $(MOCOBJ).
   * \return
   */
  vector<string> makeFileCall();

  /*!
   * \brief iomakeFileCall
   * It generates and returns the include-command of the algorithm in question.
   * \return
   */
  vector<string> iomakeFileCall();

  /*!
   * \brief headerCall
   * It generates and returns the include-command of the algorithm in question.
   * \return
   */
  vector<string> headerCall();

  /*!
   * \brief ioHeaderCall
   * If the algorithm in question is an IO-algorithm, it generates and returns the include-command
   * of the IO-widget of the algorithm in question.
   * \return
   */
  vector<string> ioHeaderCall();

  /*!
   * \brief ioCall
   * If the algorithm in question is an IO-algorithm, it generates and returns the defaut allocation
   * commands for the IO-widget of the algorithm in question, inserting it into typeCase.
   * \return
   */
  vector<string> ioCall();

  /*!
   * \brief constrCall
   * Returns a standard version of the constructor call for the algorithm in question. The algorithm
   * is allocated once, connections are made. To have a more elaborate constructor call, use
   * setConstructorCall.
   * \return
   */
  vector<string> constrCall();

  /*!
   * \brief getFrequency
   * Returns a string that contains the number of algorithm-objects that have to be allocated for
   * the algorithm in question depending possibly also on the parameter that was passed to the
   * init-method.
   * \return
   */
  string getFrequency();

  /*!
   * \brief insertToMakeFile
   * Opens the algorithm-makefile to insert the commands for the compilation of the algorithm in
   * question.
   */
  void insertToMakeFile();

  /*!
   * \brief insertToIOMakeFile
   * Opens the algorithms-header file and inserts the include call for the algorithm in question.
   */
  void insertToIOMakeFile();

  /*!
   * \brief insertToHeader
   * If the algorithm in question is an IO-algorithm, it opens the IO-algorithms-headerfile and
   * inserts the include-command for the widget of the algorithm in question.
   */
  void insertToHeader();

  /*!
   * \brief insertToIOHeader
   * If the algorithm in question is an IO-algorithm, it opens the IO-algorithms-headerfile and
   * inserts the include-command for the widget of the algorithm in question.
   */
  void insertToIOHeader();

  /*!
   * \brief insertToIO
   * If the algorithm in question is an IO-algorithm, it opens the file for the allocation of Input-
   * or Output-widgets and inserts the code to allocate the widget for the algorithm in question. To
   * make the widget available for typeCase, typeCase has to be recompiled.
   */
  void insertToIO();

  /*!
   * \brief insertToAnalyser
   * Opens the code-file of the class tofAnalysis containing the algorithms-definition-methods and
   * inserts the code for allocation of the algorithm in question within the analysis, along with
   * the code for retrieving the algorithm_parameter. To use the algorithm afterwards, the analyser
   * has to be recompiled.
   */
  void insertToAnalyser();

  /*!
   * \brief analyserAvail
   * Opens the code-file of the class tofAnalysis containing the algorithms-definition-methods and
   * parses the file to see wether the algorithm in question has been already installed and returns
   * the result.
   * \return
   */
  bool analyserAvail() const;

  /*!
   * \brief ioAvail
   * If the algorithm in question is an IO-algorithm it opens the definition-file of the Input- or
   * Output-widgets to parse them for the allocation of the widget for the algorithm in question.
   * Returns true if these commands are found.
   * \return
   */
  bool ioAvail() const;

  /*!
   * \brief ioMakeAvail
   * If the algorithm in question is an IO-algorithm it opens the IO-algorithms-makefile to parse
   * for the commands for the widget of the algorithm in question. Returns true if these commands
   * are found.
   * \return
   */
  bool ioMakeAvail() const;

  /*!
   * \brief ioHeaderAvail
   * If it is a IO-algorithm it opens the io-algorithms-header-file to parse for the
   * io-algorithm-header-file. It returns true if it is found.
   * \return
   */
  bool ioHeaderAvail() const;

  /*!
   * \brief headerAvail
   * Opens the algorithms-header-file, parses it to see wether the algorithm in question has been
   * declared and returns the result.
   * \return
   */
  bool headerAvail() const;

  /*!
   * \brief makeAvail
   * Parses the algorithms-makefile to check wether the algorithm in question is included into the
   * makefile and returns the result.
   * \return
   */
  bool makeAvail() const;

  /*!
   * \brief libAvail
   * Dummy function at the moment. Returns false. Should return wether the algorithm is defined in
   * the current libalgorithm.so.
   * \return
   */
  bool libAvail() const;

  /*!
   * \brief getNextID
   * Opens the code-file of the class tofAnalysis containing the algorithms-definition-methods and
   * parses the file to retrieve the next possible ID to give to a new algorithm.
   * \return
   */
  int getNextID();

  /*!
   * \brief getInstalledID
   * Opens the code-file of the class tofAnalysis containing the algorithms-definition-methods and
   * parses the file to retrieve the ID of the algorithm in question. If the algorithm is not
   * installed yet, it returns -1.
   * \return
   */
  int getInstalledID();
};
ostream& operator<<(ostream& o, const algorithmWriter& wr);
istream& operator>>(istream& o, algorithmWriter& wr);
#endif
