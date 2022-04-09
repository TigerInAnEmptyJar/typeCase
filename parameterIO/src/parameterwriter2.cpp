#include "parameterwriter2.h"

#include "beamtimeparameter.h"
#include "detectorparameter.h"

#include <QtGlobal>

#include <fstream>

namespace parameter {
namespace IO {

void ParameterWriter2::writeRuns(std::ostream& output,
                                 std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << getVersionString(FileType::RUN);
  ParameterWriter1::writeRuns(output, items);
}

// from write_run_parameter_1
void ParameterWriter2::writeSingleRun(std::ostream& output,
                                      std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto run = std::dynamic_pointer_cast<run_parameter>(item);
  output << "#========= " << run->getName().data() << " =========\n";
  output << static_cast<int>(run->getType()) << " " << 0 << " " << run->getRunNumber() << " \n";
  if (run->hasOwnSetup())
    output << 1 << " ";
  else
    output << 0 << " ";
  if (run->hasOwnCalibration())
    output << 1 << " ";
  else
    output << 0 << " ";
  if (run->hasAdditionalCalibration())
    output << 1;
  else
    output << 0;
  output << run->getName() << "\n";

  output << "D" << run->getDescription() << "\n";
  for (size_t i = 0; i < run->getNumberOfFiles(); i++)
    output << "F" << run->getFileType(i) << " " << run->getFileEvents(i) << " "
           << run->getFile(i).native() << "\n";
  if (run->hasOwnSetup())
    output << "S" << run->getSetupFile().native() << "\n";
  if (run->hasOwnCalibration() || run->hasAdditionalCalibration()) {
    //      o<<"C"<<d.getNumberOfCalibrationFiles()<<" ";
    for (size_t i = 0; i < run->getNumberOfCalibrationFiles(); i++) {
      output << "C" << run->getCalibrationFile(i).native() << "\n";
    }
  }
  output << "T" << run->getStartTime().tm_year + 1900 << " " << run->getStartTime().tm_mon << " "
         << run->getStartTime().tm_mday << " " << run->getStartTime().tm_hour << " "
         << run->getStartTime().tm_min << " " << run->getStartTime().tm_sec << "\n";
  output << "P" << run->getStopTime().tm_year + 1900 << " " << run->getStopTime().tm_mon << " "
         << run->getStopTime().tm_mday << " " << run->getStopTime().tm_hour << " "
         << run->getStopTime().tm_min << " " << run->getStopTime().tm_sec << "\n";
  output << "="
         << "\n";
}

void ParameterWriter2::writeSingleDetector(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto detector = std::dynamic_pointer_cast<detector_parameter>(item);
  if (!detector) {
    return;
  }
  output << detector->getNumberOfElements() << " " << detector->getStackType() << " "
         << detector->getID() << " " << detector->getMaterial() << " " << detector->getMaxDistance()
         << detector->getName().data() << endl;
  writeSingleShape(output, detector->getShape());
}

string ParameterWriter2::getVersionString(BaseParameterWriter::FileType filetype) const
{
  switch (filetype) {
  case FileType::DETECTOR_REACTION:
    return "########## detector parameter ####### file version :2 #######\n";
  case FileType::RUN:
    return "########## run parameter ####### file version :1 #######\n";
  default:
    break;
  }
  return ParameterWriter1::getVersionString(filetype);
}

} // namespace IO
} // namespace parameter
