#ifndef EPARTICLES
#define EPARTICLES
#include <string>
using namespace std;
class Eparticles
{
  static bool isInit;
  static float* masses;
  static float* charges;
  static float* lifeTimes;
  static int numberOfParticles;
  static int* GeantIDs;
  static string* names;

public:
  static void init();
  static void init(string filename);
  static void setParticle(int ID, string name, float mass, float charge, float lifetime,
                          int GeantID);
  static int getNumberOfParticles();
  static int getGeantID(int ID);
  static int getGeantID(string name);
  static string getName(int ID);
  static string getNameG(int GeantID);
  static float getMass(int ID);
  static float getMass(string name);
  static float getMassG(int GeantID);
  static float getCharge(int ID);
  static float getCharge(string name);
  static float getChargeG(int GeantID);
  static float getLifeTime(int ID);
  static float getLifeTime(string name);
  static float getLifeTimeG(int GeantID);
  static bool IsInit();
};
#endif
