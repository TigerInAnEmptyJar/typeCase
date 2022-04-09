#include "parameterwriter1.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"

#include <QtGlobal>

#include <fstream>

namespace parameter {
namespace IO {

void ParameterWriter1::writeAlgorithms(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << getVersionString(FileType::ALGORITHM);
  ParameterWriter0::writeAlgorithms(output, items);
}

void ParameterWriter1::writeDetectors(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << getVersionString(FileType::DETECTOR_REACTION);
  ParameterWriter0::writeDetectors(output, items);
}

void ParameterWriter1::writeShapes(std::ostream& output,
                                   std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << getVersionString(FileType::SHAPE);
  ParameterWriter0::writeShapes(output, items);
}

void ParameterWriter1::writeSingleAlgorithm(std::ostream& output,
                                            std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto algorithm = std::dynamic_pointer_cast<algorithm_parameter>(item);
  if (!algorithm) {
    return;
  }
  output << "===== " << algorithm->getName().data() << " ====="
         << "\n";
  output << "#" << algorithm->getID() << " ";
  output << (algorithm->IsUsed() ? 1 : 0) << " ";
  output << algorithm->getName().data() << "\n";
  output << "#" << static_cast<int>(algorithm->getCategory()) << " ";
  output << algorithm->getLevel() << " "
         << "\n";
  output << "D" << algorithm->getDescription() << "\n";
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (size_t i = 0; i < algorithm->numberOfValues(); i++) {
    paramMap[algorithm->value(i).valueType()].push_back(
        std::make_pair(algorithm->valueName(i), algorithm->value(i)));
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::BOOLEAN].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::BOOLEAN][k];
    output << "B" << (v.second.value<bool>() ? "1" : "0") << " " << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::POINT3D][k];
    output << "P" << v.second.value<point3D>().x() << " " << v.second.value<point3D>().y() << " "
           << v.second.value<point3D>().z() << " " << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR3D][k];
    output << "V" << v.second.value<vector3D>().x() << " " << v.second.value<vector3D>().y() << " "
           << v.second.value<vector3D>().z() << " " << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::INT][k];
    output << "I" << v.second.value<int>() << " " << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::FLOAT][k];
    output << "F" << v.second.value<float>() << " " << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::STRING][k];
    output << "S" << v.second.value<std::string>() << "\n" << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_INT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR_INT][k];
    auto p = v.second.value<std::vector<int>>();
    output << "N" << p.size() << " ";
    for (auto e : p) {
      output << e << " ";
    }
    output << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_FLOAT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR_FLOAT][k];
    auto p = v.second.value<std::vector<float>>();
    output << "T" << p.size() << " ";
    for (auto e : p) {
      output << e << " ";
    }
    output << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_STRING].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR_STRING][k];
    auto p = v.second.value<std::vector<std::string>>();
    output << "R" << p.size() << " ";
    for (auto e : p) {
      output << e << "\n";
    }
    output << v.first << "\n";
  }
  // o<<a.getNumberOfParam<algorithm_parameter>() <<" ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::ALGORITHM].size(); k++) {
    output << "A";
    writeSingleAlgorithm(output, paramMap[ParameterValue::ValueType::ALGORITHM][k]
                                     .second.value<std::shared_ptr<algorithm_parameter>>());
    output << " " << paramMap[ParameterValue::ValueType::ALGORITHM][k].first << "\n";
  }
}

void ParameterWriter1::writeSingleRun(std::ostream& output,
                                      std::shared_ptr<base_parameter> const& item) const
{
  // from write_run_parameter_1
  auto run = std::dynamic_pointer_cast<run_parameter>(item);
  if (!run) {
    return;
  }
  output << static_cast<int>(run->getType()) << " " << 0 << " " << run->getRunNumber() << " ";
  if (run->hasOwnSetup())
    output << 1 << " ";
  else
    output << 0 << " ";
  if (run->hasOwnCalibration())
    output << 1;
  else
    output << 0;
  output << run->getName().data() << "\n";
  string dis = run->getDescription();
  output << 1;
  output << dis << "\n";
  output << " " << run->getNumberOfFiles() << " ";
  for (size_t i = 0; i < run->getNumberOfFiles(); i++)
    output << run->getFileType(i) << " " << run->getFileEvents(i) << " " << run->getFile(i).native()
           << "\n";
  //    o<<d.getFileName().data()<<endl;
  if (run->hasOwnSetup())
    output << run->getSetupFile().native() << "\n";
  if (run->hasOwnCalibration()) {
    output << run->getNumberOfCalibrationFiles() << " ";
    for (size_t i = 0; i < run->getNumberOfCalibrationFiles(); i++) {
      output << run->getCalibrationFile(i).native() << "\n";
    }
  }
}

void ParameterWriter1::writeSingleDetector(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from write_detector_parameter_1
  auto detector = std::dynamic_pointer_cast<detector_parameter>(item);
  if (!detector) {
    return;
  }
  output << detector->getNumberOfElements() << " " << detector->getStackType() << " "
         << detector->getID() << " " << detector->getMaterial() << " " << detector->getMaxDistance()
         << detector->getName().data() << "\n";
  auto sh = detector->getShape();
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (size_t i = 0; i < sh->numberOfValues(); i++) {
    paramMap[sh->value(i).valueType()].push_back(std::make_pair(sh->valueName(i), sh->value(i)));
  }
  output << paramMap[ParameterValue::ValueType::POINT3D].size() << " ";
  output << paramMap[ParameterValue::ValueType::VECTOR3D].size() << " ";
  output << paramMap[ParameterValue::ValueType::INT].size() << " ";
  output << paramMap[ParameterValue::ValueType::FLOAT].size() << " ";
  output << paramMap[ParameterValue::ValueType::STRING].size() << sh->getName().data() << "\n";
  point3D p;
  vector3D v;
  auto pec = output.precision();
  output.precision(10);
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::POINT3D][k];
    p = v.second.value<point3D>();
    output << p.X() << " " << p.Y() << " " << p.Z() << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR3D][k];
    auto vv = v.second.value<vector3D>();
    output << vv.Theta() << " " << vv.Phi() << " " << vv.R() << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::INT][k];
    output << v.second.value<int>() << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::FLOAT][k];
    output << v.second.value<float>() << v.first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::STRING][k];
    output << v.second.value<string>() << "\n" << v.first << "\n";
  }
  output.precision(pec);
  output.flush();
}

void ParameterWriter1::writeSingleShape(std::ostream& output,
                                        std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto shape = std::dynamic_pointer_cast<shape_parameter>(item);
  if (!shape) {
    return;
  }
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (size_t i = 0; i < shape->numberOfValues(); i++) {
    paramMap[shape->value(i).valueType()].push_back(
        std::make_pair(shape->valueName(i), shape->value(i)));
  }
  output << shape->getName().data() << "\n";
  if (shape->completeWrite()) {
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
      auto v = paramMap[ParameterValue::ValueType::POINT3D][k];
      output << "P" << v.second.value<point3D>().x() << " " << v.second.value<point3D>().y() << " "
             << v.second.value<point3D>().z() << " " << v.first << "\n";
    }
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
      auto v = paramMap[ParameterValue::ValueType::VECTOR3D][k];
      output << "v" << v.second.value<vector3D>().Theta() * 180. / M_PI << " "
             << v.second.value<vector3D>().Phi() * 180. / M_PI << " "
             << v.second.value<vector3D>().R() << " " << v.first << "\n";
    }
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++) {
      auto v = paramMap[ParameterValue::ValueType::INT][k];
      output << "I" << v.second.value<int>() << " " << v.first << "\n";
    }
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++) {
      auto v = paramMap[ParameterValue::ValueType::FLOAT][k];
      output << "F" << v.second.value<float>() << " " << v.first << "\n";
    }
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++) {
      auto v = paramMap[ParameterValue::ValueType::STRING][k];
      output << "S" << v.second.value<string>() << "\n" << v.first << "\n";
    }
  } else {
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++)
      output << "P " << paramMap[ParameterValue::ValueType::POINT3D][k].first << "\n";
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++)
      output << "V " << paramMap[ParameterValue::ValueType::VECTOR3D][k].first << "\n";
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++)
      output << "I " << paramMap[ParameterValue::ValueType::INT][k].first << "\n";
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++)
      output << "F " << paramMap[ParameterValue::ValueType::FLOAT][k].first << "\n";
    for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++)
      output << "S " << paramMap[ParameterValue::ValueType::STRING][k].first << "\n";
  }
  output << "="
         << "\n"
         << std::flush;
}

string ParameterWriter1::getVersionString(BaseParameterWriter::FileType filetype) const
{
  switch (filetype) {
  case FileType::ALGORITHM:
    return "########## algorithm parameter ####### file version :1 #######\n";
  case FileType::DETECTOR_REACTION:
    return "########## detector parameter ####### file version :1 #######\n";
  case FileType::SHAPE:
    return "########## shape parameter ####### file version :1 #######\n";
  default:
    break;
  }
  return {};
}

} // namespace IO
} // namespace parameter
