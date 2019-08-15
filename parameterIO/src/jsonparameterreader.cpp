#include "jsonparameterreader.h"
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

#include <boost/uuid/string_generator.hpp>

#include <algorithm>
#include <boost/uuid/uuid_io.hpp>
#include <fstream>
#include <functional>
#include <map>

namespace {
bool addBasic(std::shared_ptr<base_parameter> parameter, QJsonObject const& object)
{
  if (!(object.contains(Keys::name) && object.contains(Keys::base_id))) {
    return false;
  }
  parameter->setName(object.value(Keys::name).toString().toStdString());
  parameter->setId(
      boost::uuids::string_generator()(object.value(Keys::base_id).toString().toStdString()));
  if (object.contains(Keys::description)) {
    parameter->setDescription(object.value(Keys::description).toString().toStdString());
  }
  return true;
}

bool readDate(tm& out, QJsonObject const& object)
{
  if (!(object.contains(Keys::month) && object.contains(Keys::year) && object.contains(Keys::day) &&
        object.contains(Keys::hour) && object.contains(Keys::minute) &&
        object.contains(Keys::second))) {
    return false;
  }
  out.tm_year = object.value(Keys::year).toInt(-1);
  out.tm_mon = object.value(Keys::month).toInt(-1);
  out.tm_mday = object.value(Keys::day).toInt(-1);
  out.tm_hour = object.value(Keys::hour).toInt(-1);
  out.tm_min = object.value(Keys::minute).toInt(-1);
  out.tm_sec = object.value(Keys::second).toInt(-1);
  return true;
}

std::vector<std::string> splitString(std::string toSplit, char const& token)
{
  std::vector<std::string> result;
  while (toSplit.find(token) < std::string::npos) {
    result.push_back(toSplit.substr(0, toSplit.find(token)));
    toSplit = toSplit.substr(toSplit.find(token) + 1);
  }
  result.push_back(toSplit);
  return result;
}

point3D toPoint(std::string const& pointString)
{
  auto splitted = splitString(pointString, ',');
  if (splitted.size() < 3) {
    return {};
  }
  return point3D(stod(splitted[0]), stod(splitted[1]), stod(splitted[2]));
}
vector3D toVector(std::string const& pointString)
{
  auto splitted = splitString(pointString, ',');
  if (splitted.size() < 3) {
    return {};
  }
  return vector3D(stod(splitted[0]), stod(splitted[1]), stod(splitted[2]));
}
std::vector<int> toIntList(std::string const& pointString)
{
  auto splitted = splitString(pointString, ',');
  std::vector<int> result;
  std::transform(splitted.begin(), splitted.end(), std::back_inserter(result),
                 [](auto element) { return stoi(element); });
  return result;
}
std::vector<float> toFloatList(std::string const& pointString)
{
  auto splitted = splitString(pointString, ',');
  std::vector<float> result;
  std::transform(splitted.begin(), splitted.end(), std::back_inserter(result),
                 [](auto element) { return stof(element); });
  return result;
}
[[maybe_unused]] std::vector<double> toDoubleList(std::string const& pointString)
{
  auto splitted = splitString(pointString, ',');
  std::vector<double> result;
  std::transform(splitted.begin(), splitted.end(), std::back_inserter(result),
                 [](auto element) { return stod(element); });
  return result;
}
std::vector<std::string> toStringList(std::string const& pointString)
{
  return splitString(pointString, ',');
}
void addParentToRun(std::vector<std::shared_ptr<base_parameter>> const& stuff,
                    std::shared_ptr<base_parameter> const& element)
{
  auto run = std::dynamic_pointer_cast<run_parameter>(element);
  if (!run) {
    return;
  }
  auto parent = std::find_if(stuff.begin(), stuff.end(), [id = run->getParentNumber()](auto item) {
    if (auto beamtime = std::dynamic_pointer_cast<beamTime_parameter>(item)) {
      return id == beamtime->id();
    }
    return false;
  });
  if (parent != stuff.end()) {
    run->setParent(std::dynamic_pointer_cast<beamTime_parameter>(*parent));
  }
}
void addMaterialToDetector(std::vector<std::shared_ptr<base_parameter>> const& stuff,
                           std::shared_ptr<base_parameter> const& element)
{
  auto detector = std::dynamic_pointer_cast<detector_parameter>(element);
  if (!detector) {
    return;
  }
  auto material =
      std::find_if(stuff.begin(), stuff.end(), [id = detector->getMaterialId()](auto item) {
        if (auto material = std::dynamic_pointer_cast<material_parameter>(item)) {
          return id == material->id();
        }
        return false;
      });
  if (material != stuff.end()) {
    detector->setMaterial(std::dynamic_pointer_cast<material_parameter>(*material).get());
  }
}
} // namespace

namespace parameter {
namespace IO {

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::readParameter(
    filesystem::path path, BaseParameterReader::FileType filetype) const
{
  if (!std::filesystem::exists(path)) {
    return {};
  }
  std::ifstream input;
  input.open(path.native());
  std::string content;
  while (!input.eof()) {
    std::string part;
    std::getline(input, part);
    content += part;
  }
  input.close();

  QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray::fromStdString(content));
  if (!jdoc.isObject()) {
    return {};
  }
  std::vector<std::shared_ptr<base_parameter>> result;

  switch (filetype) {
  case FileType::ALGORITHM:
    if (jdoc.object().contains(Keys::algorithms) &&
        jdoc.object().value(Keys::algorithms).isArray()) {
      auto tmp = extractAlgorithms(jdoc.object().value(Keys::algorithms).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::SHAPE:
    if (jdoc.object().contains(Keys::shapes) && jdoc.object().value(Keys::shapes).isArray()) {
      auto tmp = extractShapes(jdoc.object().value(Keys::shapes).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::RUN:
    if (jdoc.object().contains(Keys::runs) && jdoc.object().value(Keys::runs).isArray()) {
      auto tmp = extractRuns(jdoc.object().value(Keys::runs).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::BEAMTIME:
    if (jdoc.object().contains(Keys::beamtimes) && jdoc.object().value(Keys::beamtimes).isArray()) {
      auto tmp = extractBeamtimes(jdoc.object().value(Keys::beamtimes).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::DATABASE:
    if (jdoc.object().contains(Keys::beamtimes) && jdoc.object().value(Keys::beamtimes).isArray()) {
      auto tmp = extractBeamtimes(jdoc.object().value(Keys::beamtimes).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (jdoc.object().contains(Keys::runs) && jdoc.object().value(Keys::runs).isArray()) {
      auto tmp = extractRuns(jdoc.object().value(Keys::runs).toArray());
      std::for_each(tmp.begin(), tmp.end(),
                    [&result](auto element) { ::addParentToRun(result, element); });
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::MATERIALS:
    if (jdoc.object().contains(Keys::materials) && jdoc.object().value(Keys::materials).isArray()) {
      auto tmp = extractMaterials(jdoc.object().value(Keys::materials).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::DETECTOR_REACTION:
    if (jdoc.object().contains(Keys::reactions) && jdoc.object().value(Keys::reactions).isArray()) {
      auto tmp = extractReaction(jdoc.object().value(Keys::reactions).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (jdoc.object().contains(Keys::detectors) && jdoc.object().value(Keys::detectors).isArray()) {
      auto tmp = extractDetectors(jdoc.object().value(Keys::detectors).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::SETUP:
    if (jdoc.object().contains(Keys::reactions) && jdoc.object().value(Keys::reactions).isArray()) {
      auto tmp = extractReaction(jdoc.object().value(Keys::reactions).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (jdoc.object().contains(Keys::materials) && jdoc.object().value(Keys::materials).isArray()) {
      auto tmp = extractMaterials(jdoc.object().value(Keys::materials).toArray());
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (jdoc.object().contains(Keys::detectors) && jdoc.object().value(Keys::detectors).isArray()) {
      auto tmp = extractDetectors(jdoc.object().value(Keys::detectors).toArray());
      std::for_each(tmp.begin(), tmp.end(),
                    [&result](auto element) { ::addMaterialToDetector(result, element); });
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    break;
  case FileType::ALL:
    result = extract(jdoc.object());
    break;
  case FileType::UNDEFINED:
  case FileType::BAD_VERSION:
    break;
  }

  return result;
}

bool JsonParameterReader::isFiletypeAvailable(BaseParameterReader::FileType check) const
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
  case FileType::BAD_VERSION:
    return false;
  }
  return false;
}

BaseParameterReader::FileType JsonParameterReader::checkVersionString(string const& version) const
{
  Q_UNUSED(version);
  return BaseParameterReader::FileType::UNDEFINED;
}

std::shared_ptr<base_parameter> JsonParameterReader::toAlgorithm(QJsonObject const& input) const
{
  static std::map<ParameterValue::ValueType,
                  std::function<void(QJsonObject const&, std::shared_ptr<algorithm_parameter>,
                                     std::string const&)>>
      parameterReading = []() {
        std::map<ParameterValue::ValueType,
                 std::function<void(QJsonObject const&, std::shared_ptr<algorithm_parameter>,
                                    std::string const&)>>
            result;
        result.insert(std::make_pair(
            ParameterValue::ValueType::BOOLEAN,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<bool> s(n, o.value(Keys::pValue).toBool(false));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::INT,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              std::cout << "found int parameter" << n << " " << o.value(Keys::pValue).toInt()
                        << std::endl;
              single_parameter<int> s(n, o.value(Keys::pValue).toInt(-1));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::FLOAT,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<float> s(n, static_cast<float>(o.value(Keys::pValue).toDouble(-1)));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::DOUBLE,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<float> s(n, o.value(Keys::pValue).toDouble(-1));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::STRING,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<std::string> s(n, o.value(Keys::pValue).toString().toStdString());
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::POINT3D,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              auto pointString = o.value(Keys::pValue).toString().toStdString();
              single_parameter<point3D> s(n, toPoint(pointString));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR3D,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<vector3D> s(
                  n, toVector(o.value(Keys::pValue).toString().toStdString()));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR_INT,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<std::vector<int>> s(
                  n, ::toIntList(o.value(Keys::pValue).toString().toStdString()));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR_FLOAT,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<std::vector<float>> s(
                  n, ::toFloatList(o.value(Keys::pValue).toString().toStdString()));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR_DOUBLE,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<std::vector<float>> s(
                  n, ::toFloatList(o.value(Keys::pValue).toString().toStdString()));
              a->addParam(s);
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR_STRING,
            [](QJsonObject const& o, std::shared_ptr<algorithm_parameter> a, std::string const& n) {
              single_parameter<std::vector<std::string>> s(
                  n, ::toStringList(o.value(Keys::pValue).toString().toStdString()));
              a->addParam(s);
            }));
        // algorithm-list still missing
        return result;
      }();
  if (!(input.contains(Keys::use) && input.contains(Keys::level) &&
        input.contains(Keys::category) && input.contains(Keys::id_number))) {
    return {};
  }
  auto result = std::make_shared<algorithm_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }
  result->setUse(input.value(Keys::use).toBool(false));
  result->setLevel(input.value(Keys::level).toInt(-1));
  result->setCategory(input.value(Keys::category).toInt(-1));
  result->setID(input.value(Keys::id_number).toInt(-1));
  // parameter []
  if (input.contains(Keys::parameter)) {
    auto parameterV = input.value(Keys::parameter);
    if (!parameterV.isArray()) {
      return {};
    }
    auto parameters = parameterV.toArray();
    for (auto pIt = parameters.begin(); pIt != parameters.end(); pIt++) {
      if (!pIt->isObject()) {
        return {};
      }
      auto parameterObject = pIt->toObject();
      if (!(parameterObject.contains(Keys::pType) && parameterObject.contains(Keys::pName) &&
            parameterObject.contains(Keys::pValue))) {
        return {};
      }
      auto name = parameterObject.value(Keys::pName).toString().toStdString();
      auto tpe = static_cast<ParameterValue::ValueType>(parameterObject.value(Keys::pType).toInt());
      auto it = parameterReading.find(tpe);
      if (it != parameterReading.end()) {
        it->second(parameterObject, result, name);
      }
    }
  }
  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toShape(QJsonObject const& input) const
{
  static std::map<
      ParameterValue::ValueType,
      std::function<void(QJsonObject const&, std::shared_ptr<shape_parameter>, std::string const&)>>
      parameterReading = []() {
        std::map<ParameterValue::ValueType,
                 std::function<void(QJsonObject const&, std::shared_ptr<shape_parameter>,
                                    std::string const&)>>
            result;
        result.insert(std::make_pair(
            ParameterValue::ValueType::INT,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, o.value(Keys::pValue).toInt(-1));
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::FLOAT,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, static_cast<float>(o.value(Keys::pValue).toDouble(-1)));
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::DOUBLE,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, static_cast<float>(o.value(Keys::pValue).toDouble(-1)));
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::STRING,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, o.value(Keys::pValue).toString().toStdString());
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::POINT3D,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, toPoint(o.value(Keys::pValue).toString().toStdString()));
            }));
        result.insert(std::make_pair(
            ParameterValue::ValueType::VECTOR3D,
            [](QJsonObject const& o, std::shared_ptr<shape_parameter> a, std::string const& n) {
              a->addValue(n, toVector(o.value(Keys::pValue).toString().toStdString()));
            }));
        return result;
      }();
  auto result = std::make_shared<shape_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }
  // parameter []
  if (input.contains(Keys::parameter)) {
    auto parameterV = input.value(Keys::parameter);
    if (!parameterV.isArray()) {
      return {};
    }
    auto parameters = parameterV.toArray();
    for (auto pIt = parameters.begin(); pIt != parameters.end(); pIt++) {
      if (!pIt->isObject()) {
        return {};
      }
      auto parameterObject = pIt->toObject();
      if (!(parameterObject.contains(Keys::pType) && parameterObject.contains(Keys::pName) &&
            parameterObject.contains(Keys::pValue))) {
        return {};
      }
      auto name = parameterObject.value(Keys::pName).toString().toStdString();
      auto tpe = static_cast<ParameterValue::ValueType>(parameterObject.value(Keys::pType).toInt());
      auto it = parameterReading.find(tpe);
      if (it != parameterReading.end()) {
        it->second(parameterObject, result, name);
      }
    }
  }
  result->setCompleteWrite(true);
  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toBeamtime(QJsonObject const& input) const
{
  if (!(input.contains(Keys::runFile) && input.contains(Keys::setupFile) &&
        input.contains(Keys::month) && input.contains(Keys::year))) {
    return {};
  }
  auto result = std::make_shared<beamTime_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }
  result->setFileName(input.value(Keys::runFile).toString().toStdString());
  result->setSetupFile(input.value(Keys::setupFile).toString().toStdString());
  result->setData(input.value(Keys::month).toInt(-1), input.value(Keys::year).toInt(-1));
  // calibration-files []
  if (input.contains(Keys::calibrationFile)) {
    auto calibV = input.value(Keys::calibrationFile);
    if (!calibV.isArray()) {
      return {};
    }
    auto calibration = calibV.toArray();
    for (auto pIt = calibration.begin(); pIt != calibration.end(); pIt++) {
      if (!pIt->isString()) {
        return {};
      }
      result->addCalibrationFile(pIt->toString().toStdString());
    }
  }
  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toRun(QJsonObject const& input) const
{
  if (!(input.contains(Keys::runType) && input.contains(Keys::parent) &&
        input.contains(Keys::runNumber) && input.contains(Keys::start) &&
        input.contains(Keys::stop))) {
    return {};
  }
  auto result = std::make_shared<run_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }
  result->setType(static_cast<run_parameter::RunType>(input.value(Keys::runType).toInt(-1)));
  result->setParentNumber(
      boost::uuids::string_generator()(input.value(Keys::parent).toString().toStdString()));
  result->setRunNumber(input.value(Keys::runNumber).toInt(-1));
  tm startTime;
  ::readDate(startTime, input.value(Keys::start).toObject());
  result->setStartTime(startTime);
  tm stopTime;
  ::readDate(startTime, input.value(Keys::stop).toObject());
  result->setStopTime(stopTime);
  if (input.contains(Keys::setupFile)) {
    result->setSetupFile(input.value(Keys::setupFile).toString().toStdString());
    result->setOwnSetup(true);
  } else {
    result->setOwnSetup(false);
  }
  // calibration files []
  if (input.contains(Keys::calibrationFile)) {
    result->setAdditionalCalibration(true);
    if (input.contains(Keys::own)) {
      result->setOwnCalibration(input.value(Keys::own).toBool(false));
    }
    auto calibV = input.value(Keys::calibrationFile);
    if (!calibV.isArray()) {
      return {};
    }
    auto calibration = calibV.toArray();
    for (auto pIt = calibration.begin(); pIt != calibration.end(); pIt++) {
      result->addCalibrationFile(pIt->toString().toStdString());
    }
  } else {
    result->setAdditionalCalibration(false);
  }
  // Files
  if (input.contains(Keys::run_files)) {
    auto filesV = input.value(Keys::run_files);
    if (!filesV.isArray()) {
      return {};
    }
    auto files = filesV.toArray();
    for (auto fIt = files.begin(); fIt != files.end(); fIt++) {
      if (!fIt->isObject()) {
        return {};
      }
      auto fileO = fIt->toObject();
      if (fileO.contains(Keys::run_fileType) && fileO.contains(Keys::run_name)) {
        int events = -1;
        if (fileO.contains(Keys::run_events)) {
          events = fileO.value(Keys::run_events).toInt(-1);
        }
        result->addFile(fileO.value(Keys::run_name).toString().toStdString(),
                        static_cast<size_t>(fileO.value(Keys::run_fileType).toInt(-1)), events);
      }
    }
  }
  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toDetector(QJsonObject const& input) const
{
  if (!(input.contains(Keys::number_of_elements) && input.contains(Keys::stack_type) &&
        input.contains(Keys::detector_number) && input.contains(Keys::max_distance) &&
        input.contains(Keys::detector_shape) && input.contains(Keys::material_id))) {
    return {};
  }
  auto result = std::make_shared<detector_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }

  result->setNumberOfElements(input.value(Keys::number_of_elements).toInt());
  result->setStackType(input.value(Keys::stack_type).toInt());
  result->setID(input.value(Keys::detector_number).toInt());
  result->setMaxDistance(static_cast<float>(input.value(Keys::max_distance).toDouble()));
  if (input.contains(Keys::is_circular)) {
    result->setCircular(input.value(Keys::is_circular).toBool());
  }
  auto shape = std::dynamic_pointer_cast<shape_parameter>(
      toShape(input.value(Keys::detector_shape).toObject()));
  result->setShape(*shape);
  result->setMaterialId(
      boost::uuids::string_generator()(input.value(Keys::material_id).toString().toStdString()));

  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toMaterial(QJsonObject const& input) const
{
  if (!(input.contains(Keys::is_active) && input.contains(Keys::energyloss) &&
        input.contains(Keys::density) && input.contains(Keys::radiation_length) &&
        input.contains(Keys::speed) && input.contains(Keys::elements))) {
    return {};
  }
  auto result = std::make_shared<material_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }

  result->setActive(input.value(Keys::is_active).toBool());
  result->setSpeed(static_cast<float>(input.value(Keys::speed).toDouble()));
  result->setDensity(static_cast<float>(input.value(Keys::density).toDouble()));
  result->setRadiationLength(static_cast<float>(input.value(Keys::radiation_length).toDouble()));
  auto energyLossA = input.value(Keys::energyloss).toArray();
  for (int i = 0; i < energyLossA.count(); i++) {
    auto e = energyLossA[i].toDouble();
    result->setEnergyLoss(i, static_cast<float>(e));
  }
  auto elementA = input.value(Keys::elements).toArray();
  for (int i = 0; i < elementA.count(); i++) {
    auto element = elementA[i].toObject();
    if (!(element.contains(Keys::element_mass) && element.contains(Keys::element_charge) &&
          element.contains(Keys::element_weight))) {
      continue;
    }
    result->addElement(
        element_parameter{static_cast<float>(element.value(Keys::element_mass).toDouble()),
                          static_cast<float>(element.value(Keys::element_charge).toDouble()),
                          static_cast<float>(element.value(Keys::element_weight).toDouble())});
  }

  return std::move(result);
}

std::shared_ptr<base_parameter> JsonParameterReader::toReaction(QJsonObject const& input) const
{
  if (!(input.contains(Keys::two_beams) && input.contains(Keys::target_material_id) &&
        input.contains(Keys::beam_material_id) && input.contains(Keys::beam_momentum_1))) {
    return {};
  }
  auto result = std::make_shared<reaction_parameter>();
  if (!::addBasic(std::dynamic_pointer_cast<base_parameter>(result), input)) {
    return {};
  }

  result->setTwoBeams(input.value(Keys::two_beams).toBool());
  result->setBeamMomentum(static_cast<float>(input.value(Keys::beam_momentum_1).toDouble()), 0);
  if (input.contains(Keys::beam_momentum_2)) {
    result->setBeamMomentum(static_cast<float>(input.value(Keys::beam_momentum_2).toDouble()), 1);
  }
  result->setMaterialId(0, boost::uuids::string_generator()(
                               input.value(Keys::beam_material_id).toString().toStdString()));
  result->setMaterialId(1, boost::uuids::string_generator()(
                               input.value(Keys::target_material_id).toString().toStdString()));
  if (!result->hasTwoBeams() && input.contains(Keys::target_shape)) {
    auto shape = std::dynamic_pointer_cast<shape_parameter>(
        toShape(input.value(Keys::target_shape).toObject()));
    result->setTargetShape(*shape);
  }

  return std::move(result);
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractAlgorithms(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::algorithm) {
      continue;
    }

    auto element = toAlgorithm(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractShapes(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::shape) {
      continue;
    }

    auto element = toShape(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractBeamtimes(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::beamtime) {
      continue;
    }

    auto element = toBeamtime(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractRuns(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::run) {
      continue;
    }

    auto element = toRun(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractDetectors(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::detector) {
      continue;
    }

    auto element = toDetector(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractMaterials(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::material) {
      continue;
    }

    auto element = toMaterial(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extractReaction(
    QJsonArray const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;
  for (int i = 0; i < input.count(); i++) {
    auto item = input[i];
    if (!item.isObject() || !item.toObject().contains(Keys::parameterType) ||
        item.toObject().value(Keys::parameterType).toString() != Keys::reaction) {
      continue;
    }

    auto element = toReaction(item.toObject());
    if (element) {
      result.push_back(element);
    }
  }
  return result;
}

std::vector<std::shared_ptr<base_parameter>> JsonParameterReader::extract(
    QJsonObject const& input) const
{
  std::vector<std::shared_ptr<base_parameter>> result;

  if (input.contains(Keys::beamtimes) && input.value(Keys::beamtimes).isArray()) {
    auto tmp = extractBeamtimes(input.value(Keys::beamtimes).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  if (input.contains(Keys::runs) && input.value(Keys::runs).isArray()) {
    auto tmp = extractRuns(input.value(Keys::runs).toArray());
    std::for_each(tmp.begin(), tmp.end(),
                  [&result](auto element) { ::addParentToRun(result, element); });
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  if (input.contains(Keys::shapes) && input.value(Keys::shapes).isArray()) {
    auto tmp = extractShapes(input.value(Keys::shapes).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  if (input.contains(Keys::materials) && input.value(Keys::materials).isArray()) {
    auto tmp = extractMaterials(input.value(Keys::materials).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  if (input.contains(Keys::detectors) && input.value(Keys::detectors).isArray()) {
    auto tmp = extractDetectors(input.value(Keys::detectors).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
    std::for_each(tmp.begin(), tmp.end(),
                  [&result](auto element) { ::addMaterialToDetector(result, element); });
  }
  if (input.contains(Keys::reactions) && input.value(Keys::reactions).isArray()) {
    auto tmp = extractReaction(input.value(Keys::reactions).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  if (input.contains(Keys::algorithms) && input.value(Keys::algorithms).isArray()) {
    auto tmp = extractAlgorithms(input.value(Keys::algorithms).toArray());
    result.insert(result.end(), tmp.begin(), tmp.end());
  }
  return result;
}

} // namespace IO
} // namespace parameter
