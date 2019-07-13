#include "geometry.h"
#ifdef USEROOT
#include <TObject.h>
#endif

// geomObject::geomObject()
// {
//     status=_undefined_;
// }

// geomObject::~geomObject()
// {
// }

// geomState geomObject::getState()const
// {
//     return status;
// }

// void geomObject::Zero()
// {
//     status = _zero_;
// }

// void geomObject::Infinite()
// {
//     status = _infinite_;
// }

// void geomObject::Undefined()
// {
//     status = _undefined_;
// }

// void geomObject::Regular()
// {
//     status = _regular_;
// }
//#ifdef USEQT
string geomObject::toString(int) const
{
  switch (status) {
  case _regular_:
    return string("(regular)");
  case _undefined_:
    return string("(undefined)");
  case _infinite_:
    return string("(infinite)");
  case _zero_:
    return string("(zero)");
  }
  return string("(undefined)");
}
//#endif
