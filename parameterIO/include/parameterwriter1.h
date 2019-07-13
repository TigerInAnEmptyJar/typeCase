#pragma once

#include "parameterwriter0.h"

#include <memory>
#include <ostream>
#include <vector>

namespace parameter {
namespace IO {

class ParameterWriter1 : public ParameterWriter0
{
public:
  void writeAlgorithms(std::ostream& output,
                       std::vector<std::shared_ptr<base_parameter>> const& items) const override;
  void writeDetectors(std::ostream& output,
                      std::vector<std::shared_ptr<base_parameter>> const& items) const override;
  void writeShapes(std::ostream& output,
                   std::vector<std::shared_ptr<base_parameter>> const& items) const override;

  // from ostream operator << (...)
  void writeSingleAlgorithm(std::ostream& output,
                            std::shared_ptr<base_parameter> const& item) const override;
  // from write_run_parameter_1
  void writeSingleRun(std::ostream& output,
                      std::shared_ptr<base_parameter> const& item) const override;
  // from write_detector_parameter_1
  void writeSingleDetector(std::ostream& output,
                           std::shared_ptr<base_parameter> const& item) const override;
  // from ostream operator << (...)
  void writeSingleShape(std::ostream& output,
                        std::shared_ptr<base_parameter> const& item) const override;

  std::string getVersionString(FileType filetype) const override;
};

} // namespace IO
} // namespace parameter
