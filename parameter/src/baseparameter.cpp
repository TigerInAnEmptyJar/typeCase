#include "baseparameter.h"

#include <iterator>
#include <sstream>


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

