#ifndef RBASEREACTION
#define RBASEREACTION
#include "geometry.h"
#include "track.h"
class AFitAlgorithm;

/*!
 * \brief The RbaseReaction class
 */
class RbaseReaction : public TBase
{
protected:
  const int numberOfInitParticles;
  momentum4D* initParticles; //[numberOfInitParticles]
  const int numberOfTracks;
  TTrack** tracks; //[numberOfTracks]
  int numberOfParticles;
  momentum4D* particles; //[numberOfParticles]
  momentum4D cm;
  int* particleIDs;
  float excessEnergy;
  momentum4D* cmparticles; //[numberOfParticles]
  AFitAlgorithm* fkinFit;  //!
  float chiLimit;
  float chi2;

  /*!
   * \brief calculateCM
   * Calculate the Center-of-Mass properties of all paricipating particles.
   */
  void calculateCM();

public:
  /*!
   * \brief RbaseReaction
   * Constructor. Sets the number of outgoing tracks, the number of ingoing particles
   * and a kinfit-algorithm pointer.
   * \param kinFitIn
   * \param nTracks
   * \param nInits
   */
  RbaseReaction(AFitAlgorithm* kinFitIn, int nTracks = 2, int nInits = 2);

  /*!
   * \brief RbaseReaction
   * Copy constructor.
   * \param rIn
   */
  RbaseReaction(const RbaseReaction& rIn);

  /*!
   * \brief Destructor
   */
  virtual ~RbaseReaction();

  /*!
   * \brief getNumberOfTracks
   * Returns the number of outgoing particles
   * \return
   */
  int getNumberOfTracks() const;

  /*!
   * \brief getNumberOfPrimaries
   * Returns the number of ingoing particles.
   * \return
   */
  virtual int getNumberOfPrimaries() const;

  /*!
   * \brief getNumberOfSecondaries
   * Returns the number of outgoing tracks.
   * \return
   */
  virtual int getNumberOfSecondaries() const;

  /*!
   * \brief getParticle
   * Returns the 4-momentum of the numth outgoing particle of the reaction.
   * \param num
   * \return
   */
  virtual momentum4D getParticle(int num) const;

  /*!
   * \brief getParticleID
   * Returns the Geant ID of the numth outgoing particle of the reaction.
   * \param num
   * \return
   */
  int getParticleID(int num) const;

  /*!
   * \brief setTrack
   * Sets the track pointer of the numth particle of the reaction.
   * \param num
   * \param track
   */
  virtual void setTrack(int num, TTrack* track);

  /*!
   * \brief getTrack
   * Returns the track corresponding to the numth outgoing particle of this reaction or
   * NULL if num is non valid.
   * \param num
   * \return
   */
  TTrack* getTrack(int num);
  //  virtual float check(TTrack **tracks, int num);
  //  virtual float justCheck(TTrack **tracks, int num, int *indexes=NULL,
  //  momentum4D
  //  *results=NULL);
  //  virtual RbaseReaction* checkNew(TTrack **tracks,int num);

  /*!
   * \brief check
   * Does a reaction recognition with the num input tracks tracks. If the chi squared
   * value is below the limit, the track-pointers and the particle 4-momenta are set.
   * \param tracks_nd
   * \param num_nd
   * \param tracks_1s
   * \param num_1s
   * \param tracks_2s
   * \param num_2s
   * \return
   */
  virtual float check(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                      TTrack** tracks_2s, int num_2s);

  /*!
   * \brief justCheck
   * Does a reaction recognition with the num input tracks tracks and returns the chi
   * squared value of that check.
   * \param tracks_nd
   * \param num_nd
   * \param tracks_1s
   * \param num_1s
   * \param tracks_2s
   * \param num_2s
   * \param indexes
   * \param results
   * \return
   */
  virtual float justCheck(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                          TTrack** tracks_2s, int num_2s, int* indexes = NULL,
                          momentum4D* results = NULL);

  /*!
   * \brief checkNew
   * Does a reaction recognition with the num input tracks tracks. If the chi squared
   * value is below the limit, it returns a new reaction of that type. Please take care
   * that you delete this reaction correctly after use (potential memory leak).
   * \param tracks_nd
   * \param num_nd
   * \param tracks_1s
   * \param num_1s
   * \param tracks_2s
   * \param num_2s
   * \return
   */
  virtual RbaseReaction* checkNew(TTrack** tracks_nd, int num_nd, TTrack** tracks_1s, int num_1s,
                                  TTrack** tracks_2s, int num_2s);

  /*!
   * \brief setChiLimit
   * Set the chi squared limit for reaction recognition. Note, that the chi squared value
   * does not need to come from a kinfit, it may also correspond to the missing mass of
   * the outgoing tracks or else.
   * \param chi
   */
  void setChiLimit(float chi);

  /*!
   * \brief getChiLimit
   * Return the limit for the chi squared value in reaction recognition check. Note,
   * that the chi squared value does not need to come from a kinfit, it may also
   * correspond to the missing mass of the outgoing tracks or else.
   * \return
   */
  float getChiLimit() const;

  /*!
   * \brief getChiSquared
   * Returns the chi squared value of the particle recognition check.
   * \return
   */
  float getChiSquared() const;

  /*!
   * \brief setInitParticles
   * Sets the initial particles. Its number is set to num. Take care, that the array p
   * is at least num indexes long.
   * \param num
   * \param p
   */
  virtual void setInitParticles(int num, momentum4D* p);

  /*!
   * \brief getInitParticle
   * Return the 4-momentum of the numth initial particle.
   * \param num
   * \return
   */
  momentum4D getInitParticle(int num = 0) const;

  /*!
   * \brief getExcessEnergy
   * Return the excess energy of the reaction. If it is zero or smaller than that, the
   * reaction cannot take place.
   * \return
   */
  float getExcessEnergy() const;

  /*!
   * \brief getCenterOfMass
   * Return 4-momentum of the center of mass
   * \return
   */
  momentum4D getCenterOfMass() const;

  /*!
   * \brief getCMParticle
   * Return the numth particle in the center-of-mass.
   * \param num
   * \return
   */
  momentum4D getCMParticle(int num) const;

  /*!
   * \brief getKinFit
   * Returns the pointer to the kinfit algorithm that may be used. Note, that this may
   * be NULL.
   * \return
   */
  AFitAlgorithm* getKinFit() const;

  /*!
   * \brief getClone
   * Creates a new reaction with the same properties. Take care that the reaction is deleted
   * properly after use (potential memory leak).
   * \return
   */
  virtual RbaseReaction* getClone();

  /*!
   * \brief calculate
   * Do calculate the reaction properties.
   * \param justGeometry
   */
  virtual void calculate(int justGeometry = 1);
  //  bool operator<(const RbaseReaction &r);
  //  bool operator<=(const RbaseReaction &r);
  //  bool operator>(const RbaseReaction &r);
  //  bool operator>=(const RbaseReaction &r);
  //  virtual Bool_t IsSortable();
  //  ClassDef(RbaseReaction,2)
};

/*!
 * \brief getReactionFromName
 * \param name
 * \return
 */
RbaseReaction* getReactionFromName(const string& name);
#endif
