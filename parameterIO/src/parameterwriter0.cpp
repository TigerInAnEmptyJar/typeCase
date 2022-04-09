#include "parameterwriter0.h"

#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "detectorparameter.h"
#include "materialparameter.h"
#include "shapeparameter.h"

#include <QtGlobal>

#include <fstream>

namespace parameter {
namespace IO {

void ParameterWriter0::writeParameter(std::vector<std::shared_ptr<base_parameter>> const& items,
                                      filesystem::path path,
                                      BaseParameterWriter::FileType filetype) const
{
  if (!std::filesystem::exists(path.parent_path())) {
    std::filesystem::create_directories(path.parent_path());
  }
  std::ofstream output;
  output.open(path.native());
  switch (filetype) {
  case FileType::ALGORITHM:
    writeAlgorithms(output, items);
    break;
  case FileType::SHAPE:
    writeShapes(output, items);
    break;
  case FileType::RUN:
    writeRuns(output, items);
    break;
  case FileType::BEAMTIME:
    writeBeamtimes(output, items);
    break;
  case FileType::DATABASE:
    writeBeamtimes(output, items);
    std::for_each(items.begin(), items.end(), [this, &items](auto element) {
      auto bt = std::dynamic_pointer_cast<beamTime_parameter>(element);
      if (std::dynamic_pointer_cast<run_parameter>(bt) || !bt) {
        return;
      }
      auto filepath = bt->getFileName();
      std::vector<std::shared_ptr<base_parameter>> tmp;
      std::transform(items.begin(), items.end(), std::back_inserter(tmp),
                     [bt](auto runElement) -> std::shared_ptr<base_parameter> {
                       auto run = std::dynamic_pointer_cast<run_parameter>(runElement);
                       if (run && run->getParentNumber() == bt->id()) {
                         return runElement;
                       }
                       return nullptr;
                     });

      if (!std::filesystem::exists(filepath.parent_path())) {
        std::filesystem::create_directories(filepath.parent_path());
      }
      std::ofstream output;
      output.open(filepath.native());
      writeRuns(output, tmp);
      output.close();
    });
    break;
  case FileType::MATERIALS:
    writeMaterials(output, items);
    break;
  case FileType::DETECTOR_REACTION:
    writeDetectors(output, items);
    break;
  case FileType::SETUP: {
    writeDetectors(output, items);
    writeMaterials(output, items);
    break;
  }
  case FileType::ALL:
  case FileType::UNDEFINED:
    break;
  }
  output.flush();
  output.close();
}

void ParameterWriter0::writeAlgorithms(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<algorithm_parameter>(element) != nullptr;
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (!std::dynamic_pointer_cast<algorithm_parameter>(element)) {
      return;
    }
    writeSingleAlgorithm(output, element);
  });
}

void ParameterWriter0::writeBeamtimes(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return (std::dynamic_pointer_cast<beamTime_parameter>(element) != nullptr &&
            std::dynamic_pointer_cast<run_parameter>(element) == nullptr);
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (std::dynamic_pointer_cast<run_parameter>(element) != nullptr ||
        std::dynamic_pointer_cast<beamTime_parameter>(element) == nullptr) {
      return;
    }
    writeSingleBeamtime(output, element);
  });
}

void ParameterWriter0::writeRuns(std::ostream& output,
                                 std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<run_parameter>(element) != nullptr;
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (!std::dynamic_pointer_cast<run_parameter>(element)) {
      return;
    }
    writeSingleRun(output, element);
  });
}

void ParameterWriter0::writeDetectors(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  auto reactionIt = std::find_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<reaction_parameter>(element) != nullptr;
  });
  if (reactionIt == items.end()) {
    // throw exeption?
    return;
  }
  writeSingleReaction(output, *reactionIt);
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<detector_parameter>(element) != nullptr;
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (!std::dynamic_pointer_cast<detector_parameter>(element)) {
      return;
    }
    writeSingleDetector(output, element);
  });
}

void ParameterWriter0::writeMaterials(
    std::ostream& output, std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<material_parameter>(element) != nullptr;
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (!std::dynamic_pointer_cast<material_parameter>(element)) {
      return;
    }
    writeSingleMaterial(output, element);
  });
}

void ParameterWriter0::writeShapes(std::ostream& output,
                                   std::vector<std::shared_ptr<base_parameter>> const& items) const
{
  output << std::count_if(items.begin(), items.end(), [](auto element) -> bool {
    return std::dynamic_pointer_cast<shape_parameter>(element) != nullptr;
  }) << " ";
  std::for_each(items.begin(), items.end(), [this, &output](auto element) {
    if (!std::dynamic_pointer_cast<shape_parameter>(element)) {
      return;
    }
    writeSingleShape(output, element);
  });
}

void ParameterWriter0::writeSingleAlgorithm(std::ostream& output,
                                            std::shared_ptr<base_parameter> const& item) const
{
  // from write_algorithm_parameter_0
  auto alg = std::dynamic_pointer_cast<algorithm_parameter>(item);
  if (!alg) {
    return;
  }
  output << static_cast<int>(alg->getCategory()) << " ";
  output << alg->getLevel() << " ";
  output << (alg->IsUsed() ? 1 : 0) << " ";
  output << alg->getID() << " ";

  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (size_t i = 0; i < alg->numberOfValues(); i++) {
    paramMap[alg->value(i).valueType()].push_back(std::make_pair(alg->valueName(i), alg->value(i)));
  }
  output << paramMap[ParameterValue::ValueType::BOOLEAN].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::BOOLEAN].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::BOOLEAN][k].second.value<bool>();
    output << (p ? "1" : "0") << " " << paramMap[ParameterValue::ValueType::BOOLEAN][k].first
           << "\n";
  }
  output << paramMap[ParameterValue::ValueType::POINT3D].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::POINT3D][k].second.value<point3D>();
    output << p.x() << " " << p.y() << " " << p.z() << " "
           << paramMap[ParameterValue::ValueType::POINT3D][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::VECTOR3D].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::VECTOR3D][k].second.value<vector3D>();
    output << p.x() << " " << p.y() << " " << p.z() << " "
           << paramMap[ParameterValue::ValueType::VECTOR3D][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::INT].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::INT][k].second.value<int>();
    output << p << " " << paramMap[ParameterValue::ValueType::INT][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::FLOAT].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::FLOAT][k].second.value<float>();
    output << p << " " << paramMap[ParameterValue::ValueType::FLOAT][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::STRING].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::STRING][k].second.value<std::string>();
    output << p << "\n" << paramMap[ParameterValue::ValueType::STRING][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::VECTOR_INT].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_INT].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::VECTOR_INT][k].second.value<std::vector<int>>();
    output << p.size() << " ";
    for (auto e : p) {
      output << e << " ";
    }
    output << paramMap[ParameterValue::ValueType::VECTOR_INT][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::VECTOR_FLOAT].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_FLOAT].size(); k++) {
    auto p =
        paramMap[ParameterValue::ValueType::VECTOR_FLOAT][k].second.value<std::vector<float>>();
    output << p.size() << " ";
    for (auto e : p) {
      output << e << " ";
    }
    output << paramMap[ParameterValue::ValueType::VECTOR_FLOAT][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::VECTOR_STRING].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR_STRING].size(); k++) {
    auto p =
        paramMap[ParameterValue::ValueType::VECTOR_STRING][k].second.value<std::vector<float>>();
    output << p.size() << " ";
    for (auto e : p) {
      output << e << "\n";
    }
    output << paramMap[ParameterValue::ValueType::VECTOR_STRING][k].first << "\n";
  }
  output << paramMap[ParameterValue::ValueType::ALGORITHM].size() << " ";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::ALGORITHM].size(); k++) {
    writeSingleAlgorithm(output, paramMap[ParameterValue::ValueType::ALGORITHM][k]
                                     .second.value<std::shared_ptr<algorithm_parameter>>());
    output << " " << paramMap[ParameterValue::ValueType::ALGORITHM][k].first << "\n";
  }

  if (paramMap[ParameterValue::ValueType::ALGORITHM].size() != 0)
    output << " ";
  output << alg->getName().data() << "\n";
  output << 1 << " ";
  output << alg->getDescription() << "\n";
}

void ParameterWriter0::writeSingleBeamtime(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto beamtime = std::dynamic_pointer_cast<beamTime_parameter>(item);
  if (!beamtime) {
    return;
  }
  output << beamtime->getMonth() << " " << beamtime->getYear();
  output << beamtime->getName().data() << "\n";
  output << 1 << " " << beamtime->getDescription() << "\n";
  output << beamtime->getFileName().native() << "\n";
  output << beamtime->getSetupFile().native() << "\n";
  output << beamtime->getNumberOfCalibrationFiles() << " ";
  for (size_t i = 0; i < beamtime->getNumberOfCalibrationFiles(); i++) {
    output << beamtime->getCalibrationFile(i).native() << "\n";
  }
}

void ParameterWriter0::writeSingleRun(std::ostream& output,
                                      std::shared_ptr<base_parameter> const& item) const
{
  // from write_run_parameter_0
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
  std::string dis = run->getDescription();
  output << 1;
  output << dis << "\n";
  output << " " << run->getNumberOfFiles() << " ";
  for (size_t i = 0; i < run->getNumberOfFiles(); i++)
    output << run->getFileType(i) << " " << run->getFile(i).native() << "\n";
  if (run->hasOwnSetup())
    output << run->getSetupFile().native() << "\n";
  if (run->hasOwnCalibration()) {
    output << run->getNumberOfCalibrationFiles() << " ";
    for (size_t i = 0; i < run->getNumberOfCalibrationFiles(); i++) {
      output << run->getCalibrationFile(i).native() << "\n";
    }
  }
}

void writeReactionShape(std::ostream& output, std::shared_ptr<base_parameter> const& item)
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
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    auto p = paramMap[ParameterValue::ValueType::POINT3D][k].second.value<point3D>();
    output << "P" << p.X() << " " << p.Y() << " " << p.Z() << " "
           << paramMap[ParameterValue::ValueType::POINT3D][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    auto v = paramMap[ParameterValue::ValueType::VECTOR3D][k].second.value<vector3D>();
    output << "v" << v.Theta() * 180. / M_PI << " " << v.Phi() * 180. / M_PI << " " << v.R() << " "
           << paramMap[ParameterValue::ValueType::VECTOR3D][k].first << "\n";
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
  output << "="
         << "\n"
         << std::flush;
}

void ParameterWriter0::writeSingleReaction(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto reaction = std::dynamic_pointer_cast<reaction_parameter>(item);
  if (!reaction) {
    return;
  }
  output << ((reaction->hasTwoBeams()) ? 1 : 0) << " " << reaction->getMaterial(0) << " "
         << reaction->getMaterial(1) << " " << reaction->getBeamMomentum();
  if (reaction->hasTwoBeams())
    output << " " << reaction->getBeamMomentum(1);
  output << " " << 1 << " " << reaction->getName().data() << "\n";
  output << reaction->getDescription() << "\n" << std::flush;
  auto sh = std::make_shared<shape_parameter>(reaction->getTargetShape());
  writeReactionShape(output, sh);
}

void ParameterWriter0::writeSingleDetector(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from write_detector_parameter_0
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
  output << paramMap[ParameterValue::ValueType::STRING].size() << sh->getName() << "\n";
  point3D p;
  vector3D v;
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    p = paramMap[ParameterValue::ValueType::POINT3D][k].second.value<point3D>();
    output << p.X() << " " << p.Y() << " " << p.Z()
           << paramMap[ParameterValue::ValueType::POINT3D][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    v = paramMap[ParameterValue::ValueType::VECTOR3D][k].second.value<vector3D>();
    output << v.X() << " " << v.Y() << " " << v.Z()
           << paramMap[ParameterValue::ValueType::VECTOR3D][k].first << "\n";
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
    output << v.second.value<std::string>() << "\n" << v.first << "\n";
  }
  output.flush();
}

void ParameterWriter0::writeSingleMaterial(std::ostream& output,
                                           std::shared_ptr<base_parameter> const& item) const
{
  // from ostream operator << (...)
  auto material = std::dynamic_pointer_cast<material_parameter>(item);
  if (!material) {
    return;
  }
  output << (material->IsActive() ? 1 : 0) << " ";
  output << material->Density() << " " << material->RadiationLength() << " " << material->Speed()
         << " ";
  output << material->getName().data() << "\n";
  output << material->NumberOfEnergyLossParams() << " ";
  for (int j = 0; j < material->NumberOfEnergyLossParams(); j++) {
    output << material->EnergyLoss(j) << " ";
  }
  output << material->NumberOfElements() << " ";
  for (int j = 0; j < material->NumberOfElements(); j++)
    output << material->Element(j).getMass() << " " << material->Element(j).getCharge() << " "
           << material->Element(j).getWeight() << " " << material->Element(j).getName().data()
           << "\n";
}

void ParameterWriter0::writeSingleShape(std::ostream& output,
                                        std::shared_ptr<base_parameter> const& item) const
{
  // from write_shape_parameter_0
  auto shape = std::dynamic_pointer_cast<shape_parameter>(item);
  if (!shape) {
    return;
  }
  std::map<ParameterValue::ValueType, std::vector<std::pair<std::string, ParameterValue>>> paramMap;
  for (size_t i = 0; i < shape->numberOfValues(); i++) {
    paramMap[shape->value(i).valueType()].push_back(
        std::make_pair(shape->valueName(i), shape->value(i)));
  }
  output << paramMap[ParameterValue::ValueType::POINT3D].size() << " ";
  output << paramMap[ParameterValue::ValueType::VECTOR3D].size() << " ";
  output << paramMap[ParameterValue::ValueType::INT].size() << " ";
  output << paramMap[ParameterValue::ValueType::FLOAT].size() << " ";
  output << paramMap[ParameterValue::ValueType::STRING].size() << shape->getName() << "\n";
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::POINT3D].size(); k++) {
    output << paramMap[ParameterValue::ValueType::POINT3D][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::VECTOR3D].size(); k++) {
    output << paramMap[ParameterValue::ValueType::VECTOR3D][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::INT].size(); k++) {
    output << paramMap[ParameterValue::ValueType::INT][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::FLOAT].size(); k++) {
    output << paramMap[ParameterValue::ValueType::FLOAT][k].first << "\n";
  }
  for (size_t k = 0; k < paramMap[ParameterValue::ValueType::STRING].size(); k++) {
    output << paramMap[ParameterValue::ValueType::STRING][k].first << "\n";
  }
}

bool ParameterWriter0::isFiletypeAvailable(BaseParameterWriter::FileType check) const
{
  return (check == FileType::RUN || check == FileType::SHAPE || check == FileType::BEAMTIME ||
          check == FileType::ALGORITHM || check == FileType::MATERIALS ||
          check == FileType::DETECTOR_REACTION || check == FileType::DATABASE);
}

string ParameterWriter0::getVersionString(BaseParameterWriter::FileType filetype) const
{
  Q_UNUSED(filetype);
  return {};
}

} // namespace IO
} // namespace parameter
