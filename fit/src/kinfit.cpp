#include "fittingRoutines.h"
#include <stdlib.h>

kinFit::kinFit() : geomObject(), maxParticles(20), maxIteration(200), epsilon(__prec)
{
  in = new int*[maxParticles];
  for (int i = 0; i < maxParticles; i++)
    in[i] = new int[maxParticles + 5];
  particles = new vector3D[maxParticles];
  dpin = new vector3D[maxParticles];
  am = new float[maxParticles];
  ain = new float[maxParticles];
  p = new vector3D[maxParticles];
  nP = 0;
  nC = 0;
}
kinFit::kinFit(const kinFit& inFit)
    : geomObject(), maxParticles(inFit.MaxParticles()), maxIteration(inFit.MaxIterations()),
      epsilon(inFit.Epsilon())
{
  in = new int*[maxParticles];
  for (int i = 0; i < maxParticles; i++)
    in[i] = new int[maxParticles + 5];
  particles = new vector3D[maxParticles];
  dpin = new vector3D[maxParticles];
  am = new float[maxParticles];
  ain = new float[maxParticles];
  p = new vector3D[maxParticles];
  nP = 0;
  nC = 0;
}
kinFit::kinFit(int maxIterationIn, int maxParticlesIn, float epsilonIn)
    : geomObject(), maxParticles(maxParticlesIn), maxIteration(maxIterationIn),
      epsilon(fabs(epsilonIn))
{
  in = new int*[maxParticles];
  for (int i = 0; i < maxParticles; i++)
    in[i] = new int[maxParticles + 5];
  particles = new vector3D[maxParticles];
  dpin = new vector3D[maxParticles];
  am = new float[maxParticles];
  ain = new float[maxParticles];
  p = new vector3D[maxParticles];
  nP = 0;
  nC = 0;
}
kinFit::~kinFit()
{
  for (int i = 0; i < maxParticles; i++)
    delete[] in[i];
  delete[] in;
  delete[] particles;
  delete[] dpin;
  delete[] am;
  delete[] ain;
  delete[] p;
}
float kinFit::Epsilon() const { return epsilon; }
void kinFit::setEpsilon(float value) { epsilon = fabs(value); }
int kinFit::NParticles() const { return nP; }
int kinFit::MaxIterations() const { return maxIteration; }
int kinFit::MaxParticles() const { return maxParticles; }
void kinFit::clear() { nP = 0; }
void kinFit::add(const Vector& data)
{
  if (data.getDimension() < 7)
    return;
  if (nP >= maxParticles)
    return;
  particles[nP].setValues(data.getValue(0), data.getValue(1), data.getValue(2));
  dpin[nP].setValues(data.getValue(3), data.getValue(4), data.getValue(5));
  am[nP] = data.getValue(6);
  for (int i = 0; i < nC + 5; i++)
    if (i + 8 > data.getDimension())
      in[nP][i] = 0;
    else
      in[nP][i] = (int)data.getValue(i + 7);
  nP++;
}
void kinFit::add(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                 float mass, int representation, int in_out_going, int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  particles[nP] = properties;
  dpin[nP] = errors;
  am[nP] = mass;
  for (int i = 0; i < 3; i++)
    in[nP][i] = (int)doFit[i];
  in[nP][3] = representation;
  in[nP][4] = in_out_going;
  if (additionalConstraints != NULL)
    for (int i = 0; i < nC; i++)
      in[nP][i + 5] = additionalConstraints[i];
  else
    for (int i = 0; i < nC; i++)
      in[nP][i + 5] = 0;
  nP++;
}
void kinFit::addMomentum(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                         float mass, int in_out_going, int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 1, in_out_going, additionalConstraints);
  //   cout<<"added Px/Py/Pz :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   1,mass).toString(_cartesic_+4).data()<<endl;
}
void kinFit::addEnergyThetaPhi(const vector3D& properties, const vector3D& errors,
                               const vector3D& doFit, float mass, int in_out_going,
                               int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 2, in_out_going, additionalConstraints);
  //   cout<<"added E/th/ph :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   2,mass).toString(_spheric_+8).data()<<endl;
}
void kinFit::addBetaThetaPhi(const vector3D& properties, const vector3D& errors,
                             const vector3D& doFit, float mass, int in_out_going,
                             int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 3, in_out_going, additionalConstraints);
  //   cout<<"added ß/th/ph :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   3,mass).toString(_spheric_+12).data()<<endl;
}
void kinFit::addMomentumThetaPhi(const vector3D& properties, const vector3D& errors,
                                 const vector3D& doFit, float mass, int in_out_going,
                                 int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 4, in_out_going, additionalConstraints);
  //   cout<<"added P/th/ph :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   4,mass).toString(_spheric_+4).data()<<endl;
}
void kinFit::addEnergyDxDy(const vector3D& properties, const vector3D& errors,
                           const vector3D& doFit, float mass, int in_out_going,
                           int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 5, in_out_going, additionalConstraints);
  //   cout<<"added E/dx/dy :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   5,mass).toString(_spheric_+4).data()<<endl;
}
void kinFit::addBetaDxDy(const vector3D& properties, const vector3D& errors, const vector3D& doFit,
                         float mass, int in_out_going, int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 7, in_out_going, additionalConstraints);
  //   cout<<"added ß/dx/dy :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   7,mass).toString(_spheric_+12).data()<<endl;
}
void kinFit::addMomentumDxDy(const vector3D& properties, const vector3D& errors,
                             const vector3D& doFit, float mass, int in_out_going,
                             int* additionalConstraints)
{
  if (nP >= maxParticles)
    return;
  add(properties, errors, doFit, mass, 6, in_out_going, additionalConstraints);
  //   cout<<"added P/dx/dy :"<<properties<<" error:"<<errors<<",
  //   :"<<getMomentum(properties,
  //   6,mass).toString(_spheric_+4).data()<<endl;
}

vector3D kinFit::Errors(int num) const
{
  if (num < 0 || num >= nP)
    return vector3D();
  return dpin[num];
}
vector3D kinFit::NonFittedParticle(int num) const
{
  if (num < 0 || num >= nP)
    return vector3D();
  return particles[num];
}
float kinFit::Mass(int num) const
{
  if (num < 0 || num >= nP)
    return -1;
  return am[num];
}
int kinFit::Constraint(int numP, int numC) const
{
  if (numP < 0 || numP >= nP)
    return 0;
  if (numC < 0 || numC >= nC)
    return 0;
  return in[numP][numC + 5];
}
int kinFit::Representation(int num) const
{
  if (num >= nP || num < 0)
    return -1;
  return in[num][3];
}
void kinFit::setConstraints(int nCin, Vector masses)
{
  if (nCin >= maxParticles)
    return;
  if (masses.getDimension() < nCin)
    return;
  nC = nCin;
  for (int i = 0; i < nC; i++)
    ain[i] = masses.getValue(i);
}
float kinFit::InvariantMass(int constraint) const
{
  if (constraint < 0 || constraint >= nC)
    return -1;
  return ain[constraint];
}
int kinFit::Constraints() const { return nC; }
float kinFit::Chi2() const { return chi; }
int kinFit::OutInfo() const { return outCode; }
Vector kinFit::Result() const
{
  Vector ret(nP * 3);
  for (int i = 0; i < nP; i++) {
    ret.setValue(i * 3 + 0, p[i].getValue(0));
    ret.setValue(i * 3 + 1, p[i].getValue(1));
    ret.setValue(i * 3 + 2, p[i].getValue(2));
  }
  return ret;
}
vector3D kinFit::FittedParticle(int num) const
{
  if (num < 0 || num >= nP)
    return vector3D();
  return p[num];
}
float kinFit::fit()
{
  float chi2 = 10000;
  float d = 10000; //
  momentum4D ppx[nP];

  // copy particles
  for (int i = 0; i < nP; i++) {
    p[i] = particles[i];
    //    cout<<am[i]<<getMomentum(i,p[i]).toString(4).data()<<endl;
    // cout<<p[i]<<in[i][3]<<" "<<in[i][4]<<getMomentum(i,p[i])<<endl;
  }

  int nmea = 0, numea = 0;

  for (int i = 0; i < nP; i++)
    for (int j = 0; j < 3; j++)
      if (in[i][j] == 1)
        nmea++;
      else if (in[i][j] == 3)
        numea++;

  // define working structures
  matrixNxM BM(4 + nC, nmea), BU(4 + nC, numea); // matrices containing derivatives
  matrixNxM GB(4 + nC, 4 + nC);
  matrixNxM GB_1(4 + nC, 4 + nC);
  matrixNxM GM(nmea, nmea), GU(numea, numea); // error matrix and weighting matrix !diagonal!
  matrixNxM GM_1(nmea, nmea);                 // GU_1(numea,numea);
  matrixNxM GMIN(nmea, nmea);                 // if useCovariant own covariant matrix
  matrixNxM G1(numea, numea);
  matrixNxM* matPoint;
  Vector epsM(nmea), epsU(numea); // corrections to the variables to be introduced by the fit
  Vector FM(4 + nC), FI(4 + nC), FM0(4 + nC); //
  Vector al(4 + nC);                          // lagrange multipliers
  fm.reDimension(4 + nC);
  GU.Unity();
  al.transpose();
  FM.transpose();
  FI.transpose();
  FM0.transpose();
  epsM.transpose();
  epsU.transpose();
  for (int i = 0; i < nmea; i++)
    epsM.setValue(i, 0);
  for (int i = 0; i < numea; i++)
    epsU.setValue(i, 0);
  Vector sf[nC];
  for (int i = 0; i < nC; i++)
    sf[i].reDimension(5);

  // index saving
  int Meas[nmea], Unmeas[numea];
  //   bool condition=true;
  int iteration = 0;
  float /*chi2_prev=0,*/ d_prev /*,d0*/;
  int num, pos, number, *mmtmp;
  int sig;
  momentum4D tmp4d;
  // init measured-/unmeasured variable holders
  nmea = 0;
  numea = 0;
  for (int i = 0; i < nP; i++)
    for (int j = 0; j < 3; j++)
      if (abs(in[i][j]) == 1) {
        Meas[nmea++] = i * 10 + j;
      } else if (abs(in[i][j]) == 3) {
        Unmeas[numea++] = i * 10 + j;
      }
  const int nMeasured = nmea;
  const int nUnmeasured = numea;
  for (int i = 0; i < nMeasured; i++)
    epsM.setValue(i, 0);
  for (int i = 0; i < nUnmeasured; i++)
    epsU.setValue(i, 0);
  // init g-matrices
  GM.Unity();
  GM_1.Unity();
  for (int i = 0; i < nMeasured; i++)
    for (int j = 0; j < nMeasured; j++)
      GM.setValue(i, j, 0);
  if (false) // useCovariant)
  {
    for (int i = 0; i < nMeasured; i++)
      for (int j = 0; j < nMeasured; j++)
        GM.setValue(i, j, GMIN.getValue(((Meas[i] / 10)) * 3 + (int)(Meas[i] % 10),
                                        ((Meas[j] / 10)) * 3 + (int)(Meas[j] % 10)));
  } else {
    for (int i = 0; i < nMeasured; i++) {
      GM_1.setValue(i, i, dpin[Meas[i] / 10].getValue((int)(Meas[i] % 10)) *
                              dpin[Meas[i] / 10].getValue((int)(Meas[i] % 10)));
      GM.setValue(i, i, 1. / dpin[Meas[i] / 10].getValue((int)(Meas[i] % 10)) * 1. /
                            dpin[Meas[i] / 10].getValue((int)(Meas[i] % 10)));
    }
  }

  calculateCinematics(ppx, FM, sf, d);
  FM0 = FM;
  //  d0=d;
  //   cout<<"\nStart:"<<d0<<FM<<d<<endl;
  // iteration
  //   float value;
  for (iteration = 0; iteration <= maxIteration + 1; iteration++) {
    // cout<<iteration<<" "<<maxIteration<<endl;
    //  while(condition)
    //       if(printIterationInfo)
    // cout<<"iteration: "<<iteration<<endl;
    // derivative matrices for measured and unmeasured variables. looks
    // principally alike:
    for (int l = 0; l < 2; l++) {
      if (l == 1) // calculate Matrix with derivatives BM (info x nMeasured) for
                  // measured Variables
      {
        matPoint = &BM;
        number = nMeasured;
        mmtmp = Meas;
      } else // calculate Matrix with derivatives BU (info x nUnmeasured) for
             // unmeasured variables
      {
        matPoint = &BU;
        number = nUnmeasured;
        mmtmp = Unmeas;
      }
      for (int i = 0; i < number; i++) {
        num = mmtmp[i] / 10; // particle number
        pos = mmtmp[i] % 10; // property number
        tmp4d = getPDerivative(in[num][3], pos, p[num], am[num]);
        if (in[num][4] == 0)
          sig = 0;
        else if (in[num][4] < 1)
          sig = -1;
        else
          sig = 1;
        for (int j = 0; j < 4; j++)
          matPoint->setValue(j, i, sig * tmp4d[j]);
        for (int j = 0; j < nC; j++) {
          if (in[num][j + 5] != 0) {
            matPoint->setValue(j + 4, i, getMDerivative(in[num][3], pos, p[num], am[num], sf[j]));
          } else
            matPoint->setValue(j + 4, i, 0);
        }
      }
    }
    // cout<<iteration<<" a"<<endl;
    // calculate GB matrix
    GB = BM * ((GM_1) * !BM); // GM^-1???
    GB_1 = -GB;
    //       cout<<GB<<GB.Diagonal()<<endl;
    //       if(abs((GB_1*GB).Determinant())>2)cout<<"!!!!\n"<<GB_1<<GB<<GB_1*GB<<endl;
    FI = FM - (BM * epsM);
    // cout<<iteration<<" b"<<endl;
    if (FI.isLine())
      FI.transpose();
    if (nUnmeasured > 0) {
      // G1 matrix
      FI = FM + BM * epsM + BU * epsU;
      if (FI.isLine())
        FI.transpose();
      G1 = !BU * GB_1;
      epsU = (((-(GU + (G1 * BU))) * G1) * FI);
      epsU = epsU * -1;
      al = (GB_1 * ((BU * epsU + FI)));
    } else
      al = GB_1 * FI;
    // calculate epsilon for measured
    // cout<<iteration<<" c"<<endl;
    epsM = ((GM_1) * ((!BM) * al)) * -1;
    // finalize: calculate fitted variables, and chi^2
    int n, m;
    // cout<<iteration<<" d"<<endl;
    for (int i = 0; i < nUnmeasured; i++) {
      n = Unmeas[i] / 10;
      m = Unmeas[i] % 10;
      p[n].setValue(m, particles[n].getValue(m) + epsU.getValue(i));
    }
    vector3D ppp = p[0];
    // cout<<iteration<<" e"<<endl;
    for (int i = 0; i < nMeasured; i++) {
      n = Meas[i] / 10;
      m = Meas[i] % 10;
      p[n].setValue(m, particles[n].getValue(m) + epsM.getValue(i));
    }
    // cout<<iteration<<" f"<<endl;
    for (int i = 0; i < nP; i++)
      checkValues(in[i][3], p[i], abs(in[i][0]) != 2, abs(in[i][1]) != 2, abs(in[i][2]) != 2);

    // cout<<iteration<<" g"<<endl;
    //      chi2_prev=chi2;
    // cout<<iteration<<" h"<<endl;
    chi2 = epsM * ((GM)*epsM);
    // cout<<iteration<<" i"<<endl;
    if (nUnmeasured > 0)
      chi2 = epsM * ((GM)*epsM) + epsU * (GU * epsU) + 2 * al * (FI + BM * epsM + BU * epsU);
    // cout<<iteration<<" j"<<endl;
    // calculate FM kinematic
    d_prev = d;
    // cout<<iteration<<" k"<<endl;
    calculateCinematics(ppx, FM, sf, d);
    //       cout<<iteration<<"<"<<maxIteration<<":"<<FM<<" \t"<<d<<"
    //       "<<chi2<<endl;
    //      iteration++;
    // calculate iteration condition
    // cout<<iteration<<" l"<<endl;
    fm = FM;
    d_last = d;
    if ((d < epsilon)) //||(abs(chi2-chi2_prev)<0.000001))//||chi2<1)
    {
      outCode = iteration;
      //	  cout<<"well done "<<iteration<<" "<<outCode<<"
      //"<<d_last<<endl;
      break;
    }
    if (d > d_prev) {
      // convergence wrong
    }
    if (iteration > maxIteration) {
      outCode = iteration;
      outCode = outCode * -1;
      //	  cout<<"maximum number of iterations exceeded "<<iteration<<"
      //"<<outCode<<"
      //"<<d_last<<endl;
      break;
    }
  }
  chi = chi2;
  return (chi2);
}
void kinFit::checkValues(int rep, vector3D& pm, bool m1, bool m2, bool m3)
{
  float th = pm.Y(), ph = pm.Z(), kin = pm.X();
  float dx = pm.Y(), dy = pm.Z();
  if (abs(rep) > 1 && abs(rep) < 4) // theta phi
  {
    int n;
    if (m2) {
      if (!(th < 0 || th > 0))
        th = 0;
      if (th < 0 || th > M_PI) {
        if (th < 0)
          th = abs(th);
        if (th > M_PI) {
          n = (int)abs((th - M_PI) / M_PI);
          th = (th - M_PI * (n + 1));
        }
      }
    }
    if (m3) {
      if ((!(ph < 0)) && (!(ph > 0)))
        ph = 0;
      if (ph < -M_PI) {
        n = (int)abs((ph + M_PI) / (2 * M_PI));
        ph = ph + M_PI * (n + 1);
      }
      if (ph > M_PI) {
        n = (int)abs((ph - M_PI) / (2 * M_PI));
        ph = ph - M_PI * (n + 1);
      }
    }
  }
  if (m1) {
    if (abs(rep) > 1 && abs(rep) < 5) // energy, momentum and beta are absolute values
      if (kin < 0)
        kin = 0.001;
    if (abs(rep) == 3 || abs(rep) == 7) // beta <1
      if (abs(kin) > 1)
        kin = (kin > 0 ? 1 : -1) * 0.9995;
  }
  switch (rep) {
  case 1:
    return;
  case 2:
    pm.setValues(kin, th, ph);
    return;
  case 3:
    pm.setValues(kin, th, ph);
    return;
  case 4:
    pm.setValues(kin, th, ph);
    return;
  case 5:
    pm.setValues(kin, dx, dy);
    return;
  case 7:
    pm.setValues(kin, dx, dy);
    return;
  }
}
void kinFit::calculateCinematics(momentum4D* ppx, Vector& FM, Vector* sf, float& d)
{
  int sig = 0;
  for (int i = 0; i < nP; i++) {
    ppx[i] = getMomentum(i, p[i]);
  }
  momentum4D tmpm4d(0, 0, 0, 0);
  for (int i = 0; i < nP; i++) {
    if (in[i][4] == 0)
      continue;
    if (in[i][4] < 0)
      sig = -1;
    else
      sig = 1;
    tmpm4d = tmpm4d + (ppx[i] * sig);
  }
  FM.setValues(tmpm4d[0], tmpm4d[1], tmpm4d[2], tmpm4d[3]);
  //   for(int i=0;i<4;i++)
  //     FM.setValue(i,tmpm4d[i]);
  float sum;
  // calculate FM additional constraints
  for (int i = 0; i < nC; i++) {
    sf[i].setValues(0, 0, 0, 0, 0);
    for (int j = 0; j < nP; j++) {
      if (in[j][i + 5] == 0)
        sig = 0;
      else if (in[j][i + 5] < 1)
        sig = -1;
      else
        sig = 1;
      for (int k = 0; k < 4; k++)
        sf[i].setValue(k, sf[i][k] + sig * ppx[j][k]);
    }
    sum = 0;
    for (int j = 0; j < 3; j++)
      sum += sf[i][j] * sf[i][j];
    sum = sf[i][3] * sf[i][3] - sum;
    if (sum <= 0)
      sum = 0.001;
    sf[i].setValue(4, sqrt(sum));
    FM.setValue(i + 4, sf[i][4] - ain[i]);
  }
  d = sqrt(FM * FM);
}

vector4D kinFit::getPDerivative(int isIn, int pos, vector3D pm, float mass)
{
  vector4D ret(0, 0, 0, 0);
  float tmpf;
  float th = pm.Y(), ph = pm.Z(), kin = pm.X();
  float dx = pm.Y(), dy = pm.Z();
  if (abs(isIn) > 1 && abs(isIn) < 5) // energy, momentum and beta are absolute values
    if (kin < 0)
      kin = 0.001;
  if (abs(isIn) == 3 || abs(isIn) == 7) // beta <1
    if (abs(kin) > 1)
      kin = (kin > 0 ? 1 : -1) * 0.9995;
  switch (abs(isIn)) {
  case 1: // px,py,pz
    ret.setValues((pos == 0 ? 1 : 0), (pos == 1 ? 1 : 0), (pos == 2 ? 1 : 0),
                  pm.getValue(pos) / sqrt(pm.R() * pm.R() + mass * mass));
    break;
  case 2: // E,th,ph
  {
    tmpf = sqrt((kin + 2 * mass) * kin);
    switch (pos) {
    case 0:
      ret.setValues((kin + mass) / tmpf * (sin(th) * cos(ph)),
                    (kin + mass) / tmpf * (sin(th) * sin(ph)), (kin + mass) / tmpf * (cos(th)), 1);
      break;
    case 1:
      ret.setValues(tmpf * (cos(th) * cos(ph)), tmpf * (cos(th) * sin(ph)), -tmpf * (sin(th)), 0);
      break;
    case 2:
      ret.setValues(-tmpf * (sin(th) * sin(ph)), tmpf * (sin(th) * cos(ph)), 0, 0);
      break;
    }
    break;
  }
  case 3: // v,th,phi
  {
    float g = 1 / sqrt(1 - kin * kin);
    float p = mass * kin * g;
    tmpf = g * mass * (1 + kin * kin * g * g);
    switch (pos) {
    case 0:
      ret.setValues(tmpf * (sin(th) * cos(ph)), tmpf * (sin(th) * sin(ph)), tmpf * (cos(th)),
                    mass * kin * g * g * g);
      break;
    case 1:
      ret.setValues(p * (cos(th) * cos(ph)), p * (cos(th) * sin(ph)), -p * (sin(th)), 0);
      break;
    case 2:
      ret.setValues(-p * (sin(th) * sin(ph)), p * (sin(th) * cos(ph)), 0, 0);
      break;
    }
    break;
  }
  case 4: // p,th,ph
  {
    switch (pos) {
    case 0:
      ret.setValues(sin(th) * cos(ph), sin(th) * sin(ph), cos(th),
                    kin / sqrt(kin * kin + mass * mass));
      break;
    case 1:
      ret.setValues(kin * cos(th) * cos(ph), kin * cos(th) * sin(ph), -kin * sin(th), 0);
      break;
    case 2:
      ret.setValues(-kin * sin(th) * sin(ph), kin * sin(th) * cos(ph), 0, 0);
      break;
    }
    break;
  }
  case 5: // E,dx,dy
  {
    tmpf = sqrt((kin + 2 * mass) * kin);
    float l = sqrt(dx * dx + dy * dy + 1);
    switch (pos) {
    case 0:
      ret.setValues((kin + mass) / tmpf * (dx / l), (kin + mass) / tmpf * (dy / l),
                    (kin + mass) / tmpf * (1 / l), 1);
      break;
    case 1:
      ret.setValues(tmpf * (l * l - dx * dx) / l / l / l, -tmpf * (dx * dy / l / l / l),
                    -tmpf * (dx / l / l / l), 0);
      break;
    case 2:
      ret.setValues(-tmpf * (dx * dy / l / l / l), tmpf * (l * l - dy * dy) / l / l / l,
                    -tmpf * (dy / l / l / l), 0);
      break;
    }
    ret.setValue(3, (pos == 0 ? 1 : 0));
    break;
  }
  case 6: // p,dx,dy
  {
    tmpf = sqrt(kin * kin + mass * mass);
    float l = sqrt(dx * dx + dy * dy + 1);
    switch (pos) {
    case 0:
      ret.setValues((dx / l), dy / l, (1 / l), kin / tmpf);
      break;
    case 1:
      ret.setValues(kin * (l * l - dx * dx) / l / l / l, -kin * (dx * dy / l / l),
                    -kin * (pm.Y() / l / l / l), 0);
      break;
    case 2:
      ret.setValues(-kin * (dx * dy / l / l / l), kin * (l * l - dy * dy) / l / l / l,
                    -kin * (dy / l / l / l), 0);
      break;
    }
    break;
  }
  case 7: // b,dx,dy
  {
    float g = 1 / sqrt(1 - kin * kin);
    float p = mass * kin * g;
    tmpf = g * mass * (1 + kin * kin * g * g);
    float l = sqrt(dx * dx + dy * dy + 1);
    switch (pos) {
    case 0:
      ret.setValues(tmpf * (dx / l), tmpf * (dy / l), tmpf * (1 / l), mass * kin / g / g / g);
      break;
    case 1:
      ret.setValues(p * (l * l - dx * dx) / l / l / l, -p * (dx * dy / l / l / l),
                    -p * (dx / l / l / l), 0);
      break;
    case 2:
      ret.setValues(-p * (dx * dy / l / l / l), p * (l * l - dy * dy) / l / l / l,
                    -p * (dy / l / l / l), 0);
      break;
    }
    break;
  }
  }
  return ret;
}
float kinFit::getMDerivative(int representation, int pos, vector3D pm, float mass, Vector sf)
{
  // Derivative of invariant mass with respect to the particle in a specific
  // representation and its
  // component pos:
  // d(Minv)/d(prop) = 1/Minv*(E_sum* d(E_part)/d(prop) - SUM_i(P_sum_i *
  // d(P_i)/d(prop)))
  float sum = 0;
  momentum4D der = getPDerivative(representation, pos, pm, mass);
  for (int i = 0; i < 3; i++)
    sum += sf[i] * der[i];
  return (der[3] * sf[3] - sum) / sf[4];
}
momentum4D kinFit::getMomentum(int num, vector3D pm)
{
  momentum4D ret;
  float th = pm.Y(), ph = pm.Z(), kin = pm.X();
  float dx = pm.Y(), dy = pm.Z();
  if (abs(in[num][3]) > 1 && abs(in[num][3]) < 5) // energy, momentum and beta are absolute values
    if (kin < 0)
      kin = 0.001;
  if (abs(in[num][3]) == 3 || abs(in[num][3]) == 7) // beta <1
    if (abs(kin) > 1)
      kin = (kin > 0 ? 1 : -1) * 0.9995;
  switch (abs(in[num][3])) {
  case 1:
    ret.setPM(pm, am[num]);
    break;
  case 2: // E,th,ph
  {
    float P = sqrt(kin * (kin + 2 * am[num]));
    ret.setPM(vector3D(P * sin(th) * cos(ph), P * sin(th) * sin(ph), P * cos(th)), am[num]);
    break;
  }
  case 3: // v,th,ph
  {
    ret.setVM(kin * vector3D(cos(ph) * sin(th), sin(ph) * sin(th), cos(th)), am[num]);
    break;
  }
  case 4: // p,th,ph
  {
    ret.setPM(kin * vector3D(cos(ph) * sin(th), sin(ph) * sin(th), cos(th)), am[num]);
    break;
  }
  case 5: // E,dx,dy
  {
    float P = sqrt(kin * (kin + 2 * am[num]));
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), am[num]);
    break;
  }
  case 6: // p,dx,dy
  {
    float P = kin;
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), am[num]);
    break;
  }
  case 7: // b,dx,dy
  {
    float P = am[num] * kin / sqrt(1 - kin * kin); // p=m*ß/sqrt(1-ß²)=m ß gamma
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), am[num]);
    break;
  }
  }
  return ret;
}
momentum4D kinFit::getMomentum(vector3D pm, int rep, float mass)
{
  momentum4D ret;
  float th = pm.Y(), ph = pm.Z(), kin = pm.X();
  float dx = pm.Y(), dy = pm.Z();
  if (abs(rep) > 1 && abs(rep) < 5) // energy, momentum and beta are absolute values
    if (kin < 0)
      kin = 0.001;
  if (abs(rep) == 3 || abs(rep) == 7) // beta <1
    if (abs(kin) > 1)
      kin = (kin > 0 ? 1 : -1) * 0.9995;
  switch (rep) {
  case 1:
    ret.setPM(pm, mass);
    break;
  case 2: // E,th,ph
  {
    float P = sqrt(kin * (kin + 2 * mass));
    ret.setPM(P * vector3D(cos(ph) * sin(th), sin(ph) * sin(th), cos(th)), mass);
    break;
  }
  case 3: // v,th,ph
  {
    ret.setVM(pm.X() * vector3D(cos(ph) * sin(th), sin(ph) * sin(th), cos(th)), mass);
    break;
  }
  case 4: // p,th,ph
  {
    ret.setPM(kin * vector3D(cos(ph) * sin(th), sin(ph) * sin(th), cos(th)), mass);
    break;
  }
  case 5: // E,dx,dy
  {
    float P = sqrt(kin * (kin + 2 * mass));
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), mass);
    break;
  }
  case 6: // p,dx,dy
  {
    float P = kin;
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), mass);
    break;
  }
  case 7: //ß,dx,dy
  {
    float P = mass * kin / sqrt(1 - kin * kin); // p=m*ß/sqrt(1-ß²)=m ß gamma
    float leng = sqrt(dx * dx + dy * dy + 1);
    ret.setPM((P / leng) * vector3D(dx, dy, 1), mass);
    break;
  }
  }
  return ret;
}
Vector kinFit::FM_last() const { return fm; }
float kinFit::D_last() const { return d_last; }
int kinFit::KnownRepresentations(int** buffer)
{
  if (buffer == NULL)
    return 7;
  (*buffer) = new int[7];
  (*buffer)[0] = 1;
  (*buffer)[1] = 2;
  (*buffer)[2] = 3;
  (*buffer)[3] = 4;
  (*buffer)[4] = 5;
  (*buffer)[5] = 6;
  (*buffer)[6] = 7;
  return 7;
}
int kinFit::KnownRepresentation(int num)
{
  if (num < 0 || num >= 7)
    return -1;
  switch (num) {
  case 0:
    return 1;
  case 1:
    return 2;
  case 2:
    return 3;
  case 3:
    return 4;
  case 4:
    return 5;
  case 5:
    return 6;
  case 6:
    return 7;
  }
  return -1;
}
