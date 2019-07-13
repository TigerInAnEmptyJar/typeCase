#include "baseparameter.h"

#include <iterator>

void base_parameter::setName(const std::string& n) { _name = n; }

void base_parameter::setDescription(std::string const& d) { _description = d; }

std::string base_parameter::getName() const { return _name; }

std::string base_parameter::getDescription() const { return _description; }

bool base_parameter::operator==(base_parameter p) { return (_name == p.getName()); }

void base_parameter::setId(boost::uuids::uuid const& id) { _id = id; }
boost::uuids::uuid base_parameter::id() const { return _id; }

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
