#include "DetectorModel.hpp"

#include "parameterio.h"
#include "versions.h"

#include <QSettings>

namespace models {
DetectorModel::DetectorModel(UI::AllData& data, QObject* parent)
    : QAbstractListModel(parent), _alldata(data)
{
  auto detectors = _alldata.detectors();
  std::transform(detectors.begin(), detectors.end(), std::back_inserter(_data), [](auto item) {
    return std::make_pair(item, std::make_shared<ValuesModel>(item->getShape()));
  });
  _alldata.connectDetectorChanged([this]() {
    this->beginResetModel();
    _data.clear();
    auto detectors = _alldata.detectors();
    std::transform(detectors.begin(), detectors.end(), std::back_inserter(_data), [](auto item) {
      return std::make_pair(item, std::make_shared<ValuesModel>(item->getShape()));
    });
    this->endResetModel();
  });
}

int DetectorModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant DetectorModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case DetectorRoles::NameRole:
    return QString::fromStdString(_data[position].first->getName());
  case DetectorRoles::ElementsRole:
    return _data[position].first->getNumberOfElements();
  case DetectorRoles::StacktypeRole:
    return _data[position].first->getStackType();
  case DetectorRoles::CircularRole:
    return _data[position].first->isCircular();
  case DetectorRoles::MaxDistanceRole:
    return _data[position].first->getMaxDistance();
  case DetectorRoles::ShapeTypeRole:
    return QString::fromStdString(_data[position].first->getShape()->getName());
  case DetectorRoles::ShapeValuesRole:
    return QVariant::fromValue(_data[position].second.get());
  case DetectorRoles::MaterialRole: {
    auto materials = _alldata.materials();
    size_t mat = static_cast<size_t>(_data[position].first->getMaterial());
    if (mat < materials.size()) {
      return QString::fromStdString(_alldata.materials()[mat]->getName());
    }
    break;
  }
  }
  return {};
}

QHash<int, QByteArray> DetectorModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(DetectorRoles::NameRole, "name");
  result.insert(DetectorRoles::ElementsRole, "elements");
  result.insert(DetectorRoles::StacktypeRole, "stackType");
  result.insert(DetectorRoles::CircularRole, "circular");
  result.insert(DetectorRoles::MaxDistanceRole, "maxDistance");
  result.insert(DetectorRoles::ShapeTypeRole, "shapeType");
  result.insert(DetectorRoles::ShapeValuesRole, "shapeValues");
  result.insert(DetectorRoles::MaterialRole, "material");
  return result;
}

Qt::ItemFlags DetectorModel::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool DetectorModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return false;
  }
  switch (role) {
  case DetectorRoles::NameRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setName(value.toString().toStdString());
    break;
  }
  case DetectorRoles::ElementsRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setNumberOfElements(value.toString().toInt());
    break;
  }
  case DetectorRoles::StacktypeRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setStackType(value.toString().toInt());
    break;
  }
  case DetectorRoles::CircularRole: {
    if (value.type() != QVariant::Bool) {
      return false;
    }
    _data[position].first->setCircular(value.toBool());
    break;
  }
  case DetectorRoles::MaxDistanceRole: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setMaxDistance(value.toString().toFloat());
    break;
  }
  case DetectorRoles::ShapeTypeRole: {
    if (value.type() != QVariant::Int) {
      return false;
    }
    auto shapes = _alldata.definedShapes();
    if (shapes.size() <= value.toUInt()) {
      return false;
    }
    auto shape = shapes[value.toUInt()];
    _data[position].first->setShape(*shape);
    _data[position].second = std::make_shared<ValuesModel>(_data[position].first->getShape());
    dataChanged(index, index, {DetectorRoles::ShapeValuesRole});
    break;
  }
  case DetectorRoles::ShapeValuesRole:
    return false;
  case DetectorRoles::MaterialRole: {
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first->setMaterial(value.toInt());
    auto material = _alldata.materials()[static_cast<size_t>(value.toInt())];
    _data[position].first->setMaterial(material.get());
    break;
  }
  }
  dataChanged(index, index, {role});
  return true;
}

void DetectorModel::loadFile(QString const& filename)
{
  std::filesystem::path path{filename.right(filename.length() - 7).toStdString()};
  _alldata.loadSetup(path, parameter::IO::version_json);
}

void DetectorModel::add()
{
  auto det = std::make_shared<detector_parameter>();
  det->setName("New Detector");
  auto shapes = _alldata.definedShapes();
  det->setShape(*shapes[0]);
  det->setMaterial(0);
  det->setMaterial(_alldata.materials()[0].get());
  _alldata.addDetector(det);
}

void DetectorModel::remove(int index)
{
  if (index < 0 || static_cast<size_t>(index) >= _data.size()) {
    return;
  }
  _alldata.removeDetector(_data[static_cast<size_t>(index)].first);
}

} // namespace models
