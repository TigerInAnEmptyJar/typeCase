#include "Eparticles.h"
#include "string_utilities.h"
void Eparticles::init()
{
  isInit = true;
  if (masses)
    delete[] masses;
  if (charges)
    delete[] charges;
  if (lifeTimes)
    delete[] lifeTimes;
  if (names)
    delete[] names;
  if (GeantIDs)
    delete[] GeantIDs;
  numberOfParticles = 38;
  masses = new float[numberOfParticles];
  charges = new float[numberOfParticles];
  names = new string[numberOfParticles];
  GeantIDs = new int[numberOfParticles];
  lifeTimes = new float[numberOfParticles];
  setParticle(0, "GAMMA", .0000E+00, 0., .10000E+16, 1);
  setParticle(1, "POSITRON", .51099890E-03, 1., .10000E+16, 2);
  setParticle(2, "ELECTRON", .51099890E-03, -1., .10000E+16, 3);
  setParticle(3, "NEUTRINO", .0000E+00, 0., .10000E+16, 4);
  setParticle(4, "MUON +", .105658357E+00, 1., .21970E-05, 5);
  setParticle(5, "MUON -", .105658357E+00, -1., .21970E-05, 6);
  setParticle(6, "PION 0", .1349766E+00, 0., .84000E-16, 7);
  setParticle(7, "PION +", .13957018E+00, 1., .26030E-07, 8);
  setParticle(8, "PION -", .13957018E+00, -1., .26030E-07, 9);
  setParticle(9, "KAON 0 LONG", .497672E+00, 0., .51700E-07, 10);
  setParticle(10, "KAON +", .493677E+00, 1., .12386E-07, 11);
  setParticle(11, "KAON -", .493677E+00, -1., .12386E-07, 12);
  setParticle(12, "NEUTRON", .93956533E+00, 0., .88700E+03, 13);
  setParticle(13, "PROTON", .938272E+00, 1., .10000E+16, 14);
  setParticle(14, "ANTIPROTON", .938272E+00, -1., .10000E+16, 15);
  setParticle(15, "KAON 0 SHORT", .497672E+00, 0., .89260E-10, 16);
  setParticle(16, "ETA", .5473E+00, 0., .54850E-18, 17);
  setParticle(17, "LAMBDA", .1115683E+01, 0., .26320E-09, 18);
  setParticle(18, "SIGMA +", .118937E+01, 1., .79900E-10, 19);
  setParticle(19, "SIGMA 0", .1192642E+01, 0., .74000E-19, 20);
  setParticle(20, "SIGMA -", .1197449E+01, -1., .14790E-09, 21);
  setParticle(21, "XI 0", .131483E+01, 0., .29000E-09, 22);
  setParticle(22, "XI -", .132131E+01, -1., .16390E-09, 23);
  setParticle(23, "OMEGA -", .167245E+01, -1., .82200E-10, 24);
  setParticle(24, "ANTINEUTRON", .93956533E+00, 0., .88700E+03, 25);
  setParticle(25, "ANTILAMBDA", .1115683E+01, 0., .26320E-09, 26);
  setParticle(26, "ANTISIGMA -", .118937E+01, -1., .79900E-10, 27);
  setParticle(27, "ANTISIGMA 0", .1193E+01, 0., .74000E-19, 28);
  setParticle(28, "ANTISIGMA +", .1197E+01, 1., .14790E-09, 29);
  setParticle(29, "ANTIXI 0", .1315E+01, 0., .29000E-09, 30);
  setParticle(30, "ANTIXI +", .1321E+01, 1., .16390E-09, 31);
  setParticle(31, "ANTIOMEGA +", .1672E+01, 1., .82200E-10, 32);
  setParticle(32, "DEUTERON", .1876E+01, 1., .10000E+16, 45);
  setParticle(33, "TRITON", .2809E+01, 1., .10000E+16, 46);
  setParticle(34, "ALPHA", .3727E+01, 2., .10000E+16, 47);
  setParticle(35, "GEANTINO", .0000E+00, 0., .10000E+16, 48);
  setParticle(36, "HE3", .2809E+01, 2., .10000E+16, 49);
  setParticle(37, "Cerenkov", .0000E+00, 0., .10000E+16, 50);
}
#include <fstream>
void Eparticles::init(string filename)
{
  isInit = true;
  ifstream input;
  input.open(filename.data());
  input >> numberOfParticles;
  if (masses)
    delete[] masses;
  if (charges)
    delete[] charges;
  if (lifeTimes)
    delete[] lifeTimes;
  if (names)
    delete[] names;
  if (GeantIDs)
    delete[] GeantIDs;
  masses = new float[numberOfParticles];
  charges = new float[numberOfParticles];
  names = new string[numberOfParticles];
  GeantIDs = new int[numberOfParticles];
  lifeTimes = new float[numberOfParticles];
  int n;
  float f1, f2, f3;
  char li[100];
  for (int i = 0; i < numberOfParticles; i++) {
    input >> n >> f1 >> f2 >> f3;
    input.getline(li, 100);
    setParticle(i, string(li), f1, f2, f3, n);
  }
  input.close();
}
bool Eparticles::IsInit() { return isInit; }

void Eparticles::setParticle(int ID, string name, float mass, float charge, float lifetime,
                             int GeantID)
{
  name = upper(name);
  remove(name, " ");
  if (ID < 0 || ID > numberOfParticles)
    return;
  names[ID] = name;
  masses[ID] = mass;
  charges[ID] = charge;
  lifeTimes[ID] = lifetime;
  GeantIDs[ID] = GeantID;
}

int Eparticles::getNumberOfParticles() { return numberOfParticles; }

int Eparticles::getGeantID(int ID)
{
  if (ID < 0 || ID > numberOfParticles)
    return -1;
  return GeantIDs[ID];
}

int Eparticles::getGeantID(string name)
{
  name = upper(name);
  remove(name, " ");
  for (int i = 0; i < numberOfParticles; i++)
    if (names[i] == name)
      return GeantIDs[i];
  return -1;
}

string Eparticles::getName(int ID)
{
  if (ID < 0 || ID > numberOfParticles)
    return "";
  return names[ID];
}

string Eparticles::getNameG(int GeantID)
{
  for (int i = 0; i < numberOfParticles; i++)
    if (GeantIDs[i] == GeantID)
      return names[i];
  return "";
}

float Eparticles::getMass(int ID)
{
  if (ID < 0 || ID > numberOfParticles)
    return -1;
  return masses[ID];
}

float Eparticles::getMass(string name)
{
  name = upper(name);
  remove(name, " ");
  for (int i = 0; i < numberOfParticles; i++)
    if (names[i] == name)
      return masses[i];
  return -1;
}

float Eparticles::getMassG(int GeantID)
{
  for (int i = 0; i < numberOfParticles; i++)
    if (GeantIDs[i] == GeantID)
      return masses[i];
  return -1;
}

float Eparticles::getCharge(int ID)
{
  if (ID < 0 || ID > numberOfParticles)
    return 0;
  return charges[ID];
}

float Eparticles::getCharge(string name)
{
  name = upper(name);
  remove(name, " ");
  for (int i = 0; i < numberOfParticles; i++)
    if (names[i] == name)
      return charges[i];
  return 0;
}

float Eparticles::getChargeG(int GeantID)
{
  for (int i = 0; i < numberOfParticles; i++)
    if (GeantIDs[i] == GeantID)
      return charges[i];
  return 0;
}

float Eparticles::getLifeTime(int ID)
{
  if (ID < 0 || ID > numberOfParticles)
    return -1;
  return lifeTimes[ID];
}

float Eparticles::getLifeTime(string name)
{
  name = upper(name);
  remove(name, " ");
  for (int i = 0; i < numberOfParticles; i++)
    if (names[i] == name)
      return lifeTimes[i];
  return -1;
}

float Eparticles::getLifeTimeG(int GeantID)
{
  for (int i = 0; i < numberOfParticles; i++)
    if (GeantIDs[i] == GeantID)
      return lifeTimes[i];
  return -1;
}

bool Eparticles::isInit = false;
float* Eparticles::masses = NULL;
float* Eparticles::charges = NULL;
float* Eparticles::lifeTimes = NULL;
int* Eparticles::GeantIDs = NULL;
string* Eparticles::names = NULL;
int Eparticles::numberOfParticles = 0;
