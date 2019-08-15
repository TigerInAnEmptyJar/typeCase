#include "baseparameter.h"

#include <iterator>
#include <sstream>

bool ParameterValue::operator==(const ParameterValue& other) const
{
  return _value == other._value;
}

ParameterValue::ValueType ParameterValue::valueType() const
{
  return static_cast<ValueType>(_value.index());
}

bool ParameterValue::isDefined() const { return _value.index() < std::variant_npos; }

std::ostream& operator<<(std::ostream& o, ParameterValue const& v)
{
  switch (v.valueType()) {
  case ParameterValue::ValueType::BOOLEAN:
    o << v.value<bool>();
    break;
  case ParameterValue::ValueType::INT:
    o << v.value<int>();
    break;
  case ParameterValue::ValueType::FLOAT:
    o << v.value<float>();
    break;
  case ParameterValue::ValueType::DOUBLE:
    o << v.value<double>();
    break;
  case ParameterValue::ValueType::POINT3D:
    o << v.value<point3D>();
    break;
  case ParameterValue::ValueType::VECTOR3D:
    o << v.value<vector3D>();
    break;
  case ParameterValue::ValueType::STRING:
    o << v.value<std::string>();
    break;
  case ParameterValue::ValueType::VECTOR_INT: {
    auto& vv = v.value<std::vector<int>>();
    std::stringstream s;
    for (auto element : vv) {
      s << ", " << element;
    }
    if (vv.size() > 0) {
      o << "(" << s.str().substr(2) << ")";
    } else {
      o << "()";
    }
  } break;
  case ParameterValue::ValueType::VECTOR_FLOAT: {
    auto& vv = v.value<std::vector<float>>();
    std::stringstream s;
    for (auto element : vv) {
      s << ", " << element;
    }
    if (vv.size() > 0) {
      o << "(" << s.str().substr(2) << ")";
    } else {
      o << "()";
    }
  } break;
  case ParameterValue::ValueType::VECTOR_DOUBLE: {
    auto& vv = v.value<std::vector<double>>();
    std::stringstream s;
    for (auto element : vv) {
      s << ", " << element;
    }
    if (vv.size() > 0) {
      o << "(" << s.str().substr(2) << ")";
    } else {
      o << "()";
    }
  } break;
  case ParameterValue::ValueType::VECTOR_STRING: {
    auto& vv = v.value<std::vector<std::string>>();
    std::stringstream s;
    for (auto element : vv) {
      s << ", " << element;
    }
    if (vv.size() > 0) {
      o << "(" << s.str().substr(2) << ")";
    } else {
      o << "()";
    }
  } break;
  default:
    break;
  }
  return o;
}

//*************************************************************************************************

void base_parameter::setName(const std::string& n) { _name = n; }

void base_parameter::setDescription(std::string const& d) { _description = d; }

std::string base_parameter::getName() const { return _name; }

std::string base_parameter::getDescription() const { return _description; }

bool base_parameter::operator==(base_parameter const& p) const
{
  return (_name == p._name && _id == p._id);
}

void base_parameter::setId(boost::uuids::uuid const& id) { _id = id; }
boost::uuids::uuid base_parameter::id() const { return _id; }

//*************************************************************************************************

bool values_parameter::operator==(values_parameter const& other) const
{
  return base_parameter::operator==(other) && _values == other._values;
}

size_t values_parameter::numberOfValues() const { return _values.size(); }

std::string values_parameter::valueName(size_t index) const { return _values[index].first; }

ParameterValue const& values_parameter::value(size_t index) const { return _values[index].second; }

ParameterValue& values_parameter::value(size_t index) { return _values[index].second; }

void values_parameter::addValue(std::string const& name, ParameterValue const& value)
{
  _values.push_back(std::make_pair(name, value));
}

void values_parameter::removeValue(size_t index) { _values.erase(_values.begin() + index); }

void values_parameter::clearValues() { _values.clear(); }
