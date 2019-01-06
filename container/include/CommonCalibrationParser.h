#ifndef COMMONCALIBRATIONPARSER
#define COMMONCALIBRATIONPARSER
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*!
 * \brief The CommonCalibrationParser class
 * This is a parser for the common calibration defined for the TOF collaboration
 * in 2004.
 */
class CommonCalibrationParser
{
private:
  int numParameters;
  int numElements;
  int DetID;
  int paramID;
  float** parameter;
  int* range;
  bool valid;

public:
  /*!
   * \brief CommonCalibrationParser
   * Constructor. Sets the detector ID and the number of elements, the detector has.
   * \param nE
   * \param ID
   */
  CommonCalibrationParser(int nE, int ID);

  /*!
   * \brief CommonCalibrationParser
   * Default constructor.
   */
  CommonCalibrationParser();

  /*!
   * \brief Destructor
   */
  ~CommonCalibrationParser();

  /*!
   * \brief isValid
   * Returns true if the calibration is valid.
   * \return
   */
  bool isValid() const;

  /*!
   * \brief getFromFile
   * Reads a calibration from an input file stream and returns true if this operation
   * was sucessful. This method is obsolete, use getFileContent() instread.
   * \param input
   * \return
   */
  bool getFromFile(ifstream& input);

  /*!
   * \brief getNumParameters
   * Returns the number of parameters defined for each element.
   * \return
   */
  int getNumParameters() const;

  /*!
   * \brief getNumElements
   * Returns the number of elements of the detector.
   * \return
   */
  int getNumElements() const;

  /*!
   * \brief getDetector
   * Returns the detector ID.
   * \return
   */
  int getDetector() const;

  /*!
   * \brief getParamID
   * Returns the ID of the calibration.
   * \return
   */
  int getParamID() const;

  /*!
   * \brief getRange
   * Returns the valid range of calibration. For num is 0 or 1, it is the beginning and
   * ending event number, for num 2 or 3, it is the beginning and ending run number.
   * \param num
   * \return
   */
  int getRange(int num) const;

  /*!
   * \brief getParameter
   * Returns the numth parameter of some element.
   * \param element
   * \param num
   * \return
   */
  float getParameter(int element, int num) const;

  /*!
   * \brief setParameter
   * Set the numth parameter for a specific element to value.
   * \param element
   * \param num
   * \param value
   */
  void setParameter(int element, int num, float value);

  /*!
   * \brief setRange
   * Sets the valid range of calibration. For num is 0 or 1, it is the beginning and
   * ending event number, for num 2 or 3, it is the beginning and ending run number.
   * \param num
   * \param value
   */
  void setRange(int num, int value);

  /*!
   * \brief setRanges
   *
   * \param fromRun
   * \param toRun
   * \param fromEvent
   * \param toEvent
   */
  void setRanges(int fromRun, int toRun, int fromEvent = 0, int toEvent = 9999999);

  /*!
   * \brief setParamID
   * Set the parameter id.
   * \param value
   */
  void setParamID(int value);

  /*!
   * \brief setNumParamsElements
   * Set the number of elements in the detector and the number of parameters for
   * each element.
   * \param elements
   * \param params
   */
  void setNumParamsElements(int elements, int params);

  /*!
   * \brief setDetID
   * Set the detector id.
   * \param value
   */
  void setDetID(int value);

  /*!
   * \brief setValid
   * Sets the calibration to valid.
   * \param value
   */
  void setValid(bool value = true);

  /*!
   * \brief operator ==
   * Comparison operator. Returns true the two calibrations describe the same detector
   * with the same number of elements, the same number of parameters and if they have
   * the same ranges.
   * \param pars
   * \return
   */
  bool operator==(const CommonCalibrationParser& pars) const;

  /*!
   * \brief write
   * Writes the calibration data to an ostream in the common-calibration format. You can
   * specify an author and a beamTime for that.
   * \param output
   * \param author
   * \param beamTime
   */
  void write(ostream& output, const string& author, const string& beamTime);

  /*!
   * \brief getNumberOfCalibrationsInFile
   * Static method. Returns the number of calibrations to be found in this file.
   * \param filename
   * \return
   */
  static int getNumberOfCalibrationsInFile(string filename);

  /*!
   * \brief getNumberOfCalibrationsInFile
   * Static method. Returns the number of calibrations for detector detID in this file.
   * \param filename
   * \param detID
   * \return
   */
  static int getNumberOfCalibrationsInFile(string filename, int detID);

  /*!
   * \brief getNumberOfCalibrationsInFile
   * Static method. Returns the number of calibrations of ID pID for detector detID found in
   * the file.
   * \param filename
   * \param detID
   * \param pID
   * \return
   */
  static int getNumberOfCalibrationsInFile(string filename, int detID, int pID);

  /*!
   * \brief getCalibrationTypesInFile
   * Static method. Reads an ascii file and returns the calibration IDs of the found
   * calibrations for detector detID. The return value is the number of calibrations
   * found.
   * \param filename
   * \param pIDs
   * \param detID
   * \return
   */
  static int getCalibrationTypesInFile(string filename, vector<int>& pIDs, int detID);

  /*!
   * \brief getCalibrationTypesInFile
   * Static method. Reads an ascii file and returns the calibration and detector IDs of
   * the found calibrations. The return value is the number of calibrations found.
   * \param filename
   * \param pIDs
   * \param dets
   * \return
   */
  static int getCalibrationTypesInFile(string filename, vector<int>& pIDs, vector<int>& dets);

  /*!
   * \brief getFileContent
   * Static method. Reads an ascii file and returns the found calibrations in the vector
   * content. The return value is the number of calibrations found. Take care, that you
   * delete all entries in the vector correctly after use: potential memory leak.
   * \param filename
   * \param content
   * \return
   */
  static int getFileContent(string filename, vector<CommonCalibrationParser*>& content);
};
#endif
