#include "AGenerateEventsWithDecay.h"
#include "Eparticles.h"
#include "algorithmparameter.h"
#include <iostream>
#include <stdlib.h>
float AGenerateEventsWithDecay::modifyWeight(int decay)
{
  if (PhaseSpace)
    return 1;
  return function[decay]->Eval(0);
}
void AGenerateEventsWithDecay::setParameter(momentum4D* parts)
{
  if (PhaseSpace)
    return;
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
    for (int j = 0; j < numberOfDecays; j++) {
      function[j]->SetParameter(i, par);
    }
  }
}

bool AGenerateEventsWithDecay::accept(sLine3D* trajectories)
{
  if (!acceptance)
    return true;
  volumeShape* shape;
  int nHit;
  int susp[setup.getNumberOfDetectors()];
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    if (requiredHits[i] == 0)
      continue;
    if (setup.getDetectorr(i).getNumberOfElements() <= 0)
      continue;
    shape = setup.getDetectorr(i).getShape(0);
    if (shape == NULL)
      continue;
    nHit = 0;
    for (int j = 0; j < numberOfParticles; j++) {
      susp[i] = shape->suspect(trajectories[j], setup.getDetectorr(i).getStackType());
      if (requiredHits[i] > 0 && charge[j] == 0)
        continue;
      if (susp[i] >= 0 && susp[i] < static_cast<int>(setup.getDetectorr(i).getNumberOfElements()))
        nHit++;
    }
    if (nHit < abs(requiredHits[i]))
      return false;
  }
  for (int i = 0; i < numSums; i++) {
    nHit = 0;
    for (int j = 0; j < numSumDet[i]; j++)
      if (susp[SumDetID[i][j]] >= 0 && (requiredHits[i] < 0 || charge[j] != 0))
        nHit++;
    if (nHit < SumDetHits[i])
      return false;
  }
  return true;
}

algorithm_parameter AGenerateEventsWithDecay::getDescription()
{
  algorithm_parameter ret("Generate Events with decay", 0, 0);
  vector<string> des;
  des.push_back("This algorithm generates events with the ROOT");
  des.push_back("Phase-Space-Generator. You specify the generated ");
  des.push_back("particles by their GEANT-ID and which of these");
  des.push_back("particles decayed off primary ones. A decay parameter");
  des.push_back("of -1 means a primary particle, otherwise it decayed");
  des.push_back("off the particle with the specified number. For each");
  des.push_back("decay a modification from phase-space can be given.");
  des.push_back("There can be given some trigger conditions for some");
  des.push_back("tracks (charged (+) or charged+uncharged(-)) to ");
  des.push_back("simulate acceptance.");
  ret.setDescription(des);
  vector<int> tmp;
  vector<string> tmps;
  ret.addParam<vector<int>>(single_parameter<vector<int>>("particles", tmp));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("decayed off", tmp));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("required Hits in detector", tmp));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("required Hits in OR- detector", tmp));
  ret.addParam<vector<string>>(single_parameter<vector<string>>("functions for decay", tmps));
  ret.addParam<vector<int>>(single_parameter<vector<int>>("parameters for functions", tmp));
  ret.addParam<bool>(single_parameter<bool>("use Phase-Space", true));
  ret.addParam<bool>(single_parameter<bool>("simulate acceptance", false));
  return ret;
}
AGenerateEventsWithDecay::AGenerateEventsWithDecay(TTrack** tracksIn, int& numberOfTracksIn,
                                                   int& eventNumberIf, TSetup& setupIn,
                                                   bool& validInputIn,
                                                   const algorithm_parameter& descr)
    : AAlgorithm(""), setup(setupIn), numberOfTracks(numberOfTracksIn), eventNumber(eventNumberIf),
      validInput(validInputIn)
{
  tracks = tracksIn;
  valid = true;
  if (!Eparticles::IsInit())
    Eparticles::init();
  PhaseSpace = descr.getParam<bool>(0).getData();
  acceptance = descr.getParam<bool>(1).getData();
  numberOfParticles = descr.getParam<vector<int>>(0).getData().size();
  particleID = new int[numberOfParticles];
  decayedOff = new int[numberOfParticles];
  masses = new double[numberOfParticles];
  halfLife = new double[numberOfParticles];
  charge = new int[numberOfParticles];
  bool decays[numberOfParticles];
  bool pri[numberOfParticles];
  for (int i = 0; i < numberOfParticles; i++) {
    particleID[i] = descr.getParam<vector<int>>(0).getData().at(i);
    decayedOff[i] = descr.getParam<vector<int>>(1).getData().at(i);
    masses[i] = Eparticles::getMassG(particleID[i]);
    halfLife[i] = log(2.) * Eparticles::getLifeTimeG(particleID[i]);
    charge[i] = (int)Eparticles::getChargeG(particleID[i]);
    decays[i] = false;
    pri[i] = true;
  }
  for (int i = 0; i < numberOfParticles; i++)
    if (decayedOff[i] < numberOfParticles && decayedOff[i] >= 0) {
      decays[decayedOff[i]] = true;
      pri[i] = false;
    }
  bool generated[numberOfParticles];
  for (int i = 0; i < numberOfParticles; i++)
    generated[i] = false;
  numberOfPrimary = 0;
  for (int i = 0; i < numberOfParticles; i++)
    if (pri[i])
      numberOfPrimary++;
  Double_t mass[numberOfPrimary];
  primaries = new int[numberOfPrimary];
  numberOfPrimary = 0;
  for (int i = 0; i < numberOfParticles; i++)
    if (pri[i]) {
      generated[i] = true;
      primaries[numberOfPrimary] = i;
      mass[numberOfPrimary] = masses[i];
      numberOfPrimary++;
    }
  numberOfDecays = 0;
  for (int i = 0; i < numberOfParticles; i++)
    if (decays[i])
      numberOfDecays++;
  numDecayParticles = new int[numberOfDecays];
  decayParticle = new int[numberOfDecays];
  decayIDs = new int*[numberOfDecays];
  generator = new TGenPhaseSpace*[numberOfDecays + 1];
  for (int i = 0; i < numberOfDecays + 1; i++)
    generator[i] = NULL;
  function = new TF1*[numberOfDecays + 1];
  generator[0] = new TGenPhaseSpace();
  TLorentzVector pI(0, 0, 0, 0);
  cout << "beam: " << setup.getBeamr().getParticle() << endl;
  cout << "target: " << setup.getTargetr().getParticle() << endl;
  momentum4D p1 = setup.getBeamr().getParticle(), p2 = setup.getTargetr().getParticle();
  momentum4D p = p1 + p2;
  pI.SetPxPyPzE(p.X(), p.Y(), p.Z(), p.Energy());
  // cout<<p<<endl;
  //  p=momentum4D::CM(p1,p2);
  //  pI.SetPxPyPzE(p.X(),p.Y(),p.Z(),p.Energy());
  //  cout<<p<<endl;
  if (!generator[0]->SetDecay(pI, numberOfPrimary, mass)) {
    cout << "Error!!!!!! initial reaction kinematically not possible" << endl;
    valid = false;
  }
  //   int tmpI=random()%100;
  //   for(int i=0;i<1000;i++)
  //     {
  //       generator[0]->Generate();
  //       TLorentzVector *tmpP;
  //       cout<<i<<endl;
  //       for(int j=0;j<numberOfPrimary;j++)
  // 	{
  // 	  tmpP=generator[0]->GetDecay(j);
  // 	  cout<<tmpP->Px()<<" "<<tmpP->Py()<<" "<<tmpP->Pz()<<"
  // "<<tmpP->E()<<endl;
  // 	}
  //     }
  if (!PhaseSpace) {
    nParameters = descr.getParam<vector<int>>(4).getData().size();
    parameters = new int[nParameters];
    for (int i = 0; i < nParameters; i++)
      parameters[i] = descr.getParam<vector<int>>(4).getData().at(i);
    function[0] = new TF1("FPri", descr.getParam<vector<string>>(0).getData().at(0).data());
  }
  bool taken = false;
  int num;
  for (int j = 0; j < numberOfDecays; j++) {
    num = -1;
    for (int i = 0; i < numberOfParticles; i++) {
      if (!decays[i])
        continue;
      taken = false;
      for (int k = 0; k < j; k++)
        if (decayParticle[k] == i)
          taken = true;
      if (!taken) {
        num = i;
        break;
      }
    }
    if (num < 0) {
      numberOfDecays = j;
      break;
    }
    decayParticle[j] = num;
    numDecayParticles[j] = 0;
    for (int i = 0; i < numberOfParticles; i++)
      if (decayedOff[i] == num)
        numDecayParticles[j]++;
    decayIDs[j] = new int[numDecayParticles[j]];
    numDecayParticles[j] = 0;
    for (int i = 0; i < numberOfParticles; i++)
      if (decayedOff[i] == num) {
        decayIDs[j][numDecayParticles[j]] = i;
        numDecayParticles[j]++;
      }
    if (!PhaseSpace)
      function[j + 1] = new TF1((string("F") + string_number(j)).data(),
                                descr.getParam<vector<string>>(0).getData().at(j + 1).data());
    else
      function[j + 1] = NULL;
    generator[j + 1] = new TGenPhaseSpace();
    TLorentzVector dp(0, 0, 0, masses[decayParticle[j]]);
    Double_t ma[numDecayParticles[j]];
    for (int i = 0; i < numDecayParticles[j]; i++)
      ma[i] = masses[decayIDs[j][i]];
    if (!generator[j + 1]->SetDecay(dp, numDecayParticles[j], ma)) {
      cout << "Error!!!!! decay: " << j << " kinematically not possible!!!" << endl;
      valid = false;
    }
  }
  num = 0;
  int sorting[numberOfDecays];
  bool modified = true;
  for (int i = 0; i < numberOfDecays; i++) {
    if (generated[decayedOff[i]]) {
      sorting[i] = num;
      for (int j = 0; j < numDecayParticles[i]; j++)
        generated[decayIDs[i][j]] = true;
      num++;
    } else
      sorting[i] = -1;
  }
  while (num < numberOfDecays || !modified) {
    modified = false;
    for (int i = 0; i < numberOfDecays; i++) {
      if (sorting[i] == -1) {
        if (generated[decayParticle[i]]) {
          sorting[i] = num;
          for (int j = 0; j < numDecayParticles[i]; j++)
            generated[decayIDs[i][j]] = true;
          num++;
          modified = true;
        }
      }
    }
  }
  if (!modified && num < numberOfDecays) {
    cout << "Error!!! circular decay-dependence !!!!!" << endl;
    valid = false;
  }
  modified = false;
  for (int i = 0; i < numberOfDecays; i++)
    if (sorting[i] != i)
      modified = true;
  //   for(int i=0;i<numberOfDecays;i++)
  //     {
  //       cout<<"decay "<<i<<endl;
  //       cout<<"decayParticle "<<decayParticle[i]<<endl;
  //       cout<<numDecayParticles[i]<<" decay products"<<endl;
  //       for(int j=0;j<numDecayParticles[i];j++)
  // 	cout<<decayIDs[i][j]<<endl;
  //     }
  if (modified) {
    cout << "need to sort" << endl;
    int tmpdecayParticle[numberOfDecays];
    int tmpnumDecayParticles[numberOfDecays];
    int* tmpdecayIDs[numberOfDecays];
    TGenPhaseSpace* gen[numberOfDecays];
    TF1* func[numberOfDecays];
    for (int i = 0; i < numberOfDecays; i++) {
      tmpdecayParticle[sorting[i]] = decayParticle[i];
      tmpnumDecayParticles[sorting[i]] = numDecayParticles[i];
      tmpdecayIDs[sorting[i]] = decayIDs[i];
      gen[sorting[i]] = generator[i + 1];
      func[sorting[i]] = function[i + 1];
    }
    for (int i = 0; i < numberOfDecays; i++) {
      cout << "decay " << i << endl;
      decayParticle[i] = tmpdecayParticle[i];
      cout << "decayParticle " << decayParticle[i] << endl;
      numDecayParticles[i] = tmpnumDecayParticles[i];
      cout << numDecayParticles[i] << " decay products" << endl;
      decayIDs[i] = tmpdecayIDs[i];
      generator[i + 1] = gen[i];
      function[i + 1] = func[i];
    }
  }
  if (acceptance) {
    requiredHits = new int[setup.getNumberOfDetectors()];
    for (unsigned int i = 0; i < descr.getParam<vector<int>>(2).getData().size(); i++)
      if ((int)i < setup.getNumberOfDetectors())
        requiredHits[i] = descr.getParam<vector<int>>(2).getData().at(i);
    vector<int> tmp;
    for (unsigned int i = 0; i < descr.getParam<vector<int>>(3).getData().size(); i++)
      tmp.push_back(descr.getParam<vector<int>>(3).getData().at(i));
    numSums = 0;
    for (unsigned int i = 0; i < tmp.size(); i++) {
      numSums++;
      i += tmp[i] + 1;
    }
    numSumDet = new int[numSums];
    SumDetID = new int*[numSums];
    SumDetHits = new int[numSums];
    numSums = 0;
    for (unsigned int i = 0; i < tmp.size(); i++) {
      numSumDet[numSums] = tmp[i];
      SumDetID[i] = new int[numSumDet[numSums]];
      for (int j = 0; j < numSumDet[numSums]; j++) {
        SumDetID[numSums][j] = tmp[++i];
      }
      SumDetHits[numSums] = tmp[++i];
      numSums++;
    }
  }
  validInput = valid;
  eventNumber = -1;
}
AGenerateEventsWithDecay::~AGenerateEventsWithDecay()
{
  delete[] particleID;
  delete[] decayedOff;
  delete[] masses;
  delete[] halfLife;
  delete[] charge;
  delete[] primaries;
  if (!PhaseSpace) {
    delete function[0];
    delete generator[0];
    delete[] parameters;
  }
  for (int i = 0; i < numberOfDecays; i++) {
    delete[] decayIDs[i];
    if (!PhaseSpace) {
      delete function[i + 1];
      delete generator[i + 1];
    }
  }
  delete[] numDecayParticles;
  delete[] decayParticle;
  delete[] decayIDs;
  delete[] generator;
  delete[] function;
  if (acceptance) {
    for (int i = 0; i < numSums; i++)
      delete[] SumDetID[i];
    delete[] requiredHits;
    delete[] numSumDet;
    delete[] SumDetID;
    delete[] SumDetHits;
  }
}
void* AGenerateEventsWithDecay::process(void* ptr)
{
  validInput = valid;
  if (!validInput)
    return ptr;
  eventNumber++;
  // cout<<"event: "<<eventNumber<<endl;
  //   for(int i=0;i<numberOfDecays;i++)
  //     {
  //       cout<<"decay "<<i<<endl;
  //       cout<<"decayParticle "<<decayParticle[i]<<endl;
  //       cout<<numDecayParticles[i]<<" decay products"<<endl;
  //       for(int j=0;j<numDecayParticles[i];j++)
  // 	cout<<decayIDs[i][j]<<endl;
  //     }
  double weight = 0, number = 1;
  double decayLength, path, time;
  momentum4D parts[numberOfParticles * 2];
  int num;
  int* pps;
  TLorentzVector v;
  vector3D beta;
  momentum4D CM = momentum4D::CM(setup.getBeamr().getParticle(), setup.getTargetr().getParticle());
  point3D vertex(0, 0, 0);
  sLine3D lines[numberOfParticles];
  bool condition = true;
  int itt = 0;
  while (condition) {
    itt++;
    for (int i = 0; i < numberOfDecays + 1; i++) {
      if (generator[i] == NULL) {
        validInput = false;
        return ptr;
      }
      weight = 0;
      number = 1;
      maxWeight = generator[i]->GetWtMax();
      num = (i == 0 ? numberOfPrimary : numDecayParticles[i - 1]);
      pps = (i == 0 ? primaries : decayIDs[i - 1]);
      if (i == 0)
        beta = -1 * CM.Velocity();
      else {
        beta = parts[decayParticle[i - 1]].Velocity();
        decayLength = ((double)random()) / (double)RAND_MAX;
        time = -log(decayLength) * halfLife[decayParticle[i - 1]] / log(2.) /
               parts[decayParticle[i - 1]].Gamma();
        path = beta.R() * time * 2.99792e11;
        vertex =
            lines[decayParticle[i - 1]].Foot() + lines[decayParticle[i - 1]].Direction() * path;
      }
      while (weight < number) {
        weight = generator[i]->Generate(); // generator[i]->GetWtMax();
        number = ((double)random()) / (double)RAND_MAX;
        weight = weight / maxWeight;
        //	      cout<<"vertex "<<vertex<<endl;
        for (int j = 0; j < num; j++) {
          v = (*(generator[i]->GetDecay(j)));
          parts[pps[j] + (i == 0 ? 0 : numberOfParticles)].setPE(vector3D(v.Px(), v.Py(), v.Pz()),
                                                                 v.E());
          parts[pps[j] + (i == 0 ? numberOfParticles : 0)] =
              parts[pps[j] + (i == 0 ? 0 : numberOfParticles)].boost(beta);
          lines[pps[j]] = sLine3D(vertex, parts[pps[j]].Direction());
          // cout<<generator[i]->GetDecay(j)->Px()<<"
          // "<<generator[i]->GetDecay(j)->Py()<<"
          // "<<generator[i]->GetDecay(j)->Pz()<<"
          // "<<generator[i]->GetDecay(j)->E()<<endl;
          // cout<<v.Px()<<" "<<v.Py()<<" "<<v.Pz()<<" "<<v.E()<<endl;
          // cout<<parts[pps[j]]<<parts[pps[j]+numberOfParticles]<<endl;
        }
        if (!PhaseSpace) {
          setParameter(parts);
          weight = weight * modifyWeight(i);
        }
      }
      for (int j = 0; j < num; j++) {
        v = (*(generator[i]->GetDecay(j)));
        parts[pps[j] + (i == 0 ? 0 : numberOfParticles)].setValues(v.Px(), v.Py(), v.Pz(), v.E());
        parts[pps[j] + (i == 0 ? numberOfParticles : 0)] =
            parts[pps[j] + (i == 0 ? 0 : numberOfParticles)].boost(beta);
        lines[pps[j]] = sLine3D(vertex, parts[pps[j]].Direction());
      }
    }
    //       for(int j=0;j<numberOfParticles;j++)
    // 	cout<<j<<" "<<particleID[j]<<" "<<parts[j]<<" "<<lines[j]<<endl;
    condition = false;
    if (acceptance)
      condition = (accept(lines));
  }

  // copy to tracks
  numberOfTracks = numberOfParticles;
  momentum4D p(0, 0, 0, 0);
  for (int i = 0; i < numberOfPrimary; i++)
    p = p + parts[primaries[i]];
  for (int i = 0; i < numberOfParticles; i++) {
    tracks[i]->setPath(lines[i]);
    tracks[i]->setParticle(parts[i]);
    tracks[i]->setCharge(charge[i]);
    tracks[i]->setSpeed(parts[i].Beta());
  }
  // cout<<lines[0]<<endl;
  for (int i = 0; i < numberOfPrimary; i++) {
    tracks[primaries[i]]->setOrder(false);
  }
  for (int i = 0; i < numberOfDecays; i++) {
    for (int j = 0; j < numDecayParticles[i]; j++) {
      tracks[decayIDs[i][j]]->setPrimary(tracks[decayParticle[i]]);
      tracks[decayParticle[i]]->addSecondary(tracks[decayIDs[i][j]]);
    }
  }
  return ptr;
}
