#include "shapeparameter.h"
#include <typeinfo>

bool shape_parameter::operator==(shape_parameter const& other) const
{
  return values_parameter::operator==(other);
}

bool shape_parameter::completeWrite() const { return _completeWrite; }
void shape_parameter::setCompleteWrite(bool value) { _completeWrite = value; }

ostream& operator<<(ostream& o, const shape_parameter& sh)
{
  o << "Shape: " << sh.getName().data() << "\n";
  if (sh.completeWrite()) {
    for (size_t i = 0; i < sh.numberOfValues(); i++) {
      o << static_cast<int>(sh.value(i).valueType()) << "\t" << sh.value(i) << "\t"
        << sh.valueName(i) << "\n";
    }
  } else {
    for (size_t i = 0; i < sh.numberOfValues(); i++) {
      o << static_cast<int>(sh.value(i).valueType()) << "\t" << sh.valueName(i) << "\n";
    }
  }
  return o;
}
