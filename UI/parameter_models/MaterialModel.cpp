#include "MaterialModel.hpp"

#include "materialparameter.h"
#include "versions.h"

namespace models {
MaterialModel::MaterialModel(UI::AllData& data, QObject* parent)
    : QAbstractListModel(parent), _allData(data)
{
  _data = _allData.materials();
  _allData.connectMaterialChanged([this]() {
    this->beginResetModel();
    _data = _allData.materials();
    this->endResetModel();
  });
}

int MaterialModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant MaterialModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case MaterialRoles::NameRole:
    return QString::fromStdString(_data[position]->getName());
  case MaterialRoles::ActiveRole:
    return _data[position]->IsActive();
  case MaterialRoles::RadiationLengthRole:
    return _data[position]->RadiationLength();
  case MaterialRoles::DensityRole:
    return _data[position]->Density();
  case MaterialRoles::SpeedOfLightRole:
    return _data[position]->Speed();
  case MaterialRoles::EnergyLossRole:
    return {};
  case MaterialRoles::ElementsRole:
    return {};
  }
  return {};
}

QHash<int, QByteArray> MaterialModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(MaterialRoles::NameRole, "name");
  result.insert(MaterialRoles::ActiveRole, "active");
  result.insert(MaterialRoles::RadiationLengthRole, "radiationLength");
  result.insert(MaterialRoles::DensityRole, "density");
  result.insert(MaterialRoles::SpeedOfLightRole, "speedOfLight");
  result.insert(MaterialRoles::EnergyLossRole, "energyLoss");
  result.insert(MaterialRoles::ElementsRole, "elements");

  return result;
}

Qt::ItemFlags MaterialModel::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool MaterialModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case MaterialRoles::NameRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->setName(value.toString().toStdString());
    break;
  }
  case MaterialRoles::ActiveRole: {
    if (value.type() != QVariant::Bool) {
      return false;
    }
    _data[position]->setActive(value.toBool() == 0);
    break;
  }
  case MaterialRoles::RadiationLengthRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->setRadiationLength(value.toFloat());
    break;
  }
  case MaterialRoles::DensityRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->setDensity(value.toFloat());
    break;
  }
  case MaterialRoles::SpeedOfLightRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position]->setSpeed(value.toFloat());
    break;
  }
  case MaterialRoles::EnergyLossRole:
  case MaterialRoles::ElementsRole:
    return false;
  }
  return true;
}

void MaterialModel::loadFile(QString const& filename)
{
  std::filesystem::path path{filename.right(filename.length() - 7).toStdString()};
  _allData.loadMaterials(path, parameter::IO::version_json);
}

void MaterialModel::add()
{
  auto mat = std::make_shared<material_parameter>();
  mat->setName("New Material");
  _allData.addMaterial(mat);
}

void MaterialModel::remove(int index)
{
  if (index < 0 || static_cast<size_t>(index) >= _data.size()) {
    return;
  }
  _allData.removeMaterial(_data[static_cast<size_t>(index)]);
}

} // namespace models
