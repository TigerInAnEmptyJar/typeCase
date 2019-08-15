#include "parameterreader1.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"

#include <boost/uuid/nil_generator.hpp>

#include <algorithm>
#include <fstream>

namespace parameter {
namespace IO {

std::vector<std::shared_ptr<base_parameter>> ParameterReader1::readParameter(
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
  case FileType::SETUP: {
    result = readSetup(input);
    auto tmp = readMaterials(input);
    std::transform(tmp.begin(), tmp.end(), std::back_inserter(result),
                   [](auto element) { return element; });
    break;
  }
  case FileType::ALL: {
    result = readAlgorithms(input);
    auto shapes = readShapes(input);
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(result),
                   [](auto element) { return element; });
    auto temp = readBeamtimes(input);
    std::for_each(result.begin(), result.end(), [this, &temp](auto element) {
      auto filepath = std::dynamic_pointer_cast<beamTime_parameter>(element)->getFileName();
      std::ifstream input;
      input.open(filepath.native());
      auto tmp = readRuns(input);
      std::transform(tmp.begin(), tmp.end(), std::back_inserter(temp),
                     [](auto element) { return element; });
      input.close();
    });
    std::transform(temp.begin(), temp.end(), std::back_inserter(result),
                   [](auto element) { return element; });
    auto detectors = readSetup(input);
    std::transform(detectors.begin(), detectors.end(), std::back_inserter(result),
                   [](auto element) { return element; });
    auto materials = readMaterials(input);
    std::transform(materials.begin(), materials.end(), std::back_inserter(result),
                   [](auto element) { return element; });
    break;
  }
  case FileType::UNDEFINED:
  case FileType::BAD_VERSION:
    break;
  }
  input.close();
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader1::readAlgorithms(
    std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t num;
  char li[1000];
  // in this version, the first line contains the version number. This Line, we skip.
  input.getline(li, 1000);
  input >> num;
  for (size_t j = 0; j < num; j++) {
    auto algorithm = readSingleAlgorithm(input);
    recoverAlgorithmId(algorithm);
    result.push_back(algorithm);
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader1::readRuns(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfRuns;
  input >> numberOfRuns;
  for (size_t i = 0; i < numberOfRuns; i++) {
    result.push_back(readSingleRun(input));
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader1::readSetup(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfDetectors;
  char li[300];
  // in this version, the first line contains the version number. This Line, we skip.
  input.getline(li, 300);

  result.push_back(readSingleReaction(input));
  input >> numberOfDetectors;
  for (size_t i = 0; i < numberOfDetectors; i++) {
    result.push_back(readSingleDetector(input));
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader1::readShapes(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfShapes;
  reaction_parameter colision;
  char li[300];
  // in this version, the first line contains the version number. This Line, we skip.
  input.getline(li, 300);
  input >> numberOfShapes;
  for (size_t i = 0; i < numberOfShapes; i++) {
    auto shape = readSingleShape(input);
    recoverShapeId(shape);
    result.push_back(shape);
  }
  return result;
}

std::shared_ptr<base_parameter> ParameterReader1::readSingleAlgorithm(std::istream& input) const
{
  // From istream operator >>(...)
  algorithm_parameter a;
  a.setId(boost::uuids::nil_uuid());
  int zahl, z2;
  float fz1, fz2, fz3;
  char c, _tmp[100], _tmp2[100];
  input.get(c);
  while (c != '#' && input.good())
    input.get(c);
  input >> zahl;
  a.setID(zahl);
  input >> zahl;
  a.setUse(zahl == 1);
  input.get(c);
  input.getline(_tmp, 100);
  a.setName(_tmp);
  input.get(c);
  input >> zahl;
  a.setCategory(zahl);
  input >> zahl;
  a.setLevel(zahl);
  input.getline(_tmp, 100);
  input.get(c);
  string d;
  bool finish = (c == '=' || input.eof());
  while (!finish) {
    switch (c) {
    case 'D': // description
    {
      input.getline(_tmp, 100);
      input.get(c);
      d += string(_tmp);
      break;
    }
    case 'B': // bool
    {
      input >> zahl;
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, (zahl == 1));
      break;
    }
    case 'P': // point3D
    {
      input >> fz1 >> fz2 >> fz3;
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, point3D(fz1, fz2, fz3));
      break;
    }
    case 'V': // vector3D
    {
      input >> fz1 >> fz2 >> fz3;
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, vector3D(fz1, fz2, fz3));
      break;
    }
    case 'I': // integer
    {
      input >> zahl;
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, static_cast<int>(zahl));
      break;
    }
    case 'F': // float
    {
      input >> fz1;
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, static_cast<float>(fz1));
      break;
    }
    case 'S': // string
    {
      input.getline(_tmp, 100);
      input.getline(_tmp2, 100);
      input.get(c);
      a.addValue(_tmp2, std::string(_tmp));
      break;
    }
    case 'N': // vector<int>
    {
      vector<int> tmp;
      input >> zahl;
      for (int j = 0; j < zahl; j++) {
        input >> z2;
        tmp.push_back(z2);
      }
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, tmp);
      break;
    }
    case 'T': // vector<float>
    {
      vector<float> tmp;
      input >> zahl;
      for (int j = 0; j < zahl; j++) {
        input >> fz2;
        tmp.push_back(fz2);
      }
      input.get(c);
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, tmp);
      break;
    }
    case 'R': // vector<string>
    {
      vector<string> tmp;
      input >> zahl;
      input.get(c);
      for (int j = 0; j < zahl; j++) {
        input.getline(_tmp2, 100);
        tmp.push_back(_tmp2);
      }
      input.getline(_tmp, 100);
      input.get(c);
      a.addValue(_tmp, tmp);
      break;
    }
    case 'A': // algorithm
    {
      auto ap = readSingleAlgorithm(input);
      recoverAlgorithmId(ap);
      input.get(c);
      a.addValue(ap->getName(), std::dynamic_pointer_cast<algorithm_parameter>(ap));
      break;
    }
    case '=':
    default:
      finish = true;
    }
  }
  a.setDescription(d);
  return std::make_shared<algorithm_parameter>(a);
}

std::shared_ptr<base_parameter> ParameterReader1::readSingleRun(std::istream& input) const
{
  // From read_run_parameter_1
  run_parameter d;
  d.setId(boost::uuids::nil_uuid());
  int zahl;
  char li[500];
  char c;
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
  input.getline(li, 250);
  d.setName(string(li));
  input >> zahl;
  string dis;
  for (int I = 0; I < zahl; I++) {
    input.getline(li, 250);
    dis += string(li);
  }
  d.setDescription(dis);
  // insert
  input >> zahl;
  int zahl2;
  long zahl3;
  for (int I = 0; I < zahl; I++) {
    input >> zahl2 >> zahl3;
    input.get(c);
    input.getline(li, 250);
    d.addFile(string(li), zahl2, zahl3);
  }
  if (d.hasOwnSetup()) {
    input.getline(li, 250);
    d.setSetupFile(string(li));
  }
  if (d.hasOwnCalibration()) {
    // insert
    input >> zahl;
    input.get(c);
    for (int I = 0; I < zahl; I++) {
      input.getline(li, 250);
      d.addCalibrationFile(string(li));
    }
  }
  return std::make_shared<run_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader1::readSingleDetector(std::istream& input) const
{
  // From read_detector_parameter_1
  detector_parameter d;
  d.setId(boost::uuids::nil_generator()());
  int zahl1, zahl2, zahl3, zahl4, zahl5, zahl;
  float value1, value2, value3;
  char li[100];
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
  input.getline(li, 100);
  d.setName(string(li));
  shape_parameter sh;
  input >> zahl1;
  input >> zahl2;
  input >> zahl3;
  input >> zahl4;
  input >> zahl5;
  input.getline(li, 100);
  sh.setName(string(li));
  point3D p;
  vector3D v;
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (int k = 0; k < zahl1; k++) {
    input >> value1;
    input >> value2;
    input >> value3;
    p.setValues(value1, value2, value3);
    input.getline(li, 100);
    paramMap[ParameterValue::ValueType::POINT3D].push_back(std::make_pair(std::string{li}, p));
  }
  for (int k = 0; k < zahl2; k++) {
    input >> value1;
    input >> value2;
    input >> value3;
    input.getline(li, 100);
    v.setValues(sin(value1) * cos(value2) * value3, sin(value1) * sin(value2) * value3,
                cos(value1) * value3);
    paramMap[ParameterValue::ValueType::VECTOR3D].push_back(std::make_pair(std::string{li}, v));
  }
  for (int k = 0; k < zahl3; k++) {
    input >> zahl;
    input.getline(li, 100);
    paramMap[ParameterValue::ValueType::INT].push_back(std::make_pair(std::string{li}, zahl));
  }
  for (int k = 0; k < zahl4; k++) {
    input >> value1;
    input.getline(li, 100);
    paramMap[ParameterValue::ValueType::FLOAT].push_back(std::make_pair(std::string{li}, value1));
  }
  char li1[100];
  for (int k = 0; k < zahl5; k++) {
    input.getline(li, 100);
    input.getline(li1, 100);
    paramMap[ParameterValue::ValueType::STRING].push_back(
        std::make_pair(std::string{li1}, std::string{li}));
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::POINT3D].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::POINT3D][i].first,
                paramMap[ParameterValue::ValueType::POINT3D][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::VECTOR3D].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::VECTOR3D][i].first,
                paramMap[ParameterValue::ValueType::VECTOR3D][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::INT].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::INT][i].first,
                paramMap[ParameterValue::ValueType::INT][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::FLOAT].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::FLOAT][i].first,
                paramMap[ParameterValue::ValueType::FLOAT][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::STRING].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::STRING][i].first,
                paramMap[ParameterValue::ValueType::STRING][i].second);
  }
  auto shape = std::make_shared<base_parameter>(sh);
  recoverShapeId(shape);
  sh.setId(shape->id());
  d.setShape(sh);
  d.setMaterialId(boost::uuids::nil_uuid());
  return std::make_shared<detector_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader1::readSingleShape(std::istream& input) const
{
  // From istream operator >>(...)
  auto getALine = [](istream& i) -> std::string {
    char c;
    std::string s;
    i.get(c);
    if (c == ' ') {
      i.get(c);
    }
    while (c != '\n') {
      s = s + c;
      i.get(c);
    }
    return s;
  };
  shape_parameter sh;
  sh.setId(boost::uuids::nil_uuid());
  char c, ch;
  float f[3];
  string s = getALine(input);
  sh.setName(s);
  input.get(c);
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  while (c != '=' && !input.eof()) {
    input.get(ch);
    switch (c) {
    case 'P': {
      point3D p;
      if (ch != ' ') {
        input.unget();
        for (int j = 0; j < 3; j++)
          input >> f[j];
        p.setValues(f[0], f[1], f[2]);
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::POINT3D].push_back(std::make_pair(s, p));
      break;
    }
    case 'V': {
      vector3D p;
      if (ch != ' ') {
        input.unget();
        for (int j = 0; j < 3; j++)
          input >> f[j];
        p.setValues(sin(f[0]) * cos(f[1]) * f[2], sin(f[0]) * sin(f[1]) * f[2], cos(f[0]) * f[2]);
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::VECTOR3D].push_back(std::make_pair(s, p));
      break;
    }
    case 'v': {
      vector3D p;
      if (ch != ' ') {
        input.unget();
        for (int j = 0; j < 3; j++)
          input >> f[j];
        p.setValues(sin(f[0] / 180. * M_PI) * cos(f[1] / 180. * M_PI) * f[2],
                    sin(f[0] / 180. * M_PI) * sin(f[1] / 180. * M_PI) * f[2],
                    cos(f[0] / 180. * M_PI) * f[2]);
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::VECTOR3D].push_back(std::make_pair(s, p));
      break;
    }
    case 'I': {
      int p;
      if (ch != ' ') {
        input.unget();
        input >> p;
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::INT].push_back(std::make_pair(s, p));
      break;
    }
    case 'F': {
      float p = 0;
      if (ch != ' ') {
        input.unget();
        input >> p;
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::FLOAT].push_back(std::make_pair(s, p));
      break;
    }
    case 'S': {
      string p = "";
      if (ch != ' ') {
        input.unget();
        p = getALine(input);
        sh.setCompleteWrite(true);
      }
      s = getALine(input);
      paramMap[ParameterValue::ValueType::STRING].push_back(std::make_pair(s, p));
      break;
    }
    }
    input.get(c);
  }
  input.get(c);
  while (c != '\n' && !input.eof()) {
    input.get(c);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::POINT3D].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::POINT3D][i].first,
                paramMap[ParameterValue::ValueType::POINT3D][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::VECTOR3D].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::VECTOR3D][i].first,
                paramMap[ParameterValue::ValueType::VECTOR3D][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::INT].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::INT][i].first,
                paramMap[ParameterValue::ValueType::INT][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::FLOAT].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::FLOAT][i].first,
                paramMap[ParameterValue::ValueType::FLOAT][i].second);
  }
  for (size_t i = 0; i < paramMap[ParameterValue::ValueType::STRING].size(); i++) {
    sh.addValue(paramMap[ParameterValue::ValueType::STRING][i].first,
                paramMap[ParameterValue::ValueType::STRING][i].second);
  }
  return std::make_shared<shape_parameter>(sh);
}

BaseParameterReader::FileType ParameterReader1::checkVersionString(const string& version) const
{
  auto expected = []() {
    std::map<std::string, FileType> result;
    result.insert(std::make_pair("########## algorithm parameter ####### file version :1 #######",
                                 FileType::ALGORITHM));
    result.insert(std::make_pair("########## shape parameter ####### file version :1 #######",
                                 FileType::SHAPE));
    result.insert(std::make_pair("########## detector parameter ####### file version :1 #######",
                                 FileType::DETECTOR_REACTION));
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
