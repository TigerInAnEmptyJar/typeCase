#include "ACMTransformation.h"
#include <TH2.h>
ACMTransformation::ACMTransformation(TSetup& setup, momentum4D** particles, int maxPart,
                                     int& numPart, TDirectory** rootfile, int& evNr, int& runNrIn)
    : AAlgorithm("calculate CM properties"), maxOutParticles(maxPart), numOutParticles(numPart),
      eventNr(evNr), runNr(runNrIn)
{
  outParticles = new momentum4D*[maxOutParticles];
  transformedOutParticles = new momentum4D[maxOutParticles];
  for (int i = 0; i < maxOutParticles; i++)
    outParticles[i] = particles[i];
  rootDirectory = rootfile;
  inParticle1 = setup.getBeamr(0).getParticle();
  if (setup.hasTarget())
    inParticle2 = setup.getTargetr().getParticle();
  else
    inParticle2 = setup.getBeamr(1).getParticle();
  inParticle1.setPM(vector3D(0, 0, 3.059), 0.938271998);
  inParticle2.setPM(vector3D(0, 0, 0), 0.938271998);
  vector3D u(inParticle1.Velocity());
  gamma = 1 / sqrt(1 - u * u);
  M = 0.938271998 * (1 + gamma);
  CM = inParticle1 + inParticle2;
  beta = CM.Direction() * ((1 - 1 / gamma) / u.length());
  CM.setVM(beta, M);
  beta = beta * -1;
  gamma = 1 / sqrt(1 - beta * beta);

  particleList = new TList();
  outTree = new TTree("CM-Tree", "CM-Tree");
  outTree->Branch("EventNumber", &eventNr, "EventNumber/I");
  outTree->Branch("RunNumber", &runNr, "RunNumber/I");
  outTree->Branch("NumberOfParticles", &numOutParticles, "NumberOfParticles/I");
  outTree->Branch("reactionType", &reactionType, "reactionType/I");
  outTree->Branch("angleCM1", &angleCM1, "angleCM1/F");
  outTree->Branch("angleCM2", &angleCM2_b, "angleCM2/F");
  outTree->Branch("angleCM12", &angleCM12, "angleCM12/F");
  outTree->Branch("PangleLab1", &anglePlab1, "PangleLab1/F");
  outTree->Branch("TangleLab1", &angleTlab1, "TangleLab1/F");
  outTree->Branch("PangleLab2", &anglePlab2, "PangleLab2/F");
  outTree->Branch("TangleLab2", &angleTlab2, "TangleLab2/F");

  outTree->Branch(particleList);
  numHistos = 15;
  histos = new TH1*[numHistos];
  histos[0] = new TH1F("Theta angle first particle lab|pp", "Theta angle first particle lab|pp",
                       100, 0, M_PI);
  histos[1] = new TH1F("Phi angle first particle lab|pp", "Phi angle first particle lab|pp", 100,
                       -M_PI, M_PI);
  histos[2] = new TH1F("cosine of Angle first particle CM|pp",
                       "cosine of Angle first particle CM|pp", 100, -1, 1);
  histos[3] = new TH1F("Theta angle second particle lab|pp", "Theta angle second particle lab|pp",
                       100, 0, M_PI);
  histos[4] = new TH1F("Phi angle second particle lab|pp", "Phi angle second particle lab|pp", 100,
                       -M_PI, M_PI);
  histos[5] = new TH1F("cosine of Angle second particle CM|pp",
                       "cosine of Angle second particle CM|pp", 100, -1, 1);
  histos[6] = new TH1F("cosine of Angle between particles CM|pp",
                       "cosine of Angle between particles CM|pp", 100, -1, 1);
  histos[7] = new TH1F("Theta angle first particle lab|dpi", "Theta angle first particle lab|dpi",
                       100, 0, M_PI);
  histos[8] = new TH1F("Phi angle first particle lab|dpi", "Phi angle first particle lab|dpi", 100,
                       -M_PI, M_PI);
  histos[9] = new TH1F("cosine of Angle first particle CM|dpi",
                       "cosine of Angle first particle CM|dpi", 100, -1, 1);
  histos[10] = new TH1F("Theta angle second particle lab|dpi",
                        "Theta angle second particle lab|dpi", 100, 0, M_PI);
  histos[11] = new TH1F("Phi angle second particle lab|dpi", "Phi angle second particle lab|dpi",
                        100, -M_PI, M_PI);
  histos[12] = new TH1F("cosine of Angle second particle CM|dpi",
                        "cosine of Angle second particle CM|dpi", 100, -1, 1);
  histos[13] = new TH1F("cosine of Angle between particles CM|dpi",
                        "cosine of Angle between particles CM|dpi", 100, -1, 1);
  histos[14] = new TH2F("transversal vs. parallel momentum", "transversal vs. parallel momentum",
                        100, 0, CM.Momentum().length(), 100, 0, CM.Momentum().length());
}

ACMTransformation::~ACMTransformation()
{
  (*rootDirectory)->cd();
  outTree->Write();
  for (int i = 0; i < numHistos; i++)
    histos[i]->Write();
  for (int i = 0; i < numHistos; i++)
    delete histos[i];
  delete[] histos;
  delete[] outParticles;
  delete[] transformedOutParticles;
  delete particleList;
  delete outTree;
}

#include <TLorentzVector.h>
void* ACMTransformation::process(void* ptr)
{
  //   while (particleList->GetEntries()>0)
  //     {
  //       TObject *tmp=particleList->First();
  //       particleList->Remove(particleList->First());
  //       delete tmp;
  //     }
  particleList->Clear();
  //   for(int i=0;i< numOutParticles;i++)
  //     {
  //       if(outParticles[i]->getState()==_undefined_)
  // 	{
  //  	  while (particleList->GetEntries()>0)
  //  	    {
  //  	      TObject *tmp=particleList->First();
  //  	      particleList->Remove(particleList->First());
  //  	      delete tmp;
  //  	    }
  // 	  break;
  // 	}
  //       TLorentzVector *v=new TLorentzVector();
  //       vector3D
  //       part(outParticles[i]->X(),outParticles[i]->Y(),outParticles[i]->Z());
  //       transformedOutParticles[i].setPE(part+beta*((gamma-1)/(beta*beta)*(beta*part))+beta*gamma*outParticles[i]->Energy(),gamma*(outParticles[i]->Energy()+beta*part));
  //       v->SetXYZT(transformedOutParticles[i].X(),transformedOutParticles[i].Y(),transformedOutParticles[i].Z(),transformedOutParticles[i].Energy());
  //       particleList->Add(v);
  //     }
  //   vector3D cmd(CM.Direction());
  //   reactionType=-1;
  //   angleCM1=-2;
  //   angleCM2_b=-2;
  //   angleCM12=-2;
  //   anglePlab1=-2;
  //   angleTlab1=-2;
  //   anglePlab2=-2;
  //   angleTlab2=-2;
  //   if(numOutParticles==2 &&particleList->GetEntries()==2)
  //     {
  //       vector3D
  //       m1(/*transformedO*/outParticles[0]->Momentum()),m2(/*transformedO*/outParticles[1]->Momentum());
  //       vector3D mt1,mp1,mt2,mp2;
  //       mp1=cmd*(m1*cmd);
  //       mt1=m1-mp1;
  //       mp2=cmd*(m2*cmd);
  //       mt2=m2-mp2;
  //       m1.normalize();
  //       m2.normalize();
  //       reactionType=0;
  //       angleCM1=transformedOutParticles[0].Direction()*cmd;
  //       angleCM2_b=transformedOutParticles[1].Direction()*cmd;
  //       angleCM12=(transformedOutParticles[1].Direction()*transformedOutParticles[0].Direction());
  //       angleTlab1=(outParticles[0]->Momentum().Theta());
  //       anglePlab1=(outParticles[0]->Momentum().Phi());
  //       angleTlab2=(outParticles[1]->Momentum().Theta());
  //       anglePlab2=(outParticles[1]->Momentum().Phi());
  //       float m=outParticles[0]->Energy();
  //       vector3D v1=outParticles[0]->Velocity();
  //       m=m*sqrt(1-v1*v1);
  //       if(m>1.5)reactionType++;
  //       histos[0+reactionType*7]->Fill(angleTlab1);
  //       histos[1+reactionType*7]->Fill(anglePlab1);
  //       histos[2+reactionType*7]->Fill(angleCM1);
  //       histos[3+reactionType*7]->Fill(angleTlab2);
  //       histos[4+reactionType*7]->Fill(anglePlab2);
  //       histos[5+reactionType*7]->Fill(angleCM2_b);
  //       histos[6+reactionType*7]->Fill(angleCM12);
  //       histos[14]->Fill(mp1.length(),mt1.length());
  //       histos[14]->Fill(mp2.length(),mt2.length());
  //       anglePlab1=cos(anglePlab1);
  //       angleTlab1=cos(angleTlab1);
  //       anglePlab2=cos(anglePlab2);
  //       angleTlab2=cos(angleTlab2);
  //     }
  //   outTree->Fill();
  return ptr;
}
