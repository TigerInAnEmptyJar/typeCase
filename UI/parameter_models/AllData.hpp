#pragma once

#include <boost/uuid/uuid.hpp>

#include <boost/signals2.hpp>

#include <filesystem>
#include <memory>
#include <vector>

class shape_parameter;
class material_parameter;
class detector_parameter;
class reaction_parameter;
class beamTime_parameter;
class run_parameter;
class algorithm_parameter;

namespace UI {

class AllData
{
public:
  AllData();
  ~AllData();

  std::vector<std::shared_ptr<shape_parameter>> definedShapes();
  std::vector<std::shared_ptr<material_parameter>> materials();
  std::vector<std::shared_ptr<beamTime_parameter>> database();
  std::vector<std::shared_ptr<detector_parameter>> detectors();
  std::shared_ptr<reaction_parameter> reaction();
  std::vector<std::shared_ptr<algorithm_parameter>> definedAlgorithms();
  std::vector<std::shared_ptr<algorithm_parameter>>& selectedAlgorithms();

  bool loadMaterials(std::filesystem::path path, boost::uuids::uuid typeId);
  bool loadDatabase(std::filesystem::path path, boost::uuids::uuid typeId);
  bool loadSetup(std::filesystem::path path, boost::uuids::uuid typeId);
  bool loadSelectedAlgorithms(std::filesystem::path path, boost::uuids::uuid typeId);

  void storeMaterials(std::filesystem::path path, boost::uuids::uuid typeId);
  void storeDatabase(std::filesystem::path path, boost::uuids::uuid typeId);
  void storeSetup(std::filesystem::path path, boost::uuids::uuid typeId);
  void storeSelectedAlgorithms(std::filesystem::path path, boost::uuids::uuid typeId);

  void addMaterial(std::shared_ptr<material_parameter> material);
  void addDetector(std::shared_ptr<detector_parameter> detector);
  void addBeamtime(std::shared_ptr<beamTime_parameter> beamtime);
  void addRun(std::shared_ptr<run_parameter> run);
  void addAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);

  void removeMaterial(std::shared_ptr<material_parameter> material);
  void removeDetector(std::shared_ptr<detector_parameter> detector);
  void removeBeamtime(std::shared_ptr<beamTime_parameter> beamtime);
  void removeRun(std::shared_ptr<run_parameter> run);
  void removeAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);

  void upAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);
  void downAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);
  void topAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);
  void bottomAlgorithm(std::shared_ptr<algorithm_parameter> algorithm);

  boost::signals2::connection connectShapesAboutToChange(std::function<void()> subscriber);
  boost::signals2::connection connectShapesChanged(std::function<void()> subscriber);
  boost::signals2::connection connectMaterialAboutToChange(std::function<void()> subscriber);
  boost::signals2::connection connectMaterialChanged(std::function<void()> subscriber);
  boost::signals2::connection connectDetectorAboutToChange(std::function<void()> subscriber);
  boost::signals2::connection connectDetectorChanged(std::function<void()> subscriber);
  boost::signals2::connection connectAlgorithmAboutToChange(std::function<void()> subscriber);
  boost::signals2::connection connectAlgorithmChanged(std::function<void()> subscriber);
  boost::signals2::connection connectSelectedAlgorithmAboutToChange(
      std::function<void()> subscriber);
  boost::signals2::connection connectSelectedAlgorithmChanged(std::function<void()> subscriber);
  boost::signals2::connection connectDatabaseChanged(std::function<void()> subscriber);

private:
  std::vector<std::shared_ptr<shape_parameter>> _shapes;
  std::vector<std::shared_ptr<material_parameter>> _materials;
  std::vector<std::shared_ptr<beamTime_parameter>> _database;
  std::vector<std::shared_ptr<detector_parameter>> _detectors;
  std::shared_ptr<reaction_parameter> _reaction;
  std::vector<std::shared_ptr<algorithm_parameter>> _definedAlgorithms;
  std::vector<std::shared_ptr<algorithm_parameter>> _selectedAlgorithms;
  boost::signals2::signal<void()> _shapePre;
  boost::signals2::signal<void()> _shapePost;
  boost::signals2::signal<void()> _materialPre;
  boost::signals2::signal<void()> _materialPost;
  boost::signals2::signal<void()> _detectorPre;
  boost::signals2::signal<void()> _detectorPost;
  boost::signals2::signal<void()> _algorithmPre;
  boost::signals2::signal<void()> _algorithmPost;
  boost::signals2::signal<void()> _selectedAlgorithmPre;
  boost::signals2::signal<void()> _selectedAlgorithmPost;
  boost::signals2::signal<void()> _databaseChange;
};

} // namespace UI
