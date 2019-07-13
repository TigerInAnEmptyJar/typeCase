#include "logger.h"
#include <stdlib.h>
// logger::logger()
//{
//    own=false;
//}
logger guiLog("GUI_log.log", "GUI log");
logger ioLog("InputOutputLog.log", "input/output");
logger anaLog("analysisLog.log", "analysis log");
logger calibLog("calibrationLog.log", "analysis log");
logger pixLog("pixelLog.log", "analysis log");
logger trackLog("trackingLog.log", "analysis log");
logger Vlog("VLog.log", "Vertex search log");

logMan endli(0);
logMan incD(1);
logMan decD(2);
logMan wid(3);
logMan decI(4);
logMan kOf(5);
logMan::logMan(int idIn) { id = idIn; }

int logMan::getID() const { return id; }

logMan& logMan::operator()(int num)
{
  getter = num;
  return *this;
}

int logMan::getTer() const { return getter; }

logger::logger(ofstream& o, string n)
{
  level = 0;
  newLine = true;
  name = n;
  output = &o;
  filename = "unknown";
  own = false;
  *output << "logfile acquired for " << name.data() << "/n";
  output->flush();
}

logger::logger(string fn, string n)
{
  level = 0;
  newLine = true;
  name = n;
  filename = fn;
  output = new ofstream(fn.data(), ios_base::app);
  own = true;
  *output << "logfile created with name " << filename.data() << " for ";
  *output << name.data() << "\n";
  output->flush();
}

logger::~logger()
{
  *output << "end logging of " << name.data() << " in file " << filename.data() << "\n";
  output->flush();
  if (own)
    delete output;
}

string logger::getName() const { return name; }

void logger::setName(string n)
{
  *output << "name of logging changed from " << name.data() << "to" << n.data() << "\n";
  output->flush();
  name = n;
}

string logger::getFilename() const { return filename; }

void logger::newFile(string fn)
{
  if (own) {
    *output << "closing logfile " << filename.data() << "\n";
    output->flush();
    output->close();
    filename = fn;
    output->open(filename.data());
    *output << "opened logfile " << filename.data() << " for " << name.data() << "\n";
    output->flush();
  } else {
    filename = fn;
    *output << "changing logfile to " << filename.data() << "\n";
    own = true;
    output = new ofstream;
    output->open(filename.data());
    *output << "opened logfile " << filename.data() << " for " << name.data() << "\n";
    output->flush();
  }
}

// logger &logger::operator<<(const char* text)
//{
//    *output<<text;
//    output->flush();
//    return *this;
//}
logger& logger::operator<<(const logMan& man)
{
  switch (man.getID()) {
  case 0:
    *output << endl;
    newLine = true;
    emit textAdded(string("\n"));
    break;
  case 1:
    level++;
    break;
  case 2:
    level--;
    *output << endl;
    break;
  case 3:
    *output << setw(man.getTer());
    break;
  case 4:
    level--;
    break;
  case 5:
    delete output;
    output = new ofstream(filename.data());
    *output << "logfile created with name " << filename.data() << " for ";
    *output << name.data() << "\n";
    output->flush();
    break;
  }
  return *this;
}

void logger::doNewLine()
{
  *output << getenv("HOSTNAME") << ":";
  for (int i = 0; i < level; i++) {
    *output << " ";
    emit textAdded(string(" "));
  }
}

logger& logger::operator<<(const string& text)
{
  //    if(!output)return *this;
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  if (text.data()[text.length()] == '\n')
    newLine = true;
  (*output) << text.data();
  output->flush();
  emit textAdded(text);
  return *this;
}
logger& logger::operator<<(const char* text)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  if (text[string(text).length()] == '\n')
    newLine = true;
  (*output) << text;
  output->flush();
  emit textAdded(string(text));
  return *this;
}

logger& logger::operator<<(const int& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << setw(8) << val;
  output->flush();
  emit textAdded(string_number(val));
  return *this;
}

logger& logger::operator<<(const unsigned int& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << setw(8) << val;
  output->flush();
  emit textAdded(string_number(val));
  return *this;
}

logger& logger::operator<<(const size_t& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << setw(8) << val;
  output->flush();
  emit textAdded(string_number(static_cast<unsigned int>(val)));
  return *this;
}

logger& logger::operator<<(const float& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << setw(8) << val;
  output->flush();
  emit textAdded(string_number(val));
  return *this;
}
logger& logger::operator<<(const double& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << setw(8) << val;
  output->flush();
  emit textAdded(string_number(val));
  return *this;
}

logger& logger::operator<<(const base_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p.getName().data();
  output->flush();
  return *this;
}

logger& logger::operator<<(const algorithm_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  (*output) << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const shape_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const element_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const material_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const detector_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const beamTime_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const run_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const reaction_parameter& p)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << p;
  output->flush();
  return *this;
}

logger& logger::operator<<(const vector3D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(" + string_number(val.X()) + ", " + string_number(val.Y()) + ", " +
                 string_number(val.Z()) + ")");
  return *this;
}

logger& logger::operator<<(const point3D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(" + string_number(val.X()) + ", " + string_number(val.Y()) + ", " +
                 string_number(val.Z()) + ")");
  return *this;
}

logger& logger::operator<<(const lLine3D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(p = (" + string_number(val.P().X()) + ", " + string_number(val.P().Y()) + ", " +
                 string_number(val.P().Z()) + "), q = (" + string_number(val.Q().X()) + ", " +
                 string_number(val.Q().Y()) + ", " + string_number(val.Q().Z()) + "))");
  return *this;
}

logger& logger::operator<<(const sLine3D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(p = (" + string_number(val.Foot().X()) + ", " + string_number(val.Foot().Y()) +
                 ", " + string_number(val.Foot().Z()) + ") + t * (" +
                 string_number(val.Direction().X()) + ", " + string_number(val.Direction().Y()) +
                 ", " + string_number(val.Direction().Z()) + "))");
  return *this;
}

logger& logger::operator<<(const plane3D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(p - (" + string_number(val.Foot().X()) + ", " + string_number(val.Foot().Y()) +
                 ", " + string_number(val.Foot().Z()) + ") ) * (" +
                 string_number(val.Normal().X()) + ", " + string_number(val.Normal().Y()) + ", " +
                 string_number(val.Normal().Z()) + "))");
  return *this;
}

logger& logger::operator<<(const vector4D& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  emit textAdded("(" + string_number(val.X()) + ", " + string_number(val.Y()) + ", " +
                 string_number(val.Z()) + ", " + string_number(val.W()) + ")");
  return *this;
}

logger& logger::operator<<(const Vector& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  string s = "( ";
  for (int i = 0; i < val.getDimension() - 1; i++)
    s = s + string_number(val.getValue(i)) + ", ";
  s = s + string_number(val.getValue(val.getDimension() - 1)) + ")";
  emit textAdded(s);
  return *this;
}

logger& logger::operator<<(const point& val)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << val;
  output->flush();
  string s = "( ";
  for (int i = 0; i < val.getDimension() - 1; i++)
    s = s + string_number(val.getValue(i)) + ", ";
  s = s + string_number(val.getValue(val.getDimension() - 1)) + ")";
  emit textAdded(s);
  return *this;
}

logger& logger::operator<<(const void* ptr)
{
  if (newLine) {
    doNewLine();
    newLine = false;
  }
  *output << ptr;
  output->flush();
  //    emit textAdded(string_number(ptr));
  return *this;
}
