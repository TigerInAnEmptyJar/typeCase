#include "CommonCalibrationParser.h"
#include "string_utilities.h"
#include <iostream>
string getLine(istream& input)
{
  char c;
  input.get(c);
  string ret = "";
  while (input.good() && !input.eof() && c != '\n') {
    ret = ret + c;
    input.get(c);
  }
  while (input.good() && !input.eof() && c == '\n')
    input.get(c);
  input.unget();
  return ret;
}
string getWord(istream& input, string separator = " ", bool all = false)
{
  string ret = "";
  string tmp = "";
  char c;
  input.get(c);
  if (all) {
    while (input.good() && !input.eof() && c != '\n' && tmp != separator) {
      ret = ret + c;
      input.get(c);
      tmp = tmp + c;
      if (tmp.length() > separator.length())
        tmp = tmp.substr(tmp.length() - separator.length(), separator.length());
    }
    if (c != '\n')
      ret = ret.substr(0, ret.length() - tmp.length());
  } else {
    while (input.good() && !input.eof() && c != '\n' && !hasA(separator, c)) {
      ret = ret + c;
      input.get(c);
    }
  }
  return ret;
}
string getWord(istream& input, char separator = ' ')
{
  string ret = "";
  char c;
  input.get(c);
  while (input.good() && !input.eof() && c != separator && c != '\n') {
    ret = ret + c;
    input.get(c);
  }
  return ret;
}
string getWord(istream& input, int n = 1)
{
  string ret = "";
  char c;
  input.get(c);
  while (input.good() && !input.eof() && c != '\n' && (int)ret.length() < n) {
    ret = ret + c;
    input.get(c);
  }
  return ret;
}
void skipWhile(istream& input, char w = ' ')
{
  char c;
  input.get(c);
  while (input.good() && !input.eof() && c != '\n' && c == w)
    input.get(c);
  input.unget();
}
void skipWhile(istream& input, string w = " ")
{
  char c;
  input.get(c);
  while (input.good() && !input.eof() && c != '\n' && hasA(w, c))
    input.get(c);
  input.unget();
}
void skipLine(istream& input)
{
  char c;
  input.get(c);
  while (input.good() && !input.eof() && c != '\n')
    input.get(c);
  while (input.good() && !input.eof() && c == '\n')
    input.get(c);
  input.unget();
}
CommonCalibrationParser::CommonCalibrationParser(int nE, int ID) : numElements(nE), DetID(ID)
{
  numParameters = 0;
  parameter = new float*[nE];
  for (int i = 0; i < numElements; i++)
    parameter[i] = new float[numParameters];
  range = new int[4];
  for (int i = 0; i < 4; i++)
    range[i] = -1;
  paramID = -1;
  valid = false;
}
CommonCalibrationParser::CommonCalibrationParser() : numElements(24), DetID(0)
{
  numParameters = 0;
  parameter = new float*[numElements];
  for (int i = 0; i < numElements; i++)
    parameter[i] = new float[numParameters];
  range = new int[4];
  for (int i = 0; i < 4; i++)
    range[i] = -1;
  paramID = -1;
  valid = false;
}

CommonCalibrationParser::~CommonCalibrationParser()
{
  for (int i = 0; i < numElements; i++)
    delete[] parameter[i];
  delete[] parameter;
  delete[] range;
}

void CommonCalibrationParser::setRanges(int fromRun, int toRun, int fromEvent, int toEvent)
{
  range[0] = fromRun;
  range[1] = toRun;
  range[2] = fromEvent;
  range[3] = toEvent;
}

bool CommonCalibrationParser::isValid() const { return valid; }
void CommonCalibrationParser::setValid(bool value) { valid = value; }
bool CommonCalibrationParser::getFromFile(ifstream& input)
{
  if (!input.good()) {
    valid = false;
    return false;
  }
  char li[1000];
  string line;
  int begin, end, accParam = -1;
  input.getline(li, 1000);
  line = li;
  bool read = true;
  while (input.good() && read) {
    begin = 0;
    while ((isSpace(line[begin]) && (begin < (int)line.length())))
      begin++;
    end = begin;
    while ((!isSpace(line[end])) && (end < (int)line.length()))
      end++;
    if ((hasA(line.substr(begin, end - begin), "fromRun")) && (accParam != -1)) {
      begin = end;
      while (!isNumber(line[begin]))
        begin++;
      end = begin;
      while (isNumber(line[end]))
        end++;
      range[0] = (toInt(line.substr(begin, end - begin)));
    }
    if ((hasA(line.substr(begin, end - begin), "toRun")) && (accParam != -1)) {
      begin = end;
      while (!isNumber(line[begin]))
        begin++;
      end = begin;
      while (isNumber(line[end]))
        end++;
      range[1] = (toInt(line.substr(begin, end - begin)));
    }
    if ((hasA(line.substr(begin, end - begin), "fromEvent")) && (accParam != -1)) {
      begin = end;
      while (!isNumber(line[begin]))
        begin++;
      end = begin;
      while (isNumber(line[end]))
        end++;
      range[2] = (toInt(line.substr(begin, end - begin)));
    }
    if ((hasA(line.substr(begin, end - begin), "toEvent")) && (accParam != -1)) {
      begin = end;
      while (!isNumber(line[begin]))
        begin++;
      end = begin;
      while (isNumber(line[end]))
        end++;
      range[3] = (toInt(line.substr(begin, end - begin)));
    }
    if ((contains(line, "-->") > 0) || (contains(line, "==>") > 0)) {
      begin = end + 1;
      while ((isSpace(line[begin]) && (begin < (int)line.length())))
        begin++;
      end = begin;
      while ((!isSpace(line[end])) && (end < (int)line.length()))
        end++;
      begin = end + 1;
      while ((isSpace(line[begin]) && (begin < (int)line.length())))
        begin++;
      end = begin;
      while ((!isSpace(line[end])) && (end < (int)line.length()))
        end++;
      if (toInt(line.substr(begin, end - begin)) != DetID) {
        accParam = -1;
      } else {
        begin = end + 1;
        while ((isSpace(line[begin]) && (begin < (int)line.length())))
          begin++;
        end = begin;
        while ((!isSpace(line[end])) && (end < (int)line.length()))
          end++;
        if (toInt(line.substr(begin, end - begin)) != numElements) {
          accParam = -1;
        } else {
          begin = end + 1;
          while ((isSpace(line[begin]) && (begin < (int)line.length())))
            begin++;
          end = begin;
          while ((!isSpace(line[end])) && (end < (int)line.length()))
            end++;
          begin = end + 1;
          while ((isSpace(line[begin]) && (begin < (int)line.length())))
            begin++;
          end = begin;
          while ((!isSpace(line[end])) && (end < (int)line.length()))
            end++;
          accParam = toInt(line.substr(begin, end - begin));
          begin = end + 1;
          while ((isSpace(line[begin]) && (begin < (int)line.length())))
            begin++;
          end = begin;
          while ((!isSpace(line[end])) && (end < (int)line.length()))
            end++;
          numParameters = toInt(line.substr(begin, end - begin));
          for (int i = 0; i < numElements; i++) {
            delete[] parameter[i];
            parameter[i] = new float[numParameters];
          }
        }
      }
    }
    if ((hasA(line.substr(begin, end - begin), "gotFromFile")) && (accParam != -1)) {
      char c = ' ', li_[1000];
      int read_ = 0;
      int elNum;
      float number;
      string word;
      while (read_ < numElements) {
        while (isSpace(c))
          input.get(c);
        if (c != '#') {
          word = "";
          word += c;
          while (!(isSpace(c))) {
            input.get(c);
            word += (c);
          }
          if (toInt(word) == DetID) {
            input >> elNum;
            if ((elNum >= 0) && (elNum < numElements)) {
              read_++;
              for (int i = 0; i < numParameters; i++) {
                input >> number;
                parameter[elNum][i] = number;
                if ((!input.good()) && (read_ > numElements)) {
                  valid = false;
                  return false;
                }
              }
            }
          }
        }
        input.getline(li_, 1000);
        if ((!input.good()) && (read_ > numElements)) {
          valid = false;
          return false;
        }
        c = ' ';
      }
      paramID = accParam;
      read = true;
      valid = true;
      // 	  if(range[0]==-1) range[0]=0;
      // 	  if(range[1]==-1) range[1]=999999;
      // 	  if(range[2]==-1) range[2]=0;
      // 	  if(range[3]==-1) range[3]=999999;
      return true;
    }
    input.getline(li, 1000);
    line = li;
  }
  valid = false;
  return false;
}

int CommonCalibrationParser::getNumParameters() const { return numParameters; }

int CommonCalibrationParser::getNumElements() const { return numElements; }

int CommonCalibrationParser::getDetector() const { return DetID; }

int CommonCalibrationParser::getRange(int num) const
{
  if ((!valid) || (num > 3))
    return -1;
  if ((num == 0 || num == 2) && range[num] == -1)
    return 0;
  if (range[num] == -1)
    return 9999999;
  return range[num];
}

float CommonCalibrationParser::getParameter(int element, int num) const
{
  if (!valid)
    return -1;
  if (element >= numElements)
    return -1;
  if (num >= numParameters)
    return -1;
  return parameter[element][num];
}

int CommonCalibrationParser::getParamID() const { return paramID; }

bool CommonCalibrationParser::operator==(const CommonCalibrationParser& pars) const
{
  if (DetID != pars.getDetector())
    return false;
  if (valid != pars.isValid())
    return false;
  if (numElements != pars.getNumElements())
    return false;
  if (numParameters != pars.getNumParameters())
    return false;
  for (int i = 0; i < 4; i++) {
    if ((i == 0 || i == 2) && range[i] == -1) {
      if (pars.getRange(i) != 0)
        return false;
    } else if (range[i] == -1) {
      if (pars.getRange(i) != 9999999)
        return false;
    } else if (range[i] != pars.getRange(i))
      return false;
  }
  //   for(int i=0;i<numElements;i++)
  //     for(int j=0;j<numParameters;j++)
  //       if(parameter[i][j]!=pars.getParameter(i,j))return false;
  return true;
}
void CommonCalibrationParser::write(ostream& output, const string& author, const string& beamTime)
{
  output << "#======================new calibration type==========================" << endl;
  output << endl;
  string det, param;
  switch (DetID) {
  case 0:
    det = "QUIRL_GERADE";
    break;
  case 1:
    det = "QUIRL_LINKS";
    break;
  case 2:
    det = "QUIRL_RECHTS";
    break;
  case 3:
    det = "RING_GERADE";
    break;
  case 4:
    det = "RING_LINKS";
    break;
  case 5:
    det = "RING_RECHTS";
    break;
  case 6:
    det = "BARREL_VORNE";
    break;
  case 7:
    det = "BARREL_HINTEN";
    break;
  case 15:
    det = "TORTE_A";
    break;
  case 16:
    det = "TORTE_B";
    break;
  case 17:
    det = "HODO_X";
    break;
  case 18:
    det = "HODO_Y";
    break;
  case 19:
    det = "LARGE_HODO_X";
    break;
  case 20:
    det = "LARGE_HODO_Y";
    break;
  case 21:
    det = "DS_MIKRO_STRIP_RADIAL";
    break;
  case 22:
    det = "DS_MIKRO_STRIP_PHI";
    break;
  case 23:
    det = "HODO_D";
    break;
  case 25:
    det = "CALORIMETER";
    break;
  }
  switch (paramID) {
  case 0:
    param = "PEDESTAL";
    break;
  case 1:
    param = "TdcFactor";
    break;
  case 2:
    param = "WALK";
    break;
  case 3:
    param = "CUTS";
    break;
  case 4:
    param = "QdcFactor";
    break;
  case 5:
    param = "TdcOffset";
    break;
  case 6:
    param = "zBarrel";
    break;
  case 7:
    param = "tdcRadialPix";
    break;
  case 8:
    param = "tdcRadialPol";
    break;
  case 9:
    param = "tdcRadialSpPix";
    break;
  case 10:
    param = "tdcRadialSpPol";
    break;
  case 11:
    param = "qdcRadialPix";
    break;
  case 12:
    param = "qdcRadialPol";
    break;
  case 13:
    param = "tdcBarrelAdd";
    break;
  }
  output << "==>               " << det.data() << " " << DetID << " " << numElements << "  "
         << param.data() << "  " << paramID << "  " << numParameters << endl;
  output << endl;
  output << "#---------------------------------------------------" << endl;
  output << "author            " << author.data() << endl;
  output << "date              after 27th of January 2005 " << endl;
  if (range[0] == -1)
    output << "fromRun           " << 0 << endl;
  else
    output << "fromRun           " << range[0] << endl;
  if (range[1] == -1)
    output << "uptoRun           " << 9999999 << endl;
  else
    output << "uptoRun           " << range[1] << endl;
  if ((range[2] != -1) && (range[3] != -1)) {
    output << "fromEvent           " << range[2] << endl;
    output << "uptoEvent           " << range[3] << endl;
  }
  output << "beamTime          " << beamTime.data() << endl;
  output << "gotFromFile       newCalDat.data" << endl;
  output << "#----------------------------------------------------" << endl;
  for (int i = 0; i < numElements; i++) {
    output << DetID << "\t" << i << "\t";
    for (int j = 0; j < numParameters; j++) {
      output.width(10);
      output << parameter[i][j] << "\t";
    }
    output << endl;
  }
}

int CommonCalibrationParser::getNumberOfCalibrationsInFile(string filename)
{
  vector<int> v1, v2;
  getCalibrationTypesInFile(filename, v1, v2);
  return v1.size();
}
int CommonCalibrationParser::getNumberOfCalibrationsInFile(string filename, int detID)
{
  vector<int> v;
  getCalibrationTypesInFile(filename, v, detID);
  return v.size();
}
int CommonCalibrationParser::getNumberOfCalibrationsInFile(string filename, int detID, int pID)
{
  vector<int> v;
  getCalibrationTypesInFile(filename, v, detID);
  int num = 0;
  for (unsigned int i = 0; i < v.size(); i++)
    if (v[i] == pID)
      num++;
  return num;
}
int CommonCalibrationParser::getCalibrationTypesInFile(string filename, vector<int>& pIDs,
                                                       int detID)
{
  vector<int> v1, v2;
  getCalibrationTypesInFile(filename, v1, v2);
  for (unsigned int i = 0; i < v1.size(); i++)
    if (v2[i] == detID)
      pIDs.push_back(v1[i]);
  return pIDs.size();
}
int CommonCalibrationParser::getCalibrationTypesInFile(string filename, vector<int>& pIDs,
                                                       vector<int>& dets)
{
  ifstream input;
  input.open(filename.data());
  char li[1000], c;
  string line;
  int n1, n2, n3, n4;
  while (input.good() && !input.eof()) {
    line = "";
    while (input.good() && !input.eof() && !(hasA(line, "==>   ") || hasA(line, "-->  "))) {
      line = "";
      for (int i = 0; i < 15; i++) {
        input.get(c);
        line = line + c;
      }
      if (!(hasA(line, "==>   ") || hasA(line, "-->    ")))
        input.getline(li, 1000);
    }
    if (!(input.good() && !input.eof()))
      return pIDs.size();
    c = ' ';
    while (c == ' ' || c == '\t')
      input.get(c);
    while (c != ' ' && c != '\t')
      input.get(c);
    input >> n1 >> n2;
    input.get(c);
    line = c;
    //      c='n';
    while (c == ' ' || c == '\t')
      input.get(c);
    while (c != ' ' && c != '\t')
      input.get(c);
    input >> n3 >> n4;
    input.getline(li, 1000);
    pIDs.push_back(n3);
    dets.push_back(n1);
  }
  input.close();
  return pIDs.size();
}
void CommonCalibrationParser::setParameter(int element, int num, float value)
{
  if (element < 0 || element >= numElements)
    return;
  if (num < 0 || num >= numParameters)
    return;
  parameter[element][num] = value;
}
void CommonCalibrationParser::setRange(int num, int value)
{
  if (!(num >= 0 && num < 4))
    return;
  // cout<<DetID<<" range set:"<<num<<" "<<value<<endl;
  range[num] = value;
}
void CommonCalibrationParser::setParamID(int value) { paramID = value; }
void CommonCalibrationParser::setNumParamsElements(int elements, int params)
{
  for (int i = 0; i < numElements; i++)
    delete[] parameter[i];
  delete[] parameter;
  numElements = elements;
  numParameters = params;
  parameter = new float*[numElements];
  for (int i = 0; i < numElements; i++)
    parameter[i] = new float[numParameters];
}
void CommonCalibrationParser::setDetID(int value) { DetID = value; }
int CommonCalibrationParser::getFileContent(string filename,
                                            vector<CommonCalibrationParser*>& content)
{
  ifstream input;
  input.open(filename.data());
  char li[1000], c;
  string line;
  int detID, nEl, pID, nPar, n1, n2, nf, ncommL;
  float f[1000];
  bool begins;
  while (input.good() && !input.eof()) {
    begins = false;
    ncommL = 0;
    line = getWord(input, 15);
    while (input.good() && !input.eof() && !(hasA(line, "==>") || hasA(line, "-->"))) {
      getLine(input);
      line = getWord(input, 15);
    }
    if (!(input.good() && !input.eof()))
      return content.size();
    c = ' ';
    skipWhile(input, " \t"); // spaces
    getWord(input, " ");     // detector name
    input >> detID >> nEl;
    skipWhile(input, " \t"); // spaces
    getWord(input, " ");     // calibration name
    input >> pID >> nPar;
    skipLine(input);
    CommonCalibrationParser* tmp = new CommonCalibrationParser(nEl, detID);
    tmp->setParamID(pID);
    tmp->setNumParamsElements(nEl, nPar);
    while (!begins && !input.eof() && input.good()) {
      line = getWord(input, ' ');
      line = lower(line);
      skipWhile(input, " \t");
      if (line.find("#") <= 1) {
        if (hasA(line, "#-------------"))
          ncommL++;
        if (ncommL == 2)
          begins = true;
        continue;
      } else if (hasA(line, "fromrun")) {
        input >> n1;
        tmp->setRange(0, n1);
      } else if (hasA(line, "torun")) {
        input >> n1;
        tmp->setRange(1, n1);
      } else if (hasA(line, "fromevent")) {
        input >> n1;
        tmp->setRange(2, n1);
      } else if (hasA(line, "toevent")) {
        input >> n1;
        tmp->setRange(3, n1);
      } else if (hasA(line, "gotfromfile")) {
        //	      begins=true;
      }
      skipLine(input);
    }
    nf = 0;
    while (!input.eof() && begins && nf < nEl) {
      input.get(c);
      if (c == '#') {
        skipLine(input);
        continue;
      } else if (c == '\n')
        continue;
      input.unget();
      input >> n1 >> n2;
      for (int i = 0; i < nPar; i++)
        input >> f[i];
      input.getline(li, 1000);
      for (int i = 0; i < nPar; i++)
        tmp->setParameter(n2, i, f[i]);
      nf++;
    }
    tmp->setValid();
    if (nf == nEl)
      content.push_back(tmp);
    else
      delete tmp;
  }
  input.close();
  return content.size();
}
