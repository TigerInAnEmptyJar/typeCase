#pragma once

#include "parameterwriter1.h"

#include <memory>
#include <ostream>
#include <vector>

namespace parameter {
namespace IO {

class ParameterWriter2 : public ParameterWriter1
{
public:
  void writeRuns(std::ostream& output,
                 std::vector<std::shared_ptr<base_parameter>> const& items) const override;
  // from ostream operator << (...)
  void writeSingleRun(std::ostream& output,
                      std::shared_ptr<base_parameter> const& item) const override;
  // from ostream operator << (...)
  void writeSingleDetector(std::ostream& output,
                           std::shared_ptr<base_parameter> const& item) const override;

  std::string getVersionString(FileType filetype) const override;
};

} // namespace IO
} // namespace parameter
