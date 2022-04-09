#include "RunModel.hpp"

#include "parameterio.h"
#include "versions.h"

#include <QDateTime>
#include <QSettings>

namespace models {
RunModel::RunModel(UI::AllData& data, boost::uuids::uuid bt, QObject* parent)
    : QAbstractListModel(parent), _beamtime(bt), _alldata(data)
{
  auto onDbChanged = [this]() {
    beginResetModel();
    auto db = _alldata.database();
    _data.clear();
    std::transform(
        db.begin(), db.end(), std::back_inserter(_data),
        [this](auto item)
            -> std::pair<std::shared_ptr<beamTime_parameter>, std::shared_ptr<RunModel>> {
          if (_beamtime.is_nil()) {
            if (std::dynamic_pointer_cast<beamTime_parameter>(item) &&
                !std::dynamic_pointer_cast<run_parameter>(item)) {
              return std::make_pair(
                  std::dynamic_pointer_cast<beamTime_parameter>(item),
                  std::make_shared<RunModel>(_alldata,
                                             dynamic_pointer_cast<beamTime_parameter>(item)->id()));
            }
            return std::make_pair(nullptr, nullptr);
          }
          auto element = std::dynamic_pointer_cast<run_parameter>(item);
          if (element && element->getParentNumber() == _beamtime) {
            return std::make_pair(element, nullptr);
          }
          return std::make_pair(nullptr, nullptr);
        });
    _data.erase(std::remove_if(_data.begin(), _data.end(),
                               [](auto item) { return !(item.first || item.second); }),
                _data.end());
    endResetModel();
  };
  _alldata.connectDatabaseChanged(onDbChanged);
  onDbChanged();
}

int RunModel::rowCount(QModelIndex const&) const { return static_cast<int>(_data.size()); }

QVariant RunModel::data(const QModelIndex& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return {};
  }
  switch (role) {
  case RunRoles::NameRole:
    return QString::fromStdString(_data[position].first->getName());
  case RunRoles::RunsRole:
    if (_beamtime.is_nil()) {
      return QVariant::fromValue(_data[position].second.get());
    }
    break;
  case RunRoles::SetupFileRole:
    return QString::fromStdString(_data[position].first->getSetupFile());
  case RunRoles::MonthRole:
    return _data[position].first->getMonth();
  case RunRoles::YearRole:
    return _data[position].first->getYear();
  case RunRoles::TypeRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      return static_cast<int>(run->getType());
    }
    return {};
  }
  case RunRoles::OwnSetupRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      return run->hasOwnSetup();
    }
    return {};
  }
  case RunRoles::OwnCalibrationRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      return run->hasOwnCalibration();
    }
    return {};
  }
  case RunRoles::AdditionalCalibrationRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      return run->hasAdditionalCalibration();
    }
    return {};
  }
  case RunRoles::IdentifierRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      return run->getRunNumber();
    }
    return {};
  }
  case RunRoles::StartRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      auto time = run->getStartTime();
      return QDateTime{QDate{time.tm_year + 1900, time.tm_mon, time.tm_mday},
                       QTime{time.tm_hour, time.tm_min, time.tm_sec}}
          .toString("dd.MM.yyyy-HH:mm:ss");
    }
    return {};
  }
  case RunRoles::StopRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (run) {
      auto time = run->getStopTime();
      return QDateTime{QDate{time.tm_year + 1900, time.tm_mon, time.tm_mday},
                       QTime{time.tm_hour, time.tm_min, time.tm_sec}}
          .toString("dd.MM.yyyy-HH:mm:ss");
    }
    return {};
  }
  }
  return {};
}

Qt::ItemFlags RunModel::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool RunModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data.size() || role == RunRoles::RunsRole) {
    return false;
  }
  switch (role) {
  case RunRoles::NameRole:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setName(value.toString().toStdString());
    break;
  case RunRoles::SetupFileRole:
    if (value.type() != QVariant::String) {
      return false;
    }
    _data[position].first->setSetupFile(value.toString().toStdString());
    break;
  case RunRoles::MonthRole:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first->setData(value.toInt(), _data[position].first->getYear());
    break;
  case RunRoles::YearRole:
    if (value.type() != QVariant::Int) {
      return false;
    }
    _data[position].first->setData(_data[position].first->getMonth(), value.toInt());
    break;
  case RunRoles::TypeRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::Int) {
      return false;
    }
    run->setType(static_cast<run_parameter::RunType>(value.toInt()));
    break;
  }
  case RunRoles::OwnSetupRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::Bool) {
      return false;
    }
    run->setOwnSetup(value.toBool());
    break;
  }
  case RunRoles::OwnCalibrationRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::Bool) {
      return false;
    }
    run->setOwnCalibration(value.toBool());
    break;
  }
  case RunRoles::AdditionalCalibrationRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::Bool) {
      return false;
    }
    run->setAdditionalCalibration(value.toBool());
    break;
  }
  case RunRoles::IdentifierRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::String) {
      return false;
    }
    bool ok = false;
    int number = value.toString().toInt(&ok);
    if (!ok) {
      return false;
    }
    run->setRunNumber(number);
    break;
  }
  case RunRoles::StartRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::String) {
      return false;
    }
    QDateTime date = QDateTime::fromString(value.toString(), "dd.MM.yyyy-HH:mm:ss");
    if (!date.isValid()) {
      return false;
    }
    run->setStartTime(date.date().year(), date.date().month(), date.date().day(),
                      date.time().hour(), date.time().minute(), date.time().second());
    break;
  }
  case RunRoles::StopRole: {
    auto run = std::dynamic_pointer_cast<run_parameter>(_data[position].first);
    if (!run || value.type() != QVariant::String) {
      return false;
    }
    QDateTime date = QDateTime::fromString(value.toString(), "dd.MM.yyyy-HH:mm:ss");
    if (!date.isValid()) {
      return false;
    }
    run->setStopTime(date.date().year(), date.date().month(), date.date().day(), date.time().hour(),
                     date.time().minute(), date.time().second());
    break;
  }
  }
  dataChanged(index, index, {role});
  std::cout << "accepted " << value.toString().toStdString() << " for role " << role << std::endl;
  return true;
}

void RunModel::addBeamtime()
{
  auto bt = std::make_shared<beamTime_parameter>();
  bt->setName("New Beamtime");
  _alldata.addBeamtime(bt);
}

void RunModel::removeBeamtime(int index)
{
  if (index < 0 || static_cast<size_t>(index) >= _data.size() || !_beamtime.is_nil()) {
    return;
  }
  _alldata.removeBeamtime(_data[static_cast<size_t>(index)].first);
}

void RunModel::addRun()
{
  if (_beamtime.is_nil()) {
    return;
  }
  auto run = std::make_shared<run_parameter>();
  {
    auto all = _alldata.database();
    auto parentBt =
        std::find_if(all.begin(), all.end(), [this](auto item) { return item->id() == _beamtime; });
    if (parentBt == all.end()) {
      return;
    }
    run->setParent(*parentBt);
  }
  run->setName("New Run");
  run->setParentNumber(_beamtime);
  _alldata.addRun(run);
}

void RunModel::removeRun(int index)
{
  if (index < 0 || static_cast<size_t>(index) >= _data.size() || _beamtime.is_nil()) {
    return;
  }
  _alldata.removeRun(
      std::dynamic_pointer_cast<run_parameter>(_data[static_cast<size_t>(index)].first));
}

QHash<int, QByteArray> RunModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(RunRoles::NameRole, "name");
  result.insert(RunRoles::RunsRole, "runs");
  result.insert(RunRoles::SetupFileRole, "setupfile");
  result.insert(RunRoles::MonthRole, "month");
  result.insert(RunRoles::YearRole, "year");
  result.insert(RunRoles::TypeRole, "type");
  result.insert(RunRoles::OwnSetupRole, "ownSetup");
  result.insert(RunRoles::OwnCalibrationRole, "ownCalibration");
  result.insert(RunRoles::AdditionalCalibrationRole, "additionalCalibration");
  result.insert(RunRoles::IdentifierRole, "identifier");
  result.insert(RunRoles::StartRole, "start");
  result.insert(RunRoles::StopRole, "stop");
  return result;
}

void RunModel::loadFile(QString const& filename)
{
  std::filesystem::path path{filename.right(filename.length() - 7).toStdString()};
  _alldata.loadDatabase(path, parameter::IO::version_json);
}

} // namespace models
