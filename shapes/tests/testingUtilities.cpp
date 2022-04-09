#include "planeShape.h"
#include "shapeFactory.h"
#include "volumeShape.h"

#include <iostream>
#include <memory>

#include "testingUtilities.h"

std::ostream& operator<<(std::ostream& o, std::shared_ptr<volumeShape> const& s)
{
  o << s.get();
  return o;
}
std::ostream& operator<<(std::ostream& o, std::shared_ptr<planeShape> const& s)
{
  o << s.get();
  return o;
}
std::ostream& operator<<(std::ostream& o, ShapeType const& s)
{
  o << static_cast<int>(s);
  return o;
}
std::ostream& operator<<(std::ostream& o, volumeShape const& s)
{
  o << s.getName();
  return o;
}
std::ostream& operator<<(std::ostream& o, planeShape const& s)
{
  o << s.getName();
  return o;
}
