#include "hexPrism.h"
#include "planarShapes.h"
#include "provideShapes.h"
#include "shapeparameter.h"

#include <stdlib.h>

namespace {

const boost::uuids::uuid hexPrism_id1 = {{0x10, 0xd9, 0x58, 0x58, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid hexPrism_id2 = {{0xa2, 0x0c, 0x6c, 0x70, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};
const boost::uuids::uuid hexPrism_id3 = {{0xe2, 0x71, 0x8b, 0x10, 0x11, 0xdc, 0x11, 0xe9, 0xab,
                                          0x14, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

auto calculateLine(size_t& currentLine, size_t currentLineSize, size_t remainingElements,
                   size_t stacki) -> size_t
{
  if (remainingElements < currentLineSize) {
    return remainingElements;
  }
  if (currentLine == stacki - 2) {
    return calculateLine(++currentLine, currentLineSize, remainingElements - currentLineSize,
                         stacki);
  }
  if (currentLine >= stacki - 1) {
    return calculateLine(++currentLine, currentLineSize - 1, remainingElements - currentLineSize,
                         stacki);
  }
  return calculateLine(++currentLine, currentLineSize + 1, remainingElements - currentLineSize,
                       stacki);
}

auto calculateLine2(size_t& currentLine, size_t currentLineSize, size_t remainingElements,
                    size_t stacki) -> size_t
{
  if (remainingElements < currentLineSize) {
    return remainingElements;
  }
  if (currentLine >= stacki) {
    return calculateLine(++currentLine, currentLineSize - 1, remainingElements - currentLineSize,
                         stacki);
  }
  return calculateLine(++currentLine, currentLineSize + 1, remainingElements - currentLineSize,
                       stacki);
}

struct ShapeProvider : public FactoryShapeProvider,
                       public std::enable_shared_from_this<ShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    auto shapeCreation = [](shape_parameter const& param) -> std::shared_ptr<volumeShape> {
      return std::shared_ptr<volumeShape>(new hexPrism(
          param.getParam<point3D>(0), param.getParam<vector3D>(0), param.getParam<vector3D>(1),
          param.getParam<vector3D>(2), param.getParam<int>(0)));
    };
    auto next1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      vector3D stDir = parameter.getParam<vector3D>(2) * (2 * times);
      auto result = parameter;
      result.setParam<point3D>(0, result.getParam<point3D>(0) + stDir);
      return result;
    };
    auto next2 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      // in this case we have a stacking in zig-zags:
      // each line is stacked along dir3,
      // in the first line there are stacki elements, in the second (stacki +1), etc.
      // the second line is shifted by 1.5 * vectorToFirstPoint
      // in line stacki the middle element is left out
      // from there on there the next lines have one element less each.
      auto result = parameter;
      auto center = parameter.getParam<point3D>(0);
      auto lineStack = parameter.getParam<vector3D>(2);
      auto nextLine = parameter.getParam<vector3D>(1);
      auto stacki = parameter.getParam<int>(0);

      auto total = static_cast<size_t>(3 * abs(stacki) * (abs(stacki) - 1) - 3);
      if (times > total) {
        return {};
      }
      size_t lineNumber{0u};
      auto itemInLine = calculateLine(lineNumber, static_cast<size_t>(abs(stacki)), times,
                                      static_cast<size_t>(abs(stacki)));
      auto centerOfLine = center + nextLine * lineNumber;

      if (static_cast<int>(lineNumber) < stacki - 2) {
        centerOfLine = centerOfLine - lineStack * lineNumber;
      } else if (static_cast<int>(lineNumber) == stacki - 1) {
        centerOfLine = centerOfLine - lineStack * lineNumber;
        if (static_cast<int>(itemInLine) >= stacki) {
          itemInLine++;
        }
      } else {
        centerOfLine =
            centerOfLine -
            lineStack * abs(static_cast<int>(abs(static_cast<int>(stacki)) * 2 - 2 - lineNumber));
      }
      result.setParam<point3D>(0,
                               centerOfLine + nextLine * lineNumber + lineStack * itemInLine * 2);
      return result;
    };
    auto next3 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      // in this case we have a stacking in zig-zags:
      // each line is stacked along dir3,
      // in the first line there are stacki elements, in the second (stacki +1), etc.
      // the second line is shifted by 1.5 * vectorToFirstPoint
      // in line stacki the middle element is not left out
      // from there on there the next lines have one element less each.
      auto result = parameter;
      auto center = parameter.getParam<point3D>(0);
      auto lineStack = parameter.getParam<vector3D>(2);
      auto nextLine = parameter.getParam<vector3D>(1);
      auto stacki = parameter.getParam<int>(0);

      auto total = static_cast<size_t>(3 * abs(stacki) * (abs(stacki) - 1) - 3);
      if (times > total) {
        return {};
      }
      size_t lineNumber{0u};
      auto itemInLine = calculateLine2(lineNumber, static_cast<size_t>(abs(stacki)), times,
                                       static_cast<size_t>(abs(stacki)));
      auto centerOfLine = center + nextLine * lineNumber;

      if (static_cast<int>(lineNumber) < stacki - 1) {
        centerOfLine = centerOfLine - lineStack * lineNumber;
      } else {
        centerOfLine =
            centerOfLine -
            lineStack * abs(static_cast<int>(abs(static_cast<int>(stacki)) * 2 - 2 - lineNumber));
      }
      result.setParam<point3D>(0,
                               centerOfLine + nextLine * lineNumber + lineStack * itemInLine * 2);
      return result;
    };
    auto envelope1 = [](shape_parameter const& parameter, size_t times) -> shape_parameter {
      auto result = parameter;
      result.setParam<vector3D>(2, times * parameter.getParam<vector3D>(2));
      return result;
    };
    auto envelope2 = [](shape_parameter const& parameter, size_t) -> shape_parameter {
      auto result = parameter;
      auto center = parameter.getParam<point3D>(0);
      auto lineStack = parameter.getParam<vector3D>(2);
      auto nextLine = parameter.getParam<vector3D>(1);
      auto stacki = parameter.getParam<int>(0);

      result.setParam<point3D>(0, center + 1.5 * stacki * nextLine + 1.5 * stacki * lineStack);
      result.setParam<vector3D>(1, 3 * abs(stacki) * nextLine);
      result.setParam<vector3D>(2, 3 * abs(stacki) * lineStack);
      return result;
    };

    auto description = hexPrism::getDescription();
    description.setId(hexPrism_id1);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next1, envelope1);
    description.setId(hexPrism_id2);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next2, envelope2);
    description.setId(hexPrism_id3);
    factory.addShapeToFactory(description, ShapeType::VolumeShape, shapeCreation, next3, envelope2);
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(hexPrism_id1);
    factory.removeShapeFromFactory(hexPrism_id2);
    factory.removeShapeFromFactory(hexPrism_id3);
  }

  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
};
volatile static std::shared_ptr<ShapeProvider> prov = [] {
  auto r = std::make_shared<ShapeProvider>();
  r->install();
  return r;
}();
} // namespace

hexPrism::hexPrism(point3D c, vector3D thickness, vector3D toFirstPoint, vector3D keyWidth, int st)
    : volumeShape("hexPrism")
{
  stacki = st;
  center = c;
  dir1 = thickness;
  dir2 = toFirstPoint;
  dir3 = keyWidth;
}

hexPrism::hexPrism(const volumeShape& s) : volumeShape("hexPrism")
{
  if (getName() != "hexPrism")
    return;
  center = ((hexPrism)s).getCenter();
  dir1 = ((hexPrism)s).getDirection(0);
  dir2 = ((hexPrism)s).getDirection(1);
  dir3 = ((hexPrism)s).getDirection(2);
}
hexPrism::hexPrism(const shape_parameter& description) : volumeShape("hexPrism")
{
  if (description.getName() != "hexPrism")
    return;
  if (description.NumberOfParams<point3D>() < 1 || description.NumberOfParams<int>() < 1 ||
      description.NumberOfParams<vector3D>() < 3)
    return;
  center = description.getParam<point3D>(0);
  dir1 = description.getParam<vector3D>(0);
  dir2 = description.getParam<vector3D>(1);
  dir3 = description.getParam<vector3D>(2);
  stacki = description.getParam<int>(0);
}

volumeShape* hexPrism::getNext(int times, int stackType)
{
  switch (stackType) {
  case 2: {
    vector3D stDir = dir3;
    stDir = stDir * (2 * times);
    return (new hexPrism(center + stDir, dir1, dir2, dir3, stacki));
  }
  case 3: {
    vector3D shiftUp = dir3 * (-1);
    vector3D shiftLeft = dir2 * 1.5;
    if (stacki > 0) {
      int lev = abs(stacki);
      int n = abs(stacki) + 1;
      int ntot = 0;
      while ((times >= ntot) && (lev >= 0)) {
        n = n + 1;
        ntot = ntot + n;
        lev--;
      }
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 (shiftUp + shiftLeft) * (-lev - stacki - 1),
                             dir1, dir2, dir3, stacki));
      while ((times >= ntot) && (lev < abs(stacki) - 1)) {
        n = n - 1;
        ntot = ntot + n;
        lev++;
      }
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki - 1) +
                                 shiftLeft * (abs(stacki) + lev + 1),
                             dir1, dir2, dir3, stacki));
      return getClone();
    } else {
      int lev = abs(stacki) - 1;
      int n = abs(stacki) + 1;
      int ntot = abs(stacki) - 1;
      if (times < ntot)
        return (new hexPrism(center + dir3 * (2 * (times)), dir1, dir2, dir3, stacki));
      while ((times >= ntot) && (lev >= 1)) {
        n = n + 1;
        ntot = ntot + n;
        lev--;
      }
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 (shiftUp + shiftLeft) * (-lev - stacki - 1) + shiftUp * 2,
                             dir1, dir2, dir3, stacki));
      ntot = ntot + n - 1;
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n - 1)) +
                                 (shiftUp + shiftLeft) * abs(stacki),
                             dir1, dir2, dir3, stacki));
      ntot = ntot + n;
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki + 1) +
                                 shiftLeft * (-lev + abs(stacki) + 1),
                             dir1, dir2, dir3, stacki));
      while ((times >= ntot) && (lev < abs(stacki) - 2)) {
        n = n - 1;
        ntot = ntot + n;
        lev++;
      }
      if ((times < ntot))
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki + 1) +
                                 shiftLeft * (abs(stacki) + lev + 1),
                             dir1, dir2, dir3, stacki));
      if (times < ntot + abs(stacki) - 1)
        return (new hexPrism(center + dir3 * (2 * (times - ntot + 1)) + shiftUp * (-lev - stacki) +
                                 shiftLeft * (abs(stacki) + lev + 2),
                             dir1, dir2, dir3, stacki));
      return getClone();
    }
  }
  case -3:
    if (stacki > 0) {
      int lev = abs(stacki);
      int n = abs(stacki) + 1;
      int ntot = 0;
      vector3D shiftUp = dir3 * (-1);
      vector3D shiftLeft = dir2 * 1.5;
      int sum = 0;
      for (int i = 0; i <= abs(stacki); i++)
        sum = sum + i;
      if ((sum * 6 - 6) / 2.0 <= times)
        times++;
      while ((times >= ntot) && (lev >= 0)) {
        n = n + 1;
        ntot = ntot + n;
        lev--;
      }
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 (shiftUp + shiftLeft) * (-lev - stacki - 1),
                             dir1, dir2, dir3, stacki));
      }
      while ((times >= ntot) && (lev < abs(stacki) - 1)) {
        n = n - 1;
        ntot = ntot + n;
        lev++;
      }
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki - 1) +
                                 shiftLeft * (abs(stacki) + lev + 1),
                             dir1, dir2, dir3, stacki));
      }
      return getClone();
    } else {
      int lev = abs(stacki) - 1;
      int n = abs(stacki) + 1;
      int ntot = abs(stacki) - 1;
      vector3D shiftUp = dir3 * (-1);
      vector3D shiftLeft = dir2 * 1.5;
      int sum = 0;
      for (int i = 0; i <= abs(stacki); i++)
        sum = sum + i;
      if ((sum * 6 - 6) / 2.0 <= times)
        times++;
      if (times < ntot) {
        return (new hexPrism(center + dir3 * (2 * (times)), dir1, dir2, dir3, stacki));
      }
      while ((times >= ntot) && (lev >= 1)) {
        n = n + 1;
        ntot = ntot + n;
        lev--;
      }
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 (shiftUp + shiftLeft) * (-lev - stacki - 1) + shiftUp * 2,
                             dir1, dir2, dir3, stacki));
      }
      ntot = ntot + n - 1;
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n - 1)) +
                                 (shiftUp + shiftLeft) * abs(stacki),
                             dir1, dir2, dir3, stacki));
      }
      ntot = ntot + n;
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki + 1) +
                                 shiftLeft * (-lev + abs(stacki) + 1),
                             dir1, dir2, dir3, stacki));
      }
      while ((times >= ntot) && (lev < abs(stacki) - 2)) {
        n = n - 1;
        ntot = ntot + n;
        lev++;
      }
      if ((times < ntot)) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + n)) +
                                 shiftUp * (-lev - stacki + 1) +
                                 shiftLeft * (abs(stacki) + lev + 1),
                             dir1, dir2, dir3, stacki));
      }
      if (times < ntot + abs(stacki) - 1) {
        return (new hexPrism(center + dir3 * (2 * (times - ntot + 1)) + shiftUp * (-lev - stacki) +
                                 shiftLeft * (abs(stacki) + lev + 2),
                             dir1, dir2, dir3, stacki));
      }
      return getClone();
    }
  }
  return getClone();
}

volumeShape* hexPrism::getEnvelope(int times, int stackType)
{
  switch (stackType) {
  case 2: {
    vector3D stDir = dir3;
    stDir = stDir * (2 * times);
    return (new hexPrism(center, dir1 * times, dir2, dir3, stacki));
  }
  case 3: {
    vector3D shiftUp = dir3 * (-1);
    vector3D shiftLeft = dir2 * 1.5;
    hexPrism* ret = new hexPrism(*this);
    int ne = 1;
    int le = 1;
    while (ne < times && le <= abs(stacki))
      ne = ne + (le++) * 6;
    ret->setDirection(1, shiftLeft * le);
    ret->setDirection(2, shiftUp * le);
    return ret;
  }
  case -3: {
    vector3D shiftUp = dir3 * (-1);
    vector3D shiftLeft = dir2 * 1.5;
    hexPrism* ret = new hexPrism(*this);
    int ne = 0;
    int le = 1;
    while (ne < times && le <= abs(stacki))
      ne = ne + (le++) * 6;
    ret->setDirection(1, shiftLeft * le);
    ret->setDirection(2, shiftUp * le);
    return ret;
  }
  }
  return getClone();
}
float hexPrism::getFlightPathInShape(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  if (dist.R() > __prec)
    return 0;
  Vector hit2(Hitting(sLine3D(line.Foot(), line.Direction() * -1)));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  point3D out(hit2.getValue(0), hit2.getValue(1), hit2.getValue(2));
  return (out - enter).R();
}

point3D hexPrism::entrance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return enter;
  return point3D();
}

vector3D hexPrism::distance(const sLine3D& line)
{
  Vector hit(Hitting(line));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  return dist;
}

sLine3D hexPrism::Normal(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  if (norm.getState() != _zero_)
    return sLine3D(enter, norm);
  return sLine3D();
}
#include "fiber.h"

bool hexPrism::cut(const fiber& f)
{
  if (f.getCorner() == center)
    return true;
  return false;
}

Vector hexPrism::HitParams(const sLine3D& line)
{
  Vector hit(Hitting(line));
  point3D enter(hit.getValue(0), hit.getValue(1), hit.getValue(2));
  vector3D dist(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  vector3D norm(hit.getValue(6), hit.getValue(7), hit.getValue(8));
  vector3D pitch;
  vector3D npc;
  vector3D dist_;
  float sigma = hit.getValue(9);
  npc = enter + dist;
  pitch = (sLine3D(center, dir1) - enter);
  if (pitch.length() > 0)
    pitch.normalize();
  else
    pitch = vector3D(0, 0, 1);

  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, enter.X());
  ret.setValue(2, enter.Y());
  ret.setValue(3, enter.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist_.X());
  ret.setValue(11, dist_.Y());
  ret.setValue(12, dist_.Z());
  return ret;
}
hexPrism::~hexPrism() {}

vector3D hexPrism::getDirection(int num) const
{
  switch (num) {
  case 0:
    return dir1;
  case 1:
    return dir2;
  case 2:
    return dir3;
  }
  return vector3D();
}
void hexPrism::setDirection(int num, vector3D v)
{
  switch (num) {
  case 0:
    dir1 = v;
    return;
  case 1:
    dir2 = v;
    return;
  case 2:
    dir3 = v;
    return;
  }
}
volumeShape* hexPrism::getClone()
{
  volumeShape* sh = new hexPrism(*this);
  return sh;
}
int hexPrism::getStackingOrder() const { return stacki; }
void hexPrism::setStackingOrder(int st) { stacki = st; }

Vector hexPrism::Hitting(const sLine3D& line)
{
  Vector ret(10);
  // Hit with 4 planes only with planes that can have an entrance =>
  // normal_to_plane*line_direction<0
  plane3D p1;
  plane3D p2;
  plane3D p3;
  plane3D p4;
  point3D c1, c2, c3, c4; // corner of rectangles in each plane
  float width = dir3.length(), thickness = dir1.length();
  float sigma1 = width * 0.5, sigma2 = sigma1, sigma3 = sigma1, sigma4 = width;
  vector3D d1, d2, d3, d4;
  d4 = dir1 * (-1);
  d4.normalize();
  d1 = matrix3D(d4, 2 * M_PI / 3) * dir3;
  d1.normalize();
  d2 = matrix3D(d4, M_PI / 3) * dir3;
  d2.normalize();
  d3 = dir3;
  d3.normalize();
  int outcode1, outcode2, outcode3;
  vector3D v1_1, v1_2, v2_1, v2_2, v3_1, v3_2; // components of hit1-corner1 in each fiber direction
  float d11, d12, d21, d22, d31, d32;          // length of these components
  float dr11, dr12, dr21, dr22, dr31, dr32;    // direction of components >0 ->point inwards
  // calculate planes
  // first plane
  if (line.Direction() * d1 < 0) {
    p1 = plane3D(center + dir2, d1);
    c1 = center + dir2;
  } else {
    p1 = plane3D(center - dir2, d1 * (-1));
    c1 = center - dir2;
  }
  // second plane
  if (line.Direction() * d2 < 0) {
    p2 = plane3D(center + matrix3D(d4, M_PI / 3) * dir2, d2);
    c2 = center + matrix3D(d4, M_PI / 3) * dir2;
  } else {
    p2 = plane3D(center - matrix3D(d4, M_PI / 3) * dir2, d2 * (-1));
    c2 = center - matrix3D(d4, M_PI / 3) * dir2;
  }
  // third plane
  if (line.Direction() * d3 < 0) {
    p3 = plane3D(center + matrix3D(d4, 2 * M_PI / 3) * dir2, d3);
    c3 = center + matrix3D(d4, 2 * M_PI / 3) * dir2;
  } else {
    p3 = plane3D(center - matrix3D(d4, 2 * M_PI / 3) * dir2, d3 * (-1));
    c3 = center - matrix3D(d4, 2 * M_PI / 3) * dir2;
  }
  // fourth plane
  if (line.Direction() * d4 < 0) {
    p4 = plane3D(center, d4);
    c4 = center;
  } else {
    p4 = plane3D(center + dir1, d4);
    c4 = center + dir1;
  }
  vector3D dh1, dh2, dh3;
  // calculate hit variables
  // first plane
  point3D hit1 = p1 - line;
  v1_1 = hit1 - c1;
  v1_2 = v1_1;
  dr11 = v1_1 * d4 * (-1);
  dh1 = d4 ^ p1.Normal();
  dh1.normalize();
  dr12 = v1_2 * dh1 * (-1);
  v1_1 = (-dr11) * d4;
  v1_2 = (dr12)*dh1;
  d11 = v1_1.length();
  d12 = v1_2.length();
  outcode1 = (dr11 < 0) * 1 + (dr12 < 0) * 4 + (d11 > thickness) * 2 + (d12 > width) * 8;
  // second plane
  point3D hit2 = p2 - line;
  v2_1 = hit2 - c2;
  v2_2 = v2_1;
  dh2 = d4 ^ p2.Normal();
  dh2.normalize();
  dr21 = v2_1 * d4 * (-1);
  dr22 = v2_2 * dh2;
  v2_1 = (-dr21) * d4;
  v2_2 = (dr21)*dh2;
  d21 = v2_1.length();
  d22 = v2_2.length();
  outcode2 = (dr21 < 0) * 1 + (dr22 < 0) * 4 + (d21 > thickness) * 2 + (d22 > width) * 8;
  // third plane
  point3D hit3 = p3 - line;
  v3_1 = hit3 - c2;
  v3_2 = v3_1;
  dh3 = d4 ^ p3.Normal();
  dh3.normalize();
  dr31 = v3_1 * d4 * (-1);
  dr32 = v3_2 * dh3;
  v3_1 = (-dr31) * d4;
  v3_2 = (dr32)*dh3;
  d31 = v3_1.length();
  d32 = v3_2.length();
  outcode3 = (dr31 < 0) * 1 + (dr32 < 0) * 4 + (d31 > thickness) * 2 + (d32 > width) * 8;
  // fourth plane
  point3D hit4 = p4 - line;
  // check planes
  // first plane
  if (outcode1 == 0) {
    ret.setValues(hit1.X(), hit1.Y(), hit1.Z(), 0, 0, 0, p1.Normal().X(), p1.Normal().Y(),
                  p1.Normal().Z(), sigma1);
    return ret;
  }
  // second plane
  if (outcode2 == 0) {
    ret.setValues(hit2.X(), hit2.Y(), hit2.Z(), 0, 0, 0, p2.Normal().X(), p2.Normal().Y(),
                  p2.Normal().Z(), sigma2);
    return ret;
  }
  // third plane
  if (outcode3 == 0) {
    ret.setValues(hit3.X(), hit3.Y(), hit3.Z(), 0, 0, 0, p3.Normal().X(), p3.Normal().Y(),
                  p3.Normal().Z(), sigma3);
    return ret;
  }
  // front/back plane
  vector3D toce = hit4 - c4;
  if (toce.length() <= dir3.length()) {
    ret.setValues(hit4.X(), hit4.Y(), hit4.Z(), 0, 0, 0, p4.Normal().X(), p4.Normal().Y(),
                  p4.Normal().Z(), sigma4);
    return ret;
  }
  int i = 0;
  while (acos(toce * dir3) > M_PI / 6.0) {
    i++;
    toce = matrix3D(d4, M_PI / 3.0) * toce;
  }
  sLine3D sl(c4 + matrix3D(d4, i * M_PI / 3.0) * dir2,
             matrix3D(d4, i * M_PI / 3.0 + M_PI / 2) * dir2);
  vector3D toedge = sl - toce;
  if (toedge * toce < 0) {
    ret.setValues(hit4.X(), hit4.Y(), hit4.Z(), 0, 0, 0, p4.Normal().X(), p4.Normal().Y(),
                  p4.Normal().Z(), sigma4);
    return ret;
  }
  // no hit point calculate distances
  vector3D dist1, dist2, dist3, dist4;
  // fourth plane
  dist4 = matrix3D(d4, -(i * M_PI / 3.0)) * toedge;
  // first plane
  switch (outcode1) {
  case 5:
    dist1 = hit1 - c1;
    break;
  case 6:
    dist1 = hit1 - (c1 + dir1);
    break;
  case 9:
    dist1 = hit1 - (c1 + dh1 * thickness);
    break;
  case 10:
    dist1 = hit1 - (c1 + dir1 + dh1 * thickness);
    break;
  case 4:
    dist1 = lLine3D(c1, dir1) - hit1;
    break;
  case 1:
    dist1 = lLine3D(c1, dh1 * thickness) - hit1;
    break;
  case 2:
    dist1 = lLine3D(c1 + dir1, dh1 * thickness) - hit1;
    break;
  case 8:
    dist1 = lLine3D(c1 + dh1 * thickness, dir1) - hit1;
    break;
  }
  // second plane
  switch (outcode2) {
  case 5:
    dist2 = hit2 - c2;
    break;
  case 6:
    dist2 = hit2 - (c2 + dir1);
    break;
  case 9:
    dist2 = hit2 - (c2 + dh2 * thickness);
    break;
  case 10:
    dist2 = hit2 - (c2 + dir1 + dh2 * thickness);
    break;
  case 4:
    dist2 = lLine3D(c2, dir1) - hit2;
    break;
  case 1:
    dist2 = lLine3D(c2, dh2 * thickness) - hit2;
    break;
  case 2:
    dist2 = lLine3D(c2 + dir1, dh2 * thickness) - hit2;
    break;
  case 8:
    dist2 = lLine3D(c2 + dh2 * thickness, dir1) - hit2;
    break;
  }
  // third plane
  switch (outcode3) {
  case 5:
    dist3 = hit3 - c3;
    break;
  case 6:
    dist3 = hit3 - (c3 + dir1);
    break;
  case 9:
    dist3 = hit3 - (c3 + dh3 * thickness);
    break;
  case 10:
    dist3 = hit3 - (c3 + dir1 + dh3 * thickness);
    break;
  case 4:
    dist3 = lLine3D(c3, dir1) - hit3;
    break;
  case 1:
    dist3 = lLine3D(c3, dh3 * thickness) - hit3;
    break;
  case 2:
    dist3 = lLine3D(c3 + dir1, dh3 * thickness) - hit3;
    break;
  case 8:
    dist3 = lLine3D(c3 + dh3 * thickness, dir1) - hit3;
    break;
  }
  if ((dist1.length() <= dist2.length()) && (dist1.length() <= dist3.length()) &&
      (dist1.length() <= dist4.length())) {
    ret.setValues(hit1.X(), hit1.Y(), hit1.Z(), dist1.X(), dist1.Y(), dist1.Z(), 0, 0, 0, sigma1);
    return ret;
  }
  if (dist2.length() <= dist3.length() && dist2.length() <= dist4.length()) {
    ret.setValues(hit2.X(), hit2.Y(), hit2.Z(), dist2.X(), dist2.Y(), dist2.Z(), 0, 0, 0, sigma2);
    return ret;
  }
  if (dist3.length() <= dist4.length()) {
    ret.setValues(hit3.X(), hit3.Y(), hit3.Z(), dist3.X(), dist3.Y(), dist3.Z(), 0, 0, 0, sigma3);
    return ret;
  }
  ret.setValues(hit4.X(), hit4.Y(), hit4.Z(), dist4.X(), dist4.Y(), dist4.Z(), 0, 0, 0, sigma4);
  return ret;
}

Vector hexPrism::HitParams(const planeShape& shape, point3D origin)
{
  Vector hit(Hitting(sLine3D(origin, shape.getCenter() - origin)));
  point3D coord;
  vector3D pitch;
  vector3D npc;
  vector3D dist_(hit.getValue(3), hit.getValue(4), hit.getValue(5));
  float sigma = hit.getValue(9);

  if (dist_.length() < __prec) {
    coord.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    npc = coord + dist_;
  } else {
    float dp = shape.angularRange_Phi(origin);
    float dt = shape.angularRange_Theta(origin);
    vector3D e(hit.getValue(0), hit.getValue(1), hit.getValue(2));
    vector3D n = e + dist_;
    e = e - origin;
    n = n - origin;
    float dp1 = abs(e.Phi() - n.Phi()), dt1 = abs(e.Theta() - n.Theta());
    if (dp1 < dp && dt1 < dt) {
      int numCorners = shape.getNumberOfPoints();
      point3D corners[numCorners];
      vector3D dists[numCorners];
      point3D npcs[numCorners];
      point3D enters[numCorners];
      bool init = false;
      for (int i = 0; i < numCorners; i++) {
        corners[i] = shape.getPoint(i);
      }
      for (int i = 0; i < numCorners; i++) {
        hit = Vector(Hitting(sLine3D(origin, corners[i] - origin)));
        dists[i].setValues(hit.getValue(3), hit.getValue(4), hit.getValue(5));
        enters[i].setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
        npcs[i] = enters[i] + dists[i];
        if (dists[i].length() < __prec) {
          coord = enters[i];
          npc = npcs[i];
          dist_ = dists[i];
          init = true;
          break;
        }
      }
      if (!init) {
        bool passing = false;
        for (int i = 0; i < numCorners; i++)
          for (int j = 0; j < numCorners; j++) {
            if (dists[i] * dists[j] < 0)
              passing = true;
          }
        for (int i = 0; i < numCorners; i++) {
          if (dist_.length() > dists[i].length()) {
            dist_ = dists[i];
            coord = enters[i];
            npc = npcs[i];
          }
        }
        if (passing) {
          dist_.setValues(0, 0, 0);
        }
      }
    } else {
      coord.setValues(hit.getValue(0), hit.getValue(1), hit.getValue(2));
      npc = coord + dist_;
    }
  }
  pitch = sLine3D(center, dir1) - coord;
  if (pitch.length() > 0)
    pitch.normalize();
  else
    pitch = vector3D(0, 0, 1);

  Vector ret(13);
  ret.setValue(0, sigma);
  ret.setValue(1, coord.X());
  ret.setValue(2, coord.Y());
  ret.setValue(3, coord.Z());
  ret.setValue(4, pitch.X());
  ret.setValue(5, pitch.Y());
  ret.setValue(6, pitch.Z());
  ret.setValue(7, npc.X());
  ret.setValue(8, npc.Y());
  ret.setValue(9, npc.Z());
  ret.setValue(10, dist_.X());
  ret.setValue(11, dist_.Y());
  ret.setValue(12, dist_.Z());
  return ret;
}
#include "planeShape.h"
#include "quadrangle.h"
void hexPrism::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                    TObject** ident, int lColor, int fColor, int fStyle) const
{
  vector3D viewDir = plane.Foot() - eye;
  quadrangle s[3];
  hexagon h1, h2;
  h1.setPoints(center, center + dir2, dir1 * -1);
  h2.setPoints(center + dir1, center + dir1 + dir2, dir1);
  vector3D v[3];
  v[0] = dir3 * -1;
  matrix3D turn(dir1, M_PI / 3.);
  v[1] = turn * v[0];
  v[2] = turn * v[1];
  vector4D b[3];
  float d;
  vector4D bh;
  if (viewDir * dir1 < 0)
    h2.Draw(eye, plane, &bh, ident, lColor, fColor, fStyle);
  else
    h1.Draw(eye, plane, &bh, ident, lColor, fColor, fStyle);
  if (boundingBox != NULL) {
    *boundingBox = bh;
  }
  for (int i = 0; i < 3; i++) {
    d = v[i] * plane.Normal();
    if (abs(d) < __prec)
      continue;
    else if (d > 0) {
      s[i].setA(h1.getPoint(i + 1));
      s[i].setB(h1.getPoint(i + 2));
      s[i].setC(h2.getPoint(6 - i - 2));
      s[i].setD(h2.getPoint(6 - i - 1));
    } else {
      s[i].setA(h1.getPoint(6 - i - 2));
      s[i].setB(h1.getPoint(6 - i - 1));
      s[i].setC(h2.getPoint(i + 1));
      s[i].setD(h2.getPoint(i + 2));
    }
    s[i].Draw(eye, plane, &b[i], lColor, fColor, fStyle);
    if (boundingBox != NULL) {
      if (boundingBox->getValue(0) > b[i].getValue(0))
        boundingBox->setValue(0, b[i].getValue(0));
      if (boundingBox->getValue(1) > b[i].getValue(1))
        boundingBox->setValue(1, b[i].getValue(1));
      if (boundingBox->getValue(2) < b[i].getValue(2))
        boundingBox->setValue(2, b[i].getValue(2));
      if (boundingBox->getValue(3) < b[i].getValue(3))
        boundingBox->setValue(3, b[i].getValue(3));
    }
  }
}
void hexPrism::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
int hexPrism::suspect(const sLine3D& line, int stackType)
{
  switch (stackType) {
  case 2: {
    point3D c;
    if (dir1 * line.Direction() < 0)
      c = center;
    else
      c = center + dir1;
    plane3D pl(c, dir1);
    point3D hit = pl - line;
    vector3D l = hit - c;
    vector3D l_l = (l * dir3 / dir3.R()) * dir3 / dir3.R();
    vector3D l_t = l - l_l;
    if (l_t.R() > dir2.R())
      return -1;
    if (l_l * dir3 < 0) {
      if (l_l.R() < dir2.R())
        return 0;
      return -1;
    }
    return ((int)((l_l.R() + dir2.R()) / (dir2.R() * 2)));
  }
  case -3:
  case 3: {
    float nUtC, nLtC;
    int nE = 1;
    for (int i = 0; i < abs(stacki); i++)
      nE += 6 * (i + 1);
    nUtC = abs(stacki) / 2.;
    nLtC = abs(stacki) * 1.5;
    if (stacki < 0) {
      nE = nE - 6;
      nUtC = nUtC - 1;
    }
    if (stackType < 0)
      nE = nE - 1;
    point3D c;
    if (dir1 * line.Direction() < 0)
      c = center;
    else
      c = center + dir1;
    c = c - nUtC * dir3;
    c = c + nLtC * dir2;
    plane3D pl(c, dir1);
    point3D hit = pl - line;
    vector3D l = hit - c;
    if (l.R() > (abs(stacki) * 2 + 2) * dir2.R())
      return -1;
    vector3D v = (l * dir3 / dir3.R() / dir3.R()) * dir3;
    float nU = (v.R() / dir3.R()) / 2., nL = ((l - v).R() / dir2.R()) / 1.5;

    int centerN;
    if (stackType < 0)
      centerN = nE / 2;
    else
      centerN = (nE - 1) / 2;
    //	int nR=abs(stacki)*2;
    //	int nn=centerN;
    if (abs(nL) < 0.5) {
      if (stackType < 0 && nL > 0) {
        return ((int)(centerN - ((int)nU + 0.5)));
      }
      return ((int)(centerN - ((int)nU) - 1));
    }
    if (nU > 0) {
    }
  }
  }
  return -1;
}
shape_parameter hexPrism::description() const
{
  shape_parameter sh;
  sh.setName("hexPrism");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(center, "center of front");
  sh.addParam<vector3D>(dir1, "thickness vector");
  sh.addParam<vector3D>(dir2, "vector to first point");
  sh.addParam<vector3D>(dir3, "key width vector");
  sh.addParam<int>(stacki, "stack parameter");
  return sh;
}
shape_parameter hexPrism::getDescription()
{
  shape_parameter sh;
  sh.setName("hexPrism");
  sh.setCompleteWrite(true);
  sh.setId(hexPrism_id1);
  sh.addParam<point3D>(point3D(), "center of front");
  sh.addParam<vector3D>(vector3D(), "thickness vector");
  sh.addParam<vector3D>(vector3D(), "vector to first point");
  sh.addParam<vector3D>(vector3D(), "key width vector");
  sh.addParam<int>(0, "stack parameter");
  return sh;
}
