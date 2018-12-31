#include "AGenerateEvents.h"
#include "Eparticles.h"
#include "logger.h"
AGenerateEvents::AGenerateEvents(TTrack** tracksIn, int& numberOfTracksIn, int& eventNumberIn,
                                 TSetup& setup, vector<int> IDs, string functionIn,
                                 vector<int> parameter, bool useSAIDIN, bool& validInIn)
    : AAlgorithm("Generate Events"), numberOfTracks(numberOfTracksIn), eventNumber(eventNumberIn),
      validIn(validInIn)
{
  useSAID = useSAIDIN;
  tracks = tracksIn;
  numberOfParticles = IDs.size();
  particleIDs = new int[numberOfParticles];
  for (int i = 0; i < numberOfParticles; i++)
    particleIDs[i] = IDs[i];
  double* masses = new double[numberOfParticles];
  for (int i = 0; i < numberOfParticles; i++)
    masses[i] = Eparticles::getMassG(particleIDs[i]);
  m2.setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
  m1 = setup.getBeam().getParticle();
  //   if(!setup.hasTarget())
  //     m2=setup.getBeam(1).getParticle();
  //   else
  //     m2=setup.getTarget().getParticle();
  TLorentzVector p1(m1.X(), m1.Y(), m1.Z(), m1.Energy()), p2(m2.X(), m2.Y(), m2.Z(), m2.Energy()),
      p = p1 + p2;
  if (!generator.SetDecay(p, numberOfParticles, masses)) {
    cout << "Warning!!!! decay kinematically forbidden!!!" << endl;
  }
  function = new TF1("saidFunction", functionIn.data(), 0, TMath::Pi());
  //  function=new
  //  TF1("saidFunction","0.518962*pow([0]-TMath::Pi()/2,6)+0.02661",0,TMath::Pi());//elastic
  //  p_beam=3.059Gev/c
  nParameters = parameter.size();
  parameters = new int[nParameters];
  for (int i = 0; i < nParameters; i++)
    parameters[i] = parameter[i];
  //  double par;
  int pN, parN;
  if (useSAID) {
    anaLog << "generate events: " << functionIn.data() << endli;
    anaLog << nParameters << " parameter" << endli;
    for (int i = 0; i < nParameters; i++) {
      //	  par=0;
      parN = parameters[i] / numberOfParticles;
      pN = parameters[i] % numberOfParticles;
      //	  cout<<pN<<":"<<parN<<endl;
      switch (parN) {
      case 0:
        anaLog << "Particle: " << pN << " Theta" << endli;
        break;
      case 1:
        anaLog << "Particle: " << pN << " Phi" << endli;
        break;
      case 2:
        anaLog << "Particle: " << pN << " Beta" << endli;
        break;
      case 3:
        anaLog << "Particle: " << pN << " Momentum" << endli;
        break;
      case 4:
        anaLog << "Particle: " << pN << " Pt" << endli;
        break;
      case 5:
        anaLog << "Particle: " << pN << " Pl" << endli;
        break;
      case 6:
        anaLog << "Particle: " << pN << " Theta_CM" << endli;
        break;
      case 7:
        anaLog << "Particle: " << pN << " Phi_CM" << endli;
        break;
      case 8:
        anaLog << "Particle: " << pN << " Beta_CM" << endli;
        break;
      case 9:
        anaLog << "Particle: " << pN << " Momenutm_CMP" << endli;
        break;
      }
    }
  }
  //  float x[25];//,y[25];
  // x[0]=30.000;y[0]=5.431/5.431;
  // x[1]=35.000;y[1]=2.881/5.431;
  // x[2]=40.000;y[2]=1.506/5.431;
  // x[3]=45.000;y[3]=0.8081/5.431;
  // x[4]=50.000;y[4]=0.4755/5.431;
  // x[5]=55.000;y[5]=0.3300/5.431;
  // x[6]=60.000;y[6]=0.2664/5.431;
  // x[7]=65.000;y[7]=0.2289/5.431;
  // x[8]=70.000;y[8]=0.1989/5.431;
  // x[9]=75.000;y[9]=0.1751/5.431;
  // x[10]=80.000;y[10]=0.1583/5.431;
  // x[11]=85.000;y[11]=0.1481/5.431;
  // x[12]=90.000;y[12]=0.1445/5.431;
  // x[13]=95.000;y[13]=0.1481/5.431;
  // x[14]=100.000;y[14]=0.1583/5.431;
  // x[15]=105.000;y[15]=0.1750/5.431;
  // x[16]=110.000;y[16]=0.1989/5.431;
  // x[17]=115.000;y[17]=0.2289/5.431;
  // x[18]=120.000;y[18]=0.2664/5.431;
  // x[19]=125.000;y[19]=0.3300/5.431;
  // x[20]=130.000;y[20]=0.4755/5.431;
  // x[21]=135.000;y[21]=0.8080/5.431;
  // x[22]=140.000;y[22]=1.506/5.431;
  // x[23]=145.000;y[23]=2.881/5.431;
  // x[24]=150.000;y[24]=5.430/5.431;
  /*root [175] TF1 func("func","[0]*pow(cos(x/[1]),10)+0.1445/5.431",0,180)
root [176] func.SetParameter(1,180)
root [177] func.SetParLimits(0,1,10000)
root [178] saidHisto.Fit(&func)
 FCN=0.0636271 FROM MIGRAD    STATUS=CONVERGED     184 CALLS         185 TOTAL
                     EDM=1.2932e-14    STRATEGY= 1      ERROR MATRIX ACCURATE
  EXT PARAMETER                                   STEP         FIRST
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
   1  p0           3.05121e+00   1.88045e+00   5.65090e-06  -1.92171e-06
   2  p1           5.73094e+01   2.92754e+00   1.25978e-03   5.89160e-08
(Int_t)0
*/

  //  graph=new TGraph(25,x,y);
  betaCM = momentum4D::CM(m1, m2).Velocity() * (-1);
  maxWeight = 0;
  double w;
  for (int i = 0; i < 100000; i++) {
    w = generator.Generate();
    if (w > maxWeight)
      maxWeight = w;
  }
  delete[] masses;
  eventNumber = -1;
}
AGenerateEvents::~AGenerateEvents() { delete[] particleIDs; }
#include <stdlib.h>
float AGenerateEvents::modifyWeight(momentum4D* parts)
{
  double par;
  int pN, parN;
  for (int i = 0; i < nParameters; i++) {
    par = 0;
    parN = parameters[i] / numberOfParticles;
    pN = parameters[i] % numberOfParticles;
    switch (parN) {
    case 0:
      par = parts[pN].Momentum().Theta();
      break; // theta
    case 1:
      par = parts[pN].Momentum().Phi();
      break; // phi
    case 2:
      par = parts[pN].Beta();
      break; // beta
    case 3:
      par = parts[pN].Momentum().R();
      break; // momentum
    case 4:
      par = parts[pN].Momentum().Rho();
      break; // pt
    case 5:
      par = parts[pN].Momentum().Z();
      break; // pl
    case 6:
      par = parts[pN + numberOfParticles].Momentum().Theta();
      break; // thetaCM
    case 7:
      par = parts[pN + numberOfParticles].Momentum().Phi();
      break; // phiCM
    case 8:
      par = parts[pN + numberOfParticles].Beta();
      break; // betaCM
    case 9:
      par = parts[pN + numberOfParticles].Momentum().R();
      break; // momentumCM
    }
    function->SetParameter(i, par);
  }
  return function->Eval(0);
}
void* AGenerateEvents::process(void* ptr)
{
  eventNumber++;
  momentum4D ps[numberOfParticles * 2];
  double weight = generator.Generate();
  weight = weight / maxWeight;
  double number = ((double)random()) / (double)RAND_MAX;
  // vector3D bCM=momentum4D::CM(m1,m2).Velocity();
  for (int i = 0; i < numberOfParticles; i++) {
    TLorentzVector v = (*(generator.GetDecay(i)));
    ps[i].setValues(v.Px(), v.Py(), v.Pz(), v.E());
    ps[i + numberOfParticles] = ps[i].boost(betaCM);
  }
  if (useSAID)
    weight = weight * modifyWeight(ps);
  while (
      weight <
      number) //||mm.Momentum().Theta()*180/M_PI>150||mm.Momentum().Theta()*180/M_PI<30||v1.Theta()>M_PI/3.0||v1.Theta()<0.17||v2.Theta()>M_PI/3.0||v2.Theta()<0.17)
  {
    weight = generator.Generate() / maxWeight;
    number = ((double)random()) / ((double)RAND_MAX);
    for (int i = 0; i < numberOfParticles; i++) {
      TLorentzVector v = (*(generator.GetDecay(i)));
      ps[i].setValues(v.Px(), v.Py(), v.Pz(), v.E());
      ps[i + numberOfParticles] = ps[i].boost(betaCM);
    }
    if (useSAID)
      weight = weight * modifyWeight(ps);
  }
  point3D vertex(0, 0, 0);
  numberOfTracks = numberOfParticles;
  bool used[numberOfParticles];
  for (int i = 0; i < numberOfParticles; i++)
    used[i] = false;
  int pos;
  for (int i = 0; i < numberOfParticles; i++) {
    pos = 0;
    for (int j = 1; j < numberOfParticles; j++) {
      if ((ps[j].Direction().Theta() < ps[pos].Direction().Theta() && (!used[j])) || used[pos])
        pos = j;
    }
    tracks[i]->setValid(true);
    tracks[i]->setOrder(false);
    tracks[i]->setPath(sLine3D(vertex, ps[pos].Direction()));
    tracks[i]->setSpeed(ps[pos].Velocity().length());
    tracks[i]->setKineticEnergy(ps[pos].Energy() - ps[pos].Mass());
    tracks[i]->setCharge((int)Eparticles::getChargeG(particleIDs[pos]));
    tracks[i]->getParticler() = ps[pos];
    used[pos] = true;
  }
  return ptr;
}
