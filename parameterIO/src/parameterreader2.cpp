#include "parameterreader2.h"

#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "shapeparameter.h"

#include <boost/uuid/nil_generator.hpp>

#include <algorithm>
#include <fstream>

namespace parameter {
namespace IO {

std::vector<std::shared_ptr<base_parameter>> ParameterReader2::readParameter(
    filesystem::path path, BaseParameterReader::FileType filetype) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  std::ifstream input;
  input.open(path.native());
  switch (filetype) {
  case FileType::ALGORITHM:
    result = readAlgorithms(input);
    break;
  case FileType::SHAPE:
    result = readShapes(input);
    break;
  case FileType::RUN:
    result = readRuns(input);
    break;
  case FileType::BEAMTIME:
    result = readBeamtimes(input);
    break;
  case FileType::DATABASE: {
    result = readBeamtimes(input);
    std::vector<std::shared_ptr<base_parameter>> temp;
    std::for_each(result.begin(), result.end(), [this, &temp](auto element) {
      auto filepath = std::dynamic_pointer_cast<beamTime_parameter>(element)->getFileName();
      if (!std::filesystem::exists(filepath)) {
        return;
      }
      std::ifstream input;
      input.open(filepath.native());
      auto tmp = readRuns(input);
      std::transform(tmp.begin(), tmp.end(), std::back_inserter(temp), [element](auto runElement) {
        std::dynamic_pointer_cast<run_parameter>(runElement)
            ->setParent(std::dynamic_pointer_cast<beamTime_parameter>(element));
        std::dynamic_pointer_cast<run_parameter>(runElement)->setParentNumber(element->id());
        return runElement;
      });
      input.close();
    });
    result.insert(result.end(), temp.begin(), temp.end());
    break;
  }
  case FileType::MATERIALS:
    result = readMaterials(input);
    break;
  case FileType::DETECTOR_REACTION:
    result = readSetup(input);
    break;
  case FileType::SETUP:
    // not available in this version
  case FileType::ALL:
    // not available in this version
  case FileType::UNDEFINED:
  case FileType::BAD_VERSION:
    break;
  }
  input.close();
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader2::readRuns(istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numRuns;
  char li[1000];
  input.getline(li, 1000);
  input >> numRuns;
  for (size_t j = 0; j < numRuns; j++) {
    result.push_back(readSingleRun(input));
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader2::readSetup(istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfDetectors;
  reaction_parameter colision;
  char li[300];
  input.getline(li, 300);
  result.push_back(readSingleReaction(input));
  input >> numberOfDetectors;
  for (size_t i = 0; i < numberOfDetectors; i++) {
    result.push_back(readSingleDetector(input));
  }
  return result;
}

std::shared_ptr<base_parameter> ParameterReader2::readSingleRun(std::istream& input) const
{
  // From istream operator >>(...)
  run_parameter d;
  d.setId(boost::uuids::nil_uuid());
  int zahl;
  char li[500];
  char c;
  input.getline(li, 500);
  input >> zahl;
  d.setType(static_cast<run_parameter::RunType>(zahl));
  input >> zahl;
  //    d.setParentNumber(zahl);
  input >> zahl;
  d.setRunNumber(zahl);
  input >> zahl;
  d.setOwnSetup(zahl == 1);
  input >> zahl;
  d.setOwnCalibration(zahl == 1);
  input >> zahl;
  d.setAdditionalCalibration(zahl == 1);
  input.getline(li, 250);
  d.setName(std::string(li));
  long zahl2;
  std::string dis;
  input.get(c);
  std::string s;
  while (!(c == '=' || c == '#')) {
    switch (c) {
    case 'C':
      input.getline(li, 500);
      s = li; // expandPath(li);
      d.addCalibrationFile(s);
      break;
    case 'S':
      input.getline(li, 500);
      s = li; // expandPath(li);
      d.setSetupFile(s);
      d.setOwnSetup(true);
      break;
    case 'D':
      input.getline(li, 500);
      dis = dis + std::string(li);
      break;
    case 'F': {
      input >> zahl >> zahl2;
      input.get(c);
      input.getline(li, 500);
      s = li; // expandPath(li);
      d.addFile(s, zahl, zahl2);
      break;
    }
    case 'T': {
      int zz[6];
      for (int ii = 0; ii < 6; ii++)
        input >> zz[ii];
      input.getline(li, 500);
      d.setStartTime(zz[0], zz[1], zz[2], zz[3], zz[4], zz[5]);
      break;
    }
    case 'P': {
      int zz[6];
      for (int ii = 0; ii < 6; ii++)
        input >> zz[ii];
      input.getline(li, 500);
      d.setStopTime(zz[0], zz[1], zz[2], zz[3], zz[4], zz[5]);
      break;
    }
    }
    input.get(c);
    if (!input.good())
      return {};
  }
  input.getline(li, 500);
  d.setDescription(dis);
  return std::make_shared<run_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader2::readSingleDetector(istream& input) const
{
  // From istream operator >>(...)
  detector_parameter d;
  d.setId(boost::uuids::nil_generator()());
  //    int zahl1,zahl2,zahl3,zahl4,zahl5,zahl;
  int zahl;
  float value1; //,value2,value3;
  char c;
  string s;
  input >> zahl;
  d.setNumberOfElements(zahl);
  input >> zahl;
  d.setStackType(zahl);
  input >> zahl;
  d.setID(zahl);
  input >> zahl;
  d.setMaterial(zahl);
  input >> value1;
  d.setMaxDistance(value1);
  s = "";
  input.get(c);
  while (c != '\n' && !input.eof()) {
    s = s + c;
    input.get(c);
  }
  //  i.getline(li,100);
  d.setName(s);
  auto sh = readSingleShape(input);
  recoverShapeId(sh);
  auto shape = *dynamic_cast<shape_parameter*>(sh.get());

  d.setShape(shape);
  d.setMaterialId(boost::uuids::nil_uuid());
  return std::make_shared<detector_parameter>(d);
}

BaseParameterReader::FileType ParameterReader2::checkVersionString(string const& version) const
{
  auto expected = []() {
    std::map<std::string, FileType> result;
    result.insert(std::make_pair("########## algorithm parameter ####### file version :1 #######",
                                 FileType::ALGORITHM));
    result.insert(std::make_pair("########## shape parameter ####### file version :1 #######",
                                 FileType::SHAPE));
    result.insert(std::make_pair("########## detector parameter ####### file version :2 #######",
                                 FileType::DETECTOR_REACTION));
    result.insert(
        std::make_pair("########## run parameter ####### file version :1 #######", FileType::RUN));
    return result;
  }();

  auto exIt = expected.find(version);
  if (exIt != expected.end()) {
    return exIt->second;
  }
  return FileType::UNDEFINED;
}

} // namespace IO
} // namespace parameter
