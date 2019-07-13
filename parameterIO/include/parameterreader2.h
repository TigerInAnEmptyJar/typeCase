#pragma once

#include "parameterreader1.h"

namespace parameter {
namespace IO {

class ParameterReader2 : public ParameterReader1
{
public:
  std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                             FileType filetype) const override;
  std::vector<std::shared_ptr<base_parameter>> readRuns(std::istream& input) const override;
  std::vector<std::shared_ptr<base_parameter>> readSetup(std::istream& input) const override;

  std::shared_ptr<base_parameter> readSingleRun(std::istream& input) const override;
  std::shared_ptr<base_parameter> readSingleDetector(std::istream& input) const override;
  FileType checkVersionString(std::string const& version) const override;
};

} // namespace IO
} // namespace parameter
