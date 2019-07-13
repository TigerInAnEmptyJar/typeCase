#include "paintparameter.h"
paint_parameter::paint_parameter(const string& nameIn, int IDIn, int colorIn, bool toPaint,
                                 int markerIn)
    : base_parameter(), ID(IDIn), color(colorIn), marker(markerIn), painted(toPaint)
{
  setName(nameIn);
}
paint_parameter::paint_parameter(const paint_parameter& p)
    : base_parameter(), ID(p.getID()), color(p.getColor()), marker(p.getMarker()),
      painted(p.isToPaint())
{
  setName(p.getName());
}
paint_parameter& paint_parameter::operator=(const paint_parameter& p)
{
  setName(p.getName());
  ID = p.getID();
  color = p.getColor();
  marker = p.getMarker();
  painted = p.isToPaint();
  return *this;
}

paint_parameter::~paint_parameter() {}

int paint_parameter::getID() const { return ID; }

int paint_parameter::getMarker() const { return marker; }

int paint_parameter::getColor() const { return color; }

void paint_parameter::setID(int IDIn) { ID = IDIn; }

void paint_parameter::setColor(int colorIn) { color = colorIn; }

void paint_parameter::setMarker(int markerIn) { marker = markerIn; }

bool paint_parameter::isToPaint() const { return painted; }

void paint_parameter::setToPaint(bool to) { painted = to; }

ostream& operator<<(ostream& o, const paint_parameter& p)
{
  o << p.getID() << " " << p.getColor() << " " << (p.isToPaint() ? "1" : "0") << " "
    << p.getMarker() << " " << p.getName().data() << endl;
  return o;
}

istream& operator>>(istream& i, paint_parameter& p)
{
  int number;
  i >> number;
  p.setID(number);
  i >> number;
  p.setColor(number);
  i >> number;
  p.setToPaint(number == 1);
  i >> number;
  p.setMarker(number);
  char li[200];
  i.getline(li, 200);
  string s = li;
  int b = 0;
  while (s[b] == ' ')
    b++;
  s = s.substr(b, s.length() - b);
  p.setName(s);
  return i;
}
