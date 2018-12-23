#ifndef FITTINGROUTINES
#define FITTINGROUTINES
#include "geometry.h"

using namespace std;
class lineFit: public sLine3D
{
 private:
  const int maxPoints;
  int numPoints;
  vector3D *closestPointToLine;//[maxPoints]
  vector3D *resolution;//[maxPoints]
  float *sigma;//[maxPoints]
  bool fitted;
  float chi2;
 public:
  lineFit();
  lineFit(const lineFit& line);
  lineFit(int maxPointsIn);
  ~lineFit();
  void clear();
  int MaxPoints()const;
  int Points()const;
  vector3D Point(int num)const;
  vector3D ResolutionVector(int num)const;
  float Sigma(int num)const;
  bool Fitted()const;
  float Chi()const;
  void addPoint(const vector3D &point,const vector3D& resolution, float sigma);
  sLine3D Result();
  float fit();
  static float fit(int numPoints, vector3D *points,vector3D *resolution, float *sigmas, sLine3D &result);
#ifdef USEROOT
  ClassDef(lineFit,4)
#endif
};

class vertexFit:public geomObject
{
 private:
  const int maxLines;
  int numLines;
  const int maxPoints;
  int *numPoints;//[maxLines]
  vector3D **points;//!
  vector3D **res;//!
  float **sigma;//!
  bool fitted;
  vector3D *lines;//[maxLines]
  point3D vertex;
  float chi2;
 public:
  vertexFit(int maxLinesIn, int maxPointsIn);
  vertexFit();
  vertexFit(const vertexFit &v);
  virtual ~vertexFit();
  bool Fitted()const;
  float Chi()const;
  int MaxPoints()const;
  int MaxLines()const;
  int Points(int line)const;
  vector3D Point(int line,int num)const;
  vector3D Resolution(int line,int num)const;
  float Sigma(int line, int num)const;
  vector3D Line(int num)const;
  point3D Vertex()const;
  void clear();
  void addPoint(const vector3D &p, const vector3D &r, float s, int line);
  float fit();
#ifdef USEROOT
  ClassDef(vertexFit,3)
#endif
};

class kinFit : public geomObject
{
 private:
  const int maxParticles;
  const int maxIteration;
  int nP;
  float epsilon;
  int **in;//!
  vector3D *particles;//[maxParticles]
  vector3D *dpin;//[maxParticles]
  float *am;//[maxParticles]
  vector3D *p;//[maxParticles]
  int nC;
  float *ain;//[nC]
  bool useCovariant;
  int outCode;
  float chi;
  Vector fm;
  float d_last;
  vector4D getPDerivative(int isIn,int pos,vector3D pm,float mass);
  float getMDerivative(int representation, int pos, vector3D pm, float mass, Vector sf);
  void calculateCinematics(momentum4D *ppx, Vector &FM, Vector *sf, float &d);
  void checkValues(int rep,vector3D &pm,bool m1,bool m2,bool m3);
 public:
  kinFit();
  kinFit(const kinFit &inFit);
  kinFit(int maxIterationIn,int maxParticlesIn,float epsilonIn);
  virtual ~kinFit();
  float Epsilon()const;
  void setEpsilon(float value);
  int NParticles()const;
  int MaxIterations()const;
  int MaxParticles()const;
  void clear();
  void add(const Vector &data);
  void add(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int representation, int in_out_going=1, int *additionalConstraints=NULL);
  void addMomentum(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addEnergyThetaPhi(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addBetaThetaPhi(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addMomentumThetaPhi(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addEnergyDxDy(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addBetaDxDy(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  void addMomentumDxDy(const vector3D &properties, const vector3D &errors, const vector3D& doFit, float mass, int in_out_going=1, int *additionalConstraints=NULL);
  vector3D Errors(int num)const;
  vector3D NonFittedParticle(int num)const;
  float Mass(int num)const;
  int Constraint(int numP, int numC)const;
  int Representation(int num)const;
  static int KnownRepresentations(int **buffer=NULL);
  static int KnownRepresentation(int num);
  void setConstraints(int nCin,Vector masses);
  float InvariantMass(int constraint) const;
  int Constraints()const;
  float Chi2()const;
  int OutInfo()const;
  Vector Result() const;
  vector3D FittedParticle(int num)const;
  momentum4D getMomentum(int num,vector3D pm);
  static momentum4D getMomentum(vector3D pm, int rep,float mass);
  Vector FM_last()const;
  float D_last()const;
  float fit();
#ifdef USEROOT
  ClassDef(kinFit,4)
#endif
};

#endif
