#include "beamtimeparameter.h"
#include "string_utilities.h"
#include <stdlib.h>
beamTime_parameter::beamTime_parameter(string n) { setName(n); }

beamTime_parameter::~beamTime_parameter() {}

string beamTime_parameter::getFileName() const { return fileName; }

void beamTime_parameter::setFileName(string fn) { fileName = fn; }

string beamTime_parameter::getSetupFile() const { return setupFile; }

void beamTime_parameter::setSetupFile(string fn) { setupFile = fn; }

string beamTime_parameter::getCalibrationFile(int det, int typ) const
{
  if ((int)calibFileNames1.size() <= det)
    return "";
  if ((int)calibFileNames1[det].size() <= typ)
    return "";
  return (calibFileNames1[det])[typ];
}

void beamTime_parameter::setCalibrationFile(int det, int typ, string fn)
{
  if ((int)calibFileNames1.size() <= det)
    return;
  if ((int)calibFileNames1[det].size() <= typ)
    return;
  (calibFileNames1[det])[typ] = fn;
}

void beamTime_parameter::addDetectorCalibration(vector<string> fn)
{
  vector<string> tmp;
  vector<bool> t1;
  if (calibFileNames1.size() > 0) {
    int sz = fn.size();
    if (sz > (int)calibFileNames1[0].size())
      sz = calibFileNames1[0].size();
    for (int i = 0; i < sz; i++) {
      tmp.push_back(fn[i]);
      if (fn[i] == "") {
        t1.push_back(false);
      } else {
        t1.push_back(true);
      }
    }
    if (calibFileNames1[0].size() > fn.size()) {
      for (unsigned int i = sz; i < calibFileNames1[0].size(); i++) {
        t1.push_back(false);
        tmp.push_back("");
      }
    }
    calibFileNames1.push_back(tmp);
    hasA.push_back(t1);
  } else {
    for (unsigned int i = 0; i < fn.size(); i++) {
      t1.push_back(false);
    }
    hasA.push_back(t1);
    calibFileNames1.push_back(fn);
  }
}

void beamTime_parameter::popDetectorCalibration()
{
  if (calibFileNames1.size() == 0)
    return;
  calibFileNames1.pop_back();
  hasA.pop_back();
}

void beamTime_parameter::addTypCalibration(vector<string> fn)
{
  if (fn.size() <= 0) {
    for (unsigned int i = 0; i < calibFileNames1.size(); i++) {
      hasA[i].push_back(false);
      calibFileNames1[i].push_back("");
    }
  } else {
    int sz = fn.size();
    if (sz > (int)calibFileNames1.size())
      sz = calibFileNames1.size();
    for (int i = 0; i < sz; i++) {
      if (fn[i] == "") {
        hasA[i].push_back(false);
      } else {
        hasA[i].push_back(true);
      }
      calibFileNames1[i].push_back(fn[i]);
    }
    if (sz < (int)calibFileNames1.size()) {
      for (unsigned int i = sz; i < calibFileNames1.size(); i++) {
        calibFileNames1[i].push_back("");
        hasA[i].push_back(false);
      }
    }
  }
}

void beamTime_parameter::popTypCalibration()
{
  for (unsigned int i = 0; i < calibFileNames1.size(); i++) {
    calibFileNames1[i].pop_back();
    hasA[i].pop_back();
  }
}

bool beamTime_parameter::hasACalib(int d, int t) const
{
  if ((int)calibFileNames1.size() <= d)
    return false;
  if ((int)calibFileNames1[d].size() <= t)
    return false;
  return (hasA[d])[t];
}

void beamTime_parameter::setHasACalib(int d, int t, bool us)
{
  if ((int)calibFileNames1.size() <= d)
    return;
  if ((int)calibFileNames1[d].size() <= t)
    return;
  (hasA[d])[t] = us;
}

int beamTime_parameter::getNumCalibDets() const { return calibFileNames1.size(); }

int beamTime_parameter::getNumCalibTypes() const
{
  return ((calibFileNames1.size() == 0) ? 0 : calibFileNames1[0].size());
}
string beamTime_parameter::getCalibrationFile(int num) const
{
  if (num < 0 || num >= (int)calibFileNames.size())
    return "";
  return calibFileNames[num];
}
void beamTime_parameter::setCalibrationFile(int num, string filename)
{
  if (num < 0)
    return;
  if (num >= (int)calibFileNames.size())
    addCalibrationFile(filename);
  else
    calibFileNames[num] = filename;
}
int beamTime_parameter::getNumberOfCalibrationFiles() const { return calibFileNames.size(); }
void beamTime_parameter::addCalibrationFile(string filename) { calibFileNames.push_back(filename); }
void beamTime_parameter::clearCalibrationFiles()
{
  while (!calibFileNames.empty())
    calibFileNames.pop_back();
}
int beamTime_parameter::getNumberOfDetectors() const { return hasQ.size(); }
void beamTime_parameter::setNumberOfDetectors(int num)
{
  if (num < 0)
    num = 0;
  if (num > (int)hasQ.size()) {
    while ((int)hasQ.size() > num) {
      hasQ.pop_back();
      hasT.pop_back();
    }
  } else {
    while ((int)hasQ.size() < num) {
      hasQ.push_back(false);
      hasT.push_back(false);
    }
  }
}
void beamTime_parameter::setDetector(int num, bool hasqdc, bool hastdc)
{
  if (num < 0)
    return;
  if (num > (int)hasQ.size())
    setNumberOfDetectors(num);
  hasQ[num] = hasqdc;
  hasT[num] = hastdc;
}
bool beamTime_parameter::hasQDC(int num) const
{
  if (num < 0 || num > (int)hasQ.size())
    return false;
  return hasQ[num];
}
bool beamTime_parameter::hasTDC(int num) const
{
  if (num < 0 || num > (int)hasQ.size())
    return false;
  return hasT[num];
}

int beamTime_parameter::getMonth() const { return month; }

int beamTime_parameter::getYear() const { return year; }

void beamTime_parameter::setData(int m, int y)
{
  if ((m < 13) && (m > 0))
    month = m;
  if ((y > 0) && (y < 100)) {
    if (y < 50)
      year = 1900 + y;
    else
      year = 2000 + y;
  }
  if ((y > 1900) && (y < 2100))
    year = y;
}
void beamTime_parameter::operator=(const beamTime_parameter& r)
{
  setName(r.getName());
  setFileName(r.getFileName());
  setSetupFile(r.getSetupFile());
  while (getNumCalibDets() > 0)
    popDetectorCalibration();
  while (getNumCalibTypes() > 0)
    popTypCalibration();
  clearCalibrationFiles();
  for (int i = 0; i < r.getNumberOfCalibrationFiles(); i++)
    addCalibrationFile(r.getCalibrationFile(i));
  for (int i = 0; i < r.getNumCalibDets(); i++)
    addDetectorCalibration();
  for (int i = 0; i < r.getNumCalibTypes(); i++)
    addTypCalibration();
  for (int i = 0; i < r.getNumCalibDets(); i++) {
    for (int j = 0; j < r.getNumCalibTypes(); j++) {
      setCalibrationFile(i, j, r.getCalibrationFile(i, j));
      setHasACalib(i, j, r.hasACalib(i, j));
    }
  }
  setData(r.getMonth(), r.getYear());
}

string contractPath(string inp)
{
  string s = getenv("KTOFPACK");
  if (s.length() > 0)
    if (inp.find(s) < inp.npos)
      inp = replace(inp, s, "$(KTOFPACK)");
  return inp;
}
ostream& operator<<(ostream& o, const beamTime_parameter& d)
{
  o << d.getMonth() << " " << d.getYear();
  o << d.getName().data() << endl;
  vector<string> dis = d.getDescription();
  o << dis.size();
  for (unsigned int i = 0; i < dis.size(); i++)
    o << dis[i].data() << endl;
  o << contractPath(d.getFileName()).data() << endl;
  o << contractPath(d.getSetupFile()).data() << endl;
  o << d.getNumberOfCalibrationFiles() << " ";
  for (int i = 0; i < d.getNumberOfCalibrationFiles(); i++) {
    o << contractPath(d.getCalibrationFile(i)).data() << endl;
  }
  return o;
}

string expandPath(string inp)
{
  string s, s1, s2;
  while (inp.find("$(") < inp.npos) {
    s1 = "";
    if (inp.find("$(") > 0)
      s1 = inp.substr(0, inp.find("$("));
    inp = inp.substr(inp.find("$(") + 2, inp.length() - inp.find("$(") - 2);
    if (inp.find(")") < inp.npos) {
      s2 = inp.substr(inp.find(")") + 1, inp.length() - inp.find(")") - 1);
      s = inp.substr(0, inp.find(")"));
    } else {
      s2 = "";
      s = inp;
    }
    inp = s1 + getenv(s.data()) + s2;
  }
  return inp;
}
istream& operator>>(istream& i, beamTime_parameter& d)
{
  int zahl, z;
  char li[250];
  char c;
  i >> zahl;
  i >> z;
  d.setData(zahl, z);
  i.getline(li, 250);
  d.setName(string(li));
  i >> zahl;
  vector<string> dis;
  for (int I = 0; I < zahl; I++) {
    i.getline(li, 250);
    dis.push_back(string(li));
  }
  d.setDescription(dis);
  i.getline(li, 250);
  string s = expandPath(li);
  d.setFileName(s);
  i.getline(li, 250);
  s = expandPath(li);
  d.setSetupFile(s);
  i >> zahl;
  i.get(c);
  for (int I = 0; I < zahl; I++) {
    i.getline(li, 250);
    s = expandPath(li);
    d.addCalibrationFile(s);
  }
  return i;
}

run_parameter::run_parameter(beamTime_parameter& parent, int pn)
{
  setStartTime(1900, 1, 1, 0, 0, 0);
  setStopTime(1900, 1, 1, 0, 0, 0);
  setParent(&parent);
  setParentNumber(pn);
  setOwnSetup(false);
  setOwnCalibration(false);
  setAdditionalCalibration(false);
  setSetupFile(parent.getSetupFile());
  for (int i = 0; i < parent.getNumCalibDets(); i++)
    addDetectorCalibration();
  for (int i = 0; i < parent.getNumCalibTypes(); i++)
    addTypCalibration();
  for (int i = 0; i < parent.getNumCalibDets(); i++) {
    for (int j = 0; j < parent.getNumCalibTypes(); j++) {
      setCalibrationFile(i, j, parent.getCalibrationFile(i, j));
      setHasACalib(i, j, parent.hasACalib(i, j));
      setUseOwn(i, j, false);
    }
  }
}

run_parameter::run_parameter(const run_parameter& r) : beamTime_parameter(r.getName())
{
  setStartTime(1900, 1, 1, 0, 0, 0);
  setStopTime(1900, 1, 1, 0, 0, 0);
  setParent(r.getParent());
  setParentNumber(r.getParentNumber());
  setOwnSetup(r.hasOwnSetup());
  setOwnCalibration(r.hasOwnCalibration());
  setAdditionalCalibration(r.hasAdditionalCalibration());
  setSetupFile(r.getSetupFile());
  setFileName(r.getFileName());
  setRunNumber(r.getRunNumber());
  setStartTime(r.getStartTime());
  setStopTime(r.getStopTime());
  for (int i = 0; i < r.getNumCalibDets(); i++)
    addDetectorCalibration();
  for (int i = 0; i < r.getNumCalibTypes(); i++)
    addTypCalibration();
  for (int i = 0; i < r.getNumberOfFiles(); i++)
    addFile(r.getFile(i), r.getFileType(i), r.getFileEvents(i));
  for (int i = 0; i < r.getNumberOfCalibrationFiles(); i++)
    addCalibrationFile(r.getCalibrationFile(i));
  for (int i = 0; i < r.getNumCalibDets(); i++) {
    for (int j = 0; j < r.getNumCalibTypes(); j++) {
      setCalibrationFile(i, j, r.getCalibrationFile(i, j));
      setHasACalib(i, j, r.hasACalib(i, j));
      setUseOwn(i, j, r.useOwn(i, j));
    }
  }
}

run_parameter::run_parameter(string n)
{
  setStartTime(1900, 1, 1, 0, 0, 0);
  setStopTime(1900, 1, 1, 0, 0, 0);
  setName(n);
}

run_parameter::~run_parameter() {}

int run_parameter::getType() const { return type; }

void run_parameter::setType(int t) { type = t; }

int run_parameter::getParentNumber() const { return parent; }

void run_parameter::setParentNumber(int p) { parent = p; }

bool run_parameter::hasOwnSetup() const { return setup; }

void run_parameter::setOwnSetup(bool s) { setup = s; }

bool run_parameter::hasOwnCalibration() const { return calib; }

void run_parameter::setOwnCalibration(bool c) { calib = c; }

void run_parameter::addDetectorCalibration(vector<string> fn)
{
  vector<bool> t1;
  beamTime_parameter::addDetectorCalibration(fn);
  if (enabeles.size() > 0) {
    int sz = fn.size();
    if (sz > (int)enabeles[0].size())
      sz = enabeles[0].size();
    for (int i = 0; i < sz; i++) {
      if (fn[i] == "")
        t1.push_back(false);
      else
        t1.push_back(true);
    }
    if (enabeles[0].size() > fn.size()) {
      for (unsigned int i = sz; i < enabeles[0].size(); i++)
        t1.push_back(false);
    }
    enabeles.push_back(t1);
  } else {
    for (unsigned int i = 0; i < fn.size(); i++)
      t1.push_back(false);
    enabeles.push_back(t1);
  }
}

void run_parameter::popDetectorCalibration()
{
  beamTime_parameter::popDetectorCalibration();
  if (enabeles.size() == 0)
    return;
  enabeles.pop_back();
}

void run_parameter::addTypCalibration(vector<string> fn)
{
  beamTime_parameter::addTypCalibration(fn);
  if (fn.size() <= 0) {
    for (unsigned int i = 0; i < enabeles.size(); i++)
      enabeles[i].push_back(false);
  } else {
    int sz = fn.size();
    if (sz > (int)enabeles.size())
      sz = enabeles.size();
    for (int i = 0; i < sz; i++) {
      if (fn[i] == "")
        enabeles[i].push_back(false);
      else
        enabeles[i].push_back(true);
    }
    if (sz < (int)enabeles.size()) {
      for (unsigned int i = sz; i < enabeles.size(); i++)
        enabeles[i].push_back(false);
    }
  }
}

void run_parameter::popTypCalibration()
{
  for (unsigned int i = 0; i < enabeles.size(); i++)
    enabeles[i].pop_back();
}

bool run_parameter::useOwn(int d, int t) const
{
  if ((int)enabeles.size() <= d)
    return false;
  if ((int)enabeles[d].size() <= t)
    return false;
  return (enabeles[d])[t];
}

void run_parameter::setUseOwn(int d, int t, bool us)
{
  if ((int)enabeles.size() <= d)
    return;
  if ((int)enabeles[d].size() <= t)
    return;
  (enabeles[d])[t] = us;
}

int run_parameter::getRunNumber() const { return runNumber; }

void run_parameter::setRunNumber(int rn) { runNumber = rn; }

void run_parameter::addFile(string filename, int type, long fileEvents)
{
  files.push_back(filename);
  fileTypes.push_back(type);
  events.push_back(fileEvents);
}

void run_parameter::clearFiles()
{
  while (!files.empty()) {
    files.pop_back();
    fileTypes.pop_back();
    events.pop_back();
  }
}

int run_parameter::getNumberOfFiles() const { return files.size(); }

string run_parameter::getFile(int num) const
{
  if (num < 0 || num >= (int)files.size())
    return "";
  return files[num];
}
int run_parameter::getFileType(int num) const
{
  if (num < 0 || num >= (int)files.size())
    return -1;
  return fileTypes[num];
}
void run_parameter::setFileType(int num, int type)
{
  if (num < 0 || num >= (int)files.size())
    return;
  fileTypes[num] = type;
}
long run_parameter::getFileEvents(int num) const
{
  if (num < 0 || num >= (int)files.size())
    return -1;
  return events[num];
}
void run_parameter::setFileEvents(int num, long fileEvents)
{
  if (num < 0 || num >= (int)files.size())
    return;
  events[num] = fileEvents;
}

beamTime_parameter* run_parameter::getParent() const { return par; }

void run_parameter::setParent(beamTime_parameter* p) { par = p; }
bool run_parameter::hasAdditionalCalibration() const { return addiC; }
void run_parameter::setAdditionalCalibration(bool c) { addiC = c; }
void run_parameter::setStartTime(int year, int month, int day, int hour, int minute, int second)
{
#ifdef USE_QT
  startingTime.setDate(QDate(year, month, day));
  startingTime.setTime(QTime(hour, minute, second));
#else
  startingTime.tm_sec = second;
  startingTime.tm_min = minute;
  startingTime.tm_hour = hour;
  startingTime.tm_mday = day;
  startingTime.tm_mon = month;
  startingTime.tm_year = year - 1900;
#endif
}
void run_parameter::setStopTime(int year, int month, int day, int hour, int minute, int second)
{
#ifdef USE_QT
  stopingTime.setDate(QDate(year, month, day));
  stopingTime.setTime(QTime(hour, minute, second));
#else
  stopingTime.tm_sec = second;
  stopingTime.tm_min = minute;
  stopingTime.tm_hour = hour;
  stopingTime.tm_mday = day;
  stopingTime.tm_mon = month;
  stopingTime.tm_year = year - 1900;
#endif
}
#ifdef USE_QT
void run_parameter::setStartTime(const QDateTime& time)
{
  startingTime.setDate(time.date());
  startingTime.setTime(time.time());
}
void run_parameter::setStopTime(const QDateTime& time)
{
  stopingTime.setDate(time.date());
  stopingTime.setTime(time.time());
}
#else
void run_parameter::setStartTime(const tm& time)
{
  startingTime.tm_sec = time.tm_sec;
  startingTime.tm_min = time.tm_min;
  startingTime.tm_hour = time.tm_hour;
  startingTime.tm_mday = time.tm_mday;
  startingTime.tm_mon = time.tm_mon;
  startingTime.tm_year = time.tm_year;
}
void run_parameter::setStopTime(const tm& time)
{
  stopingTime.tm_sec = time.tm_sec;
  stopingTime.tm_min = time.tm_min;
  stopingTime.tm_hour = time.tm_hour;
  stopingTime.tm_mday = time.tm_mday;
  stopingTime.tm_mon = time.tm_mon;
  stopingTime.tm_year = time.tm_year;
}
#endif
#ifdef USE_QT
QDateTime run_parameter::getStartTime() const
#else
tm run_parameter::getStartTime() const
#endif
{
  return startingTime;
}
#ifdef USE_QT
QDateTime run_parameter::getStopTime() const
#else
tm run_parameter::getStopTime() const
#endif
{
  return stopingTime;
}
int run_parameter::getTime() const
{
#ifdef USE_QT
  return startingTime.secsTo(stopingTime);
#else
  tm st = getStartTime();
  tm sp = getStopTime();
  return (int)difftime(mktime(&st), mktime(&sp));
#endif
}

void run_parameter::operator=(const run_parameter& r)
{
  clearCalibrationFiles();
  for (int i = 0; i < r.getNumberOfCalibrationFiles(); i++)
    addCalibrationFile(r.getCalibrationFile(i));
  setName(r.getName());
  setDescription(r.getDescription());
  setType(r.getType());
  setParentNumber(r.getParentNumber());
  setParent(r.getParent());
  setOwnSetup(r.hasOwnSetup());
  setOwnCalibration(r.hasOwnCalibration());
  setAdditionalCalibration(r.hasAdditionalCalibration());
  setRunNumber(r.getRunNumber());
  setFileName(r.getFileName());
  setSetupFile(r.getSetupFile());
  //#ifdef USE_QT
  setStartTime(r.getStartTime());
  setStopTime(r.getStopTime());
  //#else
  //#endif
  clearFiles();
  for (int i = 0; i < r.getNumberOfFiles(); i++)
    addFile(r.getFile(i), r.getFileType(i), r.getFileEvents(i));
  while (getNumCalibDets() > 0)
    popDetectorCalibration();
  while (getNumCalibTypes() > 0)
    popTypCalibration();
  for (int i = 0; i < r.getNumCalibDets(); i++)
    addDetectorCalibration();
  for (int i = 0; i < r.getNumCalibTypes(); i++)
    addTypCalibration();
  for (int i = 0; i < r.getNumCalibDets(); i++) {
    for (int j = 0; j < r.getNumCalibTypes(); j++) {
      setCalibrationFile(i, j, r.getCalibrationFile(i, j));
      setHasACalib(i, j, r.hasACalib(i, j));
    }
  }
  setData(r.getMonth(), r.getYear());
}

ostream& operator<<(ostream& o, const run_parameter& d)
{
  o << "#========= " << d.getName().data() << " =========" << endl;
  o << d.getType() << " " << d.getParentNumber() << " " << d.getRunNumber() << " " << endl;
  if (d.hasOwnSetup())
    o << 1 << " ";
  else
    o << 0 << " ";
  if (d.hasOwnCalibration())
    o << 1 << " ";
  else
    o << 0 << " ";
  if (d.hasAdditionalCalibration())
    o << 1;
  else
    o << 0;
  o << d.getName().data() << endl;

  vector<string> dis = d.getDescription();
  //  o<<dis.size();
  for (unsigned int i = 0; i < dis.size(); i++)
    o << "D" << dis[i].data() << endl;
  //    o<<" "<<d.getNumberOfFiles()<<" ";
  for (int i = 0; i < d.getNumberOfFiles(); i++)
    o << "F" << d.getFileType(i) << " " << d.getFileEvents(i) << " "
      << contractPath(d.getFile(i)).data() << endl;
  //    o<<d.getFileName().data()<<endl;
  if (d.hasOwnSetup())
    o << "S" << contractPath(d.getSetupFile()).data() << endl;
  if (d.hasOwnCalibration() || d.hasAdditionalCalibration()) {
    //      o<<"C"<<d.getNumberOfCalibrationFiles()<<" ";
    for (int i = 0; i < d.getNumberOfCalibrationFiles(); i++) {
      o << "C" << contractPath(d.getCalibrationFile(i)).data() << endl;
    }
  }
#ifdef USE_QT
  o << "T" << d.getStartTime().date().year() << " " << d.getStartTime().date().month() << " "
    << d.getStartTime().date().day() << " " << d.getStartTime().time().hour() << " "
    << d.getStartTime().time().minute() << " " << d.getStartTime().time().second() << endl;
  o << "P" << d.getStopTime().date().year() << " " << d.getStopTime().date().month() << " "
    << d.getStopTime().date().day() << " " << d.getStopTime().time().hour() << " "
    << d.getStopTime().time().minute() << " " << d.getStopTime().time().second() << endl;
#else
  o << "T" << d.getStartTime().tm_year + 1900 << " " << d.getStartTime().tm_mon << " "
    << d.getStartTime().tm_mday << " " << d.getStartTime().tm_hour << " " << d.getStartTime().tm_min
    << " " << d.getStartTime().tm_sec << endl;
  o << "P" << d.getStopTime().tm_year + 1900 << " " << d.getStopTime().tm_mon << " "
    << d.getStopTime().tm_mday << " " << d.getStopTime().tm_hour << " " << d.getStopTime().tm_min
    << " " << d.getStopTime().tm_sec << endl;
#endif
  o << "=" << endl;
  return o;
}

istream& operator>>(istream& i, run_parameter& d)
{
  int zahl;
  char li[500];
  char c;
  i.getline(li, 500);
  i >> zahl;
  d.setType(zahl);
  i >> zahl;
  d.setParentNumber(zahl);
  i >> zahl;
  d.setRunNumber(zahl);
  i >> zahl;
  d.setOwnSetup(zahl == 1);
  i >> zahl;
  d.setOwnCalibration(zahl == 1);
  i >> zahl;
  d.setAdditionalCalibration(zahl == 1);
  i.getline(li, 250);
  d.setName(string(li));
  long zahl2;
  vector<string> dis;
  i.get(c);
  string s;
  //     cout<<d.getName().data()<<":"<<endl;
  while (!(c == '=' || c == '#')) {
    switch (c) {
    case 'C':
      i.getline(li, 500);
      s = expandPath(li);
      d.addCalibrationFile(s);
      break;
    case 'S':
      i.getline(li, 500);
      s = expandPath(li);
      d.setSetupFile(s);
      d.setOwnSetup(true);
      break;
    case 'D':
      i.getline(li, 500);
      dis.push_back(li);
      break;
    case 'F': {
      i >> zahl >> zahl2;
      i.get(c);
      i.getline(li, 500);
      s = expandPath(li);
      d.addFile(s, zahl, zahl2);
      break;
    }
    case 'T': {
      int zz[6];
      for (int ii = 0; ii < 6; ii++)
        i >> zz[ii];
      i.getline(li, 500);
      // fdef USE_QT
      d.setStartTime(zz[0], zz[1], zz[2], zz[3], zz[4], zz[5]);
      // lse
      // ndif
      break;
    }
    case 'P': {
      int zz[6];
      for (int ii = 0; ii < 6; ii++)
        i >> zz[ii];
      i.getline(li, 500);
      // fdef USE_QT
      d.setStopTime(zz[0], zz[1], zz[2], zz[3], zz[4], zz[5]);
      // lse
      //#endif
      break;
    }
    }
    i.get(c);
  }
  i.getline(li, 500);
  d.setDescription(dis);
  return i;
}
// #include <TClass.h>
// void beamTime_parameter::Streamer(TBuffer &b)
// {
//     if(b.IsWriting())
//     {
// 	beamTime_parameter::Class()->WriteBuffer(b, this);
// 	b<<fileName.data()<<setupFile.data();
//     }
//     else
//     {
// 	beamTime_parameter::Class()->ReadBuffer(b, this);
// 	char* line=0;
// 	b>>line;
// 	fileName=string(line);
// 	b>>line;
// 	setupFile=string(line);
//     }
// }
// void run_parameter::Streamer(TBuffer &b)
// {
// }
