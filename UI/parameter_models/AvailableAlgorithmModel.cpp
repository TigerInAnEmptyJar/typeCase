#include "AvailableAlgorithmModel.hpp"

#include "algorithmparameter.h"

namespace models {
AvailableAlgorithmModel::AvailableAlgorithmModel(UI::AllData& data, QObject* parent)
    : QAbstractListModel(parent), _alldata(data)
{
  _alldata.connectShapesChanged([this]() { onAlgorithmListChanged(); });
  onAlgorithmListChanged();
}

int AvailableAlgorithmModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant AvailableAlgorithmModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case AlgorithmRoles::NameRole:
    return QString::fromStdString(_data[position].first->getName());
  case AlgorithmRoles::DescriptionRole:
    return QString::fromStdString(_data[position].first->getDescription());
  case AlgorithmRoles::ValuesRole:
    return QVariant::fromValue(_data[position].second.get());
  }
  return {};
}

QHash<int, QByteArray> AvailableAlgorithmModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(AlgorithmRoles::NameRole, "name");
  result.insert(AlgorithmRoles::DescriptionRole, "description");
  result.insert(AlgorithmRoles::ValuesRole, "values");
  return result;
}

void AvailableAlgorithmModel::onAlgorithmListChanged()
{
  beginResetModel();
  _data.clear();
  auto algorithms = _alldata.definedAlgorithms();
  std::transform(algorithms.begin(), algorithms.end(), std::back_inserter(_data), [](auto item) {
    auto element = std::make_shared<algorithm_parameter>(*item);
    return std::make_pair(element, std::make_shared<ValuesModel>(
                                       std::dynamic_pointer_cast<values_parameter>(element)));
  });
  endResetModel();
}

} // namespace models
