#pragma once

#include "baseparameter.h"

#include <filesystem>
#include <memory>
#include <vector>

namespace parameter {
namespace IO {

class BaseParameterWriter
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
  };

public:
  virtual ~BaseParameterWriter() = default;

  virtual void writeParameter(std::vector<std::shared_ptr<base_parameter>> const& items,
                              std::filesystem::path path, FileType filetype) const = 0;

  virtual bool isFiletypeAvailable(FileType check) const = 0;

  virtual std::string getVersionString(FileType filetype) const = 0;
};

} // namespace IO
} // namespace parameter
