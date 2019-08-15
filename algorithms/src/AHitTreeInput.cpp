#include "AHitTreeInput.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include <stdlib.h>
extern logger readWriteLog;
extern bool existing(string strFilename);
algorithm_parameter AHitTreeInput::getDescription()
{
  algorithm_parameter ret("Read Hits from Tree", 0, 0);
  string des = "This algorithm reads calibrated hits from a root tree and "
               "requests the hit shapes from the setup. "
               "The hits are considered as calibrated and valid.";
  ret.addValue(string("search for event"), true);
  ret.addValue(string("use as event input list"), true);
  ret.addValue(string("use local directory"), true);
  ret.addValue(string("use all 257 hit files in chain"), true);
  ret.addValue(string("local directory"), string(""));
  ret.setDescription(des);
  return ret;
}
AHitTreeInput::AHitTreeInput(TSetup& setupIn, TCalibHit*** hitsIn, int** nH, int& evtNr, int& rnNr,
                             int& trggr, int maxH, int MaxD, bool& readInValid,
                             const algorithm_parameter& param)
    : AAlgorithm("Read hit tree"), EventNumber(evtNr), RunNumber(rnNr), Trigger(trggr),
      maxHit(maxH), maxDet(MaxD), valid(readInValid), setup(setupIn)
{
  hits = hitsIn;
  numberOfHits = nH;
  searchForEvent = param.value(0).value<bool>();
  localDirectory = false;
  localDirectory = param.value(2).value<bool>();
  directory = param.value(4).value<string>();
  hitTree = NULL;
  hitChain = NULL;
  tdcRLeaf = NULL;
  qdcRLeaf = NULL;
  tdcCLeaf = NULL;
  qdcCLeaf = NULL;
  elLeaf = NULL;
  detLeaf = NULL;
  eh_en = NULL;
  eh_rn = NULL;
  eh_tr = NULL;
  eh_nh = NULL;
  currentEntry = -1;
  valid = false;
  useChain = false;
  useChain = param.value(3).value<bool>();
  if (useChain)
    hitChain = new TChain("hits", "hits");
}
AHitTreeInput::~AHitTreeInput()
{
  clearTree();
  if (useChain)
    delete hitChain;
}
void* AHitTreeInput::process(void* ptr)
{
  if (hitTree != NULL) {
    int max = (int)hitTree->GetEntries();
    if (useChain && hitChain != NULL)
      max = (int)hitChain->GetEntries();
    if (max > (int)currentEntry - 1) {
      if (searchForEvent)
        readEvent(EventNumber);
      else {
        readEntry(currentEntry);
        EventNumber = (Int_t)eh_en->GetValue();
      }
    } else
      valid = false;
  }
  return ptr;
}
void AHitTreeInput::readEvent(int EvtNr)
{
  if (!valid)
    return;
  if (hitTree == NULL)
    return;
  if (useChain) {
    if (hitChain == NULL)
      return;
    hitChain->GetEntry(currentEntry);
    if (hitTree != hitChain->GetTree()) {
      hitTree = hitChain->GetTree();
      getHeader();
      getMain();
    }
    while (eh_en->GetValue() < EvtNr && hitTree->GetEntries() > currentEntry + 1) {
      hitChain->GetEntry(++currentEntry);
      if (hitTree != hitChain->GetTree()) {
        hitTree = hitChain->GetTree();
        getHeader();
        getMain();
      }
    }
    while (eh_en->GetValue() > EvtNr && currentEntry > 0) {
      if (hitTree != hitChain->GetTree()) {
        hitTree = hitChain->GetTree();
        getHeader();
        getMain();
      }
      hitChain->GetEntry(--currentEntry);
    }
    if (eh_en->GetValue() == EvtNr) {
      readEntry(currentEntry);
      return;
    } else {
      if (eh_en->GetValue() < EvtNr) {
        hitChain->GetEntry(++currentEntry);
        if (hitTree != hitChain->GetTree()) {
          hitTree = hitChain->GetTree();
          getHeader();
          getMain();
        }
        if (eh_en->GetValue() > EvtNr) {
          cout << "bad event " << EvtNr << endl;
          currentEntry--;
          return;
        }
      } else {
        hitChain->GetEntry(--currentEntry);
        if (hitTree != hitChain->GetTree()) {
          hitTree = hitChain->GetTree();
          getHeader();
          getMain();
        }
        if (eh_en->GetValue() < EvtNr) {
          cout << "bad event " << EvtNr << endl;
          return;
        }
      }
      if (eh_en->GetValue() == EvtNr) {
        readEntry(currentEntry);
        return;
      }
    }
    if (hitTree != hitChain->GetTree()) {
      hitTree = hitChain->GetTree();
      getHeader();
      getMain();
    }
  } else {
    if (eh_en == NULL || eh_rn == NULL || eh_tr == NULL || eh_nh == NULL)
      return;
    eh_en->GetBranch()->GetEntry(currentEntry);
    while (eh_en->GetValue() < EvtNr && hitTree->GetEntries() > currentEntry + 1)
      eh_en->GetBranch()->GetEntry(++currentEntry);
    while (eh_en->GetValue() > EvtNr && currentEntry > 0)
      eh_en->GetBranch()->GetEntry(--currentEntry);
    if (eh_en->GetValue() == EvtNr) {
      readEntry(currentEntry);
      return;
    } else {
      if (eh_en->GetValue() < EvtNr) {
        eh_en->GetBranch()->GetEntry(++currentEntry);
        if (eh_en->GetValue() > EvtNr) {
          cout << "bad event " << EvtNr << endl;
          currentEntry--;
          return;
        }
      } else {
        eh_en->GetBranch()->GetEntry(--currentEntry);
        if (eh_en->GetValue() < EvtNr) {
          cout << "bad event " << EvtNr << endl;
          return;
        }
      }
      if (eh_en->GetValue() == EvtNr) {
        readEntry(currentEntry);
        return;
      }
    }
  }
}
void AHitTreeInput::readEntry(int entry)
{
  if (!valid)
    return;
  if (hitTree == NULL)
    return;
  if (useChain) {
    hitChain->GetEntry(entry);
    if (hitTree != hitChain->GetTree()) {
      hitTree = hitChain->GetTree();
      getHeader();
      getMain();
    }
    if (tdcRLeaf == NULL || qdcRLeaf == NULL || tdcCLeaf == NULL || qdcCLeaf == NULL ||
        elLeaf == NULL || detLeaf == NULL || eh_en == NULL || eh_rn == NULL || eh_tr == NULL ||
        eh_nh == NULL)
      return;
  } else {
    if (tdcRLeaf == NULL || qdcRLeaf == NULL || tdcCLeaf == NULL || qdcCLeaf == NULL ||
        elLeaf == NULL || detLeaf == NULL || eh_en == NULL || eh_rn == NULL || eh_tr == NULL ||
        eh_nh == NULL)
      return;
    hitTree->GetEntry(entry);
  }

  for (int i = 0; i < maxDet; i++)
    (*(numberOfHits[i])) = 0;
  int numHits = (Int_t)eh_nh->GetValue();
  int q, t;
  short d, e;
  TCalibHit* ht;
  for (int i = 0; i < numHits; i++) {
    d = (Short_t)detLeaf->GetValue(i);
    e = (Short_t)elLeaf->GetValue(i);
    q = (Int_t)qdcRLeaf->GetValue(i);
    t = (Int_t)tdcRLeaf->GetValue(i);
    if (d >= setup.getNumberOfDetectors())
      continue;
    if ((*(numberOfHits[d])) >= maxHit)
      continue;
    if (e >= static_cast<int>(setup.getDetectorr(d).getNumberOfElements()) || e < 0)
      continue;
    ht = hits[d][*(numberOfHits[d])];
    ht->reset();
    ht->setRaw(TRawHit(d, e, q, t));
    ht->setADC((Float_t)qdcCLeaf->GetValue(i));
    ht->setTDC((Float_t)tdcCLeaf->GetValue(i));
    ht->setHitShape(setup.getDetectorr(d).getShape(e));
    ht->setValid(true);
    (*(numberOfHits[d])) = (*(numberOfHits[d])) + 1;
  }
  RunNumber = (Int_t)eh_rn->GetValue();
  Trigger = (Int_t)eh_tr->GetValue();
  currentEntry = entry + 1;
}
void AHitTreeInput::clearTree()
{
  if (useChain)
    hitChain->Reset();
  else {
    if (hitTree != NULL) {
      TFile* f = hitTree->GetCurrentFile();
      f->Close();
      delete f;
      hitTree = NULL;
    }
  }
  tdcRLeaf = NULL;
  qdcRLeaf = NULL;
  tdcCLeaf = NULL;
  qdcCLeaf = NULL;
  elLeaf = NULL;
  detLeaf = NULL;
  eh_en = NULL;
  eh_rn = NULL;
  eh_tr = NULL;
  eh_nh = NULL;
  currentEntry = -1;
  valid = false;
  if (localDirectory) {
    while (!copyFile.empty()) {
      string command = "rm -f ";
      command = command + copyFile.back();
      cout << command.data() << endl;
      system(command.data());
      copyFile.pop_back();
    }
  }
}
void AHitTreeInput::newRun(run_parameter& r)
{
  anaLog << "AHitTreeInput::newRun()" << endli;
  clearTree();
  vector<string> filenames;
  for (size_t i = 0; i < r.getNumberOfFiles(); i++) {
    if (r.getFileType(i) == 257) {
      if (existing(r.getFile(i)))
        filenames.push_back(r.getFile(i));
    }
  }
  if (filenames.size() <= 0)
    return;
  if (localDirectory) {
    if (!existing(directory)) {
      cout << "local directory \"" << directory.data() << "\"doesn\'t exist: create it" << endl;
      vector<string> parentdirs;
      string tmps = directory;
      while (tmps.find("/") < tmps.npos && tmps.rfind("/") > 0) {
        parentdirs.push_back(tmps.substr(0, tmps.rfind("/")));
        tmps = tmps.substr(0, tmps.rfind("/"));
        if (tmps.length() < 1) {
          tmps = parentdirs.back();
          parentdirs.pop_back();
        }
      }
      for (int i = parentdirs.size() - 1; i >= 0; i--) {
        if (existing(parentdirs[i])) {
          cout << parentdirs[i].data() << " exists" << endl;
          continue;
        }
        cout << "mkdir " << parentdirs[i].data() << endl;
        system((string("mkdir ") + parentdirs[i]).data());
      }
      system((string("mkdir ") + directory).data());
    }
  }
  if (useChain) {
    if (localDirectory) {
      for (unsigned int i = 0; i < filenames.size(); i++) {
        string s = filenames[i];
        if (hasA(s, "/"))
          s = s.substr(s.rfind("/") + 1, s.length() - s.rfind("/") - 1);
        s = directory + s;
        if (s != filenames[i]) {
          copyFile.push_back(s);
          string command = "cp ";
          command = command + filenames[i] + " " + copyFile.back();
          cout << "bash # " << command.data() << endl;
          system(command.data());
          filenames[i] = copyFile.back();
          if (!existing(copyFile.back())) {
            cout << "error during copy file \"" << copyFile.back().data() << "\" doesn\'t exist"
                 << endl;
          }
        }
      }
    }
    hitChain->Reset();
    for (unsigned int i = 0; i < filenames.size(); i++)
      hitChain->Add(filenames[i].data());
    cout << "TChain defined for " << filenames.size() << " files " << hitChain->GetNtrees()
         << " trees found with " << hitChain->GetEntries() << " events" << endl;
    readWriteLog << "TChain defined for " << (int)filenames.size() << " files "
                 << (int)hitChain->GetNtrees() << " trees found with "
                 << (int)hitChain->GetEntries() << " events" << endli;
    hitChain->GetEntry(0);
    hitTree = hitChain->GetTree();
    if (hitTree == NULL) {
      cout << "no hit tree found" << endl;
      return;
    }
  } else {
    if (localDirectory) {
      string s = filenames.back();
      if (hasA(s, "/"))
        s = s.substr(s.rfind("/") + 1, s.length() - s.rfind("/") - 1);
      s = directory + s;
      if (s != filenames.back()) {
        copyFile.push_back(s);
        string command = "cp ";
        command = command + filenames.back() + " " + copyFile.back();
        cout << "bash # " << command.data() << endl;
        system(command.data());
        filenames.back() = copyFile.back();
        if (!existing(copyFile.back())) {
          cout << "error during copy file \"" << copyFile.back().data() << "\" doesn\'t exist"
               << endl;
        }
      }
    }
    TFile* f = new TFile(filenames.back().data(), "read");
    hitTree = (TTree*)f->Get("hits");
    if (hitTree == NULL) {
      delete f;
      cout << "no hit tree found" << endl;
      return;
    }
  }
  getHeader();
  getMain();
  readWriteLog << "Open file \"" << filenames.back().data() << "\" for hit tree input with"
               << (int)hitTree->GetEntries() << " events" << endli;
  if (tdcRLeaf == NULL || qdcRLeaf == NULL || tdcCLeaf == NULL || qdcCLeaf == NULL ||
      elLeaf == NULL || detLeaf == NULL || eh_en == NULL || eh_rn == NULL || eh_tr == NULL ||
      eh_nh == NULL)
    return;
  currentEntry = 0;
  valid = true;
}

void AHitTreeInput::getHeader()
{
  eh_en = hitTree->FindLeaf("eventNumber");
  eh_rn = hitTree->FindLeaf("runNumber");
  eh_tr = hitTree->FindLeaf("trigger");
  eh_nh = hitTree->FindLeaf("numberOfHits");
}
void AHitTreeInput::getMain()
{
  tdcRLeaf = hitTree->FindLeaf("hit.tdc_r");
  qdcRLeaf = hitTree->FindLeaf("hit.qdc_r");
  tdcCLeaf = hitTree->FindLeaf("hit.tdc_c");
  qdcCLeaf = hitTree->FindLeaf("hit.qdc_c");
  elLeaf = hitTree->FindLeaf("hit.el");
  detLeaf = hitTree->FindLeaf("hit.det");
}
