#include "AHitTreeOutput.h"
#include "algorithmparameter.h"
#include "logger.h"
#include <TSystem.h>
extern logger readWriteLog;
AHitTreeOutput::AHitTreeOutput(TCalibHit*** hitIn, int** nHits, int& evtNr, int& rnNr, int& trg,
                               int maxD, const algorithm_parameter& param)
    : AAlgorithm("Write hit tree"), EventNumber(evtNr), RunNumber(rnNr), Trigger(trg), maxDet(maxD)
{
  numberOfHits = nHits;
  hits = hitIn;
  hitTree = NULL;
  outFile = param.value(1).value<string>() + "H.root";
  localDirectory = false;
  localDirectory = param.value(0).value<bool>();
  directory = param.value(2).value<string>();
  copyFile = "";
  initialize(outFile);
}
AHitTreeOutput::~AHitTreeOutput() { finalize(); }
void AHitTreeOutput::process()
{
  Event = EventNumber;
  Run = RunNumber;
  Trig = Trigger;
  nHit = 0;
  for (int i = 0; i < maxDet; i++) {
    for (int j = 0; j < (*(numberOfHits[i])); j++) {
      if (nHit >= MAX_HITS)
        continue;
      if (!hits[i][j]->isValid())
        continue;
      if (hits[i][j]->getHitShape() == NULL)
        continue;
      outHits.det[nHit] = hits[i][j]->getDetector();
      outHits.el[nHit] = hits[i][j]->getElement();
      outHits.qdc[nHit] = hits[i][j]->getRawADC();
      outHits.tdc[nHit] = hits[i][j]->getRawTDC();
      outHits.qdcc[nHit] = hits[i][j]->getADC();
      outHits.tdcc[nHit] = hits[i][j]->getTDC();
      nHit++;
    }
  }
  if (nHit > 0)
    hitTree->Fill();
}
algorithm_parameter AHitTreeOutput::getDescription()
{
  algorithm_parameter ret("Writes Hits to Tree", algorithm_parameter::Category::OUTPUT, 0);
  string des = "This algorithm writes calibrated hits to a root tree. "
               "It only writes calibrated valid hits.";
  ret.setDescription(des);
  ret.addValue(string("use local directory"), true);
  ret.addValue("file name", "");
  ret.addValue(string("local directory"), string(""));
  return ret;
}
void AHitTreeOutput::initialize(const string& filename)
{
  string fn = filename;
  if (localDirectory) {
    copyFile = filename;
    if (hasA(copyFile, "/"))
      copyFile =
          copyFile.substr(copyFile.rfind("/") + 1, copyFile.length() - copyFile.rfind("/") - 1);
    copyFile = directory + copyFile;
    if (copyFile == filename) {
      copyFile = "";
    } else {
      cout << "temporary write to \"" << copyFile.data() << "\"" << endl;
      fn = copyFile;
    }
  }
  readWriteLog << "Open file \"" << fn.data() << "\" for hit tree output" << endli;
  TFile* f = new TFile(fn.data(), "recreate");
  f->cd();
  hitTree = new TTree("hits", "hits");
  hitTree->Branch("eventNumber", &Event, "eventNumber/I");
  hitTree->Branch("runNumber", &Run, "runNumber/I");
  hitTree->Branch("trigger", &Trig, "trigger/I");
  hitTree->Branch("numberOfHits", &nHit, "numberOfHits/I");
  hitTree->Branch("hit.det", outHits.det, "hit.det[numberOfHits]/S");
  hitTree->Branch("hit.el", outHits.el, "hit.el[numberOfHits]/S");
  hitTree->Branch("hit.qdc_c", outHits.qdcc, "hit.qdc_c[numberOfHits]/F");
  hitTree->Branch("hit.tdc_c", outHits.tdcc, "hit.tdc_c[numberOfHits]/F");
  hitTree->Branch("hit.qdc_r", outHits.qdc, "hit.qdc_r[numberOfHits]/I");
  hitTree->Branch("hit.tdc_r", outHits.tdc, "hit.tdc_r[numberOfHits]/I");
}
void AHitTreeOutput::finalize()
{
  if (hitTree != NULL) {
    TFile* f = hitTree->GetCurrentFile();
    f->cd();
    hitTree->Write();
    readWriteLog << (int)hitTree->GetEntries();
    f->Close();
    readWriteLog << "Entries written to hit-tree-file \"" << f->GetName() << "\"" << endli;
    delete f;
    hitTree = NULL;
    if (localDirectory) {
      if (copyFile != "") {
        string command = "mv ";
        command = command + copyFile + " " + outFile;
        cout << "bash # " << command.data() << flush;
        gSystem->Exec(command.data());
        cout << "   ==> done" << endl;
      }
    }
  }
}
