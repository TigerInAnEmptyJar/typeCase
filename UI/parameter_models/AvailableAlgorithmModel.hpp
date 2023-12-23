#pragma once
#include "ValuesModel.hpp"

#include "AllData.hpp"

#include <QAbstractListModel>

namespace models {

class AvailableAlgorithmModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum AlgorithmRoles
  {
    NameRole = Qt::UserRole + 1,
    DescriptionRole,
    ValuesRole
  };

public:
  AvailableAlgorithmModel(UI::AllData& data, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void onAlgorithmListChanged();

private:
  UI::AllData& _alldata;
  std::vector<std::pair<std::shared_ptr<algorithm_parameter>, std::shared_ptr<ValuesModel>>> _data;
};

} // namespace models
