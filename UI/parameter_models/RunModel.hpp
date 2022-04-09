#pragma once
#include "AllData.hpp"

#include "beamtimeparameter.h"

#include <QAbstractListModel>

namespace models {

class RunModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum RunRoles
  {
    NameRole = Qt::UserRole + 1,
    RunsRole,
    SetupFileRole,
    MonthRole,
    YearRole,
    TypeRole,
    OwnSetupRole,
    OwnCalibrationRole,
    AdditionalCalibrationRole,
    IdentifierRole,
    StartRole,
    StopRole,
  };

public:
  RunModel(UI::AllData& data, boost::uuids::uuid bt, QObject* parent = nullptr);
  int rowCount(QModelIndex const& parent = QModelIndex()) const override;
  QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Qt::ItemFlags flags(QModelIndex const& index) const override;
  bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole) override;

  Q_INVOKABLE void addBeamtime();
  Q_INVOKABLE void removeBeamtime(int index);
  Q_INVOKABLE void addRun();
  Q_INVOKABLE void removeRun(int index);
  Q_INVOKABLE void loadFile(QString const& filename);

private:
  boost::uuids::uuid _beamtime;
  UI::AllData& _alldata;
  std::vector<std::pair<std::shared_ptr<beamTime_parameter>, std::shared_ptr<RunModel>>> _data;
};

} // namespace models
