#ifndef __BEAMRUNPARAM
#define __BEAMRUNPARAM
#include "baseparameter.h"
#include <vector>
using namespace std;

/*!
 * \brief The beamTime_parameter class
 */
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
  /*!
   * \brief beamTime_parameter
   *  Constructor. Creates a beam-time with name n.
   * \param n
   */
  beamTime_parameter(string n = string());

  /*!
   * \brief ~beamTime_parameter
   */
  virtual ~beamTime_parameter();

  /*!
   * \brief getFileName
   *  Returns the file, where all runs for this beam-time are stored.
   * \return
   */
  string getFileName() const;

  /*!
   * \brief setFileName
   *  Sets the file where all runs for this beam-time are stored.
   * \param fn
   */
  void setFileName(string fn);

  /*!
   * \brief getSetupFile
   *  Returns the setup file. In this file, the detector setup is described.
   * \return
   */
  string getSetupFile() const;

  /*!
   * \brief setSetupFile
   *  Sets the setup file. In this file, the detector setup is described.
   * \param fn
   */
  void setSetupFile(string fn);

  /*!
   * \brief getCalibrationFile
   *  Return the numth calibration file.
   * \param num
   * \return
   */
  string getCalibrationFile(int num) const;

  /*!
   * \brief getCalibrationFile
   *  obsolete Return the numth calibration file for detector d.
   * \param num
   * \param d
   * \return
   */
  string getCalibrationFile(int num, int d) const;

  /*!
   * \brief setCalibrationFile
   *  Set the numth calibration file.
   * \param num
   * \param filename
   */
  void setCalibrationFile(int num, string filename);

  /*!
   * \brief getNumberOfCalibrationFiles
   *  Returns the number of calibration files.
   * \return
   */
  int getNumberOfCalibrationFiles() const;

  /*!
   * \brief addCalibrationFile
   *  Adds a file to the list of calibration files.
   * \param filename
   */
  void addCalibrationFile(string filename);

  /*!
   * \brief clearCalibrationFiles
   *  Clears/removes all calibration files.
   */
  void clearCalibrationFiles();

  /*!
   * \brief setCalibrationFile
   *  obsolete Sets the calibration file for detector det of type typ.
   * \param det
   * \param typ
   * \param fn
   */
  void setCalibrationFile(int det, int typ, string fn);

  /*!
   * \brief addDetectorCalibration
   *  obsolete. Adds a calibration type detector.
   * \param fn
   */
  virtual void addDetectorCalibration(vector<string> fn = vector<string>());

  /*!
   * \brief popDetectorCalibration
   *  obsolete. Removes the last calibration type detector.
   */
  virtual void popDetectorCalibration();

  /*!
   * \brief addTypCalibration
   *  obsolete. Adds a calibration type.
   * \param fn
   */
  virtual void addTypCalibration(vector<string> fn = vector<string>());

  /*!
   * \brief popTypCalibration
   *  obsolete. Removes the last calibration type.
   */
  virtual void popTypCalibration();

  /*!
   * \brief hasACalib
   *  obsolete. Returns true if the calibration type t for detector d should be used.
   * \param d
   * \param t
   * \return
   */
  bool hasACalib(int d, int t) const;

  /*!
   * \brief setHasACalib
   *  obsolete. Sets if the calibration type t for detector d should be used.
   * \param d
   * \param t
   * \param us
   */
  void setHasACalib(int d, int t, bool us = true);

  /*!
   * \brief getNumCalibDets
   *  obsolete Returns the number of calibration detectors.
   * \return
   */
  int getNumCalibDets() const;

  /*!
   * \brief getNumCalibTypes
   *  obsolete Returns the number of calibration types.
   * \return
   */
  int getNumCalibTypes() const;

  /*!

   * \brief getNumberOfDetectors
   *  Returns the number of detectors defined in this beamtime.
   * \return
   */
  int getNumberOfDetectors() const;

  /*!
   * \brief setNumberOfDetectors
   *  Sets the number of detectors defined in this beamtime.
   * \param num
   */
  void setNumberOfDetectors(int num);

  /*!
   * \brief setDetector
   *  Sets weather the numth detector has tdc (timing information) or qdc (energy information).
   * \param num
   * \param hasqdc
   * \param hastdc
   */
  void setDetector(int num, bool hasqdc, bool hastdc);

  /*!
   * \brief hasQDC
   *  For a given detector num it returns true, if there should be a qdc (energy information for
   * hits).
   * \param num
   * \return
   */
  bool hasQDC(int num) const;

  /*!
   * \brief hasTDC
   *  For a given detector num it returns true, if there should be a tdc (timing information for
   * hits).
   * \param num
   * \return
   */
  bool hasTDC(int num) const;

  /*!
   * \brief getMonth
   *  Returns the month the beam-time took place.
   * \return
   */
  int getMonth() const;

  /*!
   * \brief getYear
   *  Returns the year, the beam-time took place.
   * \return
   */
  int getYear() const;

  /*!
   * \brief setData
   *  Sets the date, the beam-time took place.
   * \param m
   * \param y
   */
  void setData(int m, int y);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param b
   */
  void operator=(const beamTime_parameter& b);
};

/*!
 * \brief operator <<
 * \param o
 * \param d
 * \return
 */
ostream& operator<<(ostream& o, const beamTime_parameter& d);

/*!
 * \brief operator >>
 * \param i
 * \param d
 * \return
 */
istream& operator>>(istream& i, beamTime_parameter& d);

//#define USE_QT
#ifdef USE_QT
#include <qdatetime.h>
#else
#include <time.h>
#endif

/*!
 * \brief The run_parameter class
 */
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
  /*!
   * \brief run_parameter
   *  Constructor. Sets the parent beam-time and the beam-time number.
   * \param parent
   * \param pn
   */
  run_parameter(beamTime_parameter& parent, int pn);

  /*!
   * \brief run_parameter
   *  Copy constructor.
   * \param r
   */
  run_parameter(const run_parameter& r);

  /*!
   * \brief run_parameter
   *  Default constructor. Sets the name of the run.
   * \param n
   */
  run_parameter(string n = string());

  /*!
   * \brief ~run_parameter
   */
  virtual ~run_parameter();

  /*!
   * \brief getType
   *  Returns the run type. A run can be a regular data run or a variety of calibration runs, or
   * else.
   * \return
   */
  int getType() const;

  /*!
   * \brief setType
   *  Sets the run type. A run can be a regular data run or a variety of calibration runs, or else.
   * \param t
   */
  void setType(int t);

  /*!
   * \brief getParentNumber
   *  Returns the parenting beam-times number.
   * \return
   */
  int getParentNumber() const;

  /*!
   * \brief setParentNumber
   *  Sets the parenting beam-times number.
   * \param p
   */
  void setParentNumber(int p);

  /*!
   * \brief getParent
   *  Returns a pointer to the parenting beam-time. s
   * \return
   */
  beamTime_parameter* getParent() const;

  /*!
   * \brief setParent
   *  Sets the pointer to the parenting beam-time.
   * \param p
   */
  void setParent(beamTime_parameter* p);

  /*!
   * \brief hasOwnSetup
   *  Returns true if there is an own detector setup file is defined for this run. Use the setup
   * file of the parenting beam-time else.
   * \return
   */
  bool hasOwnSetup() const;

  /*!
   * \brief setOwnSetup
   *  Sets if there is an own detector setup file defined for this run. Use the setup file of the
   * parenting beam-time else.
   * \param s
   */
  void setOwnSetup(bool s = true);

  /*!
   * \brief hasOwnCalibration
   *  Returns true if there are own calibration files used for this run. If it is false, use the
   * calibration files of the parenting beamtime.
   * \return
   */
  bool hasOwnCalibration() const;

  /*!
   * \brief setOwnCalibration
   *  Sets if there is an own calibration defined for this run. Use the calibration of the parenting
   * beamtime else.
   * \param c
   */
  void setOwnCalibration(bool c = true);

  /*!
   * \brief hasAdditionalCalibration
   *  Returns true if additionally to the parenting beam-times calibration file the own ones shall
   * be used.
   * \return
   */
  bool hasAdditionalCalibration() const;

  /*!
   * \brief setAdditionalCalibration
   *  Set wether additionally to the calibration files the parenting beam-time provides, the own
   * calibration files shall be used.
   * \param c
   */
  void setAdditionalCalibration(bool c = true);

  /*!
   * \brief addDetectorCalibration
   *  obsolete Adds a detector for calibration along with the files.
   * \param fn
   */
  virtual void addDetectorCalibration(vector<string> fn = vector<string>());

  /*!
   * \brief popDetectorCalibration
   *  obsolete Removes the last calibration detector.
   */
  virtual void popDetectorCalibration();

  /*!
   * \brief addTypCalibration
   *  obsolete Adds a calibration type.
   * \param fn
   */
  virtual void addTypCalibration(vector<string> fn = vector<string>());

  /*!
   * \brief popTypCalibration
   *  obsolete Removes the last calibration type.
   */
  virtual void popTypCalibration();

  /*!
   * \brief useOwn
   *  obsolete Returns true if the calibration type t for detector d is defined separately for this
   * run.
   * \param d
   * \param t
   * \return
   */
  bool useOwn(int d, int t) const;

  /*!
   * \brief setUseOwn
   *  obsolete Sets if the calibration type t for detector d is used separately for this run.
   * \param d
   * \param t
   * \param us
   */
  void setUseOwn(int d, int t, bool us = true);

  /*!
   * \brief getRunNumber
   *  Returns the run number.
   * \return
   */
  int getRunNumber() const;

  /*!
   * \brief setRunNumber
   *  Sets the run number.
   * \param rn
   */
  void setRunNumber(int rn);

  /*!
   * \brief addFile
   *  Adds a file of type type to the list of files. As file types there are actually available: 0:
   * for tade files. 2: for root files: 4: for hit files 8: for pixel files 16: for cluster files
   * 32: for track files 64: for reaction files You may add these bits if you have e.g. a root hit
   * file you'll have file type 6.
   * \param filename
   * \param type
   * \param fileEvents
   */
  void addFile(string filename, int type, long fileEvents = -1);

  /*!
   * \brief clearFiles
   *  Empties the list of files.
   */
  void clearFiles();

  /*!
   * \brief getNumberOfFiles
   *  Returns the number of files.
   * \return
   */
  int getNumberOfFiles() const;

  /*!
   * \brief getFile
   *  Returns the numth file or an empty string if num is non valid.
   * \param num
   * \return
   */
  string getFile(int num) const;

  /*!
   * \brief getFileType
   *  Returns the file type of file num. As file types there are actually available: 0: for tade
   * files. 2: for root files: 4: for hit files 8: for pixel files 16: for cluster files 32: for
   * track files 64: for reaction files You may add these bits if you have e.g. a root hit file
   * you'll have file type 6.
   * \param num
   * \return
   */
  int getFileType(int num) const;

  /*!
   * \brief setFileType
   *  Sets the file-type for file num. ?As file types there are actually available: ?0: for tade
   * files. ?2: for root files: 4: for hit files 8: for pixel files 16: for cluster files 32: for
   * track files 64: for reaction files You may add these bits if you have e.g. a root hit file
   * you'll have file type 6.
   * \param num
   * \param type
   */
  void setFileType(int num, int type);

  /*!
   * \brief getFileEvents
   *  Returns the number of events stored in the numth file. This number might be incorrect, since
   * there is no cross-check.
   * \param num
   * \return
   */
  long getFileEvents(int num) const;

  /*!
   * \brief setFileEvents
   *  Sets the number of events in file num. This number is not cross checked.
   * \param num
   * \param fileEvents
   */
  void setFileEvents(int num, long fileEvents);

  /*!
   * \brief setStartTime
   *  Sets the start time of the run.
   * \param year
   * \param month
   * \param day
   * \param hour
   * \param minute
   * \param second
   */
  void setStartTime(int year, int month, int day, int hour, int minute, int second);

  /*!
   * \brief setStopTime
   *  Sets the stop time of the run.
   * \param year
   * \param month
   * \param day
   * \param hour
   * \param minute
   * \param second
   */
  void setStopTime(int year, int month, int day, int hour, int minute, int second);
#ifdef USE_QT

  /*!
   * \brief setStartTime
   *  Sets the start time of the run.
   * \param time
   */
  void setStartTime(const QDateTime& time);

  /*!
   * \brief setStopTime
   *  Sets the stop time of the run.
   * \param time
   */
  void setStopTime(const QDateTime& time);

  /*!
   * \brief getStartTime
   *  Returns the start time of the run (see standard header file "time.h").
   * \return
   */
  QDateTime getStartTime() const;

  /*!
   * \brief getStopTime
   *  Returns the stop time of the run (see standard header file "time.h").
   * \return
   */
  QDateTime getStopTime() const;
#else

  /*!
   * \brief setStartTime
   *  Sets the start time of the run.
   * \param time
   */
  void setStartTime(const tm& time);

  /*!
   * \brief setStopTime
   *  Sets the stop time of the run.
   * \param time
   */
  void setStopTime(const tm& time);

  /*!
   * \brief getStartTime
   * \return
   */
  tm getStartTime() const;

  /*!
   * \brief getStopTime
   * \return
   */
  tm getStopTime() const;
#endif

  /*!
   * \brief getTime
   * \return
   */
  int getTime() const;

  /*!
   * \brief operator =
   *  Copy operator.
   * \param r
   */
  void operator=(const run_parameter& r);
};

/*!
 * \brief operator <<
 * \param o
 * \param d
 * \return
 */
ostream& operator<<(ostream& o, const run_parameter& d);

/*!
 * \brief operator >>
 * \param i
 * \param d
 * \return
 */
istream& operator>>(istream& i, run_parameter& d);

/*!
 * \brief expandPath
 * \param inp
 * \return
 */
string expandPath(string inp);

/*!
 * \brief contractPath
 * \param inp
 * \return
 */
string contractPath(string inp);
#endif
