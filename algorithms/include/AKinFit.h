#include "fitAlgorithm.h"
#include "geometry.h"

class AKinFit : public AFitAlgorithm
{
  Q_OBJECT
private:
  float epsilon;
  const int maxParticles; // NPMX
  const int maxIteration;
  int nP;              // number of particles
  int** in;            //! variable types [4+NC] is containt in constraint j?
  vector3D* particles; //[maxParticles]Pin momenta of particles
  vector3D* dpin;      //[maxParticles]errors of momenta of particles
  float* am;           //[maxParticles]masses
  int nC;              // number of additional constraints
  float* ain;          //[NC] inv masses for each constraint
  bool useCovariant;
  vector3D* p; //[maxParticles] pout output momenta
  int outCode;
  vector4D getDerivative(int isIn, int pos, vector3D pm, float mass);
  float getDerivativeConstraint(int representation, int pos, vector3D pm, momentum4D px, float mass,
                                Vector sf);
  momentum4D getMomentum(int num, vector3D pm);

public:
  AKinFit(int maxIterationIn, int maxParticlesIn, float epsilonIn);
  virtual ~AKinFit();
  virtual void* process(void* ptr);
  virtual void add(Vector& data);
  virtual void clear();
  virtual Vector getResult();
  void setConstraints(int nCin, Vector masses);
  virtual AFitAlgorithm* getClone();
  int getOutInfo() const;
  //  virtual float fit();
public slots:
  virtual void getAddPoint(Vector& data);
  virtual float fit();
signals:
  void fitted(float chi, Vector& result);
};
