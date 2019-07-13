#pragma once

#include "baseparameterwriter.h"

#include <QJsonArray>
#include <QJsonObject>

#include <memory>
#include <ostream>
#include <vector>

namespace parameter {
namespace IO {

class JsonParameterWriter : public BaseParameterWriter
{
public:
  void writeParameter(std::vector<std::shared_ptr<base_parameter>> const& items,
                      std::filesystem::path path, FileType filetype) const override;

  bool isFiletypeAvailable(FileType check) const override;

  std::string getVersionString(FileType filetype) const override;

private:
  QJsonObject fromAlgorithm(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromShape(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromBeamtime(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromRun(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromDetector(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromMaterial(std::shared_ptr<base_parameter> const& input) const;
  QJsonObject fromReaction(std::shared_ptr<base_parameter> const& input) const;
};
} // namespace IO
} // namespace parameter
