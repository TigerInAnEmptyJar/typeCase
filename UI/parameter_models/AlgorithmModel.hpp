#pragma once
#include "AllData.hpp"
#include "ValuesModel.hpp"

#include "algorithmparameter.h"

#include <QAbstractListModel>

namespace models {

class AlgorithmModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum AlgorithmRoles
  {
    NameRole = Qt::UserRole + 1,
    ValuesRole,
  };

public:
  AlgorithmModel(UI::AllData& allData, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  //    Q_INVOKABLE void loadFile(QString const& filename);
private:
  std::vector<std::pair<std::shared_ptr<algorithm_parameter>, std::shared_ptr<ValuesModel>>> _data;
  UI::AllData& _allData;
};

} // namespace models
