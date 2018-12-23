#ifndef COMMONCALIBRATIONPARSER
#define COMMONCALIBRATIONPARSER
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class CommonCalibrationParser
{
 private:
  int numParameters;
  int numElements;
  int DetID;
  int paramID;
  float **parameter;
  int *range;
  bool valid;
 public:
  CommonCalibrationParser(int nE, int ID);
  CommonCalibrationParser();
  ~CommonCalibrationParser();
  bool isValid()const;
  bool getFromFile(ifstream & input);
  int getNumParameters()const;
  int getNumElements()const;
  int getDetector()const;
  int getParamID()const;
  int getRange(int num)const;
  float getParameter(int element, int num)const;
  void setParameter(int element, int num, float value);
  void setRange(int num, int value);
  void setRanges(int fromRun, int toRun, int fromEvent=0, int toEvent=9999999);
  void setParamID(int value);
  void setNumParamsElements(int elements, int params);
  void setDetID(int value);
  void setValid(bool value=true);
  bool operator==(const CommonCalibrationParser &pars)const;
  void write(ostream &output,const string &author,const string &beamTime);
  static int getNumberOfCalibrationsInFile(string filename);
  static int getNumberOfCalibrationsInFile(string filename, int detID);
  static int getNumberOfCalibrationsInFile(string filename, int detID, int pID);
  static int getCalibrationTypesInFile(string filename, vector<int> &pIDs, int detID);
  static int getCalibrationTypesInFile(string filename, vector<int> &pIDs, vector<int> &dets);
  static int getFileContent(string filename, vector<CommonCalibrationParser*> &content);
};
#endif
