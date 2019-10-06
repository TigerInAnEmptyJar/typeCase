#include "algorithmparameter.h"

#include <sstream>
#include <typeinfo>

algorithm_parameter::algorithm_parameter(string n, Category cat, int lev, int id)
{
  ID = id;
  setName(n);
  inUse = false;
  category = cat;
  level = lev;
}

algorithm_parameter::algorithm_parameter(string n, int id)
{
  ID = id;
  setName(n);
  inUse = false;
}

int algorithm_parameter::getID() const { return ID; }

void algorithm_parameter::setID(int id) { ID = id; }

algorithm_parameter::algorithm_parameter(string n, bool u, Category cat, int lev, int id)
{
  ID = id;
  setName(n);
  inUse = u;
  category = cat;
  level = lev;
}

void algorithm_parameter::setUse(bool in) { inUse = in; }

bool algorithm_parameter::IsUsed() const { return inUse; }

void algorithm_parameter::setCategory(Category cat) { category = cat; }

algorithm_parameter::Category algorithm_parameter::getCategory() const { return category; }

void algorithm_parameter::setLevel(int lev) { level = lev; }

int algorithm_parameter::getLevel() const { return level; }

bool algorithm_parameter::operator==(const algorithm_parameter& ap) const
{
  if (ID != ap.getID())
    return false;
  if (getName() != ap.getName())
    return false;
  if (numberOfValues() != ap.numberOfValues())
    return false;
  for (size_t i = 0; i < numberOfValues(); i++) {
    if (!(value(i) == ap.value(i))) {
      return false;
    }
    if (valueName(i) != ap.valueName(i)) {
      return false;
    }
  }
  return true;
}

string algorithm_parameter::toString(int pattern) const
{
  std::stringstream o;
  if (pattern == 1 || pattern == 2) {
    o << string(pattern == 2 ? "Category: " : "") + to_string(category);
    o << string(pattern == 2 ? "\nLevel: " : " ") + string_number(level);
    o << string(pattern == 2 ? "\nID: " : " ") + string_number(ID);
    o << string(pattern == 2 ? "\nis used: " : " ") + (inUse ? "1" : "0") + "\n";
  } else {
    o << string(pattern == 3 ? "\nID: " : " ") + string_number(ID) + "\n";
  }
  o << "Parameters:\n";
  for (size_t i = 0; i < numberOfValues(); i++) {
    o << valueName(i) << ": " << value(i) << "\n";
  }

  return o.str();
}
ostream& operator<<(ostream& o, const algorithm_parameter& a)
{
    string s;
    o << a.getName().data() << ":" << endl;
    o << " Category/Level:" << to_string(a.getCategory()) << "/" << a.getLevel() << endl;
    o << " Use:" << (a.IsUsed() ? "true" : "false") << endl;
    o << " ID:" << a.getID() << endl;
    for (size_t i = 0; i < a.numberOfValues(); i++) {
      o << a.value(i) << " " << a.valueName(i) << std::endl;
    }
  return o;
}

string to_string(algorithm_parameter::Category category)
{
  switch (category) {
  case algorithm_parameter::Category::INPUT:
    return "INPUT";
  case algorithm_parameter::Category::OUTPUT:
    return "OUTPUT";
  case algorithm_parameter::Category::CALIBRATION:
    return "CALIBRATION";
  case algorithm_parameter::Category::PIXEL:
    return "PIXEL";
  case algorithm_parameter::Category::TRACKING:
    return "TRACKING";
  case algorithm_parameter::Category::POST_TRACKING:
    return "POST_TRACKING";
  case algorithm_parameter::Category::HISTOGRAMMING:
    return "HISTOGRAMMING";
  case algorithm_parameter::Category::FITTING:
    return "FITTING";
  case algorithm_parameter::Category::ELSE:
    return "ELSE";
  }
  return {};
}
