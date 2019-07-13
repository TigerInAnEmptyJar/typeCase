#include "beamtimeparameter.h"
#include "string_utilities.h"

#include <boost/uuid/uuid_io.hpp>
namespace {

std::string toString(run_parameter::RunType tpe)
{
  switch (tpe) {
  case run_parameter::RunType::BACKGROUND:
    return "BACKGROUND";
  case run_parameter::RunType::REGULAR:
    return "REGULAR";
  case run_parameter::RunType::ELASTIC:
    return "ELASTIC";
  case run_parameter::RunType::CALIBRATION:
    return "CALIBRATION";
  case run_parameter::RunType::OTHER:
    return "OTHER";
  }
  return "ERROR";
}
} // namespace

beamTime_parameter::beamTime_parameter(string n) { setName(n); }

bool beamTime_parameter::operator==(beamTime_parameter const& other) const
{
  return other.getName() == getName()
      //            && other.getDescription() == getDescription()
      //            && other.id() == id()
      //            && other.year == year
      //            && other.month == month
      //            && other.fileName == fileName
      //            && other.setupFile == setupFile
      //            && other.calibFileNames == calibFileNames
      ;
}

std::filesystem::path beamTime_parameter::getFileName() const { return fileName; }

void beamTime_parameter::setFileName(std::filesystem::path fn) { fileName = fn; }

std::filesystem::path beamTime_parameter::getSetupFile() const { return setupFile; }

void beamTime_parameter::setSetupFile(std::filesystem::path fn) { setupFile = fn; }

std::filesystem::path beamTime_parameter::getCalibrationFile(size_t num) const
{
  if (num >= calibFileNames.size())
    return {};
  return calibFileNames[num];
}

void beamTime_parameter::removeCalibrationFile(size_t num)
{
  if (num >= calibFileNames.size()) {
    return;
  }
  calibFileNames.erase(calibFileNames.begin() + num);
}

size_t beamTime_parameter::getNumberOfCalibrationFiles() const { return calibFileNames.size(); }
void beamTime_parameter::addCalibrationFile(std::filesystem::path filename)
{
  calibFileNames.push_back(filename);
}
void beamTime_parameter::clearCalibrationFiles() { calibFileNames.clear(); }

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

ostream& operator<<(ostream& o, const beamTime_parameter& d)
{
  o << d.getMonth() << " " << d.getYear();
  o << d.getName() << endl;
  string dis = d.getDescription();
  o << dis << endl;
  o << d.getFileName().native() << endl;
  o << d.getSetupFile().native() << endl;
  for (size_t i = 0; i < d.getNumberOfCalibrationFiles(); i++) {
    o << d.getCalibrationFile(i).native() << endl;
  }
  return o;
}

run_parameter::run_parameter(std::shared_ptr<beamTime_parameter> parent, boost::uuids::uuid pn)
    : beamTime_parameter(*parent), _parentId(pn), _parent(parent)
{
  setParent(parent);
  setParentNumber(pn);
}

run_parameter::run_parameter(string n)
{
  setStartTime(1900, 1, 1, 0, 0, 0);
  setStopTime(1900, 1, 1, 0, 0, 0);
  setName(n);
}

run_parameter::RunType run_parameter::getType() const { return _type; }

void run_parameter::setType(run_parameter::RunType t) { _type = t; }

boost::uuids::uuid run_parameter::getParentNumber() const { return _parentId; }

void run_parameter::setParentNumber(boost::uuids::uuid p) { _parentId = p; }

bool run_parameter::hasOwnSetup() const { return _hasSetup; }

void run_parameter::setOwnSetup(bool s) { _hasSetup = s; }

bool run_parameter::hasOwnCalibration() const { return _hasCalib; }

void run_parameter::setOwnCalibration(bool c) { _hasCalib = c; }

int run_parameter::getRunNumber() const { return _runNumber; }

void run_parameter::setRunNumber(int rn) { _runNumber = rn; }

void run_parameter::addFile(std::filesystem::path filename, size_t typ, long fileEvents)
{
  FileContainer container{filename, typ, fileEvents};
  _files.push_back(container);
}

void run_parameter::removeFile(size_t num)
{
  if (num >= _files.size()) {
    return;
  }
  _files.erase(_files.begin() + num);
}

void run_parameter::clearFiles() { _files.clear(); }

size_t run_parameter::getNumberOfFiles() const { return _files.size(); }

std::filesystem::path run_parameter::getFile(size_t num) const
{
  if (num >= _files.size())
    return {};
  return _files[num].file;
}

size_t run_parameter::getFileType(size_t num) const
{
  if (num >= _files.size())
    return 0;
  return _files[num].fileType;
}

long run_parameter::getFileEvents(size_t num) const
{
  if (num >= _files.size())
    return -1;
  return _files[num].events;
}

std::shared_ptr<beamTime_parameter> run_parameter::getParent() const { return _parent; }

void run_parameter::setParent(std::shared_ptr<beamTime_parameter> p) { _parent = p; }

bool run_parameter::hasAdditionalCalibration() const { return _hasAdditionalCalib; }
void run_parameter::setAdditionalCalibration(bool c) { _hasAdditionalCalib = c; }
void run_parameter::setStartTime(int year, int month, int day, int hour, int minute, int second)
{
  _startingTime.tm_sec = second;
  _startingTime.tm_min = minute;
  _startingTime.tm_hour = hour;
  _startingTime.tm_mday = day;
  _startingTime.tm_mon = month;
  _startingTime.tm_year = year - 1900;
}
void run_parameter::setStopTime(int year, int month, int day, int hour, int minute, int second)
{
  _stopingTime.tm_sec = second;
  _stopingTime.tm_min = minute;
  _stopingTime.tm_hour = hour;
  _stopingTime.tm_mday = day;
  _stopingTime.tm_mon = month;
  _stopingTime.tm_year = year - 1900;
}
void run_parameter::setStartTime(const tm& time)
{
  _startingTime.tm_sec = time.tm_sec;
  _startingTime.tm_min = time.tm_min;
  _startingTime.tm_hour = time.tm_hour;
  _startingTime.tm_mday = time.tm_mday;
  _startingTime.tm_mon = time.tm_mon;
  _startingTime.tm_year = time.tm_year;
}
void run_parameter::setStopTime(const tm& time)
{
  _stopingTime.tm_sec = time.tm_sec;
  _stopingTime.tm_min = time.tm_min;
  _stopingTime.tm_hour = time.tm_hour;
  _stopingTime.tm_mday = time.tm_mday;
  _stopingTime.tm_mon = time.tm_mon;
  _stopingTime.tm_year = time.tm_year;
}
tm run_parameter::getStartTime() const { return _startingTime; }
tm run_parameter::getStopTime() const { return _stopingTime; }
int run_parameter::getTime() const
{
  tm st = getStartTime();
  tm sp = getStopTime();
  return (int)difftime(mktime(&st), mktime(&sp));
}

ostream& operator<<(ostream& o, const run_parameter& d)
{
  o << "#========= " << d.getName().data() << " =========" << endl;
  o << toString(d.getType()) << " " << boost::uuids::to_string(d.getParentNumber()) << " "
    << d.getRunNumber() << " " << endl;
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
  o << d.getName() << endl;

  o << "D" << d.getDescription() << endl;
  for (size_t i = 0; i < d.getNumberOfFiles(); i++)
    o << "F" << d.getFileType(i) << " " << d.getFileEvents(i) << " " << d.getFile(i).native()
      << endl;
  if (d.hasOwnSetup())
    o << "S" << d.getSetupFile().native() << endl;
  if (d.hasOwnCalibration() || d.hasAdditionalCalibration()) {
    //      o<<"C"<<d.getNumberOfCalibrationFiles()<<" ";
    for (size_t i = 0; i < d.getNumberOfCalibrationFiles(); i++) {
      o << "C" << d.getCalibrationFile(i).native() << endl;
    }
  }
  o << "T" << d.getStartTime().tm_year + 1900 << " " << d.getStartTime().tm_mon << " "
    << d.getStartTime().tm_mday << " " << d.getStartTime().tm_hour << " " << d.getStartTime().tm_min
    << " " << d.getStartTime().tm_sec << endl;
  o << "P" << d.getStopTime().tm_year + 1900 << " " << d.getStopTime().tm_mon << " "
    << d.getStopTime().tm_mday << " " << d.getStopTime().tm_hour << " " << d.getStopTime().tm_min
    << " " << d.getStopTime().tm_sec << endl;
  o << "=" << endl;
  return o;
}
