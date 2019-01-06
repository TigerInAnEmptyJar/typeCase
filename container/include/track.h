#ifndef TRACK
#define TRACK
#include "TBase.h"
#include "calibHit.h"
#include "geometry.h"
#include "pixel.h"
#include "rawHit.h"

/*!
 * \brief The TTrack class
 */
class TTrack : public TBase
{
private:
  const int maxhits;
  const int maxDets;
  int totnum;
  int* numhits; //[maxDets]
  sLine3D path;
  TCalibHit*** calibSrc; //!
  TPixel** pix;          // TPixel **pix;//[maxDets]->
  TPixel** pixsv;        // TPixel **pix;//[maxDets]->
  int numPix;
  momentum4D particle;
  float kineticEnergy;
  float speed;
  bool defined; //!
  float chiQ;
  bool order;
  TTrack* primary; // TTrack* primary;//->
  int numSec;
  TTrack** secondary; // TTrack **secondary;//[numHits]->
  bool valid;
  int charge;
  float errors[4];
  int partID;

public:
  /*!
   * \brief TTrack
   * Constructor. Sets the maximum number of hits and pixels per ID (default = 20) and the maximum
   * number of IDs (default = 50).
   * \param max
   * \param mdet
   */
  TTrack(int max = 20, int mdet = 50);

  /*!
   * \brief Destructor
   */
  ~TTrack();

  /*!
   * \brief getPath
   * Returns the path of the track. This is a straight line with the Foot-property corresponding to
   * the vertex and the Direction-property to the initial direction.
   * \return
   */
  sLine3D getPath() const;

  /*!
   * \brief getCalibSrc
   * Returns the numth hit of detector det, that is stored for this track. Returns an undefined,
   * non-valid hit, if such a hit is not stored or eighther num or det are non valid. This method
   * does not work properly and may cause crashs. Use getCalibSrcr() instead.
   * \param det
   * \param num
   * \return
   */
  TCalibHit getCalibSrc(int det, int num) const;

  /*!
   * \brief getParticle
   * Returns the particles 4-momentum.
   * \return
   */
  momentum4D getParticle() const;

  /*!
   * \brief setPath
   * Sets the path of the track. This is a straight line with the Foot-property corresponding to the
   * vertex and the Direction-property to the initial direction.
   * \param pathIn
   */
  void setPath(sLine3D pathIn);

  /*!
   * \brief addCalibSrc
   * Adds a hit to the track. Note that the detector-property of the hit should be set (0 <=
   * detector <= numberOfCalibHits(-2) ).
   * \param calibIn
   */
  void addCalibSrc(TCalibHit* calibIn);

  /*!
   * \brief clearCalibSrc
   * Sets the number of hits for all detectors to zero.
   */
  void clearCalibSrc();

  /*!
   * \brief removeCalibSrc
   * Removes a hit from the track.
   * \param calibIn
   */
  void removeCalibSrc(TCalibHit* calibIn);

  /*!
   * \brief getNumberOfCalibHits
   * Returns the number of hits stored in this track for the numth detector. If num is -1 it returns
   * the total number of hits stored in this track, if it is -2 it returns the maximum number of
   * hits to be stored in this track.
   * \param num
   * \return
   */
  int getNumberOfCalibHits(int num = -1) const;

  /*!
   * \brief setParticle
   * Sets the particles 4-momentum.
   * \param partIn
   */
  void setParticle(momentum4D partIn);

  /*!
   * \brief addPixel
   * Adds a pixel to the track.
   * \param pix
   */
  void addPixel(TPixel* pix);

  /*!
   * \brief clearPixel
   * Sets the number of pixels to zero.
   */
  void clearPixel();

  /*!
   * \brief getNumberOfPixels
   * Returns the number of pixels stored in this track.
   * \return
   */
  int getNumberOfPixels() const;

  /*!
   * \brief getPixel
   * Returns a reference to the numth pixel, that is saved for this track. The det parameter is
   * ignored.
   * \param det
   * \param num
   * \return
   */
  TPixel& getPixel(int det, int num);

  /*!
   * \brief getPathr
   * Returns a reference to the path of the track. This is a straight line with it's Foot-property
   * corresponding to the vertex and the Direction-property to its initial direction.
   * \return
   */
  sLine3D& getPathr();

  /*!
   * \brief getCalibSrcr
   * Returns a reference to the numth hit in the detth detector, that is saved for this track.
   * \param det
   * \param num
   * \return
   */
  TCalibHit* getCalibSrcr(int det, int num);

  /*!
   * \brief getParticler
   * Returns a reference to the particles 4-momentum, this track represents.
   * \return
   */
  momentum4D& getParticler();

  /*!
   * \brief getRawSrcr
   * Returns a reference to the numth raw hit in the detth detector, that is saved for this track.
   * \param det
   * \param num
   * \return
   */
  TRawHit& getRawSrcr(int det, int num);

  /*!
   * \brief isDefined
   * Returns true, if the track had been modified since creation or last reset().
   * \return
   */
  bool isDefined();

  /*!
   * \brief getChiSquared
   * Returns the chi squared property of the track. Use it for sorting or quality checks.
   * \return
   */
  float getChiSquared();

  /*!
   * \brief getChiSquaredr
   * Returns a reference to the chi squared property of the track. Use it for sorting or quality
   * checks.
   * \return
   */
  float& getChiSquaredr();

  /*!
   * \brief setChiSquared
   * Set the chi squared property of the track. Use it for sorting or quality checks.
   * \param chi
   */
  void setChiSquared(float chi);

  /*!
   * \brief operator =
   * Copy operator.
   * \param t
   */
  void operator=(TTrack& t);

  /*!
   * \brief isValid
   * Returns the valid property.
   * \return
   */
  bool isValid() const;

  /*!
   * \brief setValid
   * Sets the valid property of the track to val (default = true).
   * \param val
   */
  void setValid(bool val = true);

  /*!
   * \brief getKineticEnergy
   * Returns the kinetic energy (GeV) of the particle.
   * \return
   */
  float getKineticEnergy() const;

  /*!
   * \brief setKineticEnergy
   * Set the kinetic energy (GeV) of the particle.
   * \param val
   */
  void setKineticEnergy(float val);

  /*!
   * \brief getSpeed
   * Returns the speed of the particle. Note this can be larger than 1.
   * \return
   */
  float getSpeed() const;

  /*!
   * \brief setSpeed
   * Sets the speed of the particle in units of the speed of light. Since this is a measured
   * property with errors, this can be any number, also larger than 1 though this is forbidden by
   * definition.
   * \param val
   */
  void setSpeed(float val);

  /*!
   * \brief isSecondary
   * Returns true if the particle, this track represents did not origin in the primary reaction but
   * deayed off another particle.
   * \return
   */
  bool isSecondary() const;

  /*!
   * \brief setOrder
   * Sets the isSecondary property of the track to val (default = true). If val is true, the track
   * decayed off another track.
   * \param val
   */
  void setOrder(bool val = true);

  /*!
   * \brief getNumberOfSecondaryTracks
   * Returns the number of secondary tracks.
   * \return
   */
  int getNumberOfSecondaryTracks() const;

  /*!
   * \brief getSecondary
   * Returns the vth secondary track or NULL, if v is non-valid.
   * \return
   */
  TTrack* getSecondary(int);

  /*!
   * \brief clearSecondary
   * Sets the number of secondary tracks to zero.
   */
  void clearSecondary();

  /*!
   * \brief addSecondary
   * Adds a secondary track to the list of secondary tracks.
   */
  void addSecondary(TTrack*);

  /*!
   * \brief getPrimary
   * Returns the primary track if the particle, this track represents decayed, NULL else.
   * \return
   */
  TTrack* getPrimary();

  /*!
   * \brief setPrimary
   * Sets the primary track. This track is now a decay track, decaying of v.
   */
  void setPrimary(TTrack*);

  /*!
   * \brief setParticleID
   * Assign a GEANT-particle-ID to this track.
   * \param value
   */
  void setParticleID(int value);

  /*!
   * \brief getParticleID
   * Returns the GEANT-particle ID assigned to this track.
   * \return
   */
  int getParticleID() const;

  /*!
   * \brief reset
   * Resets the track. Sets the track to a primary track (primary-track property not modified), the
   * number of secondary tracks, kinetic energy and speed to zero, the valid property to false, as
   * well as the number of hits per ID and the number of pixels.
   */
  void reset();

  /*!
   * \brief getCharge
   * Returns the charge of the tracks particle.
   * \return
   */
  int getCharge() const;

  /*!
   * \brief getCharger
   * Returns a reference to the charge of the tracks particle.
   * \return
   */
  int& getCharger();

  /*!
   * \brief setCharge
   * Sets the charge of the tracks particle.
   * \param c
   */
  void setCharge(int c);

  /*!
   * \brief getErrorPhi
   * Returns the error variable for the property phi.
   * \return
   */
  float getErrorPhi() const;

  /*!
   * \brief getErrorTheta
   * Returns the error variable for the property theta.
   * \return
   */
  float getErrorTheta() const;

  /*!
   * \brief getErrorSpeed
   * Returns the error variable for the property speed.
   * \return
   */
  float getErrorSpeed() const;

  /*!
   * \brief getErrorEnergy
   * Returns the error variable for the property energy.
   * \return
   */
  float getErrorEnergy() const;

  /*!
   * \brief setErrorPhi
   * Sets the error variable for the property phi.
   * \param v
   */
  void setErrorPhi(float v);

  /*!
   * \brief setErrorTheta
   * Sets the error variable for the property theta.
   * \param v
   */
  void setErrorTheta(float v);

  /*!
   * \brief setErrorSpeed
   * Sets the error variable for the property speed.
   * \param v
   */
  void setErrorSpeed(float v);

  /*!
   * \brief setErrorEnergy
   * Sets the error variable for the property energy.
   * \param v
   */
  void setErrorEnergy(float v);
};

/*!
 * \brief operator <<
 * \param o
 * \param t
 * \return
 */
ostream& operator<<(ostream& o, const TTrack& t);

#endif
