#include "ATeufelCorrection.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include <fstream>
#include <stdlib.h>
extern bool existing(string filename);
ATeufelCorrection::ATeufelCorrection(TCalibHit** hitsIn, const int& numberOfHitsIn, const int detID,
                                     int nElIn, int& evtNrIn, int& runNrIn,
                                     vector<string> calibrationFiles, string referencefile)
    : AAlgorithm((string("TeufelCorrection for det ") + string_number(detID))),
      numberOfHits(numberOfHitsIn), nEl(nElIn), evtNr(evtNrIn), runNr(runNrIn)
{
  hits = hitsIn;
  sranges = NULL;
  shift = NULL;
  params = NULL;
  // getFromFile(calibrationFiles);
  char li[1000];
  char c;
  // cout<<"reference file:\""<<referencefile.data()<<"\""<<endl;
  if (!existing(referencefile)) {
    string s = getenv("KTOFPACK");
    s = s.substr(0, s.rfind("/"));
    referencefile = s + "/" + referencefile;
  }
  // cout<<"reference file:\""<<referencefile.data()<<"\""<<endl;
  ifstream refe;
  refe.open(referencefile.data());
  int n = 2 + (detID == 15 ? 0 : nEl);

  // cout<<"______________________"<<endl;
  for (int i = 0; i < n; i++) {
    refe.getline(li, 1000);
    // cout<<li<<endl;
  }
  // cout<<"______________________"<<endl;
  params = new double*[nEl];
  float f1, f2;
  for (int i = 0; i < nEl; i++) {
    params[i] = new double[2];
    for (int j = 0; j < 9; j++) {
      refe.get(c);
    } // cout<<c<<flush;}
    refe >> f1 >> f2;
    params[i][0] = f1;
    params[i][1] = f2;
    //       refe>>params[i][0];
    //       refe>>params[i][1];
    refe.getline(li, 1000);
  }
  actualrange = 0;
}

ATeufelCorrection::~ATeufelCorrection()
{
  cleanup();
  if (params != NULL) {
    for (int i = 0; i < nEl; i++)
      delete[] params[i];
    delete[] params;
  }
}

void* ATeufelCorrection::process(void* ptr)
{
  if (numSranges < 0)
    return ptr;
  //   cout<<"check range"<<endl;
  if (!(evtNr < sranges[actualrange][1] && evtNr > sranges[actualrange][0] &&
        runNr <= sranges[actualrange][2] && runNr > sranges[actualrange][3])) {
    while (runNr < sranges[actualrange][2] || actualrange >= numSranges)
      actualrange++;
    while (evtNr < sranges[actualrange][0] || actualrange >= numSranges)
      actualrange++;
    if (actualrange >= numSranges)
      return ptr;
  }
  int adc, el;
  double adcC;
  //   cout<<"do "<<numberOfHits<<endl;
  for (int i = 0; i < numberOfHits; i++) {
    el = hits[i]->getElement();
    if (el >= nEl)
      continue;
    adc = hits[i]->getRawADC();
    //       cout<<adc<<" "<<el<<" "<<actualrange<<endl;
    //       cout<<shift[actualrange]<<" "<<params[el][0]<<"
    //       "<<params[el][1]<<endl;
    adcC = adc * shift[actualrange];
    adcC = (adcC - params[el][0]) * (500 / (params[el][1] - params[el][0])) + 1000;
    // have to set raw value because calibration takes raw values
    hits[i]->getRawADCr() = (int)adcC;
  }
  return ptr;
}

void ATeufelCorrection::onNewRum(run_parameter& r)
{
  anaLog << "ATeufelCorrection::noNewRun" << endli;
  vector<string> tmp;
  if (r.getParent() != NULL)
    for (size_t i = 0; i < r.getParent()->getNumberOfCalibrationFiles(); i++)
      tmp.push_back(r.getParent()->getCalibrationFile(i));
  if (r.hasAdditionalCalibration())
    for (size_t i = 0; i < r.getNumberOfCalibrationFiles(); i++)
      tmp.push_back(r.getCalibrationFile(i));
  getFromFile(tmp);
}
void ATeufelCorrection::getFromFile(vector<string> files)
{
  cleanup();
  char c;
  string line, line1;
  vector<int> rbeg;
  vector<double> val;
  vector<int> rrun;
  int n, nr;
  double d;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (!existing(files[i]))
      continue;
    ifstream* in = new ifstream(files[i].data());
    line = "";
    while (line == "" && in->good() && !in->eof()) {
      in->get(c);
      while (c != '\n' && in->good() && !in->eof()) {
        line = line + c;
        in->get(c);
      }
      remove(line, ' ');
    }
    if (line.find("gaugevariables") < line.npos) {
      remove(line, "#gaugevariables run ");
      nr = toInt(line); //.toInt();
      while (!in->eof()) {
        for (int j = 0; j < 3; j++) {
          in->get(c);
        }
        (*in) >> n;
        (*in) >> d;
        in->get(c);
        while (c != '\n' && in->good() && !in->eof()) {
          in->get(c);
        }
        rbeg.push_back(n);
        val.push_back(d);
        rrun.push_back(nr);
      }
    }
    in->close();
    delete in;
  }
  //  cout<<rbeg.size()<<endl;
  numSranges = rbeg.size();
  sranges = new int*[numSranges];
  shift = new double[numSranges];
  for (int i = 0; i < numSranges; i++) {
    shift[i] = val[i];
    sranges[i] = new int[4];
    sranges[i][0] = (i == 0 ? 0 : rbeg[i - 1]);
    sranges[i][1] = rbeg[i - 1];
    sranges[i][2] = rrun[i];
    sranges[i][3] = rrun[i];
  }
  actualrange = 0;
}
void ATeufelCorrection::cleanup()
{
  if (sranges != NULL) {
    for (int i = 0; i < numSranges; i++)
      delete[] sranges[i];
    delete[] sranges;
  }
  if (shift != NULL)
    delete[] shift;
}
