#include "materialparameter.h"
element_parameter::element_parameter(const element_parameter& e) : base_parameter()
{
  setName(e.getName());
  mass = e.getMass();
  charge = e.getCharge();
  weight = e.getWeight();
}

element_parameter::element_parameter(float m, float c, float w, string n)
{
  setName(n);
  mass = m;
  charge = c;
  weight = w;
}
element_parameter& element_parameter::operator=(const element_parameter& e)
{
  setName(e.getName());
  mass = e.getMass();
  charge = e.getCharge();
  weight = e.getWeight();
  return *this;
}
bool element_parameter::operator==(const element_parameter& e) const
{
  if (e.getMass() != mass)
    return false;
  if (e.getCharge() != charge)
    return false;
  if (e.getName() != getName())
    return false;
  return true;
}

element_parameter::~element_parameter() {}

float element_parameter::getMass() const { return mass; }

float element_parameter::getCharge() const { return charge; }

float element_parameter::getWeight() const { return weight; }

void element_parameter::setMass(float m) { mass = m; }

void element_parameter::setCharge(float c) { charge = c; }

void element_parameter::setWeight(float w) { weight = w; }

ostream& operator<<(ostream& o, const element_parameter& ep)
{
  if ((&o == &cout) || (&o == &cerr)) {
    o << ep.getName().data() << ": mass: " << ep.getMass() << ", charge: " << ep.getCharge()
      << " , n-density: " << ep.getWeight();
  } else {
    o << ep.getMass() << " " << ep.getCharge() << " " << ep.getWeight() << " "
      << ep.getName().data();
  }
  return o;
}

material_parameter::material_parameter() {}

material_parameter::material_parameter(const material_parameter& m) : base_parameter()
{
  setName(m.getName());
  setDescription(m.getDescription());
  active = m.IsActive();
  density = m.Density();
  speed = m.Speed();
  radiationLength = m.RadiationLength();
  for (int i = 0; i < m.NumberOfElements(); i++)
    elements.push_back(m.Element(i));
  ;
  for (int i = 0; i < m.NumberOfEnergyLossParams(); i++)
    energyloss.push_back(m.EnergyLoss(i));
}
material_parameter& material_parameter::operator=(const material_parameter& m)
{
  setName(m.getName());
  setDescription(m.getDescription());
  active = m.IsActive();
  density = m.Density();
  speed = m.Speed();
  ClearEnergyLoss();
  radiationLength = m.RadiationLength();
  while (!elements.empty())
    elements.pop_back();
  for (int i = 0; i < m.NumberOfElements(); i++)
    elements.push_back(m.Element(i));
  for (int i = 0; i < m.NumberOfEnergyLossParams(); i++)
    energyloss.push_back(m.EnergyLoss(i));
  return *this;
}

material_parameter::~material_parameter() {}

bool material_parameter::IsActive() const { return active; }

float material_parameter::EnergyLoss(int num) const
{
  if ((num > (int)energyloss.size()) || (num < 0))
    return -1;
  return energyloss[num];
}

float material_parameter::Density() const { return density; }

float material_parameter::RadiationLength() const { return radiationLength; }

element_parameter material_parameter::Element(int num) const
{
  if ((num < 0) || (num >= (int)elements.size()))
    return element_parameter();
  return elements[num];
}
void material_parameter::setElement(int pos, const element_parameter& ep)
{
  if (pos >= (int)elements.size() || pos < 0)
    return;
  elements[pos].setName(ep.getName());
  elements[pos].setMass(ep.getMass());
  elements[pos].setCharge(ep.getCharge());
  elements[pos].setWeight(ep.getWeight());
}

void material_parameter::addElement(const element_parameter& ep) { elements.push_back(ep); }

int material_parameter::NumberOfElements() const { return elements.size(); }

void material_parameter::deleteElement(int num)
{
  if ((num < 0) || (num >= (int)elements.size()))
    return;
  if (num == (int)elements.size() - 1) {
    elements.pop_back();
    return;
  }
  vector<element_parameter> tmp;
  while ((int)elements.size() >= num) {
    tmp.push_back(elements.back());
    elements.pop_back();
  }
  elements.pop_back();
  while (!(tmp.empty())) {
    elements.push_back(tmp.back());
    tmp.pop_back();
  }
}

void material_parameter::setRadiationLength(float value) { radiationLength = value; }

int material_parameter::NumberOfEnergyLossParams() const { return energyloss.size(); }

void material_parameter::setActive(bool is) { active = is; }

void material_parameter::setEnergyLoss(int num, float value)
{
  if (num == (int)energyloss.size())
    energyloss.push_back(value);
  else
    energyloss.at(num) = value;
}

void material_parameter::setDensity(float value) { density = value; }
void material_parameter::ClearEnergyLoss()
{
  while (!energyloss.empty())
    energyloss.pop_back();
}
float material_parameter::Speed() const { return speed; }
void material_parameter::setSpeed(float val) { speed = val; }
bool material_parameter::operator<(const material_parameter& p)
{
  return (getName() < p.getName());
}
bool material_parameter::operator>(const material_parameter& p)
{
  return (getName() > p.getName());
}

bool material_parameter::operator==(const material_parameter& p) const
{
  return (getName() == p.getName());
}

bool material_parameter::operator<=(const material_parameter& p)
{
  return (getName() <= p.getName());
}

bool material_parameter::operator>=(const material_parameter& p)
{
  return (getName() >= p.getName());
}

istream& operator>>(istream& i, material_parameter& m)
{
  int zahli;
  float zahlf;
  char c;
  char li[100];
  i >> zahli;
  m.setActive(zahli == 1);
  i >> zahlf;
  m.setDensity(zahlf);
  i >> zahlf;
  m.setRadiationLength(zahlf);
  i >> zahlf;
  m.setSpeed(zahlf);
  i.get(c);
  i.getline(li, 100);
  m.setName(string(li));
  i >> zahli;
  m.ClearEnergyLoss();
  for (int j = 0; j < zahli; j++) {
    i >> zahlf;
    m.setEnergyLoss(j, zahlf);
  }
  i >> zahli;
  for (int I = 0; I < zahli; I++) {
    element_parameter ep;
    i >> zahlf;
    ep.setMass(zahlf);
    i >> zahlf;
    ep.setCharge(zahlf);
    i >> zahlf;
    ep.setWeight(zahlf);
    i.get(c);
    i.getline(li, 100);
    ep.setName(string(li));
    m.addElement(ep);
  }
  //  cout<<m<<endl;
  //    i.getline(li,100);
  return i;
}

ostream& operator<<(ostream& o, const material_parameter& m)
{
  if ((&o == &cout) || (&o == &cerr)) {
    o << m.getName().data() << endl;
    o << "density: " << m.Density() << ", radiation length: " << m.RadiationLength()
      << ", speed of Light in Medium " << m.Speed() << ", "
      << ((m.IsActive()) ? "active medium" : "detecting medium") << endl;
    for (int i = 0; i < m.NumberOfElements(); i++)
      o << m.Element(i) << endl;
    o << "energyloss: ";
    for (int i = 0; i < m.NumberOfEnergyLossParams() - 1; i++)
      o << m.EnergyLoss(i) << ", ";
    o << m.EnergyLoss(m.NumberOfEnergyLossParams() - 1) << endl;
  } else {
    o << (m.IsActive() ? 1 : 0) << " ";
    o << m.Density() << " " << m.RadiationLength() << " " << m.Speed() << " ";
    o << m.getName().data() << endl;
    o << m.NumberOfEnergyLossParams() << " ";
    for (int j = 0; j < m.NumberOfEnergyLossParams(); j++) {
      o << m.EnergyLoss(j) << " ";
    }
    o << m.NumberOfElements() << " ";
    for (int j = 0; j < m.NumberOfElements(); j++)
      o << m.Element(j).getMass() << " " << m.Element(j).getCharge() << " "
        << m.Element(j).getWeight() << " " << m.Element(j).getName().data() << endl;
  }
  // o<<"\n";
  return o;
}
