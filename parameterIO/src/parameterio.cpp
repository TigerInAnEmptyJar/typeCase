#include "parameterio.h"

#include <QtGlobal>

#include <fstream>

namespace {
parameter::IO::BaseParameterReader::FileType toReadType(
    parameter::IO::ParameterIO::FileType filetype)
{
  switch (filetype) {
  case parameter::IO::ParameterIO::FileType::ALGORITHM:
    return parameter::IO::BaseParameterReader::FileType::ALGORITHM;
  case parameter::IO::ParameterIO::FileType::SHAPE:
    return parameter::IO::BaseParameterReader::FileType::SHAPE;
  case parameter::IO::ParameterIO::FileType::PROCESS:
    return parameter::IO::BaseParameterReader::FileType::ALL;
  case parameter::IO::ParameterIO::FileType::BEAMTIME:
    return parameter::IO::BaseParameterReader::FileType::BEAMTIME;
  case parameter::IO::ParameterIO::FileType::RUN:
    return parameter::IO::BaseParameterReader::FileType::RUN;
  case parameter::IO::ParameterIO::FileType::DATABASE:
    return parameter::IO::BaseParameterReader::FileType::DATABASE;
  case parameter::IO::ParameterIO::FileType::DETECTOR:
    return parameter::IO::BaseParameterReader::FileType::DETECTOR_REACTION;
  case parameter::IO::ParameterIO::FileType::MATERIAL:
    return parameter::IO::BaseParameterReader::FileType::MATERIALS;
  case parameter::IO::ParameterIO::FileType::SETUP:
    return parameter::IO::BaseParameterReader::FileType::SETUP;
  case parameter::IO::ParameterIO::FileType::ALL:
    return parameter::IO::BaseParameterReader::FileType::ALL;
  }
  return parameter::IO::BaseParameterReader::FileType::UNDEFINED;
}
parameter::IO::BaseParameterWriter::FileType toWriteType(
    parameter::IO::ParameterIO::FileType filetype)
{
  switch (filetype) {
  case parameter::IO::ParameterIO::FileType::ALGORITHM:
    return parameter::IO::BaseParameterWriter::FileType::ALGORITHM;
  case parameter::IO::ParameterIO::FileType::SHAPE:
    return parameter::IO::BaseParameterWriter::FileType::SHAPE;
  case parameter::IO::ParameterIO::FileType::PROCESS:
    return parameter::IO::BaseParameterWriter::FileType::ALL;
  case parameter::IO::ParameterIO::FileType::BEAMTIME:
    return parameter::IO::BaseParameterWriter::FileType::BEAMTIME;
  case parameter::IO::ParameterIO::FileType::RUN:
    return parameter::IO::BaseParameterWriter::FileType::RUN;
  case parameter::IO::ParameterIO::FileType::DATABASE:
    return parameter::IO::BaseParameterWriter::FileType::DATABASE;
  case parameter::IO::ParameterIO::FileType::DETECTOR:
    return parameter::IO::BaseParameterWriter::FileType::DETECTOR_REACTION;
  case parameter::IO::ParameterIO::FileType::MATERIAL:
    return parameter::IO::BaseParameterWriter::FileType::MATERIALS;
  case parameter::IO::ParameterIO::FileType::SETUP:
    return parameter::IO::BaseParameterWriter::FileType::SETUP;
  case parameter::IO::ParameterIO::FileType::ALL:
    return parameter::IO::BaseParameterWriter::FileType::ALL;
  }
  return parameter::IO::BaseParameterWriter::FileType::UNDEFINED;
}
} // namespace

namespace parameter {
namespace IO {
std::vector<std::shared_ptr<base_parameter>> ParameterIO::readParameter(
    filesystem::path path, boost::uuids::uuid version, ParameterIO::FileType filetype) const
{
  auto it = _installedVersions.find(version);
  if (it == _installedVersions.end()) {
    return {};
  }
  try {
    auto reader = it->second.first;
    auto ft = ::toReadType(filetype);
    if (reader->isFiletypeAvailable(ft)) {
      return reader->readParameter(path, ft);
    }
  } catch (...) {
  }
  return {};
}

void ParameterIO::writeParameter(filesystem::path path,
                                 std::vector<std::shared_ptr<base_parameter>> const& param,
                                 boost::uuids::uuid version, ParameterIO::FileType filetype) const
{
  auto it = _installedVersions.find(version);
  if (it == _installedVersions.end()) {
    return;
  }
  try {
    auto writer = it->second.second;
    auto ft = ::toWriteType(filetype);
    if (writer->isFiletypeAvailable(ft)) {
      writer->writeParameter(param, path, ft);
    }
  } catch (...) {
  }
}

std::vector<boost::uuids::uuid> ParameterIO::getInstalledVersions() const
{
  std::vector<boost::uuids::uuid> result;
  std::transform(_installedVersions.begin(), _installedVersions.end(), std::back_inserter(result),
                 [](auto element) { return element.first; });
  return result;
}

bool ParameterIO::installVersion(std::shared_ptr<BaseParameterReader> reader,
                                 std::shared_ptr<BaseParameterWriter> writer, boost::uuids::uuid id)
{
  auto it = _installedVersions.find(id);
  if (it != _installedVersions.end()) {
    return false;
  }
  _installedVersions.insert(std::make_pair(id, std::make_pair(reader, writer)));
  return true;
}

bool ParameterIO::uninstallVersion(boost::uuids::uuid id)
{
  auto it = _installedVersions.find(id);
  if (it != _installedVersions.end()) {
    return false;
  }
  _installedVersions.erase(it);
  return true;
}

void ParameterIO::setAlgorithmIdRecoveryFunction(
    std::function<void(std::shared_ptr<base_parameter>&)> function)
{
  _algorithmRecovery = function;
  std::for_each(_installedVersions.begin(), _installedVersions.end(), [function](auto element) {
    element.second.first->setAlgorithmIdRecovery(function);
  });
}

void ParameterIO::setShapeIdRecoveryFunction(
    std::function<void(std::shared_ptr<base_parameter>&)> function)
{
  _shapeRecovery = function;
  std::for_each(_installedVersions.begin(), _installedVersions.end(),
                [function](auto element) { element.second.first->setShapeIdRecovery(function); });
}

std::shared_ptr<BaseParameterReader> ParameterIO::getResponsibleReader(
    boost::uuids::uuid versionHint, std::string const& header) const
{
  std::shared_ptr<BaseParameterReader> fallBackResult;
  auto it = _installedVersions.find(versionHint);
  if (it != _installedVersions.end()) {
    auto fileType = it->second.first->checkVersionString(header);
    if (fileType != BaseParameterReader::FileType::BAD_VERSION &&
        fileType != BaseParameterReader::FileType::UNDEFINED) {
      return it->second.first;
    }
    if (fileType == BaseParameterReader::FileType::UNDEFINED) {
      fallBackResult = it->second.first;
    }
  }
  for (auto version : _installedVersions) {
    auto fileType = version.second.first->checkVersionString(header);
    if (fileType != BaseParameterReader::FileType::BAD_VERSION &&
        fileType != BaseParameterReader::FileType::UNDEFINED) {
      return version.second.first;
    }
    if (fileType != BaseParameterReader::FileType::UNDEFINED && !fallBackResult) {
      fallBackResult = version.second.first;
    }
  }
  return fallBackResult;
}

std::shared_ptr<BaseParameterWriter> ParameterIO::getResponsibleWriter(
    boost::uuids::uuid version) const
{
  auto it = _installedVersions.find(version);
  if (it != _installedVersions.end()) {
    return it->second.second;
  }
  return {};
}

std::string ParameterIO::getFileHeader(std::filesystem::path path) const
{
  std::ifstream input;
  input.open(path.native());
  std::string result;
  std::getline(input, result);
  input.close();
  return result;
}

} // namespace IO
} // namespace parameter
