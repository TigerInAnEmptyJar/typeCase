#pragma once
#include "ValuesModel.hpp"

#include "AllData.hpp"

#include <QAbstractListModel>

namespace models {

class ShapeModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum ShapeRoles
  {
    NameRole = Qt::UserRole + 1,
    DescriptionRole,
    ValuesRole
  };

public:
  ShapeModel(UI::AllData& data, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void onShapeListChanged();

private:
  UI::AllData& _alldata;
  std::vector<std::pair<std::shared_ptr<shape_parameter>, std::shared_ptr<ValuesModel>>> _data;
};

} // namespace models
