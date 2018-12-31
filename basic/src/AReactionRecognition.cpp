#include "AReactionRecognition.h"
//#include "reactions.h"
#include "Eparticles.h"
#include "fitAlgorithm.h"
#include "logger.h"
#include <fstream>
ofstream fitFile("fitFile.txt");
//#include <TH1.h>
// extern TH1F breakReason;
// extern TTree *outTree_pkl;
// extern TTree *outTree_general_pkl;
// extern TTree *treeinput_out;
//#include "AKinFit.h"
int nCandidates = 0;
int nPassed = 0;
AReactionRecognition::AReactionRecognition(TSetup& setup, TEvent& eventIn, AFitAlgorithm* kinfitIn,
                                           string options)
    : AAlgorithm("reaction recognition for " + options), event(eventIn)
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  kinfit = kinfitIn;
  getReactions(setup, options);
  tracks = new TTrack*[event.getMaxNumber<TTrack>()];
  for (int i = 0; i < event.getMaxNumber<TTrack>(); i++)
    tracks[i] = &event.getTrack(i);
}
extern RbaseReaction* getReactionFromName(const string& name, momentum4D m1, momentum4D m2);
extern RbaseReaction* getReaction(const algorithm_parameter& param, momentum4D m1, momentum4D m2);
AReactionRecognition::AReactionRecognition(TSetup& setup, TEvent& eventIn, AFitAlgorithm* kinfitIn,
                                           vector<algorithm_parameter> processes)
    : AAlgorithm("reaction recognition"), event(eventIn)
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  kinfit = kinfitIn;
  reactions = new RbaseReaction*[processes.size()];
  int j = 0;
  momentum4D m1, m2;
  m1 = setup.getBeamr().getParticle();
  m2 = (!setup.hasTarget() ? setup.getBeamr(1).getParticle() : setup.getTargetr().getParticle());
  tracks = new TTrack*[event.getMaxNumber<TTrack>()];
  for (int i = 0; i < event.getMaxNumber<TTrack>(); i++)
    tracks[i] = &event.getTrack(i);
  anaLog << "Reactions found:" << endli;
  for (unsigned int i = 0; i < processes.size(); i++) {
    reactions[j] = getReaction(processes[i], m1, m2);
    if (reactions[j] == NULL)
      continue;
    anaLog << reactions[j]->name().data() << endli;
    j++;
  }
  numberOfReactions = j;
}
extern void printtestH();
//#include <TTree.h>
//#include <TFile.h>
// extern void writeoutTreeGeneral();
AReactionRecognition::~AReactionRecognition()
{
  printtestH();
  fitFile.close();
  cout << "pkl candidates: " << nCandidates << endl;
  cout << "pkl passed: " << nPassed << endl;
  //   TCanvas *canvas=new TCanvas();
  //   breakReason.Draw();
  //   canvas->Print("breakReason.ps");
  //  writeoutTreeGeneral();
  //   if(outTree_pkl!=NULL)
  //     {
  //       TFile *f=outTree_pkl->GetCurrentFile();
  //       f->cd();
  //       outTree_pkl->Write();
  //       f->Close();
  //       delete f;
  //     }
  //   if(outTree_general_pkl!=NULL)
  //     {
  //       TFile *f=outTree_general_pkl->GetCurrentFile();
  //       f->cd();
  //       outTree_general_pkl->Write();
  //       f->Close();
  //       delete f;
  //     }
  //   delete canvas;
  for (int i = 0; i < numberOfReactions; i++)
    delete reactions[i];
  delete[] reactions;
  delete[] tracks;
}

void* AReactionRecognition::process(void* ptr)
{
  RbaseReaction* tmp;
  event.clearReactions();
  int np = 0, nv = 0, nk = 0;
  for (int i = 0; i < event.getNumberOfTracks(); i++)
    if (!tracks[i]->isSecondary()) {
      switch (tracks[i]->getNumberOfSecondaryTracks()) {
      case 0:
        np++;
        break;
      case 1:
        nk++;
        break;
      case 2:
        nv++;
        break;
      }
    }
  TTrack *tr_p[np], *tr_v[nv], *tr_k[nk];
  np = 0;
  nv = 0;
  nk = 0;
  for (int i = 0; i < event.getNumberOfTracks(); i++)
    if (!tracks[i]->isSecondary()) {
      switch (tracks[i]->getNumberOfSecondaryTracks()) {
      case 0:
        tr_p[np] = tracks[i];
        np++;
        break;
      case 1:
        tr_k[nk] = tracks[i];
        nk++;
        break;
      case 2:
        tr_v[nv] = tracks[i];
        nv++;
        break;
      }
    }
  //  cout<<np<<" "<<nv<<" "<<nk<<endl;
  if (np == 2 && nv == 1) {
    nCandidates++;
  } // cout<<"candidate "<<event.getEventNumber()<<endl;}
  for (int i = 0; i < numberOfReactions; i++) {
    tmp = reactions[i]->checkNew(tr_p, np, tr_k, nk, tr_v, nv);
    if (tmp != NULL) {
      //	  if(!(tmp->getChiSquared()==-1||tmp->getChiSquared()>tmp->getChiLimit()))
      {
        event.addReaction(tmp);
        nPassed++;
      }
      //	  else
      //	    {
      //	      delete tmp;
      //	    }
    }
  }
  //  if(np==2&&nv==1)cout<<"\t "<<event.getEventNumber()<<"
  //  "<<event.getNumberOfReactions()<<endl;
  return ptr;
}
int AReactionRecognition::getReactions(TSetup& setup, string option)
{
  string s = option, subN, subC;
  numberOfReactions = 0;
  int n = 0;
  n += contains(s, "__ppel_");
  n += contains(s, "__pp-pkl-pkppi_");
  //  n+=s.contains("");
  //  n+=s.contains("");
  //  n+=s.contains("");
  reactions = new RbaseReaction*[n];
  //  AFitAlgorithm *tmp;
  momentum4D m1, m2;
  m1 = setup.getBeamr().getParticle();
  m2 = (!setup.hasTarget() ? setup.getBeamr(1).getParticle() : setup.getTargetr().getParticle());
  n = 0;
  for (int i = 0; i < 2; i++) {
    switch (i) {
    case 0:
      subN = "ppel";
      break;
    case 1:
      subN = "pp-pkl-pkppi";
      break;
      // 	case 2:
      // 	  subN="";
      // 	  break;
    }
    while (contains(s, (string("__") + subN + "_").data())) {
      subC = s.substr(s.find("__" + subN + "_") + subN.length() + 3,
                      s.find("_", s.find("__" + subN + "_") + subN.length() + 3) -
                          s.find("__" + subN + "_") - subN.length() - 3);
      kinfit->getClone();
      reactions[n] = getReactionFromName(subN, m1, m2);
      reactions[n]->setChiLimit(toFloat(subC));
      n++;
      remove(s, (string("__") + subN + "_" + subC + "_").data());
    }
  }
  numberOfReactions = n;
  return n;
}
