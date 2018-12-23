#ifndef PARAMETERMANAGER
#define PARAMETERMANAGER
#include "baseparameter.h"
#include "algorithmparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"
#include "paintparameter.h"
#include "beamtimeparameter.h"
//#include <qdatetime.h>
#include <time.h>
using namespace std;

class parameterManager
{
  static bool isInit;
  static int *nVersions;
  static int **versionNumbers;
  //  static QDate **versionDates;
  static tm **versionDates;
 public:
  static bool IsInit();
  static void init();
  template<class X> static int numberOfVersions();
  template<class X> static int version(int num);
  //  template<class X> static QDate versionDate(int num);
  template<class X> static tm versionDate(int num);
  static bool readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter> &ret, int version=-1);
  static bool readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter> &ret, int version=-1);
  static bool writeAlgorithmParameter_ascii(string filename, const vector<algorithm_parameter> &ret, int version=-1);
  static bool readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*> &ret, int version=-1);
  static bool readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*> &ret, int version=-1);
  static bool writeAlgorithmParameter_ascii(string filename, const vector<algorithm_parameter*> &ret, int version=-1);
  static bool readDetectorParameter_ascii(string filename, vector<detector_parameter> &ret, int version=-1);
  static bool readDetectorParameter_ascii(string filename, vector<detector_parameter> &ret, reaction_parameter &col, int version=-1);
  static bool writeDetectorParameter_ascii(string filename, const vector<detector_parameter> &ret, const reaction_parameter &col, int version=-1);
  static bool readMaterialParameter_ascii(string filename, vector<material_parameter> &ret, int version=-1);
  static bool writeMaterialParameter_ascii(string filename, const vector<material_parameter> &ret, int version=-1);
  static bool readSetup_ascii(string filename, vector<detector_parameter> &ret, vector<material_parameter> &mret, reaction_parameter &col, int version=-1, string filename1="");
  static bool writeSetup_ascii(string filename, string filename1, const vector<detector_parameter> &ret, const vector<material_parameter> &mret, const reaction_parameter &col, int version=-1);
  static bool readShapeParameter_ascii(string filename, vector<shape_parameter> &ret, int version=-1);
  static bool writeShapeParameter_ascii(string filename, const vector<shape_parameter> &ret, int version=-1);
  static bool readPaintParameter_ascii(string filename, vector<vector<paint_parameter> > &ret, int version=-1);
  static bool writePaintParameter_ascii(string filename, const vector<vector<paint_parameter> > &ret, int version=-1);
  static bool readBeamTimeParameter_ascii(string filename, vector<beamTime_parameter> &ret, int version=-1);
  static bool readRunParameter_ascii(string filename, vector<run_parameter> &ret, int version=-1);
  static bool addRunParameter_ascii(string filename, vector<run_parameter> &ret, int version=-1);
  static bool readDataBasis_ascii(string filename, vector<beamTime_parameter> &bret, vector<run_parameter> &rret, int version=-1);
  static bool readDataBasis_ascii(string filename, vector<beamTime_parameter> &bret, vector<vector<run_parameter> > &rret, int version=-1);
  static bool writeBeamTimeParameter_ascii(string filename, const vector<beamTime_parameter> &ret, int version=-1);
  static bool writeRunParameter_ascii(string filename, const vector<run_parameter> &ret, int beamTime, int version=-1);
  static bool writeRunParameter_ascii(string filename, const vector<run_parameter> &ret, beamTime_parameter *beamTime, int version=-1);
  static bool writeDataBasis_ascii(string filename, const vector<beamTime_parameter> &bret, const vector<run_parameter> &rret, int version=-1);
  static bool writeDataBasis_ascii(string filename, const vector<beamTime_parameter> &bret, const vector<vector<run_parameter> > &rret, int version=-1);
};
#endif
