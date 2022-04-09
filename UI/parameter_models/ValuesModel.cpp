#include "ValuesModel.hpp"

namespace {
template <typename T>
QString to_string(std::vector<T> const& vec)
{
  std::string result;
  std::for_each(vec.begin(), vec.end(),
                [&result](auto item) { result += std::to_string(item) + ", "; });
  if (result.length() > 1) {
    result = result.substr(0, result.length() - 2);
  }
  return QString::fromStdString(result);
}
template <>
QString to_string(std::vector<std::string> const& vec)
{
  std::string result;
  std::for_each(vec.begin(), vec.end(),
                [&result](auto item) { result += "\"" + item + "\"" + ", "; });
  if (result.length() > 1) {
    result = result.substr(0, result.length() - 2);
  }
  return QString::fromStdString(result);
}
std::optional<std::array<double, 3>> parse3DString(QString const& value)
{
  QString copy = value;
  auto values = copy.remove('(').remove(')').split(",");
  if (values.count() != 3) {
    return {};
  }
  return std::array<double, 3>{values[0].toDouble(), values[1].toDouble(), values[2].toDouble()};
}
} // namespace

namespace models {

ValuesModel::ValuesModel(std::shared_ptr<values_parameter> data, QObject* parent)
    : QAbstractListModel(parent), _data(data)
{
}

int ValuesModel::rowCount(QModelIndex const&) const
{
  return static_cast<int>(_data->numberOfValues());
}

QVariant ValuesModel::data(QModelIndex const& index, int role) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data->numberOfValues()) {
    return {};
  }
  switch (role) {
  case ValueRoles::NameRole:
    return QString::fromStdString(_data->valueName(position));
  case ValueRoles::TypeRole:
    switch (_data->value(position).valueType()) {
    case ParameterValue::ValueType::UNDEFINED:
      return "UNDEFINED";
    case ParameterValue::ValueType::BOOLEAN:
      return "BOOLEAN";
    case ParameterValue::ValueType::INT:
      return "INT";
    case ParameterValue::ValueType::FLOAT:
      return "FLOAT";
    case ParameterValue::ValueType::DOUBLE:
      return "DOUBLE";
    case ParameterValue::ValueType::POINT3D:
      return "POINT3D";
    case ParameterValue::ValueType::VECTOR3D:
      return "VECTOR3D";
    case ParameterValue::ValueType::STRING:
      return "STRING";
    case ParameterValue::ValueType::VECTOR_INT:
      return "VECTOR_INT";
    case ParameterValue::ValueType::VECTOR_FLOAT:
      return "VECTOR_FLOAT";
    case ParameterValue::ValueType::VECTOR_DOUBLE:
      return "VECTOR_DOUBLE";
    case ParameterValue::ValueType::VECTOR_STRING:
      return "VECTOR_STRING";
    case ParameterValue::ValueType::ALGORITHM:
      return "ALGORITHM";
    }
    break;
  case ValueRoles::ValueRole:
    switch (_data->value(position).valueType()) {
    case ParameterValue::ValueType::UNDEFINED:
      return "UNDEFINED";
    case ParameterValue::ValueType::BOOLEAN:
      return _data->value(position).value<bool>() ? "true" : "false";
    case ParameterValue::ValueType::INT:
      return QString::number(_data->value(position).value<int>());
    case ParameterValue::ValueType::FLOAT:
      return QString::number(_data->value(position).value<float>());
    case ParameterValue::ValueType::DOUBLE:
      return QString::number(_data->value(position).value<double>());
    case ParameterValue::ValueType::POINT3D: {
      auto value = _data->value(position).value<point3D>();
      return QString("(%1, %2, %3)").arg(value[0]).arg(value[1]).arg(value[2]);
    }
    case ParameterValue::ValueType::VECTOR3D: {
      auto value = _data->value(position).value<vector3D>();
      return QString("(%1, %2, %3)").arg(value[0]).arg(value[1]).arg(value[2]);
    }
    case ParameterValue::ValueType::STRING:
      return QString::fromStdString(_data->value(position).value<std::string>());
    case ParameterValue::ValueType::VECTOR_INT:
      return QString("{") + to_string(_data->value(position).value<std::vector<int>>()) + "}";
    case ParameterValue::ValueType::VECTOR_FLOAT:
      return QString("{") + to_string(_data->value(position).value<std::vector<float>>()) + "}";
    case ParameterValue::ValueType::VECTOR_DOUBLE:
      return QString("{") + to_string(_data->value(position).value<std::vector<double>>()) + "}";
    case ParameterValue::ValueType::VECTOR_STRING:
      return QString("{") + to_string(_data->value(position).value<std::vector<std::string>>()) +
             "}";
    case ParameterValue::ValueType::ALGORITHM:
      return "ALGORITHM";
    }
    break;
  }
  return {};
}

QHash<int, QByteArray> ValuesModel::roleNames() const
{
  QHash<int, QByteArray> result;
  result.insert(ValueRoles::NameRole, "name");
  result.insert(ValueRoles::TypeRole, "type");
  result.insert(ValueRoles::ValueRole, "value");
  return result;
}

Qt::ItemFlags ValuesModel::flags(QModelIndex const& index) const
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data->numberOfValues()) {
    return Qt::ItemFlag::NoItemFlags;
  }
  return Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable |
         Qt::ItemFlag::ItemIsEditable;
}

bool ValuesModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
  size_t const position = static_cast<size_t>(index.row());
  if (position >= _data->numberOfValues() || role != ValueRoles::ValueRole) {
    return false;
  }
  switch (_data->value(position).valueType()) {
  case ParameterValue::ValueType::UNDEFINED:
    return false;
  case ParameterValue::ValueType::BOOLEAN: {
    if (value.type() != QVariant::Bool) {
      return false;
    }
    _data->value(position) = value.toBool();
    break;
  }
  case ParameterValue::ValueType::INT: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data->value(position) = static_cast<int>(value.toString().toInt());
    break;
  }
  case ParameterValue::ValueType::FLOAT: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data->value(position) = static_cast<float>(value.toString().toDouble());
    break;
  }
  case ParameterValue::ValueType::DOUBLE: {
    if (value.type() != QVariant::String) {
      return false;
    }
    _data->value(position) = static_cast<double>(value.toString().toDouble());
    break;
  }
  case ParameterValue::ValueType::STRING: {
    if (value.type() != QVariant::String) {
      return false;
    }
    std::cout << value.toString().toStdString() << std::endl;
    _data->value(position) = value.toString().toStdString();
    break;
  }
  case ParameterValue::ValueType::POINT3D: {
    if (value.type() != QVariant::String) {
      return false;
    }
    auto v = ::parse3DString(value.toString());
    if (!v.has_value()) {
      return false;
    }
    _data->value(position) = point3D((*v)[0], (*v)[1], (*v)[2]);
    break;
  }
  case ParameterValue::ValueType::VECTOR3D: {
    if (value.type() != QVariant::String) {
      return false;
    }
    auto v = ::parse3DString(value.toString());
    if (!v.has_value()) {
      return false;
    }
    _data->value(position) = vector3D((*v)[0], (*v)[1], (*v)[2]);
    break;
  }
  case ParameterValue::ValueType::VECTOR_INT:
    return false;
  case ParameterValue::ValueType::VECTOR_FLOAT:
    return false;
  case ParameterValue::ValueType::VECTOR_DOUBLE:
    return false;
  case ParameterValue::ValueType::VECTOR_STRING:
    return false;
  case ParameterValue::ValueType::ALGORITHM:
    return false;
  }
  dataChanged(index, index, {role});
  return true;
}

} // namespace models
