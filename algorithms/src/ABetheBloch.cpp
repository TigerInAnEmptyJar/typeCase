#include "ABetheBloch.h"
#include "Eparticles.h"
ABetheBloch::ABetheBloch()
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  m_electron = Eparticles::getMassG(3); // GeV
  float c_electron = 1.602176487e-19;   // Coulomb
  c_light = 299792458;                  // m/s²
  float epsilon = 8.854187e-12;         // Coulomb/V/m
  N_a = 6.02214179e23;                  // mol-¹
  N_a = 1.78266e-24 * 1e6 * N_a * N_a;  // N_a*1e-10*1e6/(1.78266*6.02214179);//mol-¹*g/GeV
  c1 = /*c_electron*/ (c_electron / epsilon / 4 / M_PI) * 1e-9;
  c1 = 4. * M_PI / m_electron * (c1 * c1); // GeV*cm²
  //   c2=N_a*Z*density/MolarMass;//1/cm³
  //   c3=10.*Z;//eV
  //   c3=c3*1e-9;//GeV
  //   c3=2.*m_electron/c3/c3;//1/GeV
  nComponents = 1;
  c2 = new double[nComponents];
  c3 = new double[nComponents];
  Z = new double[nComponents];
  A = new double[nComponents];
  weight = new double[nComponents];
  stepwidth = 0.1;
}
ABetheBloch::ABetheBloch(const ABetheBloch& algo) : AELossAlgorithm(algo)
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  m_electron = Eparticles::getMassG(3); // GeV
  float c_electron = 1.602176487e-19;   // Coulomb
  c_light = 299792458;                  // m/s²
  epsilon = 8.854187e-12;               // Coulomb/V/m
  //	1.78266e-24//g/GeV
  N_a = 6.02214179e23;                 // mol-¹
  N_a = 1.78266e-24 * 1e6 * N_a * N_a; // N_a*1e-10*1e6/(1.78266*6.02214179);//mol-¹*g/GeV
  c1 = /*c_electron*/ (c_electron / epsilon / 4 / M_PI) * 1e-9;
  c1 = 4. * M_PI / m_electron * (c1 * c1); // *1e6;//GeV*cm²
  //  cout<<c1<<" "<<
  nComponents = 1;
  c2 = new double[nComponents];
  c3 = new double[nComponents];
  Z = new double[nComponents];
  A = new double[nComponents];
  weight = new double[nComponents];
  setParameters(algo.parameters());
  stepwidth = algo.stepSize();
}
ABetheBloch::~ABetheBloch()
{
  delete[] c2;
  delete[] c3;
  delete[] Z;
  delete[] A;
  delete[] weight;
}
Vector ABetheBloch::parameters() const
{
  Vector ret(2 + nComponents * 3);
  ret.setValues(density, radiationLength);
  for (int i = 0; i < nComponents; i++) {
    ret.setValue(i * 3 + 2, Z[i]);
    ret.setValue(i * 3 + 3, A[i]);
    ret.setValue(i * 3 + 4, weight[i]);
  }
  return ret;
}
void ABetheBloch::setParameters(const Vector& parameter)
{
  if (parameter.getDimension() < 4)
    return;
  if (parameter.getDimension() <= 5) {
    density = parameter[0];
    radiationLength = parameter[1];
    nComponents = 1;
    delete[] c2;
    delete[] c3;
    delete[] Z;
    delete[] A;
    delete[] weight;
    c2 = new double[nComponents];
    c3 = new double[nComponents];
    Z = new double[nComponents];
    A = new double[nComponents];
    weight = new double[nComponents];
    Z[0] = parameter[2];
    A[0] = parameter[3]; // GeV/nucleus
    c2[0] = N_a * Z[0] * density / A[0];
    c3[0] = 10. * Z[0] * 1e-9;
    c3[0] = 2. * m_electron / c3[0] / c3[0];
    //      cout<<N_a<<" "<<Z[0]<<" "<<A[0]<<" "<<m_electron<<" "<<c2[0]<<"
    //      "<<c3[0]<<endl;
    weight[0] = 1;
    return;
  }
  density = parameter[0];
  radiationLength = parameter[1];
  nComponents = (parameter.getDimension() - 1) / 3;
  delete[] c2;
  delete[] c3;
  delete[] Z;
  delete[] A;
  delete[] weight;
  c2 = new double[nComponents];
  c3 = new double[nComponents];
  Z = new double[nComponents];
  A = new double[nComponents];
  weight = new double[nComponents];
  for (int i = 0; i < nComponents; i++) {
    Z[i] = parameter[i * 3 + 2];
    A[i] = parameter[i * 3 + 3]; // GeV/nucleus
    weight[i] = parameter[i * 3 + 4];
    c2[i] = N_a * Z[i] * density / A[i];
    c3[i] = 10. * Z[i] * 1e-9;
    c3[i] = 2. * m_electron / c3[i] / c3[i];
  }
}
double ABetheBloch::stepSize() const { return stepwidth; }
void ABetheBloch::setStepsize(double value) { stepwidth = abs(value); }
void* ABetheBloch::process(void* ptr) { return ptr; }

float ABetheBloch::energyLoss(momentum4D particle, float path)
{
  return energyLoss(particle, path, 1.);
}
float ABetheBloch::energyLossM(int geantId, float momentum, float path)
{
  double charge = Eparticles::getChargeG(geantId);
  double mass = Eparticles::getMassG(geantId);
  double energy = sqrt(momentum * momentum + mass * mass);
  if (geantId < 7)
    return energyLoss_photon(geantId, energy, path);
  return energyLoss(path, mass, momentum / energy, energy, charge * charge);
}
float ABetheBloch::energyLossB(int geantId, float beta, float path)
{
  double mass = Eparticles::getMassG(geantId);
  double charge = Eparticles::getChargeG(geantId);
  double energy = mass / sqrt(1 - beta * beta);
  if (geantId < 7)
    return energyLoss_photon(geantId, energy, path);
  return energyLoss(path, Eparticles::getMassG(geantId), beta, energy, charge * charge);
}
float ABetheBloch::energyLossE(int geantId, float energy, float path)
{
  double charge = Eparticles::getChargeG(geantId);
  double mass = Eparticles::getMassG(geantId);
  double gamma = energy / mass;
  double beta = sqrt(-(1. / (gamma * gamma) - 1.));
  if (geantId < 7)
    return energyLoss_photon(geantId, energy, path);
  return energyLoss(path, mass, beta, energy, charge * charge);
}
float ABetheBloch::energyLoss(float path, float mass, float beta, float energy, float charge2)
{
  double beta2 = beta * beta;
  double gamma = 1 / sqrt(1 - beta2);
  double gamma2 = gamma * gamma;
  double E_kin = energy - mass;
  double fraq = m_electron / mass;
  double T_max = (2. * m_electron * beta2 * gamma2) / (1. + (2. * gamma * fraq) + (fraq * fraq));
  double sumOfWeights = 0;
  for (int i = 0; i < nComponents; i++)
    sumOfWeights += weight[i];
  // cout<<"sum of weights "<<sumOfWeights<<endl;
  //  dEdx=(charge*charge*/(beta*beta))*k*Z/A*(0.5*log(2*m_electron*c*c*beta*beta*gamma*gamma*T_max/excitationE/excitationE)-beta*beta-densityModifications/2);
  // dEdx=(charge2*beta2)*constant1*(0.5*log(constant2*beta2*gamma2*T_max)-beta2-densityModifications/2.);
  double dEdx = 0;
  // cout<<nComponents<<endl;
  // cout<<c1<<" "<<T_max<<" "<<charge2<<" "<<beta2<<" "<<gamma2<<"
  // "<<E_kin<<endl;
  for (int i = 0; i < nComponents; i++) {
    dEdx += weight[i] * c1 * c2[i] * charge2 / beta2 *
            (0.5 * log(c3[i] * beta2 * gamma2 * T_max) - beta2);
    // cout<<c2[i]<<" "<<c3[i]<<endl;
    // cout<<c1*c2[i]*charge2/beta2*(0.5*log(2.*c3[i]*beta2*gamma2*T_max)-beta2)<<"
    // "<<weight[i]<<"
    // "<<c1*c2[i]<<" "<<charge2/beta2<<"
    // "<<0.5*log(2.*c3[i]*beta2*gamma2*T_max)<<"
    // "<<c3[i]/T_max<<" "<<-beta2<<endl;
  }
  dEdx = dEdx / sumOfWeights / 1000;

  if (path < stepwidth) {
    if (dEdx * path > E_kin)
      return E_kin;
    else
      return dEdx * path;
  }
  double dE = 0, tmp;
  while (path >= stepwidth) {
    tmp = dEdx * stepwidth;
    dE += tmp;
    gamma = (energy - dE) / mass;
    gamma2 = gamma * gamma;
    beta = sqrt(-(1. / gamma2 - 1.));
    beta2 = beta * beta;
    T_max = (2. * m_electron * beta2 * gamma2) / (1. + (2. * gamma * fraq) + (fraq * fraq));
    dEdx = 0;
    for (int i = 0; i < nComponents; i++)
      dEdx = weight[i] * c1 * c2[i] * charge2 / beta2 *
             (0.5 * log(c3[i] * beta2 * gamma2 * T_max) - beta2);
    dEdx = dEdx / sumOfWeights / 1000;
    path = path - stepwidth;
    if (dE >= E_kin)
      return E_kin;
  }
  dE += dEdx * path;
  if (dE >= E_kin)
    return E_kin;
  return dE;
}
float ABetheBloch::energyLoss(momentum4D particle, float path, float charge)
{
  return energyLoss(path, particle.Mass(), particle.Beta(), particle.Energy(), charge * charge);
}
float ABetheBloch::energyLoss_photon(int geantId, float energy, float path)
{
  // for light particles and photons:
  if (geantId < 7) {
    if (geantId > 1) {
      if (Eparticles::getMassG(geantId) / energy > 0.5)
        return energyLossE(geantId, energy, path);
      return energy * (1. - exp(path / radiationLength));
    }
    // photons have radiation length of 7/9th of the one of electrons
    return energy * (1. - exp((9. / 7.) * path / radiationLength));
  } else
    return energyLossE(geantId, energy, path);
}
float ABetheBloch::penetrationDepth(momentum4D particle, double charge)
{
  double beta = particle.Beta();
  double mass = particle.Mass();
  //  double charge=1;
  double gamma = particle.Gamma();
  double energy = particle.Energy();

  double beta2 = beta * beta;
  double charge2 = charge * charge;
  double gamma2 = gamma * gamma;
  double E_kin = energy - mass;
  float sumOfWeights = 0;
  for (int i = 0; i < nComponents; i++)
    sumOfWeights += weight[i];

  double T_max = 2. * m_electron /*c_light*c_light*/ * beta2 * gamma2 /
                 (1. + 2. * gamma * m_electron / mass + m_electron * m_electron / mass / mass);
  //  dEdx=(charge*charge*/(beta*beta))*k*Z/A*(0.5*log(2*m_electron*c*c*beta*beta*gamma*gamma*T_max/excitationE/excitationE)-beta*beta-densityModifications/2);
  // dEdx=(charge2*beta2)*constant1*(0.5*log(constant2*beta2*gamma2*T_max)-beta2-densityModifications/2.);
  double dEdx = 0;
  for (int i = 0; i < nComponents; i++)
    dEdx += weight[i] * c1 * c2[i] * charge2 / beta2 *
            (0.5 * log(2. * c3[i] * beta2 * gamma2 * T_max) - beta2);
  dEdx = dEdx / sumOfWeights;
  if (dEdx * stepwidth > E_kin)
    return E_kin / dEdx;
  double dE = 0, tmp, x = 0;
  while (E_kin > dE + dEdx * stepwidth) {
    tmp = dEdx * stepwidth;
    dE += tmp;
    gamma = (energy - dE) / mass;
    gamma2 = gamma * gamma;
    beta = sqrt(-(1. / gamma2 - 1.));
    beta2 = beta * beta;
    T_max = 2. * m_electron /*c_light*c_light*/ * beta2 * gamma2 /
            (1. + 2. * gamma * m_electron / mass + m_electron * m_electron / mass / mass);
    dEdx = 0;
    for (int i = 0; i < nComponents; i++)
      dEdx += weight[i] * c1 * c2[i] * charge2 / beta2 *
              (0.5 * log(2. * c3[i] * beta2 * gamma2 * T_max) - beta2);
    dEdx = dEdx / sumOfWeights;
    x += stepwidth;
  }
  return x + (E_kin - dE) / dEdx;
}

float ABetheBloch::betaAfter(float path, momentum4D before, float charge)
{
  //  if(penetrationDepth(before, charge)<path)return 0.;
  double de = energyLoss(path, before.Mass(), before.Beta(), before.Energy(), charge * charge);
  if (de >= before.Energy() - before.Mass())
    return 0.;
  double gamma = (before.Energy() - de) / before.Mass();
  return sqrt(-(1. / (gamma * gamma) - 1.));
}
float ABetheBloch::betaAfter(float path, int geantId, float betaIn)
{
  float charge = Eparticles::getChargeG(geantId);
  float mass = Eparticles::getMassG(geantId);
  float energy = mass / sqrt(1 - betaIn * betaIn);
  double de = energyLoss(path, mass, betaIn, energy, charge * charge);
  if (de >= energy - mass)
    return 0.;
  double gamma = (energy - de) / mass;
  return sqrt(-(1. / (gamma * gamma) - 1.));
}
AELossAlgorithm* ABetheBloch::getClone() const
{
  AELossAlgorithm* ret = new ABetheBloch(*this);
  return ret;
}
