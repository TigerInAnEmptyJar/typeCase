#ifndef __BEAMRUNPARAM
#define __BEAMRUNPARAM
#include "baseparameter.h"
#include <vector>
using namespace std;
class beamTime_parameter : public base_parameter
{
private:
  string fileName;  //!
  string setupFile; //!
  vector<string> calibFileNames;
  vector<vector<string>> calibFileNames1;
  vector<bool> hasQ;
  vector<bool> hasT;
  vector<vector<bool>> hasA;
  int month;
  int year;

public:
  beamTime_parameter(string n = string());
  virtual ~beamTime_parameter();
  string getFileName() const;
  void setFileName(string fn);
  string getSetupFile() const;
  void setSetupFile(string fn);
  string getCalibrationFile(int num) const;
  string getCalibrationFile(int num, int d) const;
  void setCalibrationFile(int num, string filename);
  int getNumberOfCalibrationFiles() const;
  void addCalibrationFile(string filename);
  void clearCalibrationFiles();
  void setCalibrationFile(int det, int typ, string fn);
  virtual void addDetectorCalibration(vector<string> fn = vector<string>());
  virtual void popDetectorCalibration();
  virtual void addTypCalibration(vector<string> fn = vector<string>());
  virtual void popTypCalibration();
  bool hasACalib(int d, int t) const;
  void setHasACalib(int d, int t, bool us = true);
  int getNumCalibDets() const;
  int getNumCalibTypes() const;
  int getNumberOfDetectors() const;
  void setNumberOfDetectors(int num);
  void setDetector(int num, bool hasqdc, bool hastdc);
  bool hasQDC(int num) const;
  bool hasTDC(int num) const;
  int getMonth() const;
  int getYear() const;
  void setData(int m, int y);
  void operator=(const beamTime_parameter& b);
};
ostream& operator<<(ostream& o, const beamTime_parameter& d);
istream& operator>>(istream& i, beamTime_parameter& d);

//#define USE_QT
#ifdef USE_QT
#include <qdatetime.h>
#else
#include <time.h>
#endif
class run_parameter : public beamTime_parameter
{
private:
  int type;
  int parent;
  bool setup;
  bool calib;
  bool addiC;
  vector<vector<bool>> enabeles;
  int runNumber;
  beamTime_parameter* par;
  vector<string> files;  //! can have more than one file to read from
  vector<int> fileTypes; //! 2^0 TADE/root, 2^1 simpe, 2^2 H, 2^3 P, 2^4 C, 2^5 T
  vector<long> events;   //! 2^0 TADE/root, 2^1 simpe, 2^2 H, 2^3 P, 2^4 C, 2^5 T
#ifdef USE_QT
  QDateTime startingTime; //!
  QDateTime stopingTime;  //!
#else
  tm startingTime; //!
  tm stopingTime;  //!
#endif
public:
  run_parameter(beamTime_parameter& parent, int pn);
  run_parameter(const run_parameter& r);
  run_parameter(string n = string());
  virtual ~run_parameter();
  int getType() const;
  void setType(int t);
  int getParentNumber() const;
  void setParentNumber(int p);
  beamTime_parameter* getParent() const;
  void setParent(beamTime_parameter* p);
  bool hasOwnSetup() const;
  void setOwnSetup(bool s = true);
  bool hasOwnCalibration() const;
  void setOwnCalibration(bool c = true);
  bool hasAdditionalCalibration() const;
  void setAdditionalCalibration(bool c = true);
  virtual void addDetectorCalibration(vector<string> fn = vector<string>());
  virtual void popDetectorCalibration();
  virtual void addTypCalibration(vector<string> fn = vector<string>());
  virtual void popTypCalibration();
  bool useOwn(int d, int t) const;
  void setUseOwn(int d, int t, bool us = true);
  int getRunNumber() const;
  void setRunNumber(int rn);
  void addFile(string filename, int type, long fileEvents = -1);
  void clearFiles();
  int getNumberOfFiles() const;
  string getFile(int num) const;
  int getFileType(int num) const;
  void setFileType(int num, int type);
  long getFileEvents(int num) const;
  void setFileEvents(int num, long fileEvents);
  void setStartTime(int year, int month, int day, int hour, int minute, int second);
  void setStopTime(int year, int month, int day, int hour, int minute, int second);
#ifdef USE_QT
  void setStartTime(const QDateTime& time);
  void setStopTime(const QDateTime& time);
  QDateTime getStartTime() const;
  QDateTime getStopTime() const;
#else
  void setStartTime(const tm& time);
  void setStopTime(const tm& time);
  tm getStartTime() const;
  tm getStopTime() const;
#endif
  int getTime() const;
  void operator=(const run_parameter& r);
};
ostream& operator<<(ostream& o, const run_parameter& d);
istream& operator>>(istream& i, run_parameter& d);

string expandPath(string inp);
string contractPath(string inp);
#endif
