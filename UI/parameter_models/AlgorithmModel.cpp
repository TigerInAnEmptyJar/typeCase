#include "AlgorithmModel.hpp"

#include "parameterio.h"
#include "versions.h"

#include <QSettings>

namespace models {
AlgorithmModel::AlgorithmModel(UI::AllData& allData, QObject* parent)
    : QAbstractListModel(parent), _allData(allData)
{
  auto onAlgorithmChanged = [this]() {
    _data.clear();
    auto algos = _allData.definedAlgorithms();
    std::transform(algos.begin(), algos.end(), std::back_inserter(_data), [](auto item) {
      return std::make_pair(item, std::make_shared<ValuesModel>(item));
    });
  };
  _allData.connectAlgorithmChanged(onAlgorithmChanged);
  onAlgorithmChanged();
}

int AlgorithmModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant AlgorithmModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case AlgorithmRoles::NameRole:
    return QString::fromStdString(_data[position].first->getName());
  case AlgorithmRoles::ValuesRole:
    return QVariant::fromValue(_data[position].second.get());
  }
  return {};
}

QHash<int, QByteArray> AlgorithmModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(AlgorithmRoles::NameRole, "name");
  result.insert(AlgorithmRoles::ValuesRole, "values");
  return result;
}

// void AlgorithmModel::loadFile(QString const&filename)
//{
//     parameter::IO::ParameterIO io;
//     io.setup();
//     std::filesystem::path path{filename.right(filename.length()-7).toStdString()};
//     auto run_list = io.readParameter(path,
//                                      parameter::IO::version_json,
//                                      parameter::IO::ParameterIO::FileType::ALGORITHM);

//    QSettings settings;
//    settings.setValue(::lastFile, filename);
//    settings.sync();

//    beginResetModel();
//    _data.clear();
//    std::transform(run_list.begin(), run_list.end(), std::back_inserter(_data), [](auto item){
//        return std::dynamic_pointer_cast<algorithm_parameter>(item);
//    });
//    _data.erase(std::remove(_data.begin(), _data.end(), nullptr), _data.end());
//    endResetModel();
//}

} // namespace models
