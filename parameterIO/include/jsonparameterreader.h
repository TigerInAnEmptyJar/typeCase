#pragma once

#include <baseparameterreader.h>

class QJsonObject;
class QJsonArray;

namespace parameter {
namespace IO {

class JsonParameterReader : public BaseParameterReader
{
public:
  std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                             FileType filetype) const override;

  bool isFiletypeAvailable(FileType check) const override;

  FileType checkVersionString(std::string const& version) const override;

private:
  std::shared_ptr<base_parameter> toAlgorithm(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toShape(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toBeamtime(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toRun(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toDetector(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toMaterial(QJsonObject const& input) const;
  std::shared_ptr<base_parameter> toReaction(QJsonObject const& input) const;

  std::vector<std::shared_ptr<base_parameter>> extract(QJsonObject const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractAlgorithms(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractShapes(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractBeamtimes(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractRuns(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractDetectors(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractMaterials(QJsonArray const& input) const;
  std::vector<std::shared_ptr<base_parameter>> extractReaction(QJsonArray const& input) const;
};

} // namespace IO
} // namespace parameter
