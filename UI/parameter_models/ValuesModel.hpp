#pragma once
#include "valuesparameter.h"

#include <QAbstractListModel>

#include <optional>

namespace models {

class ValuesModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum ValueRoles
  {
    NameRole = Qt::UserRole + 1,
    TypeRole,
    ValueRole
  };

public:
  ValuesModel(std::shared_ptr<values_parameter> data, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

private:
  std::shared_ptr<values_parameter> _data;
};

} // namespace models
