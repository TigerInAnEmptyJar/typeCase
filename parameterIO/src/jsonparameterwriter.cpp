#include "jsonparameterwriter.h"
#include "jsonKeys.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <boost/uuid/uuid_io.hpp>

#include <algorithm>
#include <fstream>

namespace {

void addBaseParameter(std::shared_ptr<base_parameter> const& base, QJsonObject& output)
{
  output.insert(Keys::name, QString::fromStdString(base->getName()));
  output.insert(Keys::description, QString::fromStdString(base->getDescription()));
  output.insert(Keys::base_id, QString::fromStdString(boost::uuids::to_string(base->id())));
}

QJsonObject toDate(tm const& date)
{
  QJsonObject result;
  result.insert(Keys::year, date.tm_year);
  result.insert(Keys::month, date.tm_mon);
  result.insert(Keys::day, date.tm_mday);
  result.insert(Keys::hour, date.tm_hour);
  result.insert(Keys::minute, date.tm_min);
  result.insert(Keys::second, date.tm_sec);
  return result;
}

QString fromPoint(point3D const& p) { return QString{"%1,%2,%3"}.arg(p.x()).arg(p.y()).arg(p.z()); }
QString fromVector(vector3D const& v)
{
  return QString{"%1,%2,%3"}.arg(v.x()).arg(v.y()).arg(v.z());
}
template <typename T>
QString fromVector(std::vector<T> const& v)
{
  QString s;
  for (auto element : v) {
    s = s + QString{"%1,"}.arg(element);
  }
  if (s.length() > 0) {
    s = s.left(s.length() - 1);
  }
  return s;
}
template <>
QString fromVector(std::vector<std::string> const& v)
{
  QString s;
  for (auto element : v) {
    s = s + QString{"%1,"}.arg(QString::fromStdString(element));
  }
  if (s.length() > 0) {
    s = s.left(s.length() - 1);
  }
  return s;
}

QJsonObject makeParameter(ParameterValue const& param, std::string const& name)
{
  QJsonObject result;
  result.insert(Keys::pName, QString::fromStdString(name));
  result.insert(Keys::pType, static_cast<int>(param.valueType()));
  switch (param.valueType()) {
  case ParameterValue::ValueType::BOOLEAN:
    result.insert(Keys::pValue, param.value<bool>());
    break;
  case ParameterValue::ValueType::INT:
    result.insert(Keys::pValue, param.value<int>());
    break;
  case ParameterValue::ValueType::FLOAT:
    result.insert(Keys::pValue, static_cast<double>(param.value<float>()));
    break;
  case ParameterValue::ValueType::DOUBLE:
    result.insert(Keys::pValue, param.value<double>());
    break;
  case ParameterValue::ValueType::POINT3D:
    result.insert(Keys::pValue, ::fromPoint(param.value<point3D>()));
    break;
  case ParameterValue::ValueType::VECTOR3D:
    result.insert(Keys::pValue, ::fromVector(param.value<vector3D>()));
    break;
  case ParameterValue::ValueType::STRING:
    result.insert(Keys::pValue, QString::fromStdString(param.value<std::string>()));
    break;
  case ParameterValue::ValueType::VECTOR_INT:
    result.insert(Keys::pValue, ::fromVector(param.value<std::vector<int>>()));
    break;
  case ParameterValue::ValueType::VECTOR_FLOAT:
    result.insert(Keys::pValue, ::fromVector(param.value<std::vector<float>>()));
    break;
  case ParameterValue::ValueType::VECTOR_DOUBLE:
    result.insert(Keys::pValue, ::fromVector(param.value<std::vector<double>>()));
    break;
  case ParameterValue::ValueType::VECTOR_STRING:
    result.insert(Keys::pValue, ::fromVector(param.value<std::vector<std::string>>()));
    break;
  default:
    break;
  }
  return result;
}
template <typename T>
QJsonObject makeParameter(T const& param, std::string const& name, ParameterValue::ValueType tpe)
{
  QJsonObject result;
  result.insert(Keys::pName, QString::fromStdString(name));
  result.insert(Keys::pType, static_cast<int>(tpe));
  result.insert(Keys::pValue, param);
  return result;
}
} // namespace

namespace parameter {
namespace IO {

void JsonParameterWriter::writeParameter(std::vector<std::shared_ptr<base_parameter>> const& items,
                                         filesystem::path path,
                                         BaseParameterWriter::FileType filetype) const
{
  QJsonObject root;

  QJsonArray algorithms;
  QJsonArray shapes;
  QJsonArray beamtimes;
  QJsonArray runs;
  QJsonArray detectors;
  QJsonArray materials;
  QJsonArray reactions;

  for (auto& element : items) {
    if (std::dynamic_pointer_cast<algorithm_parameter>(element) &&
        (filetype == FileType::ALGORITHM || filetype == FileType::ALL)) {
      algorithms.append(fromAlgorithm(element));
    }
    if (std::dynamic_pointer_cast<shape_parameter>(element) &&
        (filetype == FileType::SHAPE || filetype == FileType::ALL)) {
      shapes.append(fromShape(element));
    }
    if (std::dynamic_pointer_cast<reaction_parameter>(element) &&
        (filetype == FileType::SETUP || filetype == FileType::DETECTOR_REACTION ||
         filetype == FileType::ALL)) {
      reactions.append(fromReaction(element));
    }
    if (std::dynamic_pointer_cast<detector_parameter>(element) &&
        (filetype == FileType::SETUP || filetype == FileType::DETECTOR_REACTION ||
         filetype == FileType::ALL)) {
      detectors.append(fromDetector(element));
    }
    if (std::dynamic_pointer_cast<material_parameter>(element) &&
        (filetype == FileType::SETUP || filetype == FileType::MATERIALS ||
         filetype == FileType::ALL)) {
      materials.append(fromMaterial(element));
    }
    if (std::dynamic_pointer_cast<run_parameter>(element) &&
        (filetype == FileType::RUN || filetype == FileType::DATABASE ||
         filetype == FileType::ALL)) {
      runs.append(fromRun(element));
    }
    if (std::dynamic_pointer_cast<beamTime_parameter>(element) &&
        !std::dynamic_pointer_cast<run_parameter>(element) &&
        (filetype == FileType::BEAMTIME || filetype == FileType::DATABASE ||
         filetype == FileType::ALL)) {
      beamtimes.append(fromBeamtime(element));
    }
  }
  if (algorithms.count() > 0) {
    root.insert(Keys::algorithms, algorithms);
  }
  if (shapes.count() > 0) {
    root.insert(Keys::shapes, shapes);
  }
  if (beamtimes.count() > 0) {
    root.insert(Keys::beamtimes, beamtimes);
  }
  if (runs.count() > 0) {
    root.insert(Keys::runs, runs);
  }
  if (materials.count() > 0) {
    root.insert(Keys::materials, materials);
  }
  if (detectors.count() > 0) {
    root.insert(Keys::detectors, detectors);
  }
  if (reactions.count() > 0) {
    root.insert(Keys::reactions, reactions);
  }

  QJsonDocument jdoc;
  jdoc.setObject(root);

  if (!std::filesystem::exists(path.parent_path())) {
    std::filesystem::create_directories(path.parent_path());
  }
  std::ofstream output;
  output.open(path.native());
  output << jdoc.toJson().toStdString() << std::flush;
  output.close();
}

bool JsonParameterWriter::isFiletypeAvailable(BaseParameterWriter::FileType check) const
{
  switch (check) {
  case FileType::ALGORITHM:
  case FileType::SHAPE:
  case FileType::RUN:
  case FileType::BEAMTIME:
  case FileType::DATABASE:
  case FileType::MATERIALS:
  case FileType::DETECTOR_REACTION:
  case FileType::SETUP:
  case FileType::ALL:
    return true;
  case FileType::UNDEFINED:
    return false;
  }
  return false;
}

string JsonParameterWriter::getVersionString(BaseParameterWriter::FileType filetype) const
{
  Q_UNUSED(filetype);
  return {};
}

QJsonObject JsonParameterWriter::fromAlgorithm(std::shared_ptr<base_parameter> const& input) const
{
  std::cout << "write algorithm " << input->getName() << std::endl;
  auto algorithm = std::dynamic_pointer_cast<algorithm_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::algorithm);

  ::addBaseParameter(input, result);
  result.insert(Keys::use, algorithm->IsUsed());
  result.insert(Keys::level, algorithm->getLevel());
  result.insert(Keys::category, static_cast<int>(algorithm->getCategory()));
  result.insert(Keys::id_number, algorithm->getID());
  QJsonArray parameter;
  for (size_t i = 0; i < algorithm->numberOfValues(); i++) {
    parameter.append(::makeParameter(algorithm->value(i), algorithm->valueName(i)));
  }
  //    for (int i = 0; i < algorithm->getNumberOfParam<vector3D>(); i++) {
  //        parameter.append(::makeParameter(algorithm->getParam<vector3D>(i).getData(),
  //                                                  algorithm->getParam<vector3D>(i).getName(),
  //                                                  ParameterValue::ValueType::VECTOR_ALGORITHM));
  //    }
  result.insert(Keys::parameter, parameter);
  return result;
}

QJsonObject JsonParameterWriter::fromShape(std::shared_ptr<base_parameter> const& input) const
{
  auto shape = std::dynamic_pointer_cast<shape_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::shape);

  ::addBaseParameter(input, result);
  QJsonArray parameter;
  for (size_t i = 0; i < shape->numberOfValues(); i++) {
    parameter.append(::makeParameter(shape->value(i), shape->valueName(i)));
  }
  result.insert(Keys::parameter, parameter);

  return result;
}

QJsonObject JsonParameterWriter::fromBeamtime(std::shared_ptr<base_parameter> const& input) const
{
  auto beamtime = std::dynamic_pointer_cast<beamTime_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::beamtime);

  ::addBaseParameter(input, result);
  result.insert(Keys::runFile, QString::fromStdString(beamtime->getFileName().native()));
  result.insert(Keys::setupFile, QString::fromStdString(beamtime->getSetupFile().native()));
  result.insert(Keys::month, beamtime->getMonth());
  result.insert(Keys::year, beamtime->getYear());
  QJsonArray calibration;
  for (size_t i = 0; i < beamtime->getNumberOfCalibrationFiles(); i++) {
    calibration.append(QString::fromStdString(beamtime->getCalibrationFile(i).native()));
  }
  result.insert(Keys::calibrationFile, calibration);

  return result;
}

QJsonObject JsonParameterWriter::fromRun(std::shared_ptr<base_parameter> const& input) const
{
  auto run = std::dynamic_pointer_cast<run_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::run);

  ::addBaseParameter(input, result);
  result.insert(Keys::runType, static_cast<int>(run->getType()));
  result.insert(Keys::runNumber, run->getRunNumber());
  result.insert(Keys::start, ::toDate(run->getStartTime()));
  result.insert(Keys::stop, ::toDate(run->getStopTime()));
  if (run->getParent()) {
    result.insert(Keys::parent,
                  QString::fromStdString(boost::uuids::to_string(run->getParent()->id())));
  } else {
    result.insert(Keys::parent,
                  QString::fromStdString(boost::uuids::to_string(run->getParentNumber())));
  }
  if (run->hasOwnSetup()) {
    result.insert(Keys::setupFile, QString::fromStdString(run->getSetupFile().native()));
  }
  if (run->hasOwnCalibration() || run->hasAdditionalCalibration()) {
    result.insert(Keys::own, run->hasOwnCalibration());
    if (run->getNumberOfCalibrationFiles() > 0) {
      QJsonArray calibration;
      for (size_t i = 0; i < run->getNumberOfCalibrationFiles(); i++) {
        calibration.append(QString::fromStdString(run->getCalibrationFile(i).native()));
      }
      result.insert(Keys::calibrationFile, calibration);
    }
  }
  if (run->getNumberOfFiles() > 0) {
    QJsonArray files;
    for (size_t i = 0; i < run->getNumberOfFiles(); i++) {
      QJsonObject singleFile;
      singleFile.insert(Keys::run_name, QString::fromStdString(run->getFile(i).native()));
      singleFile.insert(Keys::run_fileType, static_cast<int>(run->getFileType(i)));
      if (run->getFileEvents(i) > 0) {
        singleFile.insert(Keys::run_events, static_cast<qint64>(run->getFileEvents(i)));
      }
      files.append(singleFile);
    }
    result.insert(Keys::run_files, files);
  }

  return result;
}

QJsonObject JsonParameterWriter::fromDetector(std::shared_ptr<base_parameter> const& input) const
{
  auto detector = std::dynamic_pointer_cast<detector_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::detector);

  ::addBaseParameter(input, result);

  result.insert(Keys::number_of_elements, detector->getNumberOfElements());
  result.insert(Keys::stack_type, detector->getStackType());
  result.insert(Keys::detector_number, detector->getID());
  result.insert(Keys::max_distance, static_cast<double>(detector->getMaxDistance()));
  result.insert(Keys::is_circular, detector->isCircular());
  auto tmp = std::make_shared<shape_parameter>(detector->getShape());
  result.insert(Keys::detector_shape, fromShape(tmp));
  if (detector->material() != nullptr) {
    result.insert(Keys::material_id,
                  QString::fromStdString(boost::uuids::to_string(detector->material()->id())));
  } else {
    result.insert(Keys::material_id,
                  QString::fromStdString(boost::uuids::to_string(detector->getMaterialId())));
  }

  return result;
}

QJsonObject JsonParameterWriter::fromMaterial(std::shared_ptr<base_parameter> const& input) const
{
  auto material = std::dynamic_pointer_cast<material_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::material);

  ::addBaseParameter(input, result);
  result.insert(Keys::is_active, material->IsActive());
  result.insert(Keys::speed, static_cast<double>(material->Speed()));
  result.insert(Keys::density, static_cast<double>(material->Density()));
  result.insert(Keys::radiation_length, static_cast<double>(material->RadiationLength()));

  QJsonArray energyLoss;
  for (int i = 0; i < material->NumberOfEnergyLossParams(); i++) {
    energyLoss.append(static_cast<double>(material->EnergyLoss(i)));
  }
  result.insert(Keys::energyloss, energyLoss);

  QJsonArray elements;
  for (int i = 0; i < material->NumberOfElements(); i++) {
    QJsonObject element;
    element.insert(Keys::element_mass, static_cast<double>(material->Element(i).getMass()));
    element.insert(Keys::element_charge, static_cast<double>(material->Element(i).getCharge()));
    element.insert(Keys::element_weight, static_cast<double>(material->Element(i).getWeight()));
    elements.append(element);
  }
  result.insert(Keys::elements, elements);
  return result;
}

QJsonObject JsonParameterWriter::fromReaction(std::shared_ptr<base_parameter> const& input) const
{
  auto reaction = std::dynamic_pointer_cast<reaction_parameter>(input);
  QJsonObject result;
  result.insert(Keys::parameterType, Keys::reaction);

  ::addBaseParameter(input, result);
  result.insert(Keys::two_beams, reaction->hasTwoBeams());
  result.insert(Keys::beam_momentum_1, static_cast<double>(reaction->getBeamMomentum(0)));
  result.insert(Keys::beam_material_id,
                QString::fromStdString(boost::uuids::to_string(reaction->getMaterialId(0))));
  result.insert(Keys::target_material_id,
                QString::fromStdString(boost::uuids::to_string(reaction->getMaterialId(1))));

  if (!reaction->hasTwoBeams()) {
    result.insert(Keys::beam_momentum_2, static_cast<double>(reaction->getBeamMomentum(1)));
    auto tmp = std::make_shared<shape_parameter>(reaction->getTargetShape());
    result.insert(Keys::target_shape, fromShape(tmp));
  }
  return result;
}

} // namespace IO
} // namespace parameter
