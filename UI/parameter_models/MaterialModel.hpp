#pragma once
#include "AllData.hpp"

#include <QAbstractListModel>

#include <memory>

namespace models {

class MaterialModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum MaterialRoles
  {
    NameRole = Qt::UserRole + 1,
    ActiveRole,
    RadiationLengthRole,
    DensityRole,
    SpeedOfLightRole,
    EnergyLossRole,
    ElementsRole,
  };

public:
  MaterialModel(UI::AllData& data, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void loadFile(QString const& filename);
  Q_INVOKABLE void add();
  Q_INVOKABLE void remove(int index);

private:
  UI::AllData& _allData;
  std::vector<std::shared_ptr<material_parameter>> _data;
};

} // namespace models
