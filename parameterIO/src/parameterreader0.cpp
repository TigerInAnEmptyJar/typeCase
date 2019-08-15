#include "parameterreader0.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"

#include <boost/uuid/nil_generator.hpp>

#include <algorithm>
#include <fstream>

namespace parameter {
namespace IO {

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readParameter(
    filesystem::path path, BaseParameterReader::FileType filetype) const
{
  if (!std::filesystem::exists(path)) {
    std::cout << "file doesn't exist" << std::endl;
    return {};
  }
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

bool ParameterReader0::isFiletypeAvailable(BaseParameterReader::FileType check) const
{
  switch (check) {
  case FileType::ALGORITHM:
  case FileType::SHAPE:
  case FileType::RUN:
  case FileType::BEAMTIME:
  case FileType::DATABASE:
  case FileType::MATERIALS:
  case FileType::DETECTOR_REACTION:
    return true;
  case FileType::SETUP:
  case FileType::ALL:
  case FileType::UNDEFINED:
  case FileType::BAD_VERSION:
    return false;
  }
  return false;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readAlgorithms(
    std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t num;
  if (input.eof()) {
    return result;
  }
  input >> num;
  for (size_t i = 0; i < num; i++) {
    auto algorithm = readSingleAlgorithm(input);
    recoverAlgorithmId(algorithm);
    result.push_back(algorithm);
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readRuns(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numRuns;
  if (input.eof()) {
    return result;
  }
  input >> numRuns;
  for (size_t j = 0; j < numRuns; j++) {
    result.push_back(readSingleRun(input));
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readBeamtimes(
    std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfBeamTimes;
  if (input.eof()) {
    return result;
  }
  input >> numberOfBeamTimes;
  for (size_t i = 0; i < numberOfBeamTimes; i++) {
    result.push_back(readSingleBeamtime(input));
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readSetup(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfDetectors;
  auto col = readSingleReaction(input);
  if (input.eof()) {
    return result;
  }
  result.push_back(col);
  input >> numberOfDetectors;
  for (size_t i = 0; i < numberOfDetectors; i++) {
    result.push_back(readSingleDetector(input));
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readMaterials(
    std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t numberOfMaterials;
  if (input.eof()) {
    return result;
  }
  input >> numberOfMaterials;
  for (size_t i = 0; i < numberOfMaterials; i++) {
    result.push_back(readSingleMaterial(input));
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> ParameterReader0::readShapes(std::istream& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  size_t num;
  if (input.eof()) {
    return result;
  }
  input >> num;
  for (size_t i = 0; i < num; i++) {
    auto shape = readSingleShape(input);
    recoverShapeId(shape);
    result.push_back(shape);
    if (input.eof()) {
      return result;
    }
  }
  return result;
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleAlgorithm(istream& input) const
{
  // From read_algorithm_parameter_0
  algorithm_parameter a;
  a.setId(boost::uuids::nil_uuid());
  int zahl, z2, z3;
  float fz1, fz2, fz3;
  char c, _tmp[100];
  input >> zahl;
  a.setCategory(zahl);
  input >> zahl;
  a.setLevel(zahl);
  input >> zahl;
  a.setUse(zahl == 1);
  input >> zahl;
  a.setID(zahl);
  input >> zahl;
  for (int j = 0; j < zahl; j++) {
    input >> z2;
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<bool>(single_parameter<bool>(string(_tmp), z2 == 1));
  }
  input >> zahl;
  //    cout<<"point3D:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    input >> fz1 >> fz2 >> fz3;
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<point3D>(single_parameter<point3D>(string(_tmp), point3D(fz1, fz2, fz3)));
  }
  input >> zahl;
  //    cout<<"vector3D:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    input >> fz1 >> fz2 >> fz3;
    input.get(c);
    input.getline(_tmp, 100);
    vector3D tmpVector(fz1, fz2, fz3);
    a.addParam<vector3D>(
        single_parameter<vector3D>(string(_tmp), tmpVector)); // vector3D(fz1,fz2,fz3)));
  }
  input >> zahl;
  //    cout<<"int:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    input >> z2;
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<int>(single_parameter<int>(string(_tmp), z2));
  }
  input >> zahl;
  //    cout<<"float:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    input >> fz1;
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<float>(single_parameter<float>(string(_tmp), fz1));
  }
  input >> zahl;
  //    cout<<"string:"<<zahl<<endl;
  char _tmp2[100];
  input.get(c);
  for (int j = 0; j < zahl; j++) {
    input.getline(_tmp2, 100);
    input.getline(_tmp, 100);
    a.addParam<string>(single_parameter<string>(string(_tmp), string(_tmp2)));
  }
  input >> zahl;
  //    cout<<"vector<int>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    vector<int> tmp;
    input >> z2;
    for (int k = 0; k < z2; k++) {
      input >> z3;
      tmp.push_back(z3);
    }
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<vector<int>>(single_parameter<vector<int>>(string(_tmp), tmp));
  }
  input >> zahl;
  //    cout<<"vector<float>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    vector<float> tmp;
    input >> z2;
    for (int k = 0; k < z2; k++) {
      input >> fz1;
      tmp.push_back(fz1);
    }
    input.get(c);
    input.getline(_tmp, 100);
    a.addParam<vector<float>>(single_parameter<vector<float>>(string(_tmp), tmp));
  }
  input >> zahl;
  //    cout<<"vector<string>:"<<zahl<<endl;
  for (int j = 0; j < zahl; j++) {
    char tmp_s[1000];
    vector<string> tmp;
    input >> z2;
    input.get(c);
    for (int k = 0; k < z2; k++) {
      input.getline(tmp_s, 1000);
      tmp.push_back(string(tmp_s));
    }
    input.getline(_tmp, 100);
    a.addParam<vector<string>>(single_parameter<vector<string>>(string(_tmp), tmp));
  }
  input >> zahl;
  for (int j = 0; j < zahl; j++) {
    //      i.getline(_tmp,100);
    auto ap = readSingleAlgorithm(input);
    input.getline(_tmp, 100);
    recoverAlgorithmId(ap);
    a.addParam<algorithm_parameter>(single_parameter<algorithm_parameter>(
        ap->getName(), *std::dynamic_pointer_cast<algorithm_parameter>(ap)));
  }
  input.get(c);
  input.getline(_tmp, 100);
  //    cout << _tmp << endl;
  a.setName(string(_tmp));
  input >> zahl;
  //    cout << zahl;
  input.get(c);
  string d;
  for (int j = 0; j < zahl; j++) {
    input.getline(_tmp, 100);
    //      cout << " " << _tmp << endl;
    d += string(_tmp);
  }
  a.setDescription(d);
  return std::make_shared<algorithm_parameter>(a);
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleRun(istream& input) const
{
  // From read_run_parameter_0
  run_parameter d;
  d.setId(boost::uuids::nil_uuid());
  int zahl;
  char li[500];
  char c;
  input >> zahl;
  d.setType(static_cast<run_parameter::RunType>(zahl));
  input >> zahl;
  //        d.setParentNumber(zahl);
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
  input >> zahl;
  int zahl2;
  for (int I = 0; I < zahl; I++) {
    input >> zahl2;
    input.get(c);
    input.getline(li, 250);
    d.addFile(string(li), zahl2);
  }
  if (d.hasOwnSetup()) {
    input.getline(li, 250);
    d.setSetupFile(string(li));
  }
  if (d.hasOwnCalibration()) {
    input >> zahl;
    input.get(c);
    for (int I = 0; I < zahl; I++) {
      input.getline(li, 250);
      d.addCalibrationFile(string(li));
    }
  }
  return std::make_shared<run_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleBeamtime(istream& input) const
{
  // From istream operator >> (...)
  beamTime_parameter d;
  d.setId(boost::uuids::nil_uuid());
  int zahl, z;
  char li[250];
  char c;
  input >> zahl;
  input >> z;
  d.setData(zahl, z);
  input.getline(li, 250);
  d.setName(string(li));
  input >> zahl;
  std::string dis;
  for (int I = 0; I < zahl; I++) {
    input.getline(li, 250);
    dis = dis + std::string(li);
  }
  d.setDescription(dis);
  input.getline(li, 250);
  string s = li; // expandPath(li);
  d.setFileName(s);
  input.getline(li, 250);
  s = li; // expandPath(li);
  d.setSetupFile(s);
  input >> zahl;
  input.get(c);
  for (int I = 0; I < zahl; I++) {
    input.getline(li, 250);
    s = li; // expandPath(li);
    d.addCalibrationFile(s);
  }
  return std::make_shared<beamTime_parameter>(d);
}

std::shared_ptr<base_parameter> readReactionShape(std::istream& input)
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

std::shared_ptr<base_parameter> ParameterReader0::readSingleReaction(istream& input) const
{
  // From istream operator >>(...)
  reaction_parameter d;
  d.setId(boost::uuids::nil_uuid());
  int zahl;
  float z;
  input >> zahl;
  d.setTwoBeams(zahl == 1);
  input >> zahl;
  d.setMaterial(0, zahl);
  input >> zahl;
  d.setMaterial(1, zahl);
  input >> z;
  d.setBeamMomentum(z);
  if (d.hasTwoBeams()) {
    input >> z;
    d.setBeamMomentum(z, 1);
  }
  char li[250];
  input.getline(li, 250);
  d.setName(string(li));
  string lis;
  std::getline(input, lis);
  d.setDescription(lis);
  auto sh = readReactionShape(input);
  recoverShapeId(sh);
  d.setTargetShape(*std::dynamic_pointer_cast<shape_parameter>(sh));
  return std::make_shared<reaction_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleDetector(istream& input) const
{
  // from read_detector_parameter_0
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
    v.setValues(value1, value2, value3);
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
  auto tmp = std::make_shared<base_parameter>(sh);
  recoverShapeId(tmp);
  sh.setId(tmp->id());
  d.setShape(sh);
  d.setMaterialId(boost::uuids::nil_uuid());
  return std::make_shared<detector_parameter>(d);
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleMaterial(istream& input) const
{
  // From istream operator >> (...)
  material_parameter m;
  m.setId(boost::uuids::nil_uuid());
  int zahli;
  float zahlf;
  char c;
  char li[100];
  input >> zahli;
  m.setActive(zahli == 1);
  input >> zahlf;
  m.setDensity(zahlf);
  input >> zahlf;
  m.setRadiationLength(zahlf);
  input >> zahlf;
  m.setSpeed(zahlf);
  input.get(c);
  input.getline(li, 100);
  m.setName(string(li));
  input >> zahli;
  m.ClearEnergyLoss();
  for (int j = 0; j < zahli; j++) {
    input >> zahlf;
    m.setEnergyLoss(j, zahlf);
  }
  input >> zahli;
  for (int I = 0; I < zahli; I++) {
    element_parameter ep;
    input >> zahlf;
    ep.setMass(zahlf);
    input >> zahlf;
    ep.setCharge(zahlf);
    input >> zahlf;
    ep.setWeight(zahlf);
    input.get(c);
    input.getline(li, 100);
    ep.setName(string(li));
    m.addElement(ep);
  }
  return std::make_shared<material_parameter>(m);
}

std::shared_ptr<base_parameter> ParameterReader0::readSingleShape(istream& input) const
{
  // From read_shape_parameter_0
  shape_parameter sh;
  sh.setId(boost::uuids::nil_uuid());
  char li[100];
  int zahl[5];
  input >> zahl[0];
  input >> zahl[1];
  input >> zahl[2];
  input >> zahl[3];
  input >> zahl[4];
  input.getline(li, 100);
  sh.setName(li);
  for (int k = 0; k < zahl[0]; k++) {
    input.getline(li, 100);
    sh.addValue(std::string(li), point3D(0, 0, 0));
  }
  for (int k = 0; k < zahl[1]; k++) {
    input.getline(li, 100);
    sh.addValue(std::string(li), vector3D(0, 0, 0));
  }
  for (int k = 0; k < zahl[2]; k++) {
    input.getline(li, 100);
    sh.addValue(std::string(li), static_cast<int>(-1));
  }
  for (int k = 0; k < zahl[3]; k++) {
    input.getline(li, 100);
    sh.addValue(std::string(li), static_cast<float>(-1));
  }
  for (int k = 0; k < zahl[4]; k++) {
    input.getline(li, 100);
    sh.addValue(std::string(li), std::string{});
  }
  return std::make_shared<shape_parameter>(sh);
}

void ParameterReader0::recoverShapeId(std::shared_ptr<base_parameter>& shape) const
{
  if (_shapeRecovery) {
    _shapeRecovery(shape);
  }
}

void ParameterReader0::recoverAlgorithmId(std::shared_ptr<base_parameter>& algorithm) const
{
  if (_algorithmRecovery) {
    _algorithmRecovery(algorithm);
  }
}

void ParameterReader0::setShapeIdRecovery(
    std::function<void(std::shared_ptr<base_parameter>&)> function)
{
  _shapeRecovery = function;
}

void ParameterReader0::setAlgorithmIdRecovery(
    std::function<void(std::shared_ptr<base_parameter>&)> function)
{
  _algorithmRecovery = function;
}

BaseParameterReader::FileType ParameterReader0::checkVersionString(string const&) const
{
  return FileType::UNDEFINED;
}
} // namespace IO
} // namespace parameter
