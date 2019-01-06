#ifndef SHAPEWRITER_H
#define SHAPEWRITER_H
#include "shapeparameter.h"
#include "tmyclass.h"
#include <iostream>
#include <vector>
using namespace std;

/*!
 * \brief The shapeWriter class
 * The shapeWriter class was written to make the installation process of new shapes a lot easier.
 * Instead of editing all necessary files youself you can supply some principle information about
 * the shape, its header- and code-files and the way it should be treated at initialisation. The
 * actual installation is then done by this class. The iostream operators give the possibility to
 * save the installation parameters and reload them at a later time (possibly also by someone else).
 * There exists a graphical front-end, the ShapeWizard.
 */
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
  /*!
   * \brief shapeWriter
   * Default constructor.
   */
  shapeWriter();

  /*!
   * \brief Destructor
   */
  ~shapeWriter();

  /*!
   * \brief init
   * Initializes the static member variables. The default values for the files to alter during
installation are:
property	default
makefile	$(KTOFPACK)/shapes/makefile
header	$(KTOFPACK)/shapes/include/shapes.h
getShapeFile	$(KTOFPACK)/shapes/src/getShape.cpp
   * \param makefileIn
   * \param headerIn
   * \param getShapeFileIn
   */
  static void init(string makefileIn, string headerIn, string getShapeFileIn);

  /*!
   * \brief makefile
   * Static inline function. Returns the file-name of the makefile for the shapes-package.
   * \return
   */
  static string makefile() { return fmakefile; }

  /*!
   * \brief header
   * Static inline function. Returns the general include file for the shape-package.
   * \return
   */
  static string header() { return fheader; }

  /*!
   * \brief getShapeFile
   * Static inline function. Returns the file-name of the file where the getShape()-function is
   * implemented.
   * \return
   */
  static string getShapeFile() { return fanalyser; }

  /*!
   * \brief headerFile
   * Inline function. Returns the header-file-name.
   * \return
   */
  string headerFile() const { return fheader_file; }

  /*!
   * \brief definition
   * Inline function. Returns the definition of the class and its properties.
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
   * \brief constructorInClass
   * Returns the position of the used constructor in the list of defined constructors.
   * \return
   */
  int constructorInClass() const { return fconstructor_in_class; }

  /*!
   * \brief sourceFiles
   * Returns the numth source file in the source-files-list.
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
   * Inline function. Returns the number of source files.
   * \return
   */
  int nSourceFiles() const { return fsource_files.size(); }

  /*!
   * \brief parameter
   * Inline function. Returns the description of the shape with all necessary variables defined.
   * \return
   */
  shape_parameter parameter() const { return fparameter; }

  /*!
   * \brief copyOrLink
   * Inline function. Returns the action to be taken at the installation process with the header and
code files.
Value	Action
0	none
1	copy files to $(KTOFPACK)/shape/include and $(KTOFPACK)/shape/src
2	link files to $(KTOFPACK)/shape/include and $(KTOFPACK)/shape/src
   * \return
   */
  int copyOrLink() const { return fcopy_or_link; }

  /*!
   * \brief callParameter
   * Returns the parameter passed for the numth constructor-parameter used in the
   * getShape()-function.
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
   * \brief constructorCall
   * Returns the code to insert into the getShape()-function to generate a shape of the type in
   * question out of the description.
   * \return
   */
  vector<string> constructorCall() const { return fconstructor_call; }

  /*!
   * \brief setHeaderFile
   * Inline function. Sets the header file where the class is defined.
   * \param s
   */
  void setHeaderFile(string s) { fheader_file = s; }

  /*!
   * \brief setDefinition
   * Inline function. Set the description of the class (TMyClass).
   * \param t
   */
  void setDefinition(const TMyClass& t) { fdefinition = t; }

  /*!
   * \brief setClassInHeader
   *  Inline function. Sets the position of the shape-class in its header.
   * \param n
   */
  void setClassInHeader(int n) { fclass_in_header = n; }

  /*!
   * \brief setConstructorInClass
   *
   * \param n
   */
  void setConstructorInClass(int n) { fconstructor_in_class = n; }

  /*!
   * \brief addSourceFiles
   * Inline function. Adds a source file to the source-files-list
   * \param fn
   */
  void addSourceFiles(string fn) { fsource_files.push_back(fn); }

  /*!
   * \brief setSourceFiles
   * Inline function. Sets the numth source file to be fn.
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
   * Inline function. Removes all source files from the source-files-list.
   */
  void clearSourceFiles()
  {
    while (!fsource_files.empty())
      fsource_files.pop_back();
  }

  /*!
   * \brief removeSourceFile
   * Inline function. Removes a source file from the source-files-list.
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
   * Inline function. Sets the shape-parameter to describe the shape, that has all necessary
   * variables defined to generate a shape of the type in question.
   * \param p
   */
  void setParameter(const shape_parameter& p) { fparameter = p; }

  /*!
   * \brief setCopyOrLink
   * Inline function. Sets the action to be taken at the installation process with the header and
code files.
Value	Action
0	none
1	copy files to $(KTOFPACK)/shape/include and $(KTOFPACK)/shape/src
2	link files to $(KTOFPACK)/shape/include and $(KTOFPACK)/shape/src
   * \param n
   */
  void setCopyOrLink(int n) { fcopy_or_link = n; }

  /*!
   * \brief setCallParameter
   * Inline function. Sets the numth parameter of the constructor to be param in the call in the
   * getShape-function.
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
   * \brief setConstructorCall
   * Inline function. Sets the way the constructor of the shape is called in the
   * getShape()-function.
   * \param s
   */
  void setConstructorCall(vector<string> s) { fconstructor_call = s; }

  /*!
   * \brief makeFileCall
   * Returns the commands for the make-file, that is: the header-file, the code-file(s), the
   * object-file(s).
   * \return
   */
  vector<string> makeFileCall();

  /*!
   * \brief headerCall
   * Returns the include-command for the header-file.
   * \return
   */
  vector<string> headerCall();

  /*!
   * \brief constrCall
   * Returns the code necessary to generate a shape of the type in question off a description
   * (shape_parameter).
   * \return
   */
  vector<string> constrCall();

  /*!
   * \brief insertToMakeFile
   * Inserts the necessary commands into the makefile to include the shape into the package and the
   * library. Basically this means inserting the file-names into the HDR, SRC, OBJS lists.
   */
  void insertToMakeFile();

  /*!
   * \brief insertToHeader
   * Inserts the necessary include-command into the general-shapes-header to make it available for
   * use.
   */
  void insertToHeader();

  /*!
   * \brief insertToShapes
   * Inserts the necessary constructor-call into the code of the getShape()-function.
   */
  void insertToShapes();

  /*!
   * \brief shapeAvail
   * Returns true if the shape is already contained in the library or if the shape is contained in
   * the code of the getShape()-function but not yet compiled.
   * \return
   */
  bool shapeAvail() const;

  /*!
   * \brief headerAvail
   * Parses the header-file that includes all shapes for the shape's header-file. Returns true if
   * this include command was found.
   * \return
   */
  bool headerAvail() const;

  /*!
   * \brief makeAvail
   * Parses the makefile of the shape-package to see if the shape's-files are included. Returns true
   * if this is the case.
   * \return
   */
  bool makeAvail() const;

  /*!
   * \brief libAvail
   * Checks wether the shape is already included into the library and returns true if this is the
   * case.
   * \return
   */
  bool libAvail() const;
};

/*!
 * \brief operator <<
 * \param o
 * \param wr
 * \return
 */
ostream& operator<<(ostream& o, const shapeWriter& wr);

/*!
 * \brief operator >>
 * \param o
 * \param wr
 * \return
 */
istream& operator>>(istream& o, shapeWriter& wr);
#endif
