#ifndef PARAMETERMANAGER
#define PARAMETERMANAGER
#include "algorithmparameter.h"
#include "baseparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "paintparameter.h"
#include "shapeparameter.h"
//#include <qdatetime.h>
#include <time.h>
using namespace std;

/*!
 * \brief The parameterManager class
 *  This class is a static helper class to read and write the parameter-classes from and to file.
 * Since it can read different file-versions not just current ones it is quite useful, but take care
 * to initialize it before use.
 */
class parameterManager
{
  static bool isInit;
  static int* nVersions;
  static int** versionNumbers;
  //  static QDate **versionDates;
  static tm** versionDates;

public:
  /*!
   * \brief IsInit
   *  Static method. Returns true if initialized.
   * \return
   */
  static bool IsInit();

  /*!
   * \brief init
   *  Initialize static class-variables.
   */
  static void init();

  /*!
   * Static template method. Returns the number of versions for the with template specified property
   * was introduced.
   */
  template <class X>
  static int numberOfVersions();

  /*!
   * Static template method. Returns the version number of the numth version for the with template
   * specified property.
   */
  template <class X>
  static int version(int num);
  //  template<class X> static QDate versionDate(int num);

  /*!
   * Static template method. Returns the date, the numth version for the with template specified
   * property was introduced.
   */
  template <class X>
  static tm versionDate(int num);

  /*!
   * \brief readAlgorithmParameter_ascii
   *  Static method. Reads algorithm-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter>& ret,
                                           int version = -1);

  /*!
   * \brief readUsedAlgorithmParameter_ascii
   *  Static method. Reads algorithm-parameter from file filename and returns only the algorithms
   * with the isUsed-flag switched on. For the newer file-versions a version-check is performed,
   * unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter>& ret,
                                               int version = -1);

  /*!
   * \brief writeAlgorithmParameter_ascii
   *  Static method. Writes algorithm-parameter to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writeAlgorithmParameter_ascii(string filename, const vector<algorithm_parameter>& ret,
                                            int version = -1);

  /*!
   * \brief readAlgorithmParameter_ascii
   *  Static method. Reads algorithm-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*>& ret,
                                           int version = -1);

  /*!
   * \brief readUsedAlgorithmParameter_ascii
   *  Static method. Reads algorithm-parameter from file filename returning only the ones with the
   * isUsed flag being switched on. For the newer file-versions a version-check is performed,
   * unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readUsedAlgorithmParameter_ascii(string filename, vector<algorithm_parameter*>& ret,
                                               int version = -1);

  /*!
   * \brief writeAlgorithmParameter_ascii
   *  Static method. Writes algorithm parameter to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writeAlgorithmParameter_ascii(string filename,
                                            const vector<algorithm_parameter*>& ret,
                                            int version = -1);

  /*!
   * \brief readDetectorParameter_ascii
   *  Static method. Reads detector-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readDetectorParameter_ascii(string filename, vector<detector_parameter>& ret,
                                          int version = -1);

  /*!
   * \brief readDetectorParameter_ascii
   *  Static method. Reads detector-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param col
   * \param version
   * \return
   */
  static bool readDetectorParameter_ascii(string filename, vector<detector_parameter>& ret,
                                          reaction_parameter& col, int version = -1);

  /*!
   * \brief writeDetectorParameter_ascii
   *  Static method. Writes detector-parameter to file filename.
   * \param filename
   * \param ret
   * \param col
   * \param version
   * \return
   */
  static bool writeDetectorParameter_ascii(string filename, const vector<detector_parameter>& ret,
                                           const reaction_parameter& col, int version = -1);

  /*!
   * \brief readMaterialParameter_ascii
   *  Static method. Reads material-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readMaterialParameter_ascii(string filename, vector<material_parameter>& ret,
                                          int version = -1);

  /*!
   * \brief writeMaterialParameter_ascii
   *  Static method. Writes material-parameter to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writeMaterialParameter_ascii(string filename, const vector<material_parameter>& ret,
                                           int version = -1);

  /*!
   * \brief readSetup_ascii
   *  Static method. Reads setup from file. reaction-parameter and detector-parameter are read from
   * file filename, material-parameter from file filename1. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param mret
   * \param col
   * \param version
   * \param filename1
   * \return
   */
  static bool readSetup_ascii(string filename, vector<detector_parameter>& ret,
                              vector<material_parameter>& mret, reaction_parameter& col,
                              int version = -1, string filename1 = "");

  /*!
   * \brief writeSetup_ascii
   *  Static method. Writes setup to file. Detector-parameters and reaction-parameter are written to
   * file filename, material-parameter to file filename1.
   * \param filename
   * \param filename1
   * \param ret
   * \param mret
   * \param col
   * \param version
   * \return
   */
  static bool writeSetup_ascii(string filename, string filename1,
                               const vector<detector_parameter>& ret,
                               const vector<material_parameter>& mret,
                               const reaction_parameter& col, int version = -1);

  /*!
   * \brief readShapeParameter_ascii
   *  Static method. Reads shape-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readShapeParameter_ascii(string filename, vector<shape_parameter>& ret,
                                       int version = -1);

  /*!
   * \brief writeShapeParameter_ascii
   *  Static method. Writes shape-parameters to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writeShapeParameter_ascii(string filename, const vector<shape_parameter>& ret,
                                        int version = -1);

  /*!
   * \brief readPaintParameter_ascii
   *  Static method. Reads paint-parameters from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readPaintParameter_ascii(string filename, vector<vector<paint_parameter>>& ret,
                                       int version = -1);

  /*!
   * \brief writePaintParameter_ascii
   *  Static method. Writes paint-parameter to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writePaintParameter_ascii(string filename, const vector<vector<paint_parameter>>& ret,
                                        int version = -1);

  /*!
   * \brief readBeamTimeParameter_ascii
   *  Static method. Reads beam-time-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readBeamTimeParameter_ascii(string filename, vector<beamTime_parameter>& ret,
                                          int version = -1);

  /*!
   * \brief readRunParameter_ascii
   *  Static method. Reads run-parameter from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool readRunParameter_ascii(string filename, vector<run_parameter>& ret, int version = -1);

  /*!
   * \brief addRunParameter_ascii
   *  Static function. Reads and adds runs from file filename to ret.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool addRunParameter_ascii(string filename, vector<run_parameter>& ret, int version = -1);

  /*!
   * \brief readDataBasis_ascii
   *  Static method. Reads a data-basis from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param bret
   * \param rret
   * \param version
   * \return
   */
  static bool readDataBasis_ascii(string filename, vector<beamTime_parameter>& bret,
                                  vector<run_parameter>& rret, int version = -1);

  /*!
   * \brief readDataBasis_ascii
   *  Static method. Reads a data-basis from file filename. For the newer file-versions a
   * version-check is performed, unfortionatelly this is not possible for the older ones.
   * \param filename
   * \param bret
   * \param rret
   * \param version
   * \return
   */
  static bool readDataBasis_ascii(string filename, vector<beamTime_parameter>& bret,
                                  vector<vector<run_parameter>>& rret, int version = -1);

  /*!
   * \brief writeBeamTimeParameter_ascii
   *  Static method. Writes the beam-time-parameters to file filename.
   * \param filename
   * \param ret
   * \param version
   * \return
   */
  static bool writeBeamTimeParameter_ascii(string filename, const vector<beamTime_parameter>& ret,
                                           int version = -1);

  /*!
   * \brief writeRunParameter_ascii
   *  Static method. Writes run-parameter of the parent beamTime to file filename.
   * \param filename
   * \param ret
   * \param beamTime
   * \param version
   * \return
   */
  static bool writeRunParameter_ascii(string filename, const vector<run_parameter>& ret,
                                      int beamTime, int version = -1);

  /*!
   * \brief writeRunParameter_ascii
   *  Static method. Writes run-parameter of the parent beamTime to file filename.
   * \param filename
   * \param ret
   * \param beamTime
   * \param version
   * \return
   */
  static bool writeRunParameter_ascii(string filename, const vector<run_parameter>& ret,
                                      beamTime_parameter* beamTime, int version = -1);

  /*!
   * \brief writeDataBasis_ascii
   *  Static method. Writes a data-basis to file filename. Note that the runs are saved in the files
   * specified in bret.
   * \param filename
   * \param bret
   * \param rret
   * \param version
   * \return
   */
  static bool writeDataBasis_ascii(string filename, const vector<beamTime_parameter>& bret,
                                   const vector<run_parameter>& rret, int version = -1);

  /*!
   * \brief writeDataBasis_ascii
   *  Static method. Writes a data-basis to file filename. Note that the runs are saved in the files
   * specified in bret.
   * \param filename
   * \param bret
   * \param rret
   * \param version
   * \return
   */
  static bool writeDataBasis_ascii(string filename, const vector<beamTime_parameter>& bret,
                                   const vector<vector<run_parameter>>& rret, int version = -1);
};
#endif
