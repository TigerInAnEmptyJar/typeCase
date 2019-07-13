#pragma once

#include <baseparameter.h>

#include <filesystem>
#include <functional>
#include <istream>
#include <memory>
#include <vector>

namespace parameter {
namespace IO {

class BaseParameterReader
{
public:
  enum class FileType
  {
    ALGORITHM,
    SHAPE,
    RUN,
    BEAMTIME,
    DATABASE,
    MATERIALS,
    DETECTOR_REACTION,
    SETUP,
    ALL,
    UNDEFINED,
    BAD_VERSION,
  };

public:
  virtual ~BaseParameterReader() = default;

  virtual std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                                     FileType filetype) const = 0;

  virtual bool isFiletypeAvailable(FileType check) const = 0;

  virtual FileType checkVersionString(std::string const& version) const = 0;

  virtual void setShapeIdRecovery(std::function<void(std::shared_ptr<base_parameter>&)>) {}
  virtual void setAlgorithmIdRecovery(std::function<void(std::shared_ptr<base_parameter>&)>) {}
};

} // namespace IO
} // namespace parameter
