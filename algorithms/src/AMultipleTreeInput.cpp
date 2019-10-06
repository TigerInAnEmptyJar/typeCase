#include "AMultipleTreeInput.h"
#include "logger.h"
#include <QtCore/QFile>
//#define OUTPUT
//#define TDC_OUTPUT
#ifdef TDC_OUTPUT
#include <TFile.h>
#include <TTree.h>
typedef struct
{
  Float_t tdcB1;
  Float_t tdcB2;
  Float_t tdcB3;
  Float_t tdcF1;
  Float_t tdcF2;
  Float_t theta;
  Float_t beta;
  Float_t phi;
  Int_t det1;
  Int_t det2;
  Int_t det3;
  Int_t elB1;
  Int_t elB2;
  Int_t elB3;
  Int_t elF1;
  Int_t elF2;
} betaInfo;
TTree* treeinput_out = NULL;
betaInfo* tra_info_out;
#endif
extern void setTrackErrors(TTrack* tr);
void AMultipleTreeInput::readATrack(TTrack* out, TLeaf** in)
{
  out->reset();
  out->setSpeed(in[1]->GetValue());
  out->setKineticEnergy(in[8]->GetValue());
  out->setCharge(1);
  point3D p(in[4]->GetValue(0), in[4]->GetValue(1), in[4]->GetValue(2));
  double t = in[2]->GetValue(), ph = in[3]->GetValue();
  out->setPath(sLine3D(p, vector3D(sin(t) * cos(ph), sin(t) * sin(ph), cos(t))));
  int n = (Int_t)in[5]->GetValue(), d, e;
  if (n > 30)
    n = 30;
  for (int i = 0; i < n; i++) {
    if (in[6]->GetValue(i) < 0 || in[7]->GetValue(i) < 0)
      continue;
    d = (Int_t)in[6]->GetValue(i);
    e = (Int_t)in[7]->GetValue(i);
#ifdef OUTPUT
    bool found = false;
#endif
    for (int j = 0; j < (*numberOfHits[d]); j++) {
      if (hits[d][j]->getElement() == e)
#ifdef OUTPUT
      {
        found = true;
#endif
        out->addCalibSrc(hits[d][j]);
#ifdef OUTPUT
      }
#endif
    }
#ifdef OUTPUT
    //      if(d==0)
    {
      //	  cout<<d<<" "<<e<<" "<<flush;
      if (!found) {
        cout << "hit: not found :" << d << " " << e << " " << (*numberOfHits[d]) << "(";
        for (int j = 0; j < (*numberOfHits[d]); j++)
          cout << hits[d][j]->getElement() << ",";
        cout << endl;
      }
      //	  else cout<<endl;
    }
#endif
    //      if(out->getNumberOfCalibHits()!=n)cout<<"hits not found: "<<n<<"
    //      "<<out->getNumberOfCalibHits()<<endl;
  }
  if (out->getNumberOfCalibHits() > 0)
    setTrackErrors(out);
}
void AMultipleTreeInput::cleanUp()
{
  for (int i = 0; i < 3; i++) {
    if (trees != NULL) {
      if (trees[i] != NULL) {
        delete[] trees[i];
      }
    }
    if (leaves != NULL) {
      if (leaves[i] != NULL) {
        for (int k = 0; k < nTrees[i] * nTracksPerTree * (i + 1); k++)
          if (leaves[i][k] != NULL)
            delete[] leaves[i][k];
        delete[] leaves[i];
      }
    }
  }
  if (entryNumber != NULL)
    delete[] entryNumber;
  if (trees != NULL)
    delete[] trees;
  if (leaves != NULL)
    delete[] leaves;
  entryNumber = NULL;
  trees = NULL;
  leaves = NULL;
  if (inputFile != NULL) {
    inputFile->Close();
    delete inputFile;
  }
  countTree = NULL;
}
void AMultipleTreeInput::OnNewRun(run_parameter& rp)
{
  string filename = "";
  for (size_t i = 0; i < rp.getNumberOfFiles(); i++) {
    if (rp.getFileType(i) == 129)
      filename = rp.getFile(i);
  }
  if (QFile::exists(filename.data())) {
    cleanUp();
    setTree(filename);
    readInValid = true;
  } else
    cout << "file \"" << filename.data() << "\" doesn\'t exist." << endl;
  anaLog << "opend file \"" << filename.data() << "\" for multiple tree input " << endli;
  anaLog << nTrees[0] << " prompt-, " << nTrees[1] << " kink-, " << nTrees[2] << " vee-trees found"
         << endli;
}
extern logger readWriteLog;
void AMultipleTreeInput::setTree(const string& filename)
{
  inputFile = new TFile(filename.data(), "read");
  readWriteLog << "Opening file \"" << filename.data() << "\" for Multiple tree input" << endli;
  trees = new TTree**[3];
  countTree = (TTree*)inputFile->Get("numberOfTracks");
  if (countTree == NULL) {
    readInValid = false;
    cleanUp();
  }
  leaves = new TLeaf***[3];
  string treename;
  string pre[5] = {"", "_d", "_a", "", "_b"};
  for (int i = 0; i < 3; i++) {
    treename = "tracks_";
    switch (i) {
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
    nTrees[i] = 0;
    while (inputFile->Get((treename + string_number(nTrees[i])).data()) != NULL)
      nTrees[i]++;
    trees[i] = new TTree*[nTrees[i]];
    for (int j = 0; j < nTrees[i]; j++)
      trees[i][j] = (TTree*)inputFile->Get((treename + string_number(j)).data());
  }
  nTracksPerTree = 0;
  while (trees[0][0]->FindLeaf((string("Track") + string_number(nTracksPerTree) + ".theta").data()))
    nTracksPerTree++;
  readWriteLog << " found " << nTrees[0] << " prompt track trees, " << nTrees[1] << " kink trees, "
               << nTrees[2] << " vee trees" << endli;
  bool allLeavesFound = true;
  for (int j = 0; j < 3; j++) {
    leaves[j] = new TLeaf**[nTrees[j] * nTracksPerTree * (j + 1)];
    for (int k = 0; k < nTrees[j] * nTracksPerTree * (j + 1); k++)
      leaves[j][k] = new TLeaf*[9];
    for (int k = 0; k < nTrees[j]; k++) {
      for (int l = 0; l < nTracksPerTree; l++) {
        for (int m = 0; m <= j; m++) {
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][0] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".ID").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][1] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".beta").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][2] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".theta").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][3] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".phi").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][4] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".vertex").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][5] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".nEl").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][6] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".det").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][7] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".el").data());
          leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][8] =
              trees[j][k]->FindLeaf(
                  (string("Track") + pre[m * j] + string_number(l) + ".energy").data());
          for (int nn = 0; nn < 9; nn++)
            if (leaves[j][m * nTrees[j] * nTracksPerTree + (l + nTracksPerTree * k)][nn] == NULL)
              allLeavesFound = false;
        }
      }
    }
  }
  if (!allLeavesFound)
    readWriteLog << " error: at least one of the leaves is a dead end" << endli;
  entryNumber = new TLeaf*[3];
  entryNumber[0] = countTree->FindLeaf("EntryNumber_p");
  entryNumber[1] = countTree->FindLeaf("EntryNumber_k");
  entryNumber[2] = countTree->FindLeaf("EntryNumber_v");
  countTree->SetBranchStatus("*", 1);
  countTree->SetBranchAddress("EventNumber", &evtNr);
  countTree->SetBranchAddress("numberOfPrompt", &numberOf[0]);
  countTree->SetBranchAddress("numberOfKink", &numberOf[1]);
  countTree->SetBranchAddress("numberOfVee", &numberOf[2]);
  currentEntry = 0;
}
#ifdef TWOTWO_out
extern Int_t twotwoEvent;
extern Int_t twotwoRun;
#endif
void AMultipleTreeInput::process()
{
  if (countTree == NULL)
    return;
  if (read_given_event) {
    countTree->GetEntry(currentEntry);
    if (eventNr != evtNr) {
      while (eventNr > evtNr && currentEntry < countTree->GetEntries() - 1)
        countTree->GetEntry(++currentEntry);
      if (currentEntry >= countTree->GetEntries())
        readInValid = false;
      if (eventNr != evtNr) {
        // numberOfTracks=0;
        return;
      }
    }
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < nTrees[i]; j++) {
        if (entryNumber[i]->GetValue(j) >= 0)
          trees[i][j]->GetEntry((Int_t)entryNumber[i]->GetValue(j));
      }
    }
  } else {
    countTree->GetEntry(currentEntry);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < nTrees[i]; j++)
        if (entryNumber[i]->GetValue(j) >= 0)
          trees[i][j]->GetEntry((Int_t)entryNumber[i]->GetValue(j));
    }
    currentEntry++;
    if (currentEntry >= countTree->GetEntries())
      readInValid = false;
  }
#ifdef TWOTWO_out
  twotwoEvent = evtNr;
  twotwoRun = (int)countTree->FindLeaf("RunNumber")->GetValue();
#endif
  int nt = 0;
  for (int j = 0; j < numberOf[0]; j++)
    readATrack(tracks[nt++], leaves[0][j]);
  for (int j = 0; j < numberOf[1]; j++) {
    readATrack(tracks[nt++], leaves[1][j]);
    //      readATrack(tracks[nt++], leaves[1][j+(nTrees[1]*nTracksPerTree)]);
    //       tracks[nt-1]->setOrder(true);
    //       tracks[nt-1]->setPrimary(tracks[nt-2]);
    //       tracks[nt-2]->addSecondary(tracks[nt-1]);
  }
  for (int j = 0; j < numberOf[2]; j++) {
    readATrack(tracks[nt], leaves[2][j]);
    readATrack(tracks[nt + 1], leaves[2][j + (nTrees[2] * nTracksPerTree)]);
    readATrack(tracks[nt + 2], leaves[2][j + (nTrees[2] * nTracksPerTree) * 2]);
    tracks[nt + 1]->setOrder(true);
    tracks[nt + 1]->setPrimary(tracks[nt]);
    tracks[nt + 2]->setOrder(true);
    tracks[nt + 2]->setPrimary(tracks[nt]);
    tracks[nt]->addSecondary(tracks[nt + 1]);
    tracks[nt]->addSecondary(tracks[nt + 2]);
    nt += 3;
  }
  numberOfTracks = nt;
#ifdef TDC_OUTPUT
  if (numberOf[0] == 2 && numberOf[2] <= 1) {
    for (int i = 0; i < 4; i++) {
      TTrack* currTrack = NULL;
      if (i == 0 || i == 1)
        currTrack = tracks[i];
      else {
        currTrack = tracks[numberOf[0] + numberOf[1] + i - 1];
      }
      tra_info_out[i].theta = currTrack->getPath().Direction().Theta();
      tra_info_out[i].phi = currTrack->getPath().Direction().Phi();
      tra_info_out[i].beta = currTrack->getSpeed();
      tra_info_out[i].elF1 = -1;
      tra_info_out[i].elF2 = -1;
      tra_info_out[i].elB1 = -1;
      tra_info_out[i].elB2 = -1;
      tra_info_out[i].elB3 = -1;
      if (currTrack->getNumberOfCalibHits(15) > 0) {
        tra_info_out[i].elF1 = currTrack->getCalibSrcr(15, 0)->getElement();
        tra_info_out[i].tdcF1 = currTrack->getCalibSrcr(15, 0)->getTDC();
      }
      if (currTrack->getNumberOfCalibHits(16) > 0) {
        tra_info_out[i].elF2 = currTrack->getCalibSrcr(16, 0)->getElement();
        tra_info_out[i].tdcF2 = currTrack->getCalibSrcr(16, 0)->getTDC();
      }
      TCalibHit *th1 = 0, *th2 = 0, *th3 = 0;
      if (currTrack->getNumberOfCalibHits(0) > 0) {
        th1 = currTrack->getCalibSrcr(0, 0);
        th2 = currTrack->getCalibSrcr(1, 0);
        th3 = currTrack->getCalibSrcr(2, 0);
      } else if (currTrack->getNumberOfCalibHits(3) > 0) {
        th1 = currTrack->getCalibSrcr(3, 0);
        th2 = currTrack->getCalibSrcr(4, 0);
        th3 = currTrack->getCalibSrcr(5, 0);
      } else if (currTrack->getNumberOfCalibHits(6) > 0) {
        th1 = currTrack->getCalibSrcr(6, 0);
        th2 = currTrack->getCalibSrcr(7, 0);
      }
      if (th1 != NULL) {
        tra_info_out[i].tdcB1 = th1->getTDC();
        tra_info_out[i].det1 = th1->getDetector();
        tra_info_out[i].elB1 = th1->getElement();
      }
      if (th2 != NULL) {
        tra_info_out[i].tdcB2 = th2->getTDC();
        tra_info_out[i].det2 = th2->getDetector();
        tra_info_out[i].elB2 = th2->getElement();
      }
      if (th3 != NULL) {
        tra_info_out[i].tdcB3 = th3->getTDC();
        tra_info_out[i].det3 = th3->getDetector();
        tra_info_out[i].elB3 = th3->getElement();
      }
    }
    treeinput_out->Fill();
  }
#endif
}
AMultipleTreeInput::AMultipleTreeInput(int& eventNumber, int& runNumber, int& trigger,
                                       int** numberOfHitsIn, TTrack** tracksIn,
                                       int& numberOfTracksIn, TCalibHit*** hitsIn,
                                       bool& readInValidIn, const algorithm_parameter& param)
    : AAlgorithm("read from multiple trees"), numberOfTracks(numberOfTracksIn),
      eventNr(eventNumber), runNr(runNumber), trgg(trigger), readInValid(readInValidIn)
{
  tracks = tracksIn;
  numberOfHits = numberOfHitsIn;
  hits = hitsIn;
  leaves = NULL;
  trees = NULL;
  countTree = NULL;
  entryNumber = NULL;
  inputFile = NULL;
  read_given_event = param.value(0).value<bool>();
#ifdef TDC_OUTPUT
  TFile* f_output = new TFile("tdcoutput.root", "recreate");
  f_output->cd();
  treeinput_out = new TTree("TDCoutput", "TDCoutput");
  tra_info_out = new betaInfo[4];
  for (int i = 0; i < 4; i++)
    treeinput_out->Branch(string("track" + string_number(i)).data(), &tra_info_out[i],
                          "tdcB1/F:tdcB2:tdcB3:tdcF1:tdcF2:theta:beta:phi:det1/"
                          "I:det2:det3:elB1:elB2:elB3:elF1:elF2");
#endif
}
AMultipleTreeInput::~AMultipleTreeInput()
{
  cleanUp();
#ifdef TDC_OUTPUT
  TFile* f_output = treeinput_out->GetCurrentFile();
  f_output->cd();
  treeinput_out->Write();
  f_output->Close();
  delete f_output;
  delete[] tra_info_out;
#endif
}
algorithm_parameter AMultipleTreeInput::getDescription()
{
  algorithm_parameter ret("Read Tracks from multiple trees", algorithm_parameter::Category::INPUT,
                          0);
  ret.addValue("Search for read event", false);
  ret.addValue("use local directory", true);
  ret.addValue("Local directory", std::string{});
  string des = "This algorithm reads track information from a root tree file."
               "There is one tree stearing the read in for the other ones, "
               "containing the event- and runnumber, the trigger and the entry-"
               "numbers for the other trees to form one event."
               "The other trees come in three packages: prompt tracks, kink tracks"
               "and vees.\n"
               "Each track has information about the vertex, the direction, beta,"
               "energy and ID of the particle and the hit detectors."
               "It doesn't read in hits, but you can read the hits with another"
               "algorithm and switch the \"search for read event\"-parameter on, so"
               "the current event will be searched (if not found, the event is not"
               "skipped, but the number of tracks is set to zero).";
  ret.setDescription(des);
  return ret;
}
