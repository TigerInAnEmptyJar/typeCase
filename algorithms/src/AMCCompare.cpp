#include "AMCCompare.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include <TFile.h>
#include <fstream>
AMCCompare::AMCCompare(TEvent& eventIn, string inputFile, string outFileName, string reaction)
    : AAlgorithm("MC-Compare"), event(eventIn)
{
  outFile = outFileName;
  reactiontype = getReactionFromName(reaction);
  if (reactiontype == NULL) {
    anaLog << "Error: Reaction " << reaction << " unknown" << endli;
    reactiontype = getReactionFromName("pp-elastic");
  }
  NumberOfTracks = reactiontype->getNumberOfTracks();
  Tracks = new track[NumberOfTracks];
  MCTracks = new track[NumberOfTracks];
  MCTree = new TTree("MC-input", "MC-input");
  eventTree = new TTree("MC-Comparison", "MC-Comparison");
  eventTree->Branch("EventNumber", &EventNumber, "EventNumber/I");
  eventTree->Branch("NumberOfTracks", &NumberOfTracks, "NumberOfTracks/I");
  eventTree->Branch("RunNumber", &RunNumber, "RunNumber/I");
  eventTree->Branch("Trigger", &Trigger, "Trigger/I");
  MCTree->Branch("EventNumber", &EventNumberMC, "EventNumber/I");
  string s;
  for (int i = 0; i < NumberOfTracks; i++) {
    s = string("Track") + string_number(i + 1);
    MCTree->Branch(s.ascii(), &MCTracks[i], "phi/D:theta:beta:ID/I:nEl:ss[17]/I:qdc[17]/F:tdc[17]/"
                                            "F:vertex[3]/D:phiIn/D:thetaIn:betaIn:nElIn/I:ssIn[17]/"
                                            "I:vertexIn[3]/D:momentum/D");
    eventTree->Branch(s.ascii(), &Tracks[i], "phi/D:theta:beta:ID/I:nEl:ss[17]/I:qdc[17]/F:tdc[17]/"
                                             "F:vertex[3]/D:phiIn/D:thetaIn:betaIn:nElIn/"
                                             "I:ssIn[17]/I:vertexIn[3]/D:momentum/D");
  }
  ifstream in;
  in.open(inputFile.ascii());
  while (in.good() && !in.eof())
    readMCEvent(in);
  in.close();
}
AMCCompare::~AMCCompare()
{
  TFile f(outFile.ascii(), "RECREATE");
  eventTree->Write();
  eventTree->SetDirectory(0);
  f.Close();
  delete[] Tracks;
  delete[] MCTracks;
  delete MCTree;
  delete eventTree;
}
void* AMCCompare::process(void* ptr)
{
  if (event->getNumberOfReactions() < 1)
    return ptr;
  if (event->getReaction(0)->IsA() != reactiontype::IsA())
    return ptr;
  setEvent();
  setMCEvent(event.getEventNumber());
  eventTree->Fill();
  return ptr;
}
vector<string> AMCCompare::treeNames()
{
  vector<string> tmp;
  tmp.push_back("MC-Compare");
  tmp.push_back(MCTree->GetName());
  tmp.push_back(eventTree->GetName());
  return tmp;
}
TTree* AMCCompare::tree(string treename)
{
  if (treename == MCTree->GetName())
    return MCTree;
  if (treename == eventTree->GetName())
    return eventTree;
  return NULL;
}
void AMCCompare::readMCEvent(istream& input)
{
  in >> EventNumberMC;
  int n, m[3], pos;
  bool done[NumberOfTracks];
  for (int i = 0; i < NumberOfTracks; i++)
    done[i] = false;
  float f[12];
  in >> n;
  for (int i = 0; i < n; i++) {
    in >> m[0] >> m[1] >> f[0] >> f[1] >> f[2] >> f[3] >> f[4] >> f[5] >> f[6] >> f[7] >> f[8] >>
        f[9] >> f[10] >> f[11] >> m[2];
    pos = -1;
    int j = 0;
    while (j < NumberOfTracks && pos < 0) {
      if (m[2] == reactiontype->getParticleID(j) && !done[j])
        pos = j;
      j++;
    }
    if (pos < 0)
      continue;
    momentum4D p;
    p.setPM(vector3D(f[3], f[4], f[5]), Eparticles::getMassG(m[2]));
    MCTracks[pos].phi = p.Direction().Phi();
    MCTracks[pos].theta = p.Direction().Theta();
    MCTracks[pos].beta = p.beta();
    MCTracks[pos].momentum = p.Momentum().R();
    MCTracks[pos].vertex[0] = f[0];
    MCTracks[pos].vertex[1] = f[1];
    MCTracks[pos].vertex[2] = f[2];
    MCTracks[pos].ID = m[2];
    done[pos] = true;
  }
  MCTree->Fill();
}
void AMCCompare::setEvent()
{
  RbaseReaction* react = event.getReaction(0);
  EventNumber = event.getEventNumber();
  RunNumber = event.getRunNumber();
  Trigger = event.getTrigger();
  TTrack* tmptrack;
  momentum4D p;
  for (int i = 0; i < NumberOfTracks; i++) {
    p = react->getParticle(i);
    tmptrack = react->getTrack(i);
    Tracks[i].phi = p.Direction().Phi();
    Tracks[i].theta = p.Direction().Theta();
    Tracks[i].beta = p.Beta();
    Tracks[i].momentum = p.Momentum().R();
    Tracks[i].nEl = tmptrack->getNumberOfCalibHits();
    Tracks[i].vertex[0] = tmptrack->getPath().Foot().X();
    Tracks[i].vertex[1] = tmptrack->getPath().Foot().Y();
    Tracks[i].vertex[2] = tmptrack->getPath().Foot().Z();
    Tracks[i].ID = react->getParticleID(i);
  }
}
int AMCCompare::setMCEvent(int eventNumber)
{
  int entry = eventNumber;
  if (entry > MCTree->GetEntries())
    entry = MCTree->GetEntries() - 1;
  MCTree->GetEntry(entry);
  while (EventNumberMC < eventNumber && entry < MCTree->GetEntries() - 1) {
    entry++;
    if (entry > MCTree->GetEntries())
      entry = MCTree->GetEntries() - 1;
    MCTree->GetEntry(entry);
  }
  while (EventNumberMC > eventNumber && entry >= 0) {
    entry--;
    if (entry < 0)
      entry = 0;
    MCTree->GetEntry(entry);
  }
  if (EventNumberMC != eventNumber)
    return -1;
  for (int i = 0; i < NumberOfTracks; i++) {
    Tracks[i].phiIn = MCTracks[i].phi;
    Tracks[i].thetaIn = MCTracks[i].theta;
    Tracks[i].betaIn = MCTracks[i].beta;
    Tracks[i].vertexIn[0] = MCTracks[i].vertex[0];
    Tracks[i].vertexIn[1] = MCTracks[i].vertex[1];
    Tracks[i].vertexIn[2] = MCTracks[i].vertex[2];
  }
  return entry;
}
