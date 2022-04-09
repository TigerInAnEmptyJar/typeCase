#include "ShapeModel.hpp"

#include "shapeparameter.h"

namespace models {
ShapeModel::ShapeModel(UI::AllData& data, QObject* parent)
    : QAbstractListModel(parent), _alldata(data)
{
  _alldata.connectShapesChanged([this]() { onShapeListChanged(); });
  onShapeListChanged();
}

int ShapeModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant ShapeModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case ShapeRoles::NameRole:
    return QString::fromStdString(_data[position].first->getName());
  case ShapeRoles::DescriptionRole:
    return QString::fromStdString(_data[position].first->getDescription());
  case ShapeRoles::ValuesRole:
    return QVariant::fromValue(_data[position].second.get());
  }
  return {};
}

QHash<int, QByteArray> ShapeModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(ShapeRoles::NameRole, "name");
  result.insert(ShapeRoles::DescriptionRole, "description");
  result.insert(ShapeRoles::ValuesRole, "values");
  return result;
}

void ShapeModel::onShapeListChanged()
{
  beginResetModel();
  _data.clear();
  auto shapes = _alldata.definedShapes();
  std::transform(shapes.begin(), shapes.end(), std::back_inserter(_data), [](auto item) {
    auto element = std::make_shared<shape_parameter>(*item);
    return std::make_pair(element, std::make_shared<ValuesModel>(
                                       std::dynamic_pointer_cast<values_parameter>(element)));
  });
  endResetModel();
}

} // namespace models
