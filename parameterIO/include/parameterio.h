#pragma once

#include "baseparameterreader.h"
#include "baseparameterwriter.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "paintparameter.h"
#include "shapeparameter.h"

#include <boost/uuid/uuid.hpp>

#include <filesystem>
#include <map>
#include <memory>
#include <vector>

namespace parameter {
namespace IO {

/*!
 * \brief The ParameterIO class
 * This class provides IO for all parameters needed for analysis.
 * The parameters can be stored in files one for each type or together in one file. The types
 * ALGORITHM, SHAPE, BEAMTIME, RUN, MATERIAL and DETECTOR store single type, PROCESS, DATABASE,
 * SETUP and ALL store more than one type.
 * Not all types are supported by all versions.
 *
 * What is currently still a bit unsatisfying is the error/exception handling here. In a future
 * commit we will provide a set of exception types to be thrown by readers/writers and ParameterIO
 * itself.
 */
class ParameterIO
{
public:
  /*!
   * \brief The FileType enum
   * This defines the type of files that is expected to be read and written:
   * ALGORITHM, SHAPE, BEAMTIME, RUN, MATERIAL and DETECTOR all read/write a single type of
   * parameter (well DETECTOR reads/writes both detector_parameter and reaction_parameter).
   * ALL will read/write all provided parameters.
   * DATABASE will read/write all beamTime_parameters and run_parameters (depending on version,
   * they might go into different files!).
   * SETUP handles both DETECTOR and MATERIAL.
   * PROCESS handles everything that is used in one specific analysis.
   */
  enum class FileType
  {
    ALGORITHM,
    SHAPE,
    PROCESS,
    BEAMTIME,
    RUN,
    DATABASE,
    MATERIAL,
    DETECTOR,
    SETUP,
    ALL,
  };

public:
  ParameterIO() = default;

  /*!
   * \name Read and write
   * \{
   */

  /*!
   * \brief readParameter
   * This function reads a file of a specific version and returns all found parameters according
   * to the provided file type.
   * Be sure that the file is of the specified version and type. Some versions might cause problems
   * if the file doesn't match with version and type.
   * \param path the path of the file to read,
   * \param version the version of the file to read,
   * \param filetype the expected file-type.
   * \return the read parameters.
   */
  std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                             boost::uuids::uuid version,
                                                             FileType filetype) const;

  /*!
   * \brief writeParameter
   * This function writes provided parameters to file. Depending on the file type not all parameters
   * in the list will be written.
   * The function will create the parent directory if it doesn't exist.
   * \param path the path of the file to write,
   * \param param the list of parameters to write,
   * \param version the version to write,
   * \param filetype the type of parameters to write to file.
   */
  void writeParameter(std::filesystem::path path,
                      std::vector<std::shared_ptr<base_parameter>> const& param,
                      boost::uuids::uuid version, FileType filetype) const;
  /*!
   * \}
   */

  /*!
   * \name Management
   * \{
   */

  /*!
   * \brief getInstalledVersions
   * This function returns a list of installed version ids. They can be used with the above
   * functions.
   * \return list of installed reader/writer ids.
   */
  std::vector<boost::uuids::uuid> getInstalledVersions() const;

  /*!
   * \brief installVersion
   * Use this function to install a reader/writer pair for parameter IO.
   * \param reader the object performing reading,
   * \param writer the object performing writing,
   * \param id the id of the version.
   * \return true if the operation was successful.
   */
  bool installVersion(std::shared_ptr<BaseParameterReader> reader,
                      std::shared_ptr<BaseParameterWriter> writer, boost::uuids::uuid id);

  /*!
   * \brief uninstallVersion
   * Use this function to uninstall an installed IO-version.
   * \param id the id of the version to uninstall.
   * \return true if the operation was successful (false, if the version id wasn't installed before,
   *  for example).
   */
  bool uninstallVersion(boost::uuids::uuid id);

  /*!
   * \brief setup
   * This function installs a predefined amount of parameter IO-versions.
   */
  void setup();

  /*!
   * \brief setAlgorithmIdRecoveryFunction
   * Some reader-versions do not preserve the uuid (being too old). With this function you can
   * provide a functor that will assign an id to any found algorithm for these readers.
   * The functor will take an algorithm_parameter (it'll have to cast it, since this header only
   * uses base_parameter) and do a lookup in a suitable database to find the correct id. This id has
   * to be assigned to the provided parameter.
   * \param function the functor assigning uuids to algorithms.
   */
  void setAlgorithmIdRecoveryFunction(
      std::function<void(std::shared_ptr<base_parameter>&)> function);

  /*!
   * \brief setShapeIdRecoveryFunction
   * Some reader-versions do not preserve the uuid (being too old). With this function you can
   * provide a functor that will assign an id to any found shapes for these readers.
   * The functor will take a shape_parameter (it'll have to cast it, since this header only uses
   * base_parameter) and do a lookup in a suitable database to find the correct id. This id has
   * to be assigned to the provided parameter.
   * \param function the functor assigning uuids to shapes.
   */
  void setShapeIdRecoveryFunction(std::function<void(std::shared_ptr<base_parameter>&)> function);
  /*!
   * \}
   */

private:
  std::shared_ptr<BaseParameterReader> getResponsibleReader(boost::uuids::uuid versionHint,
                                                            std::string const& header) const;
  std::shared_ptr<BaseParameterWriter> getResponsibleWriter(boost::uuids::uuid version) const;
  std::string getFileHeader(std::filesystem::path path) const;

private:
  std::map<boost::uuids::uuid,
           std::pair<std::shared_ptr<BaseParameterReader>, std::shared_ptr<BaseParameterWriter>>>
      _installedVersions;
  std::function<void(std::shared_ptr<base_parameter>&)> _shapeRecovery;
  std::function<void(std::shared_ptr<base_parameter>&)> _algorithmRecovery;
};

} // namespace IO
} // namespace parameter
