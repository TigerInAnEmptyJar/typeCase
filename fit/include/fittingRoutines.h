#ifndef FITTINGROUTINES
#define FITTINGROUTINES
#include "geometry.h"

using namespace std;

/*!
 * \brief The lineFit class
 * This class implements a simple line fit in 3D.
 */
class lineFit : public sLine3D
{
private:
  const int maxPoints;
  int numPoints;
  vector3D* closestPointToLine; //[maxPoints]
  vector3D* resolution;         //[maxPoints]
  float* sigma;                 //[maxPoints]
  bool fitted;
  float chi2;

public:
  /*!
   * \brief lineFit
   * Default constructor.
   */
  lineFit();

  /*!
   * \brief lineFit
   * Copy constructor.
   * \param line
   */
  lineFit(const lineFit& line);

  /*!
   * \brief lineFit
   * Constructor. Defines the maximum number of points that can be fitted to a line.
   * \param maxPointsIn
   */
  lineFit(int maxPointsIn);

  /*!
   * \brief Destructor
   */
  ~lineFit();

  /*!
   * \brief clear
   * Clears the points. A new fit with new points can be done.
   */
  void clear();

  /*!
   * \brief MaxPoints
   * Returns the maximum number of points the line can be fitted to.
   * \return
   */
  int MaxPoints() const;

  /*!
   * \brief Points
   * Returns the number of point the line will be fitted to.
   * \return
   */
  int Points() const;

  /*!
   * \brief Point
   * Returns the numth point.
   * \param num
   * \return
   */
  vector3D Point(int num) const;

  /*!
   * \brief ResolutionVector
   * Returns the resolution vector for point num.
   * \param num
   * \return
   */
  vector3D ResolutionVector(int num) const;

  /*!
   * \brief Sigma
   * Returns the error value for point num.
   * \param num
   * \return
   */
  float Sigma(int num) const;

  /*!
   * \brief Fitted
   * Returns true if the fit has been done.
   * \return
   */
  bool Fitted() const;

  /*!
   * \brief Chi
   * Returns the chi-squared of the fit. Returns -1 if the fit has not been performed yet.
   * \return
   */
  float Chi() const;

  /*!
   * \brief addPoint
   * Adds a point to the line. resolution gives the direction in which the point may be modified and
   * sigma its error. Does nothing, if the number of points to be stored is at maximum.
   * \param point
   * \param resolution
   * \param sigma
   */
  void addPoint(const vector3D& point, const vector3D& resolution, float sigma);

  /*!
   * \brief Result
   * Returns the result of the fit (straight line, the foot being in the x-y-plane), if it has been
   * performed. Returns an undefined line, if not.
   * \return
   */
  sLine3D Result();

  /*!
   * \brief fit
   * Main fitting routine. Does the fit and returns the chi-squared value.
   * \return
   */
  float fit();

  /*!
   * \brief fit
   * Static method. Fits a line to numPoints points in 3D. The resolution-vectors are the vectors in
which the points can be modified, the sigmas, the errors for the points.
Take care, that the arrays points, resolution and sigmas are at least numPoints indeces long.
The result of the fit is given by the straight line result, where the Foot of the line is always in
the x-y-plane.
The return value is the chi-squared value of the fit.
   * \param numPoints
   * \param points
   * \param resolution
   * \param sigmas
   * \param result
   * \return
   */
  static float fit(int numPoints, vector3D* points, vector3D* resolution, float* sigmas,
                   sLine3D& result);
#ifdef USEROOT
  ClassDef(lineFit, 4)
#endif
};

/*!
 * \brief The vertexFit class
 * Class for fitting several lines with a common vertex. Works for 1 and 2 lines will have to be
 * extended for more lines. An object of this class can first do a 2-line-fit, then a 1-line-fit,
 * then a 2-line fit. The number of lines to be fitted is calculated on the fly.
 */
class vertexFit : public geomObject
{
private:
  const int maxLines;
  int numLines;
  const int maxPoints;
  int* numPoints;    //[maxLines]
  vector3D** points; //!
  vector3D** res;    //!
  float** sigma;     //!
  bool fitted;
  vector3D* lines; //[maxLines]
  point3D vertex;
  float chi2;

public:
  /*!
   * \brief vertexFit
   * Constructor. Sets the maximum number of lines and the maximum number of points per line.
   * \param maxLinesIn
   * \param maxPointsIn
   */
  vertexFit(int maxLinesIn, int maxPointsIn);

  /*!
   * \brief vertexFit
   * Default constructor.
   */
  vertexFit();

  /*!
   * \brief vertexFit
   * Copy constructor.
   * \param v
   */
  vertexFit(const vertexFit& v);

  /*!
   * \brief ~vertexFit
   */
  virtual ~vertexFit();

  /*!
   * \brief Fitted
   * Returns true if the fit has been performed.
   * \return
   */
  bool Fitted() const;

  /*!
   * \brief Chi
   * Returns the chi-squared of a fit. Returns -1 if the fit has not been performed yet.
   * \return
   */
  float Chi() const;

  /*!
   * \brief MaxPoints
   * Returns the maximum number of points to be stored for one line.
   * \return
   */
  int MaxPoints() const;

  /*!
   * \brief MaxLines
   * Returns the maximum number of lines to be fitted to a common vertex.
   * \return
   */
  int MaxLines() const;

  /*!
   * \brief Points
   * Returns the number of points to be fitted to line line.
   * \param line
   * \return
   */
  int Points(int line) const;

  /*!
   * \brief Point
   * Returns the numth point on line line.
   * \param line
   * \param num
   * \return
   */
  vector3D Point(int line, int num) const;

  /*!
   * \brief Resolution
   * Returns the resolution vector of the numth point on line line.
   * \param line
   * \param num
   * \return
   */
  vector3D Resolution(int line, int num) const;

  /*!
   * \brief Sigma
   * Returns the error of the numth point on line line.
   * \param line
   * \param num
   * \return
   */
  float Sigma(int line, int num) const;

  /*!
   * \brief Line
   * Returns the numth line direction vector, if the fit has been performed, an undefined vector
   * else.
   * \param num
   * \return
   */
  vector3D Line(int num) const;

  /*!
   * \brief Vertex
   * Returns the vertex point, if the fit has been performed, an undefined point else.
   * \return
   */
  point3D Vertex() const;

  /*!
   * \brief clear
   * Clears the fit. To perform a new fit add new points to the fit.
   */
  void clear();

  /*!
   * \brief addPoint
   * Adds a point p to the fit, with a resolution vector r and an error s. The parameter line
   * defined to which line the point should be fitted.
   * \param p
   * \param r
   * \param s
   * \param line
   */
  void addPoint(const vector3D& p, const vector3D& r, float s, int line);

  /*!
   * \brief fit
   * Main fitting routine. Fits the lines to a common vertex and returns the chi-squared of the fit.
   * \return
   */
  float fit();
#ifdef USEROOT
  ClassDef(vertexFit, 3)
#endif
};

/*!
 * \brief The kinFit class
 */
class kinFit : public geomObject
{
private:
  const int maxParticles;
  const int maxIteration;
  int nP;
  float epsilon;
  int** in;            //!
  vector3D* particles; //[maxParticles]
  vector3D* dpin;      //[maxParticles]
  float* am;           //[maxParticles]
  vector3D* p;         //[maxParticles]
  int nC;
  float* ain; //[nC]
  bool useCovariant;
  int outCode;
  float chi;
  Vector fm;
  float d_last;

  /*!
   * \brief getPDerivative
   * \param isIn
   * \param pos
   * \param pm
   * \param mass
   * \return
   */
  vector4D getPDerivative(int isIn, int pos, vector3D pm, float mass);

  /*!
   * \brief getMDerivative
   * \param representation
   * \param pos
   * \param pm
   * \param mass
   * \param sf
   * \return
   */
  float getMDerivative(int representation, int pos, vector3D pm, float mass, Vector sf);

  /*!
   * \brief calculateCinematics
   * \param ppx
   * \param FM
   * \param sf
   * \param d
   */
  void calculateCinematics(momentum4D* ppx, Vector& FM, Vector* sf, float& d);

  /*!
   * \brief checkValues
   * \param rep
   * \param pm
   * \param m1
   * \param m2
   * \param m3
   */
  void checkValues(int rep, vector3D& pm, bool m1, bool m2, bool m3);

public:
  /*!
   * \brief kinFit
   * Default constructor.
   */
  kinFit();

  /*!
   * \brief kinFit
   * Copy constructor.
   * \param inFit
   */
  kinFit(const kinFit& inFit);

  /*!
   * \brief kinFit
   * Constructor. Sets the maximum number of iterations and particles and the maximum derivation of
   * the kinematic variable from its restrictions.
   * \param maxIterationIn
   * \param maxParticlesIn
   * \param epsilonIn
   */
  kinFit(int maxIterationIn, int maxParticlesIn, float epsilonIn);

  /*!
   * \brief ~kinFit
   */
  virtual ~kinFit();

  /*!
   * \brief Epsilon
   * Returns the maximal derivation of the kinematic variable to the conservation laws.
   * \return
   */
  float Epsilon() const;

  /*!
   * \brief setEpsilon
   * Sets the maximal derivation of the kinematic variable to the conservation laws.
   * \param value
   */
  void setEpsilon(float value);

  /*!
   * \brief NParticles
   * Returns the current number of particles.
   * \return
   */
  int NParticles() const;

  /*!
   * \brief MaxIterations
   * Returns the maximum number of iterations for the fit.
   * \return
   */
  int MaxIterations() const;

  /*!
   * \brief MaxParticles
   * Returns the maximum number of particles.
   * \return
   */
  int MaxParticles() const;

  /*!
   * \brief clear
   * Clears the fits particles. For a new fit, add new particles.
   */
  void clear();

  /*!
   * \brief add
   * Adds a particle to the fit.
index/indices	description
0, 1, 2	particle values
3, 4, 5	value errors
6	mass
7, 8, 9	fixed/measured/unmeasured
10	representation
rest	contribute to additional constraint
   * \param data
   */
  void add(const Vector& data);

  /*!
   * \brief add
   * Add a particle to the fit with its properties, errors, its mass, the representation the values
   * are given. The parameter in_out_going shall be 1 if the particle is an outgoing particle in the
   * initial reaction, -1 if it is in-going and 0 if it doesn't take place in the initial reaction.
   * The parameter doFit sets the individual values to fixed (2), measured (1) or unmeasured (3).
   * The additionalConstraints is an array (-pointer) with its components being 0 (doesn't
   * contribute) or 1 (contributes). If the array is NULL (default), the particle doesn't contribute
   * in any additional constraint.
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param representation
   * \param in_out_going
   * \param additionalConstraints
   */
  void add(const vector3D& properties, const vector3D& errors, const vector3D& doFit, float mass,
           int representation, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addMomentum
   * Add a particle in representation (Px,Py,Pz) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addMomentum(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                   float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addEnergyThetaPhi
   * Add a particle in representation (E,theta,phi) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addEnergyThetaPhi(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                         float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addBetaThetaPhi
   * Add a particle in representation (ß,theta,phi) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addBetaThetaPhi(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                       float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addMomentumThetaPhi
   * Add a particle in representation (P,theta,phi) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addMomentumThetaPhi(const vector3D& properties, const vector3D& errors,
                           const vector3D& doFit, float mass, int in_out_going = 1,
                           int* additionalConstraints = NULL);

  /*!
   * \brief addEnergyDxDy
   * Add a particle in representation (E,dx,dy) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addEnergyDxDy(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                     float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addBetaDxDy
   * Add a particle in representation (ß,dx,dy) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addBetaDxDy(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                   float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief addMomentumDxDy
   * Add a particle in representation (P,dx,dy) see add().
   * \param properties
   * \param errors
   * \param doFit
   * \param mass
   * \param in_out_going
   * \param additionalConstraints
   */
  void addMomentumDxDy(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                       float mass, int in_out_going = 1, int* additionalConstraints = NULL);

  /*!
   * \brief Errors
   * Returns the errors for values of particle num in current representation.
   * \param num
   * \return
   */
  vector3D Errors(int num) const;

  /*!
   * \brief NonFittedParticle
   * Returns the values of the non-fitted particle num in current representation.
   * \param num
   * \return
   */
  vector3D NonFittedParticle(int num) const;

  /*!
   * \brief Mass
   * Returns the mass for the numth particle in units of GeV.
   * \param num
   * \return
   */
  float Mass(int num) const;

  /*!
   * \brief Constraint
   * Returns the constraint value for particle numP in constraint numC. This should be 0 if the
   * particle is not participating in the constraint, and 1 if it is. If numP or numC are non-valid,
   * it returns 0.
   * \param numP
   * \param numC
   * \return
   */
  int Constraint(int numP, int numC) const;

  /*!
   * \brief Representation
   * Returns the representation of the numth particle. Known representations are:
value	representation
1	px, py, pz
2	E, azimuthal-, polar angle
3	v, azimuthal-, polar angle
4	p, azimuthal-, polar angle
   * \param num
   * \return
   */
  int Representation(int num) const;

  /*!
   * \brief KnownRepresentations
   * Static method. Returns the number of known representations and the representation number if the
   * buffer pointer is not NULL, then a new array of integer will be allocated and the values will
   * be copied to it. Take care, that you dispose it correctly after use (potential memory leak).
   * \param buffer
   * \return
   */
  static int KnownRepresentations(int** buffer = NULL);

  /*!
   * \brief KnownRepresentation
   * Static method. It returns the value of the numth representation. obsolete removed from class
   * \param num
   * \return
   */
  static int KnownRepresentation(int num);

  /*!
   * \brief setConstraints
   * Sets the number of constraints and the invariant masses for each constraint. Take care that the
   * vectors dimension is at least nCin, else it does nothing.
   * \param nCin
   * \param masses
   */
  void setConstraints(int nCin, Vector masses);

  /*!
   * \brief InvariantMass
   * Returns the invariant mass (units of GeV) for constraint constraint.
   * \param constraint
   * \return
   */
  float InvariantMass(int constraint) const;

  /*!
   * \brief Constraints
   * Returns the number of additional constraints.
   * \return
   */
  int Constraints() const;

  /*!
   * \brief Chi2
   * Returns the chi-squared value of the fit.
   * \return
   */
  float Chi2() const;

  /*!
   * \brief OutInfo
   * Returns the number of Iterations needed to come to an end. If the maximum number of iterations
   * was exceeded, the return value is negative.
   * \return
   */
  int OutInfo() const;

  /*!
   * \brief Result
   * Returns the result vector that is all the particles values in current representation stuffed
   * into one vector of length nParticles*3, with the particle values together. It might be easier
   * to come to the result by FittedParticle.
   * \return
   */
  Vector Result() const;

  /*!
   * \brief FittedParticle
   * Returns the values of the numth particle in current representation.
   * \param num
   * \return
   */
  vector3D FittedParticle(int num) const;

  /*!
   * \brief getMomentum
   * Calculates the 4-momentum of the numth particle using its mass and the values in pm according
   * to its representation.
   * \param num
   * \param pm
   * \return
   */
  momentum4D getMomentum(int num, vector3D pm);

  /*!
   * \brief getMomentum
   * Satic method to calculate the 4-momentum of a particle using its mass and the values in pm
   * according to its representation rep.
   * \param pm
   * \param rep
   * \param mass
   * \return
   */
  static momentum4D getMomentum(vector3D pm, int rep, float mass);

  /*!
   * \brief FM_last
   * Returns the last balance vector of the fit. The first four components are the components of the
   * sum of all (initially participating, with sign for in-/out-going) particle 4-momenta. The rest
   * components are the difference of the invariant masses to the invariant mass of the
   * corresponding constraint.
   * \return
   */
  Vector FM_last() const;

  /*!
   * \brief D_last
   * Returns the remaining length of the balance vector after the fit. See FM_last().
   * \return
   */
  float D_last() const;

  /*!
   * \brief fit
   * Main fitting routine performs the fit and returns the chi-squared value.
   * \return
   */
  float fit();
#ifdef USEROOT
  ClassDef(kinFit, 4)
#endif
};

#endif
