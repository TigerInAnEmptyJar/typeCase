using namespace std;
#ifndef LOGGER
#define LOGGER
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "geometry.h"
#include "materialparameter.h"

#include <QtCore/QObject>
#include <fstream>
#include <iomanip>
#include <iostream>
//!
//! \brief Manipulators for the logger stream class.
//!
class logMan
{
  int id;
  int getter;

public:
  logMan(int idIn);
  int getID() const;
  logMan& operator()(int num);
  int getTer() const;
};

//!
//! \brief A loggin stream like class
//!
class logger : public QObject
{
  Q_OBJECT
private:
  ofstream* output;
  string filename;
  string name;
  int level;
  bool newLine;
  bool own;
  void doNewLine();

public:
  //    logger();
  logger(ofstream& o, string n);
  logger(string fn, string n);
  ~logger();
  string getName() const;
  void setName(string n);
  string getFilename() const;
  void newFile(string fn);
  //    logger &operator<<(const char* text);
  logger& operator<<(const logMan& man);
  logger& operator<<(const string& text);
  logger& operator<<(const char* text);
  logger& operator<<(const int& val);
  logger& operator<<(const unsigned int& val);
  logger& operator<<(const size_t& val);
  logger& operator<<(const float& val);
  logger& operator<<(const double& val);
  logger& operator<<(const void* ptr);
  logger& operator<<(const vector3D& val);
  logger& operator<<(const vector4D& val);
  logger& operator<<(const Vector& val);
  logger& operator<<(const point3D& val);
  logger& operator<<(const point& val);
  logger& operator<<(const lLine3D& val);
  logger& operator<<(const sLine3D& val);
  logger& operator<<(const plane3D& val);
  logger& operator<<(const base_parameter& p);
  logger& operator<<(const algorithm_parameter& p);
  logger& operator<<(const shape_parameter& p);
  logger& operator<<(const element_parameter& p);
  logger& operator<<(const material_parameter& p);
  logger& operator<<(const detector_parameter& p);
  logger& operator<<(const beamTime_parameter& p);
  logger& operator<<(const run_parameter& p);
  logger& operator<<(const reaction_parameter& p);
  //    logger &operator<<(const _parameter &p);
signals:
  void textAdded(const string&);
};
extern logger guiLog;
extern logger ioLog;
extern logger anaLog;
extern logger calibLog;
extern logger pixLog;
extern logger trackLog;
extern logger Vlog;

extern logMan endli;
extern logMan incD;
extern logMan decD;
extern logMan wid;
extern logMan decI;
extern logMan kOf;
#endif
