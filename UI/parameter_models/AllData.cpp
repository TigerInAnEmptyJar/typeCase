#include "AllData.hpp"

#include "shapeFactory.h"

#include "parameterio.h"
#include "versions.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"

#include <QSettings>
#include <QString>

namespace {
QString lastMaterialFile = "lastMaterialFile";
QString lastSetupFile = "lastSetupFile";
QString lastAlgorithmFile = "lastAlgorithmFile";
QString lastDatabaseFile = "lastDatabaseFile";
QString lastTmpAlgorithmFile = "lastAlgorithmFile";
} // namespace

namespace UI {

AllData::AllData()
{
  auto& factory = ShapeFactory::getInstance();
  auto subscriber = [this](auto) {
    auto& factory = ShapeFactory::getInstance();
    auto shapes = factory.definedShapes();

    _shapePre();
    _shapes.clear();
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(_shapes), [](auto item) {
      auto element = std::make_shared<shape_parameter>(item);
      return element;
    });
    _shapePost();
  };
  factory.subscribeShapeAdded(subscriber);
  factory.subscribeShapeRemoved(subscriber);
  subscriber(boost::uuids::uuid{});

  QSettings settings;
  if (!settings.value(::lastMaterialFile).toString().isEmpty()) {
    loadMaterials(settings.value(::lastMaterialFile).toString().toStdString(),
                  parameter::IO::version_json);
  }
  if (!settings.value(::lastSetupFile).toString().isEmpty()) {
    loadSetup(settings.value(::lastSetupFile).toString().toStdString(),
              parameter::IO::version_json);
  }
  if (!settings.value(::lastAlgorithmFile).toString().isEmpty()) {
    loadSelectedAlgorithms(settings.value(::lastAlgorithmFile).toString().toStdString(),
                           parameter::IO::version_json);
  }
  if (!settings.value(::lastDatabaseFile).toString().isEmpty()) {
    loadDatabase(settings.value(::lastDatabaseFile).toString().toStdString(),
                 parameter::IO::version_json);
  }
  if (!settings.value(::lastTmpAlgorithmFile).toString().isEmpty()) {
    auto path =
        std::filesystem::path{settings.value(::lastTmpAlgorithmFile).toString().toStdString()};

    parameter::IO::ParameterIO io;
    io.setup();
    auto input_list = io.readParameter(path, parameter::IO::version_json,
                                       parameter::IO::ParameterIO::FileType::ALGORITHM);

    if (!input_list.empty()) {
      _definedAlgorithms.clear();
      std::transform(
          input_list.begin(), input_list.end(), std::back_inserter(_definedAlgorithms),
          [](auto item) { return std::dynamic_pointer_cast<algorithm_parameter>(item); });
      _definedAlgorithms.erase(
          std::remove(_definedAlgorithms.begin(), _definedAlgorithms.end(), nullptr),
          _definedAlgorithms.end());
    }
  }
}

AllData::~AllData() = default;

std::vector<std::shared_ptr<shape_parameter>> AllData::definedShapes() { return _shapes; }

std::vector<std::shared_ptr<material_parameter>> AllData::materials() { return _materials; }

std::vector<std::shared_ptr<beamTime_parameter>> AllData::database() { return _database; }

std::vector<std::shared_ptr<detector_parameter>> AllData::detectors() { return _detectors; }

std::shared_ptr<reaction_parameter> AllData::reaction() { return _reaction; }

std::vector<std::shared_ptr<algorithm_parameter>> AllData::definedAlgorithms()
{
  return _definedAlgorithms;
}

std::vector<std::shared_ptr<algorithm_parameter>>& AllData::selectedAlgorithms()
{
  return _selectedAlgorithms;
}

bool AllData::loadMaterials(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  auto input_list = io.readParameter(path, typeId, parameter::IO::ParameterIO::FileType::MATERIAL);

  if (input_list.empty()) {
    return false;
  }

  QSettings settings;
  settings.setValue(::lastMaterialFile, QString::fromStdString(path.native()));
  settings.sync();

  _materialPre();
  _materials.clear();
  std::transform(input_list.begin(), input_list.end(), std::back_inserter(_materials),
                 [](auto item) { return std::dynamic_pointer_cast<material_parameter>(item); });
  _materials.erase(std::remove(_materials.begin(), _materials.end(), nullptr), _materials.end());
  _materialPost();

  _detectorPre();
  std::for_each(
      _detectors.begin(), _detectors.end(), [this](std::shared_ptr<detector_parameter>& detector) {
        auto materialID = detector->getMaterialId();
        if (!materialID.is_nil()) {
          auto material = std::find_if(_materials.begin(), _materials.end(),
                                       [materialID](std::shared_ptr<material_parameter>& item) {
                                         return item->id() == materialID;
                                       });
          if (material != _materials.end()) {
            detector->setMaterial(material->get());
            detector->setMaterial(static_cast<int>(std::distance(_materials.begin(), material)));
          } else if (detector->getMaterial() >= 0 &&
                     static_cast<size_t>(detector->getMaterial()) < _materials.size()) {
            detector->setMaterial(_materials[static_cast<size_t>(detector->getMaterial())].get());
          } else {
            detector->setMaterial(nullptr);
          }
        } else if (detector->getMaterial() >= 0 &&
                   static_cast<size_t>(detector->getMaterial()) < _materials.size()) {
          detector->setMaterial(_materials[static_cast<size_t>(detector->getMaterial())].get());
        } else {
          detector->setMaterial(nullptr);
        }
      });
  _detectorPost();
  return true;
}

bool AllData::loadDatabase(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  auto run_list = io.readParameter(path, typeId, parameter::IO::ParameterIO::FileType::DATABASE);

  QSettings settings;
  settings.setValue(::lastDatabaseFile, QString::fromStdString(path.native()));
  settings.sync();

  _database.clear();
  std::transform(
      run_list.begin(), run_list.end(), std::back_inserter(_database),
      [run_list](auto item) { return std::dynamic_pointer_cast<beamTime_parameter>(item); });
  _databaseChange();
  return true;
}

bool AllData::loadSetup(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  auto input_list = io.readParameter(path, typeId, parameter::IO::ParameterIO::FileType::DETECTOR);

  if (input_list.empty()) {
    return false;
  }

  QSettings settings;
  settings.setValue(::lastSetupFile, QString::fromStdString(path.native()));
  settings.sync();

  _detectorPre();
  _detectors.clear();

  std::transform(input_list.begin(), input_list.end(), std::back_inserter(_detectors),
                 [](auto item) { return std::dynamic_pointer_cast<detector_parameter>(item); });
  _detectors.erase(std::remove(_detectors.begin(), _detectors.end(), nullptr), _detectors.end());

  auto reaction = std::find_if(input_list.begin(), input_list.end(), [](auto item) {
    return std::dynamic_pointer_cast<reaction_parameter>(item) != nullptr;
  });
  if (reaction != input_list.end()) {
    _reaction = std::dynamic_pointer_cast<reaction_parameter>(*reaction);
  }

  _detectorPost();
  return true;
}

bool AllData::loadSelectedAlgorithms(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  auto input_list = io.readParameter(path, typeId, parameter::IO::ParameterIO::FileType::ALGORITHM);

  if (input_list.empty()) {
    return false;
  }

  QSettings settings;
  settings.setValue(::lastAlgorithmFile, QString::fromStdString(path.native()));
  settings.sync();

  _selectedAlgorithmPre();
  _selectedAlgorithms.clear();
  std::transform(input_list.begin(), input_list.end(), std::back_inserter(_selectedAlgorithms),
                 [](auto item) { return std::dynamic_pointer_cast<algorithm_parameter>(item); });
  _selectedAlgorithms.erase(
      std::remove(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), nullptr),
      _selectedAlgorithms.end());
  _selectedAlgorithmPost();
  return true;
}

void AllData::storeMaterials(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  std::vector<std::shared_ptr<base_parameter>> output_list;
  std::transform(_materials.begin(), _materials.end(), std::back_inserter(output_list),
                 [](auto element) { return std::dynamic_pointer_cast<base_parameter>(element); });
  io.writeParameter(path, output_list, typeId, parameter::IO::ParameterIO::FileType::MATERIAL);

  QSettings settings;
  settings.setValue(::lastMaterialFile, QString::fromStdString(path.native()));
  settings.sync();
}

void AllData::storeDatabase(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  std::vector<std::shared_ptr<base_parameter>> output_list;
  std::transform(_database.begin(), _database.end(), std::back_inserter(output_list),
                 [](auto element) { return std::dynamic_pointer_cast<base_parameter>(element); });
  io.writeParameter(path, output_list, typeId, parameter::IO::ParameterIO::FileType::DATABASE);

  QSettings settings;
  settings.setValue(::lastDatabaseFile, QString::fromStdString(path.native()));
  settings.sync();
}

void AllData::storeSetup(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  std::vector<std::shared_ptr<base_parameter>> output_list;
  output_list.push_back(std::dynamic_pointer_cast<base_parameter>(_reaction));
  std::transform(_detectors.begin(), _detectors.end(), std::back_inserter(output_list),
                 [](auto element) { return std::dynamic_pointer_cast<base_parameter>(element); });
  io.writeParameter(path, output_list, typeId, parameter::IO::ParameterIO::FileType::SETUP);

  QSettings settings;
  settings.setValue(::lastSetupFile, QString::fromStdString(path.native()));
  settings.sync();
}

void AllData::storeSelectedAlgorithms(std::filesystem::path path, boost::uuids::uuid typeId)
{
  parameter::IO::ParameterIO io;
  io.setup();
  std::vector<std::shared_ptr<base_parameter>> output_list;
  std::transform(_selectedAlgorithms.begin(), _selectedAlgorithms.end(),
                 std::back_inserter(output_list),
                 [](auto element) { return std::dynamic_pointer_cast<base_parameter>(element); });
  io.writeParameter(path, output_list, typeId, parameter::IO::ParameterIO::FileType::ALGORITHM);

  QSettings settings;
  settings.setValue(::lastAlgorithmFile, QString::fromStdString(path.native()));
  settings.sync();
}

void AllData::addMaterial(std::shared_ptr<material_parameter> material)
{
  if (!material) {
    return;
  }
  _materialPre();
  _materials.push_back(material);
  _materialPost();
}

void AllData::addDetector(std::shared_ptr<detector_parameter> detector)
{
  if (!detector) {
    return;
  }
  _detectorPre();
  _detectors.push_back(detector);
  _detectorPost();
}

void AllData::addBeamtime(std::shared_ptr<beamTime_parameter> beamtime)
{
  _database.push_back(beamtime);
}

void AllData::addRun(std::shared_ptr<run_parameter> run)
{
  _database.push_back(std::dynamic_pointer_cast<beamTime_parameter>(run));
}

void AllData::addAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  _selectedAlgorithmPre();
  _selectedAlgorithms.push_back(algorithm);
  _selectedAlgorithmPost();
}

void AllData::removeMaterial(std::shared_ptr<material_parameter> material)
{
  auto element = std::find(_materials.begin(), _materials.end(), material);
  if (element != _materials.end()) {
    _materialPre();
    _materials.erase(element);
    _materialPost();
  }
}

void AllData::removeDetector(std::shared_ptr<detector_parameter> detector)
{
  auto element = std::find(_detectors.begin(), _detectors.end(), detector);
  if (element != _detectors.end()) {
    _detectorPre();
    _detectors.erase(element);
    _detectorPost();
  }
}

void AllData::removeBeamtime(std::shared_ptr<beamTime_parameter> beamtime)
{
  auto element = std::find(_database.begin(), _database.end(), beamtime);
  if (element != _database.end()) {
    _database.erase(element);
  }
}

void AllData::removeRun(std::shared_ptr<run_parameter> run)
{
  auto element = std::find(_database.begin(), _database.end(), run);
  if (element != _database.end()) {
    _database.erase(element);
  }
}

void AllData::removeAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  auto element = std::find(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), algorithm);
  if (element != _selectedAlgorithms.end()) {
    _selectedAlgorithmPre();
    _selectedAlgorithms.erase(element);
    _selectedAlgorithmPost();
  }
}

void AllData::upAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  auto element = std::find(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), algorithm);
  if (element != _selectedAlgorithms.end() && element != _selectedAlgorithms.begin()) {
    _selectedAlgorithmPre();
    std::rotate(element - 1, element, element);
    _selectedAlgorithmPost();
  }
}

void AllData::downAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  auto element = std::find(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), algorithm);
  if (element != _selectedAlgorithms.end() && element + 1 != _selectedAlgorithms.end()) {
    _selectedAlgorithmPre();
    std::rotate(element, element + 1, element + 1);
    _selectedAlgorithmPost();
  }
}

void AllData::topAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  auto element = std::find(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), algorithm);
  if (element != _selectedAlgorithms.end() && element != _selectedAlgorithms.begin()) {
    _selectedAlgorithmPre();
    _selectedAlgorithms.erase(element);
    _selectedAlgorithms.insert(_selectedAlgorithms.begin(), algorithm);
    _selectedAlgorithmPost();
  }
}

void AllData::bottomAlgorithm(std::shared_ptr<algorithm_parameter> algorithm)
{
  auto element = std::find(_selectedAlgorithms.begin(), _selectedAlgorithms.end(), algorithm);
  if (element != _selectedAlgorithms.end() && element + 1 != _selectedAlgorithms.end()) {
    _selectedAlgorithmPre();
    _selectedAlgorithms.erase(element);
    _selectedAlgorithms.push_back(algorithm);
    _selectedAlgorithmPost();
  }
}

boost::signals2::connection AllData::connectShapesAboutToChange(std::function<void()> subscriber)
{
  return _shapePre.connect(subscriber);
}

boost::signals2::connection AllData::connectShapesChanged(std::function<void()> subscriber)
{
  return _shapePost.connect(subscriber);
}

boost::signals2::connection AllData::connectMaterialAboutToChange(std::function<void()> subscriber)
{
  return _materialPre.connect(subscriber);
}

boost::signals2::connection AllData::connectMaterialChanged(std::function<void()> subscriber)
{
  return _materialPost.connect(subscriber);
}

boost::signals2::connection AllData::connectDetectorAboutToChange(std::function<void()> subscriber)
{
  return _detectorPre.connect(subscriber);
}

boost::signals2::connection AllData::connectDetectorChanged(std::function<void()> subscriber)
{
  return _detectorPost.connect(subscriber);
}

boost::signals2::connection AllData::connectAlgorithmAboutToChange(std::function<void()> subscriber)
{
  return _algorithmPre.connect(subscriber);
}

boost::signals2::connection AllData::connectAlgorithmChanged(std::function<void()> subscriber)
{
  return _algorithmPost.connect(subscriber);
}

boost::signals2::connection AllData::connectSelectedAlgorithmAboutToChange(
    std::function<void()> subscriber)
{
  return _selectedAlgorithmPre.connect(subscriber);
}

boost::signals2::connection AllData::connectSelectedAlgorithmChanged(
    std::function<void()> subscriber)
{
  return _selectedAlgorithmPost.connect(subscriber);
}

boost::signals2::connection AllData::connectDatabaseChanged(std::function<void()> subscriber)
{
  return _databaseChange.connect(subscriber);
}

} // namespace UI
