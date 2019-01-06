#ifndef EPARTICLES
#define EPARTICLES
#include <string>
using namespace std;

/*!
 * \brief The Eparticles class
 * This class is a static data-base class containing properties of particles like masses, charges
 * and life-times along with the GEANT-ID.
 */
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
  /*!
   * \brief init
   * Default init method.
   */
  static void init();

  /*!
   * \brief init
   * Static function. Init method. Read the particle informations from file filename.
   * \param filename
   */
  static void init(string filename);

  /*!
   * \brief setParticle
   * Static function. Sets the particle of the internal number ID.
   * \param ID
   * \param name
   * \param mass
   * \param charge
   * \param lifetime
   * \param GeantID
   */
  static void setParticle(int ID, string name, float mass, float charge, float lifetime,
                          int GeantID);

  /*!
   * \brief getNumberOfParticles
   * Static function. Returns the number of defined particles.
   * \return
   */
  static int getNumberOfParticles();

  /*!
   * \brief getGeantID
   * Static function. Returns the GEANT-ID of the particle with the internal ID ID.
   * \param ID
   * \return
   */
  static int getGeantID(int ID);

  /*!
   * \brief getGeantID
   * Static function. Returns the GEANT-ID of the particle name.
   * \param name
   * \return
   */
  static int getGeantID(string name);

  /*!
   * \brief getName
   * Static function. Returns the name of the particle with the internal ID ID.
   * \param ID
   * \return
   */
  static string getName(int ID);

  /*!
   * \brief getNameG
   * Static function. Returns the Name of the particle with the GEANT particle ID GeantID.
   * \param GeantID
   * \return
   */
  static string getNameG(int GeantID);

  /*!
   * \brief getMass
   * Static function. Returns the mass of the particle with the internal ID ID.
   * \param ID
   * \return
   */
  static float getMass(int ID);

  /*!
   * \brief getMass
   * Static function. Returns the lmass of the particle name.
   * \param name
   * \return
   */
  static float getMass(string name);

  /*!
   * \brief getMassG
   * Static function. Returns the mass of the particle with the GEANT particle ID GeantID.
   * \param GeantID
   * \return
   */
  static float getMassG(int GeantID);

  /*!
   * \brief getCharge
   * Static function. Returns the charge of the particle with the internal ID ID.
   * \param ID
   * \return
   */
  static float getCharge(int ID);

  /*!
   * \brief getCharge
   * Static function. Returns the charge of the particle name.
   * \param name
   * \return
   */
  static float getCharge(string name);

  /*!
   * \brief getChargeG
   * Static function. Returns the charge of the particle with the GEANT particle ID GeantID.
   * \param GeantID
   * \return
   */
  static float getChargeG(int GeantID);

  /*!
   * \brief getLifeTime
   * Static function. Returns the life-time of the particle with the internal ID ID.
   * \param ID
   * \return
   */
  static float getLifeTime(int ID);

  /*!
   * \brief getLifeTime
   * Static function. Returns the life-time of the particle name.
   * \param name
   * \return
   */
  static float getLifeTime(string name);

  /*!
   * \brief getLifeTimeG
   * Static function. Returns the life-time of the particle with the GEANT particle ID GeantID.
   * \param GeantID
   * \return
   */
  static float getLifeTimeG(int GeantID);

  /*!
   * \brief IsInit
   * Static function. Returns true if Eparticles has been initialized.
   * \return
   */
  static bool IsInit();
};
#endif
