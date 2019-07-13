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
  output << "#" << algorithm->getCategory() << " ";
  output << algorithm->getLevel() << " "
         << "\n";
  output << "D" << algorithm->getDescription() << "\n";
  //      o<<a.getNumberOfParam<bool>() <<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<bool>(); i++)
    output << "B" << (algorithm->getParam<bool>(i).getData() ? "1" : "0") << " "
           << (algorithm->getParam<bool>(i).getName().data()) << "\n";
  // o<<a.getNumberOfParam<point3D>()<<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<point3D>(); i++) {
    point3D tmpP = algorithm->getParam<point3D>(i).getData();
    output << "P" << tmpP.X() << " " << tmpP.Y() << " " << tmpP.Z() << " "
           << algorithm->getParam<point3D>(i).getName().data() << "\n";
  }
  // o<<a.getNumberOfParam<vector3D>()<<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<vector3D>(); i++) {
    vector3D tmpP = algorithm->getParam<vector3D>(i).getData();
    output << "V" << tmpP.X() << " " << tmpP.Y() << " " << tmpP.Z() << " "
           << algorithm->getParam<vector3D>(i).getName().data() << "\n";
  }
  // o<<a.getNumberOfParam<int>()<<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<int>(); i++)
    output << "I" << algorithm->getParam<int>(i).getData() << " "
           << algorithm->getParam<int>(i).getName().data() << "\n";
  // o<<a.getNumberOfParam<float>()<<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<float>(); i++)
    output << "F" << algorithm->getParam<float>(i).getData() << " "
           << algorithm->getParam<float>(i).getName().data() << "\n";
  // o<<a.getNumberOfParam<string>()<<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<string>(); i++)
    output << "S" << algorithm->getParam<string>(i).getData().data() << "\n"
           << algorithm->getParam<string>(i).getName().data() << "\n";
  // o<< a.getNumberOfParam<vector<int> >()<<" ";
  for (int j = 0; j < algorithm->getNumberOfParam<vector<int>>(); j++) {
    algorithm->getParam<vector<int>>(j).getData();
    output << "N" << algorithm->getParam<vector<int>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < algorithm->getParam<vector<int>>(j).getData().size(); k++) {
      output << algorithm->getParam<vector<int>>(j).getData().at(k) << " ";
    }
    output << algorithm->getParam<vector<int>>(j).getName().data() << "\n";
  }
  // o<< a.getNumberOfParam<vector<float> >()<<" ";
  for (int j = 0; j < algorithm->getNumberOfParam<vector<float>>(); j++) {
    algorithm->getParam<vector<float>>(j).getData();
    output << "T" << algorithm->getParam<vector<float>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < algorithm->getParam<vector<float>>(j).getData().size(); k++) {
      output << algorithm->getParam<vector<float>>(j).getData().at(k) << " ";
    }
    output << algorithm->getParam<vector<float>>(j).getName().data() << "\n";
  }
  // o<< a.getNumberOfParam<vector<string> >()<<" ";
  for (int j = 0; j < algorithm->getNumberOfParam<vector<string>>(); j++) {
    algorithm->getParam<vector<string>>(j).getData();
    output << "R" << algorithm->getParam<vector<string>>(j).getData().size() << " ";
    for (unsigned int k = 0; k < algorithm->getParam<vector<string>>(j).getData().size(); k++) {
      output << algorithm->getParam<vector<string>>(j).getData().at(k) << "\n";
    }
    output << algorithm->getParam<vector<string>>(j).getName().data() << "\n";
  }
  // o<<a.getNumberOfParam<algorithm_parameter>() <<" ";
  for (int i = 0; i < algorithm->getNumberOfParam<algorithm_parameter>(); i++) {
    output << "A";
    writeSingleAlgorithm(output, std::make_shared<algorithm_parameter>(
                                     algorithm->getParam<algorithm_parameter>(i).getData()));
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
  shape_parameter sh;
  sh = detector->getShape();
  int zahl = sh.NumberOfParams<point3D>();
  output << (zahl) << " ";
  zahl = sh.NumberOfParams<vector3D>();
  output << zahl << " ";
  zahl = sh.NumberOfParams<int>();
  output << zahl << " ";
  zahl = sh.NumberOfParams<float>();
  output << zahl << " ";
  zahl = sh.NumberOfParams<string>();
  output << zahl;
  output << sh.getName().data();
  output << "\n";
  point3D p;
  vector3D v;
  auto pec = output.precision();
  output.precision(10);
  for (int k = 0; k < sh.NumberOfParams<point3D>(); k++) {
    p = sh.getParam<point3D>(k);
    output << p.X() << " " << p.Y() << " " << p.Z() << sh.getParamName<point3D>(k).data() << "\n";
  }
  for (int k = 0; k < sh.NumberOfParams<vector3D>(); k++) {
    v = sh.getParam<vector3D>(k);
    output << v.Theta() << " " << v.Phi() << " " << v.R() << sh.getParamName<vector3D>(k).data()
           << "\n";
  }
  for (int k = 0; k < sh.NumberOfParams<int>(); k++) {
    output << sh.getParam<int>(k) << sh.getParamName<int>(k).data() << "\n";
  }
  for (int k = 0; k < sh.NumberOfParams<float>(); k++) {
    output << sh.getParam<float>(k) << sh.getParamName<float>(k).data() << "\n";
  }
  for (int k = 0; k < sh.NumberOfParams<string>(); k++) {
    output << sh.getParam<string>(k).data() << "\n" << sh.getParamName<string>(k).data() << "\n";
  }
  output.precision(pec);
}

void ParameterWriter1::writeSingleShape(std::ostream& output,
                                        std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto shape = std::dynamic_pointer_cast<shape_parameter>(item);
  if (!shape) {
    return;
  }
  output << shape->getName().data() << "\n";
  if (shape->completeWrite()) {
    for (int k = 0; k < shape->NumberOfParams<point3D>(); k++)
      output << "P" << shape->getParam<point3D>(k).x() << " " << shape->getParam<point3D>(k).y()
             << " " << shape->getParam<point3D>(k).z() << " "
             << shape->getParamName<point3D>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<vector3D>(); k++)
      output << "v" << shape->getParam<vector3D>(k).Theta() * 180. / M_PI << " "
             << shape->getParam<vector3D>(k).Phi() * 180. / M_PI << " "
             << shape->getParam<vector3D>(k).R() << " " << shape->getParamName<vector3D>(k).data()
             << "\n";
    for (int k = 0; k < shape->NumberOfParams<float>(); k++)
      output << "F" << shape->getParam<float>(k) << " " << shape->getParamName<float>(k).data()
             << "\n";
    for (int k = 0; k < shape->NumberOfParams<int>(); k++)
      output << "I" << shape->getParam<int>(k) << " " << shape->getParamName<int>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<string>(); k++)
      output << "S" << shape->getParam<string>(k) << "\n"
             << shape->getParamName<string>(k).data() << "\n";
  } else {
    for (int k = 0; k < shape->NumberOfParams<point3D>(); k++)
      output << "P " << shape->getParamName<point3D>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<vector3D>(); k++)
      output << "V " << shape->getParamName<vector3D>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<float>(); k++)
      output << "F " << shape->getParamName<float>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<int>(); k++)
      output << "I " << shape->getParamName<int>(k).data() << "\n";
    for (int k = 0; k < shape->NumberOfParams<string>(); k++)
      output << "S " << shape->getParamName<string>(k).data() << "\n";
  }
  output << "="
         << "\n";
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
