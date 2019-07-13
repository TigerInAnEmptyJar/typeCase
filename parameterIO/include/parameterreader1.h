#pragma once

#include "parameterreader0.h"

namespace parameter {
namespace IO {

class ParameterReader1 : public ParameterReader0
{
public:
  std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                             FileType filetype) const override;

  std::vector<std::shared_ptr<base_parameter>> readAlgorithms(std::istream& input) const override;
  std::vector<std::shared_ptr<base_parameter>> readRuns(std::istream& input) const override;
  std::vector<std::shared_ptr<base_parameter>> readSetup(std::istream& input) const override;
  std::vector<std::shared_ptr<base_parameter>> readShapes(std::istream& input) const override;

  // That's already final.
  std::shared_ptr<base_parameter> readSingleAlgorithm(std::istream& input) const override;
  std::shared_ptr<base_parameter> readSingleRun(std::istream& input) const override;
  std::shared_ptr<base_parameter> readSingleDetector(std::istream& input) const override;
  // That's already final.
  std::shared_ptr<base_parameter> readSingleShape(std::istream& input) const override;
  FileType checkVersionString(std::string const& version) const override;
};

} // namespace IO
} // namespace parameter
