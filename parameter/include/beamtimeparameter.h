#pragma once
#include "baseparameter.h"

#include <chrono>
#include <filesystem>
#include <map>
#include <vector>

/*!
 * \brief The beamTime_parameter class
 * This class contains information about a single beam-time. Starting with the
 * name and description, this class also hosts information about setup and
 * calibration to be used for analyzing data from this period of data-taking.
 * Additionally it contains two numbers, identifying year and month: the date
 * the data-taking took place.
 */
class beamTime_parameter : public base_parameter
{
public:
  /*!
   * \brief beamTime_parameter
   *  Constructor. Creates a beam-time with name n.
   * \param n the name of the new beam-time.
   */
  beamTime_parameter(std::string n = std::string());

  beamTime_parameter(beamTime_parameter const& other) = default;
  beamTime_parameter(beamTime_parameter&& other) = default;
  beamTime_parameter& operator=(beamTime_parameter const& b) = default;
  beamTime_parameter& operator=(beamTime_parameter&& b) = default;
  ~beamTime_parameter() = default;

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the parameter to compare to.
   * \return true if the parameters are identical.
   */
  bool operator==(beamTime_parameter const& other) const;

  /*!
   * \brief getFileName
   *  Returns the file, where all runs for this beam-time are stored.
   * \return the file with the content of the beam-time.
   */
  std::filesystem::path getFileName() const;

  /*!
   * \brief setFileName
   *  Sets the file where all runs for this beam-time are stored.
   * \param fn the file with the content of the beam-time.
   */
  void setFileName(std::filesystem::path fn);

  /*!
   * \brief getSetupFile
   *  Returns the setup file. In this file, the detector setup is described.
   * \return the file with the detector setup of the beam-time.
   */
  std::filesystem::path getSetupFile() const;

  /*!
   * \brief setSetupFile
   *  Sets the setup file. In this file, the detector setup is described.
   * \param fn the file with the detector setup of the beam-time.
   */
  void setSetupFile(std::filesystem::path fn);

  /*!
   * \brief getCalibrationFile
   *  Return the numth calibration file.
   * \param num the index of the calibration file to return.
   * \return one of the calibration files
   */
  std::filesystem::path getCalibrationFile(size_t num) const;

  /*!
   * \brief removeCalibrationFile
   * Removes one calibration file from the list of files.
   * \param num the index of the calibration file to remove.
   */
  void removeCalibrationFile(size_t num);

  /*!
   * \brief getNumberOfCalibrationFiles
   *  Returns the number of calibration files.
   * \return the number of calibration files.
   */
  size_t getNumberOfCalibrationFiles() const;

  /*!
   * \brief addCalibrationFile
   *  Adds a file to the list of calibration files.
   * \param filename the file to add to the list of calibration files.
   */
  void addCalibrationFile(std::filesystem::path filename);

  /*!
   * \brief clearCalibrationFiles
   *  Clears/removes all calibration files from the list. The files are not
   * removed from disk!
   */
  void clearCalibrationFiles();

  /*!
   * \brief getMonth
   *  Returns the month the beam-time took place.
   * \return the month of the beam-time.
   */
  int getMonth() const;

  /*!
   * \brief getYear
   *  Returns the year, the beam-time took place.
   * \return the year of the beam-time.
   */
  int getYear() const;

  /*!
   * \brief setData
   *  Sets the date, the beam-time took place.
   * \param m the month the beam-time took place,
   * \param y the year the beam-time took place.
   */
  void setData(int m, int y);

private:
  std::filesystem::path fileName;
  std::filesystem::path setupFile;
  std::vector<std::filesystem::path> calibFileNames;
  int month = 0;
  int year = 1900;
};

/*!
 * \brief operator <<
 * Streaming operator for debugging purposes.
 * \param o output stream,
 * \param d the parameter to stream.
 * \return the output stream.
 */
std::ostream& operator<<(std::ostream& o, const beamTime_parameter& d);

/*!
 * \brief The run_parameter class
 */
class run_parameter : public beamTime_parameter
{
public:
  enum class RunType
  {
    REGULAR,
    CALIBRATION,
    ELASTIC,
    BACKGROUND,
    OTHER,
  };
  enum class FileType : uint8_t
  {
    TADE_TYPE = 0x00,
    ROOT_TYPE = 0x02,
    HIT_TYPE = 0x04,
    PIXEL_TYPE = 0x08,
    CLUSTER_TYPE = 0x10,
    TRACKS_TYPE = 0x20,
    REACTION_TYPE = 0x40,
  };

public:
  /*!
   * \brief run_parameter
   *  Constructor. Sets the parent beam-time and the beam-time number.
   * \param parent
   * \param pn
   */
  run_parameter(std::shared_ptr<beamTime_parameter> parent, boost::uuids::uuid pn);

  /*!
   * \brief run_parameter
   *  Default constructor. Sets the name of the run.
   * \param n
   */
  run_parameter(std::string n = std::string());

  run_parameter(run_parameter const& r) = default;
  run_parameter(run_parameter&& r) = default;
  run_parameter& operator=(run_parameter const& r) = default;
  run_parameter& operator=(run_parameter&& r) = default;
  ~run_parameter() = default;

  /*!
   * \brief getType
   *  Returns the run type. A run can be a regular data run or a variety of calibration runs, or
   * else.
   * \return
   */
  RunType getType() const;

  /*!
   * \brief setType
   *  Sets the run type. A run can be a regular data run or a variety of calibration runs, or else.
   * \param t
   */
  void setType(RunType t);

  /*!
   * \brief getParentNumber
   *  Returns the parenting beam-times number.
   * \return
   */
  boost::uuids::uuid getParentNumber() const;

  /*!
   * \brief setParentNumber
   *  Sets the parenting beam-times number.
   * \param p
   */
  void setParentNumber(boost::uuids::uuid p);

  /*!
   * \brief getParent
   *  Returns a pointer to the parenting beam-time. s
   * \return
   */
  std::shared_ptr<beamTime_parameter> getParent() const;

  /*!
   * \brief setParent
   *  Sets the pointer to the parenting beam-time.
   * \param p
   */
  void setParent(std::shared_ptr<beamTime_parameter> p);

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
   *  Adds a file of type type to the list of files. See the enum \a FileType for
   * help here. You can concat the types with OR.
   * \param filename
   * \param type
   * \param fileEvents
   */
  void addFile(std::filesystem::path filename, size_t type, long fileEvents = -1);

  /*!
   * \brief removeFile
   *  Removes a file from the list of files.
   * \param num the index of the file to remove.
   */
  void removeFile(size_t num);

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
  size_t getNumberOfFiles() const;

  /*!
   * \brief getFile
   *  Returns the numth file or an empty string if num is non valid.
   * \param num
   * \return
   */
  std::filesystem::path getFile(size_t num) const;

  /*!
   * \brief getFileType
   *  Returns the file type of file num. As file types there are actually available: 0: for tade
   * files. 2: for root files: 4: for hit files 8: for pixel files 16: for cluster files 32: for
   * track files 64: for reaction files You may add these bits if you have e.g. a root hit file
   * you'll have file type 6.
   * \param num
   * \return
   */
  size_t getFileType(size_t num) const;

  /*!
   * \brief getFileEvents
   *  Returns the number of events stored in the numth file. This number might be incorrect, since
   * there is no cross-check.
   * \param num
   * \return
   */
  long getFileEvents(size_t num) const;

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

  /*!
   * \brief getTime
   * \return
   */
  int getTime() const;

private:
  RunType _type{RunType::REGULAR};
  boost::uuids::uuid _parentId;
  bool _hasSetup{false};
  bool _hasCalib{false};
  bool _hasAdditionalCalib{false};
  std::vector<std::vector<bool>> enabeles;
  int _runNumber;
  std::shared_ptr<beamTime_parameter> _parent;

  struct FileContainer
  {
    std::filesystem::path file;
    size_t fileType;
    long events;
  };
  std::vector<FileContainer> _files;

  // In C++-20 this will most probably be replaced by time_of_day and
  // year_month_day, but until we support C++-20, we'll have to stick with tm.
  tm _startingTime;
  tm _stopingTime;
};

/*!
 * \brief operator <<
 * Streaming operator for debugging purposes.
 * \param o the output stream,
 * \param d the parameter to stream.
 * \return the output stream.
 */
std::ostream& operator<<(std::ostream& o, const run_parameter& d);
