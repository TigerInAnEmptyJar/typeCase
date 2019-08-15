#include "AMultipleTreeOutput.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include <TFile.h>
logger readWriteLog("readWrite.log", "read write log");
void AMultipleTreeOutput::cleanTree()
{
  bool hasFile = (outFile != NULL);
  if (outFile != NULL) {
    outFile->cd();
    int e = countTree->GetEntries();
    countTree->Write();
    for (int j = 0; j < 3; j++)
      for (int i = 0; i < nTrees[j]; i++)
        trees[j][i]->Write();
    if (nTrees[0] > 0)
      readWriteLog << (int)trees[0][0]->GetEntries() << " prompt entries, ";
    if (nTrees[1] > 0)
      readWriteLog << (int)trees[1][0]->GetEntries() << " kink entries, ";
    if (nTrees[2] > 0)
      readWriteLog << (int)trees[2][0]->GetEntries() << " vee entries, ";
    readWriteLog << endli;
    outFile->Close();
    readWriteLog << e << " Entries to file \"" << outFile->GetName() << "\" written" << endli;
    delete outFile;
    outFile = NULL;
  } else if (countTree != NULL)
    delete countTree;
  countTree = NULL;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < nTrees[j]; i++) {
      if (trackOut[j][i] != NULL)
        delete[] trackOut[j][i];
      trackOut[j][i] = NULL;
      if (trees[j][i] != NULL && !hasFile)
        delete trees[j][i];
      trees[j][i] = NULL;
    }
    if (entryNumbers[j] != NULL)
      delete[] entryNumbers[j];
    entryNumbers[j] = NULL;
  }
  delete[] entryNumbers;
  entryNumbers = NULL;
}
void AMultipleTreeOutput::initNumTree()
{
  if (outFile != NULL)
    outFile->cd();
  if (countTree != NULL)
    delete countTree;
  countTree = new TTree("numberOfTracks", "numberOfTracks");
  countTree->Branch("numberOfTracks", &nTracks, "numberOfTracks/I");
  countTree->Branch("EventNumber", &eventNumber, "EventNumber/I");
  countTree->Branch("RunNumber", &runNumber, "RunNumber/I");
  countTree->Branch("Trigger", &trigger, "Trigger/I");
  countTree->Branch("numberOfPrompt", &nPTracks, "numberOfPrompt/I");
  countTree->Branch("numberOfKink", &nKTracks, "numberOfKink/I");
  countTree->Branch("numberOfVee", &nVTracks, "numberOfVee/I");
  if (entryNumbers != NULL) {
    for (int i = 0; i < 3; i++)
      delete[] entryNumbers[i];
    delete[] entryNumbers;
  }
  entryNumbers = new Int_t*[3];
  for (int i = 0; i < 3; i++)
    entryNumbers[i] = new Int_t[maxtrees];
  string desc = "EntryNumber_p[";
  desc += string_number(maxtrees) + "]/I";
  countTree->Branch("EntryNumber_p", entryNumbers[0], desc.data());
  desc = "EntryNumber_k[";
  desc += string_number(maxtrees) + "]/I";
  countTree->Branch("EntryNumber_k", entryNumbers[1], desc.data());
  desc = "EntryNumber_v[";
  desc += string_number(maxtrees) + "]/I";
  countTree->Branch("EntryNumber_v", entryNumbers[2], desc.data());
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < maxtrees; j++)
      entryNumbers[i][j] = -1;
}
void AMultipleTreeOutput::addATree(int tp)
{
  if (tp < 0 || tp > 2)
    return;
  if (nTrees[tp] >= maxtrees)
    return;
  readWriteLog << "addATree(int tp=" << tp << ") nTrees[tp]=" << nTrees[tp] << " " << maxtrees
               << endli;
  /**********************************************
   * tp==0: prompt nondecaying tracks:
   * tp==1: prompt decaying charged tracks, only kink detected
   * tp==2: prompt decaying uncharged tracks, whole vee detected
   * ...?
   **********************************************/
  if (outFile != NULL)
    outFile->cd();
  trackOut[tp][nTrees[tp]] = new trackT[nTracks_per_tree * (tp + 1)];
  string treename = "tracks_";
  switch (tp) {
  case 0:
    treename += "p";
    break;
  case 1:
    treename += "k";
    break;
  case 2:
    treename += "v";
    break;
  }
  treename += string_number(nTrees[tp]);
  trees[tp][nTrees[tp]] = new TTree(treename.data(), treename.data());
  string branchname;
  for (int i = 0; i < nTracks_per_tree; i++) {
    branchname = string("Track") + string_number(i);
    trees[tp][nTrees[tp]]->Branch(branchname.data(), &trackOut[tp][nTrees[tp]][i],
                                  "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                  "D:theta:beta:energy:momentum:ID/I:nEl");
  }
  if (tp == 1) {
    for (int i = 0; i < nTracks_per_tree; i++) {
      branchname = string("Track_d") + string_number(i);
      trees[tp][nTrees[tp]]->Branch(branchname.data(),
                                    &trackOut[tp][nTrees[tp]][i + nTracks_per_tree],
                                    "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                    "D:theta:beta:energy:momentum:ID/I:nEl");
    }
  } else if (tp == 2) {
    for (int i = 0; i < nTracks_per_tree; i++) {
      branchname = string("Track_a") + string_number(i);
      trees[tp][nTrees[tp]]->Branch(branchname.data(),
                                    &trackOut[tp][nTrees[tp]][i + nTracks_per_tree],
                                    "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                    "D:theta:beta:energy:momentum:ID/I:nEl");
    }
    for (int i = 0; i < nTracks_per_tree; i++) {
      branchname = string("Track_b") + string_number(i);
      trees[tp][nTrees[tp]]->Branch(branchname.data(),
                                    &trackOut[tp][nTrees[tp]][i + nTracks_per_tree * 2],
                                    "vertex[3]/D:fitted[3]/D:el[30]/I:det[30]/I:phi/"
                                    "D:theta:beta:energy:momentum:ID/I:nEl");
    }
  }
  nTrees[tp]++;
}
void AMultipleTreeOutput::setDefault(trackT* out)
{
  out->vertex[0] = 0;
  out->vertex[1] = 0;
  out->vertex[2] = 0;
  out->phi = -5;
  out->theta = -1;
  out->beta = -2;
  out->energy = -2;
  out->momentum = -2;
  out->ID = -1;
  out->nEl = 0;
  for (int i = 0; i < 30; i++) {
    out->el[i] = -1;
    out->det[i] = -1;
  }
}
void AMultipleTreeOutput::setTree(TTrack* in, trackT* out)
{
  out->vertex[0] = in->getPath().Foot().X();
  out->vertex[1] = in->getPath().Foot().Y();
  out->vertex[2] = in->getPath().Foot().Z();
  out->phi = in->getPath().Direction().Phi();
  out->theta = in->getPath().Direction().Theta();
  out->beta = in->getSpeed();
  out->energy = in->getKineticEnergy();
  out->momentum = in->getParticle().Momentum().R();
  out->ID = in->getParticleID();
  out->nEl = in->getNumberOfCalibHits(-1);
  int n = 0;
  for (int i = 0; i < 30; i++) {
    out->det[n] = -1;
    out->el[n] = -1;
  }
  for (int i = 0; i < in->getNumberOfCalibHits(-2); i++)
    for (int j = 0; j < in->getNumberOfCalibHits(i); j++)
      if (n < 30) {
        out->el[n] = in->getCalibSrcr(i, j)->getElement();
        out->det[n] = in->getCalibSrcr(i, j)->getDetector();
        n++;
      }
}
void AMultipleTreeOutput::writeTracks()
{
  if (numberOfTracks == 0)
    return;
  int np = 0, nk = 0, nv = 0;
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->isSecondary())
      continue;
    if (tracks[i]->getNumberOfSecondaryTracks() == 0)
      np++;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 1)
      nk++;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 2)
      nv++;
  }
  int pp[np], kk[nk], vv[nv];
  np = 0;
  nk = 0;
  nv = 0;
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->isSecondary())
      continue;
    if (tracks[i]->getNumberOfSecondaryTracks() == 0)
      pp[np++] = i;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 1)
      kk[nk++] = i;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 2)
      vv[nv++] = i;
  }
  nPTracks = np;
  nVTracks = nv;
  nKTracks = nk;
  while (np > nTrees[0] * nTracks_per_tree) {
    readWriteLog << "nprompt: " << np << " space for: " << nTrees[0] * nTracks_per_tree << endli;
    addATree(0);
  }
  while (nk > nTrees[1] * nTracks_per_tree) {
    readWriteLog << "nkink: " << nk << " space for: " << nTrees[1] * nTracks_per_tree << endli;
    addATree(1);
  }
  while (nv > nTrees[2] * nTracks_per_tree) {
    readWriteLog << "nvee: " << nv << " space for: " << nTrees[2] * nTracks_per_tree << endli;
    addATree(2);
  }
  for (int i = 0; i < np; i++)
    setTree(tracks[pp[i]], &trackOut[0][i / nTracks_per_tree][i % nTracks_per_tree]);
  for (int i = 0; i < nk; i++) {
    setTree(tracks[kk[i]], &trackOut[1][i / nTracks_per_tree][i % nTracks_per_tree]);
    setTree(tracks[kk[i]]->getSecondary(0),
            &trackOut[1][i / nTracks_per_tree][i % nTracks_per_tree + nTracks_per_tree]);
  }
  for (int i = 0; i < nv; i++) {
    setTree(tracks[vv[i]], &trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree]);
    setTree(tracks[vv[i]]->getSecondary(0),
            &trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree + nTracks_per_tree]);
    setTree(tracks[vv[i]]->getSecondary(1),
            &trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree + 2 * nTracks_per_tree]);
  }
  nTracks = numberOfTracks;
  eventNumber = eventNr;
  runNumber = runNr;
  trigger = trgg;
  int npt = np / nTracks_per_tree, nvt = nv / nTracks_per_tree, nkt = nk / nTracks_per_tree;
  if ((np % nTracks_per_tree) > 0)
    npt++;
  if ((nk % nTracks_per_tree) > 0)
    nkt++;
  if ((nv % nTracks_per_tree) > 0)
    nvt++;
  for (int i = 0; i < npt; i++) {
    entryNumbers[0][i] = trees[0][i]->GetEntries();
    trees[0][i]->Fill();
  }
  for (int i = 0; i < nkt; i++) {
    entryNumbers[1][i] = trees[1][i]->GetEntries();
    trees[1][i]->Fill();
  }
  for (int i = 0; i < nvt; i++) {
    entryNumbers[2][i] = trees[2][i]->GetEntries();
    trees[2][i]->Fill();
  }
  countTree->Fill();
  for (int i = 0; i < npt; i++)
    entryNumbers[0][i] = -1;
  for (int i = 0; i < nkt; i++)
    entryNumbers[1][i] = -1;
  for (int i = 0; i < nvt; i++)
    entryNumbers[2][i] = -1;
  for (int i = 0; i < np; i++)
    setDefault(&trackOut[0][i / nTracks_per_tree][i % nTracks_per_tree]);
  for (int i = 0; i < nk; i++) {
    setDefault(&trackOut[1][i / nTracks_per_tree][i % nTracks_per_tree]);
    setDefault(&trackOut[1][i / nTracks_per_tree][i % nTracks_per_tree + nTracks_per_tree]);
  }
  for (int i = 0; i < nv; i++) {
    setDefault(&trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree]);
    setDefault(&trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree + nTracks_per_tree]);
    setDefault(&trackOut[2][i / nTracks_per_tree][i % nTracks_per_tree + 2 * nTracks_per_tree]);
  }
}
void AMultipleTreeOutput::newRun(run_parameter& rp)
{
  if (newFileEachRun) {
    cleanTree();
    string nam1 = rp.getName();
    string nam = "";
    for (unsigned int i = 0; i < nam1.length(); i++)
      if (nam1[i] != ' ')
        nam = nam + nam1;
    nam = filePattern + nam + "T.root";
    outFile = new TFile(nam.data(), "recreate");
    initNumTree();
  }
}
void* AMultipleTreeOutput::process(void* ptr)
{
  if (outFile == NULL)
    return ptr;
  writeTracks();
  return ptr;
}
AMultipleTreeOutput::AMultipleTreeOutput(int max, int& evtNr, int& rnNr, int& triggerIn,
                                         int& nTrack, TTrack** tracksIn,
                                         const algorithm_parameter& param)
    : AAlgorithm("Track output into multiple trees"), maxtrees(max), numberOfTracks(nTrack),
      eventNr(evtNr), runNr(rnNr), trgg(triggerIn)
{
  tracks = tracksIn;
  countTree = NULL;
  entryNumbers = NULL;
  outFile = NULL;
  trees = new TTree**[3];
  trackOut = new trackT**[3];
  for (int i = 0; i < 3; i++) {
    nTrees[i] = 0;
    trees[i] = new TTree*[maxtrees];
    trackOut[i] = new trackT*[maxtrees];
  }
  nTracks_per_tree = param.value(2).value<int>();
  newFileEachRun = param.value(0).value<bool>();
  filePattern = param.value(3).value<string>();
  if (!newFileEachRun) {
    outFile = new TFile((filePattern + "T.root").data(), "recreate");
    initNumTree();
  }
}
AMultipleTreeOutput::~AMultipleTreeOutput()
{
  cleanTree();
  delete[] trees;
  delete[] trackOut;
}
algorithm_parameter AMultipleTreeOutput::getDescription()
{
  algorithm_parameter ret("", 0, 0);
  string lst = "This is a output algorithm for track data."
               "It generates track information containing vertex, direction,"
               "beta, energy, momentum, particleID and information of hits "
               "in the detector (max 30!). This is written to root-file.";
  ret.setDescription(lst);
  ret.addValue("begin new file at end of run", true);
  ret.addValue("use local directory", true);
  ret.addValue("Number of tracks per tree", static_cast<int>(5));
  ret.addValue("File-pattern", std::string{});
  ret.addValue("Local directory", std::string{});
  return ret;
}
