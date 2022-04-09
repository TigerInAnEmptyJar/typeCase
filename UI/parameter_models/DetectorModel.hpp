#pragma once
#include "AllData.hpp"

#include "ValuesModel.hpp"

namespace models {

class DetectorModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum DetectorRoles
  {
    NameRole = Qt::UserRole + 1,
    ElementsRole,
    StacktypeRole,
    CircularRole,
    MaxDistanceRole,
    ShapeTypeRole,
    ShapeValuesRole,
    MaterialRole,
  };

public:
  DetectorModel(UI::AllData& data, QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void loadFile(QString const& filename);
  Q_INVOKABLE void add();
  Q_INVOKABLE void remove(int index);

private:
  UI::AllData& _alldata;
  std::vector<std::pair<std::shared_ptr<detector_parameter>, std::shared_ptr<ValuesModel>>> _data;
};

} // namespace models
